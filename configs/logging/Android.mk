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

INSTALLED_ZERO_NOLOG_PROP_TARGET := $(TARGET_OUT_VENDOR)/nolog.prop
INSTALLED_ZERO_NOLOG_PROP_SOURCE := device/samsung/noblelte-common/configs/logging/nolog.txt

ALL_DEFAULT_INSTALLED_MODULES += $(INSTALLED_ZERO_NOLOG_PROP_TARGET)

# First, read the list
$(INSTALLED_ZERO_NOLOG_PROP_TARGET): $(INSTALLED_ZERO_NOLOG_PROP_SOURCE)
	@echo Generating zero-nolog properties: $@
	$(hide) echo "#" > $@; \
	        echo "# ZERO NOLOG PROPERTIES" >> $@; \
	        echo "# auto-generated - do not edit" >> $@; \
	        echo "#" >> $@;
	$(hide) cat $(INSTALLED_ZERO_NOLOG_PROP_SOURCE) | \
	        grep -E '^[^#]' | \
	        sort | \
	        awk '{ printf "log.tag.%s=S\n", $$1 }' >> $@
