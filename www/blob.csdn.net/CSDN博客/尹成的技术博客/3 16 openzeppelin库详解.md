
# 3.16 openzeppelin库详解 - 尹成的技术博客 - CSDN博客

2018年11月08日 19:17:50[尹成](https://me.csdn.net/yincheng01)阅读数：130


地址：https://github.com/OpenZeppelin/openzeppelin-solidity
详解：https://www.linuxidc.com/Linux/2016-10/135891.htm
一个在以太坊上建立安全智能合约的框架，目前集成与Truffle和Embark
安装步骤(ubuntu)新建一个自己的合约目录，进入合约目录
Truffle init
npm init -y 该步生成一个package.json，内置一些配置信息
npm install -E openzeppelin-solidity
导入：import ‘openzeppelin-solidity/contracts/ownership/Ownable.sol’;
模块access：地址白名单和基于签名的权限管理
crowdsale： 用于管理令牌众筹的一系列智能合约，允许投资者购买eth代币
examples：一组简单的智能合约，演示如何通过多重继承向基础合约添加新功能。
introspection：对ERC165的简单实现，ERC165用于创建标准方法以发布和检测智能合约实现的接口。
lifecycle：一个用于管理合约及其资金的生命周期和行为的基础合约集合
math：对发生错误的操作进行安全检查的库。
mocks：主要用于单元测试的抽象合约集合。
ownership：一个用于管理合约以及TOKEN所有权的集合
payment：可以通过托管安排、取款、索赔管理支付相关的智能合约集合，支持单个收款人和多个收款人
proposals:对eip-1046的实现，EIP-1046对ERC20做了简单的扩展
token：一组ERC标准接口 主要是ERC20和ERC721
详解Access
Crowdsale
Examples
IntrospectionERC165:创建和发布一个标准方法、用于检测智能合约实现的接口
源码地址:https://github.com/ethereum/EIPs/blob/master/EIPS/eip-165.md
ERC165可以标准化以下内容如何识别接口
智能合约如何发布其实现的接口
如何检测智能合约是否实现了ERC165
如何检测智能合约是否实现了一个给定的接口
SupportsInterfaceWithLookup:查找支持的接口
LifecycleDestructible:销毁合约
Pausable：可暂停
TokenDestructible：销毁token
MathMath：简单的数学操作
SafeMath：带有安全检查的数学运算
Mocks
OwnershipRbacRBAC:Role-Based Access Control 角色控制
Roles:角色操作
CanReclaimToken：可回收TOKEN
Claimable：ownerable合约扩展，可用于转移合约所有权
Ownable：所有权操作
Contactable：设置合约信息
DelayedClaimable：Claimable合约扩展、限制pendingOwner只能在两个指定的区块编号之间完成所有权转移
HasNoContracts：合约所有者可收回合约的所有权
HasNoEther：合约锁定、阻止以太币转入、防止意外丢失
HasNoTokens：TOKEN回收
Superuser：超级用户：就算不是owner也可转移合约所有权
Heritable：可继承合约
PaymentConditionalEscrow：收款人取出资金的条件
Escrow：托管、余额的存取
PullPayment：存款的查询与取款、支持异步交易
RefundEscrow：可退款托管
SplitPayment：支持多个收款人按照自己所占权重/比例来提取资金
ProposalsERC721标准为不可替换的令牌引入了“tokenURI”参数来处理元数据，例如：缩略图、标题、描述、性能等等。这对于加密收藏品和游戏资产尤其重要。
TokenMetadata：TOKEN元数据(TokenURI设置与获取)

学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/337650?tuin=63946d38)
扫码获取海量视频及源码   QQ群：721929980
![](https://img-blog.csdnimg.cn/20181108132958856.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

