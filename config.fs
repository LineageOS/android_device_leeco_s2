[system/bin/qmuxd]
mode: 0700
user: AID_RADIO
group: AID_SHELL
caps: BLOCK_SUSPEND

[system/bin/mm-qcamera-daemon]
mode: 0700
user: AID_CAMERA
group: AID_SHELL
caps: SYS_NICE

[system/bin/pm-service]
mode: 0755
user: AID_SYSTEM
group: AID_SYSTEM
caps: NET_BIND_SERVICE

[system/bin/cnss-daemon]
mode: 0755
user: AID_SYSTEM
group: AID_SYSTEM
caps: NET_BIND_SERVICE

[system/bin/imsdatadaemon]
mode: 0755
user: AID_SYSTEM
group: AID_SYSTEM
caps: NET_BIND_SERVICE

[system/bin/ims_rtp_daemon]
mode: 0755
user: AID_SYSTEM
group: AID_RADIO
caps: NET_BIND_SERVICE

[system/vendor/bin/slim_daemon]
mode: 0755
user: AID_SYSTEM
group: AID_GPS
caps: NET_BIND_SERVICE

[system/bin/wcnss_filter]
mode: 0755
user: AID_BLUETOOTH
group: AID_BLUETOOTH
caps: BLOCK_SUSPEND
