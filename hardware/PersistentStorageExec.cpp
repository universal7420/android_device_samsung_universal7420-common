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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/PersistentStorage.h"

using ::std::string;
using ::vendor::nexus::zero::PersistentStorage;

static int usage() {
	printf("Zero Persistent Storage\n");
	printf("Usage:\n");
	printf("  zerops get [Key]\n");
	printf("  zerops set [Key] [Value]\n");
	return -1;
}

int main(int argc, char *argv[]) {
	if (argc == 3 && !strncmp(argv[1], "get", 3)) {
		string result;
		if (PersistentStorage::getString(argv[2], result)) {
			printf("\"%s\" = \"%s\"\n", argv[2], result.c_str());
		} else {
			printf("Could not find \"%s\"\n", argv[2]);
		}
	} else if (argc == 4 && !strncmp(argv[1], "set", 3)) {
		if (PersistentStorage::putString(argv[2], argv[3])) {
			printf("\"%s\" = \"%s\"\n", argv[2], argv[3]);
		} else {
			printf("Failed to set \"%s\"\n", argv[2]);
		}
	} else {
		return usage();
	}
}
