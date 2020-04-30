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

package org.lineageos.consumerirtransmitter.utils;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import org.lineageos.consumerirtransmitter.beans.IRCMDBean;

public class IRCMDCacheManager {
    private static final String TAG = "ConsumerirTransmitter";

    private static final int QUEUE_MAX_SIZE = 100;
    private BlockingQueue<IRCMDBean> mCacheQueue = new LinkedBlockingQueue(QUEUE_MAX_SIZE);

    private volatile static IRCMDCacheManager mInstance;
    private IRCMDCacheManager() {}

    public static IRCMDCacheManager getInstance() {
        if (null == mInstance) {
            synchronized (IRCMDCacheManager.class) {
                if (null == mInstance) {
                    mInstance = new IRCMDCacheManager();
                }
            }
        }
        return mInstance;
    }

    public synchronized void produce(IRCMDBean ircmdBean) {
        if (mCacheQueue.offer(ircmdBean)) {
            Log.d(TAG, "IRCMDCacheManager: produce a cmd success: " + ircmdBean);
        } else {
            Log.d(TAG, "IRCMDCacheManager: queue is full delete the item from head");
            IRCMDBean dropped = mCacheQueue.poll();
            Log.d(TAG, "IRCMDCacheManager: IR command dropped: " + dropped);
            if (mCacheQueue.offer(ircmdBean)) {
                Log.d(TAG, "IRCMDCacheManager: produce a cmd success: " + ircmdBean);
            } else {
                Log.d(TAG, "IRCMDCacheManager: produce a cmd failed: " + ircmdBean);
            }
        }
    }

    public synchronized IRCMDBean consume() {
        if (isEmpty()) {
            return null;
        }
        IRCMDBean ircmdBean = mCacheQueue.poll();
        if (null != ircmdBean) {
            Log.d(TAG, "IRCMDCacheManager: consume a cmd:  " + ircmdBean);
        }
        return ircmdBean;
    }

    public synchronized boolean isEmpty() { return mCacheQueue.isEmpty(); }

    public synchronized void clear() { mCacheQueue.clear(); }
}
