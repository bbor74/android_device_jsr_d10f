#include <stdlib.h>
#include <stdio.h>
//#include "log.h"
#include "configuration.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/statvfs.h>
#include <sys/mman.h>
#include <cutils/properties.h>
//#include <cutils/log.h>
#include <string.h>

#include <sys/stat.h>

void set_storages_config(const char *value)
{
    char tempPath[PATH_MAX];
    char path[PATH_MAX];
    int fd;

    snprintf(tempPath, sizeof(tempPath), "%s/.temp.XXXXXX", STORAGE_CONFIG_DIR);
    fd = mkstemp(tempPath);
    if (fd < 0) {
        ALOGE("Unable to write storages config to temp file %s: %s\n", tempPath, strerror(errno));
        return;
    }
    write(fd, value, strlen(value));
    fsync(fd);
    close(fd);

    snprintf(path, sizeof(path), "%s/%s", STORAGE_CONFIG_DIR, STORAGE_CONFIG_FILE);
    if (rename(tempPath, path)) {
        ALOGE("Unable to rename storages config file %s to %s: %s\n", tempPath, path, strerror(errno));
        unlink(tempPath);
    }
}

int get_storages_config(char *value)
{
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", STORAGE_CONFIG_DIR, STORAGE_CONFIG_FILE);
 /*    DIR* dir = opendir(STORAGE_CONFIG_DIR);
    if (!dir) {
        ERROR("Unable to open storages config directory \"%s\": %s\n",
              STORAGE_CONFIG_DIR, strerror(errno));
        return 0;
    } */
      //  INFO("%s: try open storages config file: %s \n", __func__, path);
        int fd = open(path, O_RDONLY);
        if (fd == -1) {
            ALOGE("Unable to open storages config file \"%s\": %s\n", path, strerror(errno));
            value[0] = 0;
            return 0;
        }
      //  INFO("%s: succecful open storages config file \n", __func__);

        int length = read(fd, value, sizeof(value) - 1);
        if (length >= 0) {
            value[length] = 0;
            ALOGI("%s: storages config: %s, %s)\n", __func__, path, value);
        } else {
            ALOGE("Unable to read storages config file %s: %s\n", path, strerror(errno));
            value[0] = 0;
            length = 0;
        }
    close(fd);
    return length;
}

char *mmap_xml_configuration(off_t *size) {
	FILE *xml_config_filestream = fopen(STORAGE_XML_PATH, "r+");
	if (xml_config_filestream == NULL) {
		ALOGE("Unable to mmap storage configuration XML \"%s\" - storages may be inconsistent (errno: %d (%s))!\n",
		      STORAGE_XML_PATH, errno, strerror(errno));
		return NULL;
	}

	fseeko(xml_config_filestream, 0l, SEEK_END);
	*size = ftello(xml_config_filestream)+strlen(STORAGE_XML_PRIMARY_PHYSICAL_UUID_TOKEN); // some space for expansion
	fseeko(xml_config_filestream, 0l, SEEK_SET);

	char *config = (char *) mmap(NULL, *size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fileno(xml_config_filestream), 0);
	if (config == MAP_FAILED) {
		ALOGE("Unable to mmap storage configuration XML \"%s\" - storages may be inconsistent (errno: %d (%s))!\n", STORAGE_XML_PATH, errno, strerror(errno));
		return NULL;
	}
	fclose(xml_config_filestream); // mmaped data still available
	return config;
}

static void print_xml(const char *xml_name)
{
  FILE *mf = fopen(xml_name, "r");
  if (!mf) {
    ALOGE("%d: mf not opened: %s", __LINE__,strerror(errno));
    return;
  }
  while (1)
   {
      char str[512];
      char *estr = fgets (str,sizeof(str),mf);
      if (estr == NULL)
      {
         if ( feof (mf) != 0)
         {
            ALOGI("INFO: End of xml\n");
            break;
         }
         else
         {
            ALOGE("ERROR: Error reading xml: %s\n", strerror(errno));
            break;
         }
      }
      ALOGI("%s\n", str);
   }
   fclose(mf);
}

void update_xml_configuration(char *xml_config, off_t config_size, int isDatamedia) {
	char *volumes=NULL, *version=NULL;
	version = strstr(xml_config, STORAGE_XML_HEADER);
	volumes = strstr(xml_config, STORAGE_XML_VOLUMES_TOKEN);
	if (version == NULL || volumes == NULL) {
		ALOGW("Storage config xml \"%s\" looks werid - erasing it\n", STORAGE_XML_PATH);
		unlink(STORAGE_XML_PATH);
		return;
	}

	int scanned_size=0, fields=0, iversion=0, buffer_size=0, printed_bytes, buffer_free_bytes=0;
	char primaryStorageUuid[64+1], forceAdoptable[64+1];
	char *data=NULL, *buffer=NULL, *rest_of_config=NULL;

	buffer_size=config_size + strlen(STORAGE_XML_PRIMARY_PHYSICAL_UUID_TOKEN); // needed if migrating from datamedia to classic
	buffer = data = (char *) calloc(1, buffer_size);
	if (buffer == NULL) {
		ALOGW("Out of memory while allocating %d bytes\n", buffer_size);
		return;
	}

	data += sprintf(data, STORAGE_XML_HEADER); // initialize config header

	if (isDatamedia) {
		// only datamedia needs emulated storage!
		fields = sscanf(volumes,
		                "<volumes version=\"%d\" forceAdoptable=\"%64[^\"]\">\n%n",
		                &iversion, forceAdoptable, &scanned_size);
		if (fields == 2) {
			free (buffer);
			return; // nothing to do here!
		}

		ALOGW("Storage config xml \"%s\" needs updating (looks like classic)!\n", STORAGE_XML_PATH);
		fields = sscanf(volumes,
		                "<volumes version=\"%d\" primaryStorageUuid=\"%64[^\"]\" forceAdoptable=\"%64[^\"]\">\n%n",
		                &iversion, primaryStorageUuid, forceAdoptable, &scanned_size);
		if (fields != 3) {
			ALOGW("Storage config xml \"%s\" is werid - got %d fields (want 3)!\n", STORAGE_XML_PATH, fields);
			free (buffer);
			return;
		}
		printed_bytes=sprintf(data, STORAGE_XML_CONFIG_DATAMEDIA_FMT, iversion, forceAdoptable);
		data += printed_bytes;
		buffer_free_bytes=buffer_size-strlen(STORAGE_XML_HEADER)-printed_bytes;
	} else {
		fields = sscanf(volumes,
		                "<volumes version=\"%d\" primaryStorageUuid=\"%64[^\"]\" forceAdoptable=\"%64[^\"]\">\n%n",
		                &iversion, primaryStorageUuid, forceAdoptable, &scanned_size);
		if (fields == 3) {
			free (buffer);
			return; // nothing to do here!
		}

		ALOGW("Storage config xml \"%s\" needs updating (looks like datamedia)!\n", STORAGE_XML_PATH);
		fields = sscanf(volumes,
		                "<volumes version=\"%d\" forceAdoptable=\"%64[^\"]\">\n%n",
		                &iversion, forceAdoptable, &scanned_size);
		if (fields != 2) {
			ALOGW("Storage config xml \"%s\" is werid - got %d fields (want 2)!\n", STORAGE_XML_PATH, fields);
			free (buffer);
			return; // nothing to do here!
		}
		buffer_free_bytes=buffer_size - strlen(STORAGE_XML_HEADER) - scanned_size
		                  - strlen (STORAGE_XML_PRIMARY_PHYSICAL_UUID_TOKEN) - 1;
		printed_bytes = sprintf(data, STORAGE_XML_CONFIG_CLASSIC_FMT, iversion, forceAdoptable);
		data += printed_bytes;
	}
	// just copy rest of config data
	rest_of_config=xml_config + strlen(STORAGE_XML_HEADER) + scanned_size;
	strncpy(data, rest_of_config, buffer_free_bytes);
//	INFO("Going to write '%s' into config %s\n", buffer, STORAGE_XML_PATH); // 1280-byte message too long for printk
	ALOGI("Updating %s\n", STORAGE_XML_PATH);
	munmap(xml_config, config_size);
	FILE *storage_config = fopen(STORAGE_XML_PATH, "w");
	if (storage_config == NULL) {
		ALOGE("Unable to open %s for writing (errno=%d: %s)\n", STORAGE_XML_PATH, errno, strerror(errno));
		free(buffer);
		return;
	}
	fwrite(buffer, strlen(buffer)-1, 1, storage_config);
	fclose(storage_config);
	free (buffer);
	print_xml(STORAGE_XML_PATH);
}

void set_storage_props(int usbmsc_present)
{
	char value[PROP_VALUE_MAX+1];
	int isDatamedia = FALSE;
	int rc = 0;

	if (usbmsc_present) {
		property_set(USBMSC_PRESENT_PROP, "true");
	} else {
		property_set(USBMSC_PRESENT_PROP, "false");
	}

//	rc = property_get(STORAGE_CONFIG_PROP, value, "");
	rc = get_storages_config(value);
	if (rc && !strcmp(value, STORAGES_CONFIGURATION_DATAMEDIA)) { // if datamedia
		ALOGI("Got datamedia storage configuration (" STORAGE_CONFIG_FILE " == %s)\n", value);
		isDatamedia = TRUE;
	} else if (rc && !strcmp(value, STORAGES_CONFIGURATION_INVERTED)) { // if swapped
		ALOGI("Got inverted storage configuration (" STORAGE_CONFIG_FILE " == %s)\n", value);
		property_set("ro.vold.primary_physical", "1");
	} else { 
		if (rc == 0) {  // If the storages configuration property is unspecified
			ALOGW("Storages configuration is undefined!\n");
				if (usbmsc_present) {
					strncpy(value, STORAGES_CONFIGURATION_CLASSIC, PROP_VALUE_MAX);
					property_set("ro.vold.primary_physical", "1");
					ALOGW("Trying classic storage configuration (" STORAGE_CONFIG_FILE " == %s)\n", value);
				} else {
					strncpy(value, STORAGES_CONFIGURATION_DATAMEDIA, PROP_VALUE_MAX);
					isDatamedia = TRUE;
					ALOGW("Trying datamedia storage configuration (" STORAGE_CONFIG_FILE " == %s)\n", value);
				}
			//property_set(STORAGE_CONFIG_PROP, value);
			set_storages_config(value);

		} else {// if classic
			ALOGI("Got classic storage configuration (" STORAGE_CONFIG_FILE " == %s)\n", value);
			property_set("ro.vold.primary_physical", "1");
		}
	}

        off_t size;
        char *xml_config=mmap_xml_configuration(&size);
	if (xml_config) {
		update_xml_configuration(xml_config, size, isDatamedia);
		munmap(xml_config, size);
	}

	ALOGI("Storages configuration applied\n");
}
