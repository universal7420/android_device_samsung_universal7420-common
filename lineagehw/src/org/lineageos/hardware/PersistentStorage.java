/*
 * Copyright (C) 2015 The CyanogenMod Project
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

import android.util.Slog;

import java.io.File;
import java.io.UnsupportedEncodingException;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

/**
 * Support for storage of key-value pairs which persists across device wipes /
 * factory resets. This is used for various security related features. A
 * reasonable implementation of this needs to store items on a partition which
 * is unaffected by factory reset. The actual format of the storage is left up
 * to the implementation. The implementation in this file is suitable for
 * devices which have a /persist partition (recent QCOM devices fit this
 * criteria).
 */
public class PersistentStorage {

    private static final String TAG = "PersistentStorage";

    private static final ReadWriteLock rwl = new ReentrantReadWriteLock();

	static { System.loadLibrary("/vendor/lib64/libzerops.so"); }
	
	/* bool vendor::nexus::zero::PersistentStorage::available() */
	private static native boolean _ZN6vendor5nexus4zero17PersistentStorage9availableEv();

	/* bool vendor::nexus::zero::PersistentStorage::getCString(const char *, char **) */
	private static native boolean _ZN6vendor5nexus4zero17PersistentStorage10getCStringEPKcPPc(String[] params);

	/* bool vendor::nexus::zero::PersistentStorage::putCString(const char *, const char *) */
	private static native boolean _ZN6vendor5nexus4zero17PersistentStorage10putCStringEPKcS4_(String[] params);

    /**
     * Whether device supports persistent properties
     *
     * @return boolean Supported devices must return always true
     */
    public static boolean isSupported() {
        return _ZN6vendor5nexus4zero17PersistentStorage9availableEv();
    }

    /**
     * Gets an array of bytes from persistent storage.
     *
     * @param key
     * @return previously stored byte array, null if not found
     */
    public static byte[] get(String key) {
        try {
			return getString(key).getBytes("UTF-8");
		} catch (UnsupportedEncodingException e) {
            Slog.e(TAG, e.getMessage(), e);
			return null;
        }
    }

    /**
     * Gets a string from persistent storage.
     *
     * @param key
     * @return previously stored string, null if not found
     */
    public static String getString(String key) {
        if (!isSupported()) {
            return null;
        }

        try {
            rwl.readLock().lock();

			String[] params = {
				key, /* [In]  key  */
				null /* [Out] data */
			};

			if (!_ZN6vendor5nexus4zero17PersistentStorage10getCStringEPKcPPc(params)) {
				Slog.e(TAG, "Native implementation failed to gather data");
				return null;
			}

			Slog.i(TAG, "Gathered data for \"" + key + "\"");
			return params[1];
        } catch (Exception e) {
            Slog.e(TAG, e.getMessage(), e);
        } finally {
            rwl.readLock().unlock();
        }

        return null;
	}

    /**
     * Writes a byte array to persistent storage.
     *
     * The storage of the data is implementation specific. In this implementation,
     * we hash the key with SHA-256 and use the hex string as the stored key.
     *
     * Implementations should delete the key if a null buffer is passed.
     *
     * @param key
     * @param buffer
     * @return true if the operation succeeded
     */
    public static boolean set(String key, byte[] buffer) {
        try {
			return setString(key, new String(buffer, "UTF-8"));
		} catch (UnsupportedEncodingException e) {
            Slog.e(TAG, e.getMessage(), e);
			return false;
        }
    }

    /**
     * Writes a string to persistent storage.
     *
     * The storage of the data is implementation specific. In this implementation,
     * we hash the key with SHA-256 and use the hex string as the stored key.
     *
     * Implementations should delete the key if a null buffer is passed.
     *
     * @param key
     * @param buffer
     * @return true if the operation succeeded
     */
    public static boolean setString(String key, String buffer) {
        if (!isSupported()) {
            return false;
        }

        try {
            rwl.writeLock().lock();

			String[] params = {
				key,   /* [In] key  */
				buffer /* [In] data */
			};

			if (!_ZN6vendor5nexus4zero17PersistentStorage10putCStringEPKcS4_(params)) {
				Slog.e(TAG, "Native implementation failed to store data");
				return false;
			}

			Slog.i(TAG, "Stored data for \"" + key + "\"");
			return true;
        } catch (Exception e) {
            Slog.e(TAG, e.getMessage(), e);
        } finally {
            rwl.writeLock().unlock();
        }

        return false;
    }

}
