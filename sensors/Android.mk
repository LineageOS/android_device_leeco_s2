LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE := android.hardware.sensors@1.0-service.s2
LOCAL_INIT_RC := android.hardware.sensors@1.0-service.s2.rc
LOCAL_SRC_FILES := \
        service.cpp \

LOCAL_SHARED_LIBRARIES := \
        liblog \
        libcutils \
        libdl \
        libbase \
        libutils \

LOCAL_SHARED_LIBRARIES += \
        libhidlbase \
        libhidltransport \
        android.hardware.sensors@1.0 \

include $(BUILD_EXECUTABLE)
