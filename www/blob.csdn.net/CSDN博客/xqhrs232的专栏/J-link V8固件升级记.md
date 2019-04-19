# J-link V8固件升级记 - xqhrs232的专栏 - CSDN博客
2015年11月28日 00:11:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：851
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.sina.com.cn/s/blog_5bdee3020101khfy.html](http://blog.sina.com.cn/s/blog_5bdee3020101khfy.html)

好久没为电子工程事业尽份力了！今天也稍微努把力！写写我是如何升级J-link的固件的吧！
![J-link <wbr>V8固件升级记](http://s10.sinaimg.cn/middle/5bdee302gcbeec765d859&690)
J-link是什么？晕，不是电子工程师的退散吧！这是现在比较流行的arm调试工具！我手上是那个大家都比较喜闻乐见的V8版本！盗版吗！大家懂的！50元入手的！没有软件升级与维护的！说不准哪天！升级软件就封杀了！如果固件没法更新就基本一块砖！砸人防身都可以哦！
![J-link <wbr>V8固件升级记](http://s7.sinaimg.cn/middle/5bdee302gcbeedcfbf496&690)
首先在通电情况下，短接a！然后拔下usb 接着短接b，然后插入usb 10秒，然后拔下usb。最后从新上电插入usb！于是你的电脑里面就有了一个无法识别新设备！
![J-link <wbr>V8固件升级记](http://s7.sinaimg.cn/middle/5bdee302gcbeee279d156&690)
去atmel官网下载最新版本的SAM-BA v2.12软件，你就可以正确识别硬件为一个usb转串口设备了！
[http://ishare.iask.sina.com.cn/f/34214872.html](http://ishare.iask.sina.com.cn/f/34214872.html)
双击运行SAM-BA v2.12
![J-link <wbr>V8固件升级记](http://s13.sinaimg.cn/middle/5bdee302gcbeee6c7180c&690)
设置你的设备端口号，还有芯片型号！之后点击连接！
![J-link <wbr>V8固件升级记](http://s14.sinaimg.cn/middle/5bdee302gcbeee7fa59dd&690)
进入固件刷写窗口！选择好新固件的文件地址！然后点send file
[http://ishare.iask.sina.com.cn/f/34231756.html](http://ishare.iask.sina.com.cn/f/34231756.html)
![J-link <wbr>V8固件升级记](http://s2.sinaimg.cn/middle/5bdee302g7acb178af501&690)
提示是否加密！选择yes！
![J-link <wbr>V8固件升级记](http://s3.sinaimg.cn/middle/5bdee302gcbeeec623ef2&690)
烧录进行中～～～～
![J-link <wbr>V8固件升级记](http://s15.sinaimg.cn/middle/5bdee302gcbeeee208d3e&690)
再次提示是否要加密，选择no！之后退出程序！
拔掉J-link，然后从新插入usb！
![J-link <wbr>V8固件升级记](http://s4.sinaimg.cn/middle/5bdee302gcbeef26932d3&690)
又会有新设备，这个时候要去下载安装J-Link ARM V4.40驱动就有了！
注意一定要是J-Link ARM V4.40 之后的版本都会封杀你的破解！
[http://download.csdn.net/detail/wm20031015/3926860](http://download.csdn.net/detail/wm20031015/3926860)
![J-link <wbr>V8固件升级记](http://s4.sinaimg.cn/middle/5bdee302gcbeef7f40ea3&690)
运行J-Link ARM V4.40提示升级，选择yes
![J-link <wbr>V8固件升级记](http://s6.sinaimg.cn/middle/5bdee302gcbef01d901e5&690)
![J-link <wbr>V8固件升级记](http://s14.sinaimg.cn/middle/5bdee302gcbef0232f93d&690)
提示升级成功！
从新插拔一下J-link！好了你的J-link V8又复活了！
双击J-Link ARM V4.40 程序正常运行了！
![J-link <wbr>V8固件升级记](http://s16.sinaimg.cn/middle/5bdee302g7acb190712af&690)
![J-link <wbr>V8固件升级记](http://s15.sinaimg.cn/middle/5bdee302gcbef03d0e7ce&690)
^_^，我能给的也就这么多了！如果还有什么问题，或者说新的4.5.0之后的版本需要破解的话，估计就又得等等了！不过至少现在J-link V8 又可以欢快的陪我们工作一阵子了！
