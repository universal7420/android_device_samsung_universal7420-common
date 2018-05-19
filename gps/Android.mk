LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := android.hardware.gnss@1.0-impl.exynos7420
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SRC_FILES := \
    ThreadCreationWrapper.cpp \
    AGnss.cpp \
    AGnssRil.cpp \
    Gnss.cpp \
    GnssBatching.cpp \
    GnssDebug.cpp \
    GnssGeofencing.cpp \
    GnssMeasurement.cpp \
    GnssNavigationMessage.cpp \
    GnssNi.cpp \
    GnssXtra.cpp \
    GnssConfiguration.cpp \
    GnssUtils.cpp

LOCAL_SHARED_LIBRARIES := \
    liblog \
    libhidlbase \
    libhidltransport \
    libutils \
    android.hardware.gnss@1.0 \
    libhardware

LOCAL_CFLAGS += -Werror

include $(BUILD_SHARED_LIBRARY)
