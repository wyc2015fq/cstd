# 嵌入式 uboot、fs、kernel制作和烧录简记-hi3518c - DoubleLi - 博客园






NULL RAM :

mkdir ramdisk_test  临时挂在点

dd if=/dev/zero of=rootfs.ramdisk bs=1k count=10000 建立空硬盘//15000(15M)

losetup /dev/loop0 rootfs.ramdisk 挂在临时硬盘

mke2fs -m 0 /dev/loop0 格式化此硬盘

mount –t ext2 rootfs.ramdisk ./ramdisk/ramdisk_test/

cp -rf ./rootfs_uclibc/* ./ramdisk/ramdisk_test/ 拷贝文件系统

（umount ramdisk_test（rootfs.ramdisk---ramdisk-real））



默认:

使用tftp之前要对uboot-env中的ipaddr和serverip（宿主机local-host）进行设置
{
 setenv ipaddr 10.10.1.242
 setenv serverip 10.10.1.103
}


setenv bootargs mem=64M console=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(conf),3072k(os),12800k(rootfs)


ramdisk启动：

(12M)setenv bootargs mem=64M console=ttyAMA0,115200 root=/dev/ram0 rw ramdisk=32768 initrd=0x82500000,10M init=/linuxrc mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(conf),3072k(os),12800k(rootfs);tftp 0x82500000 rootfs.ramdisk;tftp 0x82000000 uImage

(16M)setenv bootargs mem=64M console=ttyAMA0,115200 root=/dev/ram0 rw ramdisk=32768 initrd=0x82500000,16M init=/linuxrc mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(conf),3072k(os),16384k(rootfs);tftp 0x82500000 rootfs.ramdisk;tftp 0x82000000 uImage

nfs启动



tftp uclibc_64k.jffs2
sf probe 0
sf erase 0x380000 0x600000
sf write 0x80008000 0x380000 0x500000




bootcmd sf probe 0\;sf read 0x82000000 0x80000 0x300000\;bootm 0x82000000





setenv spi_boot setenv bootargs mem=64M console=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(conf),3072k(os),12800k(rootfs)\;sf probe 0\;sf read 0x82000000 0x80000 0x300000\;bootm 0x82000000


模组Uboot-info:ov9712

bootdelay=1
baudrate=115200
netmask=255.255.255.0
bootfile="uImage"
ipaddr=192.168.2.100
serverip=192.168.2.15
ethaddr=00:01:02:03:04:05
bootargs=mem=64M console=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(conf),3072k(os),3584k(rootfs),9088k(home),128k(vd)
bootcmd=sf probe 0;sf read 0x82000000 0x80000 0x300000;bootm 0x82000000
stdin=serial
stdout=serial
stderr=serial
verify=n
ver=U-Boot 2010.06 (Jan 05 2013 - 18:34:21)

Environment size: 475/131068 bytes



setenv bootargs 'mem=64M console=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(conf),3072k(os),12800k(rootfs)'


sf probe 0;sf erase 0x0 0x80000;sf write 0x82000000 0x0 0x80000;
烧录内核:
    sf probe 0;mw.b 82000000 ff 300000;tftp 82000000 uImage;sf erase 80000 300000;sf write 82000000 80000 300000
   //mw.b表示按字节显示  mw: 对RAM中的内容写操作， mw 32000000 ff 10000(把内存0x32000000开始的0x10000字节设为0xFF)


烧录文件系统:
    sf probe 0 ;mw.b 82000000 ff C80000 ;tftp 82000000 rootfs_64k.jffs2;sf erase 380000 C80000 ;sf write 82000000 380000 C80000
    注：15M文件系统的时候偏移量是F00000 12M(C80000 )

文件系统制作：

    以前的内核系统支持的是yaffs文件系统，现在使用的是yaffs2文件系统，因此需要不同的制作工具，我们在此把它称为mkyaffs2image，按照上面的命令解压后它会被安
装到/usr/sbin目录下，并产生2个文件：mkyaffs2image 和mkyaffs2image-128M。

yaffs：

   #mkyaffs2image root_qtopia root_qtopia.img

cramfs：

   ./mkcramfs _install rootfs.cramfs



  uboot env:ar0130


jffs2:
mkfs.jffs2 -d ./rootfs -l -e 0x20000 -o jffs2-root.img (128K)

(qy)mkfs.jffs2 -d ./joseph_rootfs/ -l -e 0x10000 -o rootfs_kj_last.jffs2 (64K)


设置好之后直接下载对应文件系统就可以在内存中运行文件系统了：
tftp rootfs.ramdisk



注：使用串口工具进行烧录的时候把tftp改成loadb

 ubootenv:
  setenv bootargs mem=64M console=ttyAMA0,115200 root=/dev/mtdblock3 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,256k(env),3584k(kernel),11264k(rootfs)
 kernel:
    sf probe 0;mw.b 82000000 ff 300000;loadb 0x82000000;sf erase 80000 300000;sf write 82000000 80000 300000


 HI3518C(最终的):
 Spi(cs1): 
 Block:64KB 
 Chip:16MB 
 Name:"W25Q128B"
 spi size: 0x16777216
 chip num: 1
 5 cmdlinepart partitions found on MTD device hi_sfc
 Creating 5 MTD partitions on "hi_sfc":
 0x000000000000-0x000000040000 : "boot"
 0x000000040000-0x000000060000 : "env"
 0x000000060000-0x000000080000 : "config"
 0x000000080000-0x000000380000 : "kernel"
 0x000000380000-0x000000e80000 : "rootfs"
 Fixed MDIO Bus: probed









