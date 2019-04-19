# HTTPS常用算法 - 定义域 - CSDN博客
2015年10月29日 10:56:09[定义域](https://me.csdn.net/u010278882)阅读数：848
# Base64
## bin <-> txt
Base64编码是**用可打印的字符**来表对**二进制数据**进行编码。 
某些系统只能传输文本流，而不能传输二进制流。Base64编码解决了只能对某些系统只能处理文本性数据而不能处理二进制数据的问题。 
比如，邮件内容和附件的传输、XML文件中复杂数据的传输。
下面是一封邮件的附件的部分编码
```
------=_NextPart_55DEAC75_7F70AC00_0E0C615E
Content-Type: text/html;
    charset="utf-8"
Content-Transfer-Encoding: base64
PGRpdiBzdHlsZT0iZm9udDoxNHB4LzEuNSAnTHVjaWRhIEdyYW5kZScsICflvq7ova/pm4Xp
u5EnO2NvbG9yOiMzMzM7Ij48cCBzdHlsZT0iZm9udDoxNHB4LzEuNSAnTHVjaWRhIEdyYW5k
ZSc7bWFyZ2luOjA7Ij48c3BhbiBzdHlsZT0iZm9udC1mYW1pbHk6IEFyaWFsOyBmb250LXNp
emU6IHNtYWxsOyBsaW5lLWhlaWdodDogbm9ybWFsOyBjb2xvcjogcmdiKDAsIDAsIDApOyB3
...
------=_NextPart_55DEAC75_7F70AC00_0E0C615E--
```
## 标准而非实现
Base64是一种编码方法，没有规定具体的编码规则以及编码中使用的字符。因此各种编码实现可能使用不同的编码规则。 
因为该标准是MIME提出的，所以其提供了一种实现。其他实现基本上都是从这种实现中演变而来的。 
编码字符：A-Z,a-z,0-9,+,/，同时使用=作为特殊处理 
编码过程：数据转bit，取3bit放入24bit缓冲区，然后按6bit编码 
编码后数据量约是编码前的**4/3**。
[不同Base64编码实现](https://en.wikipedia.org/wiki/Base64#Implementations_and_history)
### MIME的Base64编码示例
![MIME Base64](https://img-blog.csdn.net/20151029105256899)
说明：这里对”Man”进行编码，编码过程如下： 
1. 文本转ASCII字符，其二进制形式共24bits 
2. MIMEBase64以24bit为缓冲区，然后按6bit编码，因此24bit划分成四个单元 
3. 每个单元对应的二进制数转成十进制数 
4. 每个十进制数转成对应的Base64编码字符
### MIME Base64 Padding
![这里写图片描述](https://img-blog.csdn.net/20151029113916384)
```
20c * 8bit/c
= 160bit / 24bit // 先构造成24bit缓冲区
= 20c * 8bit/c / 24bit
= 20c / 3 // 实际等价于源数据长度/3
// 因为按照 3 为单位整除，所以要补全字符长度/3余1或2的情况
// 因此使用 = 来表示编码中源数据bit不足的情况，实际补0来补全
// 并用 = 表示少两个byte，用 == 表示少一个byte
= 20c / 3 补一个byte
= 21c * 8bit/c / 6bit/base64c
= 28base64c 且最后一个字符为 =
```
用 = 表示少两个byte，用 == 表示少一个byte
## Java 实现
// TODO 自行编码Base64编解码实现
JDK没有提供实现，可以使用 Apache Commons 的编码库
遵循RFC 2045 
[Apache Commons Base64](https://commons.apache.org/codec/apidocs/org/apache/commons/codec/binary/Base64.html)
// TODO 示例
## Android 实现
遵循MIME的Base64规范（RFC 2045和 RFC 3548） 
[Android Base64](http://developer.android.com/intl/zh-cn/reference/android/util/Base64.html)
// TODO 示例
# 主要加密算法常识
> 
密钥长度越长，就越安全
- RSA加密算法的原理：大整数的因式分解不可计算性。在给定的时间内无法计算出给定整数的因式分解。
- 安全性：ECC > RSA > DSA
- 计算时间：与安全性相同，越安全，计算时间越长
命名和提出 
RSA 发明该算法的三位数学家命名 
DSA Data Signature Algorithm(数字签名算法) 
ECC Ellipic Curvus Cryptography(椭圆曲线密码编码学) 1985年提出 
DES Data Encryption Standard，IBM设计并被1977年美国政府采用 
AES Advance Encryption Standard，实际采用算法为1999年两位研究院发明的算法 Rijndael 
MD5 Message Digest Algorithm 5（消息摘要算法 5） 
SHA1 Secure Hash Algorithm（安全散列算法），美国国家标准和技术委员会1993年提出（NIST）
- 签名算法的原理：算法计算的不可逆性。根据一串输入串计算出相应的值，该值的计算方式是不可逆的
- 安全性和计算时间：SHA1更安全，MD5计算速度更快
- 二者都是在MD4基础上发明的，有很大的相似性
- 证据表明MD5不安全，推荐使用SHA
计算输入和输出 
MD5  in: 任意长度”字节串” out: 128bit大整数 
SHA1 in: 最长 264bit的信息 out: 160bit/256bit大整数
## MD5和SHA1计算(Java实现)
[HashUtil](https://code.csdn.net/snippets/1275063.git)
# 工具
- [在线加密/解密、散列值计算](http://tool.oschina.net/encrypt)
