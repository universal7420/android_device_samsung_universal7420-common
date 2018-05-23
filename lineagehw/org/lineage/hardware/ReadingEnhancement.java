/*
 * Copyright (C) 2018 The LineageOS Project
 * Copyright (C) 2018 TeamNexus
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

package org.lineageos.hardware;

import org.lineageos.internal.util.FileUtils;

/**
 * Reader mode
 */
public class ReadingEnhancement {

	private static final String ACCESSIBILITY_FILE = "/sys/devices/13900000.dsim/lcd/panel/mdnie/accessibility";

	public static boolean isSupported() {
		return FileUtils.isFileWritable(ACCESSIBILITY_FILE);
	}

    public static boolean setGrayscale(boolean state) {
		return FileUtils.writeLine(ACCESSIBILITY_FILE,
			(state ? "5" : "0"));
    }

}
