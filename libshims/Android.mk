# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2018 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# libshims_ims
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := media/libstagefright/foundation/MediaBuffer.cpp

LOCAL_SHARED_LIBRARIES := libstagefright_foundation libui libgui

LOCAL_CFLAGS := -Wno-unused-private-field

LOCAL_MODULE := libshims_ims
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)


# libshims_camera
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    bionic/bionic_time_conversions.cpp \
    bionic/pthread_cond.cpp

LOCAL_SHARED_LIBRARIES := libc

LOCAL_MODULE := libshims_camera
LOCAL_MODULE_TAGS := optional

LOCAL_32_BIT_ONLY := true

include $(BUILD_SHARED_LIBRARY)


# libshims_rild_socket
include $(CLEAR_VARS)

LOCAL_SRC_FILES := rild_socket/rild_socket.c

LOCAL_MODULE := libshims_rild_socket
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

# fake print lib for hexedited fingerprint libs
include $(CLEAR_VARS)

LOCAL_SHARED_LIBRARIES := \
    liblog

LOCAL_SRC_FILES := fakelogprint/fakelogprint.cpp

LOCAL_MODULE := fakelogprint
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
include $(BUILD_SHARED_LIBRARY)
