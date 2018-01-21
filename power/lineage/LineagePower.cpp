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

#define LOG_TAG "android.hardware.power@1.0-impl"

#include <log/log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#include "LineagePower.h"

// for common PROFILE_MAX_USABLE
#include "../power-defs.h"

namespace vendor {
namespace lineage {
namespace power {
namespace V1_0 {
namespace implementation {

Return<int32_t> LineagePower::getFeature(LineageFeature feature)  {
    switch (feature) {
		case LineageFeature::SUPPORTED_PROFILES:
			return PROFILE_MAX_USABLE;
	}
    return -1;
}

ILineagePower* HIDL_FETCH_ILineagePower(const char* /* name */) {
    return new LineagePower();
}

} // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace lineage
}  // namespace vendor
