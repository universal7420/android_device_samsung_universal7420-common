
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

# Samsung LSI OpenMAX
BOARD_USE_VP8ENC_SUPPORT := true
BOARD_USE_HEVCDEC_SUPPORT := true
BOARD_USE_HEVC_HWIP := false
COMMON_GLOBAL_CFLAGS += -DUSE_NATIVE_SEC_NV12TILED

# Video scaling issue workaround
TARGET_OMX_LEGACY_RESCALING := true

zero_shims_omx := \
    /system/lib/omx/libOMX.Exynos.AVC.Decoder.so|/vendor/lib/SHIM_TARGET.so \
    /system/lib64/omx/libOMX.Exynos.AVC.Decoder.so|/vendor/lib64/SHIM_TARGET.so \
    /system/lib/omx/libOMX.Exynos.AVC.Encoder.so|/vendor/lib/SHIM_TARGET.so \
    /system/lib64/omx/libOMX.Exynos.AVC.Encoder.so|/vendor/lib64/SHIM_TARGET.so \
    /system/lib/omx/libOMX.Exynos.HEVC.Decoder.so|/vendor/lib/SHIM_TARGET.so \
    /system/lib64/omx/libOMX.Exynos.HEVC.Decoder.so|/vendor/lib64/SHIM_TARGET.so \
    /system/lib/omx/libOMX.Exynos.HEVC.Encoder.so|/vendor/lib/SHIM_TARGET.so \
    /system/lib64/omx/libOMX.Exynos.HEVC.Encoder.so|/vendor/lib64/SHIM_TARGET.so \
    /system/lib/omx/libOMX.Exynos.MPEG4.Decoder.so|/vendor/lib/SHIM_TARGET.so \
    /system/lib64/omx/libOMX.Exynos.MPEG4.Decoder.so|/vendor/lib64/SHIM_TARGET.so \
    /system/lib/omx/libOMX.Exynos.MPEG4.Encoder.so|/vendor/lib/SHIM_TARGET.so \
    /system/lib64/omx/libOMX.Exynos.MPEG4.Encoder.so|/vendor/lib64/SHIM_TARGET.so \
    /system/lib/omx/libOMX.Exynos.VP8.Decoder.so|/vendor/lib/SHIM_TARGET.so \
    /system/lib64/omx/libOMX.Exynos.VP8.Decoder.so|/vendor/lib64/SHIM_TARGET.so \
    /system/lib/omx/libOMX.Exynos.VP8.Encoder.so|/vendor/lib/SHIM_TARGET.so \
    /system/lib64/omx/libOMX.Exynos.VP8.Encoder.so|/vendor/lib64/SHIM_TARGET.so \
    /system/lib/omx/libOMX.Exynos.VP9.Decoder.so|/vendor/lib/SHIM_TARGET.so \
    /system/lib64/omx/libOMX.Exynos.VP9.Decoder.so|/vendor/lib64/SHIM_TARGET.so \
    /system/lib/omx/libOMX.Exynos.WMV.Decoder.so|/vendor/lib/SHIM_TARGET.so \
    /system/lib64/omx/libOMX.Exynos.WMV.Decoder.so|/vendor/lib64/SHIM_TARGET.so

# Shims: libstagefright
TARGET_LD_SHIM_LIBS += \
    $(subst SHIM_TARGET,libstagefright_shim,$(zero_shims_omx))

# Shims: libui
TARGET_LD_SHIM_LIBS += \
	$(subst SHIM_TARGET,libui_shim,$(zero_shims_omx))
