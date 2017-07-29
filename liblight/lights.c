/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (C) 2014 The Linux Foundation. All rights reserved.
 * Copyright (C) 2015 The CyanogenMod Project
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

#define LOG_TAG "lights"
#include <cutils/log.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>

#include <sys/types.h>

#include <hardware/lights.h>

/******************************************************************************/

static pthread_once_t g_init = PTHREAD_ONCE_INIT;
static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static struct light_state_t g_notification;
static struct light_state_t g_battery;
static struct light_state_t g_attention;

char const*const RED_LED_FILE
        = "/sys/class/leds/red/brightness";

char const*const RED_BLINK_FILE
	= "/sys/class/leds/red/blink";

char const*const GREEN_BLINK_FILE
	= "/sys/class/leds/green/blink";

char const*const BLUE_BLINK_FILE
	= "/sys/class/leds/blue/blink";

char const*const GREEN_LED_FILE
        = "/sys/class/leds/green/brightness";

char const*const BLUE_LED_FILE
        = "/sys/class/leds/blue/brightness";

char const*const LCD_FILE
        = "/sys/class/leds/lcd-backlight/brightness";

char const*const RED_BREATH_FILE
        = "/sys/class/leds/red/led_time";

char const*const GREEN_BREATH_FILE
        = "/sys/class/leds/green/led_time";

char const*const BLUE_BREATH_FILE
        = "/sys/class/leds/blue/led_time";

char const*const BUTTON_FILE
        = "/sys/class/leds/button-backlight/brightness";

struct color {
    unsigned int r, g, b;
    float _L, _a, _b;
};

// this hardware only allows primary colors
static struct color colors[] = {
    { 255,   0,   0, 0, 0, 0 }, // red
    { 255, 255,   0, 0, 0, 0 }, // yellow
    {   0, 255,   0, 0, 0, 0 }, // green
    {   0, 255, 255, 0, 0, 0 }, // cyan
    {   0,   0, 255, 0, 0, 0 }, // blue
    { 255,   0, 255, 0, 0, 0 }, // magenta
    { 255, 255, 255, 0, 0, 0 }, // white
    { 127, 127, 127, 0, 0, 0 }, // grey
    {   0,   0,   0, 0, 0, 0 }, // black
};

#define MAX_COLOR 9

// Convert RGB to L*a*b colorspace
// from http://www.brucelindbloom.com
static void rgb2lab(unsigned int R, unsigned int G, unsigned int B,
                    float *_L, float *_a, float *_b) {

    float r, g, b, X, Y, Z, fx, fy, fz, xr, yr, zr;
    float Ls, as, bs;
    float eps = 216.f / 24389.f;
    float k = 24389.f / 27.f;

    float Xr = 0.964221f;  // reference white D50
    float Yr = 1.0f;
    float Zr = 0.825211f;

    // RGB to XYZ
    r = R / 255.f; //R 0..1
    g = G / 255.f; //G 0..1
    b = B / 255.f; //B 0..1

    // assuming sRGB (D65)
    if (r <= 0.04045)
        r = r / 12;
    else
        r = (float) pow((r + 0.055) / 1.055, 2.4);

    if (g <= 0.04045)
        g = g / 12;
    else
        g = (float) pow((g + 0.055) / 1.055, 2.4);

    if (b <= 0.04045)
        b = b / 12;
    else
        b = (float) pow((b + 0.055) / 1.055, 2.4);


    X = 0.436052025f * r + 0.385081593f * g + 0.143087414f * b;
    Y = 0.222491598f * r + 0.71688606f * g + 0.060621486f * b;
    Z = 0.013929122f * r + 0.097097002f * g + 0.71418547f * b;

    // XYZ to Lab
    xr = X / Xr;
    yr = Y / Yr;
    zr = Z / Zr;

    if (xr > eps)
        fx = (float) pow(xr, 1 / 3.);
    else
        fx = (float) ((k * xr + 16.) / 116.);

    if (yr > eps)
        fy = (float) pow(yr, 1 / 3.);
    else
        fy = (float) ((k * yr + 16.) / 116.);

    if (zr > eps)
        fz = (float) pow(zr, 1 / 3.);
    else
        fz = (float) ((k * zr + 16.) / 116);

    Ls = (116 * fy) - 16;
    as = 500 * (fx - fy);
    bs = 200 * (fy - fz);

    *_L = (2.55 * Ls + .5);
    *_a = (as + .5);
    *_b = (bs + .5);
}

/**
 * device methods
 */

void init_globals(void)
{
    int i = 0;
    for (i = 0; i < MAX_COLOR; i++) {
        rgb2lab(colors[i].r, colors[i].g, colors[i].b,
                &colors[i]._L, &colors[i]._a, &colors[i]._b);
    }

    // init the mutex
    pthread_mutex_init(&g_lock, NULL);

}

static int
write_int(char const* path, int value)
{
    int fd;
    static int already_warned = 0;

    fd = open(path, O_RDWR);
    if (fd >= 0) {
        char buffer[20];
        int bytes = snprintf(buffer, sizeof(buffer), "%d\n", value);
        ssize_t amt = write(fd, buffer, (size_t)bytes);
        close(fd);
        return amt == -1 ? -errno : 0;
    } else {
        if (already_warned == 0) {
            ALOGE("write_int failed to open %s\n", path);
            already_warned = 1;
        }
        return -errno;
    }
}

static int
write_str(char const* path, char *value)
{
    int fd;
    static int already_warned = 0;

    fd = open(path, O_RDWR);
    if (fd >= 0) {
        char buffer[20];
        ssize_t amt = write(fd, value, (size_t)strlen(value));
        close(fd);
        return amt == -1 ? -errno : 0;
    } else {
        if (already_warned == 0) {
            ALOGE("write_str failed to open %s\n", path);
            already_warned = 1;
        }
        return -errno;
    }
}

static int
is_lit(struct light_state_t const* state)
{
    return state->color & 0x00ffffff;
}

static int
rgb_to_brightness(struct light_state_t const* state)
{
    int color = state->color & 0x00ffffff;
    return ((77*((color>>16)&0x00ff))
            + (150*((color>>8)&0x00ff)) + (29*(color&0x00ff))) >> 8;
}

// find the color with the shortest distance
static struct color *
nearest_color(unsigned int r, unsigned int g, unsigned int b)
{
    int i = 0;
    float _L, _a, _b;
    double L_dist, a_dist, b_dist, total;
    double distance = 3 * 255;

    struct color *nearest = NULL;

    rgb2lab(r, g, b, &_L, &_a, &_b);

    ALOGV("%s: r=%d g=%d b=%d L=%f a=%f b=%f", __func__,
            r, g, b, _L, _a, _b);

    for (i = 0; i < MAX_COLOR; i++) {
        L_dist = pow(_L - colors[i]._L, 2);
        a_dist = pow(_a - colors[i]._a, 2);
        b_dist = pow(_b - colors[i]._b, 2);
        total = sqrt(L_dist + a_dist + b_dist);
        ALOGV("%s: total %f distance %f", __func__, total, distance);
        if (total < distance) {
            nearest = &colors[i];
            distance = total;
        }
    }

    return nearest;
}

static int
set_light_backlight(struct light_device_t* dev,
        struct light_state_t const* state)
{
    int err = 0;
    int brightness = rgb_to_brightness(state);
    if(!dev) {
        return -1;
    }
    pthread_mutex_lock(&g_lock);
    err = write_int(LCD_FILE, brightness);
    pthread_mutex_unlock(&g_lock);
    return err;
}

static int
set_speaker_light_locked(struct light_device_t* dev,
        struct light_state_t const* state)
{
    int red, green, blue;
    int blink;
    int onMS, offMS;
    unsigned int colorRGB;
    char breath_pattern[64] = { 0, };
    struct color *nearest = NULL;

    if(!dev) {
        return -1;
    }

    write_int(RED_LED_FILE, 0);
    write_int(GREEN_LED_FILE, 0);
    write_int(BLUE_LED_FILE, 0);

    if (state == NULL) {
        return 0;
    }

    switch (state->flashMode) {
        case LIGHT_FLASH_TIMED:
            onMS = state->flashOnMS;
            offMS = state->flashOffMS;
            break;
        case LIGHT_FLASH_NONE:
        default:
            onMS = 0;
            offMS = 0;
            break;
    }

    colorRGB = state->color;

    ALOGD("set_speaker_light_locked mode %d, colorRGB=%08X, onMS=%d, offMS=%d\n",
            state->flashMode, colorRGB, onMS, offMS);

    red = (colorRGB >> 16) & 0xFF;
    green = (colorRGB >> 8) & 0xFF;
    blue = colorRGB & 0xFF;

    blink = onMS > 0 && offMS > 0;

    if (blink) {
        // Driver doesn't permit us to set individual duty cycles, so only
        // pick pure colors at max brightness when blinking.
        nearest = nearest_color(red, green, blue);

        red = nearest->r;
        green = nearest->g;
        blue = nearest->b;

        // Make sure the values are between 1 and 7 seconds
        if (onMS < 1000)
            onMS = 1000;
        else if (onMS > 7000)
            onMS = 7000;

        if (offMS < 1000)
            offMS = 1000;
        else if (offMS > 7000)
            offMS = 7000;

        // ramp up, lit, ramp down, unlit. in seconds.
        sprintf(breath_pattern,"1 %d 1 %d",(int)(onMS/1000),(int)(offMS/1000));

    } else {
        blink = 0;
        sprintf(breath_pattern,"1 2 1 2");
    }

    // Do everything with the lights out, then turn up the brightness
    write_str(RED_BREATH_FILE, breath_pattern);
    write_int(RED_BLINK_FILE, (blink && red ? 1 : 0));
    write_str(GREEN_BREATH_FILE, breath_pattern);
    write_int(GREEN_BLINK_FILE, (blink && green ? 1 : 0));
    write_str(BLUE_BREATH_FILE, breath_pattern);
    write_int(BLUE_BLINK_FILE, (blink && blue ? 1 : 0));

    write_int(RED_LED_FILE, red);
    write_int(GREEN_LED_FILE, green);
    write_int(BLUE_LED_FILE, blue);

    return 0;
}

static void
handle_speaker_battery_locked(struct light_device_t* dev)
{
    set_speaker_light_locked(dev, NULL);
    if (is_lit(&g_attention)) {
        set_speaker_light_locked(dev, &g_attention);
    } else if (is_lit(&g_notification)) {
        set_speaker_light_locked(dev, &g_notification);
    } else {
        set_speaker_light_locked(dev, &g_battery);
    }
}

static int
set_light_battery(struct light_device_t* dev,
        struct light_state_t const* state)
{
    pthread_mutex_lock(&g_lock);
    g_battery = *state;
    handle_speaker_battery_locked(dev);
    pthread_mutex_unlock(&g_lock);
    return 0;
}

static int
set_light_notifications(struct light_device_t* dev,
        struct light_state_t const* state)
{
    pthread_mutex_lock(&g_lock);
    g_notification = *state;
    handle_speaker_battery_locked(dev);
    pthread_mutex_unlock(&g_lock);
    return 0;
}

static int
set_light_attention(struct light_device_t* dev,
        struct light_state_t const* state)
{
    pthread_mutex_lock(&g_lock);

    g_attention = *state;
    if (state->flashMode == LIGHT_FLASH_HARDWARE) {
        if (g_attention.flashOnMS > 0 && g_attention.flashOffMS == 0) {
            g_attention.flashMode = LIGHT_FLASH_NONE;
        }
    } else if (state->flashMode == LIGHT_FLASH_NONE) {
        g_attention.color = 0;
    }
    handle_speaker_battery_locked(dev);

    pthread_mutex_unlock(&g_lock);

    return 0;
}

static int
set_light_buttons(struct light_device_t* dev,
        struct light_state_t const* state)
{
    int err = 0;
    if(!dev) {
        return -1;
    }
    pthread_mutex_lock(&g_lock);
    err = write_int(BUTTON_FILE, state->color & 0xFF);
    pthread_mutex_unlock(&g_lock);
    return err;
}

/** Close the lights device */
static int
close_lights(struct light_device_t *dev)
{
    if (dev) {
        free(dev);
    }
    return 0;
}


/******************************************************************************/

/**
 * module methods
 */

/** Open a new instance of a lights device using name */
static int open_lights(const struct hw_module_t* module, char const* name,
        struct hw_device_t** device)
{
    int (*set_light)(struct light_device_t* dev,
            struct light_state_t const* state);

    if (0 == strcmp(LIGHT_ID_BACKLIGHT, name))
        set_light = set_light_backlight;
    else if (0 == strcmp(LIGHT_ID_BATTERY, name))
        set_light = set_light_battery;
    else if (0 == strcmp(LIGHT_ID_NOTIFICATIONS, name))
        set_light = set_light_notifications;
    else if (0 == strcmp(LIGHT_ID_ATTENTION, name))
        set_light = set_light_attention;
    else if (0 == strcmp(LIGHT_ID_BUTTONS, name))
        set_light = set_light_buttons;
    else
        return -EINVAL;

    pthread_once(&g_init, init_globals);

    struct light_device_t *dev = malloc(sizeof(struct light_device_t));

    if(!dev)
        return -ENOMEM;

    memset(dev, 0, sizeof(*dev));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*)module;
    dev->common.close = (int (*)(struct hw_device_t*))close_lights;
    dev->set_light = set_light;

    *device = (struct hw_device_t*)dev;
    return 0;
}

static struct hw_module_methods_t lights_module_methods = {
    .open =  open_lights,
};

/*
 * The lights Module
 */
struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .version_major = 1,
    .version_minor = 0,
    .id = LIGHTS_HARDWARE_MODULE_ID,
    .name = "LeEco s2 Lights Module",
    .author = "The LineageOS Project",
    .methods = &lights_module_methods,
};
