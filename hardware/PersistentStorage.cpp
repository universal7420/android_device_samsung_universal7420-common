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

#define LOG_TAG "zero-persistent-storage"
#define LOG_NDEBUG 0

#include <log/log.h>
#include <openssl/sha.h>

#include "../include/PersistentStorage.h"
#include "Utils.h"

namespace vendor {
namespace nexus {
namespace zero {

using std::string;
	
#define ZERO_PERSISTENT_STORAGE_PATH    "/persdata/absolute/"
#define ZERO_PERSISTENT_STORAGE_PREFIX  ""

bool PersistentStorage::available() {
	return Utils::isDirectory(ZERO_PERSISTENT_STORAGE_PATH) &&
		Utils::canRead(ZERO_PERSISTENT_STORAGE_PATH) &&
		Utils::canWrite(ZERO_PERSISTENT_STORAGE_PATH);
}

bool PersistentStorage::getString(const string &key, string &data) {
	string keyPath = ZERO_PERSISTENT_STORAGE_PATH + encodeKey(std::string(key));
	string result;

	if (!Utils::read(keyPath, result)) {
		return false;
	}

	data = result;
	return true;
}

bool PersistentStorage::getCString(const char *key, char **data) {
	string result;

	if (!getString(key, result)) {
		return false;
	}

	result.copy(*data, result.length(), 0);
	return true;
}

bool PersistentStorage::getInt(const string &key, int &data) {
	string result;

	if (!getString(key, result)) {
		return false;
	}

	data = std::stoi(result);
	return true;
}

bool PersistentStorage::getBool(const string &key, bool &data) {
	string result;

	if (!getString(key, result)) {
		return false;
	}

	data = (result != "0");
	return true;
}

bool PersistentStorage::putString(const string &key, const string &data) {
	string keyPath = ZERO_PERSISTENT_STORAGE_PATH + encodeKey(std::string(key));
	return Utils::write(keyPath, data);
}

bool PersistentStorage::putCString(const char *key, const char *data) {
	return putString(key, std::string(data));
}

bool PersistentStorage::putInt(const string &key, int data) {
	return putString(key, std::to_string(data));
}

bool PersistentStorage::putBool(const string &key, bool data) {
	return putString(key, data ? "1" : "0");
}

string PersistentStorage::encodeKey(const string &key) {
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256((const uint8_t *)key.c_str(), (size_t)key.length(), hash);
	return std::string(ZERO_PERSISTENT_STORAGE_PREFIX) +
		hashToHexString(std::string((char *)hash, SHA256_DIGEST_LENGTH));
}

string PersistentStorage::hashToHexString(const string &input)
{
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();

    string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

}  // namespace zero
}  // namespace nexus
}  // namespace vendor
