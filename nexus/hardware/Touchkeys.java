/*
 * Copyright (C) 2017 TeamNexus
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package nexus.hardware;

import java.util.HashMap;

import nexus.io.NexusFileUtils;

/**
 * @hide
 */
public class Touchkeys {

    private static final String PATH_TOUCHKEYS_STATE = "/sys/class/input/input0/enabled";
    private static final String PATH_TOUCHKEYS_BRIGHTNESS = "/sys/class/sec/sec_touchkey/brightness";

	public static boolean isSupported() {
		return NexusFileUtils.isAccessible(PATH_TOUCHKEYS_STATE);
	}

	public static boolean isBacklightSupported() {
		return NexusFileUtils.isAccessible(PATH_TOUCHKEYS_BRIGHTNESS);
	}

	public static boolean getState() {
		if (NexusFileUtils.isAccessible(PATH_TOUCHKEYS_STATE))
			return Integer.parseInt(NexusFileUtils.readLine(PATH_TOUCHKEYS_STATE)) != 0;

		return false;
	}

	public static void setState(boolean state) {
		if (NexusFileUtils.isAccessible(PATH_TOUCHKEYS_STATE))
			NexusFileUtils.writeLine(PATH_TOUCHKEYS_STATE, (state ? "1" : "0"));

		if (NexusFileUtils.isAccessible(PATH_TOUCHKEYS_BRIGHTNESS) && !state)
			NexusFileUtils.writeLine(PATH_TOUCHKEYS_BRIGHTNESS, "0");
	}

}
