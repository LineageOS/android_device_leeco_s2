/*
   Copyright (c) 2013-2016, The Linux Foundation. All rights reserved.
   Copyright (c) 2017-2020, The LineageOS Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"
#include "property_service.h"

using android::init::property_set;

void property_override(const std::string& name, const std::string& value)
{
    size_t valuelen = value.size();

    prop_info* pi = (prop_info*) __system_property_find(name.c_str());
    if (pi != nullptr) {
        __system_property_update(pi, value.c_str(), valuelen);
    }
    else {
        int rc = __system_property_add(name.c_str(), name.size(), value.c_str(), valuelen);
        if (rc < 0) {
            LOG(ERROR) << "property_set(\"" << name << "\", \"" << value << "\") failed: "
                       << "__system_property_add failed";
        }
    }
}

void property_overrride_triple(const std::string& product_prop, const std::string& system_prop, const std::string& vendor_prop, const std::string& value)
{
    property_override(product_prop, value);
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

void vendor_load_properties()
{
    LOG(INFO) << "Loading vendor specific properties";
    std::string device = android::base::GetProperty("ro.leeco.devinfo", "");
    LOG(INFO) << "DEVINFO: " << device;

    if (device == "s2_open") {
        // This is X520
        property_overrride_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "X520");
    }
    else if (device == "s2_oversea") {
        // This is X522
        property_overrride_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "X522");
    }
    else if (device == "s2_india") {
        // This is X526
        property_overrride_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "X526");
    }
    else if (device == "s2_ww") {
        // This is X527
        property_overrride_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "X527");
    }
    else {
        // Unknown variant
        property_overrride_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "X52X");
        LOG(ERROR) << "Unable to set DEVINFO from ro.leeco.devinfo prop";
    }
    property_overrride_triple("ro.product.device", "ro.product.system.device", "ro.product.vendor.device", "le_s2_ww");
}
