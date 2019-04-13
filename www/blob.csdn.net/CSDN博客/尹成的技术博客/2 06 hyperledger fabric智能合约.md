
# 2.06 hyperledger fabric智能合约 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:39:41[尹成](https://me.csdn.net/yincheng01)阅读数：290


**1.智能合约**
执行环境安全隔离、不受第三方干扰
链码是fabric应用层的基石，是应用层与底层的桥梁
执行环境是一个独立的docker环境
通过gRPC协议与背书节点连接，只有背书节点才会运行链码
链码的生命周期打包
安装
实例化
升级
交互
链码的交互流程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170307816.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
系统链码LSCC（Lifecycle System Chaincode），用于管理链码的生命周期
CSCC（Configuration System Chaincode），是配置管理链码
QSCC（Query System Chaincode），用于查询账本存储
ESCC（Endorsement System Chaincode），交易背书链码
VSCC（Validation System Chaincode），用作交易验证
链码编程接口Init()
Invoke()
例子示范fabric_asset/chaincode/badexample/badexample.go
2.网络搭建配置的实现
配置文件准备设置工作路径
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170315150.png)
进入配置文件目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/201812051703195.png)
生成证书文件
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170322436.png)
可以查看生成的证书
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170329509.png)
创建目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120517033453.png)
生成系统链的创世区块
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170339564.png)
生成通道的创世交易
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170345669.png)
分别生成两个组织关于通道的锚节点交易
![在这里插入图片描述](https://img-blog.csdnimg.cn/201812051703521.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170355947.png)
[](https://img-blog.csdnimg.cn/201812051703521.png)
网络启动启动网络
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170401276.png)
查看orderer节点日志
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170406499.png)
查看peer节点日志
![在这里插入图片描述](https://img-blog.csdnimg.cn/201812051704131.png)
初始化和操作链码进入client进行初始化操作
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170421656.png)
创建通道
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170428861.png)
加入通道
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120517043425.png)
设置主节点
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120517043862.png)
安装链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170443862.png)
查看安装的链码
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170450765.png)
查看链码版本
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170457601.png)
链码实例化
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170503760.png)
查看镜像
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170509570.png)
查看部署的链码日志
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170515591.png)
链码查询
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170519658.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205170522884.png)
[](https://img-blog.csdnimg.cn/20181205170519658.png)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

