#
# Copyright (C) 2017 The LineageOS Project
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

__PRODUCT_TARGET_NAME := $(PRODUCT_NAME)

# Set OTA-required properties in default.prop
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
	ro.product.name=$(PRODUCT_NAME) \
	ro.product.device=$(PRODUCT_DEVICE) \
	ro.product.brand=$(PRODUCT_BRAND) \
	ro.product.manufacturer=$(PRODUCT_MANUFACTURER) \
	ro.product.model=$(PRODUCT_MODEL) \
	ro.vendor.product.name=$(PRODUCT_NAME) \
	ro.vendor.product.device=$(PRODUCT_DEVICE) \
	ro.vendor.product.brand=$(PRODUCT_BRAND) \
	ro.vendor.product.manufacturer=$(PRODUCT_MANUFACTURER) \
	ro.vendor.product.model=$(PRODUCT_MODEL)

# UI performance tweaks
PRODUCT_PROPERTY_OVERRIDES += \
	debug.sf.latch_unsignaled=1
	debug.sf.disable_backpressure=1
	hwui.disable_vsync=true
	persist.sys.ui.hw=1
	debug.egl.hw=1
