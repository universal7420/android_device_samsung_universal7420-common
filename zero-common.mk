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

LOCAL_PATH := device/samsung/zero-common

$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

#
# AAPT
#
## This device is 640dpi.  However the platform doesn't
## currently contain all of the bitmaps at 640dpi density so
## we do this little trick to fall back to the xxhdpi version
## if the 640dpi doesn't exist.
PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := 640dpi

# A list of dpis to select prebuilt apk, in precedence order.
PRODUCT_AAPT_PREBUILT_DPI := xxxhdpi xxhdpi xhdpi hdpi

#
# Audio
#
## Configs
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/audio/audio_effects.xml:system/vendor/etc/audio_effects.xml \
	$(LOCAL_PATH)/configs/audio/audio_policy.conf:system/vendor/etc/audio_policy.conf \
	$(LOCAL_PATH)/configs/audio/mixer_paths_0.xml:$(TARGET_COPY_OUT_SYSTEM)/etc/mixer_paths_0.xml \
	frameworks/av/services/audiopolicy/config/audio_policy_volumes.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio_policy_volumes.xml \
	frameworks/av/services/audiopolicy/config/a2dp_audio_policy_configuration.xml:$(TARGET_COPY_OUT_VENDOR)/etc/a2dp_audio_policy_configuration.xml \
	frameworks/av/services/audiopolicy/config/default_volume_tables.xml:$(TARGET_COPY_OUT_VENDOR)/etc/default_volume_tables.xml \
	frameworks/av/services/audiopolicy/config/r_submix_audio_policy_configuration.xml:$(TARGET_COPY_OUT_VENDOR)/etc/r_submix_audio_policy_configuration.xml \
	frameworks/av/services/audiopolicy/config/usb_audio_policy_configuration.xml:$(TARGET_COPY_OUT_VENDOR)/etc/usb_audio_policy_configuration.xml \
	$(LOCAL_PATH)/configs/audio/audio_policy_configuration.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio_policy_configuration.xml

## Packages
PRODUCT_PACKAGES += \
	audio.a2dp.default \
	audio.usb.default \
	audio.r_submix.default \
	audio.primary.universal7420 \
	audio.primary.universal7420_32 \
	android.hardware.audio@5.0-impl \
	android.hardware.audio.effect@5.0-impl \
	libtinycompress

#
# APEX
#
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/ld.config.txt:$(TARGET_COPY_OUT_SYSTEM)/etc/swcodec/ld.config.txt

#
# Bluetooth
#
## Packages
PRODUCT_PACKAGES += \
	android.hardware.bluetooth@1.0-impl.zero \
	android.hardware.bluetooth@1.0-service \
	libbt-vendor

#
# Bootanimation
#
TARGET_SCREEN_HEIGHT := 2560
TARGET_SCREEN_WIDTH := 1440

#
# Camera
#
## Packages
PRODUCT_PACKAGES += \
	camera.exynos5 \
	libexynoscamera_shim \
	android.hardware.camera.provider@2.4-impl \
	android.hardware.camera.provider@2.4-service

## Apps
PRODUCT_PACKAGES += \
	Snap

#
# Configstore
#
PRODUCT_PACKAGES += \
	android.hardware.configstore@1.0-impl \
	android.hardware.configstore@1.0-service

#
# DRM
#
PRODUCT_PACKAGES += \
	android.hardware.drm@1.0-impl \
	android.hardware.drm@1.0-service

#
# Features
#
PRODUCT_COPY_FILES += \
	frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/vendor/etc/permissions/android.hardware.bluetooth_le.xml \
	frameworks/native/data/etc/android.hardware.camera.xml:system/vendor/etc/permissions/android.hardware.camera.xml \
	frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/vendor/etc/permissions/android.hardware.camera.flash-autofocus.xml \
	frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
	frameworks/native/data/etc/android.hardware.camera.full.xml:system/vendor/etc/permissions/android.hardware.camera.full.xml \
	frameworks/native/data/etc/android.hardware.camera.raw.xml:system/vendor/etc/permissions/android.hardware.camera.raw.xml \
	frameworks/native/data/etc/android.hardware.consumerir.xml:system/vendor/etc/permissions/android.hardware.consumerir.xml \
	frameworks/native/data/etc/android.hardware.fingerprint.xml:system/vendor/etc/permissions/android.hardware.fingerprint.xml \
	frameworks/native/data/etc/android.hardware.location.gps.xml:system/vendor/etc/permissions/android.hardware.location.gps.xml \
	frameworks/native/data/etc/android.hardware.nfc.hce.xml:system/vendor/etc/permissions/android.hardware.nfc.hce.xml \
	frameworks/native/data/etc/android.hardware.nfc.xml:system/vendor/etc/permissions/android.hardware.nfc.xml \
	frameworks/native/data/etc/android.hardware.opengles.aep.xml:system/vendor/etc/permissions/android.hardware.opengles.aep.xml \
	frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/vendor/etc/permissions/android.hardware.sensor.accelerometer.xml \
	frameworks/native/data/etc/android.hardware.sensor.barometer.xml:system/vendor/etc/permissions/android.hardware.sensor.barometer.xml \
	frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/vendor/etc/permissions/android.hardware.sensor.compass.xml \
	frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/vendor/etc/permissions/android.hardware.sensor.gyroscope.xml \
	frameworks/native/data/etc/android.hardware.sensor.light.xml:system/vendor/etc/permissions/android.hardware.sensor.light.xml \
	frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/vendor/etc/permissions/android.hardware.sensor.proximity.xml \
	frameworks/native/data/etc/android.hardware.sensor.stepcounter.xml:system/vendor/etc/permissions/android.hardware.sensor.stepcounter.xml \
	frameworks/native/data/etc/android.hardware.sensor.stepdetector.xml:system/vendor/etc/permissions/android.hardware.sensor.stepdetector.xml \
	frameworks/native/data/etc/android.hardware.sensor.heartrate.ecg.xml:system/vendor/etc/permissions/android.hardware.sensor.heartrate.ecg.xml \
	frameworks/native/data/etc/android.hardware.sensor.heartrate.xml:system/vendor/etc/permissions/android.hardware.sensor.heartrate.xml \
	frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/vendor/etc/permissions/android.hardware.telephony.gsm.xml \
	frameworks/native/data/etc/android.hardware.telephony.cdma.xml:system/vendor/etc/permissions/android.hardware.telephony.cdma.xml \
	frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/vendor/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
	frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/vendor/etc/permissions/android.hardware.usb.accessory.xml \
	frameworks/native/data/etc/android.hardware.usb.host.xml:system/vendor/etc/permissions/android.hardware.usb.host.xml \
	frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/vendor/etc/permissions/android.hardware.wifi.direct.xml \
	frameworks/native/data/etc/android.hardware.wifi.xml:system/vendor/etc/permissions/android.hardware.wifi.xml \
	frameworks/native/data/etc/android.hardware.vulkan.level-1.xml:system/vendor/etc/permissions/android.hardware.vulkan.level.xml \
	frameworks/native/data/etc/android.hardware.vulkan.version-1_0_3.xml:system/vendor/etc/permissions/android.hardware.vulkan.version.xml \
	frameworks/native/data/etc/android.software.freeform_window_management.xml:system/vendor/etc/permissions/android.software.freeform_window_management.xml \
	frameworks/native/data/etc/com.android.nfc_extras.xml:system/vendor/etc/permissions/com.android.nfc_extras.xml \
	frameworks/native/data/etc/com.nxp.mifare.xml:system/vendor/etc/permissions/com.nxp.mifare.xml \
	frameworks/native/data/etc/handheld_core_hardware.xml:system/vendor/etc/permissions/handheld_core_hardware.xml

# Fingerprint
PRODUCT_PACKAGES += \
    libbauthtzcommon_shim \
    android.hardware.biometrics.fingerprint@2.1-service.samsung

#
# GPS
#
## HIDL
PRODUCT_PACKAGES += \
    android.hardware.gnss@1.0 \
    android.hardware.gnss@1.0-impl.zero \
    android.hardware.gnss@1.0-service

## Configs
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/gps/gps.conf:system/etc/gps.conf \
	$(LOCAL_PATH)/configs/gps/gps.xml:system/etc/gps.xml \
	$(LOCAL_PATH)/configs/gps/gnss.conf:system/etc/gnss.conf

#
# Graphics
#
PRODUCT_PACKAGES += \
	libhwc2on1adapter \
	android.hardware.graphics.allocator@2.0-impl \
	android.hardware.graphics.allocator@2.0-service \
	android.hardware.graphics.composer@2.1-impl \
	android.hardware.graphics.mapper@2.0-impl

#
# Init
#
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/init/zero.wifi.rc:system/vendor/etc/init/zero.wifi.rc

#
# IR
#
PRODUCT_PACKAGES += \
	android.hardware.ir@1.0-impl \
	android.hardware.ir@1.0-service

#
# IRQs
#
PRODUCT_PACKAGES += \
	irqbalance

PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/hardware/irqbalance/irqbalance.conf:system/vendor/etc/irqbalance.conf

#
# ION
#
PRODUCT_PACKAGES += \
        libion

#
# Keylayouts
#
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/keylayout/sec_touchkey.kl:system/vendor/usr/keylayout/sec_touchkey.kl \
	$(LOCAL_PATH)/configs/keylayout/sec_touchscreen.kl:system/vendor/usr/keylayout/sec_touchscreen.kl \
	$(LOCAL_PATH)/configs/idc/Synaptics_HID_TouchPad.idc:system/vendor/usr/idc/Synaptics_HID_TouchPad.idc \
	$(LOCAL_PATH)/configs/idc/ft5x06_ts.idc:system/vendor/usr/idc/ft5x06_ts.idc \
	$(LOCAL_PATH)/configs/idc/sec_touchscreen.idc:system/vendor/usr/idc/sec_touchscreen.idc

#
# Keymaster
#
PRODUCT_PACKAGES += \
	keystore.exynos5 \
	android.hardware.keymaster@3.0-impl \
	android.hardware.keymaster@3.0-service

#
# Keymaster configuration
#
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.software.device_id_attestation.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.software.device_id_attestation.xml

#
# Lights
#
PRODUCT_PACKAGES += \
	android.hardware.light@2.0-service.samsung

#
# Media
#
## Profiles
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/media/media_codecs.xml:system/vendor/etc/media_codecs.xml \
	$(LOCAL_PATH)/configs/media/media_codecs_performance.xml:system/vendor/etc/media_codecs_performance.xml \
	$(LOCAL_PATH)/configs/media/media_profiles_V1_0.xml:system/vendor/etc/media_profiles_V1_0.xml \
	frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/vendor/etc/media_codecs_google_audio.xml \
	frameworks/av/media/libstagefright/data/media_codecs_google_telephony.xml:system/vendor/etc/media_codecs_google_telephony.xml \
	frameworks/av/media/libstagefright/data/media_codecs_google_video.xml:system/vendor/etc/media_codecs_google_video.xml

## Shims
PRODUCT_PACKAGES += \
	libstagefright_shim \
	libui_shim \
	libExynosOMX_shim

## Service-overrides
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/init/android.hardware.media.omx@1.0-service.rc:system/vendor/etc/init/android.hardware.media.omx@1.0-service.rc \
	$(LOCAL_PATH)/configs/init/mediaserver.rc:system/etc/init/mediaserver.rc

#
# Memtrack
#
PRODUCT_PACKAGES += \
	android.hardware.memtrack@1.0-impl

#
# Network
#
PRODUCT_PACKAGES += \
	netutils-wrapper-1.0

#
# NFC
#
## Configs
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/nfc/nfcee_access.xml:system/etc/nfcee_access.xml \
	$(LOCAL_PATH)/configs/nfc/libnfc-nci.conf:system/vendor/etc/libnfc-nci.conf \
	$(LOCAL_PATH)/configs/nfc/libnfc-sec.conf:system/vendor/etc/libnfc-sec.conf \
	$(LOCAL_PATH)/configs/nfc/libnfc-sec-hal.conf:system/vendor/etc/libnfc-sec-hal.conf

## Packages
PRODUCT_PACKAGES += \
	libnfc-nci \
	libnfc_nci_jni \
	libsecnativefeature \
	NfcNci \
	Tag \
	com.android.nfc_extras \
	android.hardware.nfc@1.0-impl \
	android.hardware.nfc@1.0-service

#
# Overlays
#
## Common
DEVICE_PACKAGE_OVERLAYS += device/samsung/zero-common/overlay

## Variant-specific
ifneq ($(filter zerofltespr zeroltespr nobleltespr,$(TARGET_DEVICE)),)
  DEVICE_PACKAGE_OVERLAYS += device/samsung/zero-common/overlay-cdma
else
  DEVICE_PACKAGE_OVERLAYS += device/samsung/zero-common/overlay-gsm
endif

## ROM-specific
ifneq ($(NEXUS_VERSION),)
  DEVICE_PACKAGE_OVERLAYS += device/samsung/zero-common/overlay-nexus
endif

#
# Packages
#
PRODUCT_PACKAGES += \
	HealthService \
	Jelly \
	SamsungServiceMode

#
# Permissions
#
PRODUCT_COPY_FILES += \
	device/samsung/zero-common/configs/permissions/com.samsung.permission.HRM_EXT.xml:system/vendor/etc/permissions/com.samsung.permission.HRM_EXT.xml

#
# Power
#
PRODUCT_PACKAGES += \
	android.hardware.power@1.0-service.zero

PRODUCT_COPY_FILES += \
	device/samsung/zero-common/hardware/power/profiles.xml:system/etc/power_profiles.xml

#
# Properties
#
TARGET_SYSTEM_PROP += device/samsung/zero-common/system.prop

#
# Radio
#
PRODUCT_PACKAGES += \
	libxml2 \
	libprotobuf-cpp-full \
	rild \
	libreference-ril \
	libril \
	libsecril-client \
	libsecril-client-sap \
	android.hardware.radio@1.0 \
	android.hardware.radio.deprecated@1.0 \
	modemloader

#
# Ramdisk
#
PRODUCT_PACKAGES += \
	fstab.samsungexynos7420 \
	init.baseband.rc \
	init.battery.rc \
	init.power.rc \
	init.rilchip.prop \
	init.rilchip.rc \
	init.rilchip.sh \
	init.samsungexynos7420.rc \
	init.samsungexynos7420.usb.rc \
        init.sensors.rc \
	init.recovery.samsungexynos7420.rc \
	ueventd.samsungexynos7420.rc

#
# RenderScript HAL
#
PRODUCT_PACKAGES += \
	android.hardware.renderscript@1.0-impl

#
# Security
#
## seccomp
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/seccomp/mediacodec.policy:system/vendor/etc/seccomp_policy/mediacodec.policy \
	$(LOCAL_PATH)/configs/seccomp/mediaextractor.policy:system/vendor/etc/seccomp_policy/mediaextractor.policy

#
# Sensors
#
PRODUCT_PACKAGES += \
	android.hardware.sensors@1.0-impl \
        android.hardware.sensors@1.0-service.samsung

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/sensors/lhd.conf:system/etc/lhd.conf

#
# Skeletons
#
PRODUCT_PACKAGES += \
	android.hidl.base@1.0

# Storage
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.isolated_storage=false

#
# Synapse
#
# include device/samsung/zero-common/synapse/synapse.mk

#
# Touch
#
PRODUCT_PACKAGES += \
	vendor.lineage.touch@1.0-service.samsung

#
# Tools
#

PRODUCT_HOST_PACKAGES += \
        dtbhtoolExynos

PRODUCT_PACKAGES += \
	e2fsck_static \
	make_ext4fs \
	resize2fs_static \
	setup_fs

#
# Trust
#
PRODUCT_PACKAGES += \
    vendor.lineage.trust@1.0-service

#
# USB
#
PRODUCT_PACKAGES += \
	android.hardware.usb@1.0-service.basic

#
# Vibrator
#
PRODUCT_PACKAGES += \
	android.hardware.vibrator@1.0-impl \
	android.hardware.vibrator@1.0-service


#
# Wifi
#
## Configs
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/configs/wifi/wpa_supplicant_overlay.conf:system/vendor/etc/wifi/wpa_supplicant_overlay.conf \
	$(LOCAL_PATH)/configs/wifi/p2p_supplicant_overlay.conf:system/vendor/etc/wifi/p2p_supplicant_overlay.conf \
	$(LOCAL_PATH)/configs/wifi/filter_ie:system/etc/wifi/filter_ie

## Packages
PRODUCT_PACKAGES += \
	android.hardware.wifi@1.0 \
	android.hardware.wifi@1.0-impl \
	android.hardware.wifi@1.0-service \
	libnetcmdiface \
	libwpa_client \
	hostapd \
	macloader \
	wificond \
	wifiloader \
	wifilogd \
	wlutil \
	wpa_supplicant \
	wpa_supplicant.conf

# call Samsung LSI board support package
# $(call inherit-product, hardware/samsung_slsi-cm/exynos5/exynos5.mk)
# $(call inherit-product, hardware/samsung_slsi-cm/exynos7420/exynos7420.mk)

# Get private non-open-source blobs
$(call inherit-product-if-exists, vendor/samsung/zero-private/zero-private-vendor.mk)

# Get non-open-source specific aspects
$(call inherit-product-if-exists, vendor/samsung/noblelte-common/noblelte-common-vendor.mk)
