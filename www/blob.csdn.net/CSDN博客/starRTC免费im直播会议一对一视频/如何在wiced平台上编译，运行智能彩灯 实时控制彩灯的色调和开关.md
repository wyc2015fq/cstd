# 如何在wiced平台上编译，运行智能彩灯 实时控制彩灯的色调和开关 - starRTC免费im直播会议一对一视频 - CSDN博客
2019年03月22日 16:41:06[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：68
[https://github.com/WildDogTeam/demo-c-rgblight/tree/master/src/device](https://github.com/WildDogTeam/demo-c-rgblight/tree/master/src/device)
- 恢复出厂的GPIO口在demo_platform.h声明为WICED_GPIO_2，用户可以根据自己的需要修改，注意恢复出厂的GPIO必须能响应外部中断.
- 智能彩灯的GPIO口在demo_devices.c中定义，默认如下,用户可以根据自己的需要直接修改，注意控制智能彩灯的GPIO必须带硬件PWM
2，烧录wiced
把demo-c-rgblight/src/device/software拷贝到WICED-SDK-x.x.x\WICED-SDK\apps目录下
- 在WICED-IDE上建立Target-software.project.wiced-<yourplatform> download run（WSDB750开发板采用外接flash，因此Target为 software.project.wiced-WSDB750 JTAG=ftdi_swd download_apps download run）。
注意：使用WSDB750开发板时，烧录前需要安装WICED-SDK\tools\drivers\BCM9WCD1EVAL1_Drivers_patch_SWD\InstallDriver.exe。
- 双击Target，编译并下载运行，同时打开串口工具，波特率设置为115200-8-n-1，查看运行的log输出。
可以对设备进行调色和开关操作。
Wiced 文档和sdk获取:[http://www.broadcom.com/products/wiced/wifi/](http://www.broadcom.com/products/wiced/wifi/)
espressif sdk获取: [http://espressif.com/zh-hans/%E6%9C%80%E6%96%B0sdk%E5%8F%91%E5%B8%83/](http://espressif.com/zh-hans/%E6%9C%80%E6%96%B0sdk%E5%8F%91%E5%B8%83/)
[https://github.com/starrtc/starrtc-android-demo](https://github.com/starrtc/starrtc-android-demo)
