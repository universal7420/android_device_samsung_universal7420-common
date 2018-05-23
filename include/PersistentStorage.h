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

#ifndef VENDOR_NEXUS_ZERO_PERSISTENT_STORAGE_H
#define VENDOR_NEXUS_ZERO_PERSISTENT_STORAGE_H

#include <string>

namespace vendor {
namespace nexus {
namespace zero {

struct PersistentStorage {

public:
	static bool available();

	static bool getString(const std::string &key, std::string &data);
	static bool getCString(const char *key, char **data);
	static bool getInt(const std::string &key, int &data);
	static bool getBool(const std::string &key, bool &data);

	static bool putString(const std::string &key, const std::string &data);
	static bool putCString(const char *key, const char *data);
	static bool putInt(const std::string &key, int data);
	static bool putBool(const std::string &key, bool data);

private:
	static std::string encodeKey(const std::string &key);
	static std::string hashToHexString(const std::string &input);

};

}  // namespace zero
}  // namespace nexus
}  // namespace vendor

#endif  // VENDOR_NEXUS_ZERO_PERSISTENT_STORAGE_H
