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

#define LOG_TAG "sec-nfc"
#define LOG_NDEBUG 0
#define LOG_VDEBUG

#include <cutils/log.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hardware/nfc.h>
#include <hardware/nfc_tag.h>

#include "hal.h"

#ifdef LOG_VDEBUG
	#define ALOGVV ALOGD
#else
	#define ALOGVV(...) (void);
#endif

#define SEC_NFC_WRITE_DUMP_LEN  24
#define SEC_NFC_WRITE_DUMP_BUF_SIZE  (SEC_NFC_WRITE_DUMP_LEN * 3)

static union {
	const nfc_nci_module_t *module;
	const hw_module_t *hw_module;
} nfc_nci_vendor;

const sec_nfc_nci_device_t *current_sec_dev;

static int sec_nfc_open(
        const struct nfc_nci_device* dev,
        nfc_stack_callback_t* p_cback,
        nfc_stack_data_callback_t* p_data_cback)
{
	const sec_nfc_nci_device_t *sec_dev = (sec_nfc_nci_device_t *)dev;
	int ret = 0;

	ALOGV("%s(%p): enter(%p, %p)", __FUNCTION__, sec_dev, p_cback, p_data_cback);

	if (current_sec_dev) {
		ALOGE("%s(%p): cannot open device with current SEC NFC device already set", __FUNCTION__, sec_dev);
		ret = -EINVAL;
		goto exit;
	}

	ret = sec_dev->vendor.device->open(
	    dev,
	    sec_nfc_callback,
	    sec_nfc_data_callback);

	// store current SEC NFC instance
	current_sec_dev = sec_dev;

	// store default callbacks
	sec_dev->callbacks->sec_nfc_orig_callback = p_cback;
	sec_dev->callbacks->sec_nfc_orig_data_callback = p_data_cback;

exit:
	ALOGV("%s(%p): exit(%d)", __FUNCTION__, sec_dev, ret);
	return ret;
}

void sec_nfc_callback(
        nfc_event_t event,
        nfc_status_t event_status)
{
	ALOGV("%s: enter(%d, %d)", __FUNCTION__, event, event_status);

	// wait until sec_nfc_open() assigns current_sec_dev
	while (!current_sec_dev) ;

	current_sec_dev->callbacks->sec_nfc_orig_callback(event, event_status);

	ALOGV("%s: exit", __FUNCTION__);
}

void sec_nfc_data_callback(
        uint16_t data_len,
        uint8_t* p_data)
{
	ALOGV("%s: enter(%d, %p)", __FUNCTION__, data_len, p_data);

	// wait until sec_nfc_open() assigns current_sec_dev
	while (!current_sec_dev) ;

#ifdef LOG_VDEBUG
	int i = 0, j = 0;
	char data_dump_buf[SEC_NFC_WRITE_DUMP_BUF_SIZE];
#endif

#ifdef LOG_VDEBUG
	for (i = 0; i < data_len; i += SEC_NFC_WRITE_DUMP_LEN) {
		sprintf(data_dump_buf, "%02x", p_data[i]);
		for (j = 1; (j < SEC_NFC_WRITE_DUMP_LEN) && ((i + j) < data_len); j++) {
			sprintf(data_dump_buf, "%s %02x", data_dump_buf, p_data[i + j]);
		}
		ALOGVV("%s: %s", __FUNCTION__, data_dump_buf);
	}
#endif

	current_sec_dev->callbacks->sec_nfc_orig_data_callback(data_len, p_data);

	ALOGV("%s: exit", __FUNCTION__);
}

static int sec_nfc_write(
        const struct nfc_nci_device* dev,
        uint16_t data_len,
        const uint8_t* p_data)
{
	const sec_nfc_nci_device_t *sec_dev = (sec_nfc_nci_device_t *)dev;
	int ret = 0;
#ifdef LOG_VDEBUG
	int i = 0, j = 0;
	char data_dump_buf[SEC_NFC_WRITE_DUMP_BUF_SIZE];
#endif

	ALOGV("%s(%p): enter(%d, %p)", __FUNCTION__, sec_dev, data_len, p_data);

#ifdef LOG_VDEBUG
	for (i = 0; i < data_len; i += SEC_NFC_WRITE_DUMP_LEN) {
		sprintf(data_dump_buf, "%02x", p_data[i]);
		for (j = 1; (j < SEC_NFC_WRITE_DUMP_LEN) && ((i + j) < data_len); j++) {
			sprintf(data_dump_buf, "%s %02x", data_dump_buf, p_data[i + j]);
		}
		ALOGVV("%s(%p): %s", __FUNCTION__, sec_dev, data_dump_buf);
	}
#endif

	ret = sec_dev->vendor.device->write(dev, data_len, p_data);

	ALOGV("%s(%p): exit(%d)", __FUNCTION__, sec_dev, ret);
	return ret;
}

static int sec_nfc_core_initialized(
        const struct nfc_nci_device* dev,
        uint8_t* p_core_init_rsp_params)
{
	const sec_nfc_nci_device_t *sec_dev = (sec_nfc_nci_device_t *)dev;
	int ret = 0;

	ALOGV("%s(%p): enter(%p)", __FUNCTION__, sec_dev, p_core_init_rsp_params);

	ret = sec_dev->vendor.device->core_initialized(dev, p_core_init_rsp_params);

	ALOGV("%s(%p): exit(%d)", __FUNCTION__, sec_dev, ret);
	return ret;
}

static int sec_nfc_pre_discover(
        const struct nfc_nci_device* dev)
{
	const sec_nfc_nci_device_t *sec_dev = (sec_nfc_nci_device_t *)dev;
	int ret = 0;

	ALOGV("%s(%p): enter", __FUNCTION__, sec_dev);

	ret = sec_dev->vendor.device->pre_discover(dev);

	ALOGV("%s(%p): exit(%d)", __FUNCTION__, sec_dev, ret);
	return ret;
}

static int sec_nfc_close(
        const struct nfc_nci_device* dev)
{
	const sec_nfc_nci_device_t *sec_dev = (sec_nfc_nci_device_t *)dev;
	int ret = 0;

	ALOGV("%s(%p): enter", __FUNCTION__, sec_dev);

	ret = sec_dev->vendor.device->close(sec_dev->vendor.device);

	// reset current SEC HAL instance
	current_sec_dev = NULL;

	// reset original callbacks
	sec_dev->callbacks->sec_nfc_orig_callback = NULL;
	sec_dev->callbacks->sec_nfc_orig_data_callback = NULL;

	ALOGV("%s(%p): exit(%d)", __FUNCTION__, sec_dev, ret);
	return ret;
}

static int sec_nfc_control_granted(
        const struct nfc_nci_device* dev)
{
	const sec_nfc_nci_device_t *sec_dev = (sec_nfc_nci_device_t *)dev;
	int ret = 0;

	ALOGV("%s(%p): enter", __FUNCTION__, sec_dev);

	ret = sec_dev->vendor.device->control_granted(dev);

	ALOGV("%s(%p): exit(%d)", __FUNCTION__, sec_dev, ret);
	return ret;
}

static int sec_nfc_power_cycle(
        const struct nfc_nci_device* dev)
{
	const sec_nfc_nci_device_t *sec_dev = (sec_nfc_nci_device_t *)dev;
	int ret = 0;

	ALOGV("%s(%p): enter", __FUNCTION__, sec_dev);

	ret = sec_dev->vendor.device->power_cycle(dev);

	ALOGV("%s(%p): exit(%d)", __FUNCTION__, sec_dev, ret);
	return ret;
}

static int hal_close(
        hw_device_t *dev)
{
	ALOGV("%s(%p): enter", __FUNCTION__, dev);

	free(dev);

	ALOGV("%s(%p): exit", __FUNCTION__, dev);
	return 0;
}

static int hal_open(
        const hw_module_t* module,
        const char* name,
        hw_device_t** device)
{
	sec_nfc_nci_device_t *dev = NULL;
	int ret = 0;

	ALOGV("%s(%p): enter(%s)", __FUNCTION__, module, name);

	if (strcmp(name, NFC_NCI_CONTROLLER)) {
		ALOGV("%s(%p): passed name of instance does not match required instance name ('%s' != '%s')",
			__FUNCTION__, module, name, NFC_NCI_CONTROLLER);
		ret = -EINVAL;
		goto exit;
	}

	dev = (sec_nfc_nci_device_t *)malloc(sizeof(sec_nfc_nci_device_t));
	if (!dev) {
		ALOGV("%s(%p): not enough memory for sec_nfc_nci_device_t", __FUNCTION__, module);
		ret = -ENOMEM;
		goto exit;
	}
	memset(dev, 0, sizeof(sec_nfc_nci_device_t));

	dev->device = (sec_nfc_data_t *)malloc(sizeof(sec_nfc_data_t));
	if (!dev->device) {
		ALOGV("%s(%p): not enough memory for sec_nfc_data_t", __FUNCTION__, module);
		ret = -ENOMEM;
		goto exit;
	}
	memset(dev->device, 0, sizeof(sec_nfc_data_t));

	dev->callbacks = (sec_nfc_callbacks_t *)malloc(sizeof(sec_nfc_callbacks_t));
	if (!dev->callbacks) {
		ALOGV("%s(%p): not enough memory for sec_nfc_callbacks_t", __FUNCTION__, module);
		ret = -ENOMEM;
		goto exit;
	}
	memset(dev->callbacks, 0, sizeof(sec_nfc_callbacks_t));

	dev->base.common.tag = HARDWARE_DEVICE_TAG;
	dev->base.common.version = (1 << 16) | 0; /* major.minor */
	dev->base.common.module = (struct hw_module_t*) module;
	dev->base.common.close = hal_close;

	// NFC NCI method pointers
	dev->base.open             = sec_nfc_open;
	dev->base.write            = sec_nfc_write;
	dev->base.core_initialized = sec_nfc_core_initialized;
	dev->base.pre_discover     = sec_nfc_pre_discover;
	dev->base.close            = sec_nfc_close;
	dev->base.control_granted  = sec_nfc_control_granted;
	dev->base.power_cycle      = sec_nfc_power_cycle;

	// Vendor NCI HAL
	if (!nfc_nci_vendor.module) {
		ret = hw_get_module_by_class(
		    "nfc_nci",
		    "vendor",
		    &nfc_nci_vendor.hw_module);
		if (ret) {
			ALOGE("%s(%p): failed to load NFC NCI vendor module: %s (%d)", __FUNCTION__, module, strerror(-ret), -ret);
			goto exit;
		}
	}

	ret = nfc_nci_vendor.module->common.methods->open(
	    nfc_nci_vendor.hw_module,
	    name,
	    &dev->vendor.hw_device);
	if (ret) {
		ALOGE("%s(%p): failed to open NFC NCI vendor HAL: %s (%d)", __FUNCTION__, module, strerror(-ret), -ret);
		goto exit;
	}

	*device = (hw_device_t *)dev;

	ALOGV("%s(%p): exit(0)", __FUNCTION__, module);
	return 0;

exit:
	free(dev);
	ALOGV("%s(%p): exit(%d)", __FUNCTION__, module, ret);
	return ret;
}

static struct hw_module_methods_t module_methods = {
	.open = hal_open,
};

nfc_nci_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.module_api_version = (1 << 8) | 0, /* major.minor */
		.hal_api_version = 0x00, // 0 is only valid value
		.id = NFC_NCI_HARDWARE_MODULE_ID,
		.name = "Zero Sec NFC-NCI HAL",
		.author = "TeamNexus",
		.methods = &module_methods,
		.dso = NULL, /* remove compilation warnings */
		.reserved = {0}, /* remove compilation warnings */
	},
};
