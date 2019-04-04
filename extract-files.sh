#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2019 The LineageOS Project
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

set -e

DEVICE=s2
VENDOR=leeco

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "${MY_DIR}" ]]; then MY_DIR="${PWD}"; fi

LINEAGE_ROOT="${MY_DIR}/../../.."

HELPER="${LINEAGE_ROOT}/vendor/lineage/build/tools/extract_utils.sh"
if [ ! -f "${HELPER}" ]; then
    echo "Unable to find helper script at ${HELPER}"
    exit 1
fi
source "${HELPER}"

# Default to sanitizing the vendor folder before extraction
CLEAN_VENDOR=true
SECTION=
KANG=

while [ "$1" != "" ]; do
    case "$1" in
        -n | --no-cleanup )     CLEAN_VENDOR=false
                                ;;
        -k | --kang)            KANG="--kang"
                                ;;
        -s | --section )        shift
                                SECTION="$1"
                                CLEAN_VENDOR=false
                                ;;
        * )                     SRC="$1"
                                ;;
    esac
    shift
done

if [ -z "${SRC}" ]; then
    SRC=adb
fi

# Initialize the helper
setup_vendor "${DEVICE}" "${VENDOR}" "${LINEAGE_ROOT}" false "${CLEAN_VENDOR}"

extract "${MY_DIR}/proprietary-files.txt" "${SRC}" ${KANG} --section "${SECTION}"

extract "${MY_DIR}/proprietary-files-qc.txt" "${SRC}" ${KANG} --section "${SECTION}"


DEVICE_BLOB_ROOT="$LINEAGE_ROOT"/vendor/"$VENDOR"/"$DEVICE"/proprietary

patchelf --remove-needed libbacktrace.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpcmd
patchelf --remove-needed libunwind.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpcmd
patchelf --remove-needed libkeystore_binder.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpcmd
patchelf --remove-needed libsoftkeymasterdevice.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpcmd
patchelf --remove-needed libsoftkeymaster.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpcmd
patchelf --remove-needed libkeymaster_messages.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpcmd

patchelf --remove-needed libbacktrace.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpd
patchelf --remove-needed libunwind.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpd
patchelf --remove-needed libkeystore_binder.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpd
patchelf --remove-needed libsoftkeymasterdevice.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpd
patchelf --remove-needed libsoftkeymaster.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpd
patchelf --remove-needed libkeymaster_messages.so "$DEVICE_BLOB_ROOT"/vendor/bin/gx_fpd


"${MY_DIR}/setup-makefiles.sh"
