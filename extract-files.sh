#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2020 The LineageOS Project
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

function blob_fixup() {
    case "${1}" in

    # Remove all unused dependencies from FP blobs
    vendor/bin/gx_fpcmd | vendor/bin/gx_fpd)
        patchelf --remove-needed "libbacktrace.so" "${2}"
        patchelf --remove-needed "libunwind.so" "${2}"
        patchelf --remove-needed "libkeystore_binder.so" "${2}"
        patchelf --remove-needed "libsoftkeymasterdevice.so" "${2}"
        patchelf --remove-needed "libsoftkeymaster.so" "${2}"
        patchelf --remove-needed "libkeymaster_messages.so" "${2}"
        ;;

    # Add shim for libbase LogMessage functions
    vendor/bin/imsrcsd | vendor/lib64/lib-uceservice.so)
        for  LIBBASE_SHIM in $(grep -L "libbase_shim.so" "${2}"); do
            patchelf --add-needed "libbase_shim.so" "$LIBBASE_SHIM"
        done
        ;;

    esac
}

# Initialize the helper
setup_vendor "${DEVICE}" "${VENDOR}" "${LINEAGE_ROOT}" false "${CLEAN_VENDOR}"

extract "${MY_DIR}/proprietary-files.txt" "${SRC}" ${KANG} --section "${SECTION}"

extract "${MY_DIR}/proprietary-files-qc.txt" "${SRC}" ${KANG} --section "${SECTION}"

"${MY_DIR}/setup-makefiles.sh"
