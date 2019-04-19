# ID卡和韦根协议 - xqhrs232的专栏 - CSDN博客
2015年05月27日 14:23:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：5754
原文地址::[http://blog.chinaunix.net/uid-26849197-id-3302874.html](http://blog.chinaunix.net/uid-26849197-id-3302874.html)
相关文章
1、[韦根协议](http://www.cnblogs.com/shootingstars/archive/2009/12/14/1623952.html)----[http://www.cnblogs.com/shootingstars/archive/2009/12/14/1623952.html](http://www.cnblogs.com/shootingstars/archive/2009/12/14/1623952.html)
2、韦根协议----[http://baike.baidu.com/link?url=CDbODXQrxDr88ydHlgfcBcb7x67PNINJSZMSYKKfKpucTrXGttHRQLyz74paTyz_AgjCZtxdiYVinZyGNDrz5q](http://baike.baidu.com/link?url=CDbODXQrxDr88ydHlgfcBcb7x67PNINJSZMSYKKfKpucTrXGttHRQLyz74paTyz_AgjCZtxdiYVinZyGNDrz5q)
3、韦根协议简介----[http://wenku.baidu.com/link?url=1plGyCIyoYOVyqyYtHE7RRyVgc40xVx9L0xpZWNHPbIjX-cQ7d_PDFtJmDxYRFxrmoS3URPb3CRGwJ4bMQkUkz_IV2AqDRkQbbjBOAKYxya](http://wenku.baidu.com/link?url=1plGyCIyoYOVyqyYtHE7RRyVgc40xVx9L0xpZWNHPbIjX-cQ7d_PDFtJmDxYRFxrmoS3URPb3CRGwJ4bMQkUkz_IV2AqDRkQbbjBOAKYxya)
Wiegand26以及Wiegand34协议， 是由美国工业安全委员会SIA的隶属组织制定的一个关于存取控制的标准协议。非接触式IC卡读写器接口和输出，也采用了该协议标准。
常用到的场合在ID卡的读取中：
1 韦根协议---韦根协议的数据信号
韦根协议又称韦根码,韦根码在数据的传输中只需两条数据线,一条为DATA0,另一条为DATA1。协议规定,两条数据线在无数据时均为高电平,如果DATA0为低电平代表数据0,DATA1为低电平代表数据1(低电平信号低于1V,高电平信号大于4V) ,数据信号波形如图1所示。图1中脉冲宽度在20μs到200μs之间,两个脉冲间的时间间隔在200μs到20ms之间。
![](http://blog.chinaunix.net/attachment/201208/6/26849197_1344247180u6a7.jpg)
2韦根协议--- 韦根码的数据格式（韦根26）
韦根码有多种数据格式,本文将以读卡头中较常用的韦根码(26bit)数据格式进行介绍。
第2到第9位为分组码,分组码共有8个二进制位,有256个状态;（电子卡HID码的低8位）
第10到第25位为标识码,标识码共16个二进制位,有65536个状态;（电子卡的PID号码）
第1位是第2到第13位的偶校验位;
第26位是第14到第25位的奇校验位,如表1。
![](http://blog.chinaunix.net/attachment/201208/6/26849197_1344247214G1pD.jpg)
 编号含义：
HID号码即Hidden ID code 隐含码，PID号码即Public ID code 公开码。 PID很容易在读出器的输出结果中找到，但HID在读出器的输出结果中部分或者全部隐掉。HID是一个非常重要的号码，它不仅存在于卡中，也存在于读卡器中。如果卡中的HID与读卡器中的HID不同的话，那么这张卡就无法在这个读卡器上正常工作。
附加：
注意数据从高位输出先输出
Wiegand 26格式：
各数据位的含义：
第 1 位： 为输出数据2—13位的偶校验位
第 2—9 位： ID卡的HID码的低8位
第10-25位： ID卡的PID号码
第 26 位： 为输出数据14-25位的奇校验位
Wiegand 34格式：
各数据位的含义：
第 1 位： 为输出第2—17位的偶校验位
第 2-17 位： ID卡的HID码
第18-33位： ID卡的PID号码
第 34 位： 为输出第18-33位的奇校验位
