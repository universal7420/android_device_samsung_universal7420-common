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
public class MdnieDisplay {

    private static final String PATH_MDNIE_MODE = "/sys/class/mdnie/mdnie/mode";
    private static final String PATH_MDNIE_SCENARIO = "/sys/class/mdnie/mdnie/scenario";
    private static final String PATH_MDNIE_SENSORRGB = "/sys/class/mdnie/mdnie/sensorRGB";

	public static boolean supportsMdnieMode() {
		return NexusFileUtils.isAccessible(PATH_MDNIE_MODE);
	}

	public static boolean supportsMdnieScenarios() {
		return NexusFileUtils.isAccessible(PATH_MDNIE_SCENARIO);
	}

	public static boolean supportsColorCorrection() {
		return NexusFileUtils.isAccessible(PATH_MDNIE_SENSORRGB);
	}

	public static HashMap<String, String> getMdnieModes() {
		HashMap<String, String> map = new HashMap<String, String>();

		map.put("Standard", "1");
		map.put("Dynamic", "0");
		map.put("Natural", "2");
		map.put("Movie", "3");
		map.put("Auto", "4");

		return map;
	}

	public static HashMap<String, String> getMdnieScenarios() {
		HashMap<String, String> map = new HashMap<String, String>();

		map.put("UI", "0");
		map.put("Video", "1");
		map.put("Camera", "4");
		map.put("Navigation", "5");
		map.put("Gallery", "6");
		map.put("VT", "7");
		map.put("Browser", "8");
		map.put("E-Book", "9");
		map.put("E-Mail", "10");
		map.put("HMT-8", "11");
		map.put("HMT-16", "12");

		return map;
	}
	
	public static int getMdnieMode() {
		return Integer.parseInt(NexusFileUtils.readLine(PATH_MDNIE_MODE));
	}
	
	public static void setMdnieMode(int mode) {
		NexusFileUtils.writeLine(PATH_MDNIE_MODE, Integer.toString(mode));
	}
	
	public static int getMdnieScenario() {
		return Integer.parseInt(NexusFileUtils.readLine(PATH_MDNIE_SCENARIO));
	}
	
	public static void setMdnieScenario(int scenario) {
		NexusFileUtils.writeLine(PATH_MDNIE_SCENARIO, Integer.toString(scenario));
	}
	
	public static void setColorCorrection(int r, int g, int b) {
		NexusFileUtils.writeLine(PATH_MDNIE_SENSORRGB, r + " " + g + " " + b);
	}
	
	public static int[] getColorCorrection() {
		String[] rgb = NexusFileUtils.readLine(PATH_MDNIE_MODE).split(" ");
		if (rgb.length != 3)
			return new int[] { 255, 255, 255 };
		
		return new int[] {
			Integer.parseInt(rgb[0]),
			Integer.parseInt(rgb[1]),
			Integer.parseInt(rgb[2]),
		};
	}

}
