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

#include "Power.h"

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {
	
using ::std::string;
using ::std::map;

struct SecPowerProfileCpuCluster {

	string governor;

	unsigned int freq_min;
	unsigned int freq_max;
	unsigned int freq_hispeed;

	struct {

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
		
	} interactive;

};

struct SecPowerProfile {

	struct {

		struct SecPowerProfileCpuCluster apollo;

		struct SecPowerProfileCpuCluster atlas;

	} cpu;

	struct {

		bool boost;
		bool semiboost;

	} hmp;

	struct {

		struct {

			unsigned int freq_min;
			unsigned int freq_max;

		} dvfs;

		struct {

			unsigned int freq;
			unsigned int load;

		} highspeed;

	} gpu;

	struct {

		bool pewq;

	} kernel;

};

struct Profiles {

	static const SecPowerProfile* getProfileData(SecPowerProfiles profile);

private:
	static const SecPowerProfile kPowerProfileScreenOff;
	static const SecPowerProfile kPowerProfilePowerSave;
	static const SecPowerProfile kPowerProfileBiasPowerSave;
	static const SecPowerProfile kPowerProfileBalanced;
	static const SecPowerProfile kPowerProfileBiasPerformance;
	static const SecPowerProfile kPowerProfileHighPerformance;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace hardware
}  // namespace android

#endif // ZERO_HARDWARE_POWER_V1_0_PROFILES_H
