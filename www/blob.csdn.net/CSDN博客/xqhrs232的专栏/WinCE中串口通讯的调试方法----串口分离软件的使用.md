# WinCE中串口通讯的调试方法----串口分离软件的使用 - xqhrs232的专栏 - CSDN博客
2013年01月27日 20:36:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1015
原文地址::[http://www.cnblogs.com/we-hjb/archive/2008/12/14/1354871.html](http://www.cnblogs.com/we-hjb/archive/2008/12/14/1354871.html)
串口是嵌入式系统中应用很广的一种通讯接口。在WinCE中，通常会有一个串口供调试使用，另外的串口可与外围设备连接，如GSM和GPS等模块，以获取相应的信息并进行处理。
在WinCE中，为了使用串口，必须有相应的串口驱动程序，一般在BSP中都有实现。串口驱动是典型的流驱动。应用程序中可通过CreateFile()、ReadFile()和WriteFile()等文件系统的操作函数来访问串口，从而实现串口数据的收发。
虽然串口操作相对简单，但在实际调试时依然会碰到很多问题，譬如如何监视串口收发的数据。在调试GSM模块时，如果WinCE不能正确控制模块，我们就需要确认是发送还是接收的问题，是模块还是开发板的问题。在调试GPS模块时，经常需要监视GPS数据又不能影响固有GPS软件的运行。这些都要求我们能内建虚拟串口，以将物理串口收到的数据分发到不同的虚拟串口上，相反，发送则是由不同的虚拟串口往物理串口转发的过程。
    Serial Splitter Mobile就是这样一款专业软件，它能满足上述要求。以前曾用它调试过一个串口设备。当时用的是一个试用版，单次连接只能收发1M Bytes，超过1M就不工作了。开始没在意，以为是串口驱动和自己软件的问题，后来发现是Serial Splitter试用版的限制。调试手段引入的错误，是最让人郁闷的。为了方便使用，今天到其官网下载了最新的版本，并破解之，去除了收发数据的限制和应用程序中的注册信息。
破解前的截图如下:
![](http://images.cnblogs.com/cnblogs_com/we-hjb/wince6_splitter.png)
破解后的截图如下：
![](http://images.cnblogs.com/cnblogs_com/we-hjb/WINCE6.0_SERIAL_SPLITTER.png)
使用截图：
![](http://images.cnblogs.com/cnblogs_com/we-hjb/splitter_demo.png)
该软件使用很方便，简单测试了一下，效果还是可以的，有需要的请到这里下载：
[http://files.cnblogs.com/we-hjb/WINCE_SERIAL.rar](http://files.cnblogs.com/we-hjb/WINCE_SERIAL.rar)，其中包括WinCE6.0下的Splitter
 Mobile破解版、WinCE串口调试助手和使用参考视频。
