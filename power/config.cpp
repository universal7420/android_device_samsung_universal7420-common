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

#include "log.h"

#include <android-base/properties.h>
#include <string>

#include "config.h"

using namespace android;

static bool power_is_init = false;

void power_config_set_init(bool is_init) {
	power_is_init = is_init;
}

bool power_has_profiles_enabled() {
#ifndef TARGET_POWER_PROFILES
	return false;
#else
	return base::GetProperty("sys.power.profiles", TARGET_POWER_PROFILES_DEFAULT) == "true";
#endif
}

bool power_has_extended_profiles_enabled() {
#ifndef TARGET_POWER_EXTENDED_PROFILES
	return false;
#else
	return base::GetProperty("sys.power.profiles.extended", TARGET_POWER_EXTENDED_PROFILES_DEFAULT) == "true";
#endif
}

bool power_has_interaction_boost() {
#ifndef TARGET_POWER_INTERACTION_BOOST
	return false;
#else
	return base::GetProperty("sys.power.boost.interaction", TARGET_POWER_INTERACTION_BOOST_DEFAULT) == "true";
#endif
}

bool power_has_cpu_boost() {
#ifndef TARGET_POWER_CPU_BOOST
	return false;
#else
	return base::GetProperty("sys.power.boost.cpu", TARGET_POWER_CPU_BOOST_DEFAULT) == "true";
#endif
}

bool power_should_shutdown_fingerprint() {
#ifndef TARGET_POWER_SHUTDOWN_FINGERPRINT
	return false;
#else
	return base::GetProperty("sys.power.fingerprint.shutdown", TARGET_POWER_SHUTDOWN_FINGERPRINT_DEFAULT) == "true";
#endif
}

bool power_supports_dt2w() {
#ifndef TARGET_POWER_SUPPORT_DT2W
	return false;
#else
	return true;
#endif
}

bool power_is_subprofile_enabled(string subprofile) {
	return power_is_init ||
		base::GetProperty("sys.power.subprofile." + subprofile, "true") == "true";
}
