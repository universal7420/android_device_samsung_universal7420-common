/*
 * Copyright (C) 2018 TeamNexus
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

#define LOG_TAG "libinit_sec"

#include <sys/system_properties.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <log/log.h>

using namespace std;

enum device_variant {

    UNKNOWN = -1,

	/* Flat */
    G920F,
    G920I,
    G920K,
    G920L,
    G920P,
    G920S,
    G920T,
    G920W8,

	/* Edge */
    G925F,
    G925I,
    G925J,
    G925K,
    G925L,
    G925P,
    G925S,
    G925T,
    G925W8,
    
    	/* Edge Plus */	
    G928C,	
    G928F,	
    G928G,	
    G928I,	
    G928K,	
    G928L,	
    G928S,	
    G928T,	
    G928W8,	

        /* Note 5 */	
    N9208,	
    N920C,	
    N920G,	
    N920I,
    N920P,
    N920S,	
    N920K,	
    N920L,	
    N920T,	
    N920W8
	
};

inline bool replace(string& str, const string& from, const string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

inline void property_override(const char prop[], const string value)
{
    prop_info *pi;
    const char* c_value = value.c_str();

	ALOGI("init_sec: overriding '%s' with '%s'", prop, c_value);

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, c_value, strlen(c_value));
    else
        __system_property_add(prop, strlen(prop), c_value, strlen(c_value));
}
