LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE           := fstab
LOCAL_MODULE_TAGS      := optional
LOCAL_SRC_FILES        := fstab.c log.c configuration.c
LOCAL_CFLAGS           += -Wall -Wextra
LOCAL_STATIC_LIBRARIES := libcutils libbase liblog
LOCAL_FORCE_STATIC_EXECUTABLE := true
include $(BUILD_EXECUTABLE)
