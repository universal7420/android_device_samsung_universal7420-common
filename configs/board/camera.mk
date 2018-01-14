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

# Camera
BOARD_USE_SAMSUNG_CAMERAFORMAT_NV21 := true
TARGET_SPECIFIC_CAMERA_PARAMETER_LIBRARY := libcamera_parameters_zero
TARGET_KEEP_LEGACY_CAMERA_PACKAGE := true

# Back Sensor
BOARD_BACK_CAMERA_SENSOR := 10
BOARD_BACK_CAMERA_ROTATION := 0

# Front Sensor
BOARD_FRONT_CAMERA_SENSOR := 21
BOARD_FRONT_CAMERA_ROTATION := 0
