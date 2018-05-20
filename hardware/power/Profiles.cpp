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
#include "Utils.h"

namespace android {
namespace hardware {
namespace power {
namespace V1_0 {
namespace implementation {
	
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

#define XML_GET(_path)                                                          \
	({                                                                          \
		char objPath[255];                                                      \
		strcpy(objPath, path);                                                  \
		strcat(objPath, _path);                                                 \
		xmlXPathObject *obj =                                                   \
			xmlXPathEvalExpression ((xmlChar *)objPath, ctx);                   \
		if (!obj->nodesetval->nodeTab) {                                        \
			LOG_FATAL("loadProfileImpl: missing XML node '%s%s'", path, _path); \
		}                                                                       \
		xmlNode *node = obj->nodesetval->nodeTab[0];                            \
		((char *)node->children->content);                                      \
	})

#define XML_GET_BOOL(_path) TO_BOOL(XML_GET(_path))
#define XML_GET_INT(_path) TO_INT(XML_GET(_path))
#define XML_GET_UINT(_path) TO_UINT(XML_GET(_path))

#define XML_GET_CPUGOV(_gov)                                                    \
	profile->cpu._gov.governor     = XML_GET     ("cpu/" #_gov "/governor");    \
	profile->cpu._gov.freq_min     = XML_GET_UINT("cpu/" #_gov "/freq_min");    \
	profile->cpu._gov.freq_max     = XML_GET_UINT("cpu/" #_gov "/freq_max");    \
	profile->cpu._gov.freq_hispeed = XML_GET_UINT("cpu/" #_gov "/freq_hispeed") \

#define XML_GET_CPUGOV_NEXUS(_gov)                                                                       \
	profile->cpu._gov.nexus.lpr_ratio          = XML_GET_UINT("cpu/" #_gov "/nexus/lpr_ratio");          \
	profile->cpu._gov.nexus.lpr_down_elevation = XML_GET_UINT("cpu/" #_gov "/nexus/lpr_down_elevation"); \
	profile->cpu._gov.nexus.lpr_up_elevation   = XML_GET_UINT("cpu/" #_gov "/nexus/lpr_up_elevation");   \
	profile->cpu._gov.nexus.hispeed_delay      = XML_GET_UINT("cpu/" #_gov "/nexus/hispeed_delay");      \
	profile->cpu._gov.nexus.hispeed_load       = XML_GET_UINT("cpu/" #_gov "/nexus/hispeed_load")

#define XML_GET_CPUGOV_INTERACTIVE(_gov)                                                                               \
	profile->cpu._gov.interactive.above_hispeed_delay = XML_GET     ("cpu/" #_gov "/interactive/above_hispeed_delay"); \
	profile->cpu._gov.interactive.go_hispeed_load     = XML_GET_UINT("cpu/" #_gov "/interactive/go_hispeed_load");     \
	profile->cpu._gov.interactive.min_sample_time     = XML_GET_UINT("cpu/" #_gov "/interactive/min_sample_time");     \
	profile->cpu._gov.interactive.target_loads        = XML_GET     ("cpu/" #_gov "/interactive/target_loads");        \
	profile->cpu._gov.interactive.timer_rate          = XML_GET_UINT("cpu/" #_gov "/interactive/timer_rate");          \
	profile->cpu._gov.interactive.timer_slack         = XML_GET_UINT("cpu/" #_gov "/interactive/timer_slack")

void Profiles::loadProfileImpl(SecPowerProfile *profile, xmlXPathContext *ctx, const char *path) {
	profile->enabled               = XML_GET_BOOL("enabled");
	profile->cpu.enabled           = XML_GET_BOOL("cpu/enabled");
	profile->cpu.apollo.enabled    = XML_GET_BOOL("cpu/apollo/enabled");
	profile->cpu.atlas.enabled     = XML_GET_BOOL("cpu/atlas/enabled");
	profile->hmp.enabled           = XML_GET_BOOL("hmp/enabled");
	profile->gpu.enabled           = XML_GET_BOOL("gpu/enabled");
	profile->gpu.dvfs.enabled      = XML_GET_BOOL("gpu/dvfs/enabled");
	profile->gpu.highspeed.enabled = XML_GET_BOOL("gpu/highspeed/enabled");
	profile->kernel.enabled        = XML_GET_BOOL("kernel/enabled");
	profile->ipa.enabled           = XML_GET_BOOL("ipa/enabled");

	if (!profile->enabled) {
		return;
	}

	if (profile->cpu.enabled) {
		if (profile->cpu.apollo.enabled) {
			XML_GET_CPUGOV(apollo);

			if (profile->cpu.apollo.governor == "nexus") {
				XML_GET_CPUGOV_NEXUS(apollo);
			} else if (profile->cpu.apollo.governor == "interactive") {
				XML_GET_CPUGOV_INTERACTIVE(apollo);
				profile->cpu.apollo.interactive.single_enter_load = XML_GET_UINT("cpu/apollo/interactive/single_enter_load");
				profile->cpu.apollo.interactive.multi_enter_load  = XML_GET_UINT("cpu/apollo/interactive/multi_enter_load");
			}
		}

		if (profile->cpu.atlas.enabled) {
			XML_GET_CPUGOV(atlas);

			if (profile->cpu.atlas.governor == "nexus") {
				XML_GET_CPUGOV_NEXUS(atlas);
			} else if (profile->cpu.atlas.governor == "interactive") {
				XML_GET_CPUGOV_INTERACTIVE(atlas);
				// single
				profile->cpu.atlas.interactive.single_cluster0_min_freq = XML_GET_UINT("cpu/atlas/interactive/single_cluster0_min_freq");
				profile->cpu.atlas.interactive.single_enter_load        = XML_GET_UINT("cpu/atlas/interactive/single_enter_load");
				profile->cpu.atlas.interactive.single_enter_time        = XML_GET_UINT("cpu/atlas/interactive/single_enter_time");
				profile->cpu.atlas.interactive.single_exit_load         = XML_GET_UINT("cpu/atlas/interactive/single_exit_load");
				profile->cpu.atlas.interactive.single_exit_time         = XML_GET_UINT("cpu/atlas/interactive/single_exit_time");
				// multi
				profile->cpu.atlas.interactive.multi_cluster0_min_freq = XML_GET_UINT("cpu/atlas/interactive/multi_cluster0_min_freq");
				profile->cpu.atlas.interactive.multi_enter_load        = XML_GET_UINT("cpu/atlas/interactive/multi_enter_load");
				profile->cpu.atlas.interactive.multi_enter_time        = XML_GET_UINT("cpu/atlas/interactive/multi_enter_time");
				profile->cpu.atlas.interactive.multi_exit_load         = XML_GET_UINT("cpu/atlas/interactive/multi_exit_load");
				profile->cpu.atlas.interactive.multi_exit_time         = XML_GET_UINT("cpu/atlas/interactive/multi_exit_time");
			}
		}
	}

	if (profile->hmp.enabled) {
		profile->hmp.boost     = XML_GET_BOOL("hmp/boost");
		profile->hmp.semiboost = XML_GET_BOOL("hmp/semiboost");
	}

	if (profile->gpu.enabled) {
		if (profile->gpu.dvfs.enabled) {
			profile->gpu.dvfs.freq_min = XML_GET_UINT("gpu/dvfs/freq_min");
			profile->gpu.dvfs.freq_max = XML_GET_UINT("gpu/dvfs/freq_max");
		}
		if (profile->gpu.highspeed.enabled) {
			profile->gpu.highspeed.freq = XML_GET_UINT("gpu/highspeed/freq");
			profile->gpu.highspeed.load = XML_GET_UINT("gpu/highspeed/load");
		}
	}

	if (profile->kernel.enabled) {
		profile->kernel.pewq = XML_GET_BOOL("kernel/pewq");
	}

	if (profile->ipa.enabled) {
		profile->ipa.control_temp = XML_GET_INT("ipa/control_temp");
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