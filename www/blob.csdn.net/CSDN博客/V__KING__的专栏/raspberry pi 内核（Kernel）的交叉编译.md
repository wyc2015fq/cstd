# raspberry pi 内核（Kernel）的交叉编译 - V__KING__的专栏 - CSDN博客





2015年12月05日 11:00:30[v__king__](https://me.csdn.net/V__KING__)阅读数：1682








# raspberry pi 内核（Kernel）的交叉编译。

参考文档: [官方文档](http://elinux.org/Raspberry_Pi_Kernel_Compilation#Use_the_provided_compiler)

环境准备: 

PC操作系统: ubuntu 12.04, 64bit

相关软件: 

- linux-rpi-3.6.y.zip linux源码 

- firmware-master.zip 固件依赖 

- tools-master.zip 交叉编译环境工具 

获取方法
> 
$git clone git://github.com/raspberrypi/firmware.git PRiFirmware

$git clone git://github.com/raspberrypi/linux.git RpiLinux

$ git clone git://github.com/raspberrypi/tools.git RpiTools


由于下载比较慢，如果不升级的话，直接下载zip包靠谱一点。 

步骤:（其实和直接编译内核步骤类似） 

1 获取当前树莓派Kernel的.config文件，并进行自定义的配置，拷贝到PC上 

2 在PC上执行
> 
$ mv .config /RPiLinux

$ cd RpiLinux/


编辑老的配置文件

> 
$ make ARCH=arm CROSS_COMPILE=../RpiTools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi- oldconfig


如果需要添加新的模块

> 
$ make ARCH=arm CROSS_COMPILE=../RpiTools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi- menuconfig


（官方提供的编译工具中包含三套工具链

```
arm-bcm2708hardfp-linux-gnueabi
arm-bcm2708-linux-gnueabi
gcc-linaro-arm-linux-gnueabihf-raspbian
```

我们选择带hardfp硬解码的工具。） 

一路enter，选择默认值，可以看出里面包含不少对加载驱动的选取。

3 编译

> 
$ make ARCH=arm CROSS_COMPILE=../RpiTools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi- -j4


-j标示cpu的核数，多核编译提高速度 

这个过程可能需要二十分钟 

最终，arch/arm/boot/zImage 就是我们所编译获得的文件。zImage 是 Compressed kernel image 文件，要转换为 kernel.img 还需要进一步处理。
> 
$ cd ../RpiTools/mkimage/

$ ./imagetool-uncompressed.py ../../RpiLinux/arch/arm/boot/zImage


编译新lib： 

新的Kernel要正确运行，还需要编译所需的module，主要对应“/lib”目录下的内容。编译时，使用“INSTALL_MOD_PATH”参数指定目标路径。

> 
$ cd ../..

$ mkdir modules

$ cd RpiLinux/

$ make modules_install ARCH=arm CROSS_COMPILE=../RpiTools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi- INSTALL_MOD_PATH=../modules


备份 Firmware:

> 
$ cd ..

$ mkdir backup

$ sudo mount /dev/sdc1 /media/SD/

$ cd /media/SD/

$ mv *.elf *.bin ~/gitpool/raspberrypi/backup/


# copy内核到sd卡

将SD卡拔下插在电脑上（可使用读卡器）
- 1)升级内核

将新编好的内核拷入SD卡，改名为：kernel_new.img

打开boot目录下

找到config.txt文件，加入：kernel=kernel_new.img这一行
- 2）升级boot

将firmware/boot/目录下 以下文件拷入SD卡boot目录：fbootcode.bin fixup.dat fixup_cd.dat start.elf
- 3)更新vc库及内核modules

将第3步d步中编译出来的modules/lib/modules拷入树莓派文件系统/lib下






