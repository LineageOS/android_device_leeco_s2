/*
 * Copyright (C) 2017 The LineageOS Project
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

#include <healthd.h>
#include <fcntl.h>

void healthd_board_init(struct healthd_config*)
{
    // use defaults
}


int healthd_board_battery_update(struct android::BatteryProperties*)
{
    return 1;
}

void healthd_board_mode_charger_draw_battery(struct android::BatteryProperties*)
{
    // use defaults
}

void healthd_board_mode_charger_battery_update(struct android::BatteryProperties*)
{
    // use defaults
}

void healthd_board_mode_charger_set_backlight(bool enable)
{
    int fd;
    int value;

    if (enable)
        value = 4095;
    else
        value = 0;

    fd = open("/sys/class/leds/lcd-backlight/brightness", O_RDWR);
    if (fd >= 0) {
        char buffer[4];
        int bytes = snprintf(buffer, sizeof(buffer), "%d", value);

        if (bytes >= 0 && bytes < 4)
            write(fd, buffer, bytes);

        close(fd);
    }
}

void healthd_board_mode_charger_init()
{
    // use defaults
}
