/*
 * Copyright (C) 2018 Lukas Berger <mail@lukasberger.at>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>

#ifndef EXYNOS5_POWER_HAL_CONFIG_H_INCLUDED
#define EXYNOS5_POWER_HAL_CONFIG_H_INCLUDED

using namespace std;

#define TARGET_POWER_PROFILES_DEFAULT                "true"
#define TARGET_POWER_EXTENDED_PROFILES_DEFAULT       "true"
#define TARGET_POWER_INTERACTION_BOOST_DEFAULT       "false"
#define TARGET_POWER_CPU_BOOST_DEFAULT               "false"
#define TARGET_POWER_SHUTDOWN_FINGERPRINT_DEFAULT    "true"

void power_config_set_init(bool is_init);

bool power_has_profiles_enabled();
bool power_has_extended_profiles_enabled();
bool power_has_interaction_boost();
bool power_has_cpu_boost();
bool power_should_shutdown_fingerprint();
bool power_supports_dt2w();
bool power_is_subprofile_enabled(string subprofile);

#endif // EXYNOS5_POWER_HAL_CONFIG_H_INCLUDED
