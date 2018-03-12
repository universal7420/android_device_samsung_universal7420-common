/*
 * Copyright (C) 2018 TeamNexus
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ZERO_HELPERS_H
#define ZERO_HELPERS_H

#include <cutils/properties.h>

#ifndef __maybe_unused
  #define __maybe_unused  __attribute__((unused))
#endif

enum zero_model {
	UNDEFINED = -1,
	DEFAULT = 0,    // F and other non-specified
	INDIA = 1,      // I
	TMOBILE = 2,    // T
	CANADA = 3      // W8
};

static enum zero_model __zero_model = UNDEFINED;

static enum zero_model zero_model_read() {
	char bootloader[PROPERTY_VALUE_MAX];
	int bootloader_len = 0;

	if (__zero_model != UNDEFINED) {
		ALOGI("%s: use cached model: %d", __func__, __zero_model);
		return __zero_model;
	}

	property_get("ro.bootloader", bootloader, "");
	bootloader_len = strlen(bootloader);

	if (bootloader_len <= 0 || bootloader_len >= PROPERTY_VALUE_MAX) {
		__zero_model = UNDEFINED;
		goto exit;
	}

	if (!strncmp(bootloader, "G920I", 5) || !strncmp(bootloader, "G925I", 5)) {
		__zero_model = INDIA;
	}
	else if (!strncmp(bootloader, "G920T", 5) || !strncmp(bootloader, "G925T", 5)) {
		__zero_model = TMOBILE;
	}
	else if (!strncmp(bootloader, "G920W8", 5) || !strncmp(bootloader, "G925W8", 5)) {
		__zero_model = CANADA;
	}
	else { // if (!strncmp(bootloader, "G920F", 5) || !strncmp(bootloader, "G925F", 5))
		__zero_model = DEFAULT;
	}

exit:
	ALOGI("%s: determined model: %d", __func__, __zero_model);
	return __zero_model;
}

#endif // ZERO_HELPERS_H
