# 对称加密和分组加密中的四种模式(ECB、CBC、CFB、OFB) - z69183787的专栏 - CSDN博客
2015年11月23日 14:03:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5908
**一. AES对称加密:**
![](http://images.cnblogs.com/cnblogs_com/happyhippy/AES1.jpg)
                                                      AES加密
![](http://images.cnblogs.com/cnblogs_com/happyhippy/AES2.jpg)
                         分组
**二. 分组密码的填充**
![](http://images.cnblogs.com/cnblogs_com/happyhippy/Pad1.jpg)
                                                   分组密码的填充
e.g.:
![](http://images.cnblogs.com/cnblogs_com/happyhippy/Pad2PKCS5.jpg)
                                                         PKCS#5填充方式
**三. 流密码:**
**![](http://images.cnblogs.com/cnblogs_com/happyhippy/StreamCrypty.jpg)**
**四. 分组密码加密中的四种模式:**
**3.1 ECB模式**
![](http://images.cnblogs.com/cnblogs_com/happyhippy/1ECB.jpg)
**优点:**
1.简单；
2.有利于并行计算；
3.误差不会被传送；
**缺点:**
1.不能隐藏明文的模式；
2.可能对明文进行主动攻击；
![](http://images.cnblogs.com/cnblogs_com/happyhippy/1ECB2.jpg)
**3.2 CBC模式：**
![](http://images.cnblogs.com/cnblogs_com/happyhippy/2CBC.jpg)
**优点：**
1.不容易主动攻击,安全性好于ECB,适合传输长度长的报文,是SSL、IPSec的标准。
**缺点：**
1.不利于并行计算；
2.误差传递；
3.需要初始化向量IV
**3.3 CFB模式：**
**![](http://images.cnblogs.com/cnblogs_com/happyhippy/3CFB.jpg) 优点：**
1.隐藏了明文模式;
2.分组密码转化为流模式;
3.可以及时加密传送小于分组的数据;
**缺点:**
1.不利于并行计算;
2.误差传送：一个明文单元损坏影响多个单元;
3.唯一的IV;
**3.4 OFB模式：**
**![](http://images.cnblogs.com/cnblogs_com/happyhippy/4OFB.jpg) 优点:**
1.隐藏了明文模式;
2.分组密码转化为流模式;
3.可以及时加密传送小于分组的数据;
**缺点:**
1.不利于并行计算;
2.对明文的主动攻击是可能的;
3.误差传送：一个明文单元损坏影响多个单元;
