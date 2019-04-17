# 基于Tiny6410的LED驱动程序 - xiahouzuoxin - CSDN博客





2013年05月07日 19:03:27[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2292










![](https://img-blog.csdn.net/20130507190145724)


从电路图中看出，GPIO低电平有效（LED亮）。GPK共有16个端口，每个端口由三个寄存器控制——控制寄存器（GPKCON）、数据寄存器（GPKDAT）和上拉寄存器（GPKPUD），每个寄存器都是32位，如下图所示。

![](https://img-blog.csdn.net/20130507190154911)




GPK的控制寄存器分为两个，GPKCON0控制0~8好端口，GPKCON1控制9~15号端口。控制寄存器连续的4位控制一个端口，如下图中LED对应的控制寄存器。

![](https://img-blog.csdn.net/20130507190205551)




## 驱动程序


/*

 *===========================================================================

 *       Filename:  tiny6410_leds.c

 *    Description:  led drivers for Tiny6410

 *

 *        Created:  xhzuoxin 05/07/2013

 *       Compiler:  gcc

 *===========================================================================

 */

#include<linux/module.h>

#include<linux/init.h>

//#include <linux/mm.h>

#include<linux/fs.h>

//#include <linux/types.h>

//#include <linux/delay.h>

//#include <linux/slab.h>

//#include <linux/cdev.h>

#include<linux/miscdevice.h>

#include<linux/errno.h>

#include<linux/ioctl.h>

//#include <linux/string.h>

//#include <linux/list.h>

#include<linux/pci.h>

#include<asm/uaccess.h>

//#include <asm/atomic.h>

//#include <asm/unistd.h>

//#include <asm/irq.h>

#include<mach/regs-gpio.h>

#include<mach/hardware.h>

#include<mach/map.h>

#include<mach/regs-clock.h>

#include<mach/regs-gpio.h>

#include<plat/gpio-cfg.h>

#include<mach/gpio-bank-e.h>

#include<mach/gpio-bank-k.h>



#defineLED4 GPK7

#defineLED3 GPK6

#defineLED2 GPK5

#defineLED1 GPK4



#defineDEBUG            0

#defineDEVICE_NAME      
"tiny6410_leds"





staticlong tiny6410_leds_ioctl(structfile *flip,unsignedintcmd,
unsignedlongarg)

{

    /* arg:which led */

switch(cmd) {

unsigned tmp;

case 0:

case 1:

if (arg > 4) {

return -EINVAL;

       }

       tmp =readl(S3C64XX_GPKDAT); // read data register

       tmp&= ~(1 << (4+arg));  // clear bit

       tmp |=((!cmd) << (4+arg));  // set bitwith cmd

       writel(tmp,S3C64XX_GPKDAT);  // write data register

#ifDEBUG

       printk(DEVICE_NAME":%ld %d\n", arg, cmd);

#endif

return 0;

default:

return -EINVAL;

    }

}



staticstruct file_operations dev_fops = {

    .owner =THIS_MODULE,

    .unlocked_ioctl= tiny6410_leds_ioctl, 

};



staticstruct miscdevice misc = {

    .minor =MISC_DYNAMIC_MINOR,

    .name =DEVICE_NAME,

    .fops =&dev_fops,

};



staticint __init dev_init(void)

{

int ret;

unsigned tmp;



    /*configure config register : 0001 output */

    tmp =readl(S3C64XX_GPKCON);

    tmp = (tmp&~ (0xffffU<<16)) | (0x1111U<<16);

    writel(tmp,S3C64XX_GPKCON);



    /*configure data register : init dark */

    tmp =readl(S3C64XX_GPKDAT);

    tmp |=(0xF << 4);

    writel(tmp,S3C64XX_GPKDAT);



    ret =misc_register(&misc);



    printk(DEVICE_NAME"\tinitialized\n");



return ret;

}



staticvoid __exit dev_exit(void)

{

    misc_deregister(&misc);

}



module_init(dev_init);

module_exit(dev_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("zx");


Makefile文件如下：


ARCH=arm

COMPILE=arm-linux-



ifneq ($(KERNELRELEASE),)

obj-m:=tiny6410_leds.o



else

KDIR ?= /mnt/HappyStudy/Tiny6410/linux-2.6.38

PWD := $(shell pwd)

endif



all:

    make -C$(KDIR) M=$(PWD) modules ARCH=$(ARCH) CROSS_COMPILE=$(COMPILE)



clean:

    rm -f *.ko*.o *.mod.o *.od.c *.symvers


用户空间测试文件为led.c如下，


#include<stdio.h>

#include<stdlib.h>

#include<unistd.h>

#include<sys/ioctl.h>

#include<sys/types.h>

#include<sys/stat.h>

#include<fcntl.h>



#defineDEVICE_NAME "/dev/tiny6410_leds"



intmain(int argc,char**argv)

{

int on;

int led_no;

int fd;



if (argc != 3 || sscanf(argv[1], "%d",&led_no) != 1 || sscanf(argv[2],"%d", &on) != 1 || on < 0|| on > 1 || led_no < 0 || led_no > 3) {

       fprintf(stderr,"Usage: leds led_no 0|1\n");

       exit(1);

    }



    fd =open(DEVICE_NAME, 0);

if (fd < 0) {

       perror("opendevice leds");

       exit(1);

    }



    ioctl(fd,on, led_no);

    close(fd);



    return 0;

}


使用arm-linux编译，友善提供了一个编译led.c的Makefile文件，我们关注其中的install目标，该目标能将编译后的程序安装到系统中。


ifndef DESTDIR

DESTDIR           ?=/tmp/FriendlyARM/mini6410/rootfs

endif

CFLAGS            =-Wall -O2

CC                =arm-linux-gcc

INSTALL           =install

TARGET            =led



all: $(TARGET)



led: led.c

    $(CC)$(CFLAGS) $< -o $@



install: $(TARGET)

    $(INSTALL)$^ $(DESTDIR)/usr/bin



clean distclean:

    rm -rf *.o$(TARGET)



.PHONY: $(PHONY) install clean distclean




