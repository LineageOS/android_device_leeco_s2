# Boot animation
TARGET_SCREEN_WIDTH := 1080
TARGET_SCREEN_HEIGHT := 1920

# Inherit device configuration
$(call inherit-product, device/leeco/s2/full_s2.mk)

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := s2
PRODUCT_NAME := cm_s2
PRODUCT_BRAND := LeEco
PRODUCT_MODEL := LeEco Le 2
PRODUCT_MANUFACTURER := LeMobile

# Release name
PRODUCT_RELEASE_NAME := s2
