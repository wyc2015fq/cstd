# Atmega128  AVR Studio熔丝位(Fuse)设置 - xqhrs232的专栏 - CSDN博客
2019年04月04日 11:23:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：29
原文地址::[http://blog.sina.com.cn/s/blog_6035432c0100ol6n.html](http://blog.sina.com.cn/s/blog_6035432c0100ol6n.html)
相关文章
1、AVR fuse位详解----[http://www.eeworld.com.cn/mcu/article_2016102230709.html](http://www.eeworld.com.cn/mcu/article_2016102230709.html)
2、[关于AVR单片机熔丝位的设置和拯救方法大全 AVR单片机熔丝位的设置和详细的拯救方法](https://www.cnblogs.com/zhang747725388/archive/2012/08/17/2643397.html)----[https://www.cnblogs.com/zhang747725388/archive/2012/08/17/2643397.html](https://www.cnblogs.com/zhang747725388/archive/2012/08/17/2643397.html)
3、AVR单片机熔丝位设置方法和设置步骤 大全----[https://wenku.baidu.com/view/03fa2062102de2bd97058858.html](https://wenku.baidu.com/view/03fa2062102de2bd97058858.html)
4、AVR 熔丝位 设置----[https://jingyan.baidu.com/article/8065f87f80b75c23312498e9.html](https://jingyan.baidu.com/article/8065f87f80b75c23312498e9.html)
芯片：ATmega128,外接16M晶振
仿真和下载：AVR Studio4.17
**方法一：通过AVR Studio通过JTAG设置熔丝位**
例：把芯片时钟由内部8M晶振改成外部晶振16M
![Atmega128 <wbr> <wbr>AVR <wbr>Studio熔丝位(Fuse)设置](http://s15.sinaimg.cn/middle/6035432ct979d829b0f5e&690)
![Atmega128 <wbr> <wbr>AVR <wbr>Studio熔丝位(Fuse)设置](http://s13.sinaimg.cn/middle/6035432ct979d81a7a03c&690)
改成外接16M晶振时，先读一下当前熔丝状态，只有OCDEN,JTAGEN,SPIEN选中，
只要修改时钟源和Start-up time即可，而修改这个，在这个版本的AVR Studio中时以选择选项形式出现，很适合我们这种菜鸟。选择最后面的：
Ext.  Crystal/Resonator High Freq; Start-up time:16K CK + 64 ms
![Atmega128 <wbr> <wbr>AVR <wbr>Studio熔丝位(Fuse)设置](http://s4.sinaimg.cn/middle/6035432ct979d9e994883&690)
![Atmega128 <wbr> <wbr>AVR <wbr>Studio熔丝位(Fuse)设置](http://s7.sinaimg.cn/middle/6035432ct979d9c279e46&690)
选择完后，再次确认无误后，点击Program，修改熔丝位。
注意时钟一定要选合适，不然芯片会锁死。
-------------------------------------------------------------------------------------------------
ISP修改熔丝方式，等我用到的时候再来补充。
