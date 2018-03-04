/*
 * Copyright (C) 2018 The LineageOS Project
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

#define LOG_TAG "SecHAL-shim"
//#define LOG_NDEBUG 0

#include <utils/Log.h>

#define NFC_VER_IMAGE  0
#define NFC_VER_FW     1

#define NFC_VER_MAJOR  0
#define NFC_VER_MINOR  1
#define NFC_VER_BUILD  2

#define NFC_RUNNING_FW_PATH  "/dev/nfc/running-fw"

#define nfc_get_ver(_type, _offs) (((char *)ver_data)[(_type * 3) + _offs])

#define nfc_def_ver(_name, _type) \
	char _name##_major = nfc_get_ver(_type, NFC_VER_MAJOR), \
	     _name##_minor = nfc_get_ver(_type, NFC_VER_MINOR), \
	     _name##_build = nfc_get_ver(_type, NFC_VER_BUILD)

extern "C"
{

static int nfc_get_curr_running_fw(char *major, char *minor, char *build) {
	int fd = open(NFC_RUNNING_FW_PATH, O_RDONLY);
	int lread = 0;
	char data[3] = { 0 /* major */, 0 /* minor */, 0 /* build */ };

	if (fd < 0) {
		ALOGE("%s: failed to open: %s (%d)", __func__, strerror(errno), errno);
		return fd;
	}

	lread = read(fd, data, 3);
	if (lread != 3) {
		ALOGE("%s: failed to read: %s (%d)", __func__, strerror(errno), errno);
	}
	
	close(fd);

	if (lread != 3)
		return -EINVAL;

	ALOGI("NFC Running F/W Version: %02x.%02x.%02x", data[0], data[1], data[2]);
	*minor = data[0];
	*major = data[1];
	*build = data[2];

	return 0;
}

static int nfc_set_curr_running_fw(char major, char minor, char build) {
	int fd = open(NFC_RUNNING_FW_PATH, O_WRONLY | O_CREAT, 0600);
	int lwritten = 0;
	char data[3] = { major, minor, build };

	if (fd < 0) {
		ALOGE("%s: failed to open: %s (%d)", __func__, strerror(errno), errno);
		return fd;
	}

	lwritten = write(fd, data, 3);
	if (lwritten != 3) {
		ALOGE("%s: failed to write: %s (%d)", __func__, strerror(errno), errno);
	}

	close(fd);

	if (lwritten != 3)
		return -EINVAL;

	return 0;
}

int nfc_fw_force_update_check(int *ver_data) {
	nfc_def_ver(image, NFC_VER_IMAGE);
	nfc_def_ver(fw, NFC_VER_FW);
	char curr_major, curr_minor, curr_build;

	ALOGI("NFC Image Version: %02x.%02x.%02x", image_major, image_minor, image_build);
	ALOGI("NFC F/W Version:   %02x.%02x.%02x", fw_major, fw_minor, fw_build);
	
	if (!nfc_get_curr_running_fw(&curr_major, &curr_minor, &curr_build)) {
		return !((image_major == curr_major) && (image_minor == curr_minor) && (image_build == curr_build));
	} else {
		nfc_set_curr_running_fw(image_major, image_minor, image_build);
	}

	// force-update for now
	return 1;
}

}