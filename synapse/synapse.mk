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

SYNAPSE_PATH   := device/samsung/zero-common/synapse
SYNAPSE_TARGET := system/res/synapse

# Synapse Core Scripts
PRODUCT_COPY_FILES += \
	$(SYNAPSE_PATH)/uci:system/bin/uci \
	$(SYNAPSE_PATH)/synapse:system/bin/synapse \
	$(SYNAPSE_PATH)/synapse-update:system/bin/synapse-update \
	$(SYNAPSE_PATH)/synapse.rc:system/etc/init/synapse.rc

# Synapse Configuration Scripts
PRODUCT_COPY_FILES += \
	$(SYNAPSE_PATH)/config.json.generate:$(SYNAPSE_TARGET)/config.json.generate \
	$(SYNAPSE_PATH)/config.json.generate.about:$(SYNAPSE_TARGET)/config.json.generate.about \
	$(SYNAPSE_PATH)/config.json.generate.big:$(SYNAPSE_TARGET)/config.json.generate.big \
	$(SYNAPSE_PATH)/config.json.generate.bus:$(SYNAPSE_TARGET)/config.json.generate.bus \
	$(SYNAPSE_PATH)/config.json.generate.doze:$(SYNAPSE_TARGET)/config.json.generate.doze \
	$(SYNAPSE_PATH)/config.json.generate.dt2w:$(SYNAPSE_TARGET)/config.json.generate.dt2w \
	$(SYNAPSE_PATH)/config.json.generate.governor:$(SYNAPSE_TARGET)/config.json.generate.governor \
	$(SYNAPSE_PATH)/config.json.generate.gpu:$(SYNAPSE_TARGET)/config.json.generate.gpu \
	$(SYNAPSE_PATH)/config.json.generate.io:$(SYNAPSE_TARGET)/config.json.generate.io \
	$(SYNAPSE_PATH)/config.json.generate.ksm:$(SYNAPSE_TARGET)/config.json.generate.ksm \
	$(SYNAPSE_PATH)/config.json.generate.little:$(SYNAPSE_TARGET)/config.json.generate.little \
	$(SYNAPSE_PATH)/config.json.generate.live:$(SYNAPSE_TARGET)/config.json.generate.live \
	$(SYNAPSE_PATH)/config.json.generate.logs:$(SYNAPSE_TARGET)/config.json.generate.logs \
	$(SYNAPSE_PATH)/config.json.generate.multicore:$(SYNAPSE_TARGET)/config.json.generate.multicore \
	$(SYNAPSE_PATH)/config.json.generate.network:$(SYNAPSE_TARGET)/config.json.generate.network \
	$(SYNAPSE_PATH)/config.json.generate.reboot:$(SYNAPSE_TARGET)/config.json.generate.reboot \
	$(SYNAPSE_PATH)/config.json.generate.storage:$(SYNAPSE_TARGET)/config.json.generate.storage \
	$(SYNAPSE_PATH)/config.json.generate.vm:$(SYNAPSE_TARGET)/config.json.generate.vm \
	$(SYNAPSE_PATH)/actions/bit:$(SYNAPSE_TARGET)/actions/bit \
	$(SYNAPSE_PATH)/actions/boolean:$(SYNAPSE_TARGET)/actions/boolean \
	$(SYNAPSE_PATH)/actions/bracket-option:$(SYNAPSE_TARGET)/actions/bracket-option \
	$(SYNAPSE_PATH)/actions/buildprop:$(SYNAPSE_TARGET)/actions/buildprop \
	$(SYNAPSE_PATH)/actions/c0volt:$(SYNAPSE_TARGET)/actions/c0volt \
	$(SYNAPSE_PATH)/actions/c1volt:$(SYNAPSE_TARGET)/actions/c1volt \
	$(SYNAPSE_PATH)/actions/charge-source:$(SYNAPSE_TARGET)/actions/charge-source \
	$(SYNAPSE_PATH)/actions/default:$(SYNAPSE_TARGET)/actions/default \
	$(SYNAPSE_PATH)/actions/devtools:$(SYNAPSE_TARGET)/actions/devtools \
	$(SYNAPSE_PATH)/actions/doze:$(SYNAPSE_TARGET)/actions/doze \
	$(SYNAPSE_PATH)/actions/doze_apply:$(SYNAPSE_TARGET)/actions/doze_apply \
	$(SYNAPSE_PATH)/actions/generic:$(SYNAPSE_TARGET)/actions/generic \
	$(SYNAPSE_PATH)/actions/generic2:$(SYNAPSE_TARGET)/actions/generic2 \
	$(SYNAPSE_PATH)/actions/governor:$(SYNAPSE_TARGET)/actions/governor \
	$(SYNAPSE_PATH)/actions/gpu_governor:$(SYNAPSE_TARGET)/actions/gpu_governor \
	$(SYNAPSE_PATH)/actions/gpuvolt:$(SYNAPSE_TARGET)/actions/gpuvolt \
	$(SYNAPSE_PATH)/actions/hmp:$(SYNAPSE_TARGET)/actions/hmp \
	$(SYNAPSE_PATH)/actions/input:$(SYNAPSE_TARGET)/actions/input \
	$(SYNAPSE_PATH)/actions/intvolt:$(SYNAPSE_TARGET)/actions/intvolt \
	$(SYNAPSE_PATH)/actions/io:$(SYNAPSE_TARGET)/actions/io \
	$(SYNAPSE_PATH)/actions/ioset:$(SYNAPSE_TARGET)/actions/ioset \
	$(SYNAPSE_PATH)/actions/ispvolt:$(SYNAPSE_TARGET)/actions/ispvolt \
	$(SYNAPSE_PATH)/actions/live:$(SYNAPSE_TARGET)/actions/live \
	$(SYNAPSE_PATH)/actions/log:$(SYNAPSE_TARGET)/actions/log \
	$(SYNAPSE_PATH)/actions/mifvolt:$(SYNAPSE_TARGET)/actions/mifvolt \
	$(SYNAPSE_PATH)/actions/network:$(SYNAPSE_TARGET)/actions/network \
	$(SYNAPSE_PATH)/actions/output:$(SYNAPSE_TARGET)/actions/output \
	$(SYNAPSE_PATH)/actions/output_head:$(SYNAPSE_TARGET)/actions/output_head \
	$(SYNAPSE_PATH)/actions/output_pwamp:$(SYNAPSE_TARGET)/actions/output_pwamp \
	$(SYNAPSE_PATH)/actions/powersaving:$(SYNAPSE_TARGET)/actions/powersaving \
	$(SYNAPSE_PATH)/actions/scheduler:$(SYNAPSE_TARGET)/actions/scheduler \
	$(SYNAPSE_PATH)/actions/security:$(SYNAPSE_TARGET)/actions/security \
	$(SYNAPSE_PATH)/actions/serviceset:$(SYNAPSE_TARGET)/actions/serviceset \
	$(SYNAPSE_PATH)/actions/show:$(SYNAPSE_TARGET)/actions/show \
	$(SYNAPSE_PATH)/actions/sqlite:$(SYNAPSE_TARGET)/actions/sqlite \
	$(SYNAPSE_PATH)/actions/storage:$(SYNAPSE_TARGET)/actions/storage \
	$(SYNAPSE_PATH)/actions/sync:$(SYNAPSE_TARGET)/actions/sync \
	$(SYNAPSE_PATH)/actions/touch:$(SYNAPSE_TARGET)/actions/touch \
	$(SYNAPSE_PATH)/actions/tuning:$(SYNAPSE_TARGET)/actions/tuning \
	$(SYNAPSE_PATH)/actions/voltage:$(SYNAPSE_TARGET)/actions/voltage \
	$(SYNAPSE_PATH)/actions/voltage2:$(SYNAPSE_TARGET)/actions/voltage2 \
	$(SYNAPSE_PATH)/actions/voltage-defaults:$(SYNAPSE_TARGET)/actions/voltage-defaults \
	$(SYNAPSE_PATH)/hrtkernel/dns:$(SYNAPSE_TARGET)/hrtkernel/dns \
	$(SYNAPSE_PATH)/hrtkernel/gps_zone:$(SYNAPSE_TARGET)/hrtkernel/gps_zone \
	$(SYNAPSE_PATH)/hrtkernel/gpu_gov:$(SYNAPSE_TARGET)/hrtkernel/gpu_gov \
	$(SYNAPSE_PATH)/hrtkernel/input_key:$(SYNAPSE_TARGET)/hrtkernel/input_key \
	$(SYNAPSE_PATH)/hrtkernel/input_touchkey:$(SYNAPSE_TARGET)/hrtkernel/input_touchkey \
	$(SYNAPSE_PATH)/hrtkernel/io_tweaking:$(SYNAPSE_TARGET)/hrtkernel/io_tweaking \
	$(SYNAPSE_PATH)/hrtkernel/IPv6:$(SYNAPSE_TARGET)/hrtkernel/IPv6 \
	$(SYNAPSE_PATH)/hrtkernel/knox:$(SYNAPSE_TARGET)/hrtkernel/knox \
	$(SYNAPSE_PATH)/hrtkernel/mass_storage:$(SYNAPSE_TARGET)/hrtkernel/mass_storage \
	$(SYNAPSE_PATH)/hrtkernel/tcp_security:$(SYNAPSE_TARGET)/hrtkernel/tcp_security \
	$(SYNAPSE_PATH)/hrtkernel/tuning_hmp:$(SYNAPSE_TARGET)/hrtkernel/tuning_hmp \
	$(SYNAPSE_PATH)/hrtkernel/tuning_interactive_b:$(SYNAPSE_TARGET)/hrtkernel/tuning_interactive_b \
	$(SYNAPSE_PATH)/hrtkernel/tuning_interactive_L:$(SYNAPSE_TARGET)/hrtkernel/tuning_interactive_L \
	$(SYNAPSE_PATH)/hrtkernel/tuning_interactive_turbo:$(SYNAPSE_TARGET)/hrtkernel/tuning_interactive_turbo \
	$(SYNAPSE_PATH)/quintz/doze_idle_after_inactive_to:$(SYNAPSE_TARGET)/quintz/doze_idle_after_inactive_to \
	$(SYNAPSE_PATH)/quintz/doze_idle_factor:$(SYNAPSE_TARGET)/quintz/doze_idle_factor \
	$(SYNAPSE_PATH)/quintz/doze_idle_mode:$(SYNAPSE_TARGET)/quintz/doze_idle_mode \
	$(SYNAPSE_PATH)/quintz/doze_idle_pending_factor:$(SYNAPSE_TARGET)/quintz/doze_idle_pending_factor \
	$(SYNAPSE_PATH)/quintz/doze_idle_pending_to:$(SYNAPSE_TARGET)/quintz/doze_idle_pending_to \
	$(SYNAPSE_PATH)/quintz/doze_idle_to:$(SYNAPSE_TARGET)/quintz/doze_idle_to \
	$(SYNAPSE_PATH)/quintz/doze_inactive_to:$(SYNAPSE_TARGET)/quintz/doze_inactive_to \
	$(SYNAPSE_PATH)/quintz/doze_locating_to:$(SYNAPSE_TARGET)/quintz/doze_locating_to \
	$(SYNAPSE_PATH)/quintz/doze_location_accuracy:$(SYNAPSE_TARGET)/quintz/doze_location_accuracy \
	$(SYNAPSE_PATH)/quintz/doze_max_idle_pending_to:$(SYNAPSE_TARGET)/quintz/doze_max_idle_pending_to \
	$(SYNAPSE_PATH)/quintz/doze_max_idle_to:$(SYNAPSE_TARGET)/quintz/doze_max_idle_to \
	$(SYNAPSE_PATH)/quintz/doze_max_temp_app_whitelist_duration:$(SYNAPSE_TARGET)/quintz/doze_max_temp_app_whitelist_duration \
	$(SYNAPSE_PATH)/quintz/doze_min_time_to_alarm:$(SYNAPSE_TARGET)/quintz/doze_min_time_to_alarm \
	$(SYNAPSE_PATH)/quintz/doze_mms_temp_app_whitelist_duration:$(SYNAPSE_TARGET)/quintz/doze_mms_temp_app_whitelist_duration \
	$(SYNAPSE_PATH)/quintz/doze_motion_inactive_to:$(SYNAPSE_TARGET)/quintz/doze_motion_inactive_to \
	$(SYNAPSE_PATH)/quintz/doze_profile:$(SYNAPSE_TARGET)/quintz/doze_profile \
	$(SYNAPSE_PATH)/quintz/doze_sensing_to:$(SYNAPSE_TARGET)/quintz/doze_sensing_to \
	$(SYNAPSE_PATH)/quintz/doze_sms_temp_app_whitelist_duration:$(SYNAPSE_TARGET)/quintz/doze_sms_temp_app_whitelist_duration
