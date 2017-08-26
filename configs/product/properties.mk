#
# Copyright (C) 2016 The CyanogenMod Project
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

# Common
PRODUCT_PROPERTY_OVERRIDES += \
    ro.chipname=exynos7420 \
    ro.nfc.sec_hal=true \
    ro.sf.lcd_density=640 \
	ro.sys.sdcardfs=1 \
    af.fast_track_multiplier=1 \
    audio_hal.force_voice_config=wide \
    wifi.interface=wlan0

# GPU
PRODUCT_PROPERTY_OVERRIDES += \
	debug.composition.type=dyn \
	ro.opengles.version=196610 \
	video.accelerate.hw=1 \
	debug.egl.profiler=1

# H/W Composer
ifneq ($(NEXUS_VERSION),)
  PRODUCT_PROPERTY_OVERRIDES += \
      debug.hwc.max_hw_overlays=1 \
      debug.sf.hw=1 \
      persist.sys.ui.hw=1 \
      debug.egl.hw=1 \
      debug.hwc.force_gpu=0
else
  PRODUCT_PROPERTY_OVERRIDES += \
      debug.hwc.max_hw_overlays=0 \
      debug.sf.hw=0 \
      persist.sys.ui.hw=0 \
      debug.egl.hw=0 \
      debug.hwc.force_gpu=1
endif

# OTA
PRODUCT_PROPERTY_OVERRIDES += \
	ro.nexus.otaname=zero
