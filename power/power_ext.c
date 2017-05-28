/*
 * Copyright (C) 2016 Adam Farden
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

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOG_TAG "CPUQuiet PowerHAL"
#include <cutils/properties.h>
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#define CPUQUIET_MIN_CPUS "/sys/devices/system/cpu/cpuquiet/nr_min_cpus"
#define CPUQUIET_MAX_CPUS "/sys/devices/system/cpu/cpuquiet/nr_power_max_cpus"
#define CPUQUIET_THERMAL_CPUS "/sys/devices/system/cpu/cpuquiet/nr_thermal_max_cpus"
#define RQBALANCE_BALANCE_LEVEL "/sys/devices/system/cpu/cpuquiet/rqbalance/balance_level"
#define RQBALANCE_UP_THRESHOLD "/sys/devices/system/cpu/cpuquiet/rqbalance/nr_run_thresholds"
#define RQBALANCE_DOWN_THRESHOLD "/sys/devices/system/cpu/cpuquiet/rqbalance/nr_down_run_thresholds"

#define LOW_MIN_CPUS "cpuquiet.low.min_cpus"
#define LOW_MAX_CPUS "cpuquiet.low.max_cpus"
#define LOW_POWER_BALANCE_LEVEL "rqbalance.low.balance_level"
#define LOW_POWER_UP_THRESHOLD "rqbalance.low.up_threshold"
#define LOW_POWER_DOWN_THRESHOLD "rqbalance.low.down_threshold"

#define NORMAL_MIN_CPUS "cpuquiet.normal.min_cpus"
#define NORMAL_MAX_CPUS "cpuquiet.normal.max_cpus"
#define NORMAL_POWER_BALANCE_LEVEL "rqbalance.normal.balance_level"
#define NORMAL_POWER_UP_THRESHOLD "rqbalance.normal.up_threshold"
#define NORMAL_POWER_DOWN_THRESHOLD "rqbalance.normal.down_threshold"

#define PROPERTY_VALUE_MAX 128

char low_min_cpus[PROPERTY_VALUE_MAX];
char low_max_cpus[PROPERTY_VALUE_MAX];
char low_balance[PROPERTY_VALUE_MAX];
char low_up[PROPERTY_VALUE_MAX];
char low_down[PROPERTY_VALUE_MAX];

char normal_min_cpus[PROPERTY_VALUE_MAX];
char normal_max_cpus[PROPERTY_VALUE_MAX];
char normal_balance[PROPERTY_VALUE_MAX];
char normal_up[PROPERTY_VALUE_MAX];
char normal_down[PROPERTY_VALUE_MAX];

extern int sysfs_write(char *path, char *s);

void cm_power_set_interactive_ext(int on) {

    ALOGI("CPUQUIET config loaded");

    property_get(LOW_MIN_CPUS, low_min_cpus, "0");
    ALOGI("LOW_MIN_CPUS: %s", low_min_cpus);
    property_get(LOW_MAX_CPUS, low_max_cpus, "0");
    ALOGI("LOW_MAX_CPUS: %s", low_max_cpus);
    property_get(LOW_POWER_BALANCE_LEVEL, low_balance, "0");
    ALOGI("LOW_POWER_BALANCE_LEVEL: %s", low_balance);
    property_get(LOW_POWER_UP_THRESHOLD, low_up, "0");
    ALOGI("LOW_POWER_UP_THRESHOLD: %s", low_up);
    property_get(LOW_POWER_DOWN_THRESHOLD, low_down, "0");
    ALOGI("LOW_POWER_DOWN_THRESHOLD: %s", low_down);

    property_get(NORMAL_MIN_CPUS, normal_min_cpus, "0");
    ALOGI("NORMAL_MIN_CPUS: %s", normal_min_cpus);
    property_get(NORMAL_MAX_CPUS, normal_max_cpus, "0");
    ALOGI("NORMAL_MAX_CPUS: %s", normal_max_cpus);
    property_get(NORMAL_POWER_BALANCE_LEVEL, normal_balance, "0");
    ALOGI("NORMAL_POWER_BALANCE_LEVEL: %s", normal_balance);
    property_get(NORMAL_POWER_UP_THRESHOLD, normal_up, "0");
    ALOGI("NORMAL_POWER_UP_THRESHOLD: %s", normal_up);
    property_get(NORMAL_POWER_DOWN_THRESHOLD, normal_down, "0");
    ALOGI("NORMAL_POWER_DOWN_THRESHOLD: %s", normal_down);

    // init thermal maximum prior to setting normal power profile
    sysfs_write(CPUQUIET_THERMAL_CPUS, normal_max_cpus);

     if (!on) {
        ALOGI("Setting low power mode");
			// MIN before MAX is intentional
		sysfs_write(CPUQUIET_MIN_CPUS, low_min_cpus);
		sysfs_write(CPUQUIET_MAX_CPUS, low_max_cpus);
		sysfs_write(RQBALANCE_BALANCE_LEVEL, low_balance);
		sysfs_write(RQBALANCE_UP_THRESHOLD, low_up);
		sysfs_write(RQBALANCE_DOWN_THRESHOLD, low_down);
    } else {
         ALOGI("Setting normal power mode");
			// MAX before MIN is intentional
		sysfs_write(CPUQUIET_MAX_CPUS, normal_max_cpus);
		sysfs_write(CPUQUIET_MIN_CPUS, normal_min_cpus);
		sysfs_write(RQBALANCE_BALANCE_LEVEL, normal_balance);
		sysfs_write(RQBALANCE_UP_THRESHOLD, normal_up);
		sysfs_write(RQBALANCE_DOWN_THRESHOLD, normal_down);
	}
}
