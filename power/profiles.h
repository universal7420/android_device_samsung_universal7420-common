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

#ifndef EXYNOS5_POWER_HAL_PROFILES_INCLUDED
#define EXYNOS5_POWER_HAL_PROFILES_INCLUDED

using namespace std;

struct power_profile_cpucluster {

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

struct power_profile {

	struct {

		struct power_profile_cpucluster apollo;

		struct power_profile_cpucluster atlas;

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

} power_profiles_data[PROFILE_MAX_USABLE + 1] = {

	/***********
	 * PROFILE_SCREEN_OFF
	 */
	{
		.cpu = {
			.apollo = {
				.governor = "nexus",
				.freq_min = 200000,
				.freq_max = 400000,
				.freq_hispeed = 200000,
				.nexus = {
					.lpr_ratio = 150,
					.lpr_down_elevation = 1,
					.lpr_up_elevation = 1,
				},
			},
			.atlas = {
				.governor = "nexus",
				.freq_min = 200000,
				.freq_max = 400000,
				.freq_hispeed = 200000,
				.nexus = {
					.lpr_ratio = 150,
					.lpr_down_elevation = 1,
					.lpr_up_elevation = 1,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 100,
				.freq_max = 100,
			},
			.highspeed = {
				.freq = 100,
				.load = 100,
			},
		},
		.hmp = {
			.boost = false,
			.semiboost = false,
		},
		.kernel = {
			.pewq = true,
		},
	},

	/***********
	 * PROFILE_POWER_SAVE
	 */
	{
		.cpu = {
			.apollo = {
				.governor = "nexus",
				.freq_min = 200000,
				.freq_hispeed = 700000,
				.freq_max = 1500000,
				.nexus = {
					.lpr_ratio = 150,
					.lpr_down_elevation = 1,
					.lpr_up_elevation = 1,
				},
			},
			.atlas = {
				.governor = "nexus",
				.freq_min = 400000,
				.freq_max = 2100000,
				.freq_hispeed = 800000,
				.nexus = {
					.lpr_ratio = 150,
					.lpr_down_elevation = 1,
					.lpr_up_elevation = 1,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 160,
				.freq_max = 772,
			},
			.highspeed = {
				.freq = 772,
				.load = 95,
			},
		},
		.hmp = {
			.boost = false,
			.semiboost = false,
		},
		.kernel = {
			.pewq = true,
		},
	},

	/***********
	 * PROFILE_BALANCED
	 */
	{
		.cpu = {
			.apollo = {
				.governor = "interactive",
				.freq_min = 400000,
				.freq_max = 1500000,
				.freq_hispeed = 900000,
				.interactive = {
					.above_hispeed_delay = "19000",
					.go_hispeed_load = 85,
					.min_sample_time = 40000,
					.target_loads = "75",
					.timer_rate = 20000,
					.timer_slack = 20000,
				},
			},
			.atlas = {
				.governor = "nexus",
				.freq_min = 800000,
				.freq_max = 2100000,
				.freq_hispeed = 1200000,
				.nexus = {
					.lpr_ratio = 125,
					.lpr_down_elevation = 1,
					.lpr_up_elevation = 2,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 350,
				.freq_max = 772,
			},
			.highspeed = {
				.freq = 772,
				.load = 85,
			},
		},
		.hmp = {
			.boost = false,
			.semiboost = true,
		},
		.kernel = {
			.pewq = true,
		},
	},

	/***********
	 * PROFILE_HIGH_PERFORMANCE
	 */
	{
		.cpu = {
			.apollo = {
				.governor = "interactive",
				.freq_min = 800000,
				.freq_max = 1500000,
				.freq_hispeed = 1500000,
				.interactive = {
					.above_hispeed_delay = "19000",
					.go_hispeed_load = 85,
					.min_sample_time = 40000,
					.target_loads = "75",
					.timer_rate = 20000,
					.timer_slack = 20000,
				},
			},
			.atlas = {
				.governor = "interactive",
				.freq_min = 1000000,
				.freq_max = 2100000,
				.freq_hispeed = 2100000,
				.interactive = {
					.above_hispeed_delay = "59000 1300000:39000 1700000:19000",
					.go_hispeed_load = 89,
					.min_sample_time = 40000,
					.target_loads = "65 1500000:75",
					.timer_rate = 20000,
					.timer_slack = 20000,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 544,
				.freq_max = 772,
			},
			.highspeed = {
				.freq = 772,
				.load = 75,
			},
		},
		.hmp = {
			.boost = true,
			.semiboost = false,
		},
		.kernel = {
			.pewq = false,
		},
	},

	/***********
	 * PROFILE_BIAS_POWER_SAVE
	 */
	{
		.cpu = {
			.apollo = {
				.governor = "nexus",
				.freq_min = 300000,
				.freq_max = 1500000,
				.freq_hispeed = 800000,
				.nexus = {
					.lpr_ratio = 125,
					.lpr_down_elevation = 1,
					.lpr_up_elevation = 1,
				},
			},
			.atlas = {
				.governor = "nexus",
				.freq_min = 600000,
				.freq_max = 2100000,
				.freq_hispeed = 1000000,
				.nexus = {
					.lpr_ratio = 125,
					.lpr_down_elevation = 1,
					.lpr_up_elevation = 1,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 266,
				.freq_max = 772,
			},
			.highspeed = {
				.freq = 772,
				.load = 80,
			},
		},
		.hmp = {
			.boost = false,
			.semiboost = true,
		},
		.kernel = {
			.pewq = true,
		},
	},

	/***********
	 * PROFILE_BIAS_PERFORMANCE
	 */
	{
		.cpu = {
			.apollo = {
				.governor = "interactive",
				.freq_min = 600000,
				.freq_max = 1500000,
				.freq_hispeed = 1200000,
				.interactive = {
					.above_hispeed_delay = "19000",
					.go_hispeed_load = 85,
					.min_sample_time = 40000,
					.target_loads = "75",
					.timer_rate = 20000,
					.timer_slack = 20000,
				},
			},
			.atlas = {
				.governor = "interactive",
				.freq_min = 1000000,
				.freq_max = 2100000,
				.freq_hispeed = 1600000,
				.nexus = {
					.lpr_ratio = 100,
					.lpr_down_elevation = 1,
					.lpr_up_elevation = 2,
				},
			},
		},
		.gpu = {
			.dvfs = {
				.freq_min = 450,
				.freq_max = 772,
			},
			.highspeed = {
				.freq = 772,
				.load = 90,
			},
		},
		.hmp = {
			.boost = true,
			.semiboost = false,
		},
		.kernel = {
			.pewq = false,
		},
	},

};

#endif // EXYNOS5_POWER_HAL_PROFILES_INCLUDED
