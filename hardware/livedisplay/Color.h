/*
 * Copyright (C) 2016 The CyanogenMod Project
 * Copyright (C) 2017-2018 The LineageOS Project
 * Copyright (C) 2018 TeamNexus
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V1_0_COLOR_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V1_0_COLOR_H

#include <utils/Mutex.h>
#include <vendor/lineage/livedisplay/1.0/IColor.h>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;

using ::vendor::lineage::livedisplay::V1_0::IColor;

#define ZERO_ADAPTIVE_BACKLIGHT_PATH    "/sys/class/lcd/panel/adaptive_control"

class Color : public IColor {
public:
	Color();
	~Color();

	Return<Features> getSupportedFeatures() override;

	Return<void> getDisplayModes(getDisplayModes_cb _hidl_cb) override;
	Return<void> getCurrentDisplayMode(getCurrentDisplayMode_cb _hidl_cb) override;
	Return<void> getDefaultDisplayMode(getDefaultDisplayMode_cb _hidl_cb) override;
	Return<bool> setDisplayMode(int32_t modeID, bool makeDefault) override;

	Return<bool> setAdaptiveBacklightEnabled(bool enabled) override;
	Return<bool> isAdaptiveBacklightEnabled() override;

	Return<bool> setOutdoorModeEnabled(bool enabled) override;
	Return<bool> isOutdoorModeEnabled() override;

	Return<void> getColorBalanceRange(getColorBalanceRange_cb _hidl_cb) override;
	Return<int32_t> getColorBalance() override;
	Return<bool> setColorBalance(int32_t value) override;

	Return<bool> setPictureAdjustment(const HSIC& hsic) override;
	Return<void> getPictureAdjustment(getPictureAdjustment_cb _hidl_cb) override;
	Return<void> getDefaultPictureAdjustment(getDefaultPictureAdjustment_cb _hidl_cb) override;

	Return<void> getHueRange(getHueRange_cb _hidl_cb) override;
	Return<void> getSaturationRange(getSaturationRange_cb _hidl_cb) override;
	Return<void> getIntensityRange(getIntensityRange_cb _hidl_cb) override;
	Return<void> getContrastRange(getContrastRange_cb _hidl_cb) override;
	Return<void> getSaturationThresholdRange(getSaturationThresholdRange_cb _hidl_cb) override;

private:
	Features mFeatures;
	android::Mutex mLock;

	bool supported(Feature feature);
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V1_0_COLOR_H
