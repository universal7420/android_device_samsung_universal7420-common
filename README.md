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


### Changes required for non-LineageOS-sources
```
https://github.com/TeamNexus/android_bionic / {TOP}/bionic/
1fb540533f7b269dd54631f805b4ac20c995a81f f1ef3b274683b656a5d2922e31a020cfe235d021 a6fb1155c030661d320d895ae39b78211f57a646

https://github.com/TeamNexus/android_build_soong / {TOP}/build/soong/
e37f6db4c0b2d8380378fc132ca97900895ea591

https://github.com/TeamNexus/android_frameworks_av / {TOP}/frameworks/av
cc5a4634f0cd99c50a2f506e6d232d4efeb72125

https://github.com/TeamNexus/android_frameworks_native / {TOP}/frameworks/native/
0a311ebee77b7fe5d7483285131c489251371029

#
# This commit needs special attentions, you'll need to adjust the path to the
# vendor-repo which is used by your ROM
#
https://github.com/TeamNexus/android_vendor_lineage  / {TOP}/vendor/[***]
61f5e144d59cb1a1eab55efd5acff82534ec5767 10084bf8672eab8b6f747fcdd038313dfa2885b8 52602baa49f6dc63519aa039b60e522f1b496480 
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
