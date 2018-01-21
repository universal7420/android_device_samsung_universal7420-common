/*
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
#ifndef EXYNOS5_POWER_COMMON_HAL_H_INCLUDED
#define EXYNOS5_POWER_COMMON_HAL_H_INCLUDED

#define PROFILE_SCREEN_OFF            -2
#define PROFILE_DREAMING_OR_DOZING    -1
#define PROFILE_POWER_SAVE            0
#define PROFILE_BALANCED              1
#define PROFILE_HIGH_PERFORMANCE      2
#define PROFILE_BIAS_POWER_SAVE       3
#define PROFILE_BIAS_PERFORMANCE      4
#define PROFILE_MAX_USABLE            5

#define INPUT_STATE_DISABLE    0
#define INPUT_STATE_ENABLE     1

#define POWER_CONFIG_DT2W            "/data/power/dt2w"
#define POWER_CONFIG_PROFILES        "/data/power/profiles"
#define POWER_CONFIG_BOOST           "/data/power/boost"
#define POWER_CONFIG_FP_ALWAYS_ON    "/data/power/fp_always_on"
#define POWER_CONFIG_FP_WAKELOCKS    "/data/power/fp_wakelocks"

#define POWER_DT2W_ENABLED            "/sys/android_touch/doubletap2wake"
#define POWER_TOUCHKEYS_ENABLED       "/sys/class/input/input0/enabled"
#define POWER_TOUCHSCREEN_ENABLED     "/sys/class/input/input1/enabled"
#define POWER_TOUCHKEYS_BRIGTHNESS    "/sys/class/sec/sec_touchkey/brightness"

#define POWER_FINGERPRINT_REGULATOR    "/sys/class/fingerprint/fingerprint/regulator"
#define POWER_FINGERPRINT_WAKELOCKS    "/sys/class/fingerprint/fingerprint/wakelocks"

#endif // EXYNOS5_POWER_COMMON_HAL_H_INCLUDED