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

#define LOG_TAG "libinit_sec"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <android-base/properties.h>
#include <log/log.h>

#include "property_service.h"
#include "vendor_init.h"

#include "init_sec.h"

using namespace std;
using namespace android;

namespace android {
namespace init {

void vendor_load_properties()
{    
    string device_orig = base::GetProperty("ro.product.device", "");
    string bootloader = base::GetProperty("ro.bootloader", "");
    string model, device, product;
    bool dualsim = false;

/*
 * Flat
 */
if (bootloader.find("G920") != string::npos) {

    product = "zeroflte";

    if (bootloader.find("G920F") != string::npos) {
        if (device_orig != "zeroflteduo") {
            model = "SM-G920F";
            device = "zerofltexx";
        }
    } else if (bootloader.find("G920I") != string::npos) {
        model = "SM-G920I";
        device = "zerofltexx";
    } else if (bootloader.find("G920K") != string::npos) {
        model = "SM-G920K";
        device = "zerofltektt";
    } else if (bootloader.find("G920L") != string::npos) {
        model = "SM-G920L";
        device = "zerofltelgt";
    } else if (bootloader.find("G920P") != string::npos) {
        model = "SM-G920P";
        device = "zerofltespr";
    } else if (bootloader.find("G920S") != string::npos) {
        model = "SM-G920S";
        device = "zeroflteskt";
    } else if (bootloader.find("G920T") != string::npos) {
        model = "SM-G920T";
        device = "zerofltetmo";
    } else if (bootloader.find("G920W8") != string::npos) {
        model = "SM-G920W8";
        device = "zerofltecan";
    }
}

/*
 * Edge
 */
else if (bootloader.find("G925") != string::npos) {

     product = "zerolte";

     if (bootloader.find("G925F") != string::npos) {
        if (device_orig != "zeroflteduo") {
            model = "SM-G925F";
            device = "zeroltexx";
        }
    } else if (bootloader.find("G925I") != string::npos) {
        model = "SM-G925I";
        device = "zeroltexx";
    } else if (bootloader.find("G925K") != string::npos) {
        model = "SM-G925K";
        device = "zeroltektt";
    } else if (bootloader.find("G925L") != string::npos) {
        model = "SM-G925L";
        device = "zeroltelgt";
    } else if (bootloader.find("G925P") != string::npos) {
        model = "SM-G925P";
        device = "zeroltespr";
    } else if (bootloader.find("G925S") != string::npos) {
        model = "SM-G925S";
        device = "zerolteskt";
    } else if (bootloader.find("G925T") != string::npos) {
        model = "SM-G925T";
        device = "zeroltetmo";
    } else if (bootloader.find("G925W8") != string::npos) {
        model = "SM-G925W8";
        device = "zeroltecan";
    }
}

/*
 * Edge Plus
 */
else if (bootloader.find("G928") != string::npos) {
    
    product = "zenlte";

     if (bootloader.find("G928F") != string::npos) {
        if (device_orig != "zenltexx") {
            model = "SM-G928F";
            device = "zenltexx";
        }	
    } else if (bootloader.find("G928C") != string::npos) {	
        model = "SM-G928C";
        device = "zenltejv";	
    } else if (bootloader.find("G928I") != string::npos) {	
        model = "SM-G928I";	
        device = "zenltedv";	
    } else if (bootloader.find("G928G") != string::npos) {	
        model = "SM-G928G";
        device = "zenltedd";	
    } else if (bootloader.find("G928K") != string::npos) {	
        model = "SM-G928K";
        device = "zenltektt";
    } else if (bootloader.find("G928L") != string::npos) {	
        model = "SM-G928L";
        device = "zenltelgt";
    } else if (bootloader.find("G928S") != string::npos) {	
        model = "SM-G928S";
        device = "zenlteskt";
    } else if (bootloader.find("G928T") != string::npos) {	
        model = "SM-G928T";
        device = "zenltetmo";
    } else if (bootloader.find("G928W8") != string::npos) {	
        model = "SM-G928W8";
        device = "zenltecan";
    }
}

/*	
 * Note 5	
 */
else if (bootloader.find("N920") != string::npos) {

     product = "noblelte";
	
     if (bootloader.find("N920C") != string::npos) {	
        if (device_orig != "nobleltejv") {	
            model = "SM-N920C";
            device = "nobleltejv";
            dualsim = true;
        }
    } else if (bootloader.find("N9208") != string::npos) {	
        model = "SM-N9208";
        device = "nobleltezt";
        dualsim = true;
    } else if (bootloader.find("N920G") != string::npos) {	
        model = "SM-N920G";
        device = "nobleltedd";
    } else if (bootloader.find("N920I") != string::npos) {	
        model = "SM-N920I";
        device = "nobleltedv";
    } else if (bootloader.find("N920P") != string::npos) {	
        model = "SM-N920P";
        device = "nobleltespr";
    } else if (bootloader.find("N920S") != string::npos) {	
        model = "SM-N920S";
        device = "noblelteskt";
    } else if (bootloader.find("N920K") != string::npos) {	
        model = "SM-N920K";
        device = "nobleltektt";
    } else if (bootloader.find("N920L") != string::npos) {	
        model = "SM-N920L";
        device = "nobleltelgt";
    } else if (bootloader.find("N920T") != string::npos) {	
        model = "SM-N920T";
        device = "nobleltetmo";
    } else if (bootloader.find("N920W8") != string::npos) {	
        model = "SM-N920W8";
        device = "nobleltecan";
    }
}

    // load original properties
    string description_orig = base::GetProperty("ro.build.description", "");
    string fingerprint_orig = base::GetProperty("ro.build.fingerprint", "");

    // replace device-names with correct one
    if (device_orig != "") {
        if (description_orig != "")
            replace(description_orig, device_orig, device);

        if (fingerprint_orig != "")
            replace(fingerprint_orig, device_orig, device);
    }

    // update properties
    property_override("ro.product.model", model);
    property_override("ro.product.device", device);
    property_override("ro.build.product", product);
    property_override("ro.lineage.device", device);
    property_override("ro.vendor.product.device", device);
    property_override("ro.build.description", description_orig);
    property_override("ro.build.fingerprint", fingerprint_orig);
    property_override("ro.boot.warranty_bit", "0");
    property_override("ro.warranty_bit", "0");
    property_override("ro.boot.veritymode", "enforcing");
    property_override("ro.boot.verifiedbootstate", "green");
    property_override("ro.boot.flash.locked", "1");
    property_override("ro.boot.ddrinfo", "00000001");
    property_override("ro.build.selinux", "1");
    property_override("ro.fmp_config", "1");
    property_override("ro.boot.fmp_config", "1");
    property_override("sys.oem_unlock_allowed", "0");
    // set model-specific properties
    if (dualsim == true) {
        property_override("persist.multisim.config", "dsds");
        property_override("persist.radio.multisim.config", "dsds");
        property_override("ro.multisim.simslotcount", "2");
        property_override("ro.telephony.default_network", "9,9");
    }
}

}
}
