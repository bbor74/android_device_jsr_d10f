LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := RemovePackages
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_TAGS := optional
LOCAL_OVERRIDES_PACKAGES := AppDirectedSMSService arcore AndroidAutoStubPrebuilt
LOCAL_OVERRIDES_PACKAGES += ConnMO ConnMetrics
LOCAL_OVERRIDES_PACKAGES += DCMO DevicePolicyPrebuilt DevicePersonalizationPrebuiltPixel2020 DMService
LOCAL_OVERRIDES_PACKAGES += FilesPrebuilt
LOCAL_OVERRIDES_PACKAGES += GoogleTTS
LOCAL_OVERRIDES_PACKAGES += MaestroPrebuilt MyVerizonServices MarkupGoogle
LOCAL_OVERRIDES_PACKAGES += PixelLiveWallpaperPrebuilt
LOCAL_OVERRIDES_PACKAGES += RecorderPrebuilt
LOCAL_OVERRIDES_PACKAGES += SafetyHubPrebuilt ScribePrebuilt Showcase SoundAmplifierPrebuilt SprintDM SprintHM
LOCAL_OVERRIDES_PACKAGES += Tycho talkback TurboPrebuilt
LOCAL_OVERRIDES_PACKAGES += USCCDM
LOCAL_OVERRIDES_PACKAGES += Velvet VZWAPNLib
LOCAL_OVERRIDES_PACKAGES += WallpapersBReel2020 WellbeingPrebuilt
LOCAL_UNINSTALLABLE_MODULE := true
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_SRC_FILES := /dev/null
include $(BUILD_PREBUILT)
