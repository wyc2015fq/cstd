# Android启动多个模拟器问题概述 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年09月17日 11:37:24[boonya](https://me.csdn.net/boonya)阅读数：1603








          前言：对于Android新手而言，启动模拟器漫长的等待简直就是要kill人的节奏，主要是程序需要启动一个模拟器对应的AVD，这个AVD对应的就是Android程序的版本运行环境。但是很多新手都有这样一个疑问，为什么我设置了**run configuration**的但是仍然会启动多个相同版本的实例？于是百度上搜，但是无功而返，因为这问题不是模拟器出了问题而是自己不够耐心。

#### 1.给程序设定运行的API版本对应的AVD

![](https://img-blog.csdn.net/20140917103457180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


将程序运行设置绑定到一个特定的AVD上，以后每次启动都默认会启动这个AVD版本，所以就保证了模拟器只启动一个。

#### 2.如何保证只启动一个模拟器



这里有两种启动情况：

（1）如果项目运行配置不一样，那么就有可能启动多个模拟器的AVD。

（2）如果启动同一个项目，出现多个AVD模拟器存在，则是因为第一个AVD没有成功启动，而此时急于求成的开发者又去Run As程序了。

配置好自己程序运行的AVD，设为默认运行，以后就会直接找这个对应版本的AVD了，如果AVD没有启动就启动之，若已经启动该AVD则不再重新启动此AVD。若存在多个相同的AVD版本示例，关闭之并重新启动。




