#
# Copyright (C) 2016 The CyanogenMod Project
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

# Bootloader
TW_NO_REBOOT_BOOTLOADER := true
TW_HAS_DOWNLOAD_MODE := true

# TWRP
TW_DEVICE_VERSION := TeamNexus
TW_EXCLUDE_SUPERSU := true
# TW_INCLUDE_CRYPTO := true
TW_THEME := portrait_hdpi

PRODUCT_COPY_FILES += \
    device/samsung/zero-common/ramdisk/fstab.samsungexynos7420.twrp:recovery/root/etc/twrp.fstab

# UI
RECOVERY_GRAPHICS_USE_LINELENGTH := true
TARGET_RECOVERY_PIXEL_FORMAT := "ABGR_8888"

# Screen
TW_BRIGHTNESS_PATH := "/sys/devices/13900000.dsim/backlight/panel/brightness"
TW_MAX_BRIGHTNESS := 255
TW_SCREEN_BLANK_ON_BOOT := true

# Storage
BOARD_HAS_NO_REAL_SDCARD := true
RECOVERY_SDCARD_ON_DATA := true

# System
TARGET_RECOVERY_QCOM_RTC_FIX := true
TW_MTP_DEVICE := /dev/mtp_usb
