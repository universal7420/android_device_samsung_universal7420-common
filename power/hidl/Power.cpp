/*
 * Copyright (C) 2016 The Android Open Source Project
 * Copyright (C) 2018 The LineageOS Project
 * Copyright (C) 2018 TeamNexus
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "android.hardware.power@1.0-impl.zero"

#include <log/log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#include "Power.h"

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {

Power::Power()
    : mModule(nullptr)
{
	mModule = new SecPowerModule();
	power_init(mModule);
}

status_t Power::registerAsSystemService() {
	status_t ret = 0;

	ret = IPower::registerAsService();
	LOG_ALWAYS_FATAL_IF(ret != 0, "Failed to register IPower HAL Interface as service, aborting...");
	ALOGI("Registered IPower HAL Interface!");

	ret = ILineagePower::registerAsService();
	LOG_ALWAYS_FATAL_IF(ret != 0, "Failed to register ILineagePower HAL Interface as service, aborting...");
	ALOGI("Registered ILineagePower HAL Interface!");

	return 0;
}

Power::~Power() {
	delete(mModule);
}

// Methods from ::android::hardware::power::V1_0::IPower follow.
Return<void> Power::setInteractive(bool interactive)  {
	power_set_interactive(mModule, interactive ? 1 : 0);
	return Void();
}

Return<void> Power::powerHint(PowerHint hint, int32_t data)  {
	power_hint(mModule, static_cast<power_hint_t>(hint), data);
	return Void();
}

Return<void> Power::setFeature(Feature feature, bool activate)  {
	power_set_feature(mModule, static_cast<feature_t>(feature), (activate ? 1 : 0));
	return Void();
}

Return<void> Power::getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb)  {
	hidl_vec<PowerStatePlatformSleepState> states;

	_hidl_cb(states, Status::SUCCESS);
	return Void();
}

// Methods from ::vendor::lineage::power::V1_0::ILineagePower follow.
#ifdef POWER_HAS_LINEAGE_HINTS
Return<int32_t> Power::getFeature(LineageFeature feature)  {
	return power_get_feature(mModule, static_cast<feature_t>(feature));
}
#endif


} // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace hardware
}  // namespace android
