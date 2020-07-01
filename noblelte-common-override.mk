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

zero_build_fingerprint := "Samsung/$(PRODUCT_NAME)/$(PRODUCT_NAME):$(PLATFORM_VERSION)/$(BUILD_ID)/$(BUILD_NUMBER):$(TARGET_BUILD_VARIANT)/test-keys"
zero_build_flavor      := "$(PRODUCT_NAME)-$(TARGET_BUILD_VARIANT)"
zero_build_descr       := "$(zero_build_flavor) $(PLATFORM_VERSION) $(BUILD_ID) $(BUILD_NUMBER) test-keys"

__PRODUCT_TARGET_NAME := $(PRODUCT_NAME)

# Override build-properties to avoid too long fingerprints
PRODUCT_BUILD_PROP_OVERRIDES += \
	BUILD_DISPLAY_ID=$(zero_build_descr) \
    PRIVATE_BUILD_DESC=$(zero_build_descr) \
    BUILD_FINGERPRINT_FROM_FILE=$(zero_build_fingerprint) \
	TARGET_BUILD_FLAVOR="$(zero_build_flavor)"

# Set BUILD_FINGERPRINT variable to be picked up by both system and vendor build.prop
BUILD_FINGERPRINT := $(zero_build_fingerprint)

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

