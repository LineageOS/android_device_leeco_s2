/*
 * Copyright (C) 2016 The CyanogenMod Project
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

#ifndef _BDROID_BUILDCFG_H
#define _BDROID_BUILDCFG_H

#include <cutils/properties.h>
#include <string.h>

#define BTM_DEF_LOCAL_NAME "LeEco Le 2"

#define BT_CLEAN_TURN_ON_DISABLED 1

#define MAX_ACL_CONNECTIONS   16
#define MAX_L2CAP_CHANNELS    16

#define BLE_VND_INCLUDED   TRUE
#define BTM_SCO_ENHANCED_SYNC_ENABLED FALSE
// skips conn update at conn completion
#define BLE_PERIPHERAL_ADV_NAME FALSE
#define BTA_BLE_SKIP_CONN_UPD  FALSE
#define BTA_SKIP_BLE_READ_REMOTE_FEAT FALSE
#define BTA_BLE_SKIP_CONN_UPD  FALSE
#define BTM_LE_SECURE_CONN  TRUE

/* Increasing SEPs to 12 from 6 to support SHO/MCast i.e. two streams per codec */
#define AVDT_NUM_SEPS 12

/* Enable HFP WBS feature */
#define BTIF_HF_CLIENT_WBS_INCLUDED TRUE
#undef PROPERTY_VALUE_MAX
#endif
