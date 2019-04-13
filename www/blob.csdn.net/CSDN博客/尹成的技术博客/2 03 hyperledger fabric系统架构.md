
# 2.03 hyperledger fabric系统架构 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:37:55[尹成](https://me.csdn.net/yincheng01)阅读数：251


**1.系统架构**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205163210191.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
API：提供了GRPC调用，GRPC是跨语言的RPC框架
SDK：在API基础上，提供了SDK，封装了各种语言，例如Go、node.js、python和Java等，官方推荐node.js
事件：区块链是分布式系统，各个节点达成一致需要消耗很长时间，fabric使用异步通信模式开发，触发回调函数执行
身份：任何一个请求都要通过签名验证，依托于底层的成员服务，是联盟链的认证功能，例如CA
账本：是对区块链的查询，例如按照区块高度查询，根据区块哈希查询，可以根据交易ID查询区块，交易ID也是不重复的，还可以根据通道名称，获取整个账本的信息，在fabric中，账本是根据通道进行隔离的，不仅仅体现在逻辑上，在物理层面也是分割的
交易：是对区块链数据的修改，例如上层程序通过交易管理模块，将交易提交到交易背书节点，获得交易背书后，将交易提供给哈希节点，进行排序，排序后打造成区块，进行分发，然后将交易分发到整个网络
智能合约：做智能合约的安装、初始化和升级等操作，智能合约可以理解为函数声明，交易可以理解为函数调用
**2.网络拓扑**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205163216142.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
注册登记：向CA机构表明自己的身份，获取证书，此图CA在区块链以外，是第三方的CA，也可以使用官方提供的CA
交易提案：向两个组织的背书节点发送交易提案请求，组织1和组织2分别对应了一组peer节点，可以认为是现实生活中的两个商业主体，组织1中有4个peer节点，组织2中有3个peer节点，主节点用于从orderer节点接收区块，通过gossip协议进行区块扩散，两个组织是独立的，共同的数据来源是排序节点
提交交易：排序节点从客户端节点接收交易，在排序节点内部排序后打包成区块，广播给其他组织的主节点
像一个树形拓扑，最上面是kafka，然后是每个组织的主节点，最后是记账节点
**3.交易流程**
![在这里插入图片描述](https://img-blog.csdnimg.cn/201812051632232.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120516322838.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

