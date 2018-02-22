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

#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#ifndef EXYNOS5_POWER_HAL_UTILS_H_INCLUDED
#define EXYNOS5_POWER_HAL_UTILS_H_INCLUDED

#define OPEN_MAX_ATTEMPTS		5
#define OPEN_ATTEMPT_DELAY		50

#ifdef STRICT_BEHAVIOUR

	#define FILE_TRY_OPEN(stream, path) \
	{ \
		stream.open(path); \
		if (!stream.is_open()) { \
			ALOGE("%s: failed to open \"%s\"", __func__, path.c_str()); \
			return false; \
		} \
	}

	#define ASSERT_CPU_CORE() \
	{ \
		if (core < 0 || core >= NR_CPUS) { \
			ALOGE("%s: passed core (%d) not valid", __func__, core); \
			return false; \
		} \
	}

#else

	#define FILE_TRY_OPEN(stream, path) \
	{ \
		int attempt = 0; \
	\
		do { \
			stream.open(path); \
			if (!stream.is_open()) { \
				ALOGE("%s: failed to open \"%s\" (attempt %d out of %d)", __func__, path.c_str(), attempt + 1, OPEN_MAX_ATTEMPTS); \
				attempt++; \
				usleep(OPEN_ATTEMPT_DELAY * 1000); \
			} \
		} while (attempt < OPEN_MAX_ATTEMPTS && !stream.is_open()); \
	\
		if (!stream.is_open()) { \
			ALOGE("%s: failed to open \"%s\" after %d attempts", __func__, path.c_str(), OPEN_MAX_ATTEMPTS); \
			return false; \
		} else if (attempt != 0) { \
			ALOGI("%s: succeeded to open \"%s\" after %d attempts", __func__, path.c_str(), attempt); \
		} \
	}

	#define ASSERT_CPU_CORE(...)

#endif //STRICT_BEHAVIOUR

/** Common */
bool update_current_cpugov_path(const int core);
bool assert_cpugov(const int core, const string asserted_cpugov);
bool assert_cpugov_file(const int core, const string file);
bool is_file(const string path);
bool is_dir(const string path);

/** Writing */
bool write(const string path, const string str);
bool write(const string path, const bool flag);
bool write(const string path, const int value);
bool write(const string path, const unsigned int value);

/** Writing cpugovs */
bool write_cpugov(const int core, const string file, const string str);
bool write_cpugov(const int core, const string file, const bool flag);
bool write_cpugov(const int core, const string file, const int value);
bool write_cpugov(const int core, const string file, const unsigned int value);

/** Reading */
bool read(const string path, string &str);
bool read(const string path, bool *f);
bool read(const string path, int *v);

#endif // EXYNOS5_POWER_HAL_UTILS_H_INCLUDED
