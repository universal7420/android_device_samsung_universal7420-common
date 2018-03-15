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
RILCHIP_RESETPROP="/vendor/bin/resetprop.zero -n"

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
	if [ ! -z $RILCHIP_DEBUG ]; then
		log "D" "$1"
	fi
}

function svc_start {
	if [ "$(getprop init.svc.$1)" == "running" ]; then
		warn "failed ot start $1: already running!"
		return 0
	fi

	info "starting $1..."
	start $1
}

function svc_stop {
	if [ "$(getprop init.svc.$1)" == "stopped" ]; then
		warn "failed ot start $1: already stopped!"
		return 0
	fi

	warn "stopping $1..."
	stop $1
}

function svc_status {
	info "status of $1: $(getprop init.svc.$1)"
}

debug "rilchip command: \"$RILCHIP_STATE\""

if [ "$RILCHIP_STATE" == "init" ]; then

	if [ ! -f /proc/simslot_count ]; then
		error "cannot find /proc/simslot_count..."
		exit 0
	fi

	SIMSLOT_COUNT=$(cat /proc/simslot_count)
	debug "SIMSLOT_COUNT = $SIMSLOT_COUNT"

	if [ "$SIMSLOT_COUNT" == "1" ]; then
		# RIL configuration
		$RILCHIP_RESETPROP ro.multisim.simslotcount        1
		$RILCHIP_RESETPROP persist.radio.multisim.config   none
		$RILCHIP_RESETPROP ro.telephony.ril.config         simactivation
	elif [ "$SIMSLOT_COUNT" == "2" ]; then
		# RIL configuration
		$RILCHIP_RESETPROP ro.multisim.simslotcount        2
		$RILCHIP_RESETPROP persist.radio.multisim.config   dsds
		$RILCHIP_RESETPROP ro.telephony.ril.config         simactivation
	fi

elif [ "$RILCHIP_STATE" == "start" ]; then

	SIMSLOT_COUNT=$(cat /proc/simslot_count)
	debug "SIMSLOT_COUNT = $SIMSLOT_COUNT"

	# start modem boot daemon
	svc_start cpboot-daemon

	# start slot1 daemon
	svc_start ril-daemon

	if [ "$SIMSLOT_COUNT" == "2" ]; then
		# start slot2 daemon
		svc_start ril-daemon1
	fi

elif [ "$RILCHIP_STATE" == "stop" ]; then

	SIMSLOT_COUNT=$(cat /proc/simslot_count)
	debug "SIMSLOT_COUNT = $SIMSLOT_COUNT"

	# stop modem boot daemon
	svc_stop cpboot-daemon

	# stop slot1 daemon
	svc_stop ril-daemon

	if [ "$SIMSLOT_COUNT" == "2" ]; then
		# stop slot2 daemon
		svc_stop ril-daemon1
	fi

elif [ "$RILCHIP_STATE" == "restart" ]; then

	# stop everything
	/system/bin/sh $0 stop

	# wait for modem-shutdown
	debug "waiting for modem shutdown... (5s)"
	sleep 5

	# start everything again
	/system/bin/sh $0 start

elif [ "$RILCHIP_STATE" == "status" ]; then

	SIMSLOT_COUNT=$(cat /proc/simslot_count)
	debug "SIMSLOT_COUNT = $SIMSLOT_COUNT"

	# dump modem boot daemon
	svc_status cpboot-daemon

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
