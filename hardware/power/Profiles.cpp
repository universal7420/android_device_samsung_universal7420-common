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

#define LOG_TAG "zero-power-profiles"
#define LOG_NDEBUG 0

#include <log/log.h>

#include "Power.h"
#include "Profiles.h"
#include "../Utils.h"

#if defined(LOG_NDEBUG) && !LOG_NDEBUG
const bool LOG_DEBUG = true;
#else
const bool LOG_DEBUG = false;
#endif

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {

using ::vendor::nexus::zero::Utils;
	
SecPowerProfile Profiles::kPowerProfileScreenOff;
SecPowerProfile Profiles::kPowerProfilePowerSave;
SecPowerProfile Profiles::kPowerProfileBiasPowerSave;
SecPowerProfile Profiles::kPowerProfileBalanced;
SecPowerProfile Profiles::kPowerProfileBiasPerformance;
SecPowerProfile Profiles::kPowerProfileHighPerformance;

void Profiles::loadProfiles() {
	if (Utils::isFile(PROFILES_PATH_USER)) {
		loadProfilesImpl(PROFILES_PATH_USER);
	} else if (Utils::isFile(PROFILES_PATH_VENDOR)) {
		loadProfilesImpl(PROFILES_PATH_VENDOR);
	} else if (Utils::isFile(PROFILES_PATH_SYSTEM)) {
		loadProfilesImpl(PROFILES_PATH_SYSTEM);
	} else {
		LOG_FATAL("Could not find valid power profiles XML file");
	}
}

void Profiles::loadProfilesImpl(const char *path) {
	xmlInitParser();

	xmlDoc *doc = xmlParseFile(path);
	xmlXPathContext *ctx = xmlXPathNewContext(doc);

	loadProfileImpl(
		&kPowerProfileScreenOff,
		ctx,
		"/profiles/screen_off/");

	loadProfileImpl(
		&kPowerProfilePowerSave,
		ctx,
		"/profiles/power_save/");

	loadProfileImpl(
		&kPowerProfileBiasPowerSave,
		ctx,
		"/profiles/bias_power_save/");

	loadProfileImpl(
		&kPowerProfileBalanced,
		ctx,
		"/profiles/balanced/");

	loadProfileImpl(
		&kPowerProfileBiasPerformance,
		ctx,
		"/profiles/bias_performance/");

	loadProfileImpl(
		&kPowerProfileHighPerformance,
		ctx,
		"/profiles/performance/");
}

#define TO_BOOL(_c) ({ const char *str = _c; (!strcmp(str, "true") || !strcmp(str, "TRUE") || !strcmp(str, "1")); })
#define TO_INT(_c) (std::stoi(_c))
#define TO_UINT(_c) ((unsigned int)std::stoi(_c))

#define XML_GET(_path, _default)                                          \
	({                                                                    \
		char objPath[255];                                                \
		const char *result = NULL;                                        \
		strcpy(objPath, path);                                            \
		strcat(objPath, _path);                                           \
		xmlXPathObject *obj =                                             \
			xmlXPathEvalExpression ((xmlChar *)objPath, ctx);             \
		if (!obj->nodesetval->nodeTab ||                                  \
			!obj->nodesetval->nodeTab[0] ||                               \
			!obj->nodesetval->nodeTab[0]->children ||                     \
			!obj->nodesetval->nodeTab[0]->children->content) {            \
			ALOGE("loadProfileImpl: missing XML node '%s'"                \
				", using default '%s'", objPath, _default);               \
			result = (const char *)_default;                              \
		} else {                                                          \
			xmlNode *node = obj->nodesetval->nodeTab[0];                  \
			result = (const char *)node->children->content;               \
			if (LOG_DEBUG) ALOGV("Loaded XML node '%s': \"%s\"",          \
				objPath, result);                                         \
		}                                                                 \
		result;                                                           \
	})

#define XML_GET_BOOL(_path, _default) TO_BOOL(XML_GET(_path, _default))
#define XML_GET_INT(_path, _default) TO_INT(XML_GET(_path, _default))
#define XML_GET_UINT(_path, _default) TO_UINT(XML_GET(_path, _default))

#define XML_GET_CPUCLUSTER(_cl, _cln)                                                    \
	profile->cpu._cl.governor     = XML_GET     ("cpu/" #_cl "/governor",     "nexus");  \
	profile->cpu._cl.freq_min     = XML_GET_UINT("cpu/" #_cl "/freq_min",     "200000"); \
	profile->cpu._cl.freq_max     = XML_GET_UINT("cpu/" #_cl "/freq_max",                \
		(_cln == 0 ? "1500000" : "2100000"));                                            \
	profile->cpu._cl.freq_hispeed = XML_GET_UINT("cpu/" #_cl "/freq_hispeed",            \
		(_cln == 0 ? "1500000" : "2100000"));                                            \
	profile->cpu._cl.freq_boost   = XML_GET_UINT("cpu/" #_cl "/freq_boost",   "0");      \
	if (profile->cpu._cl.cores.enabled) {                                                \
		profile->cpu._cl.cores.core1 = XML_GET_BOOL("cpu/" #_cl "/cores/core1", "true"); \
		profile->cpu._cl.cores.core2 = XML_GET_BOOL("cpu/" #_cl "/cores/core2", "true"); \
		profile->cpu._cl.cores.core3 = XML_GET_BOOL("cpu/" #_cl "/cores/core3", "true"); \
		profile->cpu._cl.cores.core4 = XML_GET_BOOL("cpu/" #_cl "/cores/core4", "true"); \
	}

#define XML_GET_CPUGOV(_gov)                                            \
	{                                                                   \
		char objPath[255];                                              \
		strcpy(objPath, path);                                          \
		strcat(objPath, "cpu/" #_gov "/governor_data");                 \
		xmlXPathObject *obj =                                           \
			xmlXPathEvalExpression ((xmlChar *)objPath, ctx);           \
		if (!obj->nodesetval->nodeTab ||                                \
			!obj->nodesetval->nodeTab[0] ||                             \
			!obj->nodesetval->nodeTab[0]->children) {                   \
			ALOGE("loadProfileImpl: missing XML node '%s'", objPath);   \
		} else {                                                        \
			xmlNode *node = obj->nodesetval->nodeTab[0]->children;      \
			int index = 0;                                              \
			do {                                                        \
				if (node->children) {                                   \
					strcpy(profile->cpu._gov.governor_data[index].name, \
						(const char *)node->name);                      \
					strcpy(profile->cpu._gov.governor_data[index].data, \
						(const char *)node->children->content);         \
					index++;                                            \
				}                                                       \
				node = node->next;                                      \
			} while (node);                                             \
			profile->cpu._gov.governor_data[index].name[0] = 0;         \
			profile->cpu._gov.governor_data[index].data[0] = 0;         \
		}                                                               \
	}

void Profiles::loadProfileImpl(SecPowerProfile *profile, xmlXPathContext *ctx, const char *path) {
	profile->enabled                  = XML_GET_BOOL("enabled", "false");
	profile->cpu.enabled              = XML_GET_BOOL("cpu/enabled", "false");
	profile->cpu.apollo.enabled       = XML_GET_BOOL("cpu/apollo/enabled", "false");
	profile->cpu.apollo.cores.enabled = XML_GET_BOOL("cpu/apollo/cores/enabled", "false");
	profile->cpu.atlas.enabled        = XML_GET_BOOL("cpu/atlas/enabled", "false");
	profile->cpu.atlas.cores.enabled  = XML_GET_BOOL("cpu/atlas/cores/enabled", "false");
	profile->cpusets.enabled          = XML_GET_BOOL("cpusets/enabled", "false");
	profile->hmp.enabled              = XML_GET_BOOL("hmp/enabled", "false");
	profile->gpu.enabled              = XML_GET_BOOL("gpu/enabled", "false");
	profile->gpu.dvfs.enabled         = XML_GET_BOOL("gpu/dvfs/enabled", "false");
	profile->gpu.highspeed.enabled    = XML_GET_BOOL("gpu/highspeed/enabled", "false");
	profile->kernel.enabled           = XML_GET_BOOL("kernel/enabled", "false");
	profile->ipa.enabled              = XML_GET_BOOL("ipa/enabled", "false");
	profile->slow.enabled             = XML_GET_BOOL("slow/enabled", "false");
	profile->input_booster.enabled    = XML_GET_BOOL("input_booster/enabled", "false");

	if (!profile->enabled) {
		return;
	}

	if (profile->cpu.enabled) {
		if (profile->cpu.apollo.enabled) {
			XML_GET_CPUCLUSTER(apollo, 0);
			XML_GET_CPUGOV(apollo);
		}

		if (profile->cpu.atlas.enabled) {
			XML_GET_CPUCLUSTER(atlas, 1);
			XML_GET_CPUGOV(atlas);
		}
	}

	if (profile->cpusets.enabled) {
		profile->cpusets.defaults          = XML_GET("cpusets/default", "0-7");
		profile->cpusets.foreground        = XML_GET("cpusets/foreground", "0-7");
		profile->cpusets.foreground_boost  = XML_GET("cpusets/foreground_boost", "0-7");
		profile->cpusets.background        = XML_GET("cpusets/background", "0-7");
		profile->cpusets.system_background = XML_GET("cpusets/system_background", "0-7");
		profile->cpusets.top_app           = XML_GET("cpusets/top_app", "0-7");
	}

	if (profile->hmp.enabled) {
		profile->hmp.boost                   = XML_GET_BOOL("hmp/boost", "false");
		profile->hmp.semiboost               = XML_GET_BOOL("hmp/semiboost", "false");
		profile->hmp.power_migration         = XML_GET_BOOL("hmp/power_migration", "false");
		profile->hmp.active_down_migration   = XML_GET_BOOL("hmp/active_down_migration", "false");
		profile->hmp.aggressive_up_migration = XML_GET_BOOL("hmp/aggressive_up_migration", "false");
	}

	if (profile->gpu.enabled) {
		if (profile->gpu.dvfs.enabled) {
			profile->gpu.dvfs.freq_min = XML_GET_UINT("gpu/dvfs/freq_min", "100");
			profile->gpu.dvfs.freq_max = XML_GET_UINT("gpu/dvfs/freq_max", "772");
		}
		if (profile->gpu.highspeed.enabled) {
			profile->gpu.highspeed.freq = XML_GET_UINT("gpu/highspeed/freq", "772");
			profile->gpu.highspeed.load = XML_GET_UINT("gpu/highspeed/load", "99");
		}
	}

	if (profile->kernel.enabled) {
		profile->kernel.pewq = XML_GET_BOOL("kernel/pewq", "false");
	}

	if (profile->ipa.enabled) {
		profile->ipa.control_temp = XML_GET_INT("ipa/control_temp", "65");
	}

	if (profile->slow.enabled) {
		profile->slow.mode_toggle = XML_GET_BOOL("slow/mode_toggle", "false");
		profile->slow.timer_rate = XML_GET_INT("slow/timer_rate", "0");
	}

	if (profile->input_booster.enabled) {
		profile->input_booster.tail = XML_GET("input_booster/tail", "0 0 0 0 0 0");
		profile->input_booster.head = XML_GET("input_booster/head", "0 0 0 0 0 0");
	}
}

const SecPowerProfile* Profiles::getProfileData(SecPowerProfiles profile) {
	switch_uint32_t (profile)
	{
		case_uint32_t (SecPowerProfiles::SCREEN_OFF):
		{
			ALOGV("%s: returning kPowerProfileScreenOff", __func__);
			return &kPowerProfileScreenOff;
		}
		case_uint32_t (SecPowerProfiles::POWER_SAVE):
		{
			ALOGV("%s: returning kPowerProfilePowerSave", __func__);
			return &kPowerProfilePowerSave;
		}
		case_uint32_t (SecPowerProfiles::BIAS_POWER_SAVE):
		{
			ALOGV("%s: returning kPowerProfileBiasPowerSave", __func__);
			return &kPowerProfileBiasPowerSave;
		}
		case_uint32_t (SecPowerProfiles::BALANCED):
		{
			ALOGV("%s: returning kPowerProfileBalanced", __func__);
			return &kPowerProfileBalanced;
		}
		case_uint32_t (SecPowerProfiles::BIAS_PERFORMANCE):
		{
			ALOGV("%s: returning kPowerProfileBiasPerformance", __func__);
			return &kPowerProfileBiasPerformance;
		}
		case_uint32_t (SecPowerProfiles::HIGH_PERFORMANCE):
		{
			ALOGV("%s: returning kPowerProfileHighPerformance", __func__);
			return &kPowerProfileHighPerformance;
		}
	}
	return nullptr;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace power
}  // namespace hardware
}  // namespace android