# 二维码----百度百科 - xqhrs232的专栏 - CSDN博客
2013年11月05日 10:49:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4041
原文地址::[http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr)
相关文章
1、二维码生成工具V1.0----[http://www.duote.com/soft/51640.html](http://www.duote.com/soft/51640.html)
二维码（2-dimensional bar code），又称[二维条码](http://baike.baidu.com/view/71834.htm)，最早起源于[日本](http://baike.baidu.com/view/1554.htm)，它是用特定的[几何图形](http://baike.baidu.com/view/1235650.htm)按一定规律在平面（二维方向）上分布的黑白相间的图形，是所有信息数据的一把钥匙。在现代商业活动中，可实现的应用十分广泛，如：产品防伪/溯源、广告推送、网站链接、数据下载、商品交易、定位/导航、[电子凭证](http://baike.baidu.com/view/5038279.htm)、车辆管理、信息传递、名片交流、wifi共同享等等。
## 目 录
1[简介](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#1)
2[起源](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#2)
3[二维码原理](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#3)
- 3.1 [矩阵式原理](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#3_1)[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#3_1)
- 3.2 [行排式原理](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#3_2)
4[功能](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#4)
5[分类](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#5)
- 5.1 [堆叠式/行排式](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#5_1)[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#5_1)
- 5.2 [矩阵式二维码](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#5_2)[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#5_2)
6[业务分类](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#6)
- 6.1 [被读类业务](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#6_1)
- 6.2 [主读类业务](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#6_2)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#7)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#8)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#8_1)[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#8_1)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#8_2)[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#8_2)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#8_3)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#8_4)[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#8_4)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#8_5)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#8_6)[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#8_6)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#9)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#10)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#10_1)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#10_2)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#11)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#12)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#13)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#14)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#15)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#16)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_1)[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_1)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_2)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_3)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_4)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_5)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_6)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_7)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_8)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_9)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_10)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_11)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_12)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_13)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_14)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_15)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#17_16)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#18)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#19)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#20)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#21)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#22)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#22_1)[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#22_1)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#22_2)[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#22_2)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#22_3)[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#22_3)
- [](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#22_4)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#23)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#24)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#25)
[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#26)
## 1简介[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
二维条码/二维码（英文名：QR Code) 是用某种特定的[几何图形](http://baike.baidu.com/view/1235650.htm)按一定规律在平面（二维方向）分布的黑白相间的图形记录数据符号信息的。二维码是DOI（Digital Object Unique Identifier，数字对象唯一识别符）的一种，全球最大的二维码资源中心是“渡云”
 ，为全球用户统一提供了“唯一数据样本”的物品、人员、组织二维码识别信息。[1]
在[代码](http://baike.baidu.com/view/41.htm)编制上巧妙地利用构成计算机内部逻辑基础的“[0](http://baike.baidu.com/view/540005.htm)”、“[1](http://baike.baidu.com/view/26506.htm)”比特流的概念，使用若干个与二进制相对应的[几何](http://baike.baidu.com/view/15136.htm)形体来表示文字数值信息，通过图象输入设备或光电扫描设备自动识读以实现[信息](http://baike.baidu.com/view/1527.htm)自动处理。同时还具有对不同行的信息[自动识别](http://baike.baidu.com/view/1070528.htm)功能、及处理图形旋转变化等特点。在许多种类的二维条码中，常用的码制有：Data
 Matrix,[MaxiCode](http://baike.baidu.com/view/10473054.htm), Aztec,[QR
 Code](http://baike.baidu.com/view/1355593.htm), Vericode,[PDF417](http://baike.baidu.com/view/132249.htm),Ultracode, Code 49, Code 16K 等，QR Code码是1994年由[日本](http://baike.baidu.com/view/1554.htm)DW公司[发明](http://baike.baidu.com/view/89518.htm)。QR来自英文「Quick
 Response」的缩写，即快速反应的意思，源自发明者希望QR码可让其内容快速被解码。QR码最常见于日本、[韩国](http://baike.baidu.com/view/3299.htm)；并为目前日本最流行的二维空间条码。但二维码的安全性也正备受挑战，带有恶意软件和[病毒](http://baike.baidu.com/view/2584.htm)正成为二维码普及道路上的绊脚石。发展与防范二维码的滥用正成为一个亟待解决的问题。
二维码是一种比一维码更高级的条码格式。一维码只能在一个方向（一般是水平方向）上表达信息，而二维码在水平和垂直方向都可以存储信息。一维码只能由数字和字母组成，而二维码能存储汉字、数字和图片等信息，因此二维码的应用领域要广得多。
每种码制有其特定的字符集；每个[字符](http://baike.baidu.com/view/263416.htm)占有一定的宽度；具有一定的校验功能等。同时还具有对不同行的信息自动识别功能及处理图形旋转变化等特点。
## 2起源[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
二维码的起源是日本，原本是Denso Wave公司为了追踪汽车零部件而设计的一种条码。
## 3二维码原理[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
二维码的[原理](http://baike.baidu.com/view/437790.htm)可以从[矩阵式](http://baike.baidu.com/view/645099.htm)二维码的原理和行列式二维码的原理来讲述。
### 矩阵式原理
矩阵式[二维码](http://baike.baidu.com/view/132241.htm)（又称棋盘式二维码）是在一个[矩形](http://baike.baidu.com/view/150124.htm)空间通过黑、白像素在矩阵中的不同分布进行编码。右图是国内常用的矩阵式二维码实例，手机正常扫描后会跳出高可靠性的信息：
在矩阵元素位置上，出现方点、圆点或其他形状点表示二进制“1”，不出现点表示二进制的“0”，点的排列组合确定了矩阵式二维码所代表的意义。矩阵式二维码是建立在计算机图像处理技术、组合编码原理等基础上的一种新型图形符号自动识读处理码制。具有代表性的矩阵式二维码有：Code One、Maxi Code、QR Code、 Data Matrix等。
图21*21的矩阵中，黑白的区域在QR码规范中被指定为固定的位置，称为寻像图形（finder pattern）和定位图形(timingpattern)。寻像图形和定位图形用来帮助解码程序确定图形中具体符号的坐标。黄色的区域用来保存被编码的数据内容以及纠错信息码。蓝色的区域，用来标识纠错的级别（也就是Level L到Level H)和所谓的"Mask pattern",这个区域被称为“格式化信息”（format information）。
![矩阵式二维码原理](http://c.hiphotos.baidu.com/baike/s%3D220/sign=307b1fce2f738bd4c021b533918a876c/91529822720e0cf33d052ee80b46f21fbe09aa37.jpg)
矩阵式二维码原理
### 行排式原理
行排式二维码(又称：堆积式二维码或层排式二维码)，其[编码](http://baike.baidu.com/subview/237708/11062012.htm)原理是建立在一维码基础之上，按需要堆积成二行或多行。它在编码设计、校验原理、识读方式等方面继承了一维码的一些特点，识读设备与条码印刷与一维码技术兼容。但由于行数的增加，需要对行进行判定、其译码算法与软件也不完全相同于一维码。有代表性的行排式二维码有CODE49、CODE
 16K、PDF417等。其中的CODE49，是1987年由 David Allair 博士研制，Intermec 公司推出的第一个二维码。
## 4功能[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
信息获取（[名片](http://baike.baidu.com/view/48755.htm)、[地图](http://baike.baidu.com/view/30267.htm)、[WIFI](http://baike.baidu.com/view/43867.htm)密码、资料）
网站跳转（跳转到微博、手机网站、[网站](http://baike.baidu.com/view/4232.htm)）
广告推送（用户扫码，直接浏览商家推送的视频、音频广告）
[手机](http://baike.baidu.com/view/1455.htm)电商（用户扫码、手机直接购物下单）
优惠促销（用户扫码，下载电子优惠券，抽奖）
会员管理（用户手机上获取电子会员信息、VIP服务）
## 5分类[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
二维条码/二维码可以分为堆叠式/行排式二维条码和矩阵式二维条码。 堆叠式/行排式二维条码形态上是由多行短截的[一维条码](http://baike.baidu.com/view/139831.htm)堆叠而成；矩阵式二维条码以矩阵的形式组成，在矩阵相应元素位置上用“点”表示[二进制](http://baike.baidu.com/view/18536.htm)“1”，
 用“空”表示二进制“0”，“点”和“空”的排列组成代码。
### 堆叠式/行排式
堆叠式/行排式二维条码又称堆积式二维条码或层排式二维条码），其编码
![常见二维码](http://c.hiphotos.baidu.com/baike/s%3D250/sign=051dcfd9252dd42a5b0906ae333a5b2f/b90e7bec54e736d1fd1916b89b504fc2d562697c.jpg)
常见二维码
原理是建立在一维条码基础之上，按需要堆积成二行或多行。它在编码设计、校验原理、识读方式等方面继承了一维条码的一些特点，识读设备与条码印刷与一维条码技术兼容。但由于行数的增加，需要对行进行判定，其译码算法与[软件](http://baike.baidu.com/view/37.htm)也不完全相同于一维条码。有代表性的行排式二维条码有：Code
 16K、[Code 49](http://baike.baidu.com/view/3171004.htm)、[PDF417](http://baike.baidu.com/view/132249.htm)、MicroPDF417
 等。
### 矩阵式二维码
![常见二维码](http://c.hiphotos.baidu.com/baike/s%3D250/sign=051dcfd9252dd42a5b0906ae333a5b2f/b90e7bec54e736d1fd1916b89b504fc2d562697c.jpg)
常见二维码
[矩阵式](http://baike.baidu.com/view/645099.htm)二维条码（又称棋盘式二维条码）它是在一个矩形空间通过黑、白像素在矩阵中的不同分布进行编码。在矩阵相应元素位置上，用点（方点、圆点或其他形状）的出现表示二进制“1”，点的不出现表示二进制的“0”，点的排列组合确定了矩阵式二维条码所代表的意义。矩阵式二维条码是建立在[计算机图像处理](http://baike.baidu.com/view/1839652.htm)技术、组合编码原理等基础上的一种新型图形符号自动识读处理码制。具有代表性的矩阵式二维条码有：Code
 One、MaxiCode、QR Code、 Data Matrix、Han Xin Code、Grid Matrix 等。
常用的码制有：[PDF417二维条码](http://baike.baidu.com/view/333892.htm),Datamatrix二维条码, QR Code, Code 49, Code 16K ,Code one,等，除了这些常见的二维条码之外，还有Vericode条码、CP条码、Codablock F条码、田字码、
 Ultracode条码，Aztec条码。
![二维码结构](http://f.hiphotos.baidu.com/baike/s%3D250/sign=4c8eedbccc11728b342d8b27f8fdc3b3/4afbfbedab64034fe57ea02baec379310a551d6f.jpg)
二维码结构
## 6业务分类[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
二维码应用根据业务形态不同可分为被读类和主读类两大类。
### 被读类业务
平台将二维码通过彩信发到用户手机上，用户持手机到现场，通过二维码机具扫描手机进行内容识别。应用方将业务信息加密、编制成二维码图像后，通过短信或彩信的方式将二维码发送至用户的移动终端上，用户使用时通过设在服务网点的专用识读设备对移动终端上的二维码图像进行识读认证，作为交易或身份识别的凭证来支撑各种应用。
### 主读类业务
用户在手机上安装二维码客户端，使用手机拍摄并识别媒体、报纸等上面印刷的二维码图片，获取二维码所存储内容并触发相关应用。用户利用手机拍摄包含特定信息的二维码图像，通过手机客户端软件进行解码后触发手机上网、名片识读、拨打电话等多种关联操作，以此为用户提供各类信息服务。
## 7区别[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
多行组成的[条形码](http://baike.baidu.com/view/13740.htm)，不需要连接一个数据库，本身可存储大量数据，应用于：[医院](http://baike.baidu.com/view/21765.htm)、[驾驶证](http://baike.baidu.com/view/658668.htm)、[物料管理](http://baike.baidu.com/view/879309.htm)、[货物运输](http://baike.baidu.com/view/1488762.htm)，当条形码受一定破坏时，错误纠正能使条形码能正确解码二维码。它是一个多行、连续性、可变长、包含大量数据的符号标识。每个条形码有3
 - 90行，每一行有一个起始部分、数据部分、终止部分。它的字符集包括所有128个字符，最大数据含量是1850个字符。
[一维条形码](http://baike.baidu.com/view/2206274.htm)只是在一个方向（一般是水平方向）表达信息，而在垂直方向则不表达任何信息，其一定的高度通常是为了便于[阅读器](http://baike.baidu.com/view/1647906.htm)的对准。
一维条形码的应用可以提高信息录入的速度，减少差错率，但是一维条形码也存在一些不足之处：
数据容量较小： 30个[字符](http://baike.baidu.com/view/263416.htm)左右
只能包含[字母](http://baike.baidu.com/view/94782.htm)和[数字](http://baike.baidu.com/view/37407.htm)
[条形码](http://baike.baidu.com/view/13740.htm)尺寸相对较大（空间利用率较低）
条形码遭到损坏后便不能阅读
在水平和垂直方向的[二维空间](http://baike.baidu.com/view/619851.htm)存储信息的条形码， 称为[二维条形码](http://baike.baidu.com/view/886309.htm)（dimensional
 bar code）。
## 8投资形势[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
### 应用情况
在食品加工行业，生产厂家面临着如何向消费者提供放心安全食品的课题。食品
![食品安全追溯应用流程之产品生产流程](http://c.hiphotos.baidu.com/baike/s%3D220/sign=4cf6d55a0d33874498c5287e610ed937/adaf2edda3cc7cd937c2bfd33801213fb90e91c9.jpg)
食品安全追溯应用流程之产品生产流程
一旦出现问题，厂家就会蒙受重大的损失，品牌形象受损，甚而导致企业倒闭。如果能引入食品原材料追溯系统，即使上市的食品出现问题，食品厂家也能快速找出原因，可以只追回部分的问题食品而无需追回所有产品，可见食品追溯的重要性。
### 食品追溯方案
原材料[供应商](http://baike.baidu.com/view/462506.htm)在向食品厂家提供原材料时候进行批次管理，将原材料的原始生产数据制造日期、食用期限、原产地、生产者、遗传基因组合的有无使用的药剂等信息录入到二维码中并打印带有二维码的标签，粘贴在包装箱上后交与食品厂家。在食品厂家原材料入库时，使用数据采集器读取二维码，取得到货原
![食品安全追溯解决方案的经济效益](http://h.hiphotos.baidu.com/baike/s%3D220/sign=3b5ce0e58cb1cb133a693b11ed5556da/c2fdfc039245d688dbcb39aea5c27d1ed31b24ea.jpg)
食品安全追溯解决方案的经济效益
材料的原始生产数据。从该数据就可以马上确认交货的产品是否符合厂家的采购标准。然后将原材料入库。
根据当天的生产计划，制作配方。根据生产计划单，员工从仓库中提取必要的原材料，按各个批次要求使用各种原材料的重量进行称重、分包，在分包的原材料上粘贴带有二维码的标签，码中含有原材料名称、重量、投入顺序、原材料号码等信息。
根据[生产计划](http://baike.baidu.com/view/70215.htm)指示，打印带有[二维码](http://baike.baidu.com/view/132241.htm)的看板并放置在生产线的前方。看板上的二维码中录入有作业指示内容。在混合投入原材料时使用数据采集器按照作业指示读取看板上的码及各原材料上的二维码，以此来确认是否按生产计划正确进行投入并记录使用原材料的信息。在原材料投入后的各个检验工序，使用数据采集器录入以往手记录的检验数据，省去手工纪录。数据采集器中登录的数据上传到电脑中，电脑生成生产原始数据，使得产品、原材料追踪成为可能，摆脱以往使用纸张的管理方式。使用该数据库，在[互联网](http://baike.baidu.com/view/6825.htm)上向消费者公布产品的原材料信息。
### 餐厅的应用一
**手机电子菜单：**
为商家建立一个[手机](http://baike.baidu.com/view/1455.htm)电子菜单，餐饮店可以很轻松的将餐饮文化、菜品介绍等信息按照相关的指引录入。用户通过扫码获得该手机网站的跳转链接获取商家相关信息。[2]
**应用场景 ：**各宣传海报、手册，餐桌牌。
**商家利益 ：**①解决消费者由于**等位、等餐**带来的客户流失、客户抱怨问题；
②**差异化营销**，提升用户体验水平，增强品牌竞争力；
③通过电子菜单，餐厅会获得更广泛的关注和**口碑宣传**。
④提升服务员工作效率。
**消费者利益 ：**①到店就餐之前就可以通过电子菜单确定今天要点什么，**节省点餐时间**；
②扫码后电子菜单可以**永久保存**在消费者手机里，引导消费者的**再次消费**。菜单信息商家可以**实时更新**，**一次宣传，恒久收益**；
③更加详细的了解菜品，选择符合自己口味的[菜品](http://baike.baidu.com/view/996145.htm)。
### 手机购物的应用
“以二维码为入口的移动超市”是移动终端普遍应用和电子商务模式趋向成熟的标志和产物。“移动超市”将移动电子商务推向一个新台阶，它主要研究开发基于移动终端的移动交易和商家营销应用，借助二维码作为通道，实现消费者随时随地快速浏览真实商品，快速购物。另外，项目还将提供多种支撑商家营销的应用，以简单、高效、安全、便捷的创新营销模式解决企业商家新客户开发、老客户维护、移动电子商务、会员营销、打折促销、新品推广、顾客和商家互动等传统营销困境，节约了企业成本，为商家的广告、销售、促销、活动和客户反馈信息采集提供了一体化解决方案。
在国内，由广东正全科技有限公司为广大企业商家实现“以二维码为入口
![以二维码为入口的移动超市](http://c.hiphotos.baidu.com/baike/s%3D220/sign=7e769db65cdf8db1b82e7b663923dddb/c2cec3fdfc039245f49e7d218594a4c27d1e25f0.jpg)
以二维码为入口的移动超市
的移动超市”而研发的产品——正全易推，就能为企业商家提供一个手机电商平台，集专属二维码、 APP、企业网站、专用网络空间、域名、交流平台、购销平台及支付系统等功能为一体，商家可将所需的企业信息、产品介绍、优惠活动等以图文、音频或视频等形式上传到后台，形成独立并永久有效的二维码，方便顾客全方位了解商家的所有资讯，为商家打造了一个移动超市。
**扫描二维码进入手机网站**
用移动设备上的二维码扫描软件，可直接扫描二维码进入商家的手机网站，点击中意的产品，即可完成下单及支付，实现轻松购物的时尚理念。从解决企业商家、产业发展的角度来分析，“以二维码 
![手机网站](http://b.hiphotos.baidu.com/baike/s%3D220/sign=cd7b0eb75cdf8db1b82e7b663923dddb/c2cec3fdfc0392454793ee208594a4c27d1e25fe.jpg)
[手机网站(3张)](http://baike.baidu.com/albums/132241/132241/1/6455681.html#6455681$)
为入口的移动超市”的实施让企业商家的销售更具体明确，营销更便捷精准，企业商家投入非常低的成本，获得很好的营销宣传效果，一站式解决企业商家营销的多种困境，拉动企业收入规模增长，预计可直接或间接为企业增加20%-80%的收入。
从解决消费者消费需求的角度来分析，“以二维码为入口的移动超市”是一种消费革命，将改变消费者的消费习惯，让移动购物变得真实可靠，让用户作为产业链的一环参与到营销过程中，可实现双向交流，主动性实现消费人群的精准定位。
### “扫钱扫优惠”
二维码还可制成电子优惠券，宣传物料上一个醒目的二维码，配以一句简洁的如“扫钱扫优惠”之类的广告语，便能吸引消费者掏出手机扫码一探究竟。人都是充满好奇心、追求实惠的，这种扫码获取优惠券享实惠的方式，不仅能够增强消费者与商家之间的互动，同时也能提高消费者对商家的兴趣，加深印象。
二维码电子优惠券较之普通的纸质优惠券还有节约印刷成本、丰富宣传内容、便于“携带”等优势。纸质优惠券一旦印刷出来就无法更改，当优惠活动改变，优惠券就得重新印刷，这是一笔巨大的开销。而正全易推特有的二维码，支持优惠内容随时更改，但二维码本身始终保持不变，且内容可采用图文、音频、视频等各种形式相结合，还可直接链接进入商家专属网站，使优惠券信息丰富多彩。这样的二维码电子优惠券，不仅方便了商家对优惠券的管理，使优惠券宣传效果最大化，更方便了消费者将优惠券直接存储在手机中，避免了每次出门包包里都塞满了各种优惠券的困扰，让消费者觉得自己又时尚了一把。
### 可将二维码放在哪儿呢？
1、平面宣传物料：商家可在报纸、杂志、图书、海报、传单、优惠 
![二维码的应用](http://b.hiphotos.baidu.com/baike/s%3D220/sign=f0a1c5d139c79f3d8be1e3328aa0cdbc/d01373f082025aaf9d93fee5f9edab64034f1a7e.jpg)
[二维码的应用(7张)](http://baike.baidu.com/albums/132241/132241/1/6439917.html#6439917$)
券、广告牌、X展架、产品包装、个人名片等上加印二维码，既节省物料成本，让信息量最大化；又可引领潮流，实现信息快速获取，便捷保存。
2、视频广告宣传：在电视、视频、广告上巧妙地嵌入二维码，突破时空限制，实现信息延伸，又能形成观众互动，扩大传播效果。
3、线上推广：将二维码发布到企业网站、广告条、微博等媒介上以配合线上宣传。
## 9应用前景[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
食品安全是食品生产或食用时不会使消费者受害的一种担保，即食品中不应含有可能损害或威胁人体健康的有毒、有害物质或因素。食品卫生是为确保食品安全性和适合性，在食物链的所有阶段必须采取的一切条件和措施。
食品安全受环境条件的直接影响。空气、水、土壤质量，污染程度和使用的肥（饲）料，农药（兽药）直接影响种（养）植（动）物的有害物残留。工业有害物质及其他化学物质对食品的污染也渐趋严重。如多氯联苯、N－亚销基化合物、多环芳族化合物及农药等。对人类影响最大的首推具有蓄积性的农药和化学杂质如二恶英等物质。指通过摄食进入人体的有毒、有害物质所造成的疾病。一般指中毒性与传染性疾病，如食物中毒，肠道传染病，人畜共患传染病，寄生病等。
[食品](http://baike.baidu.com/view/5350.htm)带来的疾病结果给卫生体系造成压力，并降低经济生产率。全球每年发生的食源性疾病数十亿人。
2003年7月18日国务院办公厅发出“关于实施食品药品放心工程的通知”。由国家食品药品监督管理局做牵头，农业部、卫生部、质检总局、工商总局、商务部等协同实施。
2004年10月国务院发布《国务院关于进一步加强食品安全工作的决定》，明确政府部门在食品安全监管工作中的分工，从2005年1月1日起，农业部门负责初级农产品生产环节的监管，质检部门负责食品生产加工环节的监管，工商部门负责食品流通环节的监管，卫生部门负责餐饮业和食堂等消费环节的监管，食品药品监管部门负责对食品安全的综合监督、协调和依法组织查处重大事故。并且明确要求，建立食品安全监管责任制和责任追究制度。
科技部已将食品安全列入“十二五”重大科技专项，并联合卫生部、质检总局和农业部 ，投入2亿元对食品安全关键技术进行攻关，实现“从农田到餐桌”的全过程控制。“食品安全”重大专项将从我国食品安全现状和存在的问题出发，从4个方面开展行动，包括研究开发食品安全检测技术与相关设备、建立食品安全监测与评价体系、积累食品安全标准的技术基础数据和发展生产与流通过程中的控制技术，力争在“十二五”末期，构建共享的食品安全监控网络系统，包括环境和食源性疾病与危害的监测、危险性分析和评估等。这给二维码的发展带来机遇。
尽管二维码应用渐趋广泛，但与日韩等国相比，我国的二维码发展还远远不够。制约因素除了运营商的支持度外，还有技术、终端适配、盈利模式等方面。炒得很火热的是二维码与O2O(Online To Offline)模式的结合，即利用二维码的读取将线上的用户引流给线下的商家。腾讯很看好这个模式，马化腾称"二维码是线上线下的一个关键入口"。尽管有些人不看好二维码的应用，但无可否认，只要培养了足够多的用户群，再结合良好的商业模式，二维码将成为桥接现实与虚拟最得力的工具之一。
## 10相关发明[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
### PDF417码
PDF417码是由留美华人王寅敬（音）[博士](http://baike.baidu.com/view/22607.htm)发明的。PDF是取英文Portable Data File三个单词的首字母的缩写，意为“便携数据文件”。因为组成条形码的每一符号字符都是由4个条和4个空构成，如果将组成条形码的最窄条或空称为一个模块，则上述的4个条和4个空的总模块数一定为17，所以称417码或PDF417码。
1. 信息容量大
PDF417码除可以表示字母、数字、[ASCII](http://baike.baidu.com/view/15482.htm)字符外，还能表达[二进制数](http://baike.baidu.com/view/1349.htm)。为了使得编码更加紧凑，提高信息密度，PDF417在编码时有三种格式：
扩展的字母数字压缩格式 可容纳1850 个字符；
[二进制](http://baike.baidu.com/view/18536.htm)/[ASCII](http://baike.baidu.com/view/15482.htm)格式
 可容纳1108 个字节；
数字压缩格式 可容纳2710 个数字。
2. 错误纠正能力
一维条形码通常具有校验功能以防止错读，一旦条形码发生污损将被拒读。而二维条形码不仅能防止错误，而且能纠正错误，即使条形码部分损坏，也能将正确的信息还原出来。
3. 印制要求不高
普通打印设备均可打印，传真件也能阅读。
4. 可用多种阅读设备阅读
[PDF417](http://baike.baidu.com/view/132249.htm)码可用带光栅的激光阅读器，线性及面扫描的图像式阅读器阅读。
5. 尺寸可调以适应不同的打印空间
6. 码制公开已形成国际标准，我国也已制定了417码的国际标准。
### 变形
PDF417还有几种变形的码制形式：
PDF417截短码
在相对“干净”的环境中，条形码损坏的可能性很小，则可将右边的行指示符省略并减少[终止符](http://baike.baidu.com/view/514099.htm)。
PDF417微码
进一步缩减的PDF码。
宏PDF417码
当文件内容太长，无法用一个PDF417码表示时，可用包含多个（1~99999个）条形码分块的宏PDF417码来表示。
## 11优点[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
1．高密度[编码](http://baike.baidu.com/view/237708.htm)，信息容量大
2．编码范围广。
3．容错能力强，具有纠错功能：
4．[译码](http://baike.baidu.com/view/189742.htm)可靠性高：
5．可引入加密措施
6．成本低，易制作，持久耐用。
## 12缺点[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
据《2012年上半年全球手机安全报告》显示，2012年上半年查杀到手机恶意软件17676款，而其中二维码技术成为手机病毒、钓鱼网站传播的新渠道。
据警方介绍，扫描二维码有时候会刷出一条链接，提示下载软件，而有的软件可能藏有病毒。其中一部分病毒下载安装后会对手机、平板电脑造成影响；还有部分病毒则是犯罪分子伪装成应用的吸费木马，一旦下载就会导致手机自动发送信息并扣取大量话费。对此，资深手机软件专家洪志刚认为，利用二
![二维码病毒](http://e.hiphotos.baidu.com/baike/s%3D220/sign=1d29e7338a13632711edc531a18da056/c8ea15ce36d3d539baf6742c3a87e950342ab011.jpg)
二维码病毒
维码骗取手机话费是完全可行的，“理论上讲，二维码本身不会携带[病毒](http://baike.baidu.com/view/2584.htm)，但很多病毒软件可以利用二维码下载。然而，很多手机目前都使用开放式的手机平台，如果下载了这样的病毒软件，就会‘霸占’手机的短信发送接口，在用户不知道的情况下发送短信。这类短信往往都要扣除1元甚至10元的话费，手机话费就在用户懵懵懂懂之际快速流失，进了坏人的腰包。”
有相关专家提醒群众提高防范意识，扫描前先判断二维码发布来源是否权威可信，一般来说，正规的报纸、杂志，以及知名商场的海报上提供的二维码是安全的，但在网站上发布的不知来源的二维码需要引起警惕。应该选用专业的加入了监测功能的扫码工具，扫到可疑网址时，会有安全提醒。如果通过二维码来安装软件，安装好以后，最好先用杀毒软件扫描一遍再打开。
## 13注意事项[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
如果您是个扫码控，可以选用有识别功能的扫码器实时监控，比如360安全卫士等软件，都已经加入了一个监测功能，扫到有可疑网址时，会作出安全提醒。假如是通过二维码来安装软件，在安装好后，先用杀毒软件扫描一遍再打开。
其实绝大部分的恶意二维码都很难直接扣除手机费，而是通过引诱市民安装程序来实施诈骗。一定要认真阅读手机给出的安装提示。不要为了图方便就一路OK到底。
**二维码安全问题集中两方面直接扫码很危险**
有网友称其在街头张贴的广告上扫描了一个二维码之后，[手机](http://baike.baidu.com/view/1455.htm)刚充的100元话费就不翼而飞。他扫描了一个贴在墙上的交友类二维码后，并未[下载](http://baike.baidu.com/view/4238.htm)安装交友软件，然后就被通知手机欠费。结果检测手机发现，被安装了手机病毒软件，导致刚充的话费被偷走。
腾讯移动安全实验室专家表示，这是典型的二维码内置病毒案例；此外，二维码安全问题还存在于二维码扫描软件中，有些山寨二维码软件提前内置病毒或恶意代码，一旦安装就会遭遇恶意广告和扣费等问题。
## 14特点[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
1．高密度编码，信息容量大：可容纳多达1850个大写字母或2710个数字或1108个字节，或500多个汉字，比普通条码信息容量约高几十倍。
2．编码范围广：该条码可以把图片、声音、文字、签字、指纹等可以数字化的信息进行[编码](http://baike.baidu.com/view/237708.htm)，用条码表示出来；可以表示多种语言文字；可表示图像数据。
3．容错能力强，具有纠错功能：这使得二维条码因穿孔、污损等引起局部损坏时，照样可以正确得到识读，损毁面积达50%仍可恢复信息。
4．译码可靠性高：它比普通条码译码错误率百万分之二要低得多，误码率不超过千万分之一。
5．可引入加密措施：[保密性](http://baike.baidu.com/view/992854.htm)、防伪性好。
6．成本低，易制作，持久耐用。
7．条码符号形状、尺寸大小比例可变。
8．二维条码可以使用激光或CCD阅读器识读。
## 15存储[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
二维码可以存储各种信息，主要包括：
![《二维码存储的信息》的图片说明](http://f.hiphotos.baidu.com/baike/s%3D220/sign=34ad397d6c81800a6ae58e0c813733d6/dcc451da81cb39db5308a7a4d0160924aa18307c.jpg)
《二维码存储的信息》的图片说明
网址、名片、文本信息、特定代码。
根据信息的应用方式，又可以分为：
1、线上应用
如网址和特定代码，更多的是线上应用。
2、离线应用
如文本信息和名片，更多的是线下应用。
## 16颜色[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
通常我们所看到的以及大多数软件生成的二维码都是黑色的，但事实上彩色的二维码生成技术也并不复杂，并且备受年轻人的喜爱，已有一些网站开始提供彩色二维码在线免费生成服务了。基于二维码的纠错功能，即使二维码部分被覆盖或丢失，扫描设备依然能够识别出其记录的完整信息，当前已有不少“个性二维码”的生成工具，把一些个性图案与二维码进行合成，得到个性化并能被扫描设备识别的二维码，这种“个性二维码”也开始流行起来。
## 17应用[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
二维条码具有储存量大、保密性高、追踪性高、抗损性强、备援性大、成本便宜等特性，这些特性特别适用于表单、安全保密、[追踪](http://baike.baidu.com/view/331598.htm)、证照、存货盘点、资料备援等方面。
### 二维码印章
二维码不单只有商业用途，对于个人同样也有实现展示与推广的作用。
![二维码印章](http://d.hiphotos.baidu.com/baike/s%3D220/sign=7d70289548ed2e73f8e9812eb701a16d/08f790529822720e47f925eb79cb0a46f21fabe8.jpg)
[二维码印章(3张)](http://baike.baidu.com/albums/132241/132241/1/6538308.html#6538308$)
例如画家，可在自己的作品上加印二维码，关于创作过程、出版画册、举办画展等信息就能一目了然；假如是演艺明星，在宣传上加印二维码，粉丝就能欣赏到该明星的所有作品和照片，还能发布自己的最新资讯！
### 婚礼应用
传统的婚礼要大派请柬，现在（2013年）的婚礼只用发一张二维码即可。传统的婚礼现场来宾只能围观，不能互动。现在的二维码科技婚礼可以手机同步直播并且在线留言。记者看到了一款[二维码请柬](http://baike.baidu.com/view/10300499.htm)，用手机扫描之后，手机自动跳入新郎和新娘的空间，不仅看到婚礼的各项信息，而且能看到新郎和新娘的结婚照，还能留言评论收藏。
### 票务销售应用
传统票务系统升级为电子票务系统的商家和代理商，为合作者提供了从网络电商平台搭建、软硬件集成开发、开放接口、维护等全系统的方案，建立的电商平台直接接入各种网银平台，用户在线支付完成后，凭得到的电子凭证或票据即可到此电商平台的对应实体商家消费，无需排队、无需等待、无需繁琐验证，让用户立即获得一系列完美的消费体验。
### 表单应用
公文表单、商业表单、进出口报单、舱单等资料之传送交换，减少人工重覆输入表单资料，避免人为错误，降低人力成本
### 保密应用
商业情报、[经济情报](http://baike.baidu.com/view/3728039.htm)、政治情报、军事情报、私人情报等机密资料之加密及传递。
### 追踪应用
公文自动追踪、生产线零件自动追踪、客户服务自动追踪、邮购运送自动追踪、维修记录自动追踪、危险物品自动追踪、后勤补给自动追踪、医疗体检自动追踪、生态研究(动物、鸟类...)自动追踪等。
### 证照应用
护照、身份证、挂号证、驾照、会员证、[识别证](http://baike.baidu.com/view/1043722.htm)、连锁店会员证等证照之资料登记及自动输入，发挥「随到随读」、「立即取用」的资讯管理效果。
### 盘点应用
物流中心、仓储中心、联勤中心之货品及固定资产之自动盘点，发挥「立即盘点、立即决策」的效果。
### 备援应用
文件表单的资料若不愿或不能以磁碟、光碟等电子媒体储存备援时，可利用二维条码来储存备援，携带方便，不怕折叠，保存时间长，又可影印传真，做更多备份。
### 报纸应用
二维码作为一种连接报纸、手机和网络的新兴数字媒体，报纸利用二维码技术打造“立体报纸”以来，看报的用户通过使用智能手机上的各类二维码软件扫描报纸上的二维码，报纸立即成“立体”，同时还可以轻松阅读观赏报纸的延伸内容。国内应用二维码的报纸有[华西都市报](http://baike.baidu.com/view/283479.htm)、[长江日报](http://baike.baidu.com/view/480397.htm)、[成都商报](http://baike.baidu.com/view/365618.htm)等。二维码应用使报纸的容量大大扩展,读报的乐趣也大大增加,这意味着,报纸、期刊已经不仅仅是平面的新闻纸,更可以为我们带来一个全新[3D](http://baike.baidu.com/view/4376.htm)视听影像感受，并且为产品提供了更为全面的资讯以及更为便捷的购买方式，缔造了全方位的移动互联网商务平台。
### 网络资源下载
可以应用到网上的资源下载，比如电子书，游戏，应用软件等等。
### 产品溯源应用
在生产过程当中对产品和部件进行编码管理，按产品生产流程进行系统记录。可以在生产过程中避免错误，提高生产效率。同时可以进行产品质量问题追溯，比如[食品安全](http://baike.baidu.com/view/468403.htm)，[农产品追溯](http://baike.baidu.com/view/6698294.htm)，产品保修窜货管理。
### 景点门票应用
景点门票，[火车票](http://baike.baidu.com/view/48161.htm)告别传统文字纸张模式，采用二维码进行售票、检票，提高通行效率，防止伪票。
### 车辆管理应用
行驶证，驾驶证，车辆的年审文件，车辆违章处罚单等采用印制有二维码，将有关车辆上的基本信息，包括车驾号、发动机号、车型、颜色等车辆的基本信息转化保存在二维码中，其信息的隐含性起到防伪的作用，信息的数字化便于与管理部门的管理网络的实施实时监控。
### 创意应用
随着智能手机的普及，各种各样的二维码（2-dimensional bar code）应用也接踵而至，看看有哪些极具创意的二维码应用：1、二维码请柬；2、二维码展示海报；3、二维码签到；　4、二维码墓碑；5、二维码名片；6、二维码指示牌；7、二维码蛋糕；8、二维码宣传广告。9、二维码食品身份证
### 会议服务
二维码会议服务，是二维码技术在移动商务服务中的另一种应用，主要用于二维码会议签到。
主要包括：
会前：二维码彩信/短信[邀请函](http://baike.baidu.com/view/622445.htm)、彩信会刊、来宾入住安排、会议议程提示、短信温馨提示
会中：二维码签到、彩信[优惠券](http://baike.baidu.com/view/834864.htm)、[短信抽奖](http://baike.baidu.com/view/2365580.htm)、短信互动、议程提醒、就餐安排等
会后：彩信会刊、会后感谢
大大提高了会议的签到效率，由于签到二维码彩信识别终端设备会时时将与会记录通过GPRS传输至二维码签到记录平台，会议的参与情况和促销活动的效果可以做清楚的分析，真正实现会议营销的闭环，从会议主办邀请直到最后的参与情况都会非常高效的记录下来，这是未来势必会普及推广的会议签到形式，不仅高效而且低碳，是移动商务领域中的一项重大革新应用。
二维码会议签到流程：
1、登陆二维码会务管理系统
2、系统制作、生成二维码邀请函并下发给嘉宾
3、入场前扫码签到核实身份
4、实时了解嘉宾到会情况
5、到会嘉宾凭二维码参加抽奖
6、凭二维码兑奖
7、系统查询情况、导出数据
二维码应用市场逐渐发展起来，各种大小会议活动应用二维码[会务签到](http://baike.baidu.com/view/5237905.htm)越来越频密，二维码加快了签到流程，使得会议现场更加方便管理。
## 18电子商务应用[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
二维码将成为移动互联网和O2O的关键入口。随着电子商务企业越来越多地进行线上线下并行的互动，二维码已经成为电子商务企业落地的重要营销载体。二维码在电商领域的广泛应用，结合O2O的概念，带给消费者更便捷和快速的消费体验，成为电商平台连接线上与线下的一个新通路，对于产品信息的延展，横向的价格对比，都有帮助。
## 19手机二维码[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
在手机上的二维码是用特定的几何图形按一定规律在平面（二维方向上）分布的黑白相间的矩形方阵记录数据符号信息的新一代条码技术。具有信息量大、纠错能力强、识读速度快、全方位识读等特点。
条码识别：手机二维码可以印刷在报纸、杂志、广告、图书、包装以及个人名片等多种载体上，用户通过手机摄像头扫描二维码或输入二维码下面的号码、关键字即可实现快速手机上网，快速便捷地浏览网页、下载图文、音乐、视频、获取优惠券、参与抽奖、了解企业产品信息，而省去了在手机上输入UR
![手机二维码的应用](http://f.hiphotos.baidu.com/baike/s%3D220/sign=3ae3ee949c16fdfadc6cc1ec848e8cea/c8177f3e6709c93dedc31b489e3df8dcd100545f.jpg)
手机二维码的应用
L的繁琐过程，实现一键上网。同时，还可以方便地用手机识别和存储名片、自动输入短信，获取公共服务（如天气预报），实现电子地图查询定位、手机阅读等多种功能。随着3G的到来，二维码可以为网络浏览、下载、在线视频、网上购物、网上支付等提供方便的入口。[3]
## 20最大二维码[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
世界最大二维码是加拿大的一对农民夫妇Kraay与Rachel在一块面积达10英亩的玉米地上种出的，二维码的面积达到了2.8万平方米。而这块玉米地也被正式收录进了吉尼斯世界纪录，成为世界上最大的二维码。
![世界上最大二维码](http://d.hiphotos.baidu.com/baike/s%3D220/sign=5aba25f86709c93d03f209f5af3cf8bb/c995d143ad4bd113e05dd8c45aafa40f4afb05b7.jpg)
世界上最大二维码
Kraay与Rachel在翻看杂志的时候看到上面有不少的二维码，突发奇想的他们计划将自家农场的玉米地改造成二维码的形状，她在一位设计师和技术工人的帮助下完成了这幅创造纪录的巨幅“麦田”作品。二维码中包含的信息就是自家农场的网站，有人在乘飞机路过时候拿手机对着这块地一扫，就可以自动跳转到这户农家的网站。
## 21识读设备[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
**A.二维条码的阅读设备依阅读原理的不同可分为：**
（1） 线性CCD和线性图像式阅读器（Linear Imager）
可阅读一维条码和线性堆叠式二维码（如PDF417），在阅读二维码时需要沿条码的
![固定式SL-QC15S](http://a.hiphotos.baidu.com/baike/s%3D220/sign=ebf127c6b2119313c343f8b255390c10/f2deb48f8c5494ee48ade61d2df5e0fe99257e23.jpg)
固定式SL-QC15S
垂直方向扫过整个条码，我们称为“扫动式阅读”。这类产品比较便宜。
（2）带光栅的激光阅读器
可阅读一维条码和线性堆叠式二维码。阅读二维码时将光线对准条码，由光栅元件完成垂直扫描，不需要手工扫动。
（3） 图像式阅读器（Image Reader）
采用面阵CCD摄像方式将条码图像摄取后进行分析和解码，可阅读一维条码和所有类型的二维条码。
**B.二维条码的识读设备依工作方式的不同还可以分为：手持式、固定式。**
**手持式：**即二维码扫描枪。可以扫描PDF417、QR码、DM码二维码的条码扫描枪。
**固定式：**即二维码读取器，台式，非手持，放在桌子上或固定在终端设备里，比如SUMLUNG的SL-QC15S等等。
纸上印刷的二维码和手机屏幕上的二维码均可识别，因此广泛应用于电子票务，电子优惠券，会员系统，手机二维码登机等领域。
**C.二维条码的识读设备工业级应用主要使用进口品牌例如西门子**SIMATIC
SIMATIC MV420 and MV440一维/ 二维码读码器
对先进的生产系统，使用机器可识别的标记追踪产品及部件
![西门子工业级读码器](http://d.hiphotos.baidu.com/baike/s%3D220/sign=41d2b95ea8d3fd1f3209a538004f25ce/aa18972bd40735fa259464c99e510fb30f24085d.jpg)
西门子工业级读码器
是一项重要的需求。唯一的编码系统确保各部件的每项生产步骤及原料的使用都有序可控。同时，直接打码方式允许对产品批次的追踪，例如产品的责任认定。
SIMATIC MV420/MV440固定式一维/二维码读码器是一款高速、高读取可靠性的读码产品，并且有丰富的通讯及连接方式。该读码器不仅可读取标准的高对比度条码及二维码，并且可用于环境恶劣的工业应用条件下读取DPM码(Direct PartMarking)，即物体表面直接打码（通过激光蚀刻、喷墨、机械打标或化学腐蚀等方式直接标识在物体表面的码）。
灵活可选的光照方式及紧凑坚固的外型使MV420/MV440能**应用于多种工业应用，如：**汽车、包装、医药、化妆品及电子半导体、食品饮料等行业。
## 22应用案例[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
传统的O2O商业模式能将线上的用户引导到线下进行消费，对商家来说，这更像是一种短期的线上促销活动，因为用户不需要像线上那样需要先注册再进行消费，所以对用户来说这类消费主要是“一次性消费行为”，更为关键的是，线下商家的前台消费数据，在传统的O2O模式下没有一个总的数据库能够记录这些实际的消费数据。
### 背景1
二维码签到陆续在一些重要会议中出现，与会人员只需调出手机中参会前收到的二维码凭证信息，验证通过即可完成签到。
**电子签到解决方案**
二维码签到系统是一项“凭证”类的移动数据业务新产品，属于手机二维码被读应用，
![二维码签到系统](http://f.hiphotos.baidu.com/baike/s%3D220/sign=f5e0344e269759ee4e5067c982fb434e/342ac65c1038534306e8d7629313b07eca80884a.jpg)
二维码签到系统
是将[现代移动通信技术](http://baike.baidu.com/view/5993582.htm)和二维码编码技术结合在一起，把传统凭证的内容及持有者信息编码成为一个二维码图形，并通过短信、彩信等方式发送至用户的手机上，使用时，通过专用的读码设备对手机上显示的二维码图形进行识读验证即可。它最大的特点是惟一性和安全性，不仅节约了成本，更重要的是节省时间提高效率方便使用，同时还非常环保和时尚。
如何高效检测二维码：参会、参展前，主办方只需通过系统给参与人员发去含有二维码的电子邀请函（彩信、短信），此二维码含有会务信息，作为展会签到凭证。展会签到时，参与人员只需携带手机轻松完成签到，带来了全新的参会体验。同时也避免了主办方现场核对信息时手忙脚乱的尴尬，而且非常安全，有效核实身份，杜绝误闯者。参与人员的信息在验证时，通过无线通信系统传输到系统数据库，这样展会的参与情况一目了然，方便了展会后对参与情况的统计工作。整个流程采用了全电子化，以手机作为入场凭证，实现了会议签到的信息化。
### 背景2
为一些房地产公司、汽车、家居品牌等准备投放户外媒体广告的商家量身定做，通过体验式营销的解决方案让消费者能够感受到商品价值，从而产生购买行为，为企业销售和发展助力。
**广告媒体系统方案**
![](http://f.hiphotos.baidu.com/baike/s%3D250/sign=2fb52d2b10dfa9ecf92e511252d0f754/023b5bb5c9ea15ceebb5af3eb7003af33a87b2aa.jpg)
浏览商家官方网站，活动主题页面快速跳转，扫码即可浏览商家所有产品及信息， 让用户快速了解广告完整所有信息。
浏览商家微博，省去输入查找的繁琐过程，快速关注，累积粉丝新渠道，时时浏览商家微博新产品动态。
部分实体商城商品一拍即买，手机上实现购物，无论实物商品还是虚拟商品，均可一拍即购，多种支付方式，让手机购物更为便捷，折扣券、积分大礼，扫码即有。
**优势**
整合营销（二维码结合传统媒体无限延伸广告内容及时效；消费者便捷入网，手机实时查看信息）
即时互动（企业可发布调查、投票、会员注册；个人参与调查、信息评论、活动报名、手机投票、参与调查）
立体传播（二维码是移动互联网最便捷的入口，消费者时刻进行线上和线下的信息传播；社会化媒体传播的便捷工具）
**应用领域**
房地产项目、家居商品、汽车、景区游览项目、 实体商店等，商家可将二维码用于任何宣传介质上，用户通过手机软件扫码，即可在手机上获取商家所推送的各种服务。
**方案流程**
从简单的户外广告到形成完善的咨询-销售体系，户外广告媒体解决方案，让消费者从平面广告中感受到商品的价值。精准定位意向消费人群，实时统计商品关注和销售情况，让企业对户外平面广告的投放效果尽在掌握，无往不利！
### 背景3
![二维码药物标签 图片](http://g.hiphotos.baidu.com/baike/s%3D220/sign=8e1743434134970a4373172da5cbd1c0/d50735fae6cd7b89b895b1ef0f2442a7d8330ee4.jpg)
二维码药物标签 图片
美国辉瑞（动物保健品）集团，全球知名的动物保健品供应商，专注于动物保健产品的研发，及畜禽和宠物疾病的防治和治疗方面。主营产品：驱虫剂， 消炎药， 疫苗，抗生素以及相关的动物保健产品。
**面临问题**
根据欧盟最新建议：新的安全卫生规则适用于从农场到餐桌的所有食品及加强对食品安全问题的监管，以及有效应对未来食品链中可能出现的食品危机。加大了生产经营者的安全卫生责任，生产经营商主要依靠自我核查机制及对有害物的现代监控技术来确保食品安全卫生。
如何快速高效检测产品的信息成了当务之急，而准确的检测产品二维码信息则成了突破口。
**解决方法**
![](http://c.hiphotos.baidu.com/baike/s%3D220/sign=bb4d0cebd0a20cf44290f9dd46084b0c/5fdf8db1cb134954ebec6308564e9258d0094ae8.jpg)
解决之道：对于整个生产追溯的包装环节，即对[二维码](http://baike.baidu.com/view/132241.htm)标签上的追溯信息进行采集，第一个目的是保证数据的准确性，第二个目的是保证标签全球适应能力。
对于线上袋装、盒装的药物，贴表[系统实施](http://baike.baidu.com/view/265251.htm)打印，在打印完成后，对数据进行采集，并实施检测，数据被收集后，和数据库信息进行复核验证，确保没有错误后，实施贴标。在贴标完成后，对物品进行二次复核。确保产品最终完成，并可以正式出品。所有追溯用的二维条码均经过专用的二维码检测设备进行批次检测，并达到品质标准。
**矿井定位应用**
二维码的生成即利用二维码编码设备，首先将矿下工人的基本信息，例如姓名、年龄、工作情况、出勤记录和工作工段等进行采集，然后根据相应的信息利用编码软件生成特定的二维码标签，再利用二维码点阵打印机打印出含有工人特定信息的二维码标签，最后将其印制在工作服等特定的位置上便于进行识读。
### 向女朋友表白
北京邮电大学经济管理学院的方同学在明信片上用铅笔手绘一张二维码，通过手机上的二维码扫描软件，多番尝试后扫出了简单的一句情话：“小森，我爱你。”
首先找到一个在线生成二维码的网站，键入信息后生成二维码，再将图片适度放大之后打印出来，绘出参考线，将二维码图案分割成一个个小正方形，并对参考线内的小方格进行标号，然后在明信片上绘制出29×29的小方格参考线（视具体的信息量而定），并按照纸上的二维码编号将对应的方格涂黑，最后通过多个软件校验二维码即可。
还有一位在加拿大工作的长沙籍男士，在春节期间回国探亲的时候，通过某社交软件很机缘巧合的认识了一位女孩，该女孩与此男士有着类似的生活经历，其是一位美国堪萨斯大学的成都留学生，春节期间也是回国过年。二人通过很偶然的“摇一摇”，便由此开始熟识并恋爱。该男士更是利用二维码向女孩求婚，并最终修成了正果。
## 23定制[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
二维码可根据资讯内容的多少、应用的功能需求生成码图。一般简单表层的应用可将信息内容直接生成QR二维码使用。若作为有商业价值的应用，想包含更多资讯内容和延伸扩展服务，就需要开发[QR二维码](http://baike.baidu.com/view/3964421.htm)深层应用。
QR二维码**表层应用**多见于：发票、车票，简单的资讯型应用。
**优点：**普通解码软件解码
![艺术二维码（永远爱你）](http://f.hiphotos.baidu.com/baike/s%3D220/sign=d0f98144e71190ef05fb95ddfe1a9df7/314e251f95cad1c8f24aad8e7f3e6709c83d51a1.jpg)
艺术二维码（永远爱你）
**缺点：**信息不能修改编辑、仅限简单内容、不能交互信息、无加密（详见广州日报报道：动车票别乱丢！小心遭破译）
QR二维码**深层应用**多见于：验证、[防伪](http://baike.baidu.com/view/1290109.htm)、追溯、签到、军事、更多能交互信息的商业应用服务上。
**优点：**使用专业解码软件、加密机制不被破解、丰富资讯内容、[系统管理](http://baike.baidu.com/view/635537.htm)、可编辑修改、与用户交互信息、更多扩展功能、具商业价值的应用
**缺点：**手机应用国内标准未统一，需根据[手机系统](http://baike.baidu.com/view/1683550.htm)开发、下载
二维码表层的信息应用很简单，与一维条码差别不大。要实现极具商业价值的应用，必需使用二维码深层的核心技术，也是QR二维码真正的价值所在。。QR系统管理迅速在各国企业界扩展，QR方法成为企业实现竞争优势的工具。
专业二维码技术公司可对二维码进行定制服务，可变换颜色，图案，甚至可以制作出各种各样的造型。
专业的二维码应用服务不单纯有表层简单的功能供用户作为一种基本体验，更是拥有商业价值的后续应用。为广大企业接力二维码实现[市场营销](http://baike.baidu.com/view/9683.htm)。
**二维码的形成**
有几种方法本地生成和在线生成，可以说非常的简单，艺术美观的二维码稍微复杂一点，要在线生成。
**文本、短信、网址、个人名片、邮件地址、无线网络、电话、GPS等等都可以形成二维码。**
![](http://g.hiphotos.baidu.com/baike/s%3D220/sign=c3fb477051da81cb4ae684cf6267d0a4/f703738da9773912dd8e2ae5f8198618377ae28e.jpg)
## 24打印[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
二维码有印刷、打印之外，还有激光镭射方式。一些金属、陶瓷、玻璃物件，用传统的印刷和打印方式容易去除，达不到防伪效果。激光镭射，尤其是曲面激光成为首先方式。国内外一些激光机，开发的软件可以直接把二维码图片文件读入，镭射到物件上。
## 25发展方向[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
智能手机和平板电脑的普及应用催生了之前并不被看好的二维码应用，大家竞相投入大量资源进行技术研发。马化腾说：二维码是移动互联网入口。
二维码的应用，似乎一夜之间渗透到我们生活的方方面面，地铁广告、报纸、火车票、飞机票、快餐店、电影院、团购网站以及各类商品外包装上。作为物联网浪潮产业中的一个环节，二维码的应用从未这么受到关注，有专家甚至预测，将在两三年内形成上千亿的市场空间。
物联网的应用离不开自动识别，条码、二维码以及RFID被人们应用的更普遍一些，二维码相对一位码，具有数据存储量大，保密性好等特点，能够更好地与智能手机等移动终端相结合，形成了更好地互动性和用户体验。而与RFID相比较，二维码不仅成本优势凸显，他的用户体验和互动性也具有更好地应用前景。
在移动互联业务模式下，人们的经营活动范围更加宽泛，也因此更需要适时的进行信息的交互和分享。随着3G/4G移动网络环境下智能手机和平板电脑的普及，二维码应用不再受到时空和硬件设备的局限。产品基本属性、图片、声音、文字、指纹等可以数字化的信息进行编码捆绑，适用于产品质量安全追溯、物流仓储、产品促销以及商务会议、身份、物料单据识别等等。可以通过移动网络，实现物料流通的适时跟踪和追溯;帮助进行设备远程维修和保养;产品打假防窜及终端消费者激励;企业供应链流程再造等，以进一步提高客户响应度，将产品和服务延伸到终端客户。厂家也能够适时掌握市场动态，开发出更实用的产品以满足客户的需求，并最终实现按单生产，将大幅度降低生产成本和运营成本。
随着国内物联网产业的蓬勃发展，相信更多的二维码技术应用解决方案被开发出来，应用到各行各业的日常经营生活中来，届时，二维码成为移动互联网入口真正成为现实。
## 26商业模式[](http://baike.baidu.com/link?url=4w6oZeF9QxqXI7GToixJ5sV2-lxCpMNSNsjwUHgHmojQjBd82KvkPq14cyq6ueWr#)
二维码的应用的有主读和被读的概念。[国外](http://baike.baidu.com/view/1421031.htm)，二维码的平台式服务指的是有一个平台来供你生成二维码，并在那后面附上图片、文字、视频等各种各样的信息。本质上，二维码的内容是指向一个地址。所以二维码平台式服务属于被读的领域。
但这样的分类方式对用户和市场来说太模糊了，市场二维码的商业模式可以大致分成以下几种：
1.信息传播。不论是电子凭证还是[博物馆](http://baike.baidu.com/view/41497.htm)、媒体或商品信息，其实都是信息传播的概念，用户用手机扫描二维码，就可以进入它对应的地址，获得完整的数据。供应商通过发送电子凭证和铺设扫码硬件设备到本地商户，来建立一个完成的商业模式。
2.互动入口。比方你通过扫描二维码来关注微信好友，或优惠券领取、投票报名、参加调研等在手机上的可操作形式，向企业回传客户信息。企业就能将广告投放效应最大化，获得宝贵的用户互动数据。这样的互动购买模式已经在[电影](http://baike.baidu.com/view/2382.htm)、[电视](http://baike.baidu.com/view/8623.htm)、[杂志](http://baike.baidu.com/view/60467.htm)、[宣传册](http://baike.baidu.com/view/1989570.htm)、[广告](http://baike.baidu.com/view/2324.htm)等领域开始使用。
微信在较早的版本中已经开始提供扫二维码服务，用户可以通过扫拍他人二维码而直接建立微信关系，微信在4.0版本后还提供了可以直接通过扫拍二维码而实现手机和电脑微信同步的功能，非常巧妙的结合了二维码和验证的特性。新浪微博也在近期推出了制作二维码，扫二维码后可以关注微博等功能。
2012年，微信推出公众平台面向企业和个人开放，打开了自媒体时代。同时，二维码的运用更达到了前所未有的高度，扫一扫加关注几乎是所有公众号推广的方式。越来越多媒体节目登录微信公众平台（如中国梦之声），更加方便了收视观众与媒体间的沟通，通过在节目中放出账号二维码，在短期内迅速积累到庞大用户群。
3.形成购买。这就直接把你带往某个商品的电子商务平台，产生交易。
原来需要进实体店或在网上购买的流程，已经可以通过扫拍二维码而实现，在手机上完成购物支付流程。这样的方式可以弥补在原来无法涉足的空间进行消费的需求。支付宝跟分众传媒的合作就是，用户只要扫拍分众上的支付宝广告，就可以在手机上实现购物和支付。
以上几种，除了一些短小信息的二维码名片，都是用二维码指向一个链接地址，本质都是移动互联网入口的概念。中国二维码网为您 提供更优质的服务。

