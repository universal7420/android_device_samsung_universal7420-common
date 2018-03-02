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

#ifndef ZERO_HARDWARE_POWER_V1_0_UTILS_H
#define ZERO_HARDWARE_POWER_V1_0_UTILS_H

#include <map>
#include <string>

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {

using namespace ::std;

#define __stat_compare_mode(path, mode) \
	(statMode(path) & mode) == mode
	
#define __access(path, mode) \
	(!access(path.c_str(), mode))

#define __open_print_error() \
	ALOGE("%s: failed to open \"%s\": %s (%d)", __func__, path.c_str(), strerror(errno), errno)

#define __open_print_attempt_error() \
	ALOGE("%s: failed to open \"%s\": %s (%d) (attempt %d out of %d)", __func__, path.c_str(), strerror(errno), errno, attempt + 1, OPEN_MAX_ATTEMPTS)

#define __open_print_attempt_fail() \
	ALOGE("%s: failed to open \"%s\": %s (%d) after %d attempts", __func__, path.c_str(), strerror(errno), errno, OPEN_MAX_ATTEMPTS)

#define __open_print_attempt_success() \
	ALOGI("%s: succeeded to open \"%s\" after %d attempts", __func__, path.c_str(), attempt)

#ifdef STRICT_BEHAVIOUR

	#define FILE_TRY_OPEN(stream, path) \
	{ \
		stream.open(path); \
		if (!stream.is_open()) { \
			__open_print_error(); \
			return false; \
		} \
	}

	#define FILE_LEGACY_TRY_OPEN(path, mode) \
	{ \
		fd = open(path.c_str(), mode); \
		if (fd < 0) { \
			__open_print_error(); \
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
				__open_print_attempt_error(); \
				attempt++; \
				usleep(OPEN_ATTEMPT_DELAY * 1000); \
			} \
		} while (attempt < OPEN_MAX_ATTEMPTS && !stream.is_open()); \
	\
		if (!stream.is_open()) { \
			__open_print_attempt_fail(); \
			return false; \
		} else if (attempt != 0) { \
			__open_print_attempt_success(); \
		} \
	}

	#define FILE_LEGACY_TRY_OPEN(path, mode) \
	{ \
		int attempt = 0; \
	\
		do { \
			fd = open(path, mode); \
			if (fd < 0) { \
				__open_print_attempt_error(); \
				attempt++; \
				usleep(OPEN_ATTEMPT_DELAY * 1000); \
			} \
		} while (attempt < OPEN_MAX_ATTEMPTS && !stream.is_open()); \
	\
		if (fd < 0) { \
			__open_print_attempt_fail(); \
			return false; \
		} else if (attempt != 0) { \
			__open_print_attempt_success(); \
		} \
	}

	#define ASSERT_CPU_CORE(...)

#endif //STRICT_BEHAVIOUR

struct Utils {

	static bool isFile(const string path);
	static bool isDirectory(const string path);

	static bool canRead(const string path);
	static bool canWrite(const string path);
	static bool canExecute(const string path);

	static bool write(const string path, const string data);
	static bool write(const string path, const bool data) ;
	static bool write(const string path, const int data);
	static bool write(const string path, const unsigned int data);

	static bool writeLegacy(const string path, const string data);
	static bool writeLegacy(const string path, const bool data) ;
	static bool writeLegacy(const string path, const int data);
	static bool writeLegacy(const string path, const unsigned int data);

	static bool updateCpuGov(const int core);
	static bool assertCpuGov(const int core, const string cpugov);
	static string getCurrentCpuGov(const int core);

	static bool writeCpuGov(const int core, const string file, const string data);
	static bool writeCpuGov(const int core, const string file, const bool data);
	static bool writeCpuGov(const int core, const string file, const int data);
	static bool writeCpuGov(const int core, const string file, const unsigned int data);

	static bool read(const string path, string &data);
	static bool read(const string path, bool &data);
	static bool read(const string path, int &data);

private:

	// map<int core, string cpugov>
	static map<int, string> kCpuGovernors;

	static int statMode(const string path);

};

}  // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace hardware
}  // namespace android

#endif  // ZERO_HARDWARE_POWER_V1_0_UTILS_H
