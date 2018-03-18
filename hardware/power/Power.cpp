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
#define LOG_NDEBUG 0

#include <log/log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#include "Power.h"
#include "Profiles.h"
#include "Utils.h"

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {

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
Return<void> Power::setInteractive(bool interactive)  {
	ALOGV("%s: enter; interactive=%d", __func__, interactive ? 1 : 0);
	power_lock();

	this->mIsInteractive = interactive;

	if (!interactive) {
		setProfile(SecPowerProfiles::SCREEN_OFF);
	} else {
		// reset to requested- or fallback-profile
		resetProfile();
	}

	// speed up the device a bit
	Utils::write("/sys/kernel/hmp/boostpulse_duration", 2500000); // 2.5s
	Utils::write("/sys/kernel/hmp/boostpulse", true);

	setInputState(interactive);

	ALOGV("%s: exit;", __func__);
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
void Power::setProfile(SecPowerProfiles profile) {
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

	// online policy-cores before querying cpufreq-data
	CPU_ONLINE(0);
	CPU_ONLINE(4);

	/*********************
	 * CPU Cluster0
	 */
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

	cpu_apollo_write(freq_min);
	cpu_apollo_write(freq_max);
	cpu_apollo_write2(freq_hispeed, "hispeed_freq");

	if_apollo_cpugov("nexus")
	{
		ALOGV("%s: APOLLO is running with cpugov \"nexus\"", __func__);

		cpugov_apollo_write2(nexus, lpr_ratio, "lpr_down_ratio");
		cpugov_apollo_write2(nexus, lpr_ratio, "lpr_up_ratio");
		cpugov_apollo_write(nexus, lpr_down_elevation);
		cpugov_apollo_write(nexus, lpr_up_elevation);
	}
	else if_apollo_cpugov("interactive")
	{
		ALOGV("%s: APOLLO is running with cpugov \"interactive\"", __func__);

		cpugov_apollo_write(interactive, above_hispeed_delay);
		cpugov_apollo_write(interactive, go_hispeed_load);
		cpugov_apollo_write(interactive, min_sample_time);
		cpugov_apollo_write(interactive, target_loads);
		cpugov_apollo_write(interactive, timer_rate);
		cpugov_apollo_write(interactive, timer_slack);
	}
	else
	{
		ALOGE("%s: APOLLO is running with unknown/unsupported cpugov", __func__);
	}


	/*********************
	 * CPU Cluster1
	 */
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

	cpu_atlas_write(freq_min);
	cpu_atlas_write(freq_max);
	cpu_atlas_write2(freq_hispeed, "hispeed_freq");

	if_atlas_cpugov("nexus")
	{
		ALOGV("%s: ATLAS is running with cpugov \"nexus\"", __func__);

		cpugov_atlas_write2(nexus, lpr_ratio, "lpr_down_ratio");
		cpugov_atlas_write2(nexus, lpr_ratio, "lpr_up_ratio");
		cpugov_atlas_write(nexus, lpr_down_elevation);
		cpugov_atlas_write(nexus, lpr_up_elevation);
	}
	else if_atlas_cpugov("interactive")
	{
		ALOGV("%s: ATLAS is running with cpugov \"interactive\"", __func__);

		cpugov_atlas_write(interactive, above_hispeed_delay);
		cpugov_atlas_write(interactive, go_hispeed_load);
		cpugov_atlas_write(interactive, min_sample_time);
		cpugov_atlas_write(interactive, target_loads);
		cpugov_atlas_write(interactive, timer_rate);
		cpugov_atlas_write(interactive, timer_slack);
	}
	else
	{
		ALOGE("%s: ATLAS is running with unknown/unsupported cpugov", __func__);
	}

	/*********************
	 * GPU Defaults
	 */
	Utils::write("/sys/devices/14ac0000.mali/dvfs_min_lock",   data->gpu.dvfs.freq_min);
	Utils::write("/sys/devices/14ac0000.mali/dvfs_max_lock",   data->gpu.dvfs.freq_max);
	Utils::write("/sys/devices/14ac0000.mali/highspeed_clock", data->gpu.highspeed.freq);
	Utils::write("/sys/devices/14ac0000.mali/highspeed_load",  data->gpu.highspeed.load);

	/*********************
	 * Kernel Defaults
	 */

	// Kernel got edited to allow faster dynamic hotplugging, fast enough for me to
	// re-enable it. On huge screen-on-delays, disable this again.
	Utils::write("/sys/power/enable_dm_hotplug", true);

	// The power-efficient workqueue is useful for lower-power-situations, but
	// contraproductive in high-performance situations. This should reflect in
	// the static power-profiles
	Utils::write("/sys/module/workqueue/parameters/power_efficient", data->kernel.pewq);
}

void Power::resetProfile() {
	if (mRequestedProfile == SecPowerProfiles::INVALID) {
		setProfile(SecPowerProfiles::BALANCED);
	} else {
		setProfile(mRequestedProfile);
	}
}

void Power::setInputState(bool enabled) {
	if (enabled) {
		if (!mTouchControlPath.empty()) {
			Utils::write(mTouchControlPath, true);
		}

		if (mVariant != SecDeviceVariant::EDGE) {
			Utils::write(POWER_TOUCHKEYS_ENABLED, true);
			Utils::write(POWER_TOUCHKEYS_BRIGHTNESS, 255);
		}

		setDT2WState();
	} else {
		if (mVariant != SecDeviceVariant::EDGE) {
			// save to current state to prevent enabling
			Utils::read(POWER_TOUCHKEYS_ENABLED, mTouchkeysEnabled);

			// disable them
			Utils::write(POWER_TOUCHKEYS_ENABLED, false);
			Utils::write(POWER_TOUCHKEYS_BRIGHTNESS, 0);
		}

		// only disable touchscreen if we aren't using DT2W
		if (!mTouchControlPath.empty() && !mIsDT2WEnabled) {
			Utils::write(mTouchControlPath, false);
		}

		setDT2WState();
	}
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
		Utils::write(POWER_FINGERPRINT_WAKELOCKS, true);
		Utils::write(POWER_FINGERPRINT_PM, true);
	} else {
		Utils::write(POWER_FINGERPRINT_PM, false);
		Utils::write(POWER_FINGERPRINT_WAKELOCKS, false);
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
