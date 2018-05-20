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

		unsigned int timer_rate;

		unsigned int hispeed_delay;
		unsigned int hispeed_load;

		unsigned int lpr_ratio;
		unsigned int lpr_down_elevation;
		unsigned int lpr_up_elevation;

	} nexus;

	struct {

		string above_hispeed_delay;
		string target_loads;

		unsigned int go_hispeed_load;
		unsigned int min_sample_time;
		unsigned int timer_rate;
		unsigned int timer_slack;

		unsigned int single_cluster0_min_freq;
		unsigned int single_enter_load;
		unsigned int single_enter_time;
		unsigned int single_exit_load;
		unsigned int single_exit_time;

		#define INTERACTIVE_SINGLE_DEFAULT(load_coeff, time_coeff) \
			.single_cluster0_min_freq = 800000,                    \
			.single_enter_load = 95 - load_coeff,                  \
			.single_enter_time = 199000 - time_coeff,              \
			.single_exit_load = 60 - load_coeff,                   \
			.single_exit_time = 99000 - time_coeff

		unsigned int multi_cluster0_min_freq;
		unsigned int multi_enter_load;
		unsigned int multi_enter_time;
		unsigned int multi_exit_load;
		unsigned int multi_exit_time;

		#define INTERACTIVE_MULTI_DEFAULT(load_coeff, time_coeff) \
			.multi_cluster0_min_freq = 1200000,                   \
			.multi_enter_load = 360 - load_coeff,                 \
			.multi_enter_time = 79000 - time_coeff,               \
			.multi_exit_load = 240 - load_coeff,                  \
			.multi_exit_time = 299000 - time_coeff

	} interactive;

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
