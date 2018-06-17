#
# Copyright (C) 2018 TeamNexus
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

LOCAL_MODULE               := nfc_nci.universal7420
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_TAGS          := optional
LOCAL_PROPRIETARY_MODULE   := true

LOCAL_SRC_FILES := \
	hal.c

LOCAL_SHARED_LIBRARIES := \
	libbase \
	libcutils \
	libdl \
	libhardware \
	libhidlbase \
	libhidltransport \
	liblog \
	libutils \
	libzeroutils

LOCAL_CFLAGS := -Wall -Werror -Wno-unused-parameter -Wno-unused-function

LOCAL_CFLAGS += -DSTRICT_BEHAVIOUR

include $(BUILD_SHARED_LIBRARY)
