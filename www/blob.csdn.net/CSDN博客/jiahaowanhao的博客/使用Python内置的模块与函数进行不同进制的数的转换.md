
# 使用Python内置的模块与函数进行不同进制的数的转换 - jiahaowanhao的博客 - CSDN博客


2018年05月26日 21:19:09[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：63标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


[使用Python内置的模块与函数进行不同进制的数的转换](http://cda.pinggu.org/view/25652.html)
这篇文章主要介绍了使用Python内置的模块与函数进行不同进制的数的转换的方法,Python也使得读取纯二进制文件内容非常方便,需要的朋友可以参考下
binascii 模块：
它包含一个把二进制数值转换成十六进制的函数，同样也可以反过来转。 \#binary_value是二进制数值不是字符串，也不是int型的1010
binascii.b2a_hex(binary_value) \#\#binary_value 一般读二进制文件可以得到
>>'89'
python自带的builtin函数：
bin(num)   十进制数值 ===》二进制字符串
bin(10)
>> '0b1010'
oct(num)  十进制数值 ===》八进制字符串
oct(10)
>>'012'
hex(num) 十进制数值 ===》十六进制字符串
hex(20)
>>'0x14'
int(str, base) 其它进制字符串 ===》十进制的数值，其中base代表str具体是属于哪个进制，如果是2则表示str是二进制, 默认base为十进制
int('20')
>>20
int('10', 2)
>>2
int('10', 8)
>>8
int('20', 10)
>>20
int('20',16)
>>32
字符与数字转换函数：
chr(int)   整型 转 字符
chr(65)
>>'A',
ord(chr) 字符 转 整型
ord('a')
>>97,
最后，给一个读取图片文件二进制内容的示例：
\#!/usr/bin/env python
\#encoding: utf-8
import binascii
fh = open(r'C:\Temp\img\2012517165556.png', 'rb')
a = fh.read()
\#print 'raw: ',`a`,type(a)
hexstr = binascii.b2a_hex(a) \#得到一个16进制的数
\#print 'hex: ',hexstr, type(hexstr)
bsstr = bin(int(hexstr,16))[2:]
print 'bin: ',bsstr, type(bsstr)
1010刷屏的效果，是不是有点黑客帝国的赶脚啊，呵呵
![](http://www.cda.cn/uploadfile/image/20180526/20180526130127_82925.png)

