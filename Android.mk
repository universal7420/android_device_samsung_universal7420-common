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

LOCAL_PATH := $(call my-dir)

TARGET_DEVICE_IS_ZERO ?= false
TARGET_DEVICE_IS_NOBLE_ZEN ?= false
TARGET_DEVICE_IS_CDMA ?= false

ifeq ($(BOARD_VENDOR),samsung)

	ifneq ($(filter noblelte nobleltecan nobleltedd nobleltedv nobleltejv nobleltektt nobleltelgt noblelteskt nobleltespr nobleltetmo nobleltextc nobleltezt zenlte zenltecan zenltedd zenltedv zenltejv zenltektt zenltelgt zenlteskt zenltetmo zenltexx zenltezt,$(TARGET_DEVICE)),)
		TARGET_DEVICE_IS_NOBLE_ZEN = true
	endif

	ifneq ($(filter zeroflte zerofltecan zeroflteduo zeroflteskt zerofltespr zerofltetmo zerofltexx zerolte zeroltecan zerolteduo zerolteskt zeroltespr zeroltetmo zeroltexx,$(TARGET_DEVICE)),)
		TARGET_DEVICE_IS_ZERO = true
	endif

	ifneq ($(filter zerofltespr zeroltespr nobleltespr,$(TARGET_DEVICE)),)
		TARGET_DEVICE_IS_CDMA = true
	endif

	ifneq ($(filter $(TARGET_DEVICE_IS_ZERO) $(TARGET_DEVICE_IS_NOBLE_ZEN),true),)
		include $(call all-subdir-makefiles,$(LOCAL_PATH))
	endif

endif
