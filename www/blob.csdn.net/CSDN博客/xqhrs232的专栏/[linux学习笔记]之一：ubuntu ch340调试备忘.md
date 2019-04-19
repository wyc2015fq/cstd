# [linux学习笔记]之一：ubuntu ch340调试备忘 - xqhrs232的专栏 - CSDN博客
2017年07月06日 14:42:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2683
原文地址::[http://bbs.eeworld.com.cn/thread-505495-1-1.html](http://bbs.eeworld.com.cn/thread-505495-1-1.html)
相关文章
1、[Linux USB串口驱动 CH340 CP210X PL2303](http://www.huangea.com/?p=769)----[http://www.huangea.com/?p=769](http://www.huangea.com/?p=769)
2、
# [如何查看linux下串口是否可用？串口名称等？](http://blog.csdn.net/laiqun_ai/article/details/8224555)----[http://blog.csdn.net/laiqun_ai/article/details/8224555](http://blog.csdn.net/laiqun_ai/article/details/8224555)
# 3、
## Linux系统识别新增串口设备方法
 ----[http://blog.sina.com.cn/s/blog_5dcbb52d0101dlwj.html](http://blog.sina.com.cn/s/blog_5dcbb52d0101dlwj.html)
# 4、[Linux下查看串口是否可用的笨方法](http://blog.csdn.net/lu_embedded/article/details/54380990)----[http://blog.csdn.net/lu_embedded/article/details/54380990](http://blog.csdn.net/lu_embedded/article/details/54380990)
最近迷上了Linux，入了一块板。电脑使用win7+ubuntu 双系统，可是调试的时候就麻烦了，ubuntu下写好sd卡镜像，调试的时候还要切换到win7，手上的usb转串口驱动芯片时CH340，于是就想看ubuntu下时候可以安装ch340驱动。
网上搜了一下，还真有，不过时很早的内容了。
因为属于小白，所以就摸着走呗...把ch340 一端插入电脑
输入命令： lsmod | grep usbserial
输出：usbserial              53248  1 ch341
成功发现了设备，识别出时 ch341  是我记错了？？
输入：lsusb
输出一下信息：
  Bus 002 Device 003: ID 17ef:4810 Lenovo Integrated Webcam [R5U877]
Bus 002 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 008 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 007 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 006 Device 002: ID 1a81:1004 Holtek Semiconductor, Inc. 
Bus 006 Device 003: ID 1a86:7523 QinHeng Electronics HL-340 USB-Serial adapter
Bus 006 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 005 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 004 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 003 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
发现了ch341的一些相关信息
输入命令（查看USB转串口接入状态）：dmesg
输出相关信息，部分信息如下：
  [  429.184170] usb 6-1: new full-speed USB device number 3 using uhci_hcd
[  429.345937] usb 6-1: New USB device found, idVendor=1a86, idProduct=7523
[  429.345944] usb 6-1: New USB device strings: Mfr=0, Product=2, SerialNumber=0
[  429.345948] usb 6-1: Product: USB2.0-Ser!
[  429.368123] usbcore: registered new interface driver usbserial
[  429.368396] usbcore: registered new interface driver usbserial_generic
[  429.368875] usbserial: USB Serial support registered for generic
[  429.374125] usbcore: registered new interface driver ch341
[  429.374143] usbserial: USB Serial support registered for ch341-uart
[  429.374165] ch341 6-1:1.0: ch341-uart converter detected
[  429.386851] usb 6-1: ch341-uart converter now attached to ttyUSB0
ID信息与lsusb检测到的一致，ch341-uart converter now attached to ttyUSB0  成功驱动，端口为ttyUSB0
太惊喜了。免的安装驱动了![](http://bbs.eeworld.com.cn/static/image/smiley/default/titter.gif)
下一步，安装串口终端软件，搜到了一个minicom，动手试试：
安装：sudo apt-get install minicom  
打印一些信息，输入‘y’。
等待安装成功，然后用sudo minicom -s 设置minicom的端口对应到ttyUSB0（对应使用dmesg命令看到的信息）。
出现配置菜单：
![](http://5.eewimg.cn/data/attachment/forum/201611/08/212801n0vvz9ksishhs5f2.png.thumb.jpg)
选择“Serial port setup”，出现串口配置菜单：
![](http://5.eewimg.cn/data/attachment/forum/201611/08/213022ykh3dnndijp8sfsp.png.thumb.jpg)
输入A，修改serial device 由/Dev/tty0修改为/dev/ttyusb0，波特率等修改为115200 8N1。
硬件连接，上电，成功输出打印信息，呵呵
U-Boot SPL 2016.09-rc1 (Oct 01 2016 - 21:31:35)                                 
init i2c                                                                        
rand test 0 MB RW, err cnt = 0                                                  
DRAM:  512 MiB ~                                                                
CPU: 1008000000Hz, AXI/AHB/APB: 3/2/2                                           
sunxi_board_init ok!                                                            
Trying to boot from MMC1                                                        
mmc_dev=0                                                                       
common/spl/spl_mmc.c L138: mmc_initialize ok                                    
common/spl/spl_mmc.c L155: find_mmc_device ok                                   
part_init: try 'EFI': ret=-1                                                    
err=0                                                                           
mmc_init: 0, time 268
common/spl/spl_mmc.c L298: mmc_init ok                                          
common/spl/spl_mmc.c L335: goto mmc_load_image_raw_sector                       
hdr read sector 50, count=1                                                     
load_legacy                                                                     
read 906 sectors to 49ffffc0                                                    
Jumping to U-Boot                                                               
SPL malloc() used 656 bytes (0 KB)                                              
loaded - jumping to U-Boot...                                                   
image entry point: 0x                                                           
###in board_init_f                                                              
U-Boot 2016.09-rc1 (Oct 01 2016 - 21:31:35 +0800) Allwinner Technology          
CPU:   Allwinner A13 (SUN5I)                                                    
Model: Lichee A13                                                               
I2C:   ready                                                                    
DRAM:  512 MiB                                                                  
###out board_init_f                                                             
###in board_init_r                                                              
MMC:   part_init: try 'EFI': ret=-1                                             
part_init: try 'DOS': ret=0                                                     
err=0                                                                           
mmc_init: 0, time 276                                                           
*** Warning - bad CRC, using default environment                                
Setting up a 800x480 lcd console (overscan 0x0)                                 
###set hbp=110, ht=1080, hspw=10                                                
###set vbp=45, vt=527, vspw=10 
.........
下一篇分享sd卡ubuntu下创建分区，boot/根文件等镜像烧录

