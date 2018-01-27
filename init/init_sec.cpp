/*
   Copyright (c) 2015, The Dokdo Project. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   File Name : init_sec.c
   Create Date : 2015.11.03
   Author : Sunghun Ra
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <android-base/properties.h>

#include "log.h"
#include "property_service.h"
#include "util.h"
#include "vendor_init.h"

enum device_variant {
    UNKNOWN = -1,
    G920S,
    G920K,
    G920L,
    G920P,
    G920I,
    G920F,
    G925F,
    G925S,
    G925K,
    G925L,
    G925I,
    G925W8,
    G920W8
};

namespace android {
namespace init {

void vendor_load_properties()
{	
    std::string platform = android::base::GetProperty("ro.board.platform", "");
    std::string bootloader = android::base::GetProperty("ro.bootloader", "");
	device_variant variant = UNKNOWN;

    if (platform != ANDROID_TARGET) {
        return;
    }

    if (bootloader.find("G920S") != std::string::npos) {
        variant = G920S;
    } else if (bootloader.find("G920K") != std::string::npos) {
        variant = G920K;
    } else if (bootloader.find("G920L") != std::string::npos) {
        variant = G920L;
    } else if (bootloader.find("G920P") != std::string::npos) {
        variant = G920P;
    } else if (bootloader.find("G920I") != std::string::npos) {
        variant = G920I;
    } else if (bootloader.find("G920F") != std::string::npos) {
        variant = G920F;
    } else if (bootloader.find("G925F") != std::string::npos) {
        variant = G925F;
    } else if (bootloader.find("G925S") != std::string::npos) {
        variant = G925S;
    } else if (bootloader.find("G925K") != std::string::npos) {
        variant = G925K;
    } else if (bootloader.find("G925L") != std::string::npos) {
        variant = G925L;
    } else if (bootloader.find("G925I") != std::string::npos) {
        variant = G925I;
    } else if (bootloader.find("G925W8") != std::string::npos) {
        variant = G925W8;
    } else if (bootloader.find("G920W8") != std::string::npos) {
        variant = G920W8;
    } else {
		return;
    }

    switch (variant) {
        case G920S:
            /* zeroflteskt */
            android::base::SetProperty("ro.build.fingerprint", "samsung/zeroflteskt/zeroflteskt:5.1.1/LMY47X/G920SXXS3COK5:user/release-keys");
            android::base::SetProperty("ro.build.description", "zeroflteskt-user 5.1.1 LMY47X G920SXXS3COK5 release-keys");
            android::base::SetProperty("ro.product.model", "SM-G920S");
            android::base::SetProperty("ro.product.device", "zeroflteskt");
            break;
        case G920K:
            /* zerofltektt */
            android::base::SetProperty("ro.build.fingerprint", "samsung/zerofltektt/zerofltektt:5.1.1/LMY47X/G920KXXS3COK5:user/release-keys");
            android::base::SetProperty("ro.build.description", "zerofltektt-user 5.1.1 LMY47X G920KXXS3COK5 release-keys");
            android::base::SetProperty("ro.product.model", "SM-G920K");
            android::base::SetProperty("ro.product.device", "zerofltektt");
            break;
        case G920L:
            /* zerofltelgt */
            android::base::SetProperty("ro.build.fingerprint", "samsung/zerofltelgt/zerofltelgt:5.1.1/LMY47X/G920LXXS3COK5:user/release-keys");
            android::base::SetProperty("ro.build.description", "zerofltelgt-user 5.1.1 LMY47X G920LXXS3COK5 release-keys");
            android::base::SetProperty("ro.product.model", "SM-G920L");
            android::base::SetProperty("ro.product.device", "zerofltelgt");
            break;
        case G920P:
            /* zerofltespr */
            android::base::SetProperty("ro.build.fingerprint", "samsung/zerofltespr/zerofltespr:5.1.1/LMY47X/G920PVPU3BOL1:user/release-keys");
            android::base::SetProperty("ro.build.description", "zerofltespr-user 5.1.1 LMY47X G920PVPU3BOL1 release-keys");
            android::base::SetProperty("ro.product.model", "SM-G920P");
            android::base::SetProperty("ro.product.device", "zerofltespr");
            break;
        case G920I:
            /* zerofltexx */
            android::base::SetProperty("ro.build.fingerprint", "samsung/zerofltexx/zerofltexx:5.1.1/LMY47X/G920IXXS3COK5:user/release-keys");
            android::base::SetProperty("ro.build.description", "zerofltexx-user 5.1.1 LMY47X G920IXXS3COK5 release-keys");
            android::base::SetProperty("ro.product.model", "SM-G920I");
            android::base::SetProperty("ro.product.device", "zerofltexx");
            break;
        case G920F:
            /* zerofltexx */
            android::base::SetProperty("ro.build.fingerprint", "samsung/zerofltexx/zerofltexx:6.0.1/MMB29K/G920FXXU3DPBG:user/release-keys");
            android::base::SetProperty("ro.build.description", "zerofltexx-user 6.0.1 MMB29K G920FXXU3DPBG release-keys");
            android::base::SetProperty("ro.product.model", "SM-G920F");
            android::base::SetProperty("ro.product.device", "zerofltexx");
            break;
        case G925F:
            /* zeroltexx */
            android::base::SetProperty("ro.build.fingerprint", "samsung/zeroltexx/zerolte:6.0.1/MMB29K/G925FXXU5DPL4:user/release-keys");
            android::base::SetProperty("ro.build.description", "zeroltexx-user 6.0.1 MMB29K G925FXXU5DPL4 release-keys");
            android::base::SetProperty("ro.product.model", "SM-G925F");
            android::base::SetProperty("ro.product.device", "zerolte");
            break;
        case G925S:
            /* zerolteskt */
            android::base::SetProperty("ro.build.fingerprint", "samsung/zerolteskt/zerolteskt:5.1.1/LMY47X/G925SXXS3COK5:user/release-keys");
            android::base::SetProperty("ro.build.description", "zerolteskt-user 5.1.1 LMY47X G925SXXS3COK5 release-keys");
            android::base::SetProperty("ro.product.model", "SM-G925S");
            android::base::SetProperty("ro.product.device", "zerolteskt");
            break;
        case G925K:
            /* zeroltektt */
            android::base::SetProperty("ro.build.fingerprint", "samsung/zeroltelgt/zeroltelgt:5.1.1/LMY47X/G925KXXS3COK5:user/release-keys");
            android::base::SetProperty("ro.build.description", "zeroltektt-user 5.1.1 LMY47X G925KXXS3COK5 release-keys");
            android::base::SetProperty("ro.product.model", "SM-G925K");
            android::base::SetProperty("ro.product.device", "zeroltektt");
            break;
        case G925L:
            /* zeroltelgt */
            android::base::SetProperty("ro.build.fingerprint", "samsung/zeroltelgt/zeroltelgt:5.1.1/LMY47X/G925LXXS3COK5:user/release-keys");
            android::base::SetProperty("ro.build.description", "zeroltelgt-user 5.1.1 LMY47X G925LXXS3COK5 release-keys");
            android::base::SetProperty("ro.product.model", "SM-G925L");
            android::base::SetProperty("ro.product.device", "zeroltelgt");
            break;
        case G925I:
            /* zeroltexx */
            android::base::SetProperty("ro.build.fingerprint", "samsung/zeroltexx/zeroltexx:5.1.1/LMY47X/G925IXXS3COK5:user/release-keys");
            android::base::SetProperty("ro.build.description", "zeroltexx-user 5.1.1 LMY47X G925IXXS3COK5 release-keys");
            android::base::SetProperty("ro.product.model", "SM-G925I");
            android::base::SetProperty("ro.product.device", "zeroltexx");
            break;
        case G925W8:
           /* zeroltebmc */
           android::base::SetProperty("ro.build.fingerprint", "samsung/zeroltebmc/zeroltebmc:6.0.1/MMB29K/G925W8VLU5CPK4:user/release-keys");
           android::base::SetProperty("ro.build.description", "zeroltebmc-user 6.0.1 MMB29K G925W8VLU5CPK4 release-keys");
           android::base::SetProperty("ro.product.model", "SM-G925W8");
           android::base::SetProperty("ro.product.device", "zeroltebmc");
           break;
        case G920W8:
           /* zerofltebmc */
           android::base::SetProperty("ro.build.fingerprint", "samsung/zerofltebmc/zerofltebmc:6.0.1/MM29K/G920W8VLU5CPK4:user/release-keys");
           android::base::SetProperty("ro.build.description", "zerofltebmc-user 6.0.1 MMB29K G920W8VLU5CPK4 release-keys");
           android::base::SetProperty("ro.product.model", "SM-G920W8");
           android::base::SetProperty("ro.product.device", "zerofltebmc");
           break;
        default:
            return;
    }
    std::string device = android::base::GetProperty("ro.product.device", "");
}

}
}
