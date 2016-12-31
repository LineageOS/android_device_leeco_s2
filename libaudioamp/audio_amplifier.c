/*
 * Copyright (C) 2013-2014, The CyanogenMod Project
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

#include <time.h>
#include <system/audio.h>
#include <voice.h>
#include <msm8916/platform.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#define LOG_NDEBUG 0
#define LOG_TAG "AudioAmp-tfa9890"
#include <log/log.h>

#include <hardware/audio_amplifier.h>
#include <tinyalsa/asoundlib.h>

extern int exTfa98xx_calibration(void);
extern int exTfa98xx_speakeron(uint32_t);
extern int exTfa98xx_speakeroff();

#define AMP_MIXER_CTL "Smart PA I2S"

typedef enum {
    SMART_PA_FOR_AUDIO = 0,
    SMART_PA_FOR_MUSIC = 0,
    SMART_PA_FOR_VOIP = 1,
    SMART_PA_FIND = 1,          /* ??? */
    SMART_PA_FOR_VOICE = 2,
    SMART_PA_MMI = 3,           /* ??? */
} smart_pa_mode_t;

typedef struct tfa9890_device {
    amplifier_device_t amp_dev;
    audio_mode_t mode;
} tfa9890_device_t;

static tfa9890_device_t *tfa9890_dev = NULL;

static int is_speaker(uint32_t snd_device) 
{
    int speaker = 0;

    switch (snd_device) {
        case SND_DEVICE_OUT_SPEAKER:
        case SND_DEVICE_OUT_SPEAKER_REVERSE:
        case SND_DEVICE_OUT_SPEAKER_AND_HEADPHONES:
        case SND_DEVICE_OUT_VOICE_SPEAKER:
        case SND_DEVICE_OUT_SPEAKER_AND_HDMI:
        case SND_DEVICE_OUT_SPEAKER_AND_USB_HEADSET:
        case SND_DEVICE_OUT_SPEAKER_AND_ANC_HEADSET:
            speaker = 1;
            break;
    }

    return speaker;
}

static int is_voice_speaker(uint32_t snd_device) 
{
    return snd_device == SND_DEVICE_OUT_VOICE_SPEAKER;
}

static int set_clocks_enabled(bool enable)
{
    enum mixer_ctl_type type;
    struct mixer_ctl *ctl;
    struct mixer *mixer = mixer_open(0);

    if (mixer == NULL) {
        ALOGE("%s: Error opening mixer 0'", __func__);
        return -1;
    }

    ctl = mixer_get_ctl_by_name(mixer, AMP_MIXER_CTL);
    if (ctl == NULL) {
        mixer_close(mixer);
        ALOGE("%s: Could not find %s\n", __func__, AMP_MIXER_CTL);
        return -ENODEV;
    }

    type = mixer_ctl_get_type(ctl);
    if (type != MIXER_CTL_TYPE_ENUM) {
        ALOGE("%s: %s is not supported\n", __func__, AMP_MIXER_CTL);
        mixer_close(mixer);
        return -ENOTTY;
    }

    mixer_ctl_set_value(ctl, 0, enable);
    mixer_close(mixer);
    return 0;
}

static int amp_set_mode(struct amplifier_device *device, audio_mode_t mode)
{
    int ret = 0;
    tfa9890_device_t *tfa9890 = (tfa9890_device_t*) device;

    tfa9890->mode = mode;
    return ret;
}

static int amp_enable_output_devices(hw_device_t *device, uint32_t devices, bool enable) 
{
    tfa9890_device_t *tfa9890 = (tfa9890_device_t*) device;
    int ret = 0;

    if (is_speaker(devices)) {

        if (enable) {
            smart_pa_mode_t mode;

            switch(tfa9890->mode) {
                case AUDIO_MODE_IN_CALL: 
                    mode = SMART_PA_FOR_VOICE;
                    break;
                case AUDIO_MODE_IN_COMMUNICATION: 
                    mode = SMART_PA_FOR_VOIP;
                    break;
                default: 
                    mode = SMART_PA_FOR_AUDIO;
            }

            set_clocks_enabled(true);

            if ((ret = exTfa98xx_speakeron(mode)) != 0) {
                ALOGE("%s: exTfa98xx_speakeron(%d) failed: %d\n", __func__, mode, ret);
            } else {
                ALOGD("%s: Amplifier on\n", __func__);
            }

        } else {

            if ((ret = exTfa98xx_speakeroff()) != 0) {
                ALOGE("%s: exTfa98xx_speakeroff failed: %d\n", __func__, ret);
            } else {
                ALOGD("%s: Amplifier off\n", __func__);
            }

            set_clocks_enabled(false);
        }

    }
    return 0;
}

static int amp_dev_close(hw_device_t *device) 
{
    tfa9890_device_t *tfa9890 = (tfa9890_device_t*) device;
    free(tfa9890);

    return 0;
}

static int amp_init(tfa9890_device_t *tfa9890) 
{   
    int ret = 0;

    set_clocks_enabled(true);
    if ((ret = exTfa98xx_calibration()) != 0) {
        ALOGE("%s: exTfa98xx_calibration failed: %d\n", __func__, ret);
    } else {
        ALOGD("%s: Amplifier calibrated\n", __func__);
    }
    set_clocks_enabled(false);

    return 0;
}

static int amp_module_open(const hw_module_t *module,
        __attribute__((unused)) const char *name, hw_device_t **device)
{
    
    if (tfa9890_dev) {
        ALOGE("%s:%d: Unable to open second instance of TFA9890 amplifier\n",
                __func__, __LINE__);
        return -EBUSY;
    }

    tfa9890_dev = calloc(1, sizeof(tfa9890_device_t));
    if (!tfa9890_dev) {
        ALOGE("%s:%d: Unable to allocate memory for amplifier device\n",
                __func__, __LINE__);
        return -ENOMEM;
    }

    tfa9890_dev->amp_dev.common.tag = HARDWARE_DEVICE_TAG;
    tfa9890_dev->amp_dev.common.module = (hw_module_t *) module;
    tfa9890_dev->amp_dev.common.version = HARDWARE_DEVICE_API_VERSION(1, 0);
    tfa9890_dev->amp_dev.common.close = amp_dev_close;

    tfa9890_dev->amp_dev.enable_output_devices = amp_enable_output_devices;
    tfa9890_dev->amp_dev.set_mode = amp_set_mode;

    if (amp_init(tfa9890_dev)) {
        free(tfa9890_dev);
        return -ENODEV;
    }

    *device = (hw_device_t *) tfa9890_dev;

    return 0;
}

static struct hw_module_methods_t hal_module_methods = {
    .open = amp_module_open,
};

amplifier_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AMPLIFIER_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AMPLIFIER_HARDWARE_MODULE_ID,
        .name = "S2 audio amplifier HAL",
        .author = "The LineageOS Project",
        .methods = &hal_module_methods,
    },
};
