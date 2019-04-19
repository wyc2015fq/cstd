# 采用PN512的NFC驱动电路设计 - xqhrs232的专栏 - CSDN博客
2015年04月12日 22:55:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1979
原文地址::[http://www.elecfans.com/dianlutu/187/20141110358255.html](http://www.elecfans.com/dianlutu/187/20141110358255.html)
相关文章
1、PN512智能卡驱动移植过程----[http://m.blog.csdn.net/blog/u011818716/39254731](http://m.blog.csdn.net/blog/u011818716/39254731)
2、【NXP】PN512 完全兼容NFC Forum的解决方案----[http://www.elecfans.com/soft/161/2014/20141112358481.html](http://www.elecfans.com/soft/161/2014/20141112358481.html)
3、PN512读卡代码----[http://dl.21ic.com/download/code/pn512_driver-rar-ic-113035.html](http://dl.21ic.com/download/code/pn512_driver-rar-ic-113035.html)
**NFC技术原理**
　　支持NFC的设备可以在卡操作或读写器模式下交换数据。在读写器模式下，启动NFC通信的设备，也称为NFC发起设备（主设备），在整个通信过程中提供射频场（RF-field）。它可以选择106kbps、212kbps或 424kbps其中一种传输速度，将数据发送到另一台设备。另一台设备称为NFC目标设备（从设备），不必产生射频场，而使用负载调制（load modulation）技术，即可以相同的速度将数据传回发起设备。此通信机制与基于ISO14443A、MIFARE和FeliCa的非接触式智能卡兼容，因此，NFC发起设备在读写器模式下，可以用相同的连接和初始化过程检测非接触式智能卡或NFC目标设备，并与之建立联系。
**PN512电路主要特性**
![](http://www.elecfans.com/uploads/allimg/141110/110021F41-0.gif)![](http://www.elecfans.com/uploads/allimg/141110/1100214U6-1.gif) 集成了RF场检测器 集成了数据模式检测器,支持S2C接口,集成了NFCIP-1的RF接口，传输速率高达424kbit/s
![](http://www.elecfans.com/uploads/allimg/141110/110021F41-0.gif)![](http://www.elecfans.com/uploads/allimg/141110/1100214U6-1.gif) 支持主机接口：
　　－SPI接口，高达10Mbit/s
　　－I2C接口，快速模式为400kbit/s，高速模式为3400kbit/s
　　－不同传输速率的串行UART，高达1228.8kbit/s，帧随RS232接口而定，接口的电压电平取决于端口的电源
　　－8位并行接口，带/不带地址锁存使能
![采用PN512的NFC驱动电路设计](http://www.elecfans.com/uploads/allimg/141110/1772475-141110105U2L6.jpg)
　　典型电路
