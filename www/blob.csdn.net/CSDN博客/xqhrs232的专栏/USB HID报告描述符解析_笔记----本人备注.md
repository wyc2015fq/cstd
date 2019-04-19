# USB HID报告描述符解析_笔记----本人备注 - xqhrs232的专栏 - CSDN博客
2018年03月01日 15:18:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：332
原文地址::[http://blog.csdn.net/benjamin721/article/details/51218233](http://blog.csdn.net/benjamin721/article/details/51218233)
相关文章
1、USB HID设备报告描述符详解----[http://blog.csdn.net/cazicaquw/article/details/6724951](http://blog.csdn.net/cazicaquw/article/details/6724951)
2、USBHID报告及报告描述符简介----[https://wenku.baidu.com/view/8e29f560bed5b9f3f90f1c96.html](https://wenku.baidu.com/view/8e29f560bed5b9f3f90f1c96.html)
3、[USB HID Report Descriptor 报告描述符详解](http://www.cnblogs.com/AlwaysOnLines/p/3859557.html)----[https://www.cnblogs.com/AlwaysOnLines/p/3859557.html?utm_source=tuicool&utm_medium=referral](https://www.cnblogs.com/AlwaysOnLines/p/3859557.html?utm_source=tuicool&utm_medium=referral)
4、USB HID报告及报告描述符简介----[http://blog.csdn.net/fjiale/article/details/7491932](http://blog.csdn.net/fjiale/article/details/7491932)
要了解报告描述符，需要两份资料：《device class definition for human interface device (HID)》, 《Universal Serial Bus HID Usage Tables》
例如：0x05, 0x01, // USAGE_PAGE (Generic Desktop)
0x05表示前缀，0x01为数据部分
0x05转换成二进制，就是00000101，按照HID类协议5.3 generic item format的定义，这个字节被分成3个部分:
- bit0~bit1代表的是这个前缀后面跟的数据长度，两位可以表示最大4字节的数据，即bsize
- bit2~bit3代表的是这个前缀的类型，总共可以有三种类型：0=main，1=global，2=local，3=reserved；
- bit4~bit7代表前缀的tag，一般分为input(二进制的1000 00 nn,即bit4~bit7=1000,代表一个tag，bit2~bit3=00，代表main，bit0~bit1=nn，代表这个前缀后面还有nn所代表的数据)，output(二进制的 1001 00 nn)，feature(1011 00 nn)，collection(1010 00 nn)，end collection(1100 00 nn)
![](https://img-blog.csdn.net/20160422164004868)
![](https://img-blog.csdn.net/20160422112825247)
![](https://img-blog.csdn.net/20160422112845851)
![](https://img-blog.csdn.net/20160422111609268)
![](https://img-blog.csdn.net/20160422111652159)
INPUT字段定义：
![](https://img-blog.csdn.net/20160425140533348)
//===========================================================
本人备注：：
1》报表描述符(Report)具体可以参考----USB_HID协议中文版_USB接口HID设备.pdf---------第8章 USB 接口 HID 设备
