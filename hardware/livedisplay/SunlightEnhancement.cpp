/*
 * Copyright (C) 2019 The LineageOS Project
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

#include <android-base/file.h>
#include <android-base/strings.h>

#include <fstream>

#include "SunlightEnhancement.h"

using android::base::ReadFileToString;
using android::base::Trim;
using android::base::WriteStringToFile;

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

static constexpr const char *kLuxPath = "/sys/class/mdnie/mdnie/lux";
static constexpr const char *kMaxLux = "40000";

// Methods from ::vendor::lineage::livedisplay::V2_0::ISunlightEnhancement follow.
bool SunlightEnhancement::isSupported() {
    std::fstream lux(kLuxPath, lux.in | lux.out);

    return lux.good();
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IAdaptiveBacklight follow.
Return<bool> SunlightEnhancement::isEnabled() {
    std::string tmp;
    int32_t status = 0;

    if (ReadFileToString(kLuxPath, &tmp)) {
        status = std::stoi(Trim(tmp));
    }

    return status != 0;
}

Return<bool> SunlightEnhancement::setEnabled(bool enabled) {
    return WriteStringToFile(enabled ? "1" : "0", kLuxPath, true);
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
