#!/sbin/sh
#
# Copyright (C) 2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# copy devinfo partition info to a vendor prop

DEVINFO=$(strings /dev/block/mmcblk0p25 | head -n 1)

echo "DEVINFO: ${DEVINFO}"

sed -i "s/ro.leeco.devinfo=NULL/ro.leeco.devinfo=$DEVINFO/g" "/mnt/system/system/build.prop"

exit 0
