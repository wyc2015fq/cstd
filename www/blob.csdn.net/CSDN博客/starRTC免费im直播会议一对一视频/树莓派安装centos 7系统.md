# 树莓派安装centos 7系统 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年05月30日 19:20:49[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：3866
1，格式化  [https://www.sdcard.org/downloads/formatter_4/eula_windows/](https://www.sdcard.org/downloads/formatter_4/eula_windows/)
2，烧录，Win32DiskImager
[https://sourceforge.net/projects/win32diskimager/](https://sourceforge.net/projects/win32diskimager/)
3，下载centos,最好都下载最新的raw.xz
[http://mirror.centos.org/altarch/7/isos/armhfp/](http://mirror.centos.org/altarch/7/isos/armhfp/)
[http://mirror.centos.org/altarch/7/os/armhfp/Packages/](http://mirror.centos.org/altarch/7/os/armhfp/Packages/)
[https://buildlogs.centos.org/centos/7/isos/armhfp/](https://buildlogs.centos.org/centos/7/isos/armhfp/)
解压得到raw
以管理员身份运行Win32DiskImager，选择已解压好的raw文件及目标Micro-SD卡，点击Wirte开始写入，过程大约3分钟。
[https://raspberrypi.stackexchange.com/questions/81458/why-am-i-getting-a-rainbow-screen-and-flashing-red-pwr-led-with-a-new-raspberry](https://raspberrypi.stackexchange.com/questions/81458/why-am-i-getting-a-rainbow-screen-and-flashing-red-pwr-led-with-a-new-raspberry)
[https://elinux.org/R-Pi_Troubleshooting#Green_LED_blinks_in_a_specific_pattern](https://elinux.org/R-Pi_Troubleshooting#Green_LED_blinks_in_a_specific_pattern)
或在linux下
xz -cd SLES-12-SP2-ARM-X11-raspberrypi3_aarch64.aarch64-2016.10.04-GM.raw.xz | dd of=/dev/sdb bs=4096
