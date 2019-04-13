
# 2.01 hyperledger fabric概念 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:36:29[尹成](https://me.csdn.net/yincheng01)阅读数：153


**1.什么是hyperledger**
超级账本是Linux基金会在2015年发起的“开放式账本”开源项目，“开放式账本”是为推进区块链技术应用于金融领域及交易验证方向的发展而发起的，“开放式账本”随后被更名为“超级账本”
其加入成员包括：荷兰银行（ABN AMRO）、埃森哲（Accenture）等十几个不同利益体，目标是让成员共同合作，共建开放平台，满足来自多个不同行业各种用户案例，并简化业务流程
由于点对点网络的特性，分布式账本技术是完全共享、透明和去中心化的，故非常适合于在金融行业的应用，以及其他的例如制造、银行、保险、物联网等无数个其他行业
通过创建分布式账本的公开标准，实现虚拟和数字形式的价值交换，例如资产合约、能源交易、结婚证书、能够安全和高效低成本的进行追踪和交易
官网地址：[https://www.hyperledger.org/](https://www.hyperledger.org/)
**2.hyperledger与数字货币**
都是基于区块链实现的
hyperledger是区块链技术，没有挖矿的概念，有独特的共识协议
比特币1秒只支持7笔交易，以太坊每分钟可以处理几百笔交易，hyperledger理论上支持每分钟50万笔交易，因为没有挖矿，不需要很强的硬件支持，也不耗费资源
hyperledger所有节点都保存了完全一致的区块链数据，保证数据完整性和数据安全
没有51%攻击，由CA机构进行节点认证，是有许可的网络
**3.现实问题**
例如两家公司对账，由于两家公司数据存储技术栈不一样，数据对接出问题，也就是信任谁的数据
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162518616.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
银行和银联对账通知
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162527958.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
由于对账成本太高，可能找第三方机构帮助判定，认证需要交认证费，找到两个公司都信任的第三方也不容易，三方机构的安全也有问题，因为是由人来操作的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162534945.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
4.hyperledger全家福
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205162554442.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
地址：[https://github.com/hyperledger](https://github.com/hyperledger)
5.什么是hyperledger fabric
由IBM和Digital Asset最初贡献给Hyperledger项目
目标是做企业级联盟链的基础设施
可插拔的共识机制（solo和kafka等形式）
多链多通道隔离，可以做业务隔离，保证业务数据隐私
6.hyperledger fabric的重要组件
fabric CA
fabirc Peer
fabric ordering service
7.hyperledger的开发语言
Nodejs/Java/Go/Python
官方推荐Nodejs
用的最多的是Java
Go支持的最差
8.hyperledger的channel
每个channel可以理解成独立的hyperledger fabric的实例
不同的channel是私有的子网，可以进行业务数据隔离
peer可以加入不同的channel
还可以设置允许什么人加入等
9.hyperledger的chaincode
用于更新账本数据，peer去执行chaincode
在fabirc里，chaincode是更新数据的唯一方式
chaincode是属于某一个channel的
chaincode的生命周期
10.hyperledger的msp
msp（membership service provider）是一组重要的密码学签名工具
定义了你是谁，你在哪（在哪个channel中）
证书由CA去颁发
11.术语回顾
channel数据通道，可以理解成独立的fabric实例，不同channel数据是隔离的，保证区块链上的数据隐私问题，类似微信群组
world state是世界状态，是当前ledger里面存放的数据，key和value以version形式存在，存放方式有couchdb和leveldb
ledger是账本，记录的是当前所有的世界状态，是一个具有授权管理的共享账本系统，从底层设计上保证了数据的一致性、有效性和不可篡改性
chaincode链码，编写的智能合约就是区块链中的法律，ledger的变化只能通过调用chaincode来完成
peer是整个网络的基础，它是账本和智能合约的载体，一个区块链网络由peer节点组成，每一个peer节点持有一个或多个账本，以及一个或多个智能合约
network是由peer组成的网络，在同一个网络中的peer实时同步记账，保证ledger数据的一致性
ordering service排序服务，进行排序和验证，最终提交invocation，将数据写入peer的ledger
membership service provider是管理peer身份和访问许可的
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

