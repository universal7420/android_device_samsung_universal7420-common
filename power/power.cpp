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

#ifdef TARGET_POWER_DEBUGGING
  #define LOG_NDEBUG 1
#else
  #define LOG_NDEBUG 0
#endif

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

#define container_of(addr, struct_name, field_name) \
	((struct_name *)((char *)(addr) - offsetof(struct_name, field_name)))
	
#if LOG_NDEBUG
  #define ALOGDD  ALOGD
#else
  #define ALOGDD(...)  do { } while(0)
#endif

static power_module_t *shared_instance = nullptr;

/***********************************
 * Initializing
 */
static int power_open(const hw_module_t __unused * module, const char *name, hw_device_t **device) {
	int retval = 0; // 0 is ok; -1 is error

	ALOGDD("%s: enter; name=%s", __func__, name);

	if (strcmp(name, POWER_HARDWARE_MODULE_ID) == 0) {
		if (shared_instance) {
			ALOGDD("%s: using shared instance", __func__);
			*device = (hw_device_t *)shared_instance;
		} else {
			power_module_t *dev = (power_module_t *)calloc(1, sizeof(power_module_t));

			ALOGDD("%s: generating new instance", __func__);
			if (dev) {
				// Common hw_device_t fields
				dev->common.tag = HARDWARE_DEVICE_TAG;
				dev->common.module_api_version = POWER_MODULE_API_VERSION_0_5;
				dev->common.hal_api_version = HARDWARE_HAL_API_VERSION;

				dev->init = power_init;
				dev->powerHint = power_hint;
#ifdef POWER_HAS_LINEAGE_HINTS
				dev->getFeature = power_get_feature;
#endif
				dev->setFeature = power_set_feature;
				dev->setInteractive = power_set_interactive;

				*device = (hw_device_t *)dev;
				shared_instance = dev;
			} else {
				retval = -ENOMEM;
			}
		}
	} else {
		retval = -EINVAL;
	}

	ALOGDD("%s: exit; rc=%d", __func__, retval);
	return retval;
}

static void power_init(struct power_module __unused * module) {
	struct sec_power_module *sec = container_of(module, struct sec_power_module, base);

	ALOGDD("%s: enter;", __func__);
	pthread_mutex_lock(&sec->lock);

	if (sec->initialized) {
		ALOGDD("%s: exit; (already initialized)", __func__);
		return;
	}

	// get correct touchkeys/enabled-file
	// reads from input1/name:
	//   - flat will return sec_touchscreen
	//   - edge won't
	string touchscreen_input_name;
	pfread(POWER_TOUCHSCREEN_NAME, touchscreen_input_name);
	ALOGDD("%s: '%s' == '%s'", __func__, touchscreen_input_name.c_str(), POWER_TOUCHSCREEN_NAME_EXPECT);
	if (touchscreen_input_name == POWER_TOUCHSCREEN_NAME_EXPECT) {
		sec->input.touchscreen_control_path = POWER_TOUCHSCREEN_ENABLED_FLAT;
	} else {
		sec->input.touchscreen_control_path = POWER_TOUCHSCREEN_ENABLED_EDGE;
	}

	// set to normal power profile
	sec->profile.requested = PROFILE_BALANCED;
	power_set_profile(sec, PROFILE_BALANCED);

	// initialize all input-devices
	power_input_device_state(sec, INPUT_STATE_ENABLE);
	
	// disable dt2w by default
	power_dt2w_state(sec, false);
	
	// enable fingerprint by default
	power_fingerprint_state(true);

	sec->initialized = true;

	pthread_mutex_unlock(&sec->lock);
	ALOGDD("%s: exit;", __func__);
}

/***********************************
 * Hinting
 */
static void power_hint(struct power_module *module, power_hint_t hint, void *data) {
	struct sec_power_module *sec = container_of(module, struct sec_power_module, base);
	int value = (data ? *((intptr_t *)data) : 0);

	ALOGDD("%s: enter; hint=%d", __func__, hint);
	pthread_mutex_lock(&sec->lock);

	switch (hint) {

		/***********************************
		 * Profiles
		 */
#ifdef TARGET_POWER_PROFILES
		case POWER_HINT_LOW_POWER:
			ALOGI("%s: hint(POWER_HINT_LOW_POWER, %d, %llu)", __func__, value, (unsigned long long)data);
			power_set_profile(sec, value ? PROFILE_POWER_SAVE : sec->profile.requested);
			break;

#ifdef POWER_HAS_LINEAGE_HINTS
		case POWER_HINT_SET_PROFILE:
			ALOGI("%s: hint(POWER_HINT_SET_PROFILE, %d, %llu)", __func__, value, (unsigned long long)data);
			sec->profile.requested = value;
			power_set_profile(sec, value);
			break;
#endif

		case POWER_HINT_SUSTAINED_PERFORMANCE:
		case POWER_HINT_VR_MODE:
			if (hint == POWER_HINT_SUSTAINED_PERFORMANCE)
				ALOGI("%s: hint(POWER_HINT_SUSTAINED_PERFORMANCE, %d, %llu)", __func__, value, (unsigned long long)data);
			else if (hint == POWER_HINT_VR_MODE)
				ALOGI("%s: hint(POWER_HINT_VR_MODE, %d, %llu)", __func__, value, (unsigned long long)data);

			power_set_profile(sec, value ? PROFILE_HIGH_PERFORMANCE : sec->profile.requested);
			break;

#ifdef POWER_HAS_NEXUSOS_HINTS
		case POWER_HINT_DREAMING_OR_DOZING:
			ALOGI("%s: hint(POWER_HINT_DREAMING_OR_DOZING, %d, %llu)", __func__, value, (unsigned long long)data);
			power_set_profile(sec, value ? PROFILE_DREAMING_OR_DOZING : sec->profile.requested);
			break;
#endif // POWER_HAS_NEXUSOS_HINTS
#endif // TARGET_POWER_PROFILES
			
		/***********************************
		 * Boosting
		 */
#ifdef TARGET_POWER_INTERACTION_BOOST
		case POWER_HINT_INTERACTION:
			// ALOGI("%s: hint(POWER_HINT_INTERACTION, %d, %llu)", __func__, value, (unsigned long long)data);

			power_boostpulse(value ? value : 50000);
			power_boostpulse(value ? value : 50000);

			break;
#endif // TARGET_POWER_INTERACTION_BOOST

#ifdef TARGET_POWER_CPU_BOOST
#ifdef POWER_HAS_LINEAGE_HINTS
        case POWER_HINT_CPU_BOOST:
			// ALOGI("%s: hint(POWER_HINT_CPU_BOOST, %d, %llu)", __func__, value, (unsigned long long)data);

			power_boostpulse(value);
			power_boostpulse(value);

			break;
#endif // POWER_HAS_LINEAGE_HINTS
#endif // TARGET_POWER_CPU_BOOST

		/***********************************
		 * Inputs
		 */
		case POWER_HINT_DISABLE_TOUCH:
			ALOGI("%s: hint(POWER_HINT_DISABLE_TOUCH, %d, %llu)", __func__, value, (unsigned long long)data);
			power_input_device_state(sec, value ? INPUT_STATE_DISABLE : INPUT_STATE_ENABLE);
			break;

		default: break;
	}

	pthread_mutex_unlock(&sec->lock);
	ALOGDD("%s: exit;", __func__);
}

/***********************************
 * Profiles
 */
static void power_set_profile(struct sec_power_module *sec, int profile) {
 	ALOGI("%s: apply profile %d", __func__, profile);

	// store it
	sec->profile.current = profile;

	// apply settings
	struct power_profile data = power_profiles[sec->profile.current + 2];

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
#ifdef POWER_HAS_LINEAGE_HINTS
static void power_boostpulse(int duration) {
	// ALOGDD("%s: duration     = %d", __func__, duration);

	if (duration > 0) {
		pfwritegov(0, "boostpulse_duration", duration);
		pfwritegov(1, "boostpulse_duration", duration);
	}

	pfwritegov(0, "boostpulse", true);
	pfwritegov(1, "boostpulse", true);
}
#endif

/***********************************
 * Inputs
 */
static void power_fingerprint_state(bool state) {
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
		pfwrite(POWER_FINGERPRINT_WAKELOCKS, false);
	}
}

static void power_dt2w_state(struct sec_power_module *sec, bool state) {
	sec->input.dt2w = !!state;
	if (state) {
		pfwrite_legacy(POWER_DT2W_ENABLED, true);
	} else {
		pfwrite_legacy(POWER_DT2W_ENABLED, false);
	}
}
 
static void power_input_device_state(struct sec_power_module *sec, int state) {
	ALOGDD("%s: state = %d", __func__, state);

	switch (state) {
		case INPUT_STATE_DISABLE:

			// save to current state to prevent enabling
			pfread(POWER_TOUCHKEYS_ENABLED, &sec->input.touchkeys_enabled);

			pfwrite(sec->input.touchscreen_control_path, false);
			pfwrite(POWER_TOUCHKEYS_ENABLED, false);
			pfwrite(POWER_TOUCHKEYS_BRIGTHNESS, 0);

#ifdef TARGET_POWER_SHUTDOWN_FINGERPRINT
			power_fingerprint_state(false);
#endif

#ifdef TARGET_POWER_SUPPORT_DT2W
			power_dt2w_state(sec, sec->input.dt2w);
#endif

			break;

		case INPUT_STATE_ENABLE:

			pfwrite(sec->input.touchscreen_control_path, true);

			if (sec->input.touchkeys_enabled) {
				pfwrite(POWER_TOUCHKEYS_ENABLED, true);
				pfwrite(POWER_TOUCHKEYS_BRIGTHNESS, 255);
			}

			power_fingerprint_state(true);

#ifdef TARGET_POWER_SUPPORT_DT2W
			power_dt2w_state(sec, sec->input.dt2w);
#endif

			break;
	}
}

static void power_set_interactive(struct power_module __unused * module, int on) {
	struct sec_power_module *sec = container_of(module, struct sec_power_module, base);
	int screen_is_on = (on != 0);

	ALOGDD("%s: enter; on=%d", __func__, on);
	pthread_mutex_lock(&sec->lock);

	if (!screen_is_on) {
		power_set_profile(sec, PROFILE_SCREEN_OFF);
	} else {
		power_set_profile(sec, sec->profile.requested);
	}

	power_input_device_state(sec, screen_is_on ? INPUT_STATE_ENABLE : INPUT_STATE_DISABLE);

	pthread_mutex_unlock(&sec->lock);
	ALOGDD("%s: exit;", __func__);
}

/***********************************
 * Features
 */
#ifdef POWER_HAS_LINEAGE_HINTS
static int power_get_feature(struct power_module *module __unused, feature_t feature) {
	struct sec_power_module *sec = container_of(module, struct sec_power_module, base);
	int retval = -EINVAL;
	
	ALOGDD("%s: enter; feature=%d", __func__, feature);
	pthread_mutex_lock(&sec->lock);

	switch (feature) {
		case POWER_FEATURE_SUPPORTED_PROFILES:
			ALOGDD("%s: request for POWER_FEATURE_SUPPORTED_PROFILES = %d", __func__, PROFILE_MAX_USABLE);
#ifdef TARGET_POWER_EXTENDED_PROFILES
			retval = PROFILE_MAX_USABLE;
#else
			retval = PROFILE_MAX_USABLE_EXTENDED;
#endif
			break;

		case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
			ALOGDD("%s: request for POWER_FEATURE_DOUBLE_TAP_TO_WAKE = 1", __func__);
#ifdef TARGET_POWER_SUPPORT_DT2W
			retval = 1;
#else
			retval = 0;
#endif
			break;
	}

	pthread_mutex_unlock(&sec->lock);
	ALOGDD("%s: exit; rc=%d", __func__, retval);

	return retval;
}
#endif

static void power_set_feature(struct power_module *module, feature_t feature, int state) {
	struct sec_power_module *sec = container_of(module, struct sec_power_module, base);
	
	ALOGDD("%s: enter; feature=%d, state=%d", __func__, feature, state);
	pthread_mutex_lock(&sec->lock);

	switch (feature) {
#ifdef TARGET_POWER_SUPPORT_DT2W
		case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
			ALOGDD("%s: set POWER_FEATURE_DOUBLE_TAP_TO_WAKE to \"%d\"", __func__, state);
			power_dt2w_state(sec, state);
			break;
#endif

		default:
			ALOGW("Error setting the feature %d and state %d, it doesn't exist\n",
				  feature, state);
		break;
	}

	pthread_mutex_unlock(&sec->lock);
	ALOGDD("%s: exit", __func__);
}

/***********************************
 * Utilities
 */
// C++ I/O
static bool pfwrite(string path, string str) {
	ofstream file;

	file.open(path);
	if (!file.is_open()) {
		ALOGE("%s: failed to open \"%s\"", __func__, path.c_str());
		return false;
	}

	ALOGDD("%s: store \"%s\" to \"%s\"", __func__, str.c_str(), path.c_str());

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

static bool pfread(string path, string &str) {
	ifstream file(path);

	if (!file.is_open()) {
		ALOGE("%s: failed to open \"%s\"", __func__, path.c_str());
		return false;
	}

	if (!getline(file, str)) {
		ALOGE("%s: failed to read from \"%s\"", __func__, path.c_str());
		return false;
	}

	ALOGDD("%s: read \"%s\" from \"%s\"", __func__, str.c_str(), path.c_str());

	file.close();
	return true;
}

static bool pfread(string path, bool *f) {
	int out = 0;

	if (!pfread(path, &out))
		return false;

	*f = (out ? true : false);
	return true;
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

	ALOGDD("%s: read \"%s\" from \"%s\"", __func__, line.c_str(), path.c_str());
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
#ifdef POWER_HAS_LINEAGE_HINTS
		.getFeature = power_get_feature,
#endif
		.setFeature = power_set_feature,
		.setInteractive = power_set_interactive,
	},

	.lock = PTHREAD_MUTEX_INITIALIZER,

	.profile = {
		.current = PROFILE_BALANCED,
		.requested = PROFILE_BALANCED,
	},

	.input = {
		.touchkeys_enabled = true,
		.touchscreen_control_path = "",
	},

};
