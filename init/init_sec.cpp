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

    /*
     * Flat
     */
    device_variant variant = UNKNOWN;
    if (bootloader.find("G920F") != string::npos) {
        if (device_orig != "zeroflteduo") {
            variant = G920F;
        }
    } else if (bootloader.find("G920I") != string::npos) {
        variant = G920I;
    } else if (bootloader.find("G920K") != string::npos) {
        variant = G920K;
    } else if (bootloader.find("G920L") != string::npos) {
        variant = G920L;
    } else if (bootloader.find("G920P") != string::npos) {
        variant = G920P;
    } else if (bootloader.find("G920S") != string::npos) {
        variant = G920S;
    } else if (bootloader.find("G920T") != string::npos) {
        variant = G920T;
    } else if (bootloader.find("G920W8") != string::npos) {
        variant = G920W8;
    }

    /*
     * Edge
     */
    else if (bootloader.find("G925F") != string::npos) {
        if (device_orig != "zeroflteduo") {
            variant = G925F;
        }
    } else if (bootloader.find("G925I") != string::npos) {
        variant = G925I;
    } else if (bootloader.find("G925K") != string::npos) {
        variant = G925K;
    } else if (bootloader.find("G925L") != string::npos) {
        variant = G925L;
    } else if (bootloader.find("G925P") != string::npos) {
        variant = G925P;
    } else if (bootloader.find("G925S") != string::npos) {
        variant = G925S;
    } else if (bootloader.find("G925T") != string::npos) {
        variant = G925T;
    } else if (bootloader.find("G925W8") != string::npos) {
        variant = G925W8;
    } else {
        return;
    }

    /*
     * Edge, but...
     */
    if (bootloader.find("SCV31") != string::npos) {
        /* BIG, FAT TODO: ??? */
        variant = G925J;
    }

    /*	
     * Edge Plus	
     */	
    else if (bootloader.find("G928F") != string::npos) {	
        if (device_orig != "zenltexx") {	
            variant = G928F;	
        }	
    } else if (bootloader.find("G928C") != string::npos) {	
        variant = G928C;	
    } else if (bootloader.find("G928I") != string::npos) {	
        variant = G928I;	
    } else if (bootloader.find("G928G") != string::npos) {	
        variant = G928G;	
    } else if (bootloader.find("G928K") != string::npos) {	
        variant = G928K;	
    } else if (bootloader.find("G928L") != string::npos) {	
        variant = G928L;	
    } else if (bootloader.find("G928S") != string::npos) {	
        variant = G928S;	
    } else if (bootloader.find("G928T") != string::npos) {	
        variant = G928T;	
    } else if (bootloader.find("G928W8") != string::npos) {	
        variant = G928W8;	
    }	

    /*	
     * Note 5	
     */	
    else if (bootloader.find("N920C") != string::npos) {	
        if (device_orig != "nobleltejv") {	
            variant = N920C;	
        }	
    } else if (bootloader.find("N9208") != string::npos) {	
        variant = N9208;	
    } else if (bootloader.find("N920G") != string::npos) {	
        variant = N920G;	
    } else if (bootloader.find("N920I") != string::npos) {	
        variant = N920I;	
    } else if (bootloader.find("N920P") != string::npos) {	
        variant = N920P;
    } else if (bootloader.find("N920S") != string::npos) {	
        variant = N920S;	
    } else if (bootloader.find("N920K") != string::npos) {	
        variant = N920K;	
    } else if (bootloader.find("N920L") != string::npos) {	
        variant = N920L;	
    } else if (bootloader.find("N920T") != string::npos) {	
        variant = N920T;	
    } else if (bootloader.find("N920W8") != string::npos) {	
        variant = N920W8;	
    } else {	
        return;	
    }
    
    string model, device, product;
    bool dualsim;

    switch (variant) {

        /*
         * Flat
         */
        case G920F:
            /* zerofltexx */
            model = "SM-G920F";
            device = "zerofltexx";
            product = "zeroflte";
            break;

        case G920I:
            /* zerofltexx */
            model = "SM-G920I";
            device = "zerofltexx";
            product = "zeroflte";
            break;

        case G920K:
            /* zerofltektt */
            model = "SM-G920K";
            device = "zerofltektt";
            product = "zeroflte";
            break;

        case G920L:
            /* zerofltelgt */
            model = "SM-G920L";
            device = "zerofltelgt";
            product = "zeroflte";
            break;

        case G920P:
            /* zerofltespr */
            model = "SM-G920P";
            device = "zerofltespr";
            product = "zeroflte";
            break;

        case G920S:
            /* zeroflteskt */
            model = "SM-G920S";
            device = "zeroflteskt";
            product = "zeroflte";
            break;

        case G920T:
            /* zerofltetmo */
            model = "SM-G920T";
            device = "zerofltetmo";
            product = "zeroflte";
            break;

        case G920W8:
            /* zerofltecan */
            model = "SM-G920W8";
            device = "zerofltecan";
            product = "zeroflte";
            break;

        /*
         * Edge
         */
        case G925F:
            /* zeroltexx */
            model = "SM-G925F";
            device = "zeroltexx";
            product = "zerolte";
            break;

        case G925I:
            /* zeroltexx */
            model = "SM-G925I";
            device = "zeroltexx";
            product = "zerolte";
            break;

        case G925J:
            /* zeroltexx */
            model = "SCV31";
            device = "zeroltekdi";
            product = "zerolte";
            break;

        case G925K:
            /* zeroltektt */
            model = "SM-G925K";
            device = "zeroltektt";
            product = "zerolte";
            break;

        case G925L:
            /* zeroltelgt */
            model = "SM-G925L";
            device = "zeroltelgt";
            product = "zerolte";
            break;

        case G925P:
            /* zeroltespr */
            model = "SM-G925P";
            device = "zeroltespr";
            product = "zerolte";
            break;

        case G925S:
            /* zerolteskt */
            model = "SM-G925S";
            device = "zerolteskt";
            product = "zerolte";
            break;

        case G925T:
            /* zeroltetmo */
            model = "SM-G925T";
            device = "zeroltetmo";
            product = "zerolte";
            break;

        case G925W8:
            /* zeroltecan */
            model = "SM-G925W8";
            device = "zeroltecan";
            product = "zerolte";
            break;

        /*	
         * Edge Plus	
         */	
        case G928C:	
            /* zenltejv */	
            model = "SM-G928C";	
            device = "zenltejv";	
            product = "zenlte";	
            break;	

        case G928F:	
            /* zenltexx */	
            model = "SM-G928F";	
            device = "zenltexx";	
            product = "zenlte";	
            break;	

        case G928G:	
            /* zenltedd */	
            model = "SM-G928G";	
            device = "zenltedd";	
            product = "zenlte";	
            break;	

        case G928I:	
            /* zenltedv */	
            model = "SM-G928I";	
            device = "zenltedv";	
            product = "zenlte";	
            break;	

        case G928K:	
            /* zenltektt */	
            model = "SM-G928K";	
            device = "zenltektt";	
            product = "zenltektt";	
            break;	

        case G928L:	
            /* zenltelgt */	
            model = "SM-G928L";	
            device = "zenltelgt";	
            product = "zenltelgt";	
            break;	

        case G928S:	
            /* zenlteskt */	
            model = "SM-G928S";	
            device = "zenlteskt";	
            product = "zenlteskt";	
            break;	

        case G928T:	
            /* zenltetmo */	
            model = "SM-G928T";	
            device = "zenltetmo";	
            product = "zenltetmo";	
            break;	

        case G928W8:	
            /* zenltecan */	
            model = "SM-G928W8";	
            device = "zenltecan";	
            product = "zenltecan";	
            break;	

        /*	
         * Note 5	
         */	
        case N9208:	
            /* nobleltezt */	
            model = "SM-N9208";	
            device = "nobleltezt";	
            product = "noblelte";
            dualsim = true;
            break;	

        case N920C:	
            /* nobleltejv */	
            model = "SM-N920C";	
            device = "nobleltejv";	
            product = "noblelte";	
            dualsim = true;
            break;	

        case N920G:	
            /* nobleltedd */	
            model = "SM-N920G";	
            device = "nobleltedd";	
            product = "noblelte";	
            break;	

        case N920I:	
            /* nobleltedv */	
            model = "SM-N920I";	
            device = "nobleltedv";	
            product = "noblelte";	
            break;	

        case N920P:	
            /* nobleltespr */	
            model = "SM-N920P";	
            device = "nobleltespr";	
            product = "nobleltespr";	
            break;
            
        case N920S:	
            /* noblelteskt */	
            model = "SM-N920S";	
            device = "noblelteskt";	
            product = "noblelteskt";	
            break;	

        case N920K:	
            /* nobleltektt */	
            model = "SM-N920K";	
            device = "nobleltektt";	
            product = "nobleltektt";	
            break;	

        case N920L:	
            /* nobleltelgt */	
            model = "SM-N920L";	
            device = "nobleltelgt";	
            product = "nobleltelgt";	
            break;	

        case N920T:	
            /* nobleltetmo */	
            model = "SM-N920T";	
            device = "nobleltetmo";	
            product = "nobleltetmo";	
            break;	

        case N920W8:	
            /* nobleltecan */	
            model = "SM-N920W8";	
            device = "nobleltecan";	
            product = "nobleltecan";
            break;
                        
        default:
            return;
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
        property_override("ro.multisim.simslotcount", "2");
    }
}

}
}
