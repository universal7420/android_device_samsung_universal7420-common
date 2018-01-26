/*
 * Copyright (C) 2013 The Android Open Source Project
 * Copyright (C) 2017 Jesse Chan <cjx123@outlook.com>
 * Copyright (C) 2017 Lukas Berger <mail@lukasberger.at>
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

#define LOG_TAG "power.exynos5"
#define LOG_NDEBUG 1

#include <atomic>
#include <cutils/properties.h>
#include <fcntl.h>
#include <fstream>
#include <future>
#include <hardware/hardware.h>
#include <hardware/power.h>
#include <iostream>
#include <linux/stat.h>
#include <math.h>
#include <pwd.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <utils/Log.h>

#include "power.h"
#include "profiles.h"

using namespace std;

struct sec_power_module {
	struct power_module base;
	pthread_mutex_t lock;
};

#define container_of(addr, struct_name, field_name) \
	((struct_name *)((char *)(addr) - offsetof(struct_name, field_name)))

static int current_power_profile = PROFILE_BALANCED;
static int requested_power_profile = PROFILE_BALANCED;

static int input_state_touchkeys = 1;
static string input_touchscreen_path = POWER_TOUCHSCREEN_ENABLED_FLAT;

/***********************************
 * Initializing
 */
static int power_open(const hw_module_t __unused * module, const char *name, hw_device_t **device) {
	int retval = 0; // 0 is ok; -1 is error

	ALOGD("%s: enter; name=%s", __func__, name);

	if (strcmp(name, POWER_HARDWARE_MODULE_ID) == 0) {
		power_module_t *dev = (power_module_t *)calloc(1, sizeof(power_module_t));

		if (dev) {
			// Common hw_device_t fields
			dev->common.tag = HARDWARE_DEVICE_TAG;
			dev->common.module_api_version = POWER_MODULE_API_VERSION_0_5;
			dev->common.hal_api_version = HARDWARE_HAL_API_VERSION;

			dev->init = power_init;
			dev->powerHint = power_hint;
			dev->getFeature = power_get_feature;
			dev->setFeature = power_set_feature;
			dev->setInteractive = power_set_interactive;

			*device = (hw_device_t *)dev;
		} else {
			retval = -ENOMEM;
		}
	} else {
		retval = -EINVAL;
	}

	ALOGD("%s: exit %d", __func__, retval);
	return retval;
}

static void power_init(struct power_module __unused * module) {
	if (!is_file(POWER_CONFIG_FP_ALWAYS_ON))
		pfwrite(POWER_CONFIG_FP_ALWAYS_ON, false);

	if (!is_file(POWER_CONFIG_FP_WAKELOCKS))
		pfwrite(POWER_CONFIG_FP_WAKELOCKS, false);

	if (!is_file(POWER_CONFIG_DT2W))
		pfwrite(POWER_CONFIG_DT2W, false);

	if (!is_file(POWER_CONFIG_BOOST))
		pfwrite(POWER_CONFIG_BOOST, true);

	if (!is_file(POWER_CONFIG_PROFILES))
		pfwrite(POWER_CONFIG_PROFILES, true);

	// get correct touchkeys/enabled-file
	// reads from input1/name:
	//   - flat will return sec_touchscreen
	//   - edge won't
	string touchscreen_input_name;
	pfread(POWER_TOUCHSCREEN_NAME, touchscreen_input_name);
	if (touchscreen_input_name != POWER_TOUCHSCREEN_NAME_EXPECT) {
		input_touchscreen_path = POWER_TOUCHSCREEN_ENABLED_EDGE;
	}

	// set to normal power profile
	power_set_profile(PROFILE_BALANCED);

	// initialize all input-devices
	power_input_device_state(1);

	// set the default settings
	if (!is_dir("/data/power"))
		mkdir("/data/power", 0771);
}

/***********************************
 * Hinting
 */
static void power_hint(struct power_module *module, power_hint_t hint, void *data) {
	struct sec_power_module *sec = container_of(module, struct sec_power_module, base);
	int value = (data ? *((intptr_t *)data) : 0);

	pthread_mutex_lock(&sec->lock);

	switch (hint) {

		/***********************************
		 * Profiles
		 */
		case POWER_HINT_LOW_POWER:
			ALOGI("%s: hint(POWER_HINT_LOW_POWER, %d, %llu)", __func__, value, (unsigned long long)data);
			power_set_profile(value ? PROFILE_POWER_SAVE : requested_power_profile);
			break;

		case POWER_HINT_SET_PROFILE:
			ALOGI("%s: hint(POWER_HINT_SET_PROFILE, %d, %llu)", __func__, value, (unsigned long long)data);
			requested_power_profile = value;
			power_set_profile(value);
			break;

		case POWER_HINT_SUSTAINED_PERFORMANCE:
		case POWER_HINT_VR_MODE:
			if (hint == POWER_HINT_SUSTAINED_PERFORMANCE)
				ALOGI("%s: hint(POWER_HINT_SUSTAINED_PERFORMANCE, %d, %llu)", __func__, value, (unsigned long long)data);
			else if (hint == POWER_HINT_VR_MODE)
				ALOGI("%s: hint(POWER_HINT_VR_MODE, %d, %llu)", __func__, value, (unsigned long long)data);

			power_set_profile(value ? PROFILE_HIGH_PERFORMANCE : requested_power_profile);
			break;

#ifdef POWER_HAS_NEXUSOS_HINTS
		case POWER_HINT_DREAMING_OR_DOZING:
			ALOGI("%s: hint(POWER_HINT_DREAMING_OR_DOZING, %d, %llu)", __func__, value, (unsigned long long)data);
			power_set_profile(value ? PROFILE_DREAMING_OR_DOZING : requested_power_profile);
			break;
#endif
			
		/***********************************
		 * Boosting
		 */
		case POWER_HINT_INTERACTION:
			// ALOGI("%s: hint(POWER_HINT_INTERACTION, %d, %llu)", __func__, value, (unsigned long long)data);

			/* power_boostpulse(value ? value : 50000);
			power_boostpulse(value ? value : 50000); */

			break;

        case POWER_HINT_CPU_BOOST:
			// ALOGI("%s: hint(POWER_HINT_CPU_BOOST, %d, %llu)", __func__, value, (unsigned long long)data);

			power_boostpulse(value);
			power_boostpulse(value);

			break;

		/***********************************
		 * Inputs
		 */
		case POWER_HINT_DISABLE_TOUCH:
			ALOGI("%s: hint(POWER_HINT_DISABLE_TOUCH, %d, %llu)", __func__, value, (unsigned long long)data);
			power_input_device_state(value ? 0 : 1);
			break;

		default: break;
	}

	pthread_mutex_unlock(&sec->lock);
}

/***********************************
 * Profiles
 */
static void power_set_profile(int profile) {
	int profiles = 1;

	pfread(POWER_CONFIG_PROFILES, &profiles);
	if (!profiles) {
		ALOGD("%s: profiles disabled (tried to apply profile %d)", __func__, profile);
		return;
	}

 	ALOGD("%s: apply profile %d", __func__, profile);

	// store it
	current_power_profile = profile;

	// apply settings
	struct power_profile data = power_profiles[current_power_profile + 2];

	/*********************
	 * CPU Cluster0
	 */
	pfwritegov(0, "freq_min",     data.cpu.cl0.freq_min); /* Core, File, Value */
	pfwritegov(0, "freq_max",     data.cpu.cl0.freq_max);
	pfwritegov(0, "hispeed_freq", data.cpu.cl0.freq_max);
	if (pfassertgov(0, "nexus")) {
		pfwritegov(0, "down_load",                   data.cpu.nexus.down_load);
		pfwritegov(0, "down_step",                   data.cpu.nexus.down_step);
		pfwritegov(0, "down_load_to_step_ratio",     data.cpu.nexus.down_lts_ratio);
		pfwritegov(0, "down_load_to_step_elevation", data.cpu.nexus.down_lts_elev);
		pfwritegov(0, "up_load",                     data.cpu.nexus.up_load);
		pfwritegov(0, "up_step",                     data.cpu.nexus.up_step);
		pfwritegov(0, "up_load_to_step_ratio",       data.cpu.nexus.up_lts_ratio);
		pfwritegov(0, "up_load_to_step_elevation",   data.cpu.nexus.up_lts_elev);
	}

	/*********************
	 * CPU Cluster1
	 */
	pfwritegov(4, "freq_min",     data.cpu.cl1.freq_min); /* Core, File, Value */
	pfwritegov(4, "freq_max",     data.cpu.cl1.freq_max);
	pfwritegov(4, "hispeed_freq", data.cpu.cl1.freq_max);
	if (pfassertgov(4, "nexus")) {
		pfwritegov(4, "down_load",                   data.cpu.nexus.down_load);
		pfwritegov(4, "down_step",                   data.cpu.nexus.down_step);
		pfwritegov(4, "down_load_to_step_ratio",     data.cpu.nexus.down_lts_ratio);
		pfwritegov(4, "down_load_to_step_elevation", data.cpu.nexus.down_lts_elev);
		pfwritegov(4, "up_load",                     data.cpu.nexus.up_load);
		pfwritegov(4, "up_step",                     data.cpu.nexus.up_step);
		pfwritegov(4, "up_load_to_step_ratio",       data.cpu.nexus.up_lts_ratio);
		pfwritegov(4, "up_load_to_step_elevation",   data.cpu.nexus.up_lts_elev);
	}

	/*********************
	 * HMP
	 */
	pfwrite("/sys/kernel/hmp/boost",                   data.hmp.boost);
	pfwrite("/sys/kernel/hmp/semiboost",               data.hmp.semiboost);
	pfwrite("/sys/kernel/hmp/sb_down_threshold",       data.hmp.sb_down_thres);
	pfwrite("/sys/kernel/hmp/sb_up_threshold",         data.hmp.sb_up_thres);
	pfwrite("/sys/kernel/hmp/active_down_migration",   data.hmp.active_down_migration);
	pfwrite("/sys/kernel/hmp/aggressive_up_migration", data.hmp.aggressive_up_migration);

	/*********************
	 * GPU
	 */
	pfwrite("/sys/devices/14ac0000.mali/dvfs_min_lock", data.gpu.min_lock);
	pfwrite("/sys/devices/14ac0000.mali/dvfs_max_lock", data.gpu.max_lock);

	/*********************
	 * Input
	 */
	pfwrite_legacy("/sys/class/input_booster/level", (data.input.booster ? 2 : 0));
	pfwrite_legacy("/sys/class/input_booster/head", data.input.booster_table);
	pfwrite_legacy("/sys/class/input_booster/tail", data.input.booster_table);
	
	/*********************
	 * Generic Settings
	 */
	pfwrite("/sys/power/enable_dm_hotplug", false);
	pfwrite("/sys/power/ipa/control_temp", data.ipa_control_temp);
	pfwrite("/sys/module/workqueue/parameters/power_efficient", data.power_efficient_workqueue);
}

/***********************************
 * Boost
 */
static void power_boostpulse(int duration) {
	int boost = 0;

	pfread(POWER_CONFIG_BOOST, &boost);
	if (!boost) {
		return;
	}

	// ALOGD("%s: duration     = %d", __func__, duration);

	if (duration > 0) {
		pfwritegov(0, "boostpulse_duration", duration);
		pfwritegov(1, "boostpulse_duration", duration);
	}

	pfwritegov(0, "boostpulse", true);
	pfwritegov(1, "boostpulse", true);
}

/***********************************
 * Inputs
 */
static void power_fingerprint_state(bool state) {
	int fp_always_on = 0, fp_wakelocks = 1;

	pfread(POWER_CONFIG_FP_ALWAYS_ON, &fp_always_on);
	if (fp_always_on) {
		return;
	}

	pfread(POWER_CONFIG_FP_WAKELOCKS, &fp_wakelocks);

	/*
	 * Ordered power toggling:
	 *    Turn on:   +Wakelocks  ->  +Regulator
	 *    Turn off:  -Regulator  ->  -Wakelocks
	 */
	if (state) {
		pfwrite(POWER_FINGERPRINT_WAKELOCKS, true);
		pfwrite(POWER_FINGERPRINT_REGULATOR, true);
	} else {
		pfwrite(POWER_FINGERPRINT_REGULATOR, false);

		if (!fp_wakelocks)
			pfwrite(POWER_FINGERPRINT_WAKELOCKS, false);
	}
}
 
static void power_input_device_state(int state) {
	int dt2w = 0, dt2w_sysfs = 0;

	pfread(POWER_CONFIG_DT2W, &dt2w);
	pfread(POWER_DT2W_ENABLED, &dt2w_sysfs);

#if LOG_NDEBUG
	ALOGD("%s: state         = %d", __func__, state);
	ALOGD("%s: dt2w          = %d", __func__, dt2w);
	ALOGD("%s: dt2w_sysfs    = %d", __func__, dt2w_sysfs);
#endif

	switch (state) {
		case INPUT_STATE_DISABLE:

			// save to current state to prevent enabling
			pfread(POWER_TOUCHKEYS_ENABLED, &input_state_touchkeys);

			pfwrite(input_touchscreen_path, false);
			pfwrite(POWER_TOUCHKEYS_ENABLED, false);
			pfwrite(POWER_TOUCHKEYS_BRIGTHNESS, 0);

			power_fingerprint_state(false);

			break;

		case INPUT_STATE_ENABLE:

			pfwrite(input_touchscreen_path, true);

			if (input_state_touchkeys) {
				pfwrite(POWER_TOUCHKEYS_ENABLED, true);
				pfwrite(POWER_TOUCHKEYS_BRIGTHNESS, 255);
			}

			power_fingerprint_state(true);

			break;
	}

	if (dt2w) {
		pfwrite_legacy(POWER_DT2W_ENABLED, true);
	} else {
		pfwrite_legacy(POWER_DT2W_ENABLED, false);
	}

	// give hw some milliseconds to properly boot up
	usleep(100 * 1000); // 100ms
}

static void power_set_interactive(struct power_module __unused * module, int on) {
	int screen_is_on = (on != 0);

#if LOG_NDEBUG
	ALOGD("%s: on = %d", __func__, on);
#endif

	if (!screen_is_on) {
		power_set_profile(PROFILE_SCREEN_OFF);
	} else {
		power_set_profile(requested_power_profile);
	}

	power_input_device_state(screen_is_on);
}

/***********************************
 * Features
 */
static int power_get_feature(struct power_module *module __unused, feature_t feature) {
	switch (feature) {
		case POWER_FEATURE_SUPPORTED_PROFILES:
			ALOGD("%s: request for POWER_FEATURE_SUPPORTED_PROFILES = %d", __func__, PROFILE_MAX_USABLE);
			return PROFILE_MAX_USABLE;
		case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
			ALOGD("%s: request for POWER_FEATURE_DOUBLE_TAP_TO_WAKE = 1", __func__);
			return 1;
		default:
			return -EINVAL;
	}
}

static void power_set_feature(struct power_module *module, feature_t feature, int state) {
	switch (feature) {
		case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
			ALOGD("%s: set POWER_FEATURE_DOUBLE_TAP_TO_WAKE to \"%d\"", __func__, state);
			if (state) {
				pfwrite(POWER_CONFIG_DT2W, true);
				pfwrite_legacy(POWER_DT2W_ENABLED, true);
			} else {
				pfwrite(POWER_CONFIG_DT2W, false);
				pfwrite_legacy(POWER_DT2W_ENABLED, false);
			}
			break;

		default:
			ALOGW("Error setting the feature %d and state %d, it doesn't exist\n",
				  feature, state);
		break;
	}
}

/***********************************
 * Utilities
 */
// C++ I/O
static bool pfwrite(string path, string str) {
	ofstream file;

	file.open(path);
	if (!file.is_open()) {
		ALOGE("%s: failed to open %s", __func__, path.c_str());
		return false;
	}

#if LOG_NDEBUG
	ALOGI("%s: store \"%s\" to %s", __func__, str.c_str(), path.c_str());
#endif

	file << str;
	file.close();

	return true;
}

static bool pfwrite(string path, bool flag) {
	return pfwrite(path, flag ? 1 : 0);
}

static bool pfwrite(string path, int value) {
	return pfwrite(path, to_string(value));
}

static bool pfwrite(string path, unsigned int value) {
	return pfwrite(path, to_string(value));
}

static bool pfassertgov(int core, string asserted_cpugov) {
	string cpugov;
	ostringstream path;
	ostringstream cpugov_path;

	path << "/sys/devices/system/cpu/cpu" << core << "/cpufreq";
	cpugov_path << path.str() << "/scaling_governor";

	pfread(cpugov_path.str(), cpugov);

	return (cpugov == asserted_cpugov);
}

static bool pfwritegov(int core, string file, string str) {
	string cpugov;
	ostringstream path;
	ostringstream cpugov_path;

	path << "/sys/devices/system/cpu/cpu" << core << "/cpufreq";
	cpugov_path << path.str() << "/scaling_governor";

	pfread(cpugov_path.str(), cpugov);

	path << "/" << cpugov << "/" << file;

	if (!is_file(path.str())) {
		return false;
	}

	return pfwrite(path.str(), str);
}

static bool pfwritegov(int core, string file, bool flag) {
	return pfwritegov(core, file, flag ? 1 : 0);
}

static bool pfwritegov(int core, string file, int value) {
	return pfwritegov(core, file, to_string(value));
}

static bool pfwritegov(int core, string file, unsigned int value) {
	return pfwritegov(core, file, to_string(value));
}

static bool pfread(string path, int *v) {
	ifstream file(path);
	string line;

	if (!file.is_open()) {
		ALOGE("%s: failed to open %s", __func__, path.c_str());
		return false;
	}

	if (!getline(file, line)) {
		ALOGE("%s: failed to read from %s", __func__, path.c_str());
		return false;
	}

	file.close();
	*v = stoi(line);

#if LOG_NDEBUG
	ALOGI("%s: read from %s", __func__, path.c_str());
#endif

	return true;
}

static bool pfread(string path, string &str) {
	ifstream file(path);

	if (!file.is_open()) {
		ALOGE("%s: failed to open %s", __func__, path.c_str());
		return false;
	}

	if (!getline(file, str)) {
		ALOGE("%s: failed to read from %s", __func__, path.c_str());
		return false;
	}

#if LOG_NDEBUG
	ALOGI("%s: read from %s", __func__, path.c_str());
#endif

	file.close();
	return true;
}

// legacy I/O
static bool pfwrite_legacy(string path, string str) {
	FILE *file = fopen(path.c_str(), "w");
	bool ret = true;

	if (file == NULL) {
		ALOGE("%s: failed to open %s", __func__, path.c_str());
		return false;
	}

	fprintf(file, "%s\n", str.c_str());

	if (ferror(file)) {
		ALOGE("%s: failed to write to %s", __func__, path.c_str());
		ret = false;
	}

	fclose(file);
	return ret;
}

static bool pfwrite_legacy(string path, int value) {
	return pfwrite_legacy(path, to_string(value));
}

static bool pfwrite_legacy(string path, bool flag) {
	return pfwrite_legacy(path, flag ? 1 : 0);
}

// existence-helpers
static bool is_dir(string path) {
	struct stat fstat;
	const char *cpath = path.c_str();

	return !stat(cpath, &fstat) &&
		(fstat.st_mode & S_IFDIR) == S_IFDIR;
}

static bool is_file(string path) {
	struct stat fstat;
	const char *cpath = path.c_str();

	return !stat(cpath, &fstat) &&
		(fstat.st_mode & S_IFREG) == S_IFREG;
}

static struct hw_module_methods_t power_module_methods = {
	.open = power_open,
};

struct sec_power_module HAL_MODULE_INFO_SYM = {
	.base = {
		.common = {
			.tag = HARDWARE_MODULE_TAG,
			.module_api_version = POWER_MODULE_API_VERSION_0_5,
			.hal_api_version = HARDWARE_HAL_API_VERSION,
			.id = POWER_HARDWARE_MODULE_ID,
			.name = "Power HAL for Exynos 7420 SoCs",
			.author = "Lukas Berger <mail@lukasberger.at>",
			.methods = &power_module_methods,
		},

		.init = power_init,
		.powerHint = power_hint,
		.getFeature = power_get_feature,
		.setFeature = power_set_feature,
		.setInteractive = power_set_interactive,
	},

	.lock = PTHREAD_MUTEX_INITIALIZER
};
