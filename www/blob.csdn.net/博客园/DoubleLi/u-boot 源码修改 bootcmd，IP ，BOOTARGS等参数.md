# u-boot 源码修改 bootcmd，IP ，BOOTARGS等参数 - DoubleLi - 博客园






uboot1.1.6\include\configs\smdk6410.h

#define CONFIG_BOOTCOMMAND"nand read 0xc0008000 0x200000 0x500000;bootm 0xc0008000"

#define CONFIG_BOOTCOMMAND "nand led-start;nand erase ;fatload mmc 0:1 0x50008000 u-boot.bin;nand write.uboot 0x50008000 0 0x200000;fatload mmc 0:1 0x50008000 zImage;nand write.e 0x50008000 0x200000 0x500000; fatload mmc 0:1 0x50008000 rootfs.yaffs2; nand write.yaffs2 0x50008000 0x00700000 $filesize; nand beep; nand led-end"

#define CONFIG_BOOTARGS "root=/dev/mtdblock2 rootfstype=yaffs2 init=/linuxrc console=ttySAC0,115200"

#define CONFIG_ETHADDR 00:40:5c:26:0a:5b
#define CONFIG_NETMASK 255.255.255.0
#define CONFIG_IPADDR	192.168.2.111
#define CONFIG_SERVERIP	192.168.2.100
#define CONFIG_GATEWAYIP	192.168.2.1









