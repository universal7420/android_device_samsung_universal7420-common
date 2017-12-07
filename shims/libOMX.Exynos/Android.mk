#
# Copyright (C) 2017 TeamNexus
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

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	libOMX.Exynos.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libion_exynos \
	liblog \
	libui

LOCAL_C_INCLUDES := \
	$(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include \
	frameworks/native/include/media/openmax/ \
	hardware/samsung_slsi-cm/exynos/include \
	hardware/samsung_slsi-cm/exynos/libvideocodec/include \
	hardware/samsung_slsi-cm/$(TARGET_BOARD_PLATFORM)/include \
	hardware/samsung_slsi-cm/$(TARGET_SOC)/include \
	hardware/samsung_slsi-cm/openmax/component/common/ \
	hardware/samsung_slsi-cm/openmax/include/ \
	hardware/samsung_slsi-cm/openmax/include/exynos/ \
	hardware/samsung_slsi-cm/openmax/osal/ \
	$(TOP)/frameworks/native/libs/nativewindow/include

LOCAL_ADDITIONAL_DEPENDENCIES += \
	$(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr

ifeq ($(BOARD_USE_ANB), true)
    LOCAL_CFLAGS += -DUSE_ANB

    ifneq ($(TARGET_SOC), exynos5410)
        LOCAL_CFLAGS += -DUSE_ANB_REF
    endif

    ifeq ($(BOARD_USE_ANB_OUTBUF_SHARE), true)
        LOCAL_CFLAGS += -DUSE_ANB_OUTBUF_SHARE
    endif
endif

ifeq ($(BOARD_USE_DMA_BUF), true)
    LOCAL_CFLAGS += -DUSE_DMA_BUF
endif

ifeq ($(BOARD_USE_METADATABUFFERTYPE), true)
    LOCAL_CFLAGS += -DUSE_METADATABUFFERTYPE

    ifeq ($(BOARD_USE_STOREMETADATA), true)
        LOCAL_CFLAGS += -DUSE_STOREMETADATA
    endif

    ifeq ($(BOARD_USE_ANDROIDOPAQUE), true)
        LOCAL_CFLAGS += -DUSE_ANDROIDOPAQUE
    endif
endif

ifeq ($(BOARD_USE_IMPROVED_BUFFER), true)
    LOCAL_CFLAGS += -DUSE_IMPROVED_BUFFER
endif

ifeq ($(BOARD_USE_CSC_HW), true)
    LOCAL_CFLAGS += -DUSE_CSC_HW
endif

ifeq ($(BOARD_USE_NON_CACHED_GRAPHICBUFFER), true)
    LOCAL_CFLAGS += -DUSE_NON_CACHED_GRAPHICBUFFER
endif

ifeq ($(BOARD_USE_NON_SECURE_DRM), true)
    LOCAL_CFLAGS += -DUSE_NON_SECURE_DRM
endif

ifeq ($(TARGET_BOARD_PLATFORM),exynos3)
    LOCAL_CFLAGS += -DUSE_MFC5X_ALIGNMENT
endif

ifeq ($(TARGET_BOARD_PLATFORM),exynos4)
    LOCAL_CFLAGS += -DUSE_MFC5X_ALIGNMENT
endif

ifeq ($(BOARD_USE_ANDROID), true)
LOCAL_C_INCLUDES += $(ANDROID_MEDIA_INC)/hardware
endif

ifeq ($(BOARD_USE_KHRONOS_OMX_HEADER), true)
    LOCAL_CFLAGS += -DUSE_KHRONOS_OMX_HEADER
    LOCAL_C_INCLUDES += $(EXYNOS_OMX_INC)/khronos
else
    ifeq ($(BOARD_USE_ANDROID), true)
        LOCAL_C_INCLUDES += $(ANDROID_MEDIA_INC)/openmax
    endif
endif

LOCAL_MODULE := libOMX.Exynos_shim
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_PROPRIETARY_MODULE := true

include $(BUILD_SHARED_LIBRARY)
