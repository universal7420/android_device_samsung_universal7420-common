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

#include "log.h"
#include "property_service.h"
#include "util.h"
#include "vendor_init.h"

#include "init_sec.h"

std::string bootloader;
std::string device;
char* devicename;

device_variant check_device_and_get_variant()
{
    std::string platform = property_get("ro.board.platform");
    if (platform != ANDROID_TARGET) {
        return UNKNOWN;
    }

    bootloader = property_get("ro.bootloader");
    return match(bootloader);
}

void vendor_load_properties()
{
    device_variant variant = check_device_and_get_variant();

    switch (variant) {
        case G920S:
            /* zeroflteskt */
            property_set("ro.build.fingerprint", "samsung/zeroflteskt/zeroflteskt:5.1.1/LMY47X/G920SXXS3COK5:user/release-keys");
            property_set("ro.build.description", "zeroflteskt-user 5.1.1 LMY47X G920SXXS3COK5 release-keys");
            property_set("ro.product.model", "SM-G920S");
            property_set("ro.product.device", "zeroflteskt");
            break;
        case G920K:
            /* zerofltektt */
            property_set("ro.build.fingerprint", "samsung/zerofltektt/zerofltektt:5.1.1/LMY47X/G920KXXS3COK5:user/release-keys");
            property_set("ro.build.description", "zerofltektt-user 5.1.1 LMY47X G920KXXS3COK5 release-keys");
            property_set("ro.product.model", "SM-G920K");
            property_set("ro.product.device", "zerofltektt");
            break;
        case G920L:
            /* zerofltelgt */
            property_set("ro.build.fingerprint", "samsung/zerofltelgt/zerofltelgt:5.1.1/LMY47X/G920LXXS3COK5:user/release-keys");
            property_set("ro.build.description", "zerofltelgt-user 5.1.1 LMY47X G920LXXS3COK5 release-keys");
            property_set("ro.product.model", "SM-G920L");
            property_set("ro.product.device", "zerofltelgt");
            break;
        case G920P:
            /* zerofltespr */
            property_set("ro.build.fingerprint", "samsung/zerofltespr/zerofltespr:5.1.1/LMY47X/G920PVPU3BOL1:user/release-keys");
            property_set("ro.build.description", "zerofltespr-user 5.1.1 LMY47X G920PVPU3BOL1 release-keys");
            property_set("ro.product.model", "SM-G920P");
            property_set("ro.product.device", "zerofltespr");
            break;
        case G920I:
            /* zerofltexx */
            property_set("ro.build.fingerprint", "samsung/zerofltexx/zerofltexx:5.1.1/LMY47X/G920IXXS3COK5:user/release-keys");
            property_set("ro.build.description", "zerofltexx-user 5.1.1 LMY47X G920IXXS3COK5 release-keys");
            property_set("ro.product.model", "SM-G920I");
            property_set("ro.product.device", "zerofltexx");
            break;
        case G920F:
            /* zerofltexx */
            property_set("ro.build.fingerprint", "samsung/zerofltexx/zerofltexx:6.0.1/MMB29K/G920FXXU3DPBG:user/release-keys");
            property_set("ro.build.description", "zerofltexx-user 6.0.1 MMB29K G920FXXU3DPBG release-keys");
            property_set("ro.product.model", "SM-G920F");
            property_set("ro.product.device", "zerofltexx");
            break;
        default:
            ERROR("Unknown bootloader id %s detected. bailing...\n", bootloader.c_str());
            return;
    }
    device = property_get("ro.product.device");
    INFO("Found bootloader id %s setting build properties for %s device\n", bootloader.c_str(), device.c_str());
}

