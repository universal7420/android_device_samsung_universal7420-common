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

LOCAL_SRC_FILES := \
	config.cpp \
	power.cpp \
	utils.cpp

LOCAL_C_INCLUDES += system/core/base/include/

LOCAL_SHARED_LIBRARIES := \
	libbase \
	libcutils \
	libhardware \
	liblog

LOCAL_MODULE               := power.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_TAGS          := optional
LOCAL_CFLAGS               := -Wall -Werror -Wno-unused-parameter -Wno-unused-function
LOCAL_PROPRIETARY_MODULE   := true

ifneq (,$(wildcard lineage-sdk/))
  LOCAL_CFLAGS += -DPOWER_HAS_LINEAGE_HINTS
endif

ifeq ($(TARGET_POWER_DEBUGGING),true)
  LOCAL_CFLAGS += -DTARGET_POWER_DEBUGGING
endif

ifeq ($(TARGET_POWER_PROFILES),true)
  LOCAL_CFLAGS += -DTARGET_POWER_PROFILES
endif

ifeq ($(TARGET_POWER_EXTENDED_PROFILES),true)
  LOCAL_CFLAGS += -DTARGET_POWER_EXTENDED_PROFILES
endif

ifeq ($(TARGET_POWER_INTERACTION_BOOST),true)
  LOCAL_CFLAGS += -DTARGET_POWER_INTERACTION_BOOST
endif

ifeq ($(TARGET_POWER_CPU_BOOST),true)
  LOCAL_CFLAGS += -DTARGET_POWER_CPU_BOOST
endif

ifeq ($(TARGET_POWER_SHUTDOWN_FINGERPRINT),true)
  LOCAL_CFLAGS += -DTARGET_POWER_SHUTDOWN_FINGERPRINT
endif

ifeq ($(TARGET_POWER_SUPPORT_DT2W),true)
  LOCAL_CFLAGS += -DTARGET_POWER_SUPPORT_DT2W
endif

include $(BUILD_SHARED_LIBRARY)
