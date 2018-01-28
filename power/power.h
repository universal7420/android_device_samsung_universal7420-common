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
#include <hardware/hardware.h>
#include <hardware/power.h>

#include "power-defs.h"

using namespace std;

#ifndef EXYNOS5_POWER_HAL_H_INCLUDED
#define EXYNOS5_POWER_HAL_H_INCLUDED

struct sec_power_module {

	struct power_module base;
	pthread_mutex_t lock;

	bool initialized;

	struct {
		int current;
		int requested;
	} profile;

	struct {
		bool touchkeys_enabled;
		bool dt2w;
		string touchscreen_control_path;
	} input;

};

/***********************************
 * Initializing
 */
static int power_open(const hw_module_t __unused * module, const char *name, hw_device_t **device);
static void power_init(struct power_module __unused * module);

/***********************************
 * Hinting
 */
static void power_hint(struct power_module *module, power_hint_t hint, void *data);

/***********************************
 * Profiles
 */
static void power_set_profile(struct sec_power_module *sec, int profile);

/***********************************
 * Boost
 */
#ifdef POWER_HAS_LINEAGE_HINTS
static void power_boostpulse(int duration);
#endif

/***********************************
 * Inputs
 */
static void power_fingerprint_state(bool state);
static void power_dt2w_state(struct sec_power_module *sec, bool state);
static void power_input_device_state(struct sec_power_module *sec, int state);
static void power_set_interactive(struct power_module *module, int on);

/***********************************
 * Features
 */
#ifdef POWER_HAS_LINEAGE_HINTS
static int power_get_feature(struct power_module *module __unused, feature_t feature);
#endif
static void power_set_feature(struct power_module *module, feature_t feature, int state);

/***********************************
 * Utilities
 */
// C++ I/O
static bool pfwrite(string path, string str);
static bool pfwrite(string path, bool flag);
static bool pfwrite(string path, int value);
static bool pfwrite(string path, unsigned int value);
static bool pfassertgov(int core, string asserted_cpugov);
static bool pfwritegov(int core, string file, string str);
static bool pfwritegov(int core, string file, bool flag);
static bool pfwritegov(int core, string file, int value);
static bool pfwritegov(int core, string file, unsigned int value);
static bool pfread(string path, string &str);
static bool pfread(string path, bool *f);
static bool pfread(string path, int *v);

// legacy I/O
static bool pfwrite_legacy(string path, string str);
static bool pfwrite_legacy(string path, int value);
static bool pfwrite_legacy(string path, bool flag);

// I/O-helpers
static bool is_file(string path);

#endif // EXYNOS5_POWER_HAL_H_INCLUDED
