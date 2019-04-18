# 如何根据Ip获取地址信息--Java----待整理完善！！！ - weixin_33985507的博客 - CSDN博客
2015年04月20日 11:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
如何根据Ip获取地址信息--Java----待整理完善！！！
**QQWry.dat数据写入方法：**
 http://www.cnblogs.com/xumingxiang/archive/2013/02/17/2914524.html
# java读取QQwry.dat获得ip地址对应实际位置：
http://outofmemory.cn/code-snippet/541/java-duqu-QQwry-dat-get-ip-address-duiying-shiji-position
**[QQwry格式](http://blog.csdn.net/taft/article/details/77559)如下:**
A。文件头，共8字节 
B。若干条记录的结束地址+国家和区域 
C。按照从小到大排列的若干条起始地址+结束地址偏移，定长，7字节 
D。所有的IP都是用4字节整数记录的，并且遵照Intel次序，高位在后，低位在前。 
E。所有偏移量都是绝对偏移，就是从文件最开头计算。 
F。除了文件头用了两个4字节偏移，其余偏移量都用3字节。 
G。所有的偏移量也是低位在前，高位在后 
H。采用了一些字符串压缩技术
1。文件头，共8字节 
FirstStartIpOffset:4 第一个起始IP的绝对偏移 
LastStartIpOffset:4 最后一个起始IP的绝对偏移
2。起始地址+结束地址偏移记录区 
每条记录7字节，按照起始地址从小到大排列
StartIp:4 起始地址，整数形式的IP 
EndIpOffset:3 结束地址绝对偏移
3。结束地址+国家+区域记录区
EndIP:4 
国家+区域记录:不定长
4。国家+区域记录，有几种形式 
4.1。 
国家字符串，以 0x0 结束 
区域字符串，以 0x0 结束
4.2。 
Flag:1 标识取值： 0x1，后面没有Local记录 
0x2，后面还有Local记录 
sCountryOffset:3 实际的字符串要去这个偏移位置去找 
LocalRec:不定长，可选 根据Flag取值而定。这个记录也类似Country，可能采用压缩
4.3 LocalRec结构一 
flag:1 还不是十分了解这个flag含义，取值 0x1 or 0x2 
sLocalOffset:3
4.4 LocalRec结构二 
sLocal：不定长 普通的C风格字符串
注意：sCountryOffset指向的位置可能依然是4.2格式的，不知道为什么这样设计。
Flag取0x1时，sCountryOffset指向的位置可能是Flag为0x2，这时，LocalRec也在这里寻找。
现在不明白当记录Local的位置遇到0x2的标志意味着什么。
在qqwry.dat中，似乎存在一些错误。 
个别的记录Local会被写为： 
0x2,0x0,0x0,0x0 
根据规则，应该到文件最开头去寻找，可是，文件最开头显然不是记录这些的。
