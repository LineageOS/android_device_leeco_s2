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

#define LOG_TAG "ConsumerIrService"

#include "ConsumerIr.h"

#include <android-base/logging.h>
#include <fcntl.h>

#include <iostream>
#include <vector>

namespace android {
namespace hardware {
namespace ir {
namespace V1_0 {
namespace implementation {

static hidl_vec<ConsumerIrFreqRange> rangeVec{
    {.min = 30000, .max = 30000}, {.min = 33000, .max = 33000}, {.min = 36000, .max = 36000},
    {.min = 38000, .max = 38000}, {.min = 40000, .max = 40000}, {.min = 56000, .max = 56000},
};

int ConsumerIr::sendMsg(const char* msg) {
    int localsocket = -1, len;
    struct sockaddr_un remote;
    if ((localsocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        LOG(ERROR) << "could not create socket";
        return -1;
    }
    char const* name = "org.lineageos.consumerirtransmitter.localsocket";
    remote.sun_path[0] = '\0'; /* abstract namespace */
    strcpy(remote.sun_path + 1, name);
    remote.sun_family = AF_UNIX;
    int nameLen = strlen(name);
    len = 1 + nameLen + offsetof(struct sockaddr_un, sun_path);
    if (connect(localsocket, (struct sockaddr*)&remote, len) == -1) {
        LOG(ERROR) << "connect to local socket server failed, is the server running?";
        close(localsocket);
        return -1;
    } else {
        LOG(DEBUG) << "connect to local socket server success";
    }
    if (send(localsocket, msg, strlen(msg), 0) == -1) {
        LOG(ERROR) << "send msg to local socket server failed";
        close(localsocket);
        return -1;
    } else {
        LOG(DEBUG) << "send msg to local socket server success";
    }
    close(localsocket);
    return 0;
}

// Methods from ::android::hardware::ir::V1_0::IConsumerIr follow.
Return<bool> ConsumerIr::transmit(int32_t carrierFreq, const hidl_vec<int32_t>& pattern) {
    if (pattern.size() > 0) {
        std::ostringstream vts;
        std::copy(pattern.begin(), pattern.end(), std::ostream_iterator<int32_t>(vts, ","));
        vts << carrierFreq;
        std::string msg = vts.str();
        if (sendMsg(msg.c_str()) < 0) {
            LOG(ERROR) << "send msg failed";
            return false;
        } else {
            LOG(DEBUG) << "send msg success";
            return true;
        }
    }
    return false;
}

Return<void> ConsumerIr::getCarrierFreqs(getCarrierFreqs_cb _hidl_cb) {
    _hidl_cb(true, rangeVec);
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace ir
}  // namespace hardware
}  // namespace android
