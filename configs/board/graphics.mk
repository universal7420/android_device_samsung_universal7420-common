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

# FIMG2D
BOARD_USES_SKIA_FIMGAPI := true

# Gralloc
TARGET_USES_GRALLOC1_ADAPTER := true

# Graphics
USE_OPENGL_RENDERER := true
BOARD_USES_VIRTUAL_DISPLAY := true

# (G)SCALER
BOARD_USES_SCALER := true
BOARD_USES_DT := true

# HDMI
BOARD_HDMI_INCAPABLE := true
BOARD_USES_GSC_VIDEO := true

# ION
TARGET_USES_ION := true

# OpenMAX video
BOARD_USE_DMA_BUF := true
BOARD_USE_METADATABUFFERTYPE := true

# Samsung LSI OpenMAX
# COMMON_GLOBAL_CFLAGS += -DUSE_NATIVE_SEC_NV12TILED
