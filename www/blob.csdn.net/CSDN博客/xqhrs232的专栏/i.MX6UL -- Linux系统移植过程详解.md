# i.MX6UL -- Linux系统移植过程详解 - xqhrs232的专栏 - CSDN博客
2017年12月20日 17:04:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：251
原文地址::[http://blog.csdn.net/u014486599/article/details/52668702](http://blog.csdn.net/u014486599/article/details/52668702)
# i.MX6UL -- Linux系统移植过程详解
- 开发平台：   珠海鼎芯D518 -- 基于i.MX 6UL
- 最新系统：  u-boot2015.04 + Linux4.1.15_1.2.0
- 交叉编译工具：dchip-linaro-toolchain.tar.bz2
  源码下载地址：
U-Boot: (选择rel_imx_4.1.15_1.2.0_ga.tar.bz2)
[http://git.freescale.com/git/cgit.cgi/imx/uboot-imx.git/](http://git.freescale.com/git/cgit.cgi/imx/uboot-imx.git/)
Kernel: (选择rel_imx_4.1.15_1.2.0_ga.tar.bz2)
[http://git.freescale.com/git/cgit.cgi/imx/linux-2.6-imx.git/](http://git.freescale.com/git/cgit.cgi/imx/linux-2.6-imx.git/)
  源码移植过程：
1、将linux内核及uBoot源码拷贝到Ubuntu12.04系统中的dchip_imx6ul目录下；
2、使用tar命令分别将uboot和kernel解压到dchip_imx6ul目录下；
3、解压后进入uboot目录下，新建文件make_dchip_imx6ul_uboot201504.sh，且文件内容如下：
###################################################################
#               Build U-Boot.2015.04 For D518--i.MX6UL  By FRESXC                           #
###################################################################
#!/bin/bash
export ARCH=arm
export CROSS_COMPILE=<YourPath>/dchip-linaro-toolchain/bin/arm-none-linux-gnueabi-
make mrproper  # means CLEAN
make mx6ul_14x14_evk_defconfig
make2>&1|tee built_dchip_imx6ul_uboot201504.out
进入kernel目录下，新建文件make_dchip_imx6ul_linux4115120.sh，且文件内容如下：
###################################################################
#              Build Linux-4.1.15 ForD518--i.MX6UL By FRESXC                      
             #
###################################################################
#!/bin/bash
export ARCH=arm
export CROSS_COMPILE=<YourPath>/dchip-linaro-toolchain/bin/arm-none-linux-gnueabi-
make mrproper # means clean
make imx_v7_defconfig
make uImage LOADADDR=0x100080002>&1|tee built_dchip_imx6ul_linux4115120.out
make imx6ul-14x14-evk.dtb
做完以上部分，在终端中分别进入kernel和uboot源码目录下，使用./make_dchip_imx6ul_uboot201504.sh
和./make_dchip_imx6ul_linux4115120.sh命令运行上面的两个脚本文件。这样就可以编译成功了。
博客地址：http://blog.csdn.net/u014486599
