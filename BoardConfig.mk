#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

DEVICE_PATH := device/jsr/d10f

# ANT+
BOARD_ANT_WIRELESS_DEVICE := "qualcomm-smd"

# Architecture
TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_VARIANT := cortex-a7

# Audio
BOARD_USES_ALSA_AUDIO := true
BOARD_USES_FLUENCE_INCALL := true
AUDIO_FEATURE_ENABLED_EXTERNAL_SPEAKER := true
AUDIO_FEATURE_ENABLED_MULTI_VOICE_SESSIONS := true
AUDIO_FEATURE_ENABLED_NEW_SAMPLE_RATE := true
AUDIO_FEATURE_ENABLED_USBAUDIO := true
BOARD_USES_GENERIC_AUDIO := true
TARGET_USES_QCOM_MM_AUDIO := true
USE_CUSTOM_AUDIO_POLICY := 1
USE_XML_AUDIO_POLICY_CONF := 1

# Binder API version
TARGET_USES_64_BIT_BINDER := true

# Bionic
MALLOC_SVELTE := true

# Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(DEVICE_PATH)/bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_QCOM := true
BLUETOOTH_HCI_USE_MCT := true

# Bootloader
TARGET_BOOTLOADER_BOARD_NAME := MSM8226
TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true

# Charger
BOARD_CHARGER_ENABLE_SUSPEND := true
BOARD_CHARGER_DISABLE_INIT_BLANK := true
WITH_LINEAGE_CHARGER := false

# Dexpreopt
WITH_DEXPREOPT := false

# Encryption
TARGET_HW_DISK_ENCRYPTION := true
TARGET_LEGACY_HW_DISK_ENCRYPTION := true

# Exclude serif fonts for saving system.img size.
EXCLUDE_SERIF_FONTS := true

# Filesystem
BOARD_BOOTIMAGE_PARTITION_SIZE     := 16777216
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 16777216
BOARD_SYSTEMIMAGE_PARTITION_SIZE   := 1048576000
# 3145678336-16384 == 3145661952 (reserve 16KiB for crypto footer)
BOARD_USERDATAIMAGE_PARTITION_SIZE := 3145661952
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE  := ext4
BOARD_CACHEIMAGE_PARTITION_SIZE    := 134217728
BOARD_PERSISTIMAGE_PARTITION_SIZE  := 33554432
BOARD_FLASH_BLOCK_SIZE             := 131072

# FM
BOARD_HAVE_QCOM_FM := true
TARGET_QCOM_NO_FM_FIRMWARE := true

# GPS
USE_DEVICE_SPECIFIC_GPS := true
TARGET_PROVIDES_GPS_LOC_API := true

# Graphics
MAX_EGL_CACHE_KEY_SIZE := 12*1024
MAX_EGL_CACHE_SIZE := 2048*1024

NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3

OVERRIDE_RS_DRIVER := libRSDriver_adreno.so
TARGET_SUPPORTS_WEARABLES := true

TARGET_ADDITIONAL_GRALLOC_10_USAGE_BITS := 0x02000000
TARGET_CONTINUOUS_SPLASH_ENABLED := true
TARGET_FORCE_HWC_FOR_VIRTUAL_DISPLAYS := true
TARGET_USES_ION := true
TARGET_USES_NEW_ION_API := true
USE_OPENGL_RENDERER := true

# Hardware tunables
BOARD_HARDWARE_CLASS := $(DEVICE_PATH)/lineagehw/

# HIDL
DEVICE_MANIFEST_FILE := $(DEVICE_PATH)/manifest.xml
DEVICE_MATRIX_FILE := $(DEVICE_PATH)/compatibility_matrix.xml

# Headers
TARGET_SPECIFIC_HEADER_PATH := $(DEVICE_PATH)/include

# Init
TARGET_INIT_VENDOR_LIB := libinit_d10f
TARGET_RECOVERY_DEVICE_MODULES := libinit_d10f

# Kernel
TARGET_KERNEL_SOURCE := kernel/jsr/msm8226
TARGET_KERNEL_CONFIG := jsr_d10f_defconfig
BOARD_KERNEL_CMDLINE := console=ttyHSL0,115200,n8 androidboot.console=ttyHSL0 androidboot.bootdevice=msm_sdcc.1 androidboot.hardware=d10f zcache.enabled=1 androidboot.selinux=permissive androidboot.android_dt_dir=/dtrd/
BOARD_KERNEL_IMAGE_NAME := zImage
BOARD_KERNEL_BASE := 0x00000000
BOARD_KERNEL_PAGESIZE := 2048
BOARD_KERNEL_SEPARATED_DT := true
BOARD_DTBTOOL_ARGS := --force-v2
KERNEL_HAS_FINIT_MODULE := false

# Keymaster
# TARGET_KEYMASTER_WAIT_FOR_QSEE := false
TARGET_KEYMASTER_SKIP_WAITING_FOR_QSEE := true
#TARGET_PROVIDES_KEYMASTER := true

# Camera
USE_DEVICE_SPECIFIC_CAMERA := true
TARGET_HAS_LEGACY_CAMERA_HAL1 := true
TARGET_NEEDS_PLATFORM_TEXT_RELOCATIONS := true

TARGET_NEEDS_LEGACY_CAMERA_HAL1_DYN_NATIVE_HANDLE := true

# Lights
TARGET_PROVIDES_LIBLIGHT := true

# Platform
TARGET_BOARD_PLATFORM := msm8226
TARGET_BOARD_PLATFORM_GPU := qcom-adreno305
QCOM_BOARD_PLATFORMS += msm8226

# Power
TARGET_HAS_LEGACY_POWER_STATS := true
TARGET_HAS_NO_WLAN_STATS := true
#TARGET_USES_INTERACTION_BOOST := true
#TARGET_POWERHAL_VARIANT := qcom

# QCOM hardware
BOARD_USES_QCOM_HARDWARE := true

# Radio
#TARGET_RIL_VARIANT := caf

# Recovery
TARGET_RECOVERY_FSTAB := $(DEVICE_PATH)/rootdir/fstab.full
TARGET_RECOVERY_DENSITY := hdpi
TARGET_RECOVERY_PIXEL_FORMAT := "RGBX_8888"
TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true
BOARD_NO_SECURE_DISCARD := true

# Uncomment next line to build TWRP
# RECOVERY_VARIANT := twrp
ifeq ($(RECOVERY_VARIANT), twrp)
	TWHAVE_SELINUX := true
	TARGET_RECOVERY_QCOM_RTC_FIX := true
#	TW_TARGET_USES_QCOM_BSP := true
	TW_BRIGHTNESS_PATH := /sys/class/leds/lcd-backlight/brightness
	TW_SECONDARY_BRIGHTNESS_PATH := /sys/class/leds/button-backlight/brightness
	TW_THEME := portrait_hdpi
	TW_NO_EXFAT_FUSE := true
	TW_INCLUDE_NTFS_3G := true
	TW_USE_TOOLBOX := true
	PRODUCT_EXTRA_RECOVERY_KEYS += \
		build/target/product/security/testkey

#		vendor/jsr/security/cm-12.1/releasekey \
#		vendor/jsr/security/cm-13.0/releasekey \
#		vendor/jsr/security/cm-14.1/releasekey \
#		vendor/jsr/security/lineage-15.1/releasekey \
#		vendor/jsr/security/MIUI7K/releasekey \
#		vendor/jsr/security/MIUI8K/releasekey \
#		vendor/jsr/security/MIUI8M/platform \
#		vendor/jsr/security/MIUI9M/platform \

endif

# SELinux
include device/qcom/sepolicy/sepolicy.mk
# include device/qcom/sepolicy-legacy/sepolicy.mk

BOARD_PLAT_PUBLIC_SEPOLICY_DIR += device/jsr/d10f/sepolicy/public
BOARD_PLAT_PRIVATE_SEPOLICY_DIR += device/jsr/d10f/sepolicy/private
BOARD_SEPOLICY_DIRS += device/jsr/d10f/sepolicy/vendor

# Shims    /system/vendor/lib/hw/camera.msm8226.so|libboringssl-compat.so
TARGET_LD_SHIM_LIBS := \
    /system/vendor/lib/libqomx_jpegenc.so|libboringssl-compat.so \
    /system/lib/libcrypto.so|libboringssl-compat.so \
    /system/vendor/bin/thermal-engine|libshims_thermal.so \
    /system/vendor/bin/mpdecision|libshims_atomic.so

# Snapdragon LLVM
TARGET_USE_SDCLANG := true

# Time services
BOARD_USES_QC_TIME_SERVICES := true

# Properties
TARGET_SYSTEM_PROP += device/jsr/d10f/system.prop

# Extended Filesystem Support
TARGET_EXFAT_DRIVER := sdfat

# Wifi
BOARD_HAS_QCOM_WLAN := true
BOARD_WLAN_DEVICE := qcwcn
BOARD_HOSTAPD_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_qcwcn
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_qcwcn
PRODUCT_VENDOR_MOVE_ENABLED := true
WIFI_DRIVER_FW_PATH_AP := "ap"
WIFI_DRIVER_FW_PATH_STA := "sta"
WPA_SUPPLICANT_VERSION := VER_0_8_X
TARGET_PROVIDES_WCNSS_QMI := true
TARGET_USES_QCOM_WCNSS_QMI := true
TARGET_DISABLE_WCNSS_CONFIG_COPY := true
# WIFI_DRIVER_MODULE_NAME := "wlan"
# WIFI_DRIVER_MODULE_PATH := "/system/vendor/lib/modules/wlan.ko"
# WIFI_DRIVER_MODULE_ARG := ""
# WIFI_DRIVER_OPERSTATE_PATH := "/sys/class/net/wlan0/operstate"

# inherit from the proprietary version
-include vendor/jsr/d10f/BoardConfigVendor.mk
