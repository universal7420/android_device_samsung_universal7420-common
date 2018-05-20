/*
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

#ifndef ZERO_HARDWARE_POWER_V1_0_PROFILES_H
#define ZERO_HARDWARE_POWER_V1_0_PROFILES_H

#define PROFILES_PATH_USER    "/data/system/power_profiles.xml"
#define PROFILES_PATH_VENDOR  "/vendor/etc/power_profiles.xml"
#define PROFILES_PATH_SYSTEM  "/system/etc/power_profiles.xml"

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include "Power.h"

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {
	
using ::std::string;
using ::std::map;

struct SecPowerProfileCpuCluster {

	bool enabled;

	string governor;

	unsigned int freq_min;
	unsigned int freq_max;
	unsigned int freq_hispeed;

	struct {

		char name[64];

		char data[64];

	} governor_data[64];

};

struct SecPowerProfile {

	bool enabled;

	struct {

		bool enabled;

		struct SecPowerProfileCpuCluster apollo;

		struct SecPowerProfileCpuCluster atlas;

	} cpu;

	struct {

		bool enabled;

		bool boost;
		bool semiboost;

	} hmp;

	struct {

		bool enabled;

		struct {

			bool enabled;

			unsigned int freq_min;
			unsigned int freq_max;

		} dvfs;

		struct {

			bool enabled;

			unsigned int freq;
			unsigned int load;

		} highspeed;

	} gpu;

	struct {

		bool enabled;

		bool pewq;

	} kernel;

	struct {

		bool enabled;

		int control_temp;

	} ipa;

};

struct Profiles {

	static void loadProfiles();
	static const SecPowerProfile* getProfileData(SecPowerProfiles profile);

private:
	static void loadProfilesImpl(const char *path);
	static void loadProfileImpl(SecPowerProfile *profile, xmlXPathContext *ctx, const char *path);

	static SecPowerProfile kPowerProfileScreenOff;
	static SecPowerProfile kPowerProfilePowerSave;
	static SecPowerProfile kPowerProfileBiasPowerSave;
	static SecPowerProfile kPowerProfileBalanced;
	static SecPowerProfile kPowerProfileBiasPerformance;
	static SecPowerProfile kPowerProfileHighPerformance;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace hardware
}  // namespace android

#endif // ZERO_HARDWARE_POWER_V1_0_PROFILES_H
