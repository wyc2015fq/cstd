# 树莓派linux驱动学习之hello world - xqhrs232的专栏 - CSDN博客
2016年04月05日 15:31:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1381
原文地址::[http://blog.csdn.net/hcx25909/article/details/16860055](http://blog.csdn.net/hcx25909/article/details/16860055)
相关文章
1、在树莓派上编译驱动----[http://cstriker1407.info/blog/compiled-on-the-raspberry-driven/](http://cstriker1407.info/blog/compiled-on-the-raspberry-driven/)
2、[树莓派交叉编译环境搭建](http://www.cnblogs.com/emouse/archive/2013/06/07/3124063.html)----[http://www.cnblogs.com/emouse/archive/2013/06/07/3124063.html](http://www.cnblogs.com/emouse/archive/2013/06/07/3124063.html)
 最近想学习一下linux驱动，看了一些书和教学视频，大概了解了一下，不过要想深入，肯定需要实践。手上有几块linux的板子，最终选择了树莓派作为我的实验平台，资料比较丰富，接口也比较简单。
        程序员的入门经典当然就是hello world程序了，我的第一个实验就是要搭建实验环境和工具链，通过交叉编译的方式，在上位机完成hello world驱动程序的编写，最终在板子上运行。
# 一、安装树莓派系统
        首先要在树莓派上安装系统，这个网上的资料比较多，就不细说了，我使用的是[2013-09-25-wheezy-raspbian](http://downloads.raspberrypi.org/raspbian_latest)，下载地址可见：[http://www.raspberrypi.org/downloads](http://www.raspberrypi.org/downloads)
          最终搭建的平台如下，后期的通讯可以直接使用串口，就不需要HDMI输出了： 
![](https://img-blog.csdn.net/20131121140631500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGN4MjU5MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 二、下载交叉编译工具链
        交叉编译工具链可以帮助我们在PC机上完成驱动程序的开发，此外还需要和下位机配套的Linux内核代码，树莓派官方已经为我们提供了相应的内核代码和工具链，直接使用git下载：
<span style="border: 0px; margin: 0px; padding: 0px; vertical-align: baseline; background-color: transparent;"></span><pre class="bash" name="code" style="white-space: pre-wrap; word-wrap: break-word;">    $ <span style="border: 0px; margin: 0px; padding: 0px; vertical-align: baseline; color: rgb(194, 12, 185);">git clone</span><span style="background-color: transparent;"> git:</span><span style="border: 0px; margin: 0px; padding: 0px; vertical-align: baseline; background-color: transparent;">//</span><span style="background-color: transparent;">github.com</span><span style="border: 0px; margin: 0px; padding: 0px; vertical-align: baseline; background-color: transparent;">/</span><span style="background-color: transparent;">raspberrypi</span><span style="border: 0px; margin: 0px; padding: 0px; vertical-align: baseline; background-color: transparent;">/</span><span style="background-color: transparent;">linux.git RpiLinux</span><span style="border: 0px; margin: 0px; padding: 0px; vertical-align: baseline;"></span><pre class="bash" name="code" style="white-space: pre-wrap; word-wrap: break-word;">    $ <span style="border: 0px; margin: 0px; padding: 0px; vertical-align: baseline; color: rgb(194, 12, 185);">git clone</span><span style="background-color: transparent;"> git:</span><span style="border: 0px; margin: 0px; padding: 0px; vertical-align: baseline; background-color: transparent;">//</span><span style="background-color: transparent;">github.com</span><span style="border: 0px; margin: 0px; padding: 0px; vertical-align: baseline; background-color: transparent;">/</span><span style="background-color: transparent;">raspberrypi</span><span style="border: 0px; margin: 0px; padding: 0px; vertical-align: baseline; background-color: transparent;">/</span><span style="background-color: transparent;">tools.git RpiTools</span>
需要注意的是下载的时候要注意内核代码是否和我们板子中的内核版本相同，我使用的是3.6.11+,所以下载代码的时候也要选择3.6.y这个版本。
# **三、编写驱动代码**
我们在上位机编写hello world的代码：
**[cpp]**[view plain](http://blog.csdn.net/hcx25909/article/details/16860055#)[copy](http://blog.csdn.net/hcx25909/article/details/16860055#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75879/fork)
- #include <linux/init.h>
- #include <linux/module.h>
- #include <linux/moduleparam.h>
- 
- MODULE_LICENSE("Dual BSD/GPL");  
- 
- staticint hello_init(void)  
- {  
-     printk(KERN_ALERT"Hello, world\n");  
- return 0;  
- }  
- 
- staticvoid hello_exit(void)  
- {  
-     printk(KERN_ALERT"Goodbye, cruel world\n");  
- }  
- 
- module_init(hello_init);  
- module_exit(hello_exit);  
相应的makefile代码：
**[plain]**[view plain](http://blog.csdn.net/hcx25909/article/details/16860055#)[copy](http://blog.csdn.net/hcx25909/article/details/16860055#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/75879/fork)
- ifneq ($(KERNELRELEASE),)  
- 
- obj-m := hello.o  
- 
- else  
- 
- KDIR := /home/hcx/work/boards/RPi/kernel/linux-rpi-3.6.y  
- all:  
-     make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=/home/hcx/work/boards/RPi/kernel/RpiTools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi-  
- 
- clean:  
-     rm -f *.ko *.o *.mod.o *.mod.c *.symvers  modul*  
- 
- endif  
编译完成后，我们通过U盘将hello.ko文件拷贝到树莓派中，然后通过insmod插入模块，结果如下：
![](https://img-blog.csdn.net/20131121142530484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGN4MjU5MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
----------------------------------------------------------------
**欢迎大家转载我的文章。**
**转载请注明：转自古-月**
**[](http://blog.csdn.net/yorhomwang)[http://blog.csdn.net/hcx25909](http://blog.csdn.net/hcx25909)**
**欢迎继续关注我的博客**
