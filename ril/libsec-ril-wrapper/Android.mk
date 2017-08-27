#
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

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	libsec-ril-wrapper.cpp

LOCAL_SHARED_LIBRARIES := \
	liblog \
	libril

# Temporary hack for broken vendor RILs.
LOCAL_WHOLE_STATIC_LIBRARIES := \
	librilutils_static

LOCAL_CFLAGS := -DRIL_SHLIB

ifeq ($(SIM_COUNT), 2)
    LOCAL_CFLAGS += -DANDROID_MULTI_SIM
    LOCAL_CFLAGS += -DANDROID_SIM_COUNT_2
endif

LOCAL_MODULE := libsec-ril-wrapper
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES

include $(BUILD_SHARED_LIBRARY)
