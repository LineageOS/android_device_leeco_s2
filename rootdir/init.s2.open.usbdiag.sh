#!/system/bin/sh

LOG_TAG="openusbdiag"
LOG_NAME="${0}:"
loge ()
{
  /system/bin/log -t $LOG_TAG -p e "$LOG_NAME $@"
}

logi ()
{
  /system/bin/log -t $LOG_TAG -p i "$LOG_NAME $@"
}
logi "============================================="
logi "rm usb serial number flag file start"
rm -rf /persist/serialno
logi "rm usb serial number flag file end"

