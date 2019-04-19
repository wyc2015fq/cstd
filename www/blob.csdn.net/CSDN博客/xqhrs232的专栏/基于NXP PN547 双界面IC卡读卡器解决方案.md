# 基于NXP PN547 双界面IC卡读卡器解决方案 - xqhrs232的专栏 - CSDN博客
2016年07月27日 21:07:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2212
原文地址::[http://www.wpgholdings.com/news/detail/zhcn/program/18810](http://www.wpgholdings.com/news/detail/zhcn/program/18810)
相关文章
1、[[讨论]](http://bbs.52rd.com/forum.php?mod=forumdisplay&fid=82&filter=typeid&typeid=35)**PN544/PN547 NXP的NFC资料 无线支付和近距离通信----[http://bbs.52rd.com/Thread-286375-1-1.html](http://bbs.52rd.com/Thread-286375-1-1.html)**
2、PN547-C2VFBGA-NXP----[http://wenku.baidu.com/link?url=B1cbI3Gqgwvrrh-D1MOv3sL1n7vDKyv6XGJbheLliQgTdKLJAyZ-AcBmOObLe-1I41JzFbJDr0CC97fldYjYdi4K7I8VtYufSPRJM93j3T3](http://wenku.baidu.com/link?url=B1cbI3Gqgwvrrh-D1MOv3sL1n7vDKyv6XGJbheLliQgTdKLJAyZ-AcBmOObLe-1I41JzFbJDr0CC97fldYjYdi4K7I8VtYufSPRJM93j3T3)
3、GY7505 UART串口转I2C模块----[http://www.glinker.cn/GY7505_UART-I2C.asp](http://www.glinker.cn/GY7505_UART-I2C.asp)
NFC英文全称Near Field Communication，近距离无线通信。是由恩智浦公司发起，由诺基亚、索尼等著名厂商联合主推的一项无线技术，NFC由非接触式射频识别(RFID)及互联互通技术整合演变而来,在单一芯片上结合感应式读卡器、感应式卡片和点对点的功能，能在短距离内与兼容设备进行识别和数据交换。这项技术最初只是RFID技术和网络技术的简单合并，现在已经演变成一种短距离无线通信技术，发展态势相当迅速。NFC具有双向连接和识别的特点，工作于13.56MHz频率范围，作用距离接近10厘米。NFC技术在ISO
 18092、ECMA 340和ETSI TS 102 190框架下推动标准化，同时也兼容应用广泛的ISO 14443 Type-A、B以及Felica标准非接触式智能卡。
![](http://www.wpgholdings.com/uploads/bu/bu_99/iot/15Q2/SAC/152S5_0215_1.jpg)
![](http://www.wpgholdings.com/uploads/bu/bu_99/iot/15Q2/SAC/152S5_0215_4.jpg)
【展示板照片】
![展示板照片(正面)](http://www.wpgholdings.com/uploads/bu/bu_99/iot/15Q2/SAC/152S5_0215_3.jpg)
【方案方块图】
![方案方块图](http://www.wpgholdings.com/uploads/bu/bu_99/iot/15Q2/SAC/152S5_0215_2.jpg)
【系统功能】
① 1.8 or 3.3 DC输入
②13.56 MHZ 频率
③3--10MM读写距离
④Card Emulation模式
⑤Peer-to-Peer通訊模式
⑥Reader模式
【方案特性】
① ARM cortex M0核心。
②灵活的从NFC论坛主机接口符合国家标准（NCI）
③双SWP界面。
④ 可选与平台无关的软件堆栈
⑤ 提供GPIO PORT 可供外部单元控制
⑥ 支持安全单元控制接口
⑦ 支持最新的RF协议标准（ISO18000-3、Kovio tags、HID iClass tags） 
⑧ High Speed UART（PN547C2 不支持）
⑨ SPI
⑩ I2C 
- [我要联络](http://www.wpgholdings.com/news/detail/zhcn/program/18810#news-contact-content)
- [技术文档](http://www.wpgholdings.com/news/detail/zhcn/program/18810#news-tec-doc-content)
- [相关新闻](http://www.wpgholdings.com/news/detail/zhcn/program/18810#news-related-content)
|类别|标题|档案|
|----|----|----|
|簡易操作手冊|User Manual|[](http://www.wpgholdings.com/uploads/files/IoT/15Q2/SAC/152S5_0215_usermanual.zip)|
|硬件|BOM|[](http://www.wpgholdings.com/uploads/files/IoT/15Q2/SAC/152S5_0215_BOM.zip)|
|硬件|Data Sheet|[](http://www.wpgholdings.com/uploads/files/IoT/15Q2/SAC/152S5_0215_datasheet.zip)|
