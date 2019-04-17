# 嵌入式 hi3518c下ramdisk文件系统与文件系统烧写以及uboot中change-the-env - DoubleLi - 博客园






NULL RAM :

mkdir ramdisk_test  临时挂在点

dd if=/dev/zero of=123 bs=1k count=10000 建立空硬盘

losetup /dev/loop0 123 挂在临时硬盘

mke2fs -m 0 /dev/loop0 格式化此硬盘

munt –o loop 123 ./ramdisk/ramdisk_test/

cp -rf ./rootfs_uclibc/* ./ramdisk/ramdisk_test/ 拷贝文件系统

（umount ramdisk_test（123---ramdisk-real））



默认:

setenv bootargs mem=64M console=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(conf),3072k(os),12800k(rootfs)


ramdisk启动：

setenv bootargs mem=64M console=ttyAMA0,115200 root=/dev/ram0 rw ramdisk=32768 initrd=0x82500000,10M init=/linuxrc mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(conf),3072k(os),12800k(rootfs);tftp 0x82500000 123;tftp 0x82000000 uImage

nfs启动



tftp uclibc_64k.jffs2
sf probe 0
sf erase 0x380000 0x600000
sf write 0x80008000 0x380000 0x500000




bootcmd sf probe 0\;sf read 0x82000000 0x80000 0x300000\;bootm 0x82000000





setenv spi_boot setenv bootargs mem=64M console=ttyAMA0,115200 root=/dev/mtdblock4 rootfstype=jffs2 mtdparts=hi_sfc:256k(boot)ro,128k(env),128k(conf),3072k(os),12800k(rootfs)\;sf probe 0\;sf read 0x82000000 0x80000 0x300000\;bootm 0x82000000









