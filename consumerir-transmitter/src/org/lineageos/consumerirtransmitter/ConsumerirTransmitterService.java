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

package org.lineageos.consumerirtransmitter;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import org.lineageos.consumerirtransmitter.IControl;

public class ConsumerirTransmitterService extends Service {
    private static final String TAG = "ConsumerirTransmitter";
    private static final boolean DEBUG = false;

    private static final String ACTION_TRANSMIT_IR =
        "org.lineageos.consumerirtransmitter.TRANSMIT_IR";
    private static final String SYS_FILE_ENABLE_IR_BLASTER = "/sys/remote/enable";

    private boolean mBound = false;
    private IControl mControl;

    @Override
    public void onCreate() {
        if (DEBUG)
            Log.d(TAG, "Creating service");
        switchIr("1");
        bindQuickSetService();
        registerReceiver(mIrReceiver, new IntentFilter(ACTION_TRANSMIT_IR));
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (DEBUG)
            Log.d(TAG, "Starting service");
        String action = "unknown";
        if (null != intent && null != intent.getAction()) {
            action = intent.getAction();
        }
        if (ACTION_TRANSMIT_IR.equals(action)) {
            if (intent.getStringExtra("carrier_freq") != null
                && intent.getStringExtra("pattern") != null) {
                int carrierFrequency = Integer.parseInt(intent.getStringExtra("carrier_freq"));
                String patternStr = intent.getStringExtra("pattern");
                int[] pattern = Arrays.stream(patternStr.split(","))
                                    .map(String::trim)
                                    .mapToInt(Integer::parseInt)
                                    .toArray();
                transmitIrPattern(carrierFrequency, pattern);
            }
        }

        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        if (DEBUG)
            Log.d(TAG, "Destroying service");
        super.onDestroy();
        this.unregisterReceiver(mIrReceiver);
        this.unbindService(mControlServiceConnection);
        switchIr("0");
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    /**
     * Service Connection used to control the bound QuickSet SDK Service
     */
    private final ServiceConnection mControlServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mBound = true;
            mControl = new IControl(service);

            if (DEBUG)
                Log.i(TAG, "QuickSet SDK Service SUCCESSFULLY CONNECTED!");
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mBound = false;
            mControl = null;

            if (DEBUG)
                Log.i(TAG, "QuickSet SDK Service DISCONNECTED!");
        }
    };

    /**
     * Try to bind QuickSet SDK Service
     */
    public void bindQuickSetService() {
        if (DEBUG)
            Log.d(TAG,
                "Trying to bind QuickSet service: " + IControl.QUICKSET_UEI_PACKAGE_NAME + " - "
                    + IControl.QUICKSET_UEI_SERVICE_CLASS);
        try {
            Intent controlIntent = new Intent(IControl.ACTION);
            controlIntent.setClassName(
                IControl.QUICKSET_UEI_PACKAGE_NAME, IControl.QUICKSET_UEI_SERVICE_CLASS);
            boolean bindResult =
                bindService(controlIntent, mControlServiceConnection, Context.BIND_AUTO_CREATE);
            if (!bindResult && DEBUG) {
                Log.e(TAG, "Binding QuickSet Control service failed!");
            }
        } catch (Throwable t) {
            Log.e(TAG, "Binding QuickSet Control service failed!", t);
        }
    }

    /**
     * Try to send Infrared pattern, catch and log exceptions.
     *
     * @param carrierFrequency carrier frequency, see ConsumerIrManager Android
     *     API
     * @param pattern          IR pattern to send, see ConsumerIrManager Android
     *     API
     */
    public int transmitIrPattern(int carrierFrequency, int[] pattern) {
        if (DEBUG)
            Log.d(TAG,
                "transmitIrPattern called: freq: " + carrierFrequency
                    + ", pattern-len: " + pattern.length);

        if (mControl == null || !mBound) {
            if (DEBUG)
                Log.w(TAG, "QuickSet Service seems not to be bound. Trying to bind again and exit!");

            bindQuickSetService();
            return -1;
        }
        try {
            mControl.transmit(carrierFrequency, pattern);
            int resultCode = mControl.getLastResultcode();
            return resultCode;
        } catch (Throwable t) {
            Log.e(TAG, "Exception while trying to send command to QuickSet Service!", t);
            return -1;
        }
    }

    /**
     * Enable or disable Ir chip
     *
     * @param value 0 to disable, 1 to enable
     */
    private static boolean switchIr(String value) {
        File file = new File(SYS_FILE_ENABLE_IR_BLASTER);
        if (!file.exists() || !file.isFile() || !file.canWrite()) {
            return false;
        }
        try {
            FileWriter fileWriter = new FileWriter(file);
            fileWriter.write(value);
            fileWriter.flush();
            fileWriter.close();
        } catch (IOException e) {
            return false;
        }
        return true;
    }

    private BroadcastReceiver mIrReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (ACTION_TRANSMIT_IR.equals(action)) {
                if (intent.getStringExtra("carrier_freq") != null
                    && intent.getStringExtra("pattern") != null) {
                    int carrierFrequency = Integer.parseInt(intent.getStringExtra("carrier_freq"));
                    String patternStr = intent.getStringExtra("pattern");
                    int[] pattern = Arrays.stream(patternStr.split(","))
                                        .map(String::trim)
                                        .mapToInt(Integer::parseInt)
                                        .toArray();
                    transmitIrPattern(carrierFrequency, pattern);
                }
            }
        }
    }
}
