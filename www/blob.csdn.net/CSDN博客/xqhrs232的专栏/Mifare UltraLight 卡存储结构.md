# Mifare UltraLight 卡存储结构 - xqhrs232的专栏 - CSDN博客
2017年05月27日 15:34:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2026
原文地址::[http://blog.csdn.net/rabbit100/article/details/47415391](http://blog.csdn.net/rabbit100/article/details/47415391)
相关文章
1、MifareultralightIC卡说明----[https://wenku.baidu.com/view/e81a83966bec0975f465e259.html](https://wenku.baidu.com/view/e81a83966bec0975f465e259.html)
2、[Mifare Ultra Light 非接触式IC卡发卡总结](http://www.cnblogs.com/chenguanwei/archive/2011/01/07/1930169.html)----[http://www.cnblogs.com/chenguanwei/archive/2011/01/07/1930169.html](http://www.cnblogs.com/chenguanwei/archive/2011/01/07/1930169.html)
3、AN1303 MIFARE Ultralight as Type 2 Tag---- [http://www.nxp.com/documents/application_note/AN1303.pdf](http://www.nxp.com/documents/application_note/AN1303.pdf)
4、[NDEF格式的smart tag在Mifare UltraLight卡中的存储方式](http://blog.csdn.net/berryreload/article/details/6916329)----[http://blog.csdn.net/berryreload/article/details/6916329](http://blog.csdn.net/berryreload/article/details/6916329)
5、[mifare ultralight function specification](http://www.wontec.com.tw/images/jpg/IC/ultralight.pdf)----[http://www.wontec.com.tw/images/jpg/IC/ultralight.pdf](http://www.wontec.com.tw/images/jpg/IC/ultralight.pdf)
6、 [android nfc MifareUltralight读写](http://blog.csdn.net/u012303938/article/details/49928637)----[http://blog.csdn.net/u012303938/article/details/49928637](http://blog.csdn.net/u012303938/article/details/49928637)
7、射频识别技术漫谈(16)——Mifare UltraLight----[http://blog.sina.com.cn/s/blog_9ed067ad01010t8k.html](http://blog.sina.com.cn/s/blog_9ed067ad01010t8k.html)
**1.概述**
UltraLight卡是一种512位非接触式IC卡。
**2.电气特性**
  容量为512位，分为16块，每块4个字节
  每页可编程锁定只读功能
  32位用户可定义的一次性编程区域
  384位用户读、写区域
  数据可保留2年，可循环改写1000次
  唯一的7字节序列号
  工作频率：13.56MHZ
  通信速率：106KB波特率
  防 冲 突：同一时间可处理多张卡
  读写距离：在100mm以内（与天线有关）
  卡内无需电源
**3.存储结构**
UltraLight卡共512位，分为16块，每块为4个字节。存储结构如下：
|**块号**|**字节0**|**字节1**|**字节2**|**字节3**|**说明**|
|----|----|----|----|----|----|
|0|SN0|SN1|SN2|BCC0|Serial Number|
|1|SN3|SN4|SN5|SN6|Serial Number|
|2|BCC1|保留|Lock0|Lock1|保留/Lock|
|3|OTP0|OTP1|OTP2|OTP3|OTP|
|4|Data0|Data1|Data2|Data3|Data read/write|
|5|Data4|Data5|Data6|Data7|Data read/write|
|6|Data8|Data9|Data10|Data11|Data read/write|
|7|Data12|Data13|Data14|Data15|Data read/write|
|8|Data16|Data17|Data18|Data19|Data read/write|
|9|Data20|Data21|Data22|Data23|Data read/write|
|10|Data24|Data25|Data26|Data27|Data read/write|
|11|Data28|Data29|Data30|Data31|Data read/write|
|12|Data32|Data33|Data34|Data35|Data read/write|
|13|Data36|Data37|Data38|Data39|Data read/write|
|14|Data40|Data41|Data42|Data43|Data read/write|
|15|Data44|Data45|Data46|Data47|Data read/write|
（1）  第0、1块存放着卡的序列号等信息，只可读。依据ISO/IEC14443-3 校验位计算如下：
BCC0=CT⊕SN0⊕SN1⊕SN2
BCC1=SN3⊕SN4⊕SN5⊕SN6
（2）  第2块为LOCK BYTES, 设置字节2和字节3对应的位可以将从第3块到15块单独地锁定为只读区域。
  Lock0
|L7|L6|L5|L4|LOTP|BL15-10|BL9-4|BLOTP|
  Lock1
|L15|L14|L13|L12|L11|L10|L9|L8|
Lx  锁定X块为只读
BLX   锁定对应的Lx位
注意：一旦block-locking(BLX)位被设置为锁定配置，对应的内存区域将被冻结。如：BL15-10 设置为1，则L15到L10再也不能改变。
Lock0和Lock1可以通过写命令来设置，写入的内容与当前内容进行位或操作得到新的内容，初始值为0。该过程是不可逆转的。如果有一个位被置为1，就再也不能置为0。
（3）  第3页为OTP，即一次性编程，初始值为0。可以通过写命令来改变它的值，写入的值和当前值进行位或操作得到新的值。这个过程是不可逆转的。如果一个位被置为1，将再也不能置回0。
注意：该内存区域可以用作最大值为32的一次性计数器。
（4）  第4到15块为用户读/写区域，初始值为0。
下面我们来规划例子程序的MifareUltraLight标签存储。
本例子程序是把MifareUltraLight标签用来存储商品信息，为了简单起见，只包括商品名称，商品单价，销售状态。商品名称设计最长为16个中文字，商品单价设计为整数部分4位，小数部分4位。
销售状态有未售状态和已售状态，初始为未销售，初始值为”0000”，用”1111”来表示已售状态，如果未售状态读商品标签，读成功后，就把已售状态标记”1111”写进标签里。
商品名称前8个汉字存储在标签的块4至块7里。（表2黄色部分）
商品名称后8个汉字存储在标签的块8至块11里。（表2鲜绿色部分）
商品单价整数部分存储在标签的块12里。（表2红色部分）
商品单价小数部分存储在标签的块13里。（表2粉红色部分）
商品销售状态存储在块14里。（表2灰色部分）
|**块号**|**字节0**|**字节1**|**字节2**|**字节3**|**说明**|
|----|----|----|----|----|----|
|0|SN0|SN1|SN2|BCC0|Serial Number|
|1|SN3|SN4|SN5|SN6|Serial Number|
|2|BCC1|保留|Lock0|Lock1|保留/Lock|
|3|OTP0|OTP1|OTP2|OTP3|OTP|
|4|Data0|Data1|Data2|Data3|Data read/write|
|5|Data4|Data5|Data6|Data7|Data read/write|
|6|Data8|Data9|Data10|Data11|Data read/write|
|7|Data12|Data13|Data14|Data15|Data read/write|
|8|Data16|Data17|Data18|Data19|Data read/write|
|9|Data20|Data21|Data22|Data23|Data read/write|
|10|Data24|Data25|Data26|Data27|Data read/write|
|11|Data28|Data29|Data30|Data31|Data read/write|
|12|Data32|Data33|Data34|Data35|Data read/write|
|13|Data36|Data37|Data38|Data39|Data read/write|
|14|Data40|Data41|Data42|Data43|Data read/write|
|15|Data44|Data45|Data46|Data47|Data read/write|
表2
