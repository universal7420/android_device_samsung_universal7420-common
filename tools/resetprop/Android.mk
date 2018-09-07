LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := resetprop.zero
LOCAL_VENDOR_MODULE := true

LOCAL_SHARED_LIBRARIES := \
	liblog

LOCAL_STATIC_LIBRARIES := \
	libprotobuf-c-nano \
	libzerosystemproperties

LOCAL_C_INCLUDES := \
	external/nanopb-c/ \
	$(LOCAL_PATH)/include/ \
	$(LOCAL_PATH)/libzerosystemproperties/include/

LOCAL_SRC_FILES := \
	resetprop.c \
	persist_properties.c \
	system_property_api.cpp \
	system_property_set.cpp \
	utils/file.c \
	utils/misc.c \
	utils/vector.c \
	utils/xwrap.c

include $(BUILD_EXECUTABLE)

########################
# Externals
########################
include $(LOCAL_PATH)/libzerosystemproperties/Android.mk
