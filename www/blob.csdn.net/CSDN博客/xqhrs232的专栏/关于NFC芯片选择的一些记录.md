# 关于NFC芯片选择的一些记录 - xqhrs232的专栏 - CSDN博客
2016年07月27日 17:05:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1927
原文地址::[http://blog.csdn.net/lpx1233/article/details/45442347](http://blog.csdn.net/lpx1233/article/details/45442347)
相关文章
1\恩智浦推出全新NFC解决方案PN547----[http://www.21ic.com/np/rf/201203/108264.htm?&from=androidqq](http://www.21ic.com/np/rf/201203/108264.htm?&from=androidqq)
在做一个关于NFC的智能硬件项目，记录下选择NFC芯片时所获得的信息。
NXP PN532：这个应该是所有刚入门NFC开发的人最开始用的开发芯片了。这个芯片很开放，官网能下载所有文档，还有libnfc的支持。然而，pn532不遵守nfc-forum的nci标准，不能通过nfc场供电，在实现设计过后还发现它的无论是待机电流还是TX发射电流都比它datasheet上写的多（不知道是不是开发板问题）。另外，要模拟mifare 1k（4k）卡，它需要外接smartmx安全芯片，通过s2c（现在叫nfc-wi）接口。
NXP PN544（PN65N）：nxp的第二代nfc芯片。据说是Nexus S选用的nfc芯片。在nxp官网根本连这个芯片都找不到。不仅如此，libnfc也没有关于这个芯片的支持。仅有安卓源码中的libnfc-nxp（未证实）有libnfc关于pn544的移植。据我个人推断，需要和NXP签NDA后才能索取到相关的datasheet，User Manual， 天线设计指南等（太坑爹了连datasheet都没有，评估都没法评估，不过csdn还是有pn544的datasheet的→_→）。PN65N是PN544的另一个版本，内嵌smartmx安全芯片，不用外接其他芯片即可模拟mifare卡，与rfid基础设施对接。淘宝目前有这（两）款芯片的开发板，不过还不多，也就一两家。
NXP PN547（PN65O）：nxp的第三代nfc芯片。和pn544一样，官网根本搜不到。目前不清楚有什么开发工具及开发资料，也找不到datasheet。应该遵循NCI。同上，PN65O是PN547内嵌smartmx安全模块的版本。
NXP NPC100（PN65T）：应该是最新的NXP NFC芯片。找不到任何资料，但是看到有消费电子的产品应用了这个芯片。应该遵循NCI。PN65T是NPC100内嵌smartmx模块的版本。这代的nfc芯片应该特别适合消费电子的应用。
还有个pn548，也就是传说中iPhone6的65v10，据说是nxp和apple联合开发的nfc芯片，专供apple。应该也遵循NCI。
看到个帖子很有意思,copy过来:
首先，13.56M这个频段下的所有芯片，无论是读写器芯片还是卡芯片几乎都是NXP(恩智浦)的天下，当然也包括NFC芯片。NFC虽然也用的是这个频段，通讯链路层的技术也是一样的，但是应用领域不同，它只用在消费电子领域，我们都知道，消费电子的用量是很惊人的，因此也就注定了NFC芯片的价格很低，低到什么程读呢？就已楼主所说的RC522为例，这款芯片基本上算是NXP最为便宜的一块RFID读写器芯片，只支持一种协议。价格也就在10块左右，但是现在出的所有的NFC芯片都是通吃所有高频协议，价格也比10块要低，量大的话还更低。因此据我估计，NXP为了防止价格低功能强大的NFC芯片被用在传统的RFID领域，塑造了一个技术壁垒。就是资料或源码的开放程度。 
    NXP对于RFID行业的所有的读写器芯片都开放了底层的源码，这不但方便了用户去跑裸机程序，而且源码的开放也包括了对各种卡片的所有操作（读写认证）。 但是对于NFC芯片，NXP采用却是另外一种形式。  
    因为NFC只用于消费电子，但凡这种产品都是需要嵌入式操作系统的，因此NXP和包括谷歌诺基亚在内的各种系统平台供应商合作，开发基于安卓，WIN8等系统的应用，NXP只提供基于这些操作系统的API，这些API不但经过了层层的封装，而且目前只能实现一些基本的读取ID号，对卡片的简单读写等功能，并不支持复杂的操作。对于比较高级的卡芯片，比如NXP公司的DESFIRE EV1这种卡片，目前安卓所提供的API，就只能实现很简单的功能，所以NFC芯片在RFID行业基本上就没什么用了，比如我们就不能用NFC芯片去做一个传统的非接触式的读卡器了。 
    楼主所说的PN532，这款芯片比较特殊，它是NXP出的最早的一款NFC芯片，因为功能还不是很完善，后来就被NXP划为RFID芯片了，NXP对这款有着51内核的射频芯片开放了通讯协议，因此用户就可以随意使用了。但是这款芯片不支持ISO15693的协议，而该协议也被后来的NFC规范给包含进去了，因此从这点来看，它也并不是完整的NFC芯片，况且现在NXP也不对这款芯片做基于系统平台的开发。 
    对于真正的NFC芯片，比如PN54X,PN65X,价格是越来越便宜，但是对于这些芯片，NXP不提供通讯协议，不提供源码，只提供基于操作系统的一些API ,因此想在单片机平台上去使用这些芯片，目前难度很大。
引用:[准备开发款NFC模块/开发板，不知道大家有什么建议？](http://www.openedv.com/posts/list/0/27167.htm;jsessionid=8576FDF1DE67FA0A6FD3D19F659848D3) 13楼 **[luchaoyang](http://www.openedv.com/user/profile/7146.htm;jsessionid=8576FDF1DE67FA0A6FD3D19F659848D3)**http://www.openedv.com/posts/list/27167.htm
nxp下的好大一盘棋啊....
[](http://blog.csdn.net/lpx1233/article/details/45442347#)[](http://blog.csdn.net/lpx1233/article/details/45442347#)[](http://blog.csdn.net/lpx1233/article/details/45442347#)[](http://blog.csdn.net/lpx1233/article/details/45442347#)[](http://blog.csdn.net/lpx1233/article/details/45442347#)[](http://blog.csdn.net/lpx1233/article/details/45442347#)
- 
顶
0- 
踩
