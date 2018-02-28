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

#ifndef ZERO_HARDWARE_POWER_V1_0_POWER_H
#define ZERO_HARDWARE_POWER_V1_0_POWER_H

#include <android/hardware/power/1.0/IPower.h>

#ifdef POWER_HAS_LINEAGE_HINTS
#include <vendor/lineage/power/1.0/ILineagePower.h>
#endif

#include <hidl/Status.h>
#include <hidl/MQDescriptor.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#include "../power.h"

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {

using ::android::hardware::power::V1_0::Feature;
using ::android::hardware::power::V1_0::IPower;
using ::android::hardware::power::V1_0::PowerHint;
using ::android::hardware::power::V1_0::PowerStatePlatformSleepState;
using ::android::hardware::power::V1_0::Status;
#ifdef POWER_HAS_LINEAGE_HINTS
using ::vendor::lineage::power::V1_0::ILineagePower;
using ::vendor::lineage::power::V1_0::LineageFeature;
#endif

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;
using ::android::status_t;

struct Power : public IPower,
               public ILineagePower {

	Power();
	~Power();

	status_t registerAsSystemService();

	// Methods from ::android::hardware::power::V1_0::IPower follow.
	Return<void> setInteractive(bool interactive)  override;
	Return<void> powerHint(PowerHint hint, int32_t data)  override;
	Return<void> setFeature(Feature feature, bool activate)  override;
	Return<void> getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb)  override;

	// Methods from ::vendor::lineage::power::V1_0::ILineagePower follow.
#ifdef POWER_HAS_LINEAGE_HINTS
	Return<int32_t> getFeature(LineageFeature feature)  override;
#endif

private:
	struct SecPowerModule* mModule;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace hardware
}  // namespace android

#endif  // ZERO_HARDWARE_POWER_V1_0_POWER_H
