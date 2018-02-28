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

LOCAL_PATH := device/samsung/zero-common/power

include $(CLEAR_VARS)

LOCAL_MODULE               := android.hardware.power@1.0-service.zero
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_MODULE_TAGS          := optional
LOCAL_PROPRIETARY_MODULE   := true

LOCAL_INIT_RC := hidl/android.hardware.power@1.0-service.zero.rc

# HIDL Implementation
LOCAL_SRC_FILES += \
	hidl/Power.cpp \
	hidl/service.cpp

# Legacy power-HAL
LOCAL_SRC_FILES += \
	power.cpp \
	utils.cpp

LOCAL_SHARED_LIBRARIES := \
	libbase \
	libcutils \
	libdl \
	libhardware \
	libhidlbase \
	libhidltransport \
	liblog \
	libutils \
	android.hardware.power@1.0 \
	vendor.lineage.power@1.0

LOCAL_CFLAGS := -Wall -Werror -Wno-unused-parameter -Wno-unused-function

ifneq (,$(wildcard hardware/lineage/interfaces/power/1.0/ vendor/cmsdk/))
  LOCAL_CFLAGS += -DPOWER_HAS_LINEAGE_HINTS
endif

ifneq (,$(wildcard hardware/nexus/interfaces/power/1.0/))
  LOCAL_CFLAGS += -DPOWER_HAS_NEXUS_HINTS
endif

# Enables mutex-protection against multithreading-problems
# but may cause deadlocks while booting. Recommended if 
# problems can be traced back to overlapping HAL-calls
# LOCAL_CFLAGS += -DLOCK_PROTECTION

# Enforces strict limitations of rules present in power-HAL.
# This may cause errors but ensures the stability of the
# power-HAL
LOCAL_CFLAGS += -DSTRICT_BEHAVIOUR

# Allow the power-HAL to invoke the pm-interfaces to
# shutdown/boot the fingerprint. This may result in a longer
# screen-on-delay and randomly non-working fingerprint-sensor
# but ensures a complete shutdown of the sensor. The delay is
# required to properly boot up the fingerprint before
# continuing to set the sensor up
# LOCAL_CFLAGS += -DFINGERPRINT_PM
# LOCAL_CFLAGS += -DFINGERPRINT_PM_DELAY=100

LOCAL_CFLAGS += -DNR_CPUS=$(TARGET_NR_CPUS)

include $(BUILD_EXECUTABLE)
