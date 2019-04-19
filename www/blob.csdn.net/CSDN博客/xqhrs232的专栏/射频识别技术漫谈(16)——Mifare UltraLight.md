# 射频识别技术漫谈(16)——Mifare UltraLight - xqhrs232的专栏 - CSDN博客
2015年11月11日 09:59:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2004
原文地址::[http://blog.sina.com.cn/s/blog_9ed067ad01010t8k.html](http://blog.sina.com.cn/s/blog_9ed067ad01010t8k.html)
相关文章
1、[MIFARE系列1《MIFARE简介》](http://blog.csdn.net/yxstars/article/details/38079827)----[http://blog.csdn.net/yxstars/article/details/38079827](http://blog.csdn.net/yxstars/article/details/38079827)
2、MIFARE 1卡读写控制----[http://wenku.baidu.com/link?url=CekLuuVCKZaeW6WGLBfujW6tWM7tcSCAyrLwIinPv3DB-D-G8VHYqfuOh5i37FqyTdZZ85gp8E1p-Th_WcjZ6srNZWCPInclgpWzMhc_G9q#](http://wenku.baidu.com/link?url=CekLuuVCKZaeW6WGLBfujW6tWM7tcSCAyrLwIinPv3DB-D-G8VHYqfuOh5i37FqyTdZZ85gp8E1p-Th_WcjZ6srNZWCPInclgpWzMhc_G9q#)
Mifare UltraLight又称为MF0，从UltraLight(超轻的)这个名字就可以看出来，它是一个低成本、小容量的卡片。低成本，是指它是目前市场中价格最低的遵守ISO14443A协议的芯片之一；小容量，是指其存储容量只有512bit(Mifare S50有8192bit)。
    Mifare UltraLight的512bit存储容量分成16个Page，每个Page包含4个字节，如下图所示：
![射频识别技术漫谈(16)——Mifare <wbr>UltraLight](http://s14.sinaimg.cn/middle/9ed067adhc7fc5353d36d&690)
    Page0和Page1以及Page2的第1个字节是卡片的7字节序列号及其校验字节，其中BCC0=0x88⊕SN0⊕SN1⊕SN2，BCC1=SN3⊕SN4⊕SN5⊕SN6，SN0是制造商代码，由于Mifare
 UltraLight是NXP公司出品，因而SN0固定为04H。Page2的第2个字节Internal作为内部数据保留。以上共10个字节出厂时固化在存储区内，用户无法更改。
    Page3是一次性烧录(One Time Programmable,OTP)页,该页的内容在卡片出厂时全部被写为“0”，用户使用时只能把某一位的内容写为“1”，而永远也不能把“1”写为0，也就是说，新写入的4字节内容与卡内原来的内容进行异或，异或后的结果存储在卡片中。
    Page4-Page15是可读写的用户数据区，出厂时其内容初始化为0，用户可以任意读写。
    Page2的第3和第4个字节用于将存储区锁定为只读。如下图所示，L4-L15的某一位设置为1，则对应序号的Page内容锁定为只读，每一个Page都可以单独设置。Lotp用于锁定Page3为只读。“螳螂捕蝉，黄雀在后”，Lotp-L15可以锁定别人，这些位本身又被三个BL位锁定，BL15-10用于锁定L15-L10，BL9-4用于锁定L9-L4，BLotp用于锁定Lotp。所有的这16个锁定位也具有OTP特性，通俗的讲就是这些“锁”没有“钥匙”，一旦锁死就再也改不回来了，所以锁定时一定要小心。
![射频识别技术漫谈(16)——Mifare <wbr>UltraLight](http://s15.sinaimg.cn/middle/9ed067adhc8365373792e&690)
Mifare UltraLight的读写操作和 Mifare S50是完全兼容的，这里的“兼容”是指二者可以使用同一个读卡器硬件，同一套软件。当然若软硬件完全相同就不是两种卡了，二者的区别主要体现在软件操作上，包括以下4个方面：
    一是Mifare UltraLight的卡序列号有7个字节，而Mifare S50的卡序列号只有4个字节，因此在卡片防冲突选择阶段需要两层(Cascade，93H和95H)操作；
    二是Mifare UltraLight没有密码，不需要验证；
    三是Mifare UltraLight的Page相当于Mifare S50的BLOCK，因此Mifare UltraLight有16个BLOCK，且每个BLOCK只有4个字节，而Mifare S50有64个BLOCK,每个BLOCK有16个字节；
    四是Mifare UltraLight没有电子钱包功能。
    基于以上四点，在Mifare S50的程序中在卡请求命令成功执行后，如果判断卡类型字节为Mifare UltraLight(0044H),则在之后的操作中增加第二层防冲突选择，卡选择成功后直接对卡片的0-15块进行读写操作，每次读写只关注前4个字节，不使用电子钱包功能，这样就可以两种卡片完全兼容了。
    Mifare UltraLight适合一次性、不需要回收的低成本的电子票证、景区门票等场合的解决方案，据说推出Mifare UltraLight的本来目的是想与125KHz的ID卡竞争，并作为磁条卡的替代方案。从目前的情况看，Mifare UltraLight与ID卡的竞争情况并不乐观，而磁条卡的替代品也大多是ID卡。
