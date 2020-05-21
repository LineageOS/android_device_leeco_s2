/*
 * Copyright (c) 2018 The LineageOS Project
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

package org.lineageos.settings.device;

import android.content.ContentResolver;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Build;
import android.os.SystemProperties;
import android.util.Log;
import android.widget.Toast;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

public class SettingsUtils {
    public static final String TAG = "SettingsUtils";
    public static final String CAMERA_RESTART_ENABLED = "CAMERA_RESTART_ENABLED";

    private static final String CAMERA_RESTART_PROPERTY = "persist.camera.s2.restart";

    private static final String CAMERA_HAL3_ENABLE_PROPERTY = "persist.camera.HAL3.enabled";
    private static final String CAMERA_HAL3_ENABLE = "CAMERA_HAL3_ENABLED";
    public static final String PREFERENCES = "SettingsUtilsPreferences";

    public static void writeCameraRestartProp(Context context) {
        SystemProperties.set(CAMERA_RESTART_PROPERTY, "1");
        Toast.makeText(context, context.getString(R.string.camera_reset_hint), Toast.LENGTH_SHORT)
            .show();
    }

    public static void writeCameraHAL3Prop(boolean enable) {
        SystemProperties.set(CAMERA_HAL3_ENABLE_PROPERTY, enable ? "1" : "0");
    }

    public static boolean cameraHAL3Enable() {
        String enable = SystemProperties.get(CAMERA_HAL3_ENABLE_PROPERTY, "1");
        return "1".equals(enable);
    }

    public static int getInt(Context context, String name, int def) {
        SharedPreferences settings = context.getSharedPreferences(PREFERENCES, 0);
        return settings.getInt(name, def);
    }

    public static boolean putInt(Context context, String name, int value) {
        SharedPreferences settings = context.getSharedPreferences(PREFERENCES, 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putInt(name, value);
        return editor.commit();
    }

    public static void registerPreferenceChangeListener(
        Context context, SharedPreferences.OnSharedPreferenceChangeListener preferenceListener) {
        SharedPreferences settings = context.getSharedPreferences(PREFERENCES, 0);
        settings.registerOnSharedPreferenceChangeListener(preferenceListener);
    }

    public static void unregisterPreferenceChangeListener(
        Context context, SharedPreferences.OnSharedPreferenceChangeListener preferenceListener) {
        SharedPreferences settings = context.getSharedPreferences(PREFERENCES, 0);
        settings.unregisterOnSharedPreferenceChangeListener(preferenceListener);
    }

    public static boolean supportsCameraRestartSwitch() { return true; }
}
