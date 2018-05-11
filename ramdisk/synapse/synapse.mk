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

SYNAPSE_PATH := device/samsung/zero-common/ramdisk/synapse

# Synapse core script
PRODUCT_COPY_FILES += \
	$(SYNAPSE_PATH)/uci:system/bin/uci \
	$(SYNAPSE_PATH)/synapse:system/bin/synapse-init \
	$(SYNAPSE_PATH)/synapse.rc:system/etc/init/synapse.rc

# Synapse core script
PRODUCT_COPY_FILES += \
	$(SYNAPSE_PATH)/config.json.generate:root/res/synapse/config.json.generate \
	$(SYNAPSE_PATH)/config.json.generate.about:root/res/synapse/config.json.generate.about \
	$(SYNAPSE_PATH)/config.json.generate.big:root/res/synapse/config.json.generate.big \
	$(SYNAPSE_PATH)/config.json.generate.bus:root/res/synapse/config.json.generate.bus \
	$(SYNAPSE_PATH)/config.json.generate.doze:root/res/synapse/config.json.generate.doze \
	$(SYNAPSE_PATH)/config.json.generate.dt2w:root/res/synapse/config.json.generate.dt2w \
	$(SYNAPSE_PATH)/config.json.generate.governor:root/res/synapse/config.json.generate.governor \
	$(SYNAPSE_PATH)/config.json.generate.gpu:root/res/synapse/config.json.generate.gpu \
	$(SYNAPSE_PATH)/config.json.generate.io:root/res/synapse/config.json.generate.io \
	$(SYNAPSE_PATH)/config.json.generate.ksm:root/res/synapse/config.json.generate.ksm \
	$(SYNAPSE_PATH)/config.json.generate.little:root/res/synapse/config.json.generate.little \
	$(SYNAPSE_PATH)/config.json.generate.live:root/res/synapse/config.json.generate.live \
	$(SYNAPSE_PATH)/config.json.generate.logs:root/res/synapse/config.json.generate.logs \
	$(SYNAPSE_PATH)/config.json.generate.multicore:root/res/synapse/config.json.generate.multicore \
	$(SYNAPSE_PATH)/config.json.generate.network:root/res/synapse/config.json.generate.network \
	$(SYNAPSE_PATH)/config.json.generate.reboot:root/res/synapse/config.json.generate.reboot \
	$(SYNAPSE_PATH)/config.json.generate.storage:root/res/synapse/config.json.generate.storage \
	$(SYNAPSE_PATH)/config.json.generate.vm:root/res/synapse/config.json.generate.vm \
	$(SYNAPSE_PATH)/actions/bit:root/res/synapse/actions/bit \
	$(SYNAPSE_PATH)/actions/boolean:root/res/synapse/actions/boolean \
	$(SYNAPSE_PATH)/actions/bracket-option:root/res/synapse/actions/bracket-option \
	$(SYNAPSE_PATH)/actions/buildprop:root/res/synapse/actions/buildprop \
	$(SYNAPSE_PATH)/actions/c0volt:root/res/synapse/actions/c0volt \
	$(SYNAPSE_PATH)/actions/c1volt:root/res/synapse/actions/c1volt \
	$(SYNAPSE_PATH)/actions/charge-source:root/res/synapse/actions/charge-source \
	$(SYNAPSE_PATH)/actions/default:root/res/synapse/actions/default \
	$(SYNAPSE_PATH)/actions/devtools:root/res/synapse/actions/devtools \
	$(SYNAPSE_PATH)/actions/doze:root/res/synapse/actions/doze \
	$(SYNAPSE_PATH)/actions/doze_apply:root/res/synapse/actions/doze_apply \
	$(SYNAPSE_PATH)/actions/generic:root/res/synapse/actions/generic \
	$(SYNAPSE_PATH)/actions/generic2:root/res/synapse/actions/generic2 \
	$(SYNAPSE_PATH)/actions/governor:root/res/synapse/actions/governor \
	$(SYNAPSE_PATH)/actions/gpu_governor:root/res/synapse/actions/gpu_governor \
	$(SYNAPSE_PATH)/actions/gpuvolt:root/res/synapse/actions/gpuvolt \
	$(SYNAPSE_PATH)/actions/hmp:root/res/synapse/actions/hmp \
	$(SYNAPSE_PATH)/actions/input:root/res/synapse/actions/input \
	$(SYNAPSE_PATH)/actions/intvolt:root/res/synapse/actions/intvolt \
	$(SYNAPSE_PATH)/actions/io:root/res/synapse/actions/io \
	$(SYNAPSE_PATH)/actions/ioset:root/res/synapse/actions/ioset \
	$(SYNAPSE_PATH)/actions/ispvolt:root/res/synapse/actions/ispvolt \
	$(SYNAPSE_PATH)/actions/live:root/res/synapse/actions/live \
	$(SYNAPSE_PATH)/actions/log:root/res/synapse/actions/log \
	$(SYNAPSE_PATH)/actions/mifvolt:root/res/synapse/actions/mifvolt \
	$(SYNAPSE_PATH)/actions/network:root/res/synapse/actions/network \
	$(SYNAPSE_PATH)/actions/output:root/res/synapse/actions/output \
	$(SYNAPSE_PATH)/actions/output_head:root/res/synapse/actions/output_head \
	$(SYNAPSE_PATH)/actions/output_pwamp:root/res/synapse/actions/output_pwamp \
	$(SYNAPSE_PATH)/actions/powersaving:root/res/synapse/actions/powersaving \
	$(SYNAPSE_PATH)/actions/scheduler:root/res/synapse/actions/scheduler \
	$(SYNAPSE_PATH)/actions/security:root/res/synapse/actions/security \
	$(SYNAPSE_PATH)/actions/serviceset:root/res/synapse/actions/serviceset \
	$(SYNAPSE_PATH)/actions/show:root/res/synapse/actions/show \
	$(SYNAPSE_PATH)/actions/sqlite:root/res/synapse/actions/sqlite \
	$(SYNAPSE_PATH)/actions/storage:root/res/synapse/actions/storage \
	$(SYNAPSE_PATH)/actions/sync:root/res/synapse/actions/sync \
	$(SYNAPSE_PATH)/actions/touch:root/res/synapse/actions/touch \
	$(SYNAPSE_PATH)/actions/tuning:root/res/synapse/actions/tuning \
	$(SYNAPSE_PATH)/actions/voltage:root/res/synapse/actions/voltage \
	$(SYNAPSE_PATH)/actions/voltage2:root/res/synapse/actions/voltage2 \
	$(SYNAPSE_PATH)/hrtkernel/dns:root/res/synapse/hrtkernel/dns \
	$(SYNAPSE_PATH)/hrtkernel/gps_zone:root/res/synapse/hrtkernel/gps_zone \
	$(SYNAPSE_PATH)/hrtkernel/gpu_gov:root/res/synapse/hrtkernel/gpu_gov \
	$(SYNAPSE_PATH)/hrtkernel/input_key:root/res/synapse/hrtkernel/input_key \
	$(SYNAPSE_PATH)/hrtkernel/input_touchkey:root/res/synapse/hrtkernel/input_touchkey \
	$(SYNAPSE_PATH)/hrtkernel/io_tweaking:root/res/synapse/hrtkernel/io_tweaking \
	$(SYNAPSE_PATH)/hrtkernel/IPv6:root/res/synapse/hrtkernel/IPv6 \
	$(SYNAPSE_PATH)/hrtkernel/knox:root/res/synapse/hrtkernel/knox \
	$(SYNAPSE_PATH)/hrtkernel/mass_storage:root/res/synapse/hrtkernel/mass_storage \
	$(SYNAPSE_PATH)/hrtkernel/tcp_security:root/res/synapse/hrtkernel/tcp_security \
	$(SYNAPSE_PATH)/hrtkernel/tuning_hmp:root/res/synapse/hrtkernel/tuning_hmp \
	$(SYNAPSE_PATH)/hrtkernel/tuning_interactive_b:root/res/synapse/hrtkernel/tuning_interactive_b \
	$(SYNAPSE_PATH)/hrtkernel/tuning_interactive_L:root/res/synapse/hrtkernel/tuning_interactive_L \
	$(SYNAPSE_PATH)/hrtkernel/tuning_interactive_turbo:root/res/synapse/hrtkernel/tuning_interactive_turbo \
	$(SYNAPSE_PATH)/quintz/doze_idle_after_inactive_to:root/res/synapse/quintz/doze_idle_after_inactive_to \
	$(SYNAPSE_PATH)/quintz/doze_idle_factor:root/res/synapse/quintz/doze_idle_factor \
	$(SYNAPSE_PATH)/quintz/doze_idle_mode:root/res/synapse/quintz/doze_idle_mode \
	$(SYNAPSE_PATH)/quintz/doze_idle_pending_factor:root/res/synapse/quintz/doze_idle_pending_factor \
	$(SYNAPSE_PATH)/quintz/doze_idle_pending_to:root/res/synapse/quintz/doze_idle_pending_to \
	$(SYNAPSE_PATH)/quintz/doze_idle_to:root/res/synapse/quintz/doze_idle_to \
	$(SYNAPSE_PATH)/quintz/doze_inactive_to:root/res/synapse/quintz/doze_inactive_to \
	$(SYNAPSE_PATH)/quintz/doze_locating_to:root/res/synapse/quintz/doze_locating_to \
	$(SYNAPSE_PATH)/quintz/doze_location_accuracy:root/res/synapse/quintz/doze_location_accuracy \
	$(SYNAPSE_PATH)/quintz/doze_max_idle_pending_to:root/res/synapse/quintz/doze_max_idle_pending_to \
	$(SYNAPSE_PATH)/quintz/doze_max_idle_to:root/res/synapse/quintz/doze_max_idle_to \
	$(SYNAPSE_PATH)/quintz/doze_max_temp_app_whitelist_duration:root/res/synapse/quintz/doze_max_temp_app_whitelist_duration \
	$(SYNAPSE_PATH)/quintz/doze_min_time_to_alarm:root/res/synapse/quintz/doze_min_time_to_alarm \
	$(SYNAPSE_PATH)/quintz/doze_mms_temp_app_whitelist_duration:root/res/synapse/quintz/doze_mms_temp_app_whitelist_duration \
	$(SYNAPSE_PATH)/quintz/doze_motion_inactive_to:root/res/synapse/quintz/doze_motion_inactive_to \
	$(SYNAPSE_PATH)/quintz/doze_profile:root/res/synapse/quintz/doze_profile \
	$(SYNAPSE_PATH)/quintz/doze_sensing_to:root/res/synapse/quintz/doze_sensing_to \
	$(SYNAPSE_PATH)/quintz/doze_sms_temp_app_whitelist_duration:root/res/synapse/quintz/doze_sms_temp_app_whitelist_duration
