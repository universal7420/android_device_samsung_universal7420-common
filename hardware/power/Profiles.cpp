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

#define LOG_TAG "zero-power-profiles"
#define LOG_NDEBUG 0

#include <log/log.h>

#include "Power.h"
#include "Profiles.h"

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {

const SecPowerProfile* Profiles::getProfileData(SecPowerProfiles profile) {
	switch_uint32_t (profile)
	{
		case_uint32_t (SecPowerProfiles::SCREEN_OFF):
		{
			ALOGV("%s: returning kPowerProfileScreenOff", __func__);
			return &kPowerProfileScreenOff;
		}
		case_uint32_t (SecPowerProfiles::POWER_SAVE):
		{
			ALOGV("%s: returning kPowerProfilePowerSave", __func__);
			return &kPowerProfilePowerSave;
		}
		case_uint32_t (SecPowerProfiles::BIAS_POWER_SAVE):
		{
			ALOGV("%s: returning kPowerProfileBiasPowerSave", __func__);
			return &kPowerProfileBiasPowerSave;
		}
		case_uint32_t (SecPowerProfiles::BALANCED):
		{
			ALOGV("%s: returning kPowerProfileBalanced", __func__);
			return &kPowerProfileBalanced;
		}
		case_uint32_t (SecPowerProfiles::BIAS_PERFORMANCE):
		{
			ALOGV("%s: returning kPowerProfileBiasPerformance", __func__);
			return &kPowerProfileBiasPerformance;
		}
		case_uint32_t (SecPowerProfiles::HIGH_PERFORMANCE):
		{
			ALOGV("%s: returning kPowerProfileHighPerformance", __func__);
			return &kPowerProfileHighPerformance;
		}
	}
	return nullptr;
}

/***********
 * ROFILE_SCREEN_OFF
 */
const SecPowerProfile Profiles::kPowerProfileScreenOff = {
	.cpu = {
		.apollo = {
			.governor = "nexus",
			.freq_min = 200000,
			.freq_max = 600000,
			.freq_hispeed = 400000,
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
			.freq_hispeed = 300000,
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
		.semiboost = true,
	},
	.kernel = {
		.pewq = true,
	},
};

/***********
 * PROFILE_POWER_SAVE
 */
const SecPowerProfile Profiles::kPowerProfilePowerSave = {
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
};

/***********
 * PROFILE_BIAS_POWER_SAVE
 */
const SecPowerProfile Profiles::kPowerProfileBiasPowerSave = {
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
};

/***********
 * PROFILE_BALANCED
 */
const SecPowerProfile Profiles::kPowerProfileBalanced = {
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
};

/***********
 * PROFILE_BIAS_PERFORMANCE
 */
const SecPowerProfile Profiles::kPowerProfileBiasPerformance = {
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
};

/***********
 * PROFILE_HIGH_PERFORMANCE
 */
const SecPowerProfile Profiles::kPowerProfileHighPerformance = {
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
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace hardware
}  // namespace android