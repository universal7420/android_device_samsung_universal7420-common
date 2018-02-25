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

# Gralloc
TARGET_USES_GRALLOC1_ADAPTER := true

# Graphics
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3
USE_OPENGL_RENDERER := true

# ION
TARGET_USES_ION := true

#
# Samsung LSI Graphics
#

# Buffers
BOARD_USE_ANB_OUTBUF_SHARE := true
BOARD_USE_IMPROVED_BUFFER := true
BOARD_USE_NON_CACHED_GRAPHICBUFFER := true

# FIMG2D
BOARD_USES_SKIA_FIMGAPI := true

# gscaler
BOARD_USES_DT := true
BOARD_USES_SCALER := true

# HDMI
BOARD_HDMI_INCAPABLE := true

# Metadata
BOARD_USE_METADATABUFFERTYPE := true
BOARD_USE_STOREMETADATA := true

# Others
BOARD_USE_CSC_HW := false
BOARD_USE_DMA_BUF := true
BOARD_USE_GSC_RGB_ENCODER := true
BOARD_USE_QOS_CTRL := false
BOARD_USE_S3D_SUPPORT := false
BOARD_USES_GSC_VIDEO := true
BOARD_USES_VIRTUAL_DISPLAY := true
