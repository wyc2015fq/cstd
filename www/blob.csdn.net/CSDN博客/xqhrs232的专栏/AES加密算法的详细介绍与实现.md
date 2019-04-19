# AES加密算法的详细介绍与实现 - xqhrs232的专栏 - CSDN博客
2018年05月15日 21:44:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：465
原文地址::[https://blog.csdn.net/qq_28205153/article/details/55798628](https://blog.csdn.net/qq_28205153/article/details/55798628)
# AES简介
高级加密标准(AES,Advanced Encryption Standard)为最常见的对称加密算法(微信小程序加密传输就是用这个加密算法的)。对称加密算法也就是加密和解密用相同的密钥，具体的加密流程如下图： 
![加密流程图](https://img-blog.csdn.net/20170219082909688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面简单介绍下各个部分的作用与意义：
- 
明文P
> 
没有经过加密的数据。
- 
密钥K
> 
用来加密明文的密码，在对称加密算法中，加密与解密的密钥是相同的。密钥为接收方与发送方协商产生，但不可以直接在网络上传输，否则会导致密钥泄漏，通常是通过非对称加密算法加密密钥，然后再通过网络传输给对方，或者直接面对面商量密钥。密钥是绝对不可以泄漏的，否则会被攻击者还原密文，窃取机密数据。
- 
AES加密函数
> 
设AES加密函数为E，则 C = E(K, P),其中P为明文，K为密钥，C为密文。也就是说，把明文P和密钥K作为加密函数的参数输入，则加密函数E会输出密文C。
- 
密文C
> 
经加密函数处理后的数据
- 
AES解密函数
> 
设AES解密函数为D，则 P = D(K, C),其中C为密文，K为密钥，P为明文。也就是说，把密文C和密钥K作为解密函数的参数输入，则解密函数会输出明文P。
在这里简单介绍下对称加密算法与非对称加密算法的区别。
- 
对称加密算法
> 
加密和解密用到的密钥是相同的，这种加密方式加密速度非常快，适合经常发送数据的场合。缺点是密钥的传输比较麻烦。
- 
非对称加密算法
> 
加密和解密用的密钥是不同的，这种加密方式是用数学上的难解问题构造的，通常加密解密的速度比较慢，适合偶尔发送数据的场合。优点是密钥传输方便。常见的非对称加密算法为RSA、ECC和EIGamal。
实际中，一般是通过RSA加密AES的密钥，传输到接收方，接收方解密得到AES密钥，然后发送方和接收方用AES密钥来通信。
本文下面AES原理的介绍参考自《现代密码学教程》，AES的实现在介绍完原理后开始。
# AES的基本结构
AES为分组密码，分组密码也就是把明文分成一组一组的，每组长度相等，每次加密一组数据，直到加密完整个明文。在AES标准规范中，分组长度只能是128位，也就是说，每个分组为16个字节（每个字节8位）。密钥的长度可以使用128位、192位或256位。密钥的长度不同，推荐加密轮数也不同，如下表所示：
|AES|密钥长度（32位比特字)|分组长度(32位比特字)|加密轮数|
|----|----|----|----|
|AES-128|4|4|10|
|AES-192|6|4|12|
|AES-256|8|4|14|
轮数在下面介绍，这里实现的是AES-128，也就是密钥的长度为128位，加密轮数为10轮。 
上面说到，AES的加密公式为C = E(K,P)，在加密函数E中，会执行一个轮函数，并且执行10次这个轮函数，这个轮函数的前9次执行的操作是一样的，只有第10次有所不同。也就是说，一个明文分组会被加密10轮。AES的核心就是实现一轮中的所有操作。
AES的处理单位是字节，128位的输入明文分组P和输入密钥K都被分成16个字节，分别记为P = P0 P1 … P15 和 K = K0 K1 … K15。如，明文分组为P = abcdefghijklmnop,其中的字符a对应P0，p对应P15。一般地，明文分组用字节为单位的正方形矩阵描述，称为状态矩阵。在算法的每一轮中，状态矩阵的内容不断发生变化，最后的结果作为密文输出。该矩阵中字节的排列顺序为从上到下、从左至右依次排列，如下图所示： 
![state](https://img-blog.csdn.net/20170219132548906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
现在假设明文分组P为”abcdefghijklmnop”，则对应上面生成的状态矩阵图如下： 
![state2](https://img-blog.csdn.net/20170219134722812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上图中，0x61为字符a的十六进制表示。可以看到，明文经过AES加密后，已经面目全非。
类似地，128位密钥也是用字节为单位的矩阵表示，矩阵的每一列被称为1个32位比特字。通过密钥编排函数该密钥矩阵被扩展成一个44个字组成的序列W[0],W[1], … ,W[43],该序列的前4个元素W[0],W[1],W[2],W[3]是原始密钥，用于加密运算中的初始密钥加（下面介绍）;后面40个字分为10组，每组4个字（128比特）分别用于10轮加密运算中的轮密钥加，如下图所示： 
![keystate](https://img-blog.csdn.net/20170219152638324?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上图中，设K = “abcdefghijklmnop”，则K0 = a, K15 = p, W[0] = K0 K1 K2 K3 = “abcd”。
AES的整体结构如下图所示，其中的W[0,3]是指W[0]、W[1]、W[2]和W[3]串联组成的128位密钥。加密的第1轮到第9轮的轮函数一样，包括4个操作：字节代换、行位移、列混合和轮密钥加。最后一轮迭代不执行列混合。另外，在第一轮迭代之前，先将明文和原始密钥进行一次异或加密操作。 
![aes_struct](https://img-blog.csdn.net/20170219161202485?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上图也展示了AES解密过程，解密过程仍为10轮，每一轮的操作是加密操作的逆操作。由于AES的4个轮操作都是可逆的，因此，解密操作的一轮就是顺序执行逆行移位、逆字节代换、轮密钥加和逆列混合。同加密操作类似，最后一轮不执行逆列混合，在第1轮解密之前，要执行1次密钥加操作。
下面分别介绍AES中一轮的4个操作阶段，这4分操作阶段使输入位得到充分的混淆。
# 一、字节代换
## 1.字节代换操作
AES的字节代换其实就是一个简单的查表操作。AES定义了一个S盒和一个逆S盒。 
AES的S盒：
|行/列|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|0|0x63|0x7c|0x77|0x7b|0xf2|0x6b|0x6f|0xc5|0x30|0x01|0x67|0x2b|0xfe|0xd7|0xab|0x76|
|1|0xca|0x82|0xc9|0x7d|0xfa|0x59|0x47|0xf0|0xad|0xd4|0xa2|0xaf|0x9c|0xa4|0x72|0xc0|
|2|0xb7|0xfd|0x93|0x26|0x36|0x3f|0xf7|0xcc|0x34|0xa5|0xe5|0xf1|0x71|0xd8|0x31|0x15|
|3|0x04|0xc7|0x23|0xc3|0x18|0x96|0x05|0x9a|0x07|0x12|0x80|0xe2|0xeb|0x27|0xb2|0x75|
|4|0x09|0x83|0x2c|0x1a|0x1b|0x6e|0x5a|0xa0|0x52|0x3b|0xd6|0xb3|0x29|0xe3|0x2f|0x84|
|5|0x53|0xd1|0x00|0xed|0x20|0xfc|0xb1|0x5b|0x6a|0xcb|0xbe|0x39|0x4a|0x4c|0x58|0xcf|
|6|0xd0|0xef|0xaa|0xfb|0x43|0x4d|0x33|0x85|0x45|0xf9|0x02|0x7f|0x50|0x3c|0x9f|0xa8|
|7|0x51|0xa3|0x40|0x8f|0x92|0x9d|0x38|0xf5|0xbc|0xb6|0xda|0x21|0x10|0xff|0xf3|0xd2|
|8|0xcd|0x0c|0x13|0xec|0x5f|0x97|0x44|0x17|0xc4|0xa7|0x7e|0x3d|0x64|0x5d|0x19|0x73|
|9|0x60|0x81|0x4f|0xdc|0x22|0x2a|0x90|0x88|0x46|0xee|0xb8|0x14|0xde|0x5e|0x0b|0xdb|
|A|0xe0|0x32|0x3a|0x0a|0x49|0x06|0x24|0x5c|0xc2|0xd3|0xac|0x62|0x91|0x95|0xe4|0x79|
|B|0xe7|0xc8|0x37|0x6d|0x8d|0xd5|0x4e|0xa9|0x6c|0x56|0xf4|0xea|0x65|0x7a|0xae|0x08|
|C|0xba|0x78|0x25|0x2e|0x1c|0xa6|0xb4|0xc6|0xe8|0xdd|0x74|0x1f|0x4b|0xbd|0x8b|0x8a|
|D|0x70|0x3e|0xb5|0x66|0x48|0x03|0xf6|0x0e|0x61|0x35|0x57|0xb9|0x86|0xc1|0x1d|0x9e|
|E|0xe1|0xf8|0x98|0x11|0x69|0xd9|0x8e|0x94|0x9b|0x1e|0x87|0xe9|0xce|0x55|0x28|0xdf|
|F|0x8c|0xa1|0x89|0x0d|0xbf|0xe6|0x42|0x68|0x41|0x99|0x2d|0x0f|0xb0|0x54|0xbb|0x16|
状态矩阵中的元素按照下面的方式映射为一个新的字节：把该字节的高4位作为行值，低4位作为列值，取出S盒或者逆S盒中对应的行的元素作为输出。例如，加密时，输出的字节S1为0x12,则查S盒的第0x01行和0x02列，得到值0xc9,然后替换S1原有的0x12为0xc9。状态矩阵经字节代换后的图如下： 
![byte](https://img-blog.csdn.net/20170219171003857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2.字节代换逆操作
逆字节代换也就是查逆S盒来变换，逆S盒如下：
|行/列|0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|0|0x52|0x09|0x6a|0xd5|0x30|0x36|0xa5|0x38|0xbf|0x40|0xa3|0x9e|0x81|0xf3|0xd7|0xfb|
|1|0x7c|0xe3|0x39|0x82|0x9b|0x2f|0xff|0x87|0x34|0x8e|0x43|0x44|0xc4|0xde|0xe9|0xcb|
|2|0x54|0x7b|0x94|0x32|0xa6|0xc2|0x23|0x3d|0xee|0x4c|0x95|0x0b|0x42|0xfa|0xc3|0x4e|
|3|0x08|0x2e|0xa1|0x66|0x28|0xd9|0x24|0xb2|0x76|0x5b|0xa2|0x49|0x6d|0x8b|0xd1|0x25|
|4|0x72|0xf8|0xf6|0x64|0x86|0x68|0x98|0x16|0xd4|0xa4|0x5c|0xcc|0x5d|0x65|0xb6|0x92|
|5|0x6c|0x70|0x48|0x50|0xfd|0xed|0xb9|0xda|0x5e|0x15|0x46|0x57|0xa7|0x8d|0x9d|0x84|
|6|0x90|0xd8|0xab|0x00|0x8c|0xbc|0xd3|0x0a|0xf7|0xe4|0x58|0x05|0xb8|0xb3|0x45|0x06|
|7|0xd0|0x2c|0x1e|0x8f|0xca|0x3f|0x0f|0x02|0xc1|0xaf|0xbd|0x03|0x01|0x13|0x8a|0x6b|
|8|0x3a|0x91|0x11|0x41|0x4f|0x67|0xdc|0xea|0x97|0xf2|0xcf|0xce|0xf0|0xb4|0xe6|0x73|
|9|0x96|0xac|0x74|0x22|0xe7|0xad|0x35|0x85|0xe2|0xf9|0x37|0xe8|0x1c|0x75|0xdf|0x6e|
|A|0x47|0xf1|0x1a|0x71|0x1d|0x29|0xc5|0x89|0x6f|0xb7|0x62|0x0e|0xaa|0x18|0xbe|0x1b|
|B|0xfc|0x56|0x3e|0x4b|0xc6|0xd2|0x79|0x20|0x9a|0xdb|0xc0|0xfe|0x78|0xcd|0x5a|0xf4|
|C|0x1f|0xdd|0xa8|0x33|0x88|0x07|0xc7|0x31|0xb1|0x12|0x10|0x59|0x27|0x80|0xec|0x5f|
|D|0x60|0x51|0x7f|0xa9|0x19|0xb5|0x4a|0x0d|0x2d|0xe5|0x7a|0x9f|0x93|0xc9|0x9c|0xef|
|E|0xa0|0xe0|0x3b|0x4d|0xae|0x2a|0xf5|0xb0|0xc8|0xeb|0xbb|0x3c|0x83|0x53|0x99|0x61|
|F|0x17|0x2b|0x04|0x7e|0xba|0x77|0xd6|0x26|0xe1|0x69|0x14|0x63|0x55|0x21|0x0c|0x7d|
二、行移位
## 1.行移位操作
行移位是一个简单的左循环移位操作。当密钥长度为128比特时，状态矩阵的第0行左移0字节，第1行左移1字节，第2行左移2字节，第3行左移3字节，如下图所示： 
![shiftRows](https://img-blog.csdn.net/20170219174015167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2.行移位的逆变换
行移位的逆变换是将状态矩阵中的每一行执行相反的移位操作，例如AES-128中，状态矩阵的第0行右移0字节，第1行右移1字节，第2行右移2字节，第3行右移3字节。
# 三、列混合
## 1.列混合操作
列混合变换是通过矩阵相乘来实现的，经行移位后的状态矩阵与固定的矩阵相乘，得到混淆后的状态矩阵，如下图的公式所示： 
![col](https://img-blog.csdn.net/20170219203346436?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
状态矩阵中的第j列(0 ≤j≤3)的列混合可以表示为下图所示： 
![col2](https://img-blog.csdn.net/20170219203742516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中，矩阵元素的乘法和加法都是定义在基于GF(2^8)上的二元运算,并不是通常意义上的乘法和加法。这里涉及到一些信息安全上的数学知识，不过不懂这些知识也行。其实这种二元运算的加法等价于两个字节的异或，乘法则复杂一点。对于一个8位的二进制数来说，使用域上的乘法乘以(00000010)等价于左移1位(低位补0)后，再根据情况同(00011011)进行异或运算，设S1 = (a7 a6 a5 a4 a3 a2 a1 a0)，刚0x02 * S1如下图所示： 
![col3](https://img-blog.csdn.net/20170219204822517?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
也就是说，如果a7为1，则进行异或运算，否则不进行。 
类似地，乘以(00000100)可以拆分成两次乘以(00000010)的运算： 
![col4](https://img-blog.csdn.net/20170219205601683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
乘以(0000 0011)可以拆分成先分别乘以(0000 0001)和(0000 0010)，再将两个乘积异或： 
![col5](https://img-blog.csdn.net/20170219210554133?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因此，我们只需要实现乘以2的函数，其他数值的乘法都可以通过组合来实现。 
下面举个具体的例子,输入的状态矩阵如下：
|||||
|----|----|----|----|
|C9|E5|FD|2B|
|7A|F2|78|6E|
|63|9C|26|67|
|B0|A7|82|E5|
下面，进行列混合运算： 
以第一列的运算为例： 
![col7](https://img-blog.csdn.net/20170219213747965?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其它列的计算就不列举了，列混合后生成的新状态矩阵如下：|||||
|----|----|----|----|
|D4|E7|CD|66|
|28|02|E5|BB|
|BE|C6|D6|BF|
|22|0F|DF|A5|
2.列混合逆运算
逆向列混合变换可由下图的矩阵乘法定义： 
![col6](https://img-blog.csdn.net/20170219211139752?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以验证，逆变换矩阵同正变换矩阵的乘积恰好为单位矩阵。
# 四、轮密钥加
轮密钥加是将128位轮密钥Ki同状态矩阵中的数据进行逐位异或操作，如下图所示。其中，密钥Ki中每个字W[4i],W[4i+1],W[4i+2],W[4i+3]为32位比特字，包含4个字节，他们的生成算法下面在下面介绍。轮密钥加过程可以看成是字逐位异或的结果，也可以看成字节级别或者位级别的操作。也就是说，可以看成S0 S1 S2 S3 组成的32位字与W[4i]的异或运算。 
![roundadd](https://img-blog.csdn.net/20170220080512086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
轮密钥加的逆运算同正向的轮密钥加运算完全一致，这是因为异或的逆操作是其自身。轮密钥加非常简单，但却能够影响S数组中的每一位。
# 密钥扩展
AES首先将初始密钥输入到一个4*4的状态矩阵中，如下图所示。 
![keyextends](https://img-blog.csdn.net/20170220082316736?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个4*4矩阵的每一列的4个字节组成一个字，矩阵4列的4个字依次命名为W[0]、W[1]、W[2]和W[3]，它们构成一个以字为单位的数组W。例如，设密钥K为”abcdefghijklmnop”,则K0 = ‘a’,K1 = ‘b’, K2 = ‘c’,K3 = ‘d’,W[0] = “abcd”。 
接着，对W数组扩充40个新列，构成总共44列的扩展密钥数组。新列以如下的递归方式产生： 
1.如果i不是4的倍数，那么第i列由如下等式确定： 
W[i]=W[i-4]⨁W[i-1] 
2.如果i是4的倍数，那么第i列由如下等式确定： 
W[i]=W[i-4]⨁T(W[i-1]) 
其中，T是一个有点复杂的函数。 
函数T由3部分组成：字循环、字节代换和轮常量异或，这3部分的作用分别如下。 
a.字循环：将1个字中的4个字节循环左移1个字节。即将输入字[b0, b1, b2, b3]变换成[b1,b2,b3,b0]。 
b.字节代换：对字循环的结果使用S盒进行字节代换。 
c.轮常量异或：将前两步的结果同轮常量Rcon[j]进行异或，其中j表示轮数。 
轮常量Rcon[j]是一个字，其值见下表。
|j|1|2|3|4|5|
|----|----|----|----|----|----|
|Rcon[j]|01 00 00 00|02 00 00 00|04 00 00 00|08 00 00 00|10 00 00 00|
|j|6|7|8|9|10|
|Rcon[j]|20 00 00 00|40 00 00 00|80 00 00 00|1B 00 00 00|36 00 00 00|
下面举个例子： 
设初始的128位密钥为： 
3C A1 0B 21 57 F0 19 16 90 2E 13 80 AC C1 07 BD 
那么4个初始值为： 
W[0] = 3C A1 0B 21 
W[1] = 57 F0 19 16 
W[2] = 90 2E 13 80 
W[3] = AC C1 07 BD 
下面求扩展的第1轮的子密钥(W[4],W[5],W[6],W[7])。 
由于4是4的倍数，所以： 
W[4] = W[0] ⨁ T(W[3]) 
T(W[3])的计算步骤如下： 
1. 循环地将W[3]的元素移位：AC C1 07 BD变成C1 07 BD AC; 
2. 将 C1 07 BD AC 作为S盒的输入，输出为78 C5 7A 91; 
3. 将78 C5 7A 91与第一轮轮常量Rcon[1]进行异或运算，将得到79 C5 7A 91，因此，T(W[3])=79 C5 7A 91，故 
W[4] = 3C A1 0B 21 ⨁ 79 C5 7A 91 = 45 64 71 B0 
其余的3个子密钥段的计算如下： 
W[5] = W[1] ⨁ W[4] = 57 F0 19 16 ⨁ 45 64 71 B0 = 12 94 68 A6 
W[6] = W[2] ⨁ W[5] =90 2E 13 80 ⨁ 12 94 68 A6 = 82 BA 7B 26 
W[7] = W[3] ⨁ W[6] = AC C1 07 BD ⨁ 82 BA 7B 26 = 2E 7B 7C 9B 
所以，第一轮的密钥为 45 64 71 B0 12 94 68 A6 82 BA 7B 26 2E 7B 7C 9B。
# AES解密
在文章开始的图中，有AES解密的流程图，可以对应那个流程图来进行解密。下面介绍的是另一种等价的解密模式，流程图如下图所示。这种等价的解密模式使得解密过程各个变换的使用顺序同加密过程的顺序一致，只是用逆变换取代原来的变换。 
![deaes](https://img-blog.csdn.net/20170220094853620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
AES原理到这里就结束了，下面主要为AES的实现，对以上原理中的每一个小节进行实现讲解，讲解的时候会插入一些关键代码，完整的代码参见文章最后。文章最后提供两个完整的程序，一个能在linux下面编译运行，一个能在VC6.0下面编译通过。
# AES算法实现
## AES加密函数预览
aes加密函数中，首先进行密钥扩展，然后把128位长度的字符串读进一个4*4的整数数组中，这个数组就是状态矩阵。例如，pArray[0][0] = S0,pArray[1][0] = S1, pArray[0][1] = S4。这个读取过程是通过 convertToIntArray()函数来实现的。每个轮操作的函数都对pArray进行修改，也就是对状态矩阵进行混淆。在执行完10轮加密后，会把pArray转换回字符串，再存入明文p的字符数组中，所以，在加密完后，明文p的字符串中的字符就是加密后的字符了。这个转换过程是通过convertArrayToStr()函数来实现的。
```java
/**
 * 参数 p: 明文的字符串数组。
 * 参数 plen: 明文的长度。
 * 参数 key: 密钥的字符串数组。
 */
void aes(char *p, int plen, char *key){
    int keylen = strlen(key);
    if(plen == 0 || plen % 16 != 0) {
        printf("明文字符长度必须为16的倍数！\n");
        exit(0);
    }
    if(!checkKeyLen(keylen)) {
        printf("密钥字符长度错误！长度必须为16、24和32。当前长度为%d\n",keylen);
        exit(0);
    }
    extendKey(key);//扩展密钥
    int pArray[4][4];
    for(int k = 0; k < plen; k += 16) {
        convertToIntArray(p + k, pArray);
        addRoundKey(pArray, 0);//一开始的轮密钥加
        for(int i = 1; i < 10; i++){//前9轮
            subBytes(pArray);//字节代换
            shiftRows(pArray);//行移位
            mixColumns(pArray);//列混合
            addRoundKey(pArray, i);
        }
        //第10轮
        subBytes(pArray);//字节代换
        shiftRows(pArray);//行移位
        addRoundKey(pArray, 10);
        convertArrayToStr(pArray, p + k);
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
## 1.密钥扩展的实现
在开始加密前，必须先获得第一轮加密用到的密钥，故先实现密钥扩展 
下面是密钥扩展函数的实现，这个函数传入密钥key的字符串表示，然后从字符串中读取W[0]到W[3],函数getWordFromStr()用于实现此功能。读取后，就开始扩展密钥，当i是4的倍数的时候，就会调用T()函数来进行扩展，因为T函数的行为与加密的轮数有关，故要把加密的轮数 j 作为参数传进去。
```java
//密钥对应的扩展数组
static int w[44];
/**
 * 扩展密钥，结果是把w[44]中的每个元素初始化
 */
static void extendKey(char *key) {
    for(int i = 0; i < 4; i++)
        w[i] = getWordFromStr(key + i * 4); 
    for(int i = 4, j = 0; i < 44; i++) {
        if( i % 4 == 0) {
            w[i] = w[i - 4] ^ T(w[i - 1], j); 
            j++;//下一轮
        }else {
            w[i] = w[i - 4] ^ w[i - 1]; 
        }
    }   
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
下面是T()函数的代码实现，T()函数中接收两个参数，参数num为上面传进的W[i - 1]，round为加密的轮数。首先用一个numArray储存从32位的W[i-1]中取得4个字节。如果W[i-1]为0x12ABCDEF,那么numArray[0] = 0x12,numArray[1] = 0xAB。函数splitIntToArray()用于从32位整数中读取这四个字节，之所以这样做是因为整数数组比较容易操作。然后调用leftLoop4int()函数把numArray数组中的4个元素循环左移1位。然后执行字节代换，通过getNumFromSBox()函数来获取S盒中相应的值来替换numArray中的值。接着通过mergeArrayToInt()函数把字节代换后的numArray合并回32位的整数，在进行轮常量异或后返回。
```java
/**
 * 常量轮值表
 */
static const int Rcon[10] = { 0x01000000, 0x02000000,
    0x04000000, 0x08000000,
    0x10000000, 0x20000000,
    0x40000000, 0x80000000,
    0x1b000000, 0x36000000 };
/**
 * 密钥扩展中的T函数
 */
static int T(int num, int round) {
    int numArray[4];
    splitIntToArray(num, numArray);
    leftLoop4int(numArray, 1);//字循环
    //字节代换
    for(int i = 0; i < 4; i++)
        numArray[i] = getNumFromSBox(numArray[i]);
    int result = mergeArrayToInt(numArray);
    return result ^ Rcon[round];
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
## 2. 字节代换的实现
字节代换的代码很简单，就是把状态矩阵中的每个元素传进getNumFromSBox()函数中，然后取得前面8位中的高4位作为行值，低4位作为列值，然后返回S[row][col]，这里的S是储存S盒的数组。
```java
/**
 * 根据索引，从S盒中获得元素
 */
static int getNumFromSBox(int index) {
    int row = getLeft4Bit(index);
    int col = getRight4Bit(index);
    return S[row][col];
}
/**
 * 字节代换
 */
static void subBytes(int array[4][4]){
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            array[i][j] = getNumFromSBox(array[i][j]);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
## 3.行移位的实现
行移位的时候，首先把状态矩阵中第2，3，4行复制出来，然后对它们行进左移相应的位数，然后再复制回去状态矩阵array中。
```java
/**
 * 将数组中的元素循环左移step位
 */
static void leftLoop4int(int array[4], int step) {
    int temp[4];
    for(int i = 0; i < 4; i++)
        temp[i] = array[i];
    int index = step % 4 == 0 ? 0 : step % 4;
    for(int i = 0; i < 4; i++){
        array[i] = temp[index];
        index++;
        index = index % 4;
    }
}
/**
 * 行移位
 */
static void shiftRows(int array[4][4]) {
    int rowTwo[4], rowThree[4], rowFour[4];
    //复制状态矩阵的第2,3,4行
    for(int i = 0; i < 4; i++) {
        rowTwo[i] = array[1][i];
        rowThree[i] = array[2][i];
        rowFour[i] = array[3][i];
    }
    //循环左移相应的位数
    leftLoop4int(rowTwo, 1);
    leftLoop4int(rowThree, 2);
    leftLoop4int(rowFour, 3);
    //把左移后的行复制回状态矩阵中
    for(int i = 0; i < 4; i++) {
        array[1][i] = rowTwo[i];
        array[2][i] = rowThree[i];
        array[3][i] = rowFour[i];
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
## 4.列混合的实现
列混合函数中，先把状态矩阵初始状态复制一份到tempArray中，然后把tempArray与colM矩阵相乘，colM为存放要乘的常数矩阵的数组。其中的GFMul()函数定义了矩阵相乘时的乘法，加法则直接通过异或来实现。GFMul()通过调用乘以各个数对应的函数来实现乘法。例如，S1 * 2 刚通过调用GFMul2(S1)来实现。S1 * 3 刚通过GFMul3(S1)来实现。在这里，主要实现GFMul2()函数就行了，其它的都可以通过GFMul2()的组合来实现。举个例子吧，为计算下面这条等式，需要像下面这样调用函数 
![ex](https://img-blog.csdn.net/20170220173019366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgyMDUxNTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
s = GFMul3(0xC9) ^ 0x7A ^ 0x63 ^ GFMul2(0xB0)
```java
/**
 * 列混合要用到的矩阵
 */
static const int colM[4][4] = { 2, 3, 1, 1,
    1, 2, 3, 1,
    1, 1, 2, 3,
    3, 1, 1, 2 };
static int GFMul2(int s) {
    int result = s << 1;
    int a7 = result & 0x00000100;
    if(a7 != 0) {
        result = result & 0x000000ff;
        result = result ^ 0x1b;
    }
    return result;
}
static int GFMul3(int s) {
    return GFMul2(s) ^ s;
}
/**
 * GF上的二元运算
 */
static int GFMul(int n, int s) {
    int result;
    if(n == 1)
        result = s;
    else if(n == 2)
        result = GFMul2(s);
    else if(n == 3)
        result = GFMul3(s);
    else if(n == 0x9)
        result = GFMul9(s);
    else if(n == 0xb)//11
        result = GFMul11(s);
    else if(n == 0xd)//13
        result = GFMul13(s);
    else if(n == 0xe)//14
        result = GFMul14(s);
    return result;
}
/**
 * 列混合
 */
static void mixColumns(int array[4][4]) {
    int tempArray[4][4];
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            tempArray[i][j] = array[i][j];
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++){
            array[i][j] = GFMul(colM[i][0],tempArray[0][j]) ^ GFMul(colM[i][1],tempArray[1][j])
                ^ GFMul(colM[i][2],tempArray[2][j]) ^ GFMul(colM[i][3], tempArray[3][j]);
        }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
## 5.轮密钥加的实现
轮密钥加的实现很简单，就是根据传入的轮数来把状态矩阵与相应的W[i]异或。
```java
/**
 * 轮密钥加
 */
static void addRoundKey(int array[4][4], int round) {
    int warray[4];
    for(int i = 0; i < 4; i++) {
        splitIntToArray(w[ round * 4 + i], warray);
        for(int j = 0; j < 4; j++) {
            array[j][i] = array[j][i] ^ warray[j];
        }
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
## AES解密函数
AES的解密函数和加密函数有点不同，可以参考上面的等价解密流程图来理解，解密函数中调用的是各轮操作的逆函数。逆函数在这里就不详细讲解了，可以参考最后的完整代码。
```java
/**
 * 参数 c: 密文的字符串数组。
 * 参数 clen: 密文的长度。
 * 参数 key: 密钥的字符串数组。
 */
void deAes(char *c, int clen, char *key) {
    int keylen = strlen(key);
    if(clen == 0 || clen % 16 != 0) {
        printf("密文字符长度必须为16的倍数！现在的长度为%d\n",clen);
        exit(0);
    }
    if(!checkKeyLen(keylen)) {
        printf("密钥字符长度错误！长度必须为16、24和32。当前长度为%d\n",keylen);
        exit(0);
    }
    extendKey(key);//扩展密钥
    int cArray[4][4];
    for(int k = 0; k < clen; k += 16) {
        convertToIntArray(c + k, cArray);
        addRoundKey(cArray, 10);
        int wArray[4][4];
        for(int i = 9; i >= 1; i--) {
            deSubBytes(cArray);
            deShiftRows(cArray);
            deMixColumns(cArray);
            getArrayFrom4W(i, wArray);
            deMixColumns(wArray);
            addRoundTowArray(cArray, wArray);
        }
        deSubBytes(cArray);
        deShiftRows(cArray);
        addRoundKey(cArray, 0);
        convertArrayToStr(cArray, c + k);
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
# 完整的程序代码
## Linux版本
### aes.h
```java
#ifndef AES_H
#define AES_H
/**
 * 参数 p: 明文的字符串数组。
 * 参数 plen: 明文的长度,长度必须为16的倍数。
 * 参数 key: 密钥的字符串数组。
 */
void aes(char *p, int plen, char *key);
/**
 * 参数 c: 密文的字符串数组。
 * 参数 clen: 密文的长度,长度必须为16的倍数。
 * 参数 key: 密钥的字符串数组。
 */
void deAes(char *c, int clen, char *key);
#endif
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
### aes.c
```java
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"
/**
 * S盒
 */
static const int S[16][16] = { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
/**
 * 逆S盒
 */
static const int S2[16][16] = { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
/**
 * 获取整形数据的低8位的左4个位
 */
static int getLeft4Bit(int num) {
    int left = num & 0x000000f0;
    return left >> 4;
}
/**
 * 获取整形数据的低8位的右4个位
 */
static int getRight4Bit(int num) {
    return num & 0x0000000f;
}
/**
 * 根据索引，从S盒中获得元素
 */
static int getNumFromSBox(int index) {
    int row = getLeft4Bit(index);
    int col = getRight4Bit(index);
    return S[row][col];
}
/**
 * 把一个字符转变成整型
 */
static int getIntFromChar(char c) {
    int result = (int) c;
    return result & 0x000000ff;
}
/**
 * 把16个字符转变成4X4的数组，
 * 该矩阵中字节的排列顺序为从上到下，
 * 从左到右依次排列。
 */
static void convertToIntArray(char *str, int pa[4][4]) {
    int k = 0;
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++) {
            pa[j][i] = getIntFromChar(str[k]);
            k++;
        }
}
/**
 * 打印4X4的数组
 */
static void printArray(int a[4][4]) {
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++)
            printf("a[%d][%d] = 0x%x ", i, j, a[i][j]);
        printf("\n");
    }
    printf("\n");
}
/**
 * 打印字符串的ASSCI，
 * 以十六进制显示。
 */
static void printASSCI(char *str, int len) {
    for(int i = 0; i < len; i++)
        printf("0x%x ", getIntFromChar(str[i]));
    printf("\n");
}
/**
 * 把连续的4个字符合并成一个4字节的整型
 */
static int getWordFromStr(char *str) {
    int one = getIntFromChar(str[0]);
    one = one << 24;
    int two = getIntFromChar(str[1]);
    two = two << 16;
    int three = getIntFromChar(str[2]);
    three = three << 8;
    int four = getIntFromChar(str[3]);
    return one | two | three | four;
}
/**
 * 把一个4字节的数的第一、二、三、四个字节取出，
 * 入进一个4个元素的整型数组里面。
 */
static void splitIntToArray(int num, int array[4]) {
    int one = num >> 24;
    array[0] = one & 0x000000ff;
    int two = num >> 16;
    array[1] = two & 0x000000ff;
    int three = num >> 8;
    array[2] = three & 0x000000ff;
    array[3] = num & 0x000000ff;
}
/**
 * 将数组中的元素循环左移step位
 */
static void leftLoop4int(int array[4], int step) {
    int temp[4];
    for(int i = 0; i < 4; i++)
        temp[i] = array[i];
    int index = step % 4 == 0 ? 0 : step % 4;
    for(int i = 0; i < 4; i++){
        array[i] = temp[index];
        index++;
        index = index % 4;
    }
}
/**
 * 把数组中的第一、二、三和四元素分别作为
 * 4字节整型的第一、二、三和四字节，合并成一个4字节整型
 */
static int mergeArrayToInt(int array[4]) {
    int one = array[0] << 24;
    int two = array[1] << 16;
    int three = array[2] << 8;
    int four = array[3];
    return one | two | three | four;
}
/**
 * 常量轮值表
 */
static const int Rcon[10] = { 0x01000000, 0x02000000,
    0x04000000, 0x08000000,
    0x10000000, 0x20000000,
    0x40000000, 0x80000000,
    0x1b000000, 0x36000000 };
/**
 * 密钥扩展中的T函数
 */
static int T(int num, int round) {
    int numArray[4];
    splitIntToArray(num, numArray);
    leftLoop4int(numArray, 1);//字循环
    //字节代换
    for(int i = 0; i < 4; i++)
        numArray[i] = getNumFromSBox(numArray[i]);
    int result = mergeArrayToInt(numArray);
    return result ^ Rcon[round];
}
//密钥对应的扩展数组
static int w[44];
/**
 * 扩展密钥，结果是把w[44]中的每个元素初始化
 */
static void extendKey(char *key) {
    for(int i = 0; i < 4; i++)
        w[i] = getWordFromStr(key + i * 4);
    for(int i = 4, j = 0; i < 44; i++) {
        if( i % 4 == 0) {
            w[i] = w[i - 4] ^ T(w[i - 1], j);
            j++;//下一轮
        }else {
            w[i] = w[i - 4] ^ w[i - 1];
        }
    }
}
/**
 * 轮密钥加
 */
static void addRoundKey(int array[4][4], int round) {
    int warray[4];
    for(int i = 0; i < 4; i++) {
        splitIntToArray(w[ round * 4 + i], warray);
        for(int j = 0; j < 4; j++) {
            array[j][i] = array[j][i] ^ warray[j];
        }
    }
}
/**
 * 字节代换
 */
static void subBytes(int array[4][4]){
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            array[i][j] = getNumFromSBox(array[i][j]);
}
/**
 * 行移位
 */
static void shiftRows(int array[4][4]) {
    int rowTwo[4], rowThree[4], rowFour[4];
    //复制状态矩阵的第2,3,4行
    for(int i = 0; i < 4; i++) {
        rowTwo[i] = array[1][i];
        rowThree[i] = array[2][i];
        rowFour[i] = array[3][i];
    }
    //循环左移相应的位数
    leftLoop4int(rowTwo, 1);
    leftLoop4int(rowThree, 2);
    leftLoop4int(rowFour, 3);
    //把左移后的行复制回状态矩阵中
    for(int i = 0; i < 4; i++) {
        array[1][i] = rowTwo[i];
        array[2][i] = rowThree[i];
        array[3][i] = rowFour[i];
    }
}
/**
 * 列混合要用到的矩阵
 */
static const int colM[4][4] = { 2, 3, 1, 1,
    1, 2, 3, 1,
    1, 1, 2, 3,
    3, 1, 1, 2 };
static int GFMul2(int s) {
    int result = s << 1;
    int a7 = result & 0x00000100;
    if(a7 != 0) {
        result = result & 0x000000ff;
        result = result ^ 0x1b;
    }
    return result;
}
static int GFMul3(int s) {
    return GFMul2(s) ^ s;
}
static int GFMul4(int s) {
    return GFMul2(GFMul2(s));
}
static int GFMul8(int s) {
    return GFMul2(GFMul4(s));
}
static int GFMul9(int s) {
    return GFMul8(s) ^ s;
}
static int GFMul11(int s) {
    return GFMul9(s) ^ GFMul2(s);
}
static int GFMul12(int s) {
    return GFMul8(s) ^ GFMul4(s);
}
static int GFMul13(int s) {
    return GFMul12(s) ^ s;
}
static int GFMul14(int s) {
    return GFMul12(s) ^ GFMul2(s);
}
/**
 * GF上的二元运算
 */
static int GFMul(int n, int s) {
    int result;
    if(n == 1)
        result = s;
    else if(n == 2)
        result = GFMul2(s);
    else if(n == 3)
        result = GFMul3(s);
    else if(n == 0x9)
        result = GFMul9(s);
    else if(n == 0xb)//11
        result = GFMul11(s);
    else if(n == 0xd)//13
        result = GFMul13(s);
    else if(n == 0xe)//14
        result = GFMul14(s);
    return result;
}
/**
 * 列混合
 */
static void mixColumns(int array[4][4]) {
    int tempArray[4][4];
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            tempArray[i][j] = array[i][j];
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++){
            array[i][j] = GFMul(colM[i][0],tempArray[0][j]) ^ GFMul(colM[i][1],tempArray[1][j]) 
                ^ GFMul(colM[i][2],tempArray[2][j]) ^ GFMul(colM[i][3], tempArray[3][j]);
        }
}
/**
 * 把4X4数组转回字符串
 */
static void convertArrayToStr(int array[4][4], char *str) {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            *str++ = (char)array[j][i]; 
}
/**
 * 检查密钥长度
 */
static int checkKeyLen(int len) {
    if(len == 16)
        return 1;
    else
        return 0;
}
/**
 * 参数 p: 明文的字符串数组。
 * 参数 plen: 明文的长度。
 * 参数 key: 密钥的字符串数组。
 */
void aes(char *p, int plen, char *key){
    int keylen = strlen(key);
    if(plen == 0 || plen % 16 != 0) {
        printf("明文字符长度必须为16的倍数！\n");
        exit(0);
    }
    if(!checkKeyLen(keylen)) {
        printf("密钥字符长度错误！长度必须为16、24和32。当前长度为%d\n",keylen);
        exit(0);
    }
    extendKey(key);//扩展密钥
    int pArray[4][4];
    for(int k = 0; k < plen; k += 16) { 
        convertToIntArray(p + k, pArray);
        addRoundKey(pArray, 0);//一开始的轮密钥加
        for(int i = 1; i < 10; i++){//前9轮
            subBytes(pArray);//字节代换
            shiftRows(pArray);//行移位
            mixColumns(pArray);//列混合
            addRoundKey(pArray, i);
        }
        //第10轮
        subBytes(pArray);//字节代换
        shiftRows(pArray);//行移位
        addRoundKey(pArray, 10);
        convertArrayToStr(pArray, p + k);
    }
}
/**
 * 根据索引从逆S盒中获取值
 */
static int getNumFromS1Box(int index) {
    int row = getLeft4Bit(index);
    int col = getRight4Bit(index);
    return S2[row][col];
}
/**
 * 逆字节变换
 */
static void deSubBytes(int array[4][4]) {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            array[i][j] = getNumFromS1Box(array[i][j]);
}
/**
 * 把4个元素的数组循环右移step位
 */
static void rightLoop4int(int array[4], int step) {
    int temp[4];
    for(int i = 0; i < 4; i++)
        temp[i] = array[i];
    int index = step % 4 == 0 ? 0 : step % 4;
    index = 3 - index;
    for(int i = 3; i >= 0; i--) {
        array[i] = temp[index];
        index--;
        index = index == -1 ? 3 : index;
    }
}
/**
 * 逆行移位
 */
static void deShiftRows(int array[4][4]) {
    int rowTwo[4], rowThree[4], rowFour[4];
    for(int i = 0; i < 4; i++) {
        rowTwo[i] = array[1][i];
        rowThree[i] = array[2][i];
        rowFour[i] = array[3][i];
    }
    rightLoop4int(rowTwo, 1);
    rightLoop4int(rowThree, 2);
    rightLoop4int(rowFour, 3);
    for(int i = 0; i < 4; i++) {
        array[1][i] = rowTwo[i];
        array[2][i] = rowThree[i];
        array[3][i] = rowFour[i];
    }
}
/**
 * 逆列混合用到的矩阵
 */
static const int deColM[4][4] = { 0xe, 0xb, 0xd, 0x9,
    0x9, 0xe, 0xb, 0xd,
    0xd, 0x9, 0xe, 0xb,
    0xb, 0xd, 0x9, 0xe };
/**
 * 逆列混合
 */
static void deMixColumns(int array[4][4]) {
    int tempArray[4][4];
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            tempArray[i][j] = array[i][j];
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++){
            array[i][j] = GFMul(deColM[i][0],tempArray[0][j]) ^ GFMul(deColM[i][1],tempArray[1][j]) 
                ^ GFMul(deColM[i][2],tempArray[2][j]) ^ GFMul(deColM[i][3], tempArray[3][j]);
        }
}
/**
 * 把两个4X4数组进行异或
 */
static void addRoundTowArray(int aArray[4][4],int bArray[4][4]) {
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            aArray[i][j] = aArray[i][j] ^ bArray[i][j];
}
/**
 * 从4个32位的密钥字中获得4X4数组，
 * 用于进行逆列混合
 */
static void getArrayFrom4W(int i, int array[4][4]) {
    int index = i * 4;
    int colOne[4], colTwo[4], colThree[4], colFour[4];
    splitIntToArray(w[index], colOne);
    splitIntToArray(w[index + 1], colTwo);
    splitIntToArray(w[index + 2], colThree);
    splitIntToArray(w[index + 3], colFour);
    for(int i = 0; i < 4; i++) {
        array[i][0] = colOne[i];
        array[i][1] = colTwo[i];
        array[i][2] = colThree[i];
        array[i][3] = colFour[i];
    }
}
/**
 * 参数 c: 密文的字符串数组。
 * 参数 clen: 密文的长度。
 * 参数 key: 密钥的字符串数组。
 */
void deAes(char *c, int clen, char *key) {
    int keylen = strlen(key);
    if(clen == 0 || clen % 16 != 0) {
        printf("密文字符长度必须为16的倍数！现在的长度为%d\n",clen);
        exit(0);
    }
    if(!checkKeyLen(keylen)) {
        printf("密钥字符长度错误！长度必须为16、24和32。当前长度为%d\n",keylen);
        exit(0);
    }
    extendKey(key);//扩展密钥
    int cArray[4][4];
    for(int k = 0; k < clen; k += 16) {
        convertToIntArray(c + k, cArray);
        addRoundKey(cArray, 10);
        int wArray[4][4];
        for(int i = 9; i >= 1; i--) {
            deSubBytes(cArray);
            deShiftRows(cArray);
            deMixColumns(cArray);
            getArrayFrom4W(i, wArray);
            deMixColumns(wArray);
            addRoundTowArray(cArray, wArray);
        }
        deSubBytes(cArray);
        deShiftRows(cArray);
        addRoundKey(cArray, 0);
        convertArrayToStr(cArray, c + k);
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
- 187
- 188
- 189
- 190
- 191
- 192
- 193
- 194
- 195
- 196
- 197
- 198
- 199
- 200
- 201
- 202
- 203
- 204
- 205
- 206
- 207
- 208
- 209
- 210
- 211
- 212
- 213
- 214
- 215
- 216
- 217
- 218
- 219
- 220
- 221
- 222
- 223
- 224
- 225
- 226
- 227
- 228
- 229
- 230
- 231
- 232
- 233
- 234
- 235
- 236
- 237
- 238
- 239
- 240
- 241
- 242
- 243
- 244
- 245
- 246
- 247
- 248
- 249
- 250
- 251
- 252
- 253
- 254
- 255
- 256
- 257
- 258
- 259
- 260
- 261
- 262
- 263
- 264
- 265
- 266
- 267
- 268
- 269
- 270
- 271
- 272
- 273
- 274
- 275
- 276
- 277
- 278
- 279
- 280
- 281
- 282
- 283
- 284
- 285
- 286
- 287
- 288
- 289
- 290
- 291
- 292
- 293
- 294
- 295
- 296
- 297
- 298
- 299
- 300
- 301
- 302
- 303
- 304
- 305
- 306
- 307
- 308
- 309
- 310
- 311
- 312
- 313
- 314
- 315
- 316
- 317
- 318
- 319
- 320
- 321
- 322
- 323
- 324
- 325
- 326
- 327
- 328
- 329
- 330
- 331
- 332
- 333
- 334
- 335
- 336
- 337
- 338
- 339
- 340
- 341
- 342
- 343
- 344
- 345
- 346
- 347
- 348
- 349
- 350
- 351
- 352
- 353
- 354
- 355
- 356
- 357
- 358
- 359
- 360
- 361
- 362
- 363
- 364
- 365
- 366
- 367
- 368
- 369
- 370
- 371
- 372
- 373
- 374
- 375
- 376
- 377
- 378
- 379
- 380
- 381
- 382
- 383
- 384
- 385
- 386
- 387
- 388
- 389
- 390
- 391
- 392
- 393
- 394
- 395
- 396
- 397
- 398
- 399
- 400
- 401
- 402
- 403
- 404
- 405
- 406
- 407
- 408
- 409
- 410
- 411
- 412
- 413
- 414
- 415
- 416
- 417
- 418
- 419
- 420
- 421
- 422
- 423
- 424
- 425
- 426
- 427
- 428
- 429
- 430
- 431
- 432
- 433
- 434
- 435
- 436
- 437
- 438
- 439
- 440
- 441
- 442
- 443
- 444
- 445
- 446
- 447
- 448
- 449
- 450
- 451
- 452
- 453
- 454
- 455
- 456
- 457
- 458
- 459
- 460
- 461
- 462
- 463
- 464
- 465
- 466
- 467
- 468
- 469
- 470
- 471
- 472
- 473
- 474
- 475
- 476
- 477
- 478
- 479
- 480
- 481
- 482
- 483
- 484
- 485
- 486
- 487
- 488
- 489
- 490
- 491
- 492
- 493
- 494
- 495
- 496
- 497
- 498
- 499
- 500
- 501
- 502
- 503
- 504
- 505
- 506
- 507
- 508
- 509
- 510
- 511
- 512
- 513
- 514
- 515
- 516
- 517
- 518
- 519
- 520
- 521
- 522
- 523
- 524
- 525
- 526
- 527
- 528
- 529
- 530
- 531
- 532
- 533
- 534
- 535
- 536
- 537
- 538
- 539
- 540
- 541
- 542
- 543
- 544
- 545
- 546
- 547
- 548
- 549
- 550
- 551
- 552
- 553
- 554
- 555
- 556
- 557
- 558
- 559
- 560
- 561
- 562
- 563
- 564
- 565
- 566
- 567
- 568
- 569
- 570
- 571
- 572
- 573
- 574
- 575
### main.c
```cpp
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "aes.h"
#define MAXLEN 1024
void getString(char *str, int len){
    int slen = read(0, str, len);
    for(int i = 0; i < slen; i++,str++){
        if(*str == '\n'){
            *str = '\0';
            break;
        }
    }
}
void printASCCI(char *str, int len) {
    int c;
    for(int i = 0; i < len; i++) {
        c = (int)*str++;
        c = c & 0x000000ff;
        printf("0x%x ", c);
    }
    printf("\n");
}
/**
 * 从标准输入中读取用户输入的字符串
 */
void readPlainText(char *str, int *len) {
    int plen;
    while(1) {
        getString(str, MAXLEN);
        plen = strlen(str);
        if(plen != 0 && plen % 16 == 0) {
            printf("你输入的明文为：%s\n", str);
            break;
        }else{
            printf("明文字符长度必须为16的倍数,现在的长度为%d\n", plen);
        }
    }
    *len = plen;
}
/**
 * 把字符串写进文件
 */
void writeStrToFile(char *str, int len, char *fileName) {
    FILE *fp;
    fp = fopen(fileName, "wb");
    for(int i = 0; i < len; i++)
        putc(str[i], fp);
    fclose(fp);
}
void aesStrToFile(char *key) {
    char p[MAXLEN];
    int plen;
    printf("请输入你的明文，明文字符长度必须为16的倍数\n");
    readPlainText(p,&plen);
    printf("进行AES加密..................\n");
    aes(p, plen, key);//AES加密
    printf("加密完后的明文的ASCCI为：\n");
    printASCCI(p, plen);
    char fileName[64];
    printf("请输入你想要写进的文件名，比如'test.txt':\n");
    if(scanf("%s", fileName) == 1) {    
        writeStrToFile(p, plen, fileName);
        printf("已经将密文写进%s中了,可以在运行该程序的当前目录中找到它。\n", fileName);
    }
}
/**
 * 从文件中读取字符串
 */
int readStrFromFile(char *fileName, char *str) {
    FILE *fp = fopen(fileName, "rb");
    if(fp == NULL) {
        printf("打开文件出错，请确认文件存在当前目录下！\n");
        exit(0);
    }
    int i;
    for(i = 0; i < MAXLEN && (str[i] = getc(fp)) != EOF; i++);
    if(i >= MAXLEN) {
        printf("解密文件过大！\n");
        exit(0);
    }
    str[i] = '\0';
    fclose(fp);
    return i;
}
void deAesFile(char *key) {
    char fileName[64];
    char c[MAXLEN];//密文字符串
    printf("请输入要解密的文件名，该文件必须和本程序在同一个目录\n");
    if(scanf("%s", fileName) == 1) {
        int clen = readStrFromFile(fileName, c);
        printf("开始解密.........\n");
        deAes(c, clen, key);
        printf("解密后的明文ASCII为：\n");
        printASCCI(c, clen);
        printf("明文为：%s\n", c);
        writeStrToFile(c,clen,fileName);
        printf("现在可以打开%s来查看解密后的密文了！\n",fileName);
    }
}
void aesFile(char *key) {
    char fileName[64];
    char fileP[MAXLEN];
    printf("请输入要加密的文件名，该文件必须和本程序在同一个目录\n");
    if(scanf("%s", fileName) == 1) {
        readStrFromFile(fileName, fileP);
        int plen = strlen(fileP);
        printf("开始加密.........\n");
        printf("加密前文件中字符的ASCII为：\n");
        printASCCI(fileP, plen);
        aes(fileP, plen, key);//开始加密
        printf("加密后的密文ASCII为：\n");
        printASCCI(fileP, plen);
        writeStrToFile(fileP,plen,fileName);
        printf("已经将加密后的密文写进%s中了\n",fileName);
    }
}
int main(int argc, char const *argv[]) {
    char key[17];
    printf("请输入16个字符的密钥：\n");
    int klen;
    while(1){
        getString(key,17);
        klen = strlen(key);
        if(klen != 16){
            printf("请输入16个字符的密钥,当前密钥的长度为%d\n",klen);
        }else{
            printf("你输入的密钥为：%s\n",key);
            break;
        }
    }
    printf("输入's'表示要加密输入的字符串,并将加密后的内容写入到文件\n");
    printf("请输入要功能选项并按回车，输入'f'表示要加密文件\n");
    printf("输入'p'表示要解密文件\n");
    char c;
    if(scanf("%s",&c) == 1) {
        if(c == 's')
            aesStrToFile(key);//用AES加密字符串，并将字符串写进文件中
        else if(c == 'p')
            deAesFile(key);//把文件中的密文解密，并写回文件中
        else if(c == 'f')//用AES加密文件
            aesFile(key);
    }
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
通过下面的gcc命令来编译运行：
`gcc -o aes aes.c main.c`- 1
## VC6.0版本
由于VC6.0的编译器比较坑，要先声明，后使用变量，故要对代码进行相应的修改。
### aes.h
```java
#ifndef MY_AES_H
#define MY_AES_H
/**
 * 参数 p: 明文的字符串数组。
 * 参数 plen: 明文的长度,长度必须为16的倍数。
 * 参数 key: 密钥的字符串数组。
 */
void aes(char *p, int plen, char *key);
/**
 * 参数 c: 密文的字符串数组。
 * 参数 clen: 密文的长度,长度必须为16的倍数。
 * 参数 key: 密钥的字符串数组。
 */
void deAes(char *c, int clen, char *key);
#endif
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
### aes.cpp
```java
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"
/**
 * S盒
 */
static const int S[16][16] = { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
/**
 * 逆S盒
 */
static const int S2[16][16] = { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
/**
 * 获取整形数据的低8位的左4个位
 */
static int getLeft4Bit(int num) {
    int left = num & 0x000000f0;
    return left >> 4;
}
/**
 * 获取整形数据的低8位的右4个位
 */
static int getRight4Bit(int num) {
    return num & 0x0000000f;
}
/**
 * 根据索引，从S盒中获得元素
 */
static int getNumFromSBox(int index) {
    int row = getLeft4Bit(index);
    int col = getRight4Bit(index);
    return S[row][col];
}
/**
 * 把一个字符转变成整型
 */
static int getIntFromChar(char c) {
    int result = (int) c;
    return result & 0x000000ff;
}
/**
 * 把16个字符转变成4X4的数组，
 * 该矩阵中字节的排列顺序为从上到下，
 * 从左到右依次排列。
 */
static void convertToIntArray(char *str, int pa[4][4]) {
    int k = 0;
    int i,j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++) {
            pa[j][i] = getIntFromChar(str[k]);
            k++;
        }
}
/**
 * 打印4X4的数组
 */
static void printArray(int a[4][4]) {
    int i,j;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++)
            printf("a[%d][%d] = 0x%x ", i, j, a[i][j]);
        printf("\n");
    }
    printf("\n");
}
/**
 * 打印字符串的ASSCI，
 * 以十六进制显示。
 */
static void printASSCI(char *str, int len) {
    int i;
    for(i = 0; i < len; i++)
        printf("0x%x ", getIntFromChar(str[i]));
    printf("\n");
}
/**
 * 把连续的4个字符合并成一个4字节的整型
 */
static int getWordFromStr(char *str) {
    int one, two, three, four;
    one = getIntFromChar(str[0]);
    one = one << 24;
    two = getIntFromChar(str[1]);
    two = two << 16;
    three = getIntFromChar(str[2]);
    three = three << 8;
    four = getIntFromChar(str[3]);
    return one | two | three | four;
}
/**
 * 把一个4字节的数的第一、二、三、四个字节取出，
 * 入进一个4个元素的整型数组里面。
 */
static void splitIntToArray(int num, int array[4]) {
    int one, two, three;
    one = num >> 24;
    array[0] = one & 0x000000ff;
    two = num >> 16;
    array[1] = two & 0x000000ff;
    three = num >> 8;
    array[2] = three & 0x000000ff;
    array[3] = num & 0x000000ff;
}
/**
 * 将数组中的元素循环左移step位
 */
static void leftLoop4int(int array[4], int step) {
    int temp[4];
    int i;
    int index;
    for(i = 0; i < 4; i++)
        temp[i] = array[i];
    index = step % 4 == 0 ? 0 : step % 4;
    for(i = 0; i < 4; i++){
        array[i] = temp[index];
        index++;
        index = index % 4;
    }
}
/**
 * 把数组中的第一、二、三和四元素分别作为
 * 4字节整型的第一、二、三和四字节，合并成一个4字节整型
 */
static int mergeArrayToInt(int array[4]) {
    int one = array[0] << 24;
    int two = array[1] << 16;
    int three = array[2] << 8;
    int four = array[3];
    return one | two | three | four;
}
/**
 * 常量轮值表
 */
static const int Rcon[10] = { 0x01000000, 0x02000000,
    0x04000000, 0x08000000,
    0x10000000, 0x20000000,
    0x40000000, 0x80000000,
    0x1b000000, 0x36000000 };
/**
 * 密钥扩展中的T函数
 */
static int T(int num, int round) {
    int numArray[4];
    int i;
    int result;
    splitIntToArray(num, numArray);
    leftLoop4int(numArray, 1);//字循环
    //字节代换
    for(i = 0; i < 4; i++)
        numArray[i] = getNumFromSBox(numArray[i]);
    result = mergeArrayToInt(numArray);
    return result ^ Rcon[round];
}
//密钥对应的扩展数组
static int w[44];
/**
 * 打印W数组
 */
static void printW() {
    int i, j;
    for(i = 0, j = 1; i < 44; i++,j++){
        printf("w[%d] = 0x%x ", i, w[i]);
        if(j % 4 == 0)
            printf("\n");
    }
    printf("\n");
}
/**
 * 扩展密钥，结果是把w[44]中的每个元素初始化
 */
static void extendKey(char *key) {
    int i,j;
    for(i = 0; i < 4; i++)
        w[i] = getWordFromStr(key + i * 4);
    for(i = 4, j = 0; i < 44; i++) {
        if( i % 4 == 0) {
            w[i] = w[i - 4] ^ T(w[i - 1], j);
            j++;//下一轮
        }else {
            w[i] = w[i - 4] ^ w[i - 1];
        }
    }
}
/**
 * 轮密钥加
 */
static void addRoundKey(int array[4][4], int round) {
    int warray[4];
    int i,j;
    for(i = 0; i < 4; i++) {
        splitIntToArray(w[ round * 4 + i], warray);
        for(j = 0; j < 4; j++) {
            array[j][i] = array[j][i] ^ warray[j];
        }
    }
}
/**
 * 字节代换
 */
static void subBytes(int array[4][4]){
    int i,j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            array[i][j] = getNumFromSBox(array[i][j]);
}
/**
 * 行移位
 */
static void shiftRows(int array[4][4]) {
    int rowTwo[4], rowThree[4], rowFour[4];
    int i;
    for(i = 0; i < 4; i++) {
        rowTwo[i] = array[1][i];
        rowThree[i] = array[2][i];
        rowFour[i] = array[3][i];
    }
    leftLoop4int(rowTwo, 1);
    leftLoop4int(rowThree, 2);
    leftLoop4int(rowFour, 3);
    for(i = 0; i < 4; i++) {
        array[1][i] = rowTwo[i];
        array[2][i] = rowThree[i];
        array[3][i] = rowFour[i];
    }
}
/**
 * 列混合要用到的矩阵
 */
static const int colM[4][4] = { 2, 3, 1, 1,
    1, 2, 3, 1,
    1, 1, 2, 3,
    3, 1, 1, 2 };
static int GFMul2(int s) {
    int result = s << 1;
    int a7 = result & 0x00000100;
    if(a7 != 0) {
        result = result & 0x000000ff;
        result = result ^ 0x1b;
    }
    return result;
}
static int GFMul3(int s) {
    return GFMul2(s) ^ s;
}
static int GFMul4(int s) {
    return GFMul2(GFMul2(s));
}
static int GFMul8(int s) {
    return GFMul2(GFMul4(s));
}
static int GFMul9(int s) {
    return GFMul8(s) ^ s;
}
static int GFMul11(int s) {
    return GFMul9(s) ^ GFMul2(s);
}
static int GFMul12(int s) {
    return GFMul8(s) ^ GFMul4(s);
}
static int GFMul13(int s) {
    return GFMul12(s) ^ s;
}
static int GFMul14(int s) {
    return GFMul12(s) ^ GFMul2(s);
}
/**
 * GF上的二元运算
 */
static int GFMul(int n, int s) {
    int result;
    if(n == 1)
        result = s;
    else if(n == 2)
        result = GFMul2(s);
    else if(n == 3)
        result = GFMul3(s);
    else if(n == 0x9)
        result = GFMul9(s);
    else if(n == 0xb)//11
        result = GFMul11(s);
    else if(n == 0xd)//13
        result = GFMul13(s);
    else if(n == 0xe)//14
        result = GFMul14(s);
    return result;
}
/**
 * 列混合
 */
static void mixColumns(int array[4][4]) {
    int tempArray[4][4];
    int i,j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            tempArray[i][j] = array[i][j];
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++){
            array[i][j] = GFMul(colM[i][0],tempArray[0][j]) ^ GFMul(colM[i][1],tempArray[1][j])
                ^ GFMul(colM[i][2],tempArray[2][j]) ^ GFMul(colM[i][3], tempArray[3][j]);
        }
}
/**
 * 把4X4数组转回字符串
 */
static void convertArrayToStr(int array[4][4], char *str) {
    int i,j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            *str++ = (char)array[j][i];
}
/**
 * 检查密钥长度
 */
static int checkKeyLen(int len) {
    if(len == 16)
        return 1;
    else
        return 0;
}
/**
 * 参数 p: 明文的字符串数组。
 * 参数 plen: 明文的长度。
 * 参数 key: 密钥的字符串数组。
 */
void aes(char *p, int plen, char *key){
    int keylen = strlen(key);
    int pArray[4][4];
    int k,i;
    if(plen == 0 || plen % 16 != 0) {
        printf("明文字符长度必须为16的倍数！\n");
        exit(0);
    }
    if(!checkKeyLen(keylen)) {
        printf("密钥字符长度错误！长度必须为16。当前长度为%d\n",keylen);
        exit(0);
    }
    extendKey(key);//扩展密钥
    for(k = 0; k < plen; k += 16) {
        convertToIntArray(p + k, pArray);
        addRoundKey(pArray, 0);//一开始的轮密钥加
        for(i = 1; i < 10; i++){
            subBytes(pArray);//字节代换
            shiftRows(pArray);//行移位
            mixColumns(pArray);//列混合
            addRoundKey(pArray, i);
        }
        subBytes(pArray);//字节代换
        shiftRows(pArray);//行移位
        addRoundKey(pArray, 10);
        convertArrayToStr(pArray, p + k);
    }
}
/**
 * 根据索引从逆S盒中获取值
 */
static int getNumFromS1Box(int index) {
    int row = getLeft4Bit(index);
    int col = getRight4Bit(index);
    return S2[row][col];
}
/**
 * 逆字节变换
 */
static void deSubBytes(int array[4][4]) {
    int i,j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            array[i][j] = getNumFromS1Box(array[i][j]);
}
/**
 * 把4个元素的数组循环右移step位
 */
static void rightLoop4int(int array[4], int step) {
    int temp[4];
    int i;
    int index;
    for(i = 0; i < 4; i++)
        temp[i] = array[i];
    index = step % 4 == 0 ? 0 : step % 4;
    index = 3 - index;
    for(i = 3; i >= 0; i--) {
        array[i] = temp[index];
        index--;
        index = index == -1 ? 3 : index;
    }
}
/**
 * 逆行移位
 */
static void deShiftRows(int array[4][4]) {
    int rowTwo[4], rowThree[4], rowFour[4];
    int i;
    for(i = 0; i < 4; i++) {
        rowTwo[i] = array[1][i];
        rowThree[i] = array[2][i];
        rowFour[i] = array[3][i];
    }
    rightLoop4int(rowTwo, 1);
    rightLoop4int(rowThree, 2);
    rightLoop4int(rowFour, 3);
    for(i = 0; i < 4; i++) {
        array[1][i] = rowTwo[i];
        array[2][i] = rowThree[i];
        array[3][i] = rowFour[i];
    }
}
/**
 * 逆列混合用到的矩阵
 */
static const int deColM[4][4] = { 0xe, 0xb, 0xd, 0x9,
    0x9, 0xe, 0xb, 0xd,
    0xd, 0x9, 0xe, 0xb,
    0xb, 0xd, 0x9, 0xe };
/**
 * 逆列混合
 */
static void deMixColumns(int array[4][4]) {
    int tempArray[4][4];
    int i,j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            tempArray[i][j] = array[i][j];
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++){
            array[i][j] = GFMul(deColM[i][0],tempArray[0][j]) ^ GFMul(deColM[i][1],tempArray[1][j])
                ^ GFMul(deColM[i][2],tempArray[2][j]) ^ GFMul(deColM[i][3], tempArray[3][j]);
        }
}
/**
 * 把两个4X4数组进行异或
 */
static void addRoundTowArray(int aArray[4][4],int bArray[4][4]) {
    int i,j;
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            aArray[i][j] = aArray[i][j] ^ bArray[i][j];
}
/**
 * 从4个32位的密钥字中获得4X4数组，
 * 用于进行逆列混合
 */
static void getArrayFrom4W(int i, int array[4][4]) {
    int index,j;
    int colOne[4], colTwo[4], colThree[4], colFour[4];
    index = i * 4;
    splitIntToArray(w[index], colOne);
    splitIntToArray(w[index + 1], colTwo);
    splitIntToArray(w[index + 2], colThree);
    splitIntToArray(w[index + 3], colFour);
    for(j = 0; j < 4; j++) {
        array[j][0] = colOne[j];
        array[j][1] = colTwo[j];
        array[j][2] = colThree[j];
        array[j][3] = colFour[j];
    }
}
/**
 * 参数 c: 密文的字符串数组。
 * 参数 clen: 密文的长度。
 * 参数 key: 密钥的字符串数组。
 */
void deAes(char *c, int clen, char *key) {
    int cArray[4][4];
    int keylen,k;
    keylen = strlen(key);
    if(clen == 0 || clen % 16 != 0) {
        printf("密文字符长度必须为16的倍数！现在的长度为%d\n",clen);
        exit(0);
    }
    if(!checkKeyLen(keylen)) {
        printf("密钥字符长度错误！长度必须为16、24和32。当前长度为%d\n",keylen);
        exit(0);
    }
    extendKey(key);//扩展密钥
    for(k = 0; k < clen; k += 16) {
        int i;
        int wArray[4][4];
        convertToIntArray(c + k, cArray);
        addRoundKey(cArray, 10);
        for(i = 9; i >= 1; i--) {
            deSubBytes(cArray);
            deShiftRows(cArray);
            deMixColumns(cArray);
            getArrayFrom4W(i, wArray);
            deMixColumns(wArray);
            addRoundTowArray(cArray, wArray);
        }
        deSubBytes(cArray);
        deShiftRows(cArray);
        addRoundKey(cArray, 0);
        convertArrayToStr(cArray, c + k);
    }
}
```
