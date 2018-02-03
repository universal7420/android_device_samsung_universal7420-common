# Common Device Tree for the Samsung Galaxy S6

-> Work In Progress. <-

-> Force-pushing is likely to happen! <-

### Copyright

	Copyright (C) 2015-2016 Brandon McAnsh <brandon.mcansh@gmail.com>
	Copyright (C) 2017 Aljoshua (J0SH1X) Hell
	Copyright (C) 2017 Luca (RakuDev) Polesel <lucapolesel@outlook.it>
	Copyright (C) 2017 Lukas (Lukas0610) Berger <mail@lukasberger.at>
	Copyright (C) 2017 Philip (corewell) Jacobs <p.jcs@icloud.com>
	Copyright (C) 2017 Usaamah (Usaamah99) Patel


### Disclaimer
	/*
	 * Your warranty will be VOID after installing a custom ROM.
	 *
	 * Get notified yourself that I am not responsible for bricked devices, dead SD cards,
	 * thermonuclear war, or you getting fired because the alarm app failed.
	 * Please do some research if you have any concerns about features included in this ROM
	 * before flashing it!
	 * YOU are choosing to make these modifications,
	 * and if you point the finger at me for messing up your device,
	 * I will laugh at you.
	 */


### Setup for non-LineageOS-sources
 1. Make sure the device-tree (zerofltexx, zeroltexx, ...) itself has support for your wanted ROM
 2. Before building the ROM, make sure you have applied all required commits (You'll find them at "Commands for non-LineageOS-sources") for a successful build & boot


### Commands for non-LineageOS-sources
```
git -C build/soong fetch https://github.com/TeamNexus/android_build_soong nx-8.1
git -C frameworks/av cherry-pick e37f6db4c0b2d8380378fc132ca97900895ea591

git -C frameworks/native fetch https://github.com/TeamNexus/android_frameworks_av nx-8.1
git -C frameworks/av cherry-pick 395604e4cfc894bf063d831e7f9af54b5f44e0b3 96cc6439d2cb51efa9803e562ae427a9a1d54e23

git -C frameworks/native fetch https://github.com/TeamNexus/android_frameworks_native nx-8.1
git -C frameworks/native cherry-pick e11e1f5e007817cef2974e33bd039a35e344a1cd

# This commit needs special attentions, you'll need to adjust the path to the vendor-repo your ROM uses
git -C vendor/[YOUR ROM'S VENDOR] fetch https://github.com/TeamNexus/android_vendor_lineage nx-8.1
git -C vendor/[YOUR ROM'S VENDOR] cherry-pick f5c1d15d2e7823bc70240d2d5b430b6524bed710
```

### Further Credits
 * [@minealex2244](https://github.com/minealex2244),
 * [@Noxxxious](https://github.com/Noxxxious) and 
 * [@Wyzp](https://github.com/Wyzp) for their help on the kernel


### License
Apache License 2.0 (Apache-2.0) (Located at LICENSE, read more at [tldrlegal.com/license/apache-license-2.0](https://tldrlegal.com/license/apache-license-2.0-%28apache-2.0%29))

Short (no legal advice) summary of the used license:


**Can**

 * Commercial Use
 * Modify
 * Distribute
 * Sublicense
 * Private Use
 * Use Patent Claims
 * Place Warranty
 

**Cannot**

 * Hold Liable
 * Use Trademark 


**Must**

 * **Include Copyright**
 * **Include License**
 * **State Changes**
 * **Include Notice**
