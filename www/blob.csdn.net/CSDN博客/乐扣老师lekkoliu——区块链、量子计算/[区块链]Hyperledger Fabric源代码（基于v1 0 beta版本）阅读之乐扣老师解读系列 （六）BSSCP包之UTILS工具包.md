
# [区块链]Hyperledger Fabric源代码（基于v1.0 beta版本）阅读之乐扣老师解读系列 （六）BSSCP包之UTILS工具包 - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

置顶2017年06月19日 11:45:05[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：1686所属专栏：[区块链](https://blog.csdn.net/column/details/20660.html)



四，核心包之BSSCP（区块链加密服务提供者）包之UTIL工具包
包内容简介：本包主要是和BCCSP（区块链加密服务提供者）相关的各核心文件的接口文件，主要是提供一些密码学相关的操作实现的借口，包含哈希、签名、验证及加解密等等。文件中是一些主要加密算法的结构接口描述，如AES、椭圆曲线签名算法、SHA哈希算法、RSA算法以及一些配置文件接口如密钥库配置、可选项配置等等。这些主要支持MSP的相关调用。
## 核心文件1 bccsp.go:
BCCSP 接口：主要是定义密码学相关操作，包括加解密、签名和验证、签名、Hash、Key 的生命周期管理等方法。 命名主要以ops为后缀进行。
## 核心文件2 aesopts.go:
AES 系列接口：AES 算法相关选项结构，主要包含三类key：128位key系列AES128KeyGenOpts操作、196位key系列AES192KeyGenOpts操作、256位key系列AES256KeyGenOpts操作。
## 核心文件3 ecdsaopts.go :
ECDSA 系列接口：ECDSA 算法相关选项结构，主要包含两类key：256位key系列ECDSAP256KeyGenOpts操作、384位key系列ECDSAP384KeyGenOpts操作。
## 核心文件4 hashopts.go :
HASH 系列接口：HASH 算法相关选项结构，主要包含两类四种key：256位key系列SHA256Opts操作、256位key系列SHA3_256Opts操作、384位key系列SHA384Opts操作、384位key系列SHA3_384Opts操作。
## 核心文件5 rsaopts.go :
HASH 系列接口：HASH 算法相关选项结构，主要包含四类key：1024位key系列RSA1024KeyGenOpts操作、2048位key系列RSA1024KeyGenOpts操作、3072位key系列RSA3072KeyGenOpts操作、4096位key系列RSA4096KeyGenOpts操作。
## 核心文件6 keystore.go :
密钥库相关接口：主要是用来定义 KeyStore 接口，存储加密秘钥。其主要三个方法接口：一个是存key、一个是取key、还一个是用来约束密钥库是否是只读状态。
## 核心文件7 opts.go :
设置接口：主要是对一些常量进行名称等相关设定，另外。
内置方法：Algorithm() string
⭕️大部分包含Algorithm() 算法方法的，基本都是规划到后面的版本才会用到（to be used)）。
内置方法：Ephemeral() bool
⭕️描述是否终止某个循环中的作业。当为false时候即终止，否则继续。
————————————————————————————
## UTILS工具包小结
整个加密包主要内容围绕上述实现的接口来进行设定，包含了几个主流的可插拔加密技术。
可以知道，在后面维护加密相关内容的时候，应该同时维护这边的接口状态信息，以保证信息的完整性。
## 结束语
觉得好的朋友可以给我大赏一杯咖啡钱缓解通宵梳理环境的疲乏〜！
方式一：支付宝
![这里写图片描述](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
方式二：以太坊
![这里写图片描述](https://img-blog.csdn.net/2018083110244130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[
](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
