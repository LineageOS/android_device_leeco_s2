#
# Copyright (C) 2018-2020 The LineageOS Project
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
#

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit from s2 device
$(call inherit-product, $(LOCAL_PATH)/device.mk)

# Inherit some common LineageOS stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# Device identifier. This must come after all inclusions.
PRODUCT_BRAND := LeEco
PRODUCT_DEVICE := s2
PRODUCT_MANUFACTURER := LeMobile
PRODUCT_NAME := lineage_s2

PRODUCT_GMS_CLIENTID_BASE := android-leeco

PRODUCT_BUILD_PROP_OVERRIDES += \
    TARGET_DEVICE=le_s2_ww \
    PRODUCT_NAME=Le2_WW \
    PRIVATE_BUILD_DESC="s2-user 6.0.1 IIXOSOP5801910121S 44 release-keys"

BUILD_FINGERPRINT := Letv/Le2_WW/le_s2_ww:6.0.1/IIXOSOP5801910121S/44:user/release-keys
