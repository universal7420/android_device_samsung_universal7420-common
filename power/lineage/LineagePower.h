/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef VENDOR_LINEAGE_POWER_V1_0_POWER_H
#define VENDOR_LINEAGE_POWER_V1_0_POWER_H

#include <vendor/lineage/power/1.0/ILineagePower.h>
#include <vendor/lineage/power/1.0/types.h>

#include <hidl/Status.h>
#include <hidl/MQDescriptor.h>

namespace vendor {
namespace lineage {
namespace power {
namespace V1_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_string;
using ::android::sp;
using ::vendor::lineage::power::V1_0::ILineagePower;
using ::vendor::lineage::power::V1_0::LineageFeature;
using ::vendor::lineage::power::V1_0::LineageFeature;

struct LineagePower : public ILineagePower {
    Return<int32_t> getFeature(LineageFeature feature)  override;
};

extern "C" ILineagePower* HIDL_FETCH_ILineagePower(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_POWER_V1_0_POWER_H
