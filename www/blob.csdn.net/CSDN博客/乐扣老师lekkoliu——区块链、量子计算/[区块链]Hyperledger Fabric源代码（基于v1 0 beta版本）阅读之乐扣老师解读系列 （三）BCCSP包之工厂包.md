
# [区块链]Hyperledger Fabric源代码（基于v1.0 beta版本）阅读之乐扣老师解读系列 （三）BCCSP包之工厂包 - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

置顶2017年06月15日 13:21:30[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：3163所属专栏：[区块链](https://blog.csdn.net/column/details/20660.html)



## 核心包之BCCSP（区块链加密服务提供者）包之factory工厂包
首先，我们看其代码结构：
![这里写图片描述](https://img-blog.csdn.net/20170615110208804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHN0dG95/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170615110208804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHN0dG95/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
主要包含factory工厂包，mocks模拟测试包，加解密关联包pkcs11、signer签名包、sw加解密实现包以及工具包utils。
根据整个结构我们对本讲的内容做如下安排：
一，**factory工厂包**，主要描述了sw工厂的实现，pkcs11的工厂方法实现，同时还包含了一部分opts的实现。
二，**pkcs11实现包**，主要描述了对于标准算法的实现。
三，**signer签名包**，主要描述了签名者相关实现。
四，**sw实现包**，主要描述了可插拔式自定制算法的实现。
五，**utils工具包**，主要描述了其他相关工具的实现。
## 一，核心包之BCCSP（区块链加密服务提供者）包之factory工厂包
包内容简介：提供工厂模式支持，将来包括若干类型的 BCCSP 实现。
null：空实现，测试用；
software：软件实现；
HSM：PKCS11，基于高安全模块的实现。
**包中核心文件的内容及方法：**
核心文件1*factory.go:*
方法：BCCSPFactory interface
⭕️接口来获取BCCSP实例。
方法：GetBCCSP(name string) (bccsp.BCCSP, error)
⭕️根据输入【名字】来返回 BCCSP实例。
方法：initBCCSP(f BCCSPFactory, config *FactoryOpts) error
⭕️加载配置文件，如果格式或者内容有错，则报错，计入日志中并返回空。
————————————————————————————
核心文件2*nopkcs11.go:*
方法：InitFactories(config *FactoryOpts) error
⭕️InitFactories方法必须使用在工厂接口之前，并且可以接受配置文件为空或者一些默认配置。但如果defaultBCCSP没有被设置会报错。
方法：GetBCCSPFromOpts(config *FactoryOpts) (bccsp.BCCSP, error)
⭕️当一些可选的输入在配置文件中被正确读取的时候，GetBCCSPFromOpts能够生成BCCSP实例。
————————————————————————————
核心文件3*opts.go:*
方法：GetDefaultOpts() *FactoryOpts
⭕️为一些可选的设置提供实现，如生产者的名称SW、哈希家族参数SHA2、SecLevel安全层级256级。临时属性Ephemeral。
————————————————————————————
核心文件4*pkcs11.go:*
方法：InitFactories(config *FactoryOpts) error
⭕️InitFactories方法必须使用在工厂接口之前，并且可以接受配置文件为空或者一些默认配置。但如果defaultBCCSP没有被设置会报错。
方法：setFactories(config *FactoryOpts) error
⭕️主要是为了一些关键参数服务，读取参数后为空的来设定默认参数。包括ProviderName、SwOpts、initBCCSP、Pkcs11Opts等
方法：GetBCCSPFromOpts(config *FactoryOpts) (bccsp.BCCSP, error)
⭕️当一些可选的输入在配置文件中被正确读取的时候，GetBCCSPFromOpts能够生成BCCSP实例。
————————————————————————————
核心文件5*pkcs11factory.go:*
在这里公钥加密标准是PKCS\#11，整个母工厂是基于此标准的实现，因此名字设定为PKCS11（即PKCS11BasedFactoryName）。
而且整个PKCS11Factory是一个（HSM）基于分级存储管理的Bccsp 工厂实现。
方法：(f *PKCS11Factory) Get(config *FactoryOpts) (bccsp.BCCSP, error)
⭕️该方法主要是从加密标准着手，通过读取PKCS11Factory的配置文件config来生成工厂实例。在beta版本中，这里有个TODO特别进行了说明。大意是PKCS11是不需要密钥库（keystore）的，但目前还没有从PKCS11 BCCSP中拆分出去，所以这里留着待后续进行改进，因此代码实现中依然保留了一部分keystore的实现。
————————————————————————————
核心文件6*swfactory.go:*
通过可插拔式的软件算法实现，来实现的一个工厂。名字设定为SW（即SoftwareBasedFactoryName）
方法：(f *SWFactory) Get(config *FactoryOpts) (bccsp.BCCSP, error)
⭕️该方法的密钥库（keystore）是取决于swOpts中的配置信息来决定。前文配置的四个参数中Ephemeral为true时，通过NewDummyKeyStore来获取签文件方法。否则就是通过读取文件的方式来NewFileBasedKeyStore获取签名证书方法。
结构：SwOpts struct
⭕️这里主要是描述了swopts的参数构成，如有前文已经介绍的SecLevel安全等级、HashFamily哈希选型，还有一些可选的密钥库（keystore）参数如Ephemeral临时性参数、及上述介绍了的密钥库（keystore）方法FileKeystore与DummyKeystore。
结构： FileKeystoreOpts struct
⭕️该方法的结构体中主要是密钥库（keystore）的地址。
————————————————————————————
## 工厂包小结
整个工厂包可以看到更多还是关于配置的相关设置，同时考虑了default和例外的处理，同时对于初始化以及操作逻辑上做了特别的注释。
## 结束语
觉得好的朋友可以给我大赏一杯咖啡钱缓解通宵梳理环境的疲乏〜！
方式一：支付宝
![这里写图片描述](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
方式二：以太坊
![这里写图片描述](https://img-blog.csdn.net/2018083110244130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[
](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
