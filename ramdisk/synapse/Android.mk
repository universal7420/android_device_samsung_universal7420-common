#
# Copyright (C) 2018 TeamNexus
# Copyright (C) 2018 Pop Alexandru Radu (minealex2244)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

# Synapse core script
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/uci:system/bin/uci

# Synapse core script
PRODUCT_COPY_FILES += \
	$(LOCAL_PATH)/config.json.generate:/res/synapse/config.json.generate \
	$(LOCAL_PATH)/config.json.generate.about:/res/synapse/config.json.generate.about \
	$(LOCAL_PATH)/config.json.generate.big:/res/synapse/config.json.generate.big \
	$(LOCAL_PATH)/config.json.generate.bus:/res/synapse/config.json.generate.bus \
	$(LOCAL_PATH)/config.json.generate.doze:/res/synapse/config.json.generate.doze \
	$(LOCAL_PATH)/config.json.generate.dt2w:/res/synapse/config.json.generate.dt2w \
	$(LOCAL_PATH)/config.json.generate.governor:/res/synapse/config.json.generate.governor \
	$(LOCAL_PATH)/config.json.generate.gpu:/res/synapse/config.json.generate.gpu \
	$(LOCAL_PATH)/config.json.generate.io:/res/synapse/config.json.generate.io \
	$(LOCAL_PATH)/config.json.generate.ksm:/res/synapse/config.json.generate.ksm \
	$(LOCAL_PATH)/config.json.generate.little:/res/synapse/config.json.generate.little \
	$(LOCAL_PATH)/config.json.generate.live:/res/synapse/config.json.generate.live \
	$(LOCAL_PATH)/config.json.generate.logs:/res/synapse/config.json.generate.logs \
	$(LOCAL_PATH)/config.json.generate.multicore:/res/synapse/config.json.generate.multicore \
	$(LOCAL_PATH)/config.json.generate.network:/res/synapse/config.json.generate.network \
	$(LOCAL_PATH)/config.json.generate.reboot:/res/synapse/config.json.generate.reboot \
	$(LOCAL_PATH)/config.json.generate.storage:/res/synapse/config.json.generate.storage \
	$(LOCAL_PATH)/config.json.generate.vm:/res/synapse/config.json.generate.vm \
	$(LOCAL_PATH)/actions:/res/synapse/actions \
	$(LOCAL_PATH)/actions/bit:/res/synapse/actions/bit \
	$(LOCAL_PATH)/actions/boolean:/res/synapse/actions/boolean \
	$(LOCAL_PATH)/actions/bracket-option:/res/synapse/actions/bracket-option \
	$(LOCAL_PATH)/actions/buildprop:/res/synapse/actions/buildprop \
	$(LOCAL_PATH)/actions/c0volt:/res/synapse/actions/c0volt \
	$(LOCAL_PATH)/actions/c1volt:/res/synapse/actions/c1volt \
	$(LOCAL_PATH)/actions/charge-source:/res/synapse/actions/charge-source \
	$(LOCAL_PATH)/actions/default:/res/synapse/actions/default \
	$(LOCAL_PATH)/actions/devtools:/res/synapse/actions/devtools \
	$(LOCAL_PATH)/actions/doze:/res/synapse/actions/doze \
	$(LOCAL_PATH)/actions/doze_apply:/res/synapse/actions/doze_apply \
	$(LOCAL_PATH)/actions/generic:/res/synapse/actions/generic \
	$(LOCAL_PATH)/actions/generic2:/res/synapse/actions/generic2 \
	$(LOCAL_PATH)/actions/governor:/res/synapse/actions/governor \
	$(LOCAL_PATH)/actions/gpu_governor:/res/synapse/actions/gpu_governor \
	$(LOCAL_PATH)/actions/gpuvolt:/res/synapse/actions/gpuvolt \
	$(LOCAL_PATH)/actions/hmp:/res/synapse/actions/hmp \
	$(LOCAL_PATH)/actions/input:/res/synapse/actions/input \
	$(LOCAL_PATH)/actions/intvolt:/res/synapse/actions/intvolt \
	$(LOCAL_PATH)/actions/io:/res/synapse/actions/io \
	$(LOCAL_PATH)/actions/ioset:/res/synapse/actions/ioset \
	$(LOCAL_PATH)/actions/ispvolt:/res/synapse/actions/ispvolt \
	$(LOCAL_PATH)/actions/live:/res/synapse/actions/live \
	$(LOCAL_PATH)/actions/log:/res/synapse/actions/log \
	$(LOCAL_PATH)/actions/mifvolt:/res/synapse/actions/mifvolt \
	$(LOCAL_PATH)/actions/network:/res/synapse/actions/network \
	$(LOCAL_PATH)/actions/output:/res/synapse/actions/output \
	$(LOCAL_PATH)/actions/output_head:/res/synapse/actions/output_head \
	$(LOCAL_PATH)/actions/output_pwamp:/res/synapse/actions/output_pwamp \
	$(LOCAL_PATH)/actions/powersaving:/res/synapse/actions/powersaving \
	$(LOCAL_PATH)/actions/scheduler:/res/synapse/actions/scheduler \
	$(LOCAL_PATH)/actions/security:/res/synapse/actions/security \
	$(LOCAL_PATH)/actions/serviceset:/res/synapse/actions/serviceset \
	$(LOCAL_PATH)/actions/show:/res/synapse/actions/show \
	$(LOCAL_PATH)/actions/sqlite:/res/synapse/actions/sqlite \
	$(LOCAL_PATH)/actions/storage:/res/synapse/actions/storage \
	$(LOCAL_PATH)/actions/sync:/res/synapse/actions/sync \
	$(LOCAL_PATH)/actions/touch:/res/synapse/actions/touch \
	$(LOCAL_PATH)/actions/tuning:/res/synapse/actions/tuning \
	$(LOCAL_PATH)/actions/voltage:/res/synapse/actions/voltage \
	$(LOCAL_PATH)/actions/voltage2:/res/synapse/actions/voltage2 \
	$(LOCAL_PATH)/hrtkernel:/res/synapse/hrtkernel \
	$(LOCAL_PATH)/hrtkernel/dns:/res/synapse/hrtkernel/dns \
	$(LOCAL_PATH)/hrtkernel/gps_zone:/res/synapse/hrtkernel/gps_zone \
	$(LOCAL_PATH)/hrtkernel/gpu_gov:/res/synapse/hrtkernel/gpu_gov \
	$(LOCAL_PATH)/hrtkernel/input_key:/res/synapse/hrtkernel/input_key \
	$(LOCAL_PATH)/hrtkernel/input_touchkey:/res/synapse/hrtkernel/input_touchkey \
	$(LOCAL_PATH)/hrtkernel/io_tweaking:/res/synapse/hrtkernel/io_tweaking \
	$(LOCAL_PATH)/hrtkernel/IPv6:/res/synapse/hrtkernel/IPv6 \
	$(LOCAL_PATH)/hrtkernel/knox:/res/synapse/hrtkernel/knox \
	$(LOCAL_PATH)/hrtkernel/mass_storage:/res/synapse/hrtkernel/mass_storage \
	$(LOCAL_PATH)/hrtkernel/tcp_security:/res/synapse/hrtkernel/tcp_security \
	$(LOCAL_PATH)/hrtkernel/tuning_hmp:/res/synapse/hrtkernel/tuning_hmp \
	$(LOCAL_PATH)/hrtkernel/tuning_interactive_b:/res/synapse/hrtkernel/tuning_interactive_b \
	$(LOCAL_PATH)/hrtkernel/tuning_interactive_L:/res/synapse/hrtkernel/tuning_interactive_L \
	$(LOCAL_PATH)/hrtkernel/tuning_interactive_turbo:/res/synapse/hrtkernel/tuning_interactive_turbo \
	$(LOCAL_PATH)/quintz:/res/synapse/quintz \
	$(LOCAL_PATH)/quintz/doze_idle_after_inactive_to:/res/synapse/quintz/doze_idle_after_inactive_to \
	$(LOCAL_PATH)/quintz/doze_idle_factor:/res/synapse/quintz/doze_idle_factor \
	$(LOCAL_PATH)/quintz/doze_idle_mode:/res/synapse/quintz/doze_idle_mode \
	$(LOCAL_PATH)/quintz/doze_idle_pending_factor:/res/synapse/quintz/doze_idle_pending_factor \
	$(LOCAL_PATH)/quintz/doze_idle_pending_to:/res/synapse/quintz/doze_idle_pending_to \
	$(LOCAL_PATH)/quintz/doze_idle_to:/res/synapse/quintz/doze_idle_to \
	$(LOCAL_PATH)/quintz/doze_inactive_to:/res/synapse/quintz/doze_inactive_to \
	$(LOCAL_PATH)/quintz/doze_locating_to:/res/synapse/quintz/doze_locating_to \
	$(LOCAL_PATH)/quintz/doze_location_accuracy:/res/synapse/quintz/doze_location_accuracy \
	$(LOCAL_PATH)/quintz/doze_max_idle_pending_to:/res/synapse/quintz/doze_max_idle_pending_to \
	$(LOCAL_PATH)/quintz/doze_max_idle_to:/res/synapse/quintz/doze_max_idle_to \
	$(LOCAL_PATH)/quintz/doze_max_temp_app_whitelist_duration:/res/synapse/quintz/doze_max_temp_app_whitelist_duration \
	$(LOCAL_PATH)/quintz/doze_min_time_to_alarm:/res/synapse/quintz/doze_min_time_to_alarm \
	$(LOCAL_PATH)/quintz/doze_mms_temp_app_whitelist_duration:/res/synapse/quintz/doze_mms_temp_app_whitelist_duration \
	$(LOCAL_PATH)/quintz/doze_motion_inactive_to:/res/synapse/quintz/doze_motion_inactive_to \
	$(LOCAL_PATH)/quintz/doze_profile:/res/synapse/quintz/doze_profile \
	$(LOCAL_PATH)/quintz/doze_sensing_to:/res/synapse/quintz/doze_sensing_to \
	$(LOCAL_PATH)/quintz/doze_sms_temp_app_whitelist_duration:/res/synapse/quintz/doze_sms_temp_app_whitelist_duration
