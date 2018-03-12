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

# Radio
PRODUCT_PACKAGES += \
	libxml2 \
	libprotobuf-cpp-full \
	rild.zero \
	libreference-ril \
	libril \
	libsecril-client \
	libsecril-client-sap \
	android.hardware.radio@1.0 \
	android.hardware.radio.deprecated@1.0 \
	modemloader

# Dual-SIM Support
ifeq ($(BOARD_HAS_DUAL_SIM),true)
  include $(LOCAL_PATH)/configs/radio/dual/product.mk
else
  include $(LOCAL_PATH)/configs/radio/single/product.mk
endif
