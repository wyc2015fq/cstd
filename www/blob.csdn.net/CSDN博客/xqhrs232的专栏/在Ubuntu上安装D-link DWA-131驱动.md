# 在Ubuntu上安装D-link DWA-131驱动 - xqhrs232的专栏 - CSDN博客
2012年01月04日 13:58:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2018标签：[ubuntu																[include																[makefile																[service																[linux																[basic](https://so.csdn.net/so/search/s.do?q=basic&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
 原文地址::[http://all2h.com/?p=30](http://all2h.com/?p=30)
本子上的无线网卡不堪我的蹂躏在一个多月前彻底罢工，大约是什么地方接触出问题了，网卡本身倒是没有什么问题
昨天在珠江路顺手买了个D-link DWA-131的无线网卡来接手，发现在Ubuntu下编译驱动还是挺费劲的
在经过google以后终于解决编译过程中遇到的各种错误，记录一下
## **第一步**
去官网下载最新的linux驱动包 [RealTek
 8192SU](http://www.realtek.com.tw/downloads/downloadsView.aspx?Langid=2&PNid=48&PFid=48&Level=5&Conn=4&DownTypeID=3&GetDown=false&Downloads=true#RTL8192SU)
## **第二步**
解压下载的驱动包，从driver文件夹中获得压缩包”rtl8712_8188_8191_8192SU_usb_linux_v2.6.6.0.20100625.tar.gz”
再次解压该文件，里面才是需要编译的驱动文件，比如都解压到 ~/DWA_driver 即可
## **第三步**
打开终端，输入：
> 
cd ~/DWA_driver/include
nano osdep_service.h
增加一行修改如下，红色部分为增加内容：
> 
#ifndef __OSDEP_SERVICE_H_
#define __OSDEP_SERVICE_H_
#include <drv_conf.h>
#include <basic_types.h>
#include <linux/sched.h>
//#include <rtl871x_byteorder.h>
…… 
然后”ctrl+x”退出并选择“y”保存
## **第四步**
返回上一级目录即“~/DWA_driver”中继续修改
> 
cd ..
nano Makefile
修改如下，其中红字部分为增加的内容
> 
EXTRA_CFLAGS += -O1 -Wno-unused-variable -Wno-unused-value -Wno-unused-label -W$
EXTRA_CFLAGS += -I$(src)/include -Wno-unused -Wno-unused-function
CONFIG_BUILT_IN = n
export TOPDIR := $(PWD)
nullstring :=
ifeq ($(CONFIG_BUILT_IN), y)
include $(src)/config
else
ifeq ($(TOPDIR), $(nullstring))
include config
else
include $(TOPDIR)/config
endif
endif
ifeq ($(CONFIG_RTL8711), y)
修改完成后保存退出
## **第五步**
编译并安装使用
> 
make clean
make
sudo make install
sudo modprobe 8712u
echo “8712u” | sudo tee -a /etc/modules
然后直接在网络管理器中就会发现已经正常运行可以搜索到周围的wifi热点咯:)
注：每次更新内核都需要重新编译安装才可正常启用，并且笔者在更新2.6.35-28-generic内核后无法使用了，之前版本的内核都可正常工作
本文参考：[HOWTO : RealTek 8192SU USB dongle (RTL8192SU) on Ubuntu 10.04](http://samiux.blogspot.com/2010/05/howto-realtek-8192su-usb-dongle.html)
