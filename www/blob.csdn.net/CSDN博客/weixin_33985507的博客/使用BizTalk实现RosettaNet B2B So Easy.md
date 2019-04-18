# 使用BizTalk实现RosettaNet B2B So Easy - weixin_33985507的博客 - CSDN博客
2015年10月26日 10:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
# 使用BizTalk实现RosettaNet B2B So Easy
最近完成了一个vmi-hub的B2B项目，使用Rosettanet 2.0的标准与一家品牌商，OEM，供应商实现B2B。一共交换4个报文，4B2，3B2，4B2 POD，4C1，说白了就是收发存这些数据；
项目环境
BizTalk 2009 Accelerator for RosettaNet，并没有使用最新的BizTalk版本因为客户已经在使用了，所以就在现有的环境下进行。
## 下面说一下如何配置RosettaNet
- 
### 交换证书
RosettaNet B2B中证书的用途和导入就不在这里详细说了，建议服务器端证书（IIS的SSL证书）和解密/验签的使用同一本证书
- 
### 配置partner
配置Partner/Home Organization,这部分配置也非常简介，关键是Name和GBI 两个栏位填写正确就可以了
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103541466-175477278.png)
- 
### 配置Process Configuration
定义每个pip的处理规则。以下我们用需要了3b2,4B2,4C1,0A1
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103544154-2051970893.png)
每个pip代码都是有标准的，第一次做项目的时候你可能不知道如何填写
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103545091-836592845.png)
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103545669-1333077602.png)
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103546482-1709630323.png)
以上内容都可以通过[https://www.rosettanet.org/RosettaNet-Standards/Standards-Library/PIP-Directory](https://www.rosettanet.org/RosettaNet-Standards/Standards-Library/PIP-Directory) 可以查到，都是标准定义
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103547357-996622889.png)
- 
### 配置Agreement
OA1 agreement的做作用是当处理这个pip流程时如果错误就会返回给对方一个0A1报文
Home role 如果选择Initiator表示做为发送方，Responder表示接收方
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103548810-554662188.png)
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103549388-1771237003.png)
Ports 都填写对方的URL
其它的基本默认就可以了
以上配置还是非常简单的
## 流程如何调用RosettaNet API
在流程中只需要执行SubmitRNIF.SubmitMessage这个方法就可以了，但是在执行这个方法前必须先执行以下代码，在XML字符串的头部加上已下定义，应为Rosettanet的报文是通过dtd验证的，所以必须要加
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103550247-1457722671.png)
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103550888-1891766816.png)
## RosettaNet Troubleshooting
- 
### UNP.SCON.VALERR：A failure occurred while validating the service content.
         Error Number:2005
         Description:A failure occurred while validating the service content.
验证Playload内容是否和dtd的定义一致，RosettaNet 加速器是严格的执行XML Schema验证只要有一个节点不符合就会报以上错误
- 
### UNP.SHDR.VALERR : A failure occurred while validating the service header.
请检查Process Configuration配置和消息的playload/serviceHeade 是否一致
- 
### 400 Bad Request OR Time out
双方检查证书是否导入正确，特别是对于自发布的证书，对方需要导入根证书到受信任的目录下，如果对方是linux或其他OS，则需要根据相应的操作
- 
### Error code:UNP.MESG.DCRYPTERR
检查配置 Is Persistent Confidentiality Required 一般选None就可以了，不排除对方会选Palyload
还有一种情况是 acknowledgement of receipt\Non-Repudiation Required 也需要双方定义
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103551482-597055031.png)
## 运维工具
测试阶段免不了报文重发，做了一个报文重发的工具，可以选择发送错误报文再次重发，在原来LOB Application的源代码基础上加了一个选择发送失败的报文再次重发
源代码提供下载
![](https://images2015.cnblogs.com/blog/5997/201510/5997-20151026103552325-2056089052.png)
希望对初次使用BizTalk做Rosettanet B2B 项目的能提供一些帮助，当然有问题也可以留言哦
