# qt 之 加密算法 - baidu_33570760的博客 - CSDN博客
2017年05月04日 12:01:46[carman_风](https://me.csdn.net/baidu_33570760)阅读数：738
转载自：http://blog.sina.com.cn/s/blog_a6fb6cc90101ge8c.html
在写这篇文章之前，我曾反复思量关于加密的叫法是否准确，更为严格来说，应该是密码散列-将数据（如中英文字母、特殊字符）通过复杂的算法转换为另一种固定长度的值。
**QCryptographicHash类**
    在Qt中，QCryptographicHash类提供了生成密码散列的方法。该类可以用于生成二进制或文本数据的加密散列值。目前支持MD4、MD5、SHA-1、SHA-224、SHA-256、SHA-384和SHA-512。
  这个类在QtCore4.3中被引入。
**公共类型**
- enumAlgorithm { Md4, Md5, Sha1, Sha224, ...,Sha3_512 }
**公共方法**
- QCryptographicHash(Algorithmmethod)
- ~QCryptographicHash()
- void addData(const char * data, int length)
- bool addData(QIODevice * device)
- void addData(const QByteArray & data)
- void reset()
- QByteArray result() const
**静态公有成员**
- QByteArray hash(const QByteArray & data, Algorithmmethod)
**成员类型文档**
- enum QCryptographicHash::Algorithm
|常量|取值|描述|
|----|----|----|
|QCryptographicHash::Md4|0|生成一个MD4散列|
|QCryptographicHash::Md5|1|生成一个MD5散列|
|QCryptographicHash::Sha1|2|生成一个SHA-1散列|
|QCryptographicHash::Sha224|3|生成一个SHA-224散列（SHA-2）。在Qt5.0介绍|
|QCryptographicHash::Sha256|4|生成一个SHA-256散列（SHA-2）。在Qt5.0介绍|
|QCryptographicHash::Sha384|5|生成一个SHA-384散列（SHA-2）。在Qt5.0介绍|
|QCryptographicHash::Sha512|6|生成一个SHA-512散列（SHA-2）。在Qt5.0介绍|
|QCryptographicHash::Sha3_224|7|生成一个SHA3-224散列。在Qt5.1介绍|
|QCryptographicHash::Sha3_256|8|生成一个SHA3-256散列。在Qt5.1介绍|
|QCryptographicHash::Sha3_384|9|生成一个SHA3-384散列。在Qt5.1介绍|
|QCryptographicHash::Sha3_512|10|生成一个SHA3-512散列。在Qt5.1介绍|
**成员函数文档**
- QCryptographicHash::QCryptographicHash(Algorithm method)
构造一个可以把数据创建为加密哈希值的对象。
- QCryptographicHash::~QCryptographicHash()
销毁对象。
- void QCryptographicHash::addData(const char * data, intlength)
将第一长度字符数据的加密哈希。
- bool QCryptographicHash::addData(QIODevice * device)
从开放的输入输出设备读取数据，直到结束并哈希它。如果成功读取，则返回true。
QtCore5.0中引入此功能。
- void QCryptographicHash::addData(const QByteArray &data)
这个函数的重载addData()。
- QByteArray QCryptographicHash::hash(const QByteArray &data, Algorithm method) [static]
使用此方法返回哈希数据。
- void QCryptographicHash::reset()
重置对象。
- QByteArray QCryptographicHash::result() const
返回最后的哈希值。
举例（对文本为“password”的字符串加密）：
（1）通过静态hase()方法计算
  QByteArray byte_array;
   byte_array.append("password");
    QByteArray hash_byte_array =QCryptographicHash::hash(byte_array,QCryptographicHash::Md5);
    QString md5 =hash_byte_array.toHex();
> 
（2）通过result()方法计算
    QByteArray byte_array;
   byte_array.append("password");
    QCryptographicHashhash(QCryptographicHash::Md5);  
    hash.addData(byte_array); //添加数据到加密哈希值
    QByteArray result_byte_array= hash.result();  //返回最终的哈希值
    QString md5 =result_byte_array.toHex();
> 
   md5结果：5f4dcc3b5aa765d61d8327deb882cf99，可以去找相应的工具进行验证！
    推荐一个网址：[http://www.md5.com.cn/](http://www.md5.com.cn/)。
    效果如下：
![](https://img-blog.csdn.net/20170515150709402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如上所示，无论使用穷举法还是其他手段来破解，都足以说明没有绝对的安全。因为理论上通过逐个查找匹配，是可以破解任何一种密文的，问题只在于如何缩短时间而已。
**MD5与SHA-1比较**
   二者均由MD4导出，所以SHA-1和MD5很相似。他们的强度和其它特性也很相似，但还有以下几点不同：
（1）对强性攻击的安全性：最显著和最重要的区别是SHA-1摘要比MD5要长32位。使用强行技术，产生任何一个报文使其摘要等于给定报文摘要的难度对MD5为2^128数量级操作，而对SHA-1则是2^160数量级操作。这样，SHA-1对强攻击有更大的优势。
（2）对密码分析的安全性：由于MD5的设计，易受密码分析的攻击，相比之下，SHA-1则不然。
（3）速度：相同硬件上，SHA-1运行速度比MD5慢。
**碰撞：**由于HASH函数产生定长的密文，结果是有限集合。而待处理的明文可以是计算机网络传输的任何信息。也就是说，明文信息是一个无限集合，密文信息却有限，两集合之间无一一对应关系。总有多个不同明文产生相同密文的情况发生，这就是所谓的碰撞。
   MD5与SHA-1曾被认为是足够安全的HASH算法，早在1994就有报告称，运算能力最强的机器，平均24天就可能找到一个MD5碰撞。王小云教授的方法已经为短时间内找到MD5与SHA-1碰撞成为可能。虽然如此，也并不意味着两种方法就此失效，再者，也可以通过自己的手段来进一步处理。比如：通过MD5与SHA结合实现。将A进行MD5处理得到B，将A在进行SHA处理得到C，再将B与C结合（比如：相加），也可把结合后的结果再进行MD5加密。这足以将碰撞机滤降至很小很小，所以没有绝对的安全，只有更安全。
更多参考：
   QCA-（基于Qt的加密体系结构）
- [QCA](http://delta.affinix.com/docs/qca/index.html)
- [QCA(wiki)](https://en.wikipedia.org/wiki/QCA)
- [Using libraries: QCA (Qt CryptographicArchitecture)](http://www.essentialunix.org/index.php?option=com_content&view=article&id=48:qcatutorial&catid=34:qttutorials&Itemid=53)
**注：**
   技术在于交流、沟通，转载请注明出处并保持作品的完整性。

