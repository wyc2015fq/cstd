# VGA,DVI,RS232,UART,SCSI，SATA等等接口怎么区分？ - L_Andy的专栏 - CSDN博客

2016年04月12日 11:10:24[卡哥](https://me.csdn.net/L_Andy)阅读数：5871
个人分类：[通信工程](https://blog.csdn.net/L_Andy/article/category/2035349)


VGA和DVI都是视频传输线。前者是传统的模拟传输，后者种类比较多，有仅仅传输数字的或者模拟的或者数模都传的，简单来说后者的视频信号传输质量要高于前者，而且传输速度更快。现在市场上很多显示器都用上了DVI接口，（包括HDMI、HDTV以及谱瑞的Displayport等）

接口非常容易区分的：

![](https://img-blog.csdn.net/20160412110219195?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

RS232确切地说应该是一种协议标准。是美国电子工业联盟（EIA）制定的串行数据通信的接口标准。你可以简单地认为UART是RS232的一种实用化。![](https://img-blog.csdn.net/20160412110324008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**UART**就是我们通常说的串口。异步工作，串行的。一般都会搭配MAX232使用，作用是把电平转换成PC机能接收的232电平或将PC机输出的232电平转换成单片机能接收的TTL电平。另外，在调试的时候，也会搭配串口调试助手，能够实现很多功能，使用起来非常方便。

**SCSI**小型计算机系统接口(英语:Small Computer System Interface; 简写:SCSI)，一种用于计算机和智能[设备](http://baike.so.com/doc/5342970-5578413.html)之间(硬盘、软驱、[光驱](http://baike.so.com/doc/1233207-1304359.html)、[打印机](http://baike.so.com/doc/5389022-5625600.html)、[扫描仪](http://baike.so.com/doc/3068119-3233874.html)等)系统级接口的独立处理器标准。
 SCSI是一种智能的通用接口标准。

**SATA**就是我们平常非常熟悉的硬盘接口，包括传统的机械硬盘和现在的固态硬盘。

这些都是非常好区分的，因为从外型上就完全不一样。
VGA：

![](https://img-blog.csdn.net/20160412110234492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

接口的定义:

 ：![](https://img-blog.csdn.net/20160412110518462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

DVI:

![](https://img-blog.csdn.net/20160412110626030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20160412110649031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

接口线：

![](https://img-blog.csdn.net/20160412110718250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

UART:

![](https://img-blog.csdn.net/20160412110804385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

接口定义:

![](https://img-blog.csdn.net/20160412110832291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

SCSI:

![](https://img-blog.csdn.net/20160412110901018?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

接口定义:

![](https://img-blog.csdn.net/20160412110926269?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

SATA:

:![](https://img-blog.csdn.net/20160412111002683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


