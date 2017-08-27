/*
 * Copyright (C) 2017 TeamNexus
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

#define LOG_TAG "libsec-ril-wrapper"
#define LOG_NDEBUG 0

#include <dlfcn.h>

#include <cutils/log.h>
#include <telephony/ril.h>
#include <libril/ril_ex.h>

typedef void (*RILEnv_OnRequestComplete)(RIL_Token t, RIL_Errno e,
										 void *response, size_t responselen);

#if defined(ANDROID_MULTI_SIM)
typedef void (*RILEnv_OnUnsolicitedResponse)(int unsolResponse, const void *data, size_t datalen, RIL_SOCKET_ID socket_id);
#else
typedef void (*RILEnv_OnUnsolicitedResponse)(int unsolResponse, const void *data, size_t datalen);
#endif

typedef void (*RILEnv_RequestTimedCallback)(RIL_TimedCallback callback,
											void *param, const struct timeval *relativeTime);

typedef void (*RILEnv_OnRequestAck)(RIL_Token t);

void RILEnv_OnRequestComplete_Wrapper(RIL_Token t, RIL_Errno e, void *response, size_t responselen);
#if defined(ANDROID_MULTI_SIM)
void RILEnv_OnUnsolicitedResponse_Wrapper(int unsolResponse, const void *data, size_t datalen, RIL_SOCKET_ID socket_id);
#else
void RILEnv_OnUnsolicitedResponse_Wrapper(int unsolResponse, const void *data, size_t datalen);
#endif
void RILEnv_RequestTimedCallback_Wrapper(RIL_TimedCallback callback, void *param, const struct timeval *relativeTime);
void RILEnv_OnRequestAck_Wrapper(RIL_Token t);

void RIL_Unsol_Signal_Strength(RIL_SignalStrength_v10 *data);

static const struct RIL_Env *rildEnv;
static struct RIL_Env wrapperEnv = {
	RILEnv_OnRequestComplete_Wrapper,
	RILEnv_OnUnsolicitedResponse_Wrapper,
	RILEnv_RequestTimedCallback_Wrapper,
	RILEnv_OnRequestAck_Wrapper
};

const RIL_RadioFunctions *RIL_Init(const struct RIL_Env *env, int argc, char **argv) {
	int i;
	const char *rilLibPath = NULL; // vendor ril lib path either passed in as -l parameter
	const char *clientId = NULL; // ril/socket id received as -c parameter, otherwise set to 0
	void *dlHandle; // handle for vendor ril lib
	const RIL_RadioFunctions *(*rilInit)(const struct RIL_Env *, int, char **); // Pointer to ril init function in vendor ril

	for (i = 1; i < argc ;) {
		if (0 == strcmp(argv[i], "-l") && (argc - i > 1)) {
			rilLibPath = argv[i + 1];
			i += 2;
		} else if (0 == strcmp(argv[i], "-c") &&  (argc - i > 1)) {
			clientId = argv[i + 1];
			i += 2;
		}
	}

	if (rilLibPath == NULL) {
		RLOGE("failed to get rilLibPath: %s", dlerror());
		exit(EXIT_FAILURE);
	}

	dlHandle = dlopen(rilLibPath, RTLD_NOW);
	if (dlHandle == NULL) {
		RLOGE("dlopen failed: %s", dlerror());
		exit(EXIT_FAILURE);
	}

	// load libsec-ril
	rilInit = (const RIL_RadioFunctions *(*)(const struct RIL_Env *, int, char **))
		dlsym(dlHandle, "RIL_Init");

	if (rilInit == NULL) {
		RLOGE("RIL_Init not defined or exported in %s\n", rilLibPath);
		exit(EXIT_FAILURE);
	}
	
	// store rild-functions	
	rildEnv = env;

	return rilInit(&wrapperEnv, argc, argv);
}

void RILEnv_OnRequestComplete_Wrapper(RIL_Token t, RIL_Errno e, void *response, size_t responselen)
{
	// pass to libsec-ril
	RLOGD("%s: pass OnRequestComplete", __func__);
	rildEnv->OnRequestComplete(t, e, response, responselen);
}

#if defined(ANDROID_MULTI_SIM)
void RILEnv_OnUnsolicitedResponse_Wrapper(int unsolResponse, const void *data, size_t datalen, RIL_SOCKET_ID socket_id)
#else
void RILEnv_OnUnsolicitedResponse_Wrapper(int unsolResponse, const void *data, size_t datalen)
#endif
{
	switch (unsolResponse) {
		case RIL_UNSOL_SIGNAL_STRENGTH:		
			RIL_Unsol_Signal_Strength((RIL_SignalStrength_v10 *)const_cast<void*>(data));
			break;
	}

	// pass to libsec-ril
	RLOGD("%s: pass OnUnsolicitedResponse(%d)", __func__, unsolResponse);
#if defined(ANDROID_MULTI_SIM)
	rildEnv->OnUnsolicitedResponse(unsolResponse, data, datalen, socket_id);
#else
	rildEnv->OnUnsolicitedResponse(unsolResponse, data, datalen);
#endif
}

void RILEnv_RequestTimedCallback_Wrapper(RIL_TimedCallback callback, void *param, const struct timeval *relativeTime)
{
	// pass to libsec-ril
	RLOGD("%s: pass RequestTimedCallback", __func__);
	rildEnv->RequestTimedCallback(callback, param, relativeTime);
}

void RILEnv_OnRequestAck_Wrapper(RIL_Token t)
{
	// pass to libsec-ril
	RLOGD("%s: pass OnRequestAck", __func__);
	rildEnv->OnRequestAck(t);
}

void RIL_Unsol_Signal_Strength(RIL_SignalStrength_v10 *data) {
	// gsm
	data->GW_SignalStrength.signalStrength &= 0xff;

	// cdma
	data->CDMA_SignalStrength.dbm %= 256;
	data->EVDO_SignalStrength.dbm %= 256;

	// lte
	data->LTE_SignalStrength.signalStrength &= 0xff;
}
