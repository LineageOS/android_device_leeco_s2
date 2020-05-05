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
#include <sys/types.h>
#include <unistd.h>

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

// Methods from ::android::hardware::ir::V1_0::IConsumerIr follow.
Return<bool> ConsumerIr::transmit(int32_t carrierFreq, const hidl_vec<int32_t>& pattern) {
    if (pattern.size() > 0) {
        std::ostringstream vts;
        // Convert all but the last element to avoid a trailing ","
        std::copy(pattern.begin(), pattern.end() - 1, std::ostream_iterator<int32_t>(vts, ","));
        vts << pattern[pattern.size() - 1];
        std::stringstream ss;
        ss << "/system/bin/am broadcast -a "
              "org.lineageos.consumerirtransmitter.TRANSMIT_IR --es carrier_freq "
           << carrierFreq << " --es pattern " << vts.str();
        int child = fork();
        if (child == 0) {
            execl("/system/bin/sh", "sh", "-c", ss.str().c_str(), (char*)0);
        }
        return true;
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
