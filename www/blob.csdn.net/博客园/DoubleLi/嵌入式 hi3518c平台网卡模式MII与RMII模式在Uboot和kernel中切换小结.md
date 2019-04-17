# 嵌入式 hi3518c平台网卡模式MII与RMII模式在Uboot和kernel中切换小结 - DoubleLi - 博客园






由于公司项目的需要，我们需要在原有的MII的基础上，修改为RMII模式，针对hi3518c平台，我的网卡是LAN8701需要修改的地方有如下几个：

首先我的uboot中env是：

bootargs=mem=64M console=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(config),3072k(kernel),11264k(rootfs)
bootcmd=sf probe 0;sf read 0x82000000 0x80000 0x300000;bootm 0x82000000
bootdelay=1
baudrate=115200
ipaddr=10.10.1.156
netmask=255.255.255.0
bootfile="uImage"
ethaddr=00:12:12:59:26:b5
HWID=8043420002748432
gatewayip=10.10.1.1
serverip=10.10.1.197
stdin=serial
stdout=serial
stderr=serial
verify=n
ver=U-Boot 2010.06NJQY-JOSEPH-IPNC Alter By KongJun  (Oct 09 2014 - 21:54:07)

Environment size: 534/262140 bytes

1、首先是uboot，要切换成RMII模式

需要修改两个地方：

1.1、源码：



1）Uboot：include/configs/hi3518c.h文件

#define    HIETH_MII_RMII_MODE_U          HISFV_RMII_MODE //HISFV_MII_MODE

#define    HIETH_MII_RMII_MODE_D          HISFV_RMII_MODE //HISFV_MII_MODE


1.2、uboot配置表：



配置表格中修改：

200300cc的bit3 为1（原来0）：

![](http://img.blog.csdn.net/20141009222734252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2tka2p6eg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

200f005c为3（原来1）：

![](http://img.blog.csdn.net/20141009222605656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2tka2p6eg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注：地址为0x200300cc的寄存器需要自己增加寄存器！

配置表下载地址：[http://download.csdn.net/detail/skdkjxy/8018935](http://download.csdn.net/detail/skdkjxy/8018935)



2、其次是kernel中要切换为RMII模式，这里需要修改后内核配置，如下图：

![](http://img.blog.csdn.net/20141009153632294?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2tka2p6eg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.config 文件直接修改：

# CONFIG_EEE_SUPPORT is not set


CONFIG_HIETH_MONITOR_TIMER=10
CONFIG_HIETH_HWQ_XMIT_DEPTH=12
CONFIG_HIETH_MDIO_FRQDIV=2
CONFIG_HIETH_MII_RMII_MODE_U=1
CONFIG_HIETH_MII_RMII_MODE_D=1
CONFIG_HIETH_PHYID_U=1
CONFIG_HIETH_PHYID_D=2

# CONFIG_HIETH_DOWNPORT_EN is not set



然后重新编译就可以了，内核修改就到此。

3、最后就是hi3518官方驱动添加时候的修改为RMII模式：

文件为“./load3518:102:    source ./pinmux_hi3518.sh mii i2c > /dev/null”

改为“./load3518:102:    source ./pinmux_hi3518.sh rmii i2c > /dev/null”

然后没有意外的话就可以看到这样ping www.baidu.com 啦！





from:http://blog.csdn.net/skdkjzz/article/details/39931915









