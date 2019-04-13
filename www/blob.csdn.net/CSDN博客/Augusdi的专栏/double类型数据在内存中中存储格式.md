
# double类型数据在内存中中存储格式 - Augusdi的专栏 - CSDN博客


2017年04月20日 13:55:18[Augusdi](https://me.csdn.net/Augusdi)阅读数：2889


﻿﻿
# [double类型数据在内存中中存储格式](http://www.cnblogs.com/onedime/archive/2012/11/20/2779723.html)
一个值为-1.35的float类型数据,它在存储器里面怎么存储,存放的格式是什么
应该是ieee standard 754的存储方式。
也就是第一位是符号位，然后是8位指数位，然后是23位尾数
IEEE Standard 754 for Binary Floating-Point Arithmetic
[http://www.wikilib.com/wiki?title=IEEE_754&variant=zh-sg](http://www.wikilib.com/wiki?title=IEEE_754&variant=zh-sg)
double双精度数据类型存储格式
IEEE 双精度格式为8字节64位，由三个字段组成：52 位小数 f ； 11 位偏置指数 e ；以及 1 位符号 s。
这些字段连续存储在两个 32 位字中
<iframe id="iframe_0.1399269880822277" src="https://www.cnblogs.com/show-blocking-image.aspx?url=http%3A%2F%2Fhiphotos.baidu.com%2F%25CA%25B7%25C4%25FE%2Fpic%2Fitem%2F9d42f51f51d60ab7e1fe0b89.jpg&amp;maxWidth=1200&amp;origin=https://www.cnblogs.com&amp;iframeId=iframe_0.1399269880822277" frameborder="0" scrolling="no" style="border: currentColor; border-image: none; width: 1200px;"></iframe>
它所表示的值由下表决定，
<iframe id="iframe_0.8775323699972194" src="https://www.cnblogs.com/show-blocking-image.aspx?url=http%3A%2F%2Fhiphotos.baidu.com%2F%25CA%25B7%25C4%25FE%2Fpic%2Fitem%2Fe9529c25d3ce7b2235a80f89.jpg&amp;maxWidth=1200&amp;origin=https://www.cnblogs.com&amp;iframeId=iframe_0.8775323699972194" frameborder="0" scrolling="no" style="border: currentColor; border-image: none; width: 1200px;"></iframe><iframe id="iframe_0.8830414731384697" src="https://www.cnblogs.com/show-blocking-image.aspx?url=http%3A%2F%2Fhiphotos.baidu.com%2F%25CA%25B7%25C4%25FE%2Fpic%2Fitem%2Fb59d59604557799d8cb10d89.jpg&amp;maxWidth=1200&amp;origin=https://www.cnblogs.com&amp;iframeId=iframe_0.8830414731384697" frameborder="0" scrolling="no" style="border: currentColor; border-image: none; width: 1200px;"></iframe>
详见数值计算指南
如何知道机器double类型数据的存储方式
从存储结构和算法上来讲，double和float是一样的，不一样的地方仅仅是float是32位的，double是64位的，所以double能存储更
高的精度。
任何数据在内存中都是以二进制（0或1）顺序存储的，每一个1或0被称为1位，而在x86CPU上一个字节是8位。比如一个16位（2
字节）的short int型变量的值是1000，那么它的二进制表达就是：00000011 11101000。由于Intel CPU的架构原因，它是按字节倒
序存储的，那么就因该是这样：11101000 00000011，这就是定点数1000在内存中的结构。
目前C/C++编译器标准都遵照IEEE制定的浮点数表示法来进行float,double运算。这种结构是一种科学计数法，用符号、指数和
尾数来表示，底数定为2——即把一个浮点数表示为尾数乘以2的指数次方再添上符号。下面是具体的规格：
````````符号位 阶码 尾数 长度
float 1 8 23 32
double 1 11 52 64
临时数 1 15 64 80
由于通常C编译器默认浮点数是double型的，下面以double为例：
共计64位，折合8字节。由最高到最低位分别是第63、62、61、……、0位：
最高位63位是符号位，1表示该数为负，0正；
62-52位，一共11位是指数位；
51-0位，一共52位是尾数位。

按照IEEE浮点数表示法，下面将把double型浮点数38414.4转换为十六进制代码。
把整数部和小数部分开处理:整数部直接化十六进制：960E。小数的处理:
0.4=0.5*0+0.25*1+0.125*1+0.0625*0+……
实际上这永远算不完！这就是著名的浮点数精度问题。所以直到加上前面的整数部分算够53位就行了（隐藏位技术：最高位的1
不写入内存）。
如果你够耐心，手工算到53位那么因该是：38414.4(10)=1001011000001110.0110101010101010101010101010101010101(2)
科学记数法为：1.001……乘以2的15次方。指数为15！
于是来看阶码，一共11位，可以表示范围是-1024 ~ 1023。因为指数可以为负，为了便于计算，规定都先加上1023，在这里，
15+1023=1038。二进制表示为：100 00001110
符号位：正—— 0 ！
合在一起（尾数二进制最高位的1不要）：
01000000 11100010 11000001 11001101 01010101 01010101 01010101 01010101
按字节倒序存储的十六进制数就是：
55 55 55 55 CD C1 E2 40

