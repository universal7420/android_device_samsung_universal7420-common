#
# Copyright (C) 2016 The CyanogenMod Project
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

# Audio HAL variant
TARGET_AUDIOHAL_VARIANT := samsung

# Camera
BOARD_USE_SAMSUNG_CAMERAFORMAT_NV21 := true

# Include an expanded selection of fonts
EXTENDED_FONT_FOOTPRINT := true

# Include path
TARGET_SPECIFIC_HEADER_PATH := $(LOCAL_PATH)/include

# Init
TARGET_INIT_VENDOR_LIB := libinit_sec
TARGET_UNIFIED_DEVICE := true

# Renderscript
BOARD_OVERRIDE_RS_CPU_VARIANT_32 := cortex-a53.a57
BOARD_OVERRIDE_RS_CPU_VARIANT_64 := cortex-a57

# TWRP
ifneq ($(strip $(wildcard $(TOP)/bootable/recovery/variables.h)),)
-include device/samsung/zero-common/twrp.mk
endif

# WFD
BOARD_USES_WFD := true
