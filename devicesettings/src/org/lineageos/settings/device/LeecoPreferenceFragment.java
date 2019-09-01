/*
 * Copyright (C) 2018 The LineageOS Project
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

import android.os.Bundle;
import android.provider.Settings;
import androidx.preference.PreferenceFragment;
import androidx.preference.SwitchPreference;
import androidx.preference.Preference;
import androidx.preference.PreferenceScreen;
import android.util.Log;
import android.widget.Toast;

public class LeecoPreferenceFragment extends PreferenceFragment {
    private static final String KEY_CAMERA_RESTART = "key_camera_restart";
    private static final String KEY_CAMHAL3_ENABLE = "key_camera_hal3_enable";
    private Preference mCameraReset;
    private SwitchPreference mCamHal3Enable;

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        getActivity().getActionBar().setDisplayHomeAsUpEnabled(true);
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        addPreferencesFromResource(R.xml.leeco_settings_panel);
        final PreferenceScreen prefSet = getPreferenceScreen();
        mCameraReset = (Preference) findPreference(KEY_CAMERA_RESTART);
        mCamHal3Enable = (SwitchPreference) findPreference(KEY_CAMHAL3_ENABLE);
        mCamHal3Enable.setChecked(SettingsUtils.cameraHAL3Enable());
        mCamHal3Enable.setOnPreferenceChangeListener(mCameraHal3PrefListener);

        if (SettingsUtils.supportsCameraRestartSwitch()) {
            mCameraReset.setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {
                public boolean onPreferenceClick(Preference preference) {
                    SettingsUtils.writeCameraRestartProp(getActivity());
                    return true;
                }
            });
        } else {
            prefSet.removePreference(mCameraReset);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        getListView().setPadding(0, 0, 0, 0);
    }

    private Preference.OnPreferenceChangeListener mCameraHal3PrefListener =
        new Preference.OnPreferenceChangeListener() {
            @Override
            public boolean onPreferenceChange(Preference preference, Object value) {
                final String key = preference.getKey();

                if (KEY_CAMHAL3_ENABLE.equals(key)) {
                    boolean enabled = (boolean) value;
                    SettingsUtils.writeCameraHAL3Prop(enabled);
                }

                return true;
            }
        };
}
