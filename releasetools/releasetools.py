#
# Copyright (C) 2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

import hashlib
import common
import re

def FullOTA_InstallEnd(info):
  info.script.AppendExtra('mount("ext4", "EMMC", "/dev/block/bootdevice/by-name/system", "/mnt/system");');
  RunCustomScript(info, "devinfo.sh", "")
  info.script.AppendExtra('unmount("/mnt/system");');
  return

def IncrementalOTA_InstallEnd(info):
  info.script.AppendExtra('mount("ext4", "EMMC", "/dev/block/bootdevice/by-name/system", "/mnt/system");');
  RunCustomScript(info, "devinfo.sh", "")
  info.script.AppendExtra('unmount("/mnt/system");');
  return

def RunCustomScript(info, name, arg):
  info.script.AppendExtra(('run_program("/tmp/install/bin/%s", "%s");' % (name, arg)))
  return
