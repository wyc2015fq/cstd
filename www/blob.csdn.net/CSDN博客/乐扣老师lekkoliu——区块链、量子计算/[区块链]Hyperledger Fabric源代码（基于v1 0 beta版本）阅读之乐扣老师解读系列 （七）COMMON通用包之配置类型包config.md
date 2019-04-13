
# [区块链]Hyperledger Fabric源代码（基于v1.0 beta版本）阅读之乐扣老师解读系列 （七）COMMON通用包之配置类型包config - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

置顶2017年06月20日 15:42:40[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：2384所属专栏：[区块链](https://blog.csdn.net/column/details/20660.html)



核心包之COMMON通用包之配置类型包config
首先，我们看其代码结构：
![这里写图片描述](https://img-blog.csdn.net/20170620110839768?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHN0dG95/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170620110839768?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHN0dG95/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里主要是包含各种策略定义工具包、编译工具包、配置文件工具包、加解密工具包、账本包、和docker挂载相关的工具包、基本数据metadata工具包等等。
对于本包的内容，我们大概分为参数配置类型及策略配置相关包、系统工具类型包、加解密相关的包以及其他包这样四块内容来讲解。
一，配置类型包，主要包含cauthdsl、config、configtx及policies这四块核心包内容。
二，系统工具类型包主要包含tools包、util包、metadata包、flogging包以及ledger包。
三，加解密包crypto包、localmsp包。
四，其他系统包如errors包、genesis包以及viperutil包。
---一，COMMON通用包之配置类型包

---包内容简介：本包主要是关于fabric全链的一个配置。包含对applicaition应用群组的配置、对1.0版特有的通道channel

---核心包1 config:

---核心文件系列1 application:

---application系列核心内容：包含对applicaition应用群组的配置、获取应用实例、校验。还有可以创建无head内容的对等体用来锚定节点的TemplateAnchorPeers方法。还有通过ApplicationOrgConfig来进行锚定链的方法AnchorPeers，其返回有效的orderer地址列表用以连接到调用Broadcast / Deliver。

---核心文件系列2 channel:

---channel系列核心内容：channel通道，主要是包含对应通道相关的数据，包括其包含的各数据模块、宽度、联盟相关信息等。channel_util中还包含了一部分哈希算法的实现，区块哈希结构的实现以及对应的order地址。

---⭕️常量结构系列：包含常用的参数有“Consortium”，被定义ConsortiumKey，其做为联盟消息cb.ConfigValue的关键信息。HashingAlgorithmKey是“HashingAlgorithm”消息的cb.ConfigItem类型密钥名称。OrdererAddressesKey是“OrdererAddresses”消息的cb.ConfigItem类型密钥名称。“Channel”是一个用来标示群体密钥的。

---⭕️核心方法系列：Allocate()是为待配置的更新来创建新的配置资源。HashingAlgorithm()哈希算法返回一个指向链哈希算法的函数指针。BlockDataHashingStructureWidth()返回在形成块数据散列结构时使用的宽度.OrdererAddresses()订单地址返回要连接的有效订单地址列表，用来调用广播/传送。ConsortiumName()返回此通道创建的联盟的名称。Validate（）校验检查生成的配置文件，确保值正确，和设置在Commit后可能引用的ChannelConfig字段。

---核心文件系列3 order:

---order系列核心内容：order节点主要是用来做共识，。

---⭕️常量结构系列：包含常用的参数有ConsensusTypeKey(共识总类key)是ConsensusType消息的cb.ConfigItem类型密钥名称。同时还有另外三个key包括批处理大小的BatchSize、批处理过期时间的BatchTimeout、channel的频道限制字符ChannelRestrictions以及卡夫卡服务器标示的KafkaBrokers。这四个信息就是一个OrdererProtos的完整结构，其同用做为OrdererConfig的source配置。

---⭕️核心方法系列：KafkaBrokers()KafkaBrokers返回一组“引导”的地址（IP：端口符号）卡夫卡服务器(broker)不一定是传统意义上的整套卡夫卡服务器，本服务器(broker)用于共识中心（orderer）。MaxChannelsCount()返回此订单支持的最大通道数。Organizations() 用来返回该channel中的组织数。还有一些对上面四个参数的非空验证方法以及对broker的系统性检查（即满足3f+1等等）

---核心文件系列4 organizaiton:

---organizaiton系列核心内容：主要是对MSP(成员管理服务)的信息进行处理和操作。包括获取实例、姓名、成员服务ID等等。

---⭕️常量结构系列：包含常用的参数有MSPKey是序列化的mspconfig.MSPConfig的值键。OrganizationGroup是配置存储组织的常用配置信息，包含推荐人proposer、组织配置信息OrganizationConfig、名称name及msp密钥的配置操作者mspConfigHandler。

---核心文件系列4 proposer:

---proposer系列核心内容：proposer主要是用来做预处理提交，是fabric 1.0的一个新功能。

---⭕️常量结构系列：包含常用的参数有ValueDeserializer结构提供了一种检索proto消息以反序列化配置值的机制。Values定义了一种配置向未序列化提供消息的机制和一个验证结果的机制。

---⭕️核心方法系列：BeginValueProposals()定义了启动配置提议时调用的一种方法。PreCommit()预提交，验证确保新的配置值是有效的更改。RollbackProposals()当配置提议被放弃时调用。CommitProposals()提交配置提议时调用。

---核心文件系列4 root:

---root系列核心内容：Root作为锚定其余配置的对象。其意图是在重构结束时提升一个级别。

---⭕️常量结构系列：包含Root结构，里面包含channel的群组channel，和msp的配置操作者mspConfigHandler两个参数。

---⭕️核心方法系列：串行化检测器Deserialize()，用来对invoke前的字符串进行格式检查。BeginValueProposals()用于启动新的配置提议。RollbackConfig用于放弃新的配置提议。Channel返回相关的通道级配置。Orderer返回相关的Orderer级别配置。Application返回相关的application级配置。Consortiums联合体返回其配置。

---核心文件系列5 standardValues:

---standardValues系列核心内容：Root作为锚定其余配置的对象。其意图是在重构结束时提升一个级别。

---⭕️核心方法系列：

---NewStandardValues(protosStructs …interface{}) (*standardValues, error)

---NewStandardValues接受只能包含protobuf消息的结构类型。 结构可以嵌入满足其他（非指针）结构同样的条件。 NewStandard值将实例化所有proto的内存消息并构建从结构字段名称到proto消息实例的查找映射，这是一个很好的方式来轻松实现Values的接口。

---(sv *standardValues) Deserialize(key string, value []byte) (proto.Message, error)

---反序列化查找给定名称的值proto，  将给定的字节解构用以填充支持消息结构，并返回引用的保留的反序列化消息（或因为密钥不存在的一个错误，或者一个错误的解构等）

---结束语

---觉得好的朋友可以给我大赏一杯咖啡钱缓解通宵梳理环境的疲乏〜！

---方式一：支付宝

![这里写图片描述](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---[ ](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

---方式二：以太坊

![这里写图片描述](https://img-blog.csdn.net/2018083110244130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

---[
](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

