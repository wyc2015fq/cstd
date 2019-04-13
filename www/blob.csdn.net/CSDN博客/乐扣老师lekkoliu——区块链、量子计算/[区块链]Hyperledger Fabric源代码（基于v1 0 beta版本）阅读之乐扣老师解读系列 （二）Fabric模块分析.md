
# [区块链]Hyperledger Fabric源代码（基于v1.0 beta版本）阅读之乐扣老师解读系列 （二）Fabric模块分析 - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

置顶2017年06月14日 11:19:25[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：7613所属专栏：[区块链](https://blog.csdn.net/column/details/20660.html)



## Hyperledger Fabric模块划分
`Hyperledger项目的git地址：https://github.com/hyperledger/fabric.git`
本第二讲主要是从fabric源代码结构开始着手分析。
首先看下整体结构：
整个Hyperledger的fabric技术架构部分
主要是由以下为三部分组成：
fabric-peer：主要起到 peer 作用，包括 endorser、committer 两种角色；
fabric-ca：即原先的 membersrvc，独立成一个新的项目。
fabric-order：起到 order 作用。
其中，fabric-peer 和 fabric-order 代码暂时都在 fabric 项目中，未来可能进一步拆分。
## 下面是fabric中各个包的大概内容：
## 一，bccsp ✳️
区块链加密服务提供者（Blockchain Crypto Service Provider），提供一些密码学相关操作的实现，包括 Hash、签名、校验、加解密等。
主要支持 MSP 的相关调用。
## 二，bddtests
行为驱动测试（Behaviour Driven Development）相关代码。主要是关于各种测试，线下peer节点部署等相关的操作。
## 三，common
一些通用的功能模块。包括常用的配置config、加密签名的crypto、ledger设置，工具包含协议设置等等。
## 四，core ✳️
大部分核心实现代码都在本包下。其它包的代码封装上层接口，最终调用本包内代码。包含区块链操作Chaincode代码实现、peer节点消息处理及行为的实现、容器container的实现如docker交互实现、策略实现policy及预处理endorser等等。
## 五，devenv
主要是方便本地搭建开发平台的一些脚本。主要包含了CouchDB设置、golang编译脚本、64位ubantu配置脚本等等。
## 六，docs
项目相关的所有文档。包含客户定制主题以及一些工具的源代码。
## 七，events ✳️
EventHub 服务处理相关的模块。主要是包含了消费者，生产者的实现代码。另外，Even服务其包含了四种类型定义如下：REGISTER = 0;BLOCK = 1;CHAINCODE = 2;REJECTION = 3。
## 八，examples
示例文件夹，包括一些 chaincode 示例和监听事件的示例。
## 九，gossip ✳️
流言算法–gossip算法。一个基于pull的gossip算法的实现。最终确保状态一致。 该协议大致如下：
1）A发起者发送Hello（B唯一标识，nonce）消息到B远程节点（多个）。
2）收Hello信息后发送SendDigest到A节点，其中包含nonce
3）A收到SendDigest，校验数据和nonce，把B作为待发送节点，并封装想要pull的数据SendReq到B节点
4）B收到SendReq发送SendRes到A节点，数据为SendReq不包含的数据
## 十，gotools
go 相关的开发工具的安装脚本：golint、govendor、goimports、protoc-gen-go、ginkgo、gocov、gocov-xml 等。
## 十一，images
一些跟 Docker 镜像生成相关的配置和脚本。主要包括各个镜像的 Dockerfile.in 文件。这些文件是生成 Dockerfile 的模板。
## 十二，msp ✳️
成员服务提供者（Member Service Provider），提供一组认证相关的密码学机制和协议，用来负责对网络提供证书分发、校验，身份认证管理等。一些成员管理的实现代码等。
## 十三，orderer ✳️
在 fabric 1.0 架构中，共识功能被抽取出来，作为单独的 fabric-orderer 模块来实现，完成核心的排序功能。最核心的功能是实现从客户端过来的 broadcast 请求，和从 orderer 发送到 peer 节点的 deliver 接口。同时，orderer 需要支持多 channel 的维护。主要包含Solo、kafka及bft三个方法。
## 十四，peer ✳️
peer节点的相关主命令模块。
作为服务端时候，支持 node 子命令；作为命令行时候，支持 chaincode、channel 等子命令。其中包含一些命令操作的实现等等。
## 十五，proposals
一些建议，包含现在对区块的结构优化建议及时序图的呈现。还有其他方面的一些建议文件。
## 十六，protos ✳️
Protobuf 格式的数据结构和消息协议。都在同一个 protos 包内。
这里面是所有基本的数据结构（message）定义和 GRPC 的服务（service）接口声明。
## 十七，release
关于如何从dockerhub中拉取docker镜像的相关操作及脚本代码。
## 十八，release_notes
关于最新2017年6月8日beta版本更新的相关资讯。主要包括release笔记内容及版本变根日志。
## 十九，sampleconfig
提供了一些样例证书文件和配置文件。pem格式，通过openssl来查看内容。内容基于BASE64来进行编码。
## 二十，scripts
一些辅助脚本，多数为外部 Makefile 调用。比如一些依赖环境的安装如python-pip、然后pip的安装包中的一些依赖环境等。还有一些配置，如让容器永不退出等。
## 二十一，test
用于测试的一些脚本。 主要包含chaincode、回归测试脚本、容器关联order节点及peer节点测试脚本、环境构筑测试相关脚本如channel、以及一部分的工具LTE、OTE、PTE。
## 二十二，unit-test
单点docker配置测试脚本
## 二十三，vendor
关于部分提供商的内容及管理依赖，包含github.com、golang.org、google系列及gopkg.in相关内容。
## 除了上述的包信息之外，主目录里面还包括一些说明文档、安装需求说明、License 信息文件等。（ ✳️标号的部分会在后续系列中做专题讲解。）
## 结束语
觉得好的朋友可以给我大赏一杯咖啡钱缓解通宵梳理环境的疲乏〜！
方式一：支付宝
![这里写图片描述](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
方式二：以太坊
![这里写图片描述](https://img-blog.csdn.net/2018083110244130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[
](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
