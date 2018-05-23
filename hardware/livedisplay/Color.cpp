/*
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

#define LOG_TAG "zero-livedisplay"
#define LOG_NDEBUG 0

#include <log/log.h>

#include "Color.h"
#include "../Utils.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V1_0 {
namespace implementation {

using ::android::Mutex;
using ::android::NO_INIT;
using ::android::OK;
using ::android::sp;
using ::android::status_t;

using ::vendor::nexus::zero::Utils;

Color::Color() {
}

Color::~Color() {
}

Return<Features> Color::getSupportedFeatures() {
    mFeatures = 0;

	/* Adaptive Backlight */
	if (Utils::canRead(ZERO_ADAPTIVE_BACKLIGHT_PATH) &&
		Utils::canWrite(ZERO_ADAPTIVE_BACKLIGHT_PATH))
	{
		mFeatures |= Feature::ADAPTIVE_BACKLIGHT;
	}

	return mFeatures;
}

bool Color::supported(Feature feature) {
	return (mFeatures & feature) == mFeatures;
}

Return<void> Color::getDisplayModes(getDisplayModes_cb _hidl_cb) {
	/* not supported */
	hidl_vec<DisplayMode> profiles;
    _hidl_cb(profiles);
    return Void();
}

Return<void> Color::getCurrentDisplayMode(getCurrentDisplayMode_cb _hidl_cb) {
	/* not supported */
    DisplayMode mode;
    _hidl_cb(mode);
    return Void();
}

Return<void> Color::getDefaultDisplayMode(getDefaultDisplayMode_cb _hidl_cb) {
	/* not supported */
    DisplayMode mode;
    _hidl_cb(mode);
    return Void();
}

Return<bool> Color::setDisplayMode(int32_t /* modeID */, bool /* makeDefault */) {
	/* not supported */
    return false;
}

Return<bool> Color::setAdaptiveBacklightEnabled(bool enabled) {
    Mutex::Autolock _l(mLock);

	if (supported(Feature::ADAPTIVE_BACKLIGHT)) {
		return Utils::write(ZERO_ADAPTIVE_BACKLIGHT_PATH, enabled);
	}

	return false;
}

Return<bool> Color::isAdaptiveBacklightEnabled() {
    Mutex::Autolock _l(mLock);

	if (supported(Feature::ADAPTIVE_BACKLIGHT)) {
		bool enabled = false;
		return Utils::read(ZERO_ADAPTIVE_BACKLIGHT_PATH, enabled) && enabled;
	}

	return false;
}

Return<bool> Color::setOutdoorModeEnabled(bool /* enabled */) {
	/* not supported */
    return false;
}

Return<bool> Color::isOutdoorModeEnabled() {
	/* not supported */
    return false;
}

Return<void> Color::getColorBalanceRange(getColorBalanceRange_cb _hidl_cb) {
	/* not supported */
    Range range;
    _hidl_cb(range);
    return Void();
}

Return<int32_t> Color::getColorBalance() {
	/* not supported */
    return 0;
}

Return<bool> Color::setColorBalance(int32_t /* value */) {
	/* not supported */
    return true;
}

Return<bool> Color::setPictureAdjustment(const HSIC& /* hsic */) {
	/* not supported */
    return true;
}

Return<void> Color::getPictureAdjustment(getPictureAdjustment_cb _hidl_cb) {
	/* not supported */
    HSIC hsic;
    _hidl_cb(hsic);
    return Void();
}

Return<void> Color::getDefaultPictureAdjustment(getDefaultPictureAdjustment_cb _hidl_cb) {
	/* not supported */
    HSIC hsic;
    _hidl_cb(hsic);
    return Void();
}

Return<void> Color::getHueRange(getHueRange_cb _hidl_cb) {
	/* not supported */
    Range range;
    _hidl_cb(range);
    return Void();
}

Return<void> Color::getSaturationRange(getSaturationRange_cb _hidl_cb) {
	/* not supported */
    FloatRange range;
    _hidl_cb(range);
    return Void();
}

Return<void> Color::getIntensityRange(getIntensityRange_cb _hidl_cb) {
	/* not supported */
    FloatRange range;
    _hidl_cb(range);
    return Void();
}

Return<void> Color::getContrastRange(getContrastRange_cb _hidl_cb) {
	/* not supported */
    FloatRange range;
    _hidl_cb(range);
    return Void();
}

Return<void> Color::getSaturationThresholdRange(getSaturationThresholdRange_cb _hidl_cb) {
	/* not supported */
    FloatRange range;
    _hidl_cb(range);
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
