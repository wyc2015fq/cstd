
# 3.15 solidity ERC20, ERC721接口详解 - 尹成的技术博客 - CSDN博客

2018年11月08日 19:16:51[尹成](https://me.csdn.net/yincheng01)阅读数：94


**ERC20**
ERC20：TOKEN标准接口
ERC20最初是一种尝试，其目的是为了为以太坊上的各类token合约提供一个特征与接口的共同标准
ERC-20允许钱包显示数以百计不同token的账户余额;创建一个交易工具，只需要提供token的合约地址就可以将更多的token列入表中
ERC20标准接口允许以太坊上的任何TOKEN被其它的应用重用
方法：![](https://img-blog.csdnimg.cn/201811081914068.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)
事件
![](https://img-blog.csdnimg.cn/20181108191452486.png)
ERC-20代币发布实例源码地址：https://blog.csdn.net/pony_maggie/article/details/79588259
步骤编写ERC-20标准合约继承自ERC-20标准
定义相关变量与结构
构造函数：初始化
转账从msg.sender转账
从指定地址转账
获取余额
设置配额
获取配额
通过remix+metaMask发布选择 Injected Web3
选择需要布置的合约
初始化相关参数
点击Deploy、该步会连接上MetaMask(需要翻墙)
Submit等待部署提交成功之后点击添加token
转账测试进入 www.myetherwallet.com
点击”发送以太币/发送代币”
选择”MetaMask/Mist”,点击Connect to MetaMask
在代币余额处选择 “Add Custom Token”，写入自己发布的代币的合约地址并点击保存
选取目的地址、转账

ERC721:TOKEN标准接口(不可分隔资产)与ERC20相比，ERC721 是用于处理不可替换资产的另一种以太坊代币标准。可替换资产像是货币，它可以被其他任何平等单位替代，但诸如房屋、家具则属不可替换资产，它不能被替代，也不能被分割，这就是 ERC721 规格标准的主要效果
方法
![](https://img-blog.csdnimg.cn/20181108191527528.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)
事件
![](https://img-blog.csdnimg.cn/20181108191607321.png)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/337650?tuin=63946d38)
扫码获取海量视频及源码   QQ群：721929980
![](https://img-blog.csdnimg.cn/20181108132958856.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)


