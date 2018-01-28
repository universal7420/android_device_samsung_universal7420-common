#
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

# enable debugging mode of the power-HAL
TARGET_POWER_DEBUGGING := false

# enable power-profiles for user controlled performance
#  - only available on LineageOS
TARGET_POWER_PROFILES := true

# enables extended power-profiles "Efficiency" and "Quick"
#  - only available on LineageOS
TARGET_POWER_EXTENDED_PROFILES := true

# boost device on interaction
TARGET_POWER_INTERACTION_BOOST := false

# boost device on system-request
#  - only available on LineageOS
TARGET_POWER_CPU_BOOST := false

# shutdown fingerprint-sensor on screen-off
TARGET_POWER_SHUTDOWN_FINGERPRINT := true

# enable support for doubletap2wake
TARGET_POWER_SUPPORT_DT2W := true
