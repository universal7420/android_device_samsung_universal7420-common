#
# Copyright (C) 2020 Team Sexynos
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

#
# TARGET_DEVICE_IS_ definitions
#
TARGET_DEVICE_IS_ZERO := false
TARGET_DEVICE_IS_NOBLE_ZEN := false
TARGET_DEVICE_IS_CDMA := false

# Dynamically set the definitions based on the target device
ifneq ($(filter noblelte nobleltecan nobleltedd nobleltedv nobleltejv nobleltektt nobleltelgt noblelteskt nobleltespr nobleltetmo nobleltextc nobleltezt zenlte zenltecan zenltedd zenltedv zenltejv zenltektt zenltelgt zenlteskt zenltetmo zenltexx zenltezt,$(TARGET_DEVICE)),)
  TARGET_DEVICE_IS_NOBLE_ZEN = true
endif
ifneq ($(filter zeroflte zerofltecan zeroflteduo zeroflteskt zerofltespr zerofltetmo zerofltexx zerolte zeroltecan zerolteduo zerolteskt zeroltespr zeroltetmo zeroltexx,$(TARGET_DEVICE)),)
  TARGET_DEVICE_IS_ZERO = true
endif
ifneq ($(filter zerofltespr zeroltespr nobleltespr,$(TARGET_DEVICE)),)
  TARGET_DEVICE_IS_CDMA = true
endif

# Check to make sure we are a supported device
ifeq ($(TARGET_DEVICE_IS_NOBLE_ZEN),false)
  ifeq ($(TARGET_DEVICE_IS_ZERO),false)
    $(error Target device is unset)
  endif
endif

#
# Overlays
#
## Variant-specific
ifeq ($(TARGET_DEVICE_IS_CDMA), true)
  DEVICE_PACKAGE_OVERLAYS += device/samsung/universal7420-common/overlay-cdma
else
  DEVICE_PACKAGE_OVERLAYS += device/samsung/universal7420-common/overlay-gsm
endif
ifeq ($(TARGET_DEVICE_IS_ZERO), true)
  DEVICE_PACKAGE_OVERLAYS += device/samsung/universal7420-common/overlay-zero
else ifeq ($(TARGET_DEVICE_IS_NOBLE_ZEN), true)
  DEVICE_PACKAGE_OVERLAYS += device/samsung/universal7420-common/overlay-noble-zen
endif
