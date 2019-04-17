# flash文件制作笔记 - DoubleLi - 博客园






在uboot串口台输入printenv 可以分区以及其他信息，如下

hisilicon # printenv 
bootdelay=1
baudrate=115200
ethaddr=00:00:23:34:45:66
bootfile="uImage"
UPDATE=netupdate JVS-HI3518ES-8188 0x83FFF000 0x780000 0x80000
filesize=1B0000
fileaddr=82000000
netmask=255.255.255.0
ipaddr=172.18.198.189
serverip=172.18.198.141
bootargs=mem=38M console=ttyAMA0,115200 root=/dev/mtdblock2 rootfstype=jffs2 mtdparts=hi_sfc:1M(boot),2M(kernel),3M(system),10M(application)
sf=probe 0;sf read 0x82000000 0x100000 0x200000;bootm 0x82000000
bootcmd=sf probe 0;sf read 0x82000000 0x100000 0x300000;bootm 0x82000000
console=ttyAMA1
stdin=serial
stdout=serial
stderr=serial
verify=n
ver=U-Boot 2010.06 (Nov 11 2014 - 18:15:12)
Environment size: 626/65532 bytes



我们可以看到分区信息：mtdparts=hi_sfc:1M(boot),2M(kernel),3M(system),10M(application)

分别可以对应到Linux系统启动后的/dev/mtdblock0  /dev/mtdblock1  /dev/mtdblock2  /dev/mtdblock3 

如下：

# ls  /dev/mtdblock* -l
brw-rw----    1 adminlvj root       31,   0 Jan  1 00:00 /dev/mtdblock0
brw-rw----    1 adminlvj root       31,   1 Jan  1 00:00 /dev/mtdblock1
brw-rw----    1 adminlvj root       31,   2 Jan  1 00:00 /dev/mtdblock2
brw-rw----    1 adminlvj root       31,   3 Jan  1 00:00 /dev/mtdblock3
# 

此时我们可以将各个分区存储一一读出来

【flash读命令】


dd if=/dev/mtdblock0 of=u-boot.bin

dd if=/dev/mtdblock1 of=kernel.img

dd if=/dev/mtdblock2 of=rootfs.img

dd if=/dev/mtdblock3 of=app 



然后再将读出来的分区文件进行拼接


【文件拼接命令】

dd if=u-boot.bin of=flash-burnfile.img

dd if=kernel.img of=flash-burnfile.img bs=1k seek=1024

dd if=rootfs.img of=flash-burnfile.img bs=1k seek=3072 

dd if=app of=flash-burnfile.bin bs=1k seek=6144 

拼接完成可以生成flash-burnfile.bin,这就是整个flash的bin文件，刚好16M。



制作好的flash bin文件可以通过烧录器烧写到flash里面，也可以通过uboot进行整个flash的烧写。


【uboot烧写命令】

mw.b 0x82000000 ff 1000000;tftp 0x82000000 flash-burnfile.img;sf probe 0;sf erase 0x000000 0x1000000;sf write 0x82000000 0x000000 0x1000000;

=====

1000000 是16进制换算成10进制为16777216，16777216/1024/2014 = 16M ,即为整个Flash的大小 ;

所以0x000000 0x1000000就是起始地址和烧写的大小，先erase后write

问题：0x82000000 这个值是什么意思，没想明白，知道的回复一下博客，感谢！（本文是海思hi3518e平台的学习笔记）





【uboot烧写注意事项】

1、首先开启tftp服务器，再将bin文件拷贝至服务器目录。

2、配置uboot参数：



setenv ipaddr 172.18.198.189

setenv serverip 172.18.198.201

saveenv 

3、一定要记得插上网线，配置完uboot参数后，ping一下tftp服务器IP172.18.198.201，检查是否通。









