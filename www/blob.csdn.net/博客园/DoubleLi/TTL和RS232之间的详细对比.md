# TTL和RS232之间的详细对比 - DoubleLi - 博客园






【背景】

之前就听过TTL，一直没搞懂其和RS232的区别。

最近，打算去买个USB转RS232的芯片，结果找到此产品：

[六合一多功能USB转UART串口模块CP2102 usb TTL485 232互转自恢复](http://detail.tmall.com/item.htm?id=23592088189)

正面：

![six-in-one cp2102 usb ttl 485 232 front](https://i1.wp.com/www.crifan.com/files/pic/uploads/2013/12/six-in-one-cp2102-usb-ttl-485-232-front_thumb.jpg?resize=615%2C303)

背面：

![six-in-one cp2102 usb ttl 485 232 rear](https://i2.wp.com/www.crifan.com/files/pic/uploads/2013/12/six-in-one-cp2102-usb-ttl-485-232-rear_thumb.jpg?resize=615%2C325)

其中的：

![six-in-one usb rs232 ttl rs485 chip functions](https://i2.wp.com/www.crifan.com/files/pic/uploads/2013/12/six-in-one-usb-rs232-ttl-rs485-chip-functions_thumb.jpg?resize=581%2C558)

以及引脚说明：

![six-in-one cp2102 usb ttl 485 232 pins note](https://i0.wp.com/www.crifan.com/files/pic/uploads/2013/12/six-in-one-cp2102-usb-ttl-485-232-pins-note_thumb.jpg?resize=398%2C286)

都提到了，不仅仅支持RS232，还支持TTL。

所以，再次遇到TTL，需要去搞清楚，TTL和RS232的区别。



【折腾过程】

1.参考：

[RS-232 vs. TTL Serial Communication](https://www.sparkfun.com/tutorials/215)

整理如下：

常见的微控制器中，都有了内置的UART(Universally Asynchronous Receiver/Transmitter)。

UART可以用来已串行方式收发数据。

UART是，以固定的某个速率（1200bps,9600bps,115200bps等），一次只能只传输一个bit比特位（所以叫做串行传输）

这种串行通信的方法，有时候也被叫做TTL（Transistor-Transistor Logic）Serial。

这种串行通信，在TTL级别上来说，对应的物理电平，始终是在0V和Vcc之间，其中常见的Vcc是5V或3.3V。

其中：

逻辑高电平==’1’==Vcc

逻辑低电平==’0’==0V



与此相对应的是：

你的（台式机，笔记本等）电脑中的串口，是和RS232（通信标准）所兼容的（所一致的） => 不是和TTL的标准所一致的

# RS232和TTL在软件协议层面是一样的

RS232的标准中，和你的微控制器中的串行信号所一样的，有：
- 也是：一次只传输一个bit比特位 -> 表示是serial
- 也是：也是以某个固定的速率去传输的->baudrate
- 也是：带或不带，parity极性->即校验位
- 也是：带或不带，停止位stop bit(s)



# RS232和TTL唯一不同在于硬件：电平表示的逻辑含义不同（相反）

RS232和TTL，唯一的，最根本的不同在于：

硬件（机制）不同：

（1）TTL

逻辑高电平==’**1**‘==Vcc==**3.3V或5V**

逻辑低电平==’**0**‘==0V==**0V**



（3）RS232：

逻辑高电平==’**0**‘==负电压== -3V~-25V==常为：**-13V**

逻辑低电平==’**1**‘==正电压== 3V~25V==常为：**13V**



# 为何RS232中要（很奇怪的）用负电压表示逻辑高电平呢？

按理来说：

TTL用正电压（3.3V或5V）表示逻辑1，用0电压表示逻辑0，是相对来说，比较符合人类的逻辑的

但是，为何RS232却，很奇怪的，用负电压表示逻辑高电平==1，呢？

那是因为：

此设计（用负电压表示逻辑1，正电压表示逻辑0）相对来说，更加：
- 抗（外界的电磁）干扰
- 抗外界的（电磁信号）噪音干扰
- 抗（信号的）衰减
- 使得和同样的TTL信号相比，RS232信号可以传输的更远

由此使得：

信号传输，相对更加稳定和可靠。



# RS232和TTL时序图对比

对于同样传输0b01010101来说，RS232和TTL的时序对比：



![rs232 vs ttl timing diagram](https://i0.wp.com/www.crifan.com/files/pic/uploads/2013/12/rs232-vs-ttl-timing-diagram_thumb.png?resize=615%2C365)



# RS232和TTL之间的转换

RS232和TTL之间的转换，不仅仅是简单的电平转换，还要考虑到其他一些因素，比如调节和矫正一些电平（提高或降低对应的电平），确保可能的有害的RS232电压不会破坏微控制器的串口针脚。

关于如何在RS232和TTL之间转换，目前已经有很多种解决方案了。

比如：

[RS-232 vs. TTL Serial Communication](https://www.sparkfun.com/tutorials/215)

中的[MAX-232](http://www.sparkfun.com/products/316)。



# 总结

很多知识，都是慢慢积累，才慢慢逐渐更加清楚的。。。

包括这个TTL。









