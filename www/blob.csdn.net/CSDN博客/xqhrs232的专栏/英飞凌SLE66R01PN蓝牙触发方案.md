# 英飞凌SLE66R01PN蓝牙触发方案 - xqhrs232的专栏 - CSDN博客
2017年10月26日 23:08:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：150
个人分类：[NFC技术](https://blog.csdn.net/xqhrs232/article/category/2906045)
原文地址::[http://www.elecfans.com/tongxin/rf/20141208360346.html](http://www.elecfans.com/tongxin/rf/20141208360346.html)
相关文章
1、英飞凌 SLE66R01PN蓝牙触发方案介绍----[http://www.iot-online.com/net/nfc/2014/0729/25504.html](http://www.iot-online.com/net/nfc/2014/0729/25504.html)
2、infineon-technologies-ag-sle66r01pn_8a3ca6f10e----[https://wenku.baidu.com/view/55be4e76bed5b9f3f80f1c1f.html](https://wenku.baidu.com/view/55be4e76bed5b9f3f80f1c1f.html)
作为一款符合ISO14443A协议和NFC Forum Type 2 Tag 标准的 NFC标签芯片，英飞凌SLE66R01PN带有128 Bytes的用户存储容量，7 Bytes的UID，1万次以上的数据读写次数和10年的数据保存时间。
　　蓝牙配对功能是目前NFC Forum Type 2 Tag标签的主流应用领域，而大部分应用方案都是采用类似于下图1的不带触发功能的设计。工作前提是蓝牙模组处于开启状态，在NFC标签端存储蓝牙MAC地址，通过NFC匹配蓝牙MAC，以达到蓝牙配对的目的。
![蓝牙配对功能](http://www.elecfans.com/uploads/141208/1751355-14120Q55GH63.jpg)
　　而在另外一些产品应用中，需要通过NFC功能触发蓝牙模组的开启，实现蓝牙配对。SLE66R01PN芯片本身不带FD触发管脚，所以就如何实现触发功能，下面简单的介绍几种对比方案（芯片采用2.0x2.0x0.50mm，DFN4脚封装）。注：J1引脚连接到蓝牙模组端MCU的GPIO口。
　　方案一：需接外围电路，如图2。
![蓝牙配对功能](http://www.elecfans.com/uploads/141208/1751355-14120Q55R4E6.jpg)
　　引脚J1输出电压是以交流的方式输出，如果要实现触发功能还需要在蓝牙模组电路上添加整流电路，再连接到蓝牙模组MCU的GPIO上。
　　方案二：三极管开关触发，如图3
![蓝牙配对功能方案三](http://www.elecfans.com/uploads/141208/1751355-14120Q55TKX.jpg)
　　需要外接电源（蓝牙模组端电源）作为触发信号，而标签端的电场能量作为控制三极管的一个开关信号，从而实现触发，而此时引脚J1输出的为外接电源电压。
　　方案三：二极管稳压触发，如图4。
![二极管稳压触发](http://www.elecfans.com/uploads/141208/1751355-14120Q6000IW.jpg)
　　原理是电场能量通过二极管砍波、电容滤波实现输出一个稳定触发功能。二极管稳压触发模块是目前这类触发方案中性价比最高的一个，不需要外接电源，简化了蓝牙模组端的电路设计。
　　在实际电路中，蓝牙模组MCU的工作电压一般为1.8V-3.6V，方案三J1引脚输出的电压通过调节可以稳定在1.8V-3.3 V左右，从而实现稳定触发的功能。如图5（带触发功能NFC模块与蓝牙模组连接图 ）
![带触发功能NFC模块与蓝牙模组连接图](http://www.elecfans.com/uploads/141208/1751355-14120Q601295K.jpg)
图5 带触发功能NFC模块与蓝牙模组连接图
　　与目前市场上带FD输出管脚的NFC芯片相比，SLE66R01PN在Bom和电路设计上并没有劣势。因为带FD输出功能的NFC芯片在使用触发功能时，也需要在标签或蓝牙模组电路上增加一个上拉电阻实现触发功能，因为此类芯片的FD输出是开漏输出，需要蓝牙模组端电源作为触发信号，如果不加上拉电阻的话，依然无法实现触发功能。
　　随着可穿戴产品、耳机、互动游戏标签等低功耗设计要求产品的市场需求不断增加，带触发功能的NFC标签产品市场将不断扩大。
**更多智能蓝牙资讯与技术文章，可参阅电子发烧友Designs of week栏目**——“[**让你的智能蓝牙产品设计不再听天由命**](http://www.elecfans.com/EEReview/2014/1205/360193.html)”

