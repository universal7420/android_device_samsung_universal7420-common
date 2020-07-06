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

LOCAL_PATH := device/samsung/universal7420-common
TARGET_LD_SHIM_LIBS :=

TARGET_SLSI_VARIANT := bsp

#
# Architecture
#
TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-a
TARGET_CPU_ABI := arm64-v8a
TARGET_CPU_ABI2 :=
TARGET_CPU_VARIANT := generic
TARGET_CPU_VARIANT_RUNTIME := cortex-a57

TARGET_2ND_ARCH := arm
TARGET_2ND_ARCH_VARIANT := armv8-a
TARGET_2ND_CPU_ABI := armeabi-v7a
TARGET_2ND_CPU_ABI2 := armeabi
TARGET_2ND_CPU_VARIANT := generic
TARGET_2ND_CPU_VARIANT_RUNTIME := cortex-a53

TARGET_NR_CPUS := 8

#
# Audio
#
TARGET_AUDIOHAL_VARIANT := samsung
USE_XML_AUDIO_POLICY_CONF := 1
AUDIOSERVER_MULTILIB := 32

#
# AOSP
#
BUILD_BROKEN_DUP_RULES := true
BUILD_BROKEN_PHONY_TARGETS := true

#
# APEX
#
TARGET_FLATTEN_APEX := true
DEXPREOPT_GENERATE_APEX_IMAGE := false

#
# Binder
#
TARGET_USES_64_BIT_BINDER := true

#
# Bluetooth
#
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(LOCAL_PATH)/include/hardware
BOARD_CUSTOM_BT_CONFIG := $(LOCAL_PATH)/configs/bluetooth/libbt_vndcfg.txt
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true

#
# Bootanimation
#
TARGET_BOOTANIMATION_PRELOAD := true
TARGET_BOOTANIMATION_TEXTURE_CACHE := true
TARGET_SCREEN_HEIGHT := 2560
TARGET_SCREEN_WIDTH := 1440

#
# Bootloader
#
TARGET_NO_BOOTLOADER := true

#
# Build
#
EXTENDED_FONT_FOOTPRINT := true
PRODUCT_SKIP_FINGERPRINT_FROM_FILE := true
TARGET_UNOFFICIAL_BUILD_ID := TeamNexus

TARGET_SPECIFIC_HEADER_PATH := \
	$(LOCAL_PATH)/include

#
# Camera
#
BOARD_USE_SAMSUNG_CAMERAFORMAT_NV21 := true
TARGET_KEEP_LEGACY_CAMERA_PACKAGE := true

## Back Sensor
BOARD_BACK_CAMERA_SENSOR := 10
BOARD_BACK_CAMERA_ROTATION := 0

## Front Sensor
BOARD_FRONT_CAMERA_SENSOR := 21
BOARD_FRONT_CAMERA_ROTATION := 0

## Shims
TARGET_LD_SHIM_LIBS += \
	/system/lib/libexynoscamera.so|/vendor/lib/libexynoscamera_shim.so \
	/system/lib64/libexynoscamera.so|/vendor/lib64/libexynoscamera_shim.so

#
# Charger
#
WITH_LINEAGE_CHARGER := false
BACKLIGHT_PATH := /sys/devices/13900000.dsim/backlight/panel/brightness
BOARD_BATTERY_DEVICE_NAME := battery
BOARD_CHARGER_ENABLE_SUSPEND := true
BOARD_CHARGING_MODE_BOOTING_LPM := /sys/class/power_supply/battery/batt_lp_charging
CHARGING_ENABLED_PATH := "/sys/class/power_supply/battery/batt_lp_charging"

#
# Debugging
#
TARGET_BUILD_DEBUGGABLE := true

#
# Device
#
BOARD_VENDOR := samsung
TARGET_BOOTLOADER_BOARD_NAME := universal7420
TARGET_BOARD_PLATFORM := exynos5
TARGET_SOC := exynos7420

#
# DEX pre-optimizations
#
WITH_DEXPREOPT_BOOT_IMG_AND_SYSTEM_SERVER_ONLY := false
WITH_DEXPREOPT := true

# Use SDFAT driver
TARGET_EXFAT_DRIVER := sdfat

#
# Fingerprint
#
TARGET_SEC_FP_CALL_NOTIFY_ON_CANCEL := true
TARGET_SEC_FP_CALL_CANCEL_ON_ENROLL_COMPLETION := true
TARGET_SEC_FP_USES_PERCENTAGE_SAMPLES := true

## Shims
TARGET_LD_SHIM_LIBS += \
	/system/lib64/libbauthserver.so|/vendor/lib64/libbauthtzcommon_shim.so
#
# GPS
#
## SDK target overrides
TARGET_PROCESS_SDK_VERSION_OVERRIDE += \
    /system/vendor/bin/gpsd=27 \
    /vendor/bin/gpsd=27

#
# Hardware Classes
#
## LineageOS HW
JAVA_SOURCE_OVERLAYS := \
    org.lineageos.hardware|hardware/samsung/lineagehw|**/*.java \
    org.lineageos.hardware|device/samsung/universal7420-common/lineagehw|**/*.java

#
# Init
#
TARGET_INIT_VENDOR_LIB := libinit_sec
TARGET_UNIFIED_DEVICE := true

#
# Kernel
#
TARGET_KERNEL_ARCH := arm64
TARGET_KERNEL_HEADER_ARCH := arm64
TARGET_KERNEL_SOURCE := kernel/samsung/exynos7420
TARGET_LINUX_KERNEL_VERSION := 3.10

## Bootimage
BOARD_CUSTOM_BOOTIMG := true
BOARD_CUSTOM_BOOTIMG_MK := hardware/samsung/mkbootimg.mk
BOARD_KERNEL_BASE := 0x10000000
BOARD_KERNEL_IMAGE_NAME := Image
BOARD_KERNEL_PAGESIZE := 2048
BOARD_KERNEL_SEPARATED_DT := true
BOARD_MKBOOTIMG_ARGS := --kernel_offset 0x10008000 --ramdisk_offset 0x11000000 --tags_offset 0x10000100
TARGET_CUSTOM_DTBTOOL := dtbhtoolExynos

#
# Manifest
#
DEVICE_MANIFEST_FILE += device/samsung/universal7420-common/configs/manifest.xml

#
# Networking
#
TARGET_NEEDS_NETD_DIRECT_CONNECT_RULE := true

#
# NFC
#
BOARD_NFC_HAL_SUFFIX := universal7420.unused

#
# OpenMAX
#
# Shim
TARGET_LD_SHIM_LIBS += \
    /system/bin/mediaserver|/vendor/lib/libstagefright_shim.so

#
# OTA
#
BLOCK_BASED_OTA := true

#
# Partitions
#
TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true
BOARD_FLASH_BLOCK_SIZE := 131072
BOARD_BOOTIMAGE_PARTITION_SIZE := 29360128
BOARD_CACHEIMAGE_PARTITION_SIZE := 209715200
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 37651584
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 3124019200
#BOARD_USERDATAIMAGE_PARTITION_SIZE := 59183980544  # 64GB
BOARD_HAS_LARGE_FILESYSTEM := true
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_ROOT_EXTRA_FOLDERS += efs
TARGET_FS_CONFIG_GEN := $(LOCAL_PATH)/configs/config.fs

#
# Radio
#
BOARD_PROVIDES_LIBRIL := true
ENABLE_VENDOR_RIL_SERVICE := true
SIM_COUNT := 2

#
# Recovery
#
BOARD_HAS_NO_SELECT_BUTTON := true
BOARD_HAS_LARGE_FILESYSTEM := true
BOARD_HAS_NO_MISC_PARTITION := true
BOARD_USE_CUSTOM_RECOVERY_FONT := \"roboto_23x41.h\"
TARGET_RECOVERY_FSTAB := $(LOCAL_PATH)/ramdisk/fstab.samsungexynos7420.recovery
BOARD_SUPPRESS_SECURE_ERASE := true
BOARD_HAS_DOWNLOAD_MODE := true

## Releasetools - for resizing system filesystem
TARGET_RELEASETOOLS_EXTENSIONS := device/samsung/universal7420-common/releasetools

#
# Renderscript
#
BOARD_OVERRIDE_RS_CPU_VARIANT_32 := cortex-a53.a57
BOARD_OVERRIDE_RS_CPU_VARIANT_64 := cortex-a57

#
# Security
#
BOARD_USES_TRUST_KEYMASTER := true

# SELinux
BOARD_SEPOLICY_DIRS += device/samsung/universal7420-common/sepolicy
BOARD_SEPOLICY_VERS := $(PLATFORM_SDK_VERSION).0
SELINUX_IGNORE_NEVERALLOWS := true

## Seccomp filters
BOARD_SECCOMP_POLICY += device/samsung/universal7420-common/seccomp

#
# Sensors
#
TARGET_NO_SENSOR_PERMISSION_CHECK := true

#
# TWRP
#
ifneq ($(strip $(wildcard $(TOP)/bootable/recovery/variables.h)),)
  -include device/samsung/universal7420-common/twrp.mk
endif

# Vendor Security Patch Level
VENDOR_SECURITY_PATCH := 2019-02-05

#
# WFD
#
BOARD_USES_WFD := true

#
# Wifi
#
BOARD_HAVE_SAMSUNG_WIFI          := true
BOARD_WLAN_DEVICE                := bcmdhd
WPA_SUPPLICANT_USE_HIDL          := true
WPA_SUPPLICANT_VERSION           := VER_0_8_X
WPA_SUPPLICANT_USE_HIDL          := true
BOARD_WPA_SUPPLICANT_DRIVER      := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_bcmdhd
BOARD_HOSTAPD_DRIVER             := NL80211
BOARD_HOSTAPD_PRIVATE_LIB        := lib_driver_cmd_bcmdhd
WIFI_DRIVER_FW_PATH_PARAM        := "/sys/module/dhd/parameters/firmware_path"
WIFI_DRIVER_FW_PATH_STA          := "/system/vendor/etc/wifi/bcmdhd_sta.bin"
WIFI_DRIVER_FW_PATH_AP           := "/system/vendor/etc/wifi/bcmdhd_apsta.bin"
WIFI_BAND                        := 802_11_ABG

# Graphics
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3
BOARD_USES_EXYNOS5_COMMON_GRALLOC := true

# VR Front buffer
#BOARD_USES_VR_FRONT_BUFFER := true

# Samsung OpenMAX Video
BOARD_USE_STOREMETADATA := true
BOARD_USE_METADATABUFFERTYPE := true
BOARD_USE_DMA_BUF := true
BOARD_USE_ANB_OUTBUF_SHARE := true
BOARD_USE_IMPROVED_BUFFER := true
BOARD_USE_NON_CACHED_GRAPHICBUFFER := true
BOARD_USE_GSC_RGB_ENCODER := true
BOARD_USE_CSC_HW := false
BOARD_USE_QOS_CTRL := false
BOARD_USE_S3D_SUPPORT := true
BOARD_USE_TIMESTAMP_REORDER_SUPPORT := false
BOARD_USE_DEINTERLACING_SUPPORT := false
BOARD_USE_VP8ENC_SUPPORT := true
BOARD_USE_HEVCDEC_SUPPORT := true
BOARD_USE_HEVCENC_SUPPORT := true
BOARD_USE_HEVC_HWIP := false
BOARD_USE_VP9DEC_SUPPORT := true
BOARD_USE_VP9ENC_SUPPORT := false
BOARD_USE_CUSTOM_COMPONENT_SUPPORT := true
BOARD_USE_VIDEO_EXT_FOR_WFD_HDCP := false
BOARD_USE_SINGLE_PLANE_IN_DRM := false

# HWComposer
BOARD_USES_VPP := true
#BOARD_USES_VPP_V2 := true // 8890 only
BOARD_HDMI_INCAPABLE := true

# Scalar
BOARD_USES_SCALER := true

# HWCServices - requires framework support
#BOARD_USES_HWC_SERVICES := true

# WiFiDisplay
#BOARD_USES_VIRTUAL_DISPLAY := true - depends on platform changes
BOARD_USES_VIRTUAL_DISPLAY_DECON_EXT_WB := false
BOARD_USE_VIDEO_EXT_FOR_WFD_DRM := false
BOARD_USES_VDS_BGRA8888 := true
BOARD_VIRTUAL_DISPLAY_DISABLE_IDMA_G0 := false

# LIBHWJPEG
TARGET_USES_UNIVERSAL_LIBHWJPEG := true

# FIMG2D
BOARD_USES_SKIA_FIMGAPI := true
BOARD_USES_FIMGAPI_V5X := true

# SCALER
BOARD_USES_DEFAULT_CSC_HW_SCALER := true
BOARD_USES_SCALER_M2M1SHOT := true

# inherit from the proprietary version
-include vendor/samsung/universal7420-common/BoardConfigVendor.mk