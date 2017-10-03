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

# Override build-properties to avoid too long fingerprints
PRODUCT_BUILD_PROP_OVERRIDES += \
    BUILD_FINGERPRINT="Samsung/$(PRODUCT_NAME)/$(PRODUCT_NAME):8.0.0/OPR6.170623.012/$(BUILD_NUMBER):userdebug/test-keys" \
    PRIVATE_BUILD_DESC="$(PRODUCT_NAME)-userdebug 8.0.0 OPR6.170623.012 $(BUILD_NUMBER) test-keys"
