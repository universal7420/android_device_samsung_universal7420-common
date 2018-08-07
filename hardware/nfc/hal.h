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

#include <hardware/nfc.h>
#include <hardware/nfc_tag.h>

typedef struct sec_nfc_data {
	bool opened;
} sec_nfc_data_t;

typedef struct sec_nfc_callbacks {
	nfc_stack_callback_t *sec_nfc_orig_callback;
	nfc_stack_data_callback_t *sec_nfc_orig_data_callback;
} sec_nfc_callbacks_t;

typedef struct sec_nfc_nci_device {
	nfc_nci_device_t base;

	sec_nfc_data_t *device;
	sec_nfc_callbacks_t *callbacks;

	union {
		nfc_nci_device_t *device;
		hw_device_t *hw_device;
	} vendor;
} sec_nfc_nci_device_t;

static int sec_nfc_open(
        const struct nfc_nci_device* dev,
        nfc_stack_callback_t* p_cback,
        nfc_stack_data_callback_t* p_data_cback);

void sec_nfc_callback(
        nfc_event_t event,
        nfc_status_t event_status);

void sec_nfc_data_callback(
        uint16_t data_len,
        uint8_t* p_data);

static int sec_nfc_write(
        const struct nfc_nci_device* dev,
        uint16_t data_len,
        const uint8_t* p_data);

static int sec_nfc_core_initialized(
        const struct nfc_nci_device* dev,
        uint8_t* p_core_init_rsp_params);

static int sec_nfc_pre_discover(
        const struct nfc_nci_device* dev);

static int sec_nfc_close(
        const struct nfc_nci_device* dev);

static int sec_nfc_control_granted(
        const struct nfc_nci_device* dev);

static int sec_nfc_power_cycle(
        const struct nfc_nci_device* dev);

static int hal_open(
        const hw_module_t* module,
        const char* name,
        hw_device_t** device);
		
static int hal_close(
        hw_device_t *dev);
