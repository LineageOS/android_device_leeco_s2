package org.lineageos.consumerirtransmitter.utils;

import static org.lineageos.consumerirtransmitter.Constants.LOG_ENABLE;

public class Log {
    public static void i(String TAG, String str) {
        if (LOG_ENABLE) {
            android.util.Log.i(TAG, str);
        }
    }

    public static void d(String TAG, String str) {
        if (LOG_ENABLE) {
            android.util.Log.d(TAG, str);
        }
    }

    public static void d(String TAG, String str, Throwable e) {
        // print out exception log unconditional
        android.util.Log.d(TAG, str, e);
    }

    public static void w(String TAG, String str) {
        if (LOG_ENABLE) {
            android.util.Log.w(TAG, str);
        }
    }

    public static void e(String TAG, String str) {
        // print out error log unconditional
        android.util.Log.d(TAG, str);
    }

    public static void e(String TAG, String str, Throwable e) {
        // print out exception log unconditional
        android.util.Log.d(TAG, str, e);
    }
}
