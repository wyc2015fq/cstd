# 飞控及OnboardSDK遇到问题 - KinboSong的博客 - CSDN博客
2017年07月12日 16:28:52[songjinbo3](https://me.csdn.net/KinboSong)阅读数：1244
**1、飞控连接至PC 步骤 && Windows下使用onboardSDK sample文件夹下的PureQt激活API控制的时候，步骤如下：**
(1)通过usb-TTL连接PC机和飞控主板的UART口，只连接RX,TX,GND即可
(2)插入USB-TTL到电脑端，在设备管理器中找到这个COM口，如果有感叹号，说明操作系统是盗版的，重新安装这个驱动，一定要安装11年之前的（PL2303_Prolific_DriverInstaller_v1.5.0.exe ,这个驱动程序和DJI assistant 2等放在了一个目录下面），因为11年之前的驱动不会检查盗版系统
(3)如果驱动安装成功，并且飞控成功通过UART连接到PC端，那么在设备管理器的端口（COM和LPT）中会有一项名字叫做“Profilic USB-to-Serial Comm Port”  （或者：DJI USB Virtual COM（COM4））
PS：若连接不成功，要着重注意设备是否连接至虚拟机，或者数据线是否有问题
(4)运行生成的程序，先只连接GND，点击刷新端口的按钮，选择USB-TTL对应的COM口并打开，打开之后再连接RX,TX；如果一开始就将GND,RX,TX全部连上，那么会程序会显示串口不存在，原因是程序把USB-TTL当做鼠标来处理了
