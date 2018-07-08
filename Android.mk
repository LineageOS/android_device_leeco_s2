#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

ifneq ($(filter s2, $(TARGET_DEVICE)),)
include $(call all-makefiles-under,$(LOCAL_PATH))

include $(CLEAR_VARS)

KEYMASTER_IMAGES := keymaster.b00 keymaster.b01 keymaster.b02 keymaster.b03 keymaster.mdt
KEYMASTER_SYMLINKS := $(addprefix $(TARGET_OUT_VENDOR)/firmware/keymaster/,$(notdir $(KEYMASTER_IMAGES)))
$(KEYMASTER_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "Keymaster firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(KEYMASTER_SYMLINKS)


WIDEVINE_IMAGES := widevine.b00 widevine.b01 widevine.b02 widevine.b03 widevine.mdt
WIDEVINE_SYMLINKS := $(addprefix $(TARGET_OUT_VENDOR)/firmware/,$(notdir $(WIDEVINE_IMAGES)))
$(WIDEVINE_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "Widevine firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(WIDEVINE_SYMLINKS)

IMS_LIBS := libimscamera_jni.so libimsmedia_jni.so
IMS_SYMLINKS := $(addprefix $(TARGET_OUT_APPS)/ims/lib/arm64/,$(notdir $(IMS_LIBS)))
$(IMS_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "IMS lib link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /system/lib64/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(IMS_SYMLINKS)

WCNSS_CFG_INI := $(TARGET_OUT_VENDOR)/firmware/wlan/prima/WCNSS_qcom_cfg.ini
$(WCNSS_CFG_INI): $(LOCAL_INSTALLED_MODULE)
	@echo "WCNSS_qcom_cfg.ini firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /data/misc/wifi/$(notdir $@) $@

WCNSS_DICT_DAT := $(TARGET_OUT_ETC)/firmware/wlan/prima/WCNSS_wlan_dictionary.dat
$(WCNSS_DICT_DAT): $(LOCAL_INSTALLED_MODULE)
	@echo "WCNSS_wlan_dictionary.dat firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /persist/$(notdir $@) $@

WLAN_MAC := $(TARGET_OUT_ETC)/firmware/wlan/prima/wlan_mac.bin
$(WLAN_MAC): $(LOCAL_INSTALLED_MODULE)
	@echo "wlan_mac.bin firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /persist/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(WCNSS_CFG_INI) $(WCNSS_DICT_DAT) $(WLAN_MAC)


CMNLIB_IMAGES := cmnlib.b00 cmnlib.b01 cmnlib.b02 cmnlib.b03 cmnlib.mdt
CMNLIB_SYMLINKS := $(addprefix $(TARGET_OUT_VENDOR)/firmware/,$(notdir $(CMNLIB_IMAGES)))
$(CMNLIB_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "TZ Apps firmware link: $@"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf /firmware/image/$(notdir $@) $@

ALL_DEFAULT_INSTALLED_MODULES += $(CMNLIB_SYMLINKS)

# RFS symlinks
RFS_SYMLINKS := $(TARGET_OUT)/rfs
$(RFS_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "RFS links"
	@rm -rf $(TARGET_OUT)/rfs
	@mkdir -p $(TARGET_OUT)/rfs/apq/gnss/readonly
	$(hide) ln -sf /persist/hlos_rfs/shared $(TARGET_OUT)/rfs/apq/gnss/hlos
	$(hide) ln -sf /data/tombstones/lpass $(TARGET_OUT)/rfs/apq/gnss/ramdumps
	$(hide) ln -sf /persist/rfs/apq/gnss $(TARGET_OUT)/rfs/apq/gnss/readwrite
	$(hide) ln -sf /persist/rfs/shared $(TARGET_OUT)/rfs/apq/gnss/shared
	$(hide) ln -sf /firmware $(TARGET_OUT)/rfs/apq/gnss/readonly/firmware
	@mkdir -p $(TARGET_OUT)/rfs/msm/adsp/readonly
	$(hide) ln -sf /persist/hlos_rfs/shared $(TARGET_OUT)/rfs/msm/adsp/hlos
	$(hide) ln -sf /data/tombstones/lpass $(TARGET_OUT)/rfs/msm/adsp/ramdumps
	$(hide) ln -sf /persist/rfs/msm/adsp $(TARGET_OUT)/rfs/msm/adsp/readwrite
	$(hide) ln -sf /persist/rfs/shared $(TARGET_OUT)/rfs/msm/adsp/shared
	$(hide) ln -sf /firmware $(TARGET_OUT)/rfs/msm/adsp/readonly/firmware
	@mkdir -p $(TARGET_OUT)/rfs/msm/mpss/readonly
	$(hide) ln -sf /persist/hlos_rfs/shared $(TARGET_OUT)/rfs/msm/mpss/hlos
	$(hide) ln -sf /data/tombstones/lpass $(TARGET_OUT)/rfs/msm/mpss/ramdumps
	$(hide) ln -sf /persist/rfs/msm/mpss $(TARGET_OUT)/rfs/msm/mpss/readwrite
	$(hide) ln -sf /persist/rfs/shared $(TARGET_OUT)/rfs/msm/mpss/shared
	$(hide) ln -sf /firmware $(TARGET_OUT)/rfs/msm/mpss/readonly/firmware

ALL_DEFAULT_INSTALLED_MODULES += $(RFS_SYMLINKS)

endif
