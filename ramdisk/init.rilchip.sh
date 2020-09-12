#!/system/bin/sh
#
# Copyright (C) 2018 TeamNexus
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

RILCHIP_STATE=$1
RILCHIP_RESETPROP="/vendor/bin/resetprop.universal7420 -n"

function log {
	echo "[init.rilchip.sh] $1 ($RILCHIP_STATE): $2" > /dev/kmsg
	echo "$(date) $1 ($RILCHIP_STATE): $2"
}

function info {
	log "I" "$1"
}

function warn {
	log "W" "$1"
}

function error {
	log "E" "$1"
}

function debug {
	log "D" "$1"
}

function svc_start {
	if [ "$(getprop init.svc.$1)" == "running" ]; then
		warn "failed to start $1: already running!"
		return 0
	fi

	info "starting $1..."
	start $1
}

function svc_stop {
	if [[ "$(getprop init.svc.$1)" == "stopped" ]] || [[ "$(getprop init.svc.$1)" == "" ]]; then
		warn "failed to stop $1: already stopped!"
		return 0
	fi

	warn "stopping $1..."
	stop $1
}

function svc_status {
	info "status of $1: $(getprop init.svc.$1)"
}

debug "rilchip command: \"$RILCHIP_STATE\""

if [ "$(getprop ro.boot.baseband)" == "mdm2" ]; then
	IS_QC_DEVICE=1
else
	IS_QC_DEVICE=0
fi
debug "baseband: \"$(getprop ro.boot.baseband)\""

if [ ! -f /proc/simslot_count ]; then
	error "cannot find /proc/simslot_count, assuming 1"
	SIMSLOT_COUNT=1
else
	SIMSLOT_COUNT=$(cat /proc/simslot_count)
fi
debug "simslot count: $SIMSLOT_COUNT"

if [ "$RILCHIP_STATE" == "init" ]; then

	if [ "$SIMSLOT_COUNT" == "1" ]; then
		# RIL configuration
		$RILCHIP_RESETPROP ro.multisim.simslotcount        1
		$RILCHIP_RESETPROP persist.radio.multisim.config   none
		$RILCHIP_RESETPROP rild.libpath                    /system/vendor/lib64/libsec-ril.so
		$RILCHIP_RESETPROP ro.telephony.default_network    9
		$RILCHIP_RESETPROP ro.telephony.ril.config         simactivation
	elif [ "$SIMSLOT_COUNT" == "2" ]; then
		# RIL configuration
		$RILCHIP_RESETPROP ro.multisim.simslotcount        2
		$RILCHIP_RESETPROP persist.radio.multisim.config   dsds
		$RILCHIP_RESETPROP persist.multisim.config         dsds
		$RILCHIP_RESETPROP rild.libpath                    /system/vendor/lib64/libsec-ril.so
		$RILCHIP_RESETPROP rild.libpath2                   /system/vendor/lib64/libsec-ril-dsds.so
		$RILCHIP_RESETPROP ro.telephony.default_network    9,9
		$RILCHIP_RESETPROP ro.telephony.ril.config         simactivation
	fi

elif [ "$RILCHIP_STATE" == "late-init" ]; then

	NORIL=$(getprop ro.ril.noril)

	# If RIL is disabled, stop everything
	if [[ "$NORIL" == "1" ]] || [[ "$NORIL" == "yes" ]] || [[ $IS_QC_DEVICE == 1 ]]; then
		/system/bin/sh $0 stop
	fi

elif [ "$RILCHIP_STATE" == "start" ]; then

	# on QC-modem devices we need to control other services too
	if [ $IS_QC_DEVICE == 1 ]; then
		svc_start qmuxd
		svc_start mdm_helper_proxy
	else
		# start modem boot daemon (not needed on QC-devices)
		svc_start cpboot-daemon
	fi

	# start slot1 daemon
	svc_start ril-daemon

	if [ "$SIMSLOT_COUNT" == "2" ]; then
		# start slot2 daemon
		svc_start ril-daemon1
	fi

elif [ "$RILCHIP_STATE" == "stop" ]; then

	# on QC-modem devices we need to control other services too
	if [ $IS_QC_DEVICE == 1 ]; then
		svc_stop qmuxd
		svc_stop mdm_helper_proxy
	else
		# stop modem boot daemon (not needed on QC-devices)
		svc_stop cpboot-daemon
	fi

	# stop slot1 daemon
	svc_stop ril-daemon

	if [ "$SIMSLOT_COUNT" == "2" ]; then
		# stop slot2 daemon
		svc_stop ril-daemon1
	fi

	# FIXUP: disable RIL-wakelock after stopping the services
	echo "radio-interface" > /sys/power/wake_unlock

elif [ "$RILCHIP_STATE" == "restart" ]; then

	# stop everything
	/system/bin/sh $0 stop

	# wait for modem-shutdown
	debug "waiting for modem shutdown... (5s)"
	sleep 5

	# start everything again
	/system/bin/sh $0 start

elif [ "$RILCHIP_STATE" == "status" ]; then

	# dump QC-modem daemons
	if [ $IS_QC_DEVICE == 1 ]; then
		svc_status qmuxd
		svc_status mdm_helper_proxy
	else
		# dump modem boot daemon (not needed on QC-devices)
		svc_status cpboot-daemon
	fi

	# dump slot1 daemon
	svc_status ril-daemon

	if [ "$SIMSLOT_COUNT" == "2" ]; then
		# dump slot2 daemon
		svc_status ril-daemon1
	fi
	
else

	error "unknown rilchip command"

fi

exit 0
