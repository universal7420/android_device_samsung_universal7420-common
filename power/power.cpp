/*
 * Copyright (C) 2013 The Android Open Source Project
 * Copyright (C) 2017 Jesse Chan <cjx123@outlook.com>
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

#include <atomic>
#include <cutils/properties.h>
#include <future>
#include <hardware/hardware.h>
#include <hardware/power.h>
#include <linux/stat.h>
#include <math.h>
#include <pwd.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>

#include "power.h"
#include "utils.h"

using namespace std;

#define container_of(addr, struct_name, field_name) \
	((struct_name *)((char *)(addr) - offsetof(struct_name, field_name)))

void power_init(SecPowerModule *power) {
	ALOGDD("%s: enter;", __func__);
	POWER_LOCK();

	// set defaults
	power->initialized = false;
	power->profile.current = SecPowerProfiles::INVALID;
	power->profile.requested = SecPowerProfiles::INVALID;
	power->input.touchkeys_enabled = true;
	power->input.touchscreen_control_path = "";

	// get correct touchkeys/enabled-file
	// reads from input1/name:
	//   - flat will return sec_touchscreen
	//   - edge won't
	string touchscreen_input_name;
	read(POWER_TOUCHSCREEN_NAME, touchscreen_input_name);
	ALOGDD("%s: '%s' == '%s'", __func__, touchscreen_input_name.c_str(), POWER_TOUCHSCREEN_NAME_EXPECT);
	if (touchscreen_input_name == POWER_TOUCHSCREEN_NAME_EXPECT) {
		power->variant = SecDeviceVariant::FLAT;
		power->input.touchscreen_control_path = POWER_TOUCHSCREEN_ENABLED_FLAT;
	} else {
		power->variant = SecDeviceVariant::EDGE;
		power->input.touchscreen_control_path = POWER_TOUCHSCREEN_ENABLED_EDGE;
	}

	// set to normal power profile
	power->profile.requested = SecPowerProfiles::BALANCED;
	power_set_profile(power, SecPowerProfiles::BALANCED);

	// initialize all input-devices
	power_input_device_state(power, true);

	// disable dt2w by default
	power_dt2w_state(power, false);

	// enable fingerprint by default
	power_fingerprint_state(power, true);

	power->initialized = true;
	POWER_UNLOCK();
	ALOGDD("%s: exit;", __func__);
}

/***********************************
 * Hinting
 */
void power_hint(SecPowerModule *power, power_hint_t hint, int32_t data) {
	SecPowerProfiles profile = SecPowerProfiles::INVALID;

	ALOGDD("%s: enter; hint=%d", __func__, hint);
	POWER_LOCK();

	if (!power->initialized) {
		ALOGDD("%s: exit; (not yet initialized)", __func__);
		return;
	}

	switch (hint) {

		/***********************************
		 * Profiles
		 */
#ifdef POWER_HAS_LINEAGE_HINTS
		case POWER_HINT_SET_PROFILE:
			ALOGI_HINT("POWER_HINT_SET_PROFILE");
			profile = static_cast<SecPowerProfiles>(data);

			power->profile.requested = profile;
			power_set_profile(power, profile);
			break;
#endif

		case POWER_HINT_LOW_POWER:
			ALOGI_HINT("POWER_HINT_LOW_POWER");
			if (data) {
				power_set_profile(power, SecPowerProfiles::POWER_SAVE);
			} else {
				// reset to requested- or fallback-profile
				power_reset_profile(power);
			}
			break;

		case POWER_HINT_SUSTAINED_PERFORMANCE:
			ALOGI_HINT("POWER_HINT_SUSTAINED_PERFORMANCE");
			if (data) {
				power_set_profile(power, SecPowerProfiles::HIGH_PERFORMANCE);
			} else {
				// reset to requested- or fallback-profile
				power_reset_profile(power);
			}
			break;

		case POWER_HINT_VR_MODE:
			ALOGI_HINT("POWER_HINT_VR_MODE");
			if (data) {
				power_set_profile(power, SecPowerProfiles::HIGH_PERFORMANCE);
			} else {
				// reset to requested- or fallback-profile
				power_reset_profile(power);
			}
			break;

		/***********************************
		 * Profiles
		 */
		case POWER_HINT_VSYNC:
			ALOGDD_HINT("POWER_HINT_VSYNC");
			break;

		case POWER_HINT_INTERACTION:
			ALOGDD_HINT("POWER_HINT_INTERACTION");
			break;

		case POWER_HINT_LAUNCH:
			ALOGDD_HINT("POWER_HINT_LAUNCH");
			break;

		/***********************************
		 * Inputs
		 */
		case POWER_HINT_DISABLE_TOUCH:
			ALOGI_HINT("POWER_HINT_DISABLE_TOUCH");
			if (!power->screen_on) {
				power_input_device_state(power, data ? false : true);
			} else {
				// don't do this...
			}
			break;

		default: break;
	}

	POWER_UNLOCK();
	ALOGDD("%s: exit;", __func__);
}


/***********************************
 * Profiles
 */
#include "profiles.h"
static const map<SecPowerProfiles, SecPowerProfile*> kPowerProfiles = {
	{ SecPowerProfiles::SCREEN_OFF,       &kPowerProfileScreenOff },
	{ SecPowerProfiles::POWER_SAVE,       &kPowerProfilePowerSave },
	{ SecPowerProfiles::BIAS_POWER_SAVE,  &kPowerProfileBiasPowerSave },
	{ SecPowerProfiles::BALANCED,         &kPowerProfileBalanced },
	{ SecPowerProfiles::BIAS_PERFORMANCE, &kPowerProfileBiasPerformance },
	{ SecPowerProfiles::HIGH_PERFORMANCE, &kPowerProfileHighPerformance },
};

static void power_set_profile(SecPowerModule *power, SecPowerProfiles profile) {
 	ALOGI("%s: apply profile %d", __func__, profile);

	// store it
	power->profile.current = profile;

	// apply settings
	const SecPowerProfile* data = kPowerProfiles.find(power->profile.current)->second;

	// online policy-cores before querying cpufreq-data
	CPU_ONLINE(0);
	CPU_ONLINE(4);

	// read current configuration
	if (!update_current_cpugov_path(0) ||
		!update_current_cpugov_path(4)) {
		ALOGW("Failed to load current cpugov-configuration");
#ifdef STRICT_BEHAVIOUR
		return;
#endif
	}

	/*********************
	 * CPU Cluster0
	 */
	write("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", data->cpu.apollo.governor);
	
	cpu_apollo_write(freq_min);
	cpu_apollo_write(freq_max);
	cpu_apollo_write(freq_hispeed);

	if_apollo_cpugov("nexus")
	{
		cpugov_apollo_write2(nexus, lpr_ratio, "lpr_down_ratio");
		cpugov_apollo_write2(nexus, lpr_ratio, "lpr_up_ratio");
		cpugov_apollo_write(nexus, lpr_down_elevation);
		cpugov_apollo_write(nexus, lpr_up_elevation);
	}
	else if_apollo_cpugov("interactive")
	{
		cpugov_apollo_write(interactive, above_hispeed_delay);
		cpugov_apollo_write(interactive, go_hispeed_load);
		cpugov_apollo_write(interactive, min_sample_time);
		cpugov_apollo_write(interactive, target_loads);
		cpugov_apollo_write(interactive, timer_rate);
		cpugov_apollo_write(interactive, timer_slack);
	}

	/*********************
	 * CPU Cluster1
	 */
	write("/sys/devices/system/cpu/cpu4/cpufreq/scaling_governor", data->cpu.atlas.governor);

	cpu_atlas_write(freq_min);
	cpu_atlas_write(freq_max);
	cpu_atlas_write(freq_max);

	if_atlas_cpugov("nexus")
	{
		cpugov_atlas_write2(nexus, lpr_ratio, "lpr_down_ratio");
		cpugov_atlas_write2(nexus, lpr_ratio, "lpr_up_ratio");
		cpugov_atlas_write(nexus, lpr_down_elevation);
		cpugov_atlas_write(nexus, lpr_up_elevation);
	}
	else if_atlas_cpugov("interactive")
	{
		cpugov_atlas_write(interactive, above_hispeed_delay);
		cpugov_atlas_write(interactive, go_hispeed_load);
		cpugov_atlas_write(interactive, min_sample_time);
		cpugov_atlas_write(interactive, target_loads);
		cpugov_atlas_write(interactive, timer_rate);
		cpugov_atlas_write(interactive, timer_slack);
	}

	/*********************
	 * GPU Defaults
	 */
	write("/sys/devices/14ac0000.mali/dvfs_min_lock",   data->gpu.dvfs.freq_min);
	write("/sys/devices/14ac0000.mali/dvfs_max_lock",   data->gpu.dvfs.freq_max);
	write("/sys/devices/14ac0000.mali/highspeed_clock", data->gpu.highspeed.freq);
	write("/sys/devices/14ac0000.mali/highspeed_load",  data->gpu.highspeed.load);

	/*********************
	 * Kernel Defaults
	 */

	// Keep dynamic hotplugging disabled to 1.) ensure availability of all
	// clusters when power-HAL gets a setInteractive()-event and 2.)
	// to drastically lower the the screen-on-delay
	write("/sys/power/enable_dm_hotplug", false);

	// The power-efficient workqueue is useful for lower-power-situations, but
	// contraproductive in high-performance situations. This should reflect in
	// the static power-profiles
	write("/sys/module/workqueue/parameters/power_efficient", data->kernel.pewq);
}

static void power_reset_profile(SecPowerModule *power) {
	if (power->profile.requested == SecPowerProfiles::INVALID) {
		// fallback to BALANCED if no valid profile was set using SET_PROFILE-hint
		power_set_profile(power, SecPowerProfiles::BALANCED);
	} else {
		power_set_profile(power, power->profile.requested);
	}
}


/***********************************
 * Inputs
 */
static void power_fingerprint_state(SecPowerModule *power, bool state) {
	/*
	 * Ordered power toggling:
	 *	Turn on:   +Wakelocks  ->  (+PM   ->  Delay)  ->  +Regulator
	 *	Turn off:  -Regulator  ->  (	  -PM	  )  ->  -Wakelocks
	 */
	if (state) {
		write(POWER_FINGERPRINT_WAKELOCKS, true);
#ifdef FINGERPRINT_POWER_MANAGEMENT
		write(POWER_FINGERPRINT_PM, true);
		delay(FINGERPRINT_PM_DELAY * 1000);
#endif
		write(POWER_FINGERPRINT_REGULATOR, true);
	} else {
		write(POWER_FINGERPRINT_REGULATOR, false);
#ifdef FINGERPRINT_POWER_MANAGEMENT
		write(POWER_FINGERPRINT_PM, false);
#endif
		write(POWER_FINGERPRINT_WAKELOCKS, false);
	}
}

static void power_dt2w_state(SecPowerModule *power, bool state) {
	power->input.dt2w = !!state;
	if (state) {
		write(POWER_DT2W_ENABLED, true);
	} else {
		write(POWER_DT2W_ENABLED, false);
	}
}
 
static void power_input_device_state(SecPowerModule *power, bool state) {
	ALOGDD("%s: state = %d", __func__, state);

	if (state) {
		if (power->input.touchscreen_control_path != "") {
			write(power->input.touchscreen_control_path, true);
		}

		if (power->input.touchkeys_enabled && power->variant != SecDeviceVariant::EDGE) {
			write(POWER_TOUCHKEYS_ENABLED, true);
			write(POWER_TOUCHKEYS_BRIGHTNESS, 255);
		}

		power_fingerprint_state(power, true);
		power_dt2w_state(power, power->input.dt2w);
	} else {
		if (power->variant != SecDeviceVariant::EDGE) {
			// save to current state to prevent enabling
			read(POWER_TOUCHKEYS_ENABLED, &power->input.touchkeys_enabled);

			// disable them
			write(POWER_TOUCHKEYS_ENABLED, false);
			write(POWER_TOUCHKEYS_BRIGHTNESS, 0);
		}

		if (power->input.touchscreen_control_path != "" && !power->input.dt2w) {
			write(power->input.touchscreen_control_path, false);
		}

		power_fingerprint_state(power, false);
		power_dt2w_state(power, power->input.dt2w);
	}
}

void power_set_interactive(SecPowerModule *power, bool interactive) {
	ALOGDD("%s: enter; on=%d", __func__, interactive ? 1 : 0);
	POWER_LOCK();

	if (!power->initialized) {
		ALOGDD("%s: exit; (not yet initialized)", __func__);
		return;
	}

	power->screen_on = interactive;

	if (!interactive) {
		power_set_profile(power, SecPowerProfiles::SCREEN_OFF);
	} else {
		// reset to requested- or fallback-profile
		power_reset_profile(power);
	}

	power_input_device_state(power, interactive);

	POWER_UNLOCK();
	ALOGDD("%s: exit;", __func__);
}

/***********************************
 * Features
 */
#ifdef POWER_HAS_LINEAGE_HINTS
int power_get_feature(SecPowerModule *power, feature_t feature) {
	int retval = -EINVAL;

	ALOGDD("%s: enter; feature=%d", __func__, feature);
	POWER_LOCK();

	if (!power->initialized) {
		ALOGDD("%s: exit; (not yet initialized)", __func__);
		goto exit;
	}

	switch (feature) {
		case POWER_FEATURE_SUPPORTED_PROFILES:
			ALOGDD("%s: request for POWER_FEATURE_SUPPORTED_PROFILES = %d", __func__, PROFILE_MAX_USABLE);
			retval = static_cast<int>(SecPowerProfiles::MAX_PROFILES);
			break;

		case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
			ALOGDD("%s: request for POWER_FEATURE_DOUBLE_TAP_TO_WAKE = 1", __func__);
			retval = is_file(POWER_DT2W_ENABLED);
			break;
	}

exit:
	POWER_UNLOCK();
	ALOGDD("%s: exit; rc=%d", __func__, retval);

	return retval;
}
#endif // POWER_HAS_LINEAGE_HINTS

void power_set_feature(SecPowerModule *power, feature_t feature, int32_t data) {
	ALOGDD("%s: enter; feature=%d, data=%d", __func__, feature, data);
	POWER_LOCK();

	if (!power->initialized) {
		ALOGDD("%s: exit; (not yet initialized)", __func__);
		return;
	}

	switch (feature) {
		case POWER_FEATURE_DOUBLE_TAP_TO_WAKE:
			ALOGDD("%s: set POWER_FEATURE_DOUBLE_TAP_TO_WAKE to \"%d\"", __func__, data);
			power_dt2w_state(power, data);
			break;

		default:
			ALOGW("Error setting the feature %d and data %d, it doesn't exist\n",
				  feature, data);
			break;
	}

	POWER_UNLOCK();
	ALOGDD("%s: exit", __func__);
}
