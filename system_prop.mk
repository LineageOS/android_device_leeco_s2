#
# System Properties for msm8952
#

# Radio
PRODUCT_PROPERTY_OVERRIDES += \
    persist.radio.multisim.config=dsds \
    persist.radio.csvt.enabled=false \
    persist.radio.rat_on=combine \
    persist.radio.mt_sms_ack=20 \
    ro.telephony.call_ring.multiple=false \
    persist.radio.flexmap_type=none \
    persist.radio.ignore_dom_time=5 \
    persist.radio.cs_srv_type=1

# NITZ
PRODUCT_PROPERTY_OVERRIDES += \
    persist.rild.nitz_plmn="" \
    persist.rild.nitz_long_ons_0="" \
    persist.rild.nitz_long_ons_1="" \
    persist.rild.nitz_long_ons_2="" \
    persist.rild.nitz_long_ons_3="" \
    persist.rild.nitz_short_ons_0="" \
    persist.rild.nitz_short_ons_1="" \
    persist.rild.nitz_short_ons_2="" \
    persist.rild.nitz_short_ons_3=""

# Factory Reset Protection
PRODUCT_PROPERTY_OVERRIDES += \
    ro.frp.pst=/dev/block/bootdevice/by-name/config

# CNE
PRODUCT_PROPERTY_OVERRIDES += \
    persist.cne.feature=1

# MM
PRODUCT_PROPERTY_OVERRIDES += \
    media.msm8956hw=0 \
    mm.enable.smoothstreaming=true \
    mmp.enable.3g2=true \
    media.aac_51_output_enabled=true \
    mm.enable.qcom_parser=4194303 \
    media.vpp.enable=false \
    media.vpp.default.config=false \
    persist.media.rtsp.syncframe=1 \
    persist.media.rtsp.dropaudio=1 \
    av.debug.disable.pers.cache=1 \
    vidc.enc.dcvs.extra-buff-count=2 \
    vidc.enc.narrow.searchrange=1

# Audio
PRODUCT_PROPERTY_OVERRIDES += \
    tunnel.audio.encode = false \
    audio.offload.buffer.size.kb=64 \
    audio.offload.min.duration.secs=30 \
    audio.offload.video=true \
    audio.offload.pcm.16bit.enable=true \
    audio.offload.pcm.24bit.enable=true \
    audio.offload.track.enable=false \
    audio.offload.gapless.enabled=true \
    audio.offload.multiple.enabled=false \
    audio.deep_buffer.media=true \
    audio.playback.mch.downsample=true \
    audio.safx.pbe.enabled=true \
    audio.pp.asphere.enabled=false \
    audio.dolby.ds2.enabled=true \
    audio.parser.ip.buffer.size=262144 \
    use.voice.path.for.pcm.voip=true \
    ro.qc.sdk.audio.ssr=false \
    ro.qc.sdk.audio.fluencetype=fluence \
    persist.audio.fluence.voicecall=true \
    persist.audio.fluence.voicerec=false \
    persist.audio.fluence.speaker=false

# Audio voice concurrency related flags
PRODUCT_PROPERTY_OVERRIDES += \
    voice.playback.conc.disabled=true \
    voice.record.conc.disabled=false \
    voice.voip.conc.disabled=false \
    voice.conc.fallbackpath=deep-buffer

# Data modules
PRODUCT_PROPERTY_OVERRIDES += \
    ro.use_data_netmgrd=true \
    persist.data.netmgrd.qos.enable=true \
    persist.data.mode=concurrent

# Time-Services
PRODUCT_PROPERTY_OVERRIDES += \
    persist.timed.enable=true

# CABL
PRODUCT_PROPERTY_OVERRIDES += \
    ro.qualcomm.cabl=0

# Bluetooth
PRODUCT_PROPERTY_OVERRIDES += \
    bluetooth.hfp.client=1

# SDCard
PRODUCT_PROPERTY_OVERRIDES += \
    persist.fuse_sdcard=true

# System property for FM transmitter
PRODUCT_PROPERTY_OVERRIDES += \
    ro.fm.transmitter=false

# property to enable user to access Google WFD settings
PRODUCT_PROPERTY_OVERRIDES += \
    persist.debug.wfd.enable=1

# property to enable VDS WFD solution
PRODUCT_PROPERTY_OVERRIDES += \
    persist.hwc.enable_vds=1

# selects CoreSight configuration to enable
PRODUCT_PROPERTY_OVERRIDES += \
    persist.debug.coresight.config=stm-events

# property for vendor specific library
PRODUCT_PROPERTY_OVERRIDES += \
    ro.vendor.at_library=libqti-at.so \
    ro.vendor.gt_library=libqti-gt.so

# min/max cpu in core control
PRODUCT_PROPERTY_OVERRIDES += \
    ro.core_ctl_min_cpu=2 \
    ro.core_ctl_max_cpu=4

# Enable B service adj transition by default
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sys.fw.bservice_enable=true \
    ro.sys.fw.bservice_limit=5 \
    ro.sys.fw.bservice_age=5000

# Enable Delay Service Restart
PRODUCT_PROPERTY_OVERRIDES += \
    ro.am.reschedule_service=true

# Trim properties
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sys.fw.use_trim_settings=true \
    ro.sys.fw.empty_app_percent=50 \
    ro.sys.fw.trim_empty_percent=100 \
    ro.sys.fw.trim_cache_percent=100 \
    ro.sys.fw.trim_enable_memory=2147483648

# Optimal dex2oat threads for faster app installation
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sys.fw.dex2oat_thread_count=4

# set cutoff voltage to 3200mV
PRODUCT_PROPERTY_OVERRIDES += \
    ro.cutoff_voltage_mv=3200

# set the debug configuration for 8956/76
PRODUCT_PROPERTY_OVERRIDES += \
    persist.debug.8976.config=true

# Camera
PRODUCT_PROPERTY_OVERRIDES += \
    camera.hal1.packagelist=com.skype.raider,com.google.android.talk \
    camera.display.umax=1920x1080 \
    camera.display.lmax=1280x720 \
    camera.lowpower.record.enable=1 \
    persist.camera.stats.test=5

# Enable game colocation feature
PRODUCT_PROPERTY_OVERRIDES += \
    sched.colocate.enable=1

# Misc
PRODUCT_PROPERTY_OVERRIDES += \
    debug.sf.hw=0 \
    debug.egl.hw=0 \
    persist.hwc.mdpcomp.enable=true \
    debug.mdpcomp.logs=0 \
    dev.pm.dyn_samplingrate=1 \
    persist.demo.hdmirotationlock=false \
    debug.enable.sglscale=1 \
    persist.hwc.downscale_threshold=1.15 \
    qcom.hw.aac.encoder=true \
    persist.sys.usb.gps.notify=0 \
    ro.qcom.ad=1 \
    ro.qcom.ad.calib.data=/system/etc/ad_calib.cfg
