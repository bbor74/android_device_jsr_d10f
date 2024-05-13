#ifndef _HAVE_CONFIGURATION_H
#define _HAVE_CONFIGURATION_H
void set_storage_props(int usbmsc_present);
void set_storages_config(const char *value);
int get_storages_config(char *value);

#include <cutils/log.h>

#define FALSE 0
#define TRUE 1

//#define STORAGE_CONFIG_PROP "persist.storages.configuration"
#define STORAGE_CONFIG_FILE "storages.configuration"
#define USBMSC_PRESENT_PROP "ro.usbmsc.present"
//#define PERSISTENT_PROPERTY_DIR  "/data/property"
#define STORAGE_CONFIG_DIR  "/data/system"

#define STORAGES_CONFIGURATION_CLASSIC   "0"
#define STORAGES_CONFIGURATION_INVERTED  "1"
#define STORAGES_CONFIGURATION_DATAMEDIA "2"
#define SERVICE_VOLD "vold"

#define STORAGE_XML_PATH "/data/system/storage.xml"
// #define STORAGE_XML_TMP_PATH "/data/system/storage.tmp"
#define STORAGE_XML_HEADER "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\n"
#define STORAGE_XML_VOLUMES_TOKEN "<volumes version=\""
#define STORAGE_XML_PRIMARY_PHYSICAL_UUID_TOKEN "primaryStorageUuid=\"primary_physical\" "

#define STORAGE_XML_CONFIG_CLASSIC_FMT   "<volumes version=\"%d\" primaryStorageUuid=\"primary_physical\" forceAdoptable=\"%s\">\n"
#define STORAGE_XML_CONFIG_DATAMEDIA_FMT "<volumes version=\"%d\" forceAdoptable=\"%s\">\n"



#endif // _HAVE_CONFIGURATION_H
