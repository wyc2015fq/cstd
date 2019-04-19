# 让 Linux 启动时加载自己的驱动模块 .ko - xqhrs232的专栏 - CSDN博客
2016年04月05日 13:37:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3805
原文地址::[http://fgsink.blog.163.com/blog/static/16716997020107981857597/](http://fgsink.blog.163.com/blog/static/16716997020107981857597/)
相关文章
1、**Linux驱动的两种加载方式过程分析----[http://www.2cto.com/os/201406/311006.html](http://www.2cto.com/os/201406/311006.html)**
2、Linux驱动的两种加载方式过程分析----[http://www.linuxidc.com/Linux/2014-06/103569.htm](http://www.linuxidc.com/Linux/2014-06/103569.htm)
3、[如何在Linux下添加自己的驱动](http://blog.csdn.net/huanggenda568/article/details/7712269)----[http://blog.csdn.net/huanggenda568/article/details/7712269](http://blog.csdn.net/huanggenda568/article/details/7712269)
 整个开机流程是
(1) 载入BIOS的硬件信息，并取得第一个开机装置的代号
(2)读取第一个开机装置的MBR的boot Loader (grub)的开机信息
(3)载入OS Kernel信息，解压Kernel，尝试驱动硬件
(4) Kernel执行init程序并获得run-lebel信息(如3或5)
(5) init执行/etc/rc.d/rc.sysinit
(6)启动内核外挂模块(/etc/modprobe.conf)（/etc/profile）
(7) init执行run-level的各种Scripts，启动服务
(8) init执行/etc/rc.d/rc.local
(9)执行/bin/login，等待用户Login
(10)Login后进入Shell
正确的方式是把需要加载的模块放在(5)或(6)，
***************************************************
   SEP4020 ARM Linux-2.6.16 SDK 3.1  for Xiaogui   
***************************************************
# mount all...........
# Starting mdev.........
starting pid 250, tty '': '-/bin/sh'
fgs add .....
hwclock: settimeofday() failed: Invalid argument
Created led device OK
request tp irq8 failed!
insmod: cannot insert '/drivers/sep4020_tp.ko': Operation not permitted
sep4020 Keys module insmod OK
registered!
mount: mounting /dev/mmcblk0p1 on /mnt/sd/ failed: No such file or directory
mount: mounting /dev/uba1 on /mnt/usb failed: No such file or directory
keydevice is open!!!!!!!!!!!
TSLIB_TSDEVICE is open!!!!!!!!!!!
ts_open(): No such device or address
/tmp #
如上打印信息中的绿色部分，执行的脚本在 /nfs/etc/profile中
内容如下：
#Set search library path
#echo "Set search library path..........."
export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib:$LD_LIBRARY_PATH
#Set user path
echo "fgs add ....."
export PATH=/bin:/sbin:/usr/bin:/usr/sbin:$PATH
mknod /dev/ttyp5   c 3 5
mknod /dev/ptyp5   c 2 5
mknod /dev/tp    c 250 0
mknod /dev/flowled   c 253 0
mknod /dev/mg_card  c 255 0
mknod /dev/key   c  254 0
mknod /dev/sep4020_gpio c  240 0
mknod /dev/printer  c 249 0
mknod /dev/psam   c 252 0
mknod /dev/cmx865  c 251 0
mknod /dev/sep4020_pwm c 245 0
export T_ROOT=/
export LD_LIBRARY_PATH=/lib 
export TSLIB_CONSOLEDEVICE=none  
export TSLIB_TSDEVICE=/dev/tp
export TSLIB_CALIBFILE=/etc/pointercal     
export TSLIB_CONFFILE=/etc/ts.conf   
export TSLIB_PLUGINDIR=/plugins
hwclock -s
insmod  /drivers/sep4020_led.ko
insmod  /drivers/sep4020_tp.ko 
insmod  /drivers/sep4020_key.ko 
insmod  /drivers/sep4020_pwm.ko
insmod  /drivers/sep4020_psam.ko
#mount -t yaffs /dev/mtdblock2 /Mini4020/Mp3
mount -t vfat -o sync /dev/mmcblk0p1 /mnt/sd/
mount /dev/uba1 /mnt/usb
cp -ar Mini4020/mini/res/ /tmp/
cp -ar Mini4020/mini/sdres/ /tmp/
cp /Mini4020/mini/prisonbreak.mp3 /tmp/
cp /Mini4020/Mp3/madplay.arm /tmp/
cp /Mini4020/mini/mini /tmp/
cp -ar Mini4020/mini/pwm/ /tmp/
cd /tmp
./mini
