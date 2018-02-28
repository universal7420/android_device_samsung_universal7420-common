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

#include <hardware/hardware.h>
#include <hardware/power.h>

#include <map>

using namespace std;

#ifndef EXYNOS5_POWER_HAL_POWER_H_INCLUDED
#define EXYNOS5_POWER_HAL_POWER_H_INCLUDED

#define POWER_DT2W_ENABLED				"/sys/android_touch/doubletap2wake"
#define POWER_TOUCHKEYS_ENABLED		   "/sys/class/sec/sec_touchkey/input/enabled"
#define POWER_TOUCHSCREEN_NAME			"/sys/class/input/input1/name"
#define POWER_TOUCHSCREEN_NAME_EXPECT	 "sec_touchscreen"
#define POWER_TOUCHSCREEN_ENABLED_FLAT	"/sys/class/input/input1/enabled"
#define POWER_TOUCHSCREEN_ENABLED_EDGE	"/sys/class/input/input0/enabled"
#define POWER_TOUCHKEYS_BRIGHTNESS		"/sys/class/sec/sec_touchkey/brightness"
#define POWER_FINGERPRINT_REGULATOR	   "/sys/class/fingerprint/fingerprint/regulator"
#define POWER_FINGERPRINT_PM			  "/sys/class/fingerprint/fingerprint/pm"
#define POWER_FINGERPRINT_WAKELOCKS	   "/sys/class/fingerprint/fingerprint/wakelocks"

#define POWER_DEFAULT_BOOSTPULSE	50000

#ifdef LOCK_PROTECTION
  #define POWER_LOCK()	  pthread_mutex_lock(&power->lock)
  #define POWER_UNLOCK()	pthread_mutex_unlock(&power->lock)
#else
  #define POWER_LOCK(...)
  #define POWER_UNLOCK(...)
#endif // LOCK_PROTECTION

#define __CPU_ONLINE(core, state) \
{ \
	ostringstream cos_path; \
	cos_path << "/sys/devices/system/cpu/cpu" << core << "/online"; \
	write(cos_path.str().c_str(), !!state); \
}

#define CPU_OFFLINE(core)  __CPU_ONLINE(core, 0)
#define CPU_ONLINE(core)  __CPU_ONLINE(core, 1)

#define __CPU_ONLINE_ALL(state) \
{ \
	for (int __coai = 0; __coai < NR_CPUS; __coai++) \
		__CPU_ONLINE(__coai, state) \
}

#define CPU_OFFLINE_ALL()  __CPU_ONLINE_ALL(0)
#define CPU_ONLINE_ALL()  __CPU_ONLINE_ALL(1)

/*
 * Write cpugov-independent settings
 */
// single-argument macro, var == filename
#define __cpu_cluster_write(core, cluster, var)  write_cpugov(core, #var, data->cpu.cluster.var)
#define cpu_apollo_write(var)  __cpu_cluster_write(0, apollo, var)
#define cpu_atlas_write(var)  __cpu_cluster_write(4, atlas, var)

// double-argument macro, var != filename
#define __cpu_cluster_write2(core, cluster, var, file)  write_cpugov(core, file, data->cpu.cluster.var)
#define cpu_apollo_write2(var, file)  __cpu_cluster_write2(0, apollo, var, file)
#define cpu_atlas_write2(var, file)  __cpu_cluster_write2(4, atlas, var, file)

/*
 * Write cpugov-specific settings
 */
// single-argument macro, var == filename
#define __cpugov_cluster_write(cpugov, core, cluster, var)  write_cpugov(core, #var, data->cpu.cluster.cpugov.var)
#define cpugov_apollo_write(cpugov, var)  __cpugov_cluster_write(cpugov, 0, apollo, var)
#define cpugov_atlas_write(cpugov, var)  __cpugov_cluster_write(cpugov, 4, atlas, var)

// double-argument macro, var != filename
#define __cpugov_cluster_write2(cpugov, core, cluster, var, file)  write_cpugov(core, file, data->cpu.cluster.cpugov.var)
#define cpugov_apollo_write2(cpugov, var, file)  __cpugov_cluster_write2(cpugov, 0, apollo, var, file)
#define cpugov_atlas_write2(cpugov, var, file)  __cpugov_cluster_write2(cpugov, 4, atlas, var, file)

/*
 * Assert cpugovs
 */
#define __if_cluster_cpugov(core, gov)  if (assert_cpugov(core, gov))
#define if_apollo_cpugov(gov)  __if_cluster_cpugov(0, gov)
#define if_atlas_cpugov(gov)  __if_cluster_cpugov(4, gov)

enum class SecPowerProfiles : int32_t {
	INVALID          = (-0x7FFFFFFF - 1),
	SCREEN_OFF       = -1,
	POWER_SAVE       = 0,
	BALANCED         = 1,
	HIGH_PERFORMANCE = 2,
	BIAS_POWER_SAVE  = 3,
	BIAS_PERFORMANCE = 4,
	MAX_PROFILES     = 5,
	MAX              = 6
};

enum class SecDeviceVariant : int32_t {
	FLAT = 0,
	EDGE = 1,
};

struct SecPowerModule {

#ifdef LOCK_PROTECTION
	pthread_mutex_t lock;
#endif

	bool initialized;
	bool screen_on;

	SecDeviceVariant variant;

	struct {
		SecPowerProfiles current;
		SecPowerProfiles requested;
	} profile;

	struct {
		bool touchkeys_enabled;
		bool dt2w;
		string touchscreen_control_path;
	} input;

};

/** Initializing */
void power_init(SecPowerModule *module);

/** Hinting */
void power_hint(SecPowerModule *module, power_hint_t hint, int32_t data);

/** Profiles */
static void power_set_profile(SecPowerModule *module, SecPowerProfiles profile);
static void power_reset_profile(SecPowerModule *module);

/** Inputs */
static void power_fingerprint_state(SecPowerModule *power, bool state);
static void power_dt2w_state(SecPowerModule *module, bool state);
static void power_input_device_state(SecPowerModule *module, bool state);
void power_set_interactive(SecPowerModule *module, bool interactive);

/** Features */
int power_get_feature(SecPowerModule *module, feature_t feature);
void power_set_feature(SecPowerModule *module, feature_t feature, int data);

#endif // EXYNOS5_POWER_HAL_POWER_H_INCLUDED
