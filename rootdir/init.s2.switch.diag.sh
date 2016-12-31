#!/system/bin/sh
LOG_TAG="opendiagcharge"
LOG_NAME="${0}:"
loge ()
{
  /system/bin/log -t $LOG_TAG -p e "$LOG_NAME $@"
}
logi ()
{
  /system/bin/log -t $LOG_TAG -p i "$LOG_NAME $@"
}
#< LAFITE-348 shenxinyu 2016-01-18 begin>
bootmode=`getprop ro.bootmode`
if [ "$bootmode" = "charger" ]; then
    usr_flag=`cat /persist/serialno`
    if [ "$usr_flag" = "1" ]; then
        logi "boot into charger user mode"
    setprop persist.sys.usb.config mtp
        while [ 1 -eq 1 ]
        do
            usb_value=`cat /sys/class/android_usb/android0/diag_status`
            logi "in cerculate the diag_status"
            if [ $usb_value -eq 1 ];then
                setprop persist.sys.usb.config diag,serial_tty,serial_smd,adb
                sleep 3
                echo 0 > /sys/class/android_usb/android0/diag_status
                break
            fi
            sleep 1
        done
    else
    setprop persist.sys.usb.config diag,serial_tty,serial_smd,adb
#< LAFITE-348 shenxinyu 2016-01-18 end>
    fi
fi

