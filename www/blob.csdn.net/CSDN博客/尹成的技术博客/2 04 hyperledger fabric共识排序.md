
# 2.04 hyperledger fabric共识排序 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:38:28[尹成](https://me.csdn.net/yincheng01)阅读数：298


**1.共识机制介绍**
交易背书：客户端节点根据背书策略，选择背书节点，发送交易提案，背书节点调用智能合约执行模拟交易，执行完成后，经过签名背书，返回给客户端节点，整个过程是模拟的
交易排序：排序节点接收已经签名背书的交易，确定交易顺序，将排好序的交易打包成区块，分发给其他组织主节点，排序节点不会去关心交易是否正确，只负责排序和打包区块
交易验证：区块存储和交易验证不冲突，区块存储前进行交易验证，fabric中区块状态是世界状态
**2.Orderer节点作用**
交易排序
区块分发
多通道数据隔离
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170017552.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
3.源码阅读
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170025575.png)
4.共识机制源码目录结构
目录结构介绍
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170031115.png)
bccsp：与密码学相关的，主要包括加密、数字签名和证书等，将密码学中的函数抽象成一组接口，可以很方便地扩展
bddtests：是一个新型的软件开发模式，叫做行为驱动开发，传统软件是从需求到概要设计再到详细设计最后到开发，行为驱动开发是需求直接到开发
common：公共库，错误处理、日志处理、账本存储以及各种工具等
core：是fabric的核心库，子目录中有与各个模块相关的目录chaincode：与智能合约相关
comm：与网络通信相关
endorser：背书节点相关
devenv：官方提供的开发环境，使用的是Vagrant
docs：文档相关
enents：事件监听机制
examples：一些例子程序
gossip：通信协议，最终一致性的共识算法，用于组织内部区块同步
gotools：用于编译
images：用作docker镜像打包
msp：成员服务管理，member service provider，fabric会为每一个成员提供证书，msp模块用于读取证书做签名等
orderer：排序节点的入口
peer：peer节点的入口
proposals：新功能提案
protos：fabric提供的所有数据结构和服务都在这里面定义
下面的就不重要了
5.共识机制源码阅读
看orderer中的main.go
6.Manageer源码阅读
点击server.go中的Manager进入Manager.go
7.ChainSupport源码阅读
点击Manager.go中的ChainSupport进入chainsupport.go
8.区块切割和solo模式源码阅读
点击chainsupport.go中的Receiver进入blockcutter.go
9.交易收集和区块扩散源码阅读
查看交易收集
查看区块如何扩散
10.共识机制总结
main.go是入口，是orderer节点初始化的流程
manager.go是多链的控制中枢，所有对链的操作，都要从这里拿到chainsupport对象的实例
chainsupport.go是链对象的帮助接口，是链的代理，与链一一对应
区块切割中重点是Orderer方法，判断当前排序节点的交易是否应该被切割到一个新的区块，如果是，则调用Cut方法切割
solo共识将区块链分布式系统简化为了一个单机版的中心化系统，容易理解
交易收集和区块扩散：是server.go中两个接口的实现
排序节点实现了一个可插拔的服务，实现了多通道功能，现节点官方实现了solo和kafka两种共识，也可以扩展其他的共识
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

