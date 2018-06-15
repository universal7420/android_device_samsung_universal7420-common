/*
 * Copyright (C) 2016 The Android Open Source Project
 * Copyright (C) 2018 The LineageOS Project
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

#define LOG_TAG "zero-power"
// #define LOG_NDEBUG 0

#include <log/log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#include "Power.h"
#include "Profiles.h"
#include "../Utils.h"

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {

using ::vendor::nexus::zero::Utils;

Power::Power()
{
	ALOGV("%s: enter;", __func__);
	power_lock();

	//
	// defaults
	//
	mIsInteractive = false;
	mRequestedProfile = SecPowerProfiles::INVALID;
	mCurrentProfile = SecPowerProfiles::INVALID;
	mVariant = SecDeviceVariant::UNKNOWN;
	mTouchControlPath = "";
	mTouchkeysEnabled = true;
	mIsDT2WEnabled = false;

	//
	// reading, asserting
	//
	string touchDeviceName;
	if (!Utils::read(POWER_TOUCHSCREEN_NAME, touchDeviceName)) {
		LOG_ALWAYS_FATAL("%s: failed to read name of touchscreen sysfs-device", __func__);
	}

	if (touchDeviceName == POWER_TOUCHSCREEN_NAME_EXPECT) {
		mVariant = SecDeviceVariant::FLAT;
		mTouchControlPath = POWER_TOUCHSCREEN_ENABLED_FLAT;
	} else {
		mVariant = SecDeviceVariant::EDGE;
		mTouchControlPath = POWER_TOUCHSCREEN_ENABLED_EDGE;
	}

	//
	// profiles
	//
	DEBUG_TIMING(profiles, Profiles::loadProfiles());

	//
	// initial device-setup
	//

	// set default power-profile
	mRequestedProfile = SecPowerProfiles::BALANCED;
	setProfile(mRequestedProfile);

	// enable all input-devices
	setInputState(true);
	setFingerprintState(true);

	ALOGV("%s: exit;", __func__);
}

status_t Power::registerAsSystemService() {
	status_t ret = 0;

	ret = IPower::registerAsService();
	LOG_ALWAYS_FATAL_IF(ret != 0, "%s: failed to register IPower HAL Interface as service (%d), aborting...", __func__, ret);
	ALOGI("%s: registered IPower HAL Interface!", __func__);

#ifdef POWER_HAS_LINEAGE_HINTS
	ret = ILineagePower::registerAsService();
	LOG_ALWAYS_FATAL_IF(ret != 0, "%s: failed to register ILineagePower HAL Interface as service (%d), aborting...", __func__, ret);
	ALOGI("%s: registered ILineagePower HAL Interface!", __func__);
#else
	ALOGI("%s: current ROM does not support ILineagePower HAL Interface!", __func__);
#endif

	return 0;
}

Power::~Power() { }

// Methods from ::android::hardware::power::V1_0::IPower follow.
Return<void> Power::setInteractive(bool interactive) {
	auto begin = Utils::getTime();

	ALOGV("%s: enter; interactive=%d", __func__, interactive ? 1 : 0);
	power_lock();

	this->mIsInteractive = interactive;

	if (!interactive && Utils::screenIsOn()) {
		ALOGW("%s: not disabling interactive state when screen is still on", __func__);
		goto exit;
	}

	if (!interactive) {
		setProfile(SecPowerProfiles::SCREEN_OFF);
	} else {
		// reset to requested- or fallback-profile
		resetProfile(500);
	}

	// speed up the device a bit
	/* Utils::write("/sys/kernel/hmp/boostpulse_duration", 2500000); // 2.5s
	Utils::write("/sys/kernel/hmp/boostpulse", true); */

	setInputState(interactive);

exit:
	auto end = Utils::getTime();
	auto diff = end - begin;

	ALOGV("%s: exit; took %lldms", __func__, diff.count());
	return Void();
}

Return<void> Power::powerHint(PowerHint hint, int32_t data)  {
	// ALOGV("%s: enter; hint=%d, data=%d", __func__, hint, data);
	power_lock();

	switch_uint32_t (hint)
	{
		/*
		 * Profiles
		 */
#ifdef POWER_HAS_LINEAGE_HINTS
		case_uint32_t (LineagePowerHint::SET_PROFILE):
		{
			ALOGV("%s: LineagePowerHint::SET_PROFILE(%d)", __func__, data);
			mRequestedProfile = static_cast<SecPowerProfiles>(data);
			setProfile(mRequestedProfile);
			break;
		}
#endif
		case_uint32_t (PowerHint::LOW_POWER):
		{
			ALOGV("%s: PowerHint::LOW_POWER(%d)", __func__, data);
			if (data) {
				setProfile(SecPowerProfiles::POWER_SAVE);
			} else {
				resetProfile();
			}
			break;
		}
		case_uint32_t (PowerHint::SUSTAINED_PERFORMANCE):
		{
			ALOGV("%s: PowerHint::SUSTAINED_PERFORMANCE(%d)", __func__, data);
			if (data) {
				setProfile(SecPowerProfiles::HIGH_PERFORMANCE);
			} else {
				resetProfile();
			}
			break;
		}
		case_uint32_t (PowerHint::VR_MODE):
		{
			ALOGV("%s: PowerHint::VR_MODE(%d)", __func__, data);
			if (data) {
				setProfile(SecPowerProfiles::HIGH_PERFORMANCE);
			} else {
				resetProfile();
			}
			break;
		}
		
		/*
		 * Interaction/Boosting
		 */
		case_uint32_t (PowerHint::INTERACTION):
		{
			// ALOGV("%s: PowerHint::INTERACTION(%d)", __func__, data);
			boostpulse(data);
			break;
		}
		case_uint32_t (PowerHint::LAUNCH):
		{
			// ALOGV("%s: PowerHint::LAUNCH(%d)", __func__, data);
			break;
		}
		case_uint32_t (PowerHint::VSYNC):
		{
			// ALOGV("%s: PowerHint::VSYNC(%d)", __func__, data);
			break;
		}
	}

	// ALOGV("%s: exit;", __func__);
	return Void();
}

Return<void> Power::setFeature(Feature feature, bool activate)  {
	ALOGV("%s: enter; feature=%d, activate=%d", __func__, feature, activate ? 1 : 0);
	power_lock();

	switch_uint32_t (feature)
	{
		case_uint32_t (Feature::POWER_FEATURE_DOUBLE_TAP_TO_WAKE):
		{
			mIsDT2WEnabled = activate;
			setDT2WState();
			break;
		}
	}

	ALOGV("%s: exit;", __func__);
	return Void();
}

Return<void> Power::getPlatformLowPowerStats(getPlatformLowPowerStats_cb _hidl_cb)  {
	ALOGV("%s: enter;", __func__);
	power_lock();

	hidl_vec<PowerStatePlatformSleepState> states;
	_hidl_cb(states, Status::SUCCESS);

	ALOGV("%s: exit;", __func__);
	return Void();
}

// Methods from ::vendor::lineage::power::V1_0::ILineagePower follow.
#ifdef POWER_HAS_LINEAGE_HINTS
Return<int32_t> Power::getFeature(LineageFeature feature)  {	
	ALOGV("%s: enter; feature=%d", __func__, feature);
	power_lock();

	switch_uint32_t (feature)
	{
		case_uint32_t (LineageFeature::SUPPORTED_PROFILES):
		{
			return static_cast<int>(SecPowerProfiles::MAX_PROFILES);
		}

		case_uint32_t (Feature::POWER_FEATURE_DOUBLE_TAP_TO_WAKE):
		{
			return (Utils::isFile(POWER_DT2W_ENABLED) && 
			    Utils::canWrite(POWER_DT2W_ENABLED)) ? 1 : 0;
		}
	}

	ALOGV("%s: exit;", __func__);
	return -1;
}
#endif

// Private Methods
void Power::boostpulse(int duration) {
	std::lock_guard<std::mutex> autolock(mBoostpulseLock);

	if (duration <= 0) {
		duration = (1000 / 60) * 10;
	}

	// get current profile data
	const SecPowerProfile* data = Profiles::getProfileData(mCurrentProfile);

	if (data->cpu.apollo.freq_boost) {
		if (!Utils::updateCpuGov(0)) {
			ALOGW("Failed to load current cpugov-configuration for APOLLO");
#ifdef STRICT_BEHAVIOUR
			return;
#endif
		}

		Utils::writeCpuGov(0, "boost_freq", data->cpu.apollo.freq_boost);
		Utils::writeCpuGov(0, "boostpulse_duration", duration * 1000);
		Utils::writeCpuGov(0, "boostpulse", true);
	}

	if (data->cpu.atlas.freq_boost) {
		if (!Utils::updateCpuGov(4)) {
			ALOGW("Failed to load current cpugov-configuration for ATLAS");
#ifdef STRICT_BEHAVIOUR
			return;
#endif
		}

		Utils::writeCpuGov(4, "boost_freq", data->cpu.atlas.freq_boost);
		Utils::writeCpuGov(4, "boostpulse_duration", duration * 1000);
		Utils::writeCpuGov(4, "boostpulse", true);
	}
}

void Power::setProfile(SecPowerProfiles profile) {
	auto begin = Utils::getTime();
 	ALOGI("%s: applying profile %d", __func__, profile);

	// store it
	mCurrentProfile = profile;

	// check if user disabled power-profiles
	if (!isModuleEnabled("profiles")) {
		ALOGI("power profiles disabled by user!");
		return;
	}

	// apply settings
	const SecPowerProfile* data = Profiles::getProfileData(mCurrentProfile);

	if (!data->enabled) {
		return;
	}

	// online policy-cores before querying cpufreq-data
	CPU_ONLINE(0);
	CPU_ONLINE(4);

	if (data->cpu.enabled)
	{
		/*********************
		 * CPU Cluster0
		 */
		if (data->cpu.apollo.enabled) {
			Utils::write("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", data->cpu.apollo.governor);

			if (!Utils::updateCpuGov(0)) {
				ALOGW("Failed to load current cpugov-configuration for APOLLO");
#ifdef STRICT_BEHAVIOUR
				return;
#endif
			}

			// to keep frequencies in range while screen-on we use the cpugovs, but
			// while screen-off it can happen that frequencies get increased too much
			// make sure our limits are being applied to the then-limiting,
			// not very reliable, files too
			Utils::write("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq", data->cpu.apollo.freq_min);
			Utils::write("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", data->cpu.apollo.freq_max);

			Utils::write("/sys/devices/system/cpu/cpu0/online", data->cpu.apollo.cores.core1);
			Utils::write("/sys/devices/system/cpu/cpu1/online", data->cpu.apollo.cores.core2);
			Utils::write("/sys/devices/system/cpu/cpu2/online", data->cpu.apollo.cores.core3);
			Utils::write("/sys/devices/system/cpu/cpu3/online", data->cpu.apollo.cores.core4);

			cpu_apollo_write(freq_min);
			cpu_apollo_write(freq_max);
			cpu_apollo_write2(freq_hispeed, "hispeed_freq");

			for (int i = 0; data->cpu.apollo.governor_data[i].name[0]; i++) {
				Utils::writeCpuGov(0, data->cpu.apollo.governor_data[i].name,
					std::string(data->cpu.apollo.governor_data[i].data));
			}
		}

		/*********************
		 * CPU Cluster1
		 */
		if (data->cpu.atlas.enabled) {
			Utils::write("/sys/devices/system/cpu/cpu4/cpufreq/scaling_governor", data->cpu.atlas.governor);

			if (!Utils::updateCpuGov(4)) {
				ALOGW("Failed to load current cpugov-configuration for ATLAS");
#ifdef STRICT_BEHAVIOUR
				return;
#endif
			}

			// to keep frequencies in range while screen-on we use the cpugovs, but
			// while screen-off it can happen that frequencies get increased too much
			// make sure our limits are being applied to the then-limiting,
			// not very reliable, files too
			Utils::write("/sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq", data->cpu.atlas.freq_min);
			Utils::write("/sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq", data->cpu.atlas.freq_max);

			Utils::write("/sys/devices/system/cpu/cpu4/online", data->cpu.atlas.cores.core1);
			Utils::write("/sys/devices/system/cpu/cpu5/online", data->cpu.atlas.cores.core2);
			Utils::write("/sys/devices/system/cpu/cpu6/online", data->cpu.atlas.cores.core3);
			Utils::write("/sys/devices/system/cpu/cpu7/online", data->cpu.atlas.cores.core4);

			cpu_atlas_write(freq_min);
			cpu_atlas_write(freq_max);
			cpu_atlas_write2(freq_hispeed, "hispeed_freq");

			for (int i = 0; data->cpu.atlas.governor_data[i].name[0]; i++) {
				Utils::writeCpuGov(4, data->cpu.atlas.governor_data[i].name,
					std::string(data->cpu.atlas.governor_data[i].data));
			}
		}
	}

	/*********************
	 * cpusets
	 */
	if (data->cpusets.enabled) {
		Utils::write("/dev/cpuset/cpus",                   data->cpusets.defaults);
		Utils::write("/dev/cpuset/foreground/cpus",        data->cpusets.foreground);
		Utils::write("/dev/cpuset/foreground/boost/cpus",  data->cpusets.foreground_boost);
		Utils::write("/dev/cpuset/background/cpus",        data->cpusets.background);
		Utils::write("/dev/cpuset/system-background/cpus", data->cpusets.system_background);
		Utils::write("/dev/cpuset/top-app/cpus",           data->cpusets.top_app);
	}

	/*********************
	 * IPA
	 */
	if (data->ipa.enabled) {
		Utils::write("/sys/power/ipa/enabled", "Y");
		Utils::write("/sys/power/ipa/control_temp", data->ipa.control_temp);
	}

	/*********************
	 * GPU Defaults
	 */
	if (data->gpu.enabled) {
		if (data->gpu.dvfs.enabled) {
			Utils::write("/sys/devices/platform/gpusysfs/gpu_min_clock", data->gpu.dvfs.freq_min);
			Utils::write("/sys/devices/platform/gpusysfs/gpu_max_clock", data->gpu.dvfs.freq_max);
		}
		if (data->gpu.highspeed.enabled) {
			Utils::write("/sys/devices/14ac0000.mali/highspeed_clock",   data->gpu.highspeed.freq);
			Utils::write("/sys/devices/14ac0000.mali/highspeed_load",    data->gpu.highspeed.load);
		}
	}

	/*********************
	 * Kernel Defaults
	 */
	if (data->hmp.enabled) {
		Utils::write("/sys/kernel/hmp/boost", data->hmp.boost);
		Utils::write("/sys/kernel/hmp/semiboost", data->hmp.semiboost);
	}

	if (data->kernel.enabled) {
		// Keep dynamic hotplugging disabled to 1.) ensure availability of all
		// clusters when power-HAL gets a setInteractive()-event and 2.)
		// to drastically lower the the screen-on-delay
		Utils::write("/sys/power/enable_dm_hotplug", false);

		// The power-efficient workqueue is useful for lower-power-situations, but
		// contraproductive in high-performance situations. This should reflect in
		// the static power-profiles
		Utils::write("/sys/module/workqueue/parameters/power_efficient", data->kernel.pewq);
	}

	/*********************
	 * Slow Mode Defaults
	 */
	if (data->slow.enabled) {
		Utils::write("/sys/devices/virtual/sec/sec_slow/enforced_slow_mode", data->slow.mode_toggle);
		Utils::write("/sys/devices/virtual/sec/sec_slow/timer_rate", data->slow.timer_rate);
	}

	/*********************
	 * Input-Booster Defaults
	 */
	if (data->input_booster.enabled) {
		Utils::write("/sys/class/input_booster/tail", data->input_booster.tail);
		Utils::write("/sys/class/input_booster/head", data->input_booster.head);
	}

	auto end = Utils::getTime();
	auto diff = end - begin;

	ALOGV("%s: exit; took %lldms", __func__, diff.count());
}

void Power::setProfile(SecPowerProfiles profile, int delay) {
	DELAY(setProfile(profile), delay);
}

void Power::resetProfile(int delay) {
	if (mRequestedProfile == SecPowerProfiles::INVALID) {
		setProfile(SecPowerProfiles::BALANCED, delay);
	} else {
		setProfile(mRequestedProfile, delay);
	}
}

void Power::setInputState(bool enabled) {
	auto begin = Utils::getTime();
 	ALOGI("%s: enter; enabled=%d", __func__, enabled ? 1 : 0);

	if (enabled) {
		if (!mTouchControlPath.empty()) {
			DEBUG_TIMING(touchscreen, Utils::write(mTouchControlPath, true));
		}

		if (mVariant != SecDeviceVariant::EDGE) {
			ASYNC(DEBUG_TIMING(touchkeys_state, Utils::write(POWER_TOUCHKEYS_ENABLED, mTouchkeysEnabled)));
			DEBUG_TIMING(touchkeys_brightness, Utils::write(POWER_TOUCHKEYS_BRIGHTNESS, 255));
		}
	} else {
		if (mVariant != SecDeviceVariant::EDGE) {
			// save to current state to prevent enabling
			DEBUG_TIMING(touchkeys_state_read, Utils::read(POWER_TOUCHKEYS_ENABLED, mTouchkeysEnabled));

			// disable them
			DEBUG_TIMING(touchkeys_state, Utils::write(POWER_TOUCHKEYS_ENABLED, false));
			DEBUG_TIMING(touchkeys_brightness, Utils::write(POWER_TOUCHKEYS_BRIGHTNESS, 0));
		}

		// only disable touchscreen if we aren't using DT2W
		if (!mTouchControlPath.empty() && !mIsDT2WEnabled) {
			DEBUG_TIMING(touchscreen, Utils::write(mTouchControlPath, false));
		}
	}

	DEBUG_TIMING(dt2w, setDT2WState());

	auto end = Utils::getTime();
	auto diff = end - begin;

	ALOGV("%s: exit; took %lldms", __func__, diff.count());
}

void Power::setFingerprintState(bool enabled) {
	/*
	 * vfs7xxx power management interface:
	 *
	 * regulator: fp sensor's main GPIO regulation pins
	 * sleep-pin: GPIO pin setting the current power-state
	 * SMC: secure monitor (TrustZone) call
	 *
	 * 1 (true)  -> power on:
	 *   * removing internal pm management block
	 *   * resuming debugging timer work
	 *   * powering on regulator
	 *       * SMC to initialize FP sensor boot
	 *       * delaying for ~2,95-3,00ms
	 *       * powering on ldo_pin2 GPIO pin
	 *       * powering on ldo_pin GPIO pin
	 *   * delaying for 10ms
	 *   * hard resetting GPIO sleep pin
	 *       * setting sleep pin(0)
	 *       * delaying for 1ms
	 *       * setting sleep pin(1)
	 *       * delaying for 5ms
	 *   * delaying for 20ms
	 *
	 * 0 (false)  -> power off:
	 *   * powering off regulator
	 *       * powering off ldo_pin GPIO pin
	 *       * powering off ldo_pin2 GPIO pin
	 *       * SMC to finish FP sensor shutdown
	 *   * setting GPIO sleep pin
	 *       * setting sleep pin(0)
	 *   * stopping debugging timer work
	 *   * setting internal pm management block
	 */
	if (enabled) {
		DEBUG_TIMING(fingerprint_wakelocks, Utils::write(POWER_FINGERPRINT_WAKELOCKS, true));
		DEBUG_TIMING(fingerprint_pm, Utils::write(POWER_FINGERPRINT_PM, true));
	} else {
		DEBUG_TIMING(fingerprint_pm, Utils::write(POWER_FINGERPRINT_PM, false));
		DEBUG_TIMING(fingerprint_wakelocks, Utils::write(POWER_FINGERPRINT_WAKELOCKS, false));
	}
}

void Power::setDT2WState() {
	if (mIsDT2WEnabled) {
		Utils::write(POWER_DT2W_ENABLED, true);
	} else {
		Utils::write(POWER_DT2W_ENABLED, false);
	}
}

bool Power::isModuleEnabled(string module) {
	return (GetProperty("sys.power." + module, "true") == "true");
}

} // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace hardware
}  // namespace android
