
# [区块链]Hyperledger Fabric源代码（基于v1.0 beta版本）阅读之乐扣老师解读系列 （四）BSSCP包之pkcs11加密包 - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

置顶2017年06月15日 16:17:30[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：2383所属专栏：[区块链](https://blog.csdn.net/column/details/20660.html)



## 二，核心包之BSSCP（区块链加密服务提供者）包之PKCS11加密包
包内容简介：主要提供了整个PKCS11实现过程的核心文件，配置文件conf，bccsp的实现代码、key生成、签名、验证等实现细节的代码。
## 核心文件1
## pkcs11.go
## :
主要是关于整个pkcs11的整个标准体系进行go代码的实现。
方法：loadLib(lib, pin, label string) (*pkcs11.Ctx, uint, *pkcs11.SessionHandle, error)
⭕️该方法主要是通过加载lib文件，来初始化数据。然后通过GetSlotList来解析slot数据，来通过GetTokenInfo获取token信息。另外，还可以通过pkcs11.SessionHandle方法来获取session
方法：(csp *impl) getSession() (session pkcs11.SessionHandle)
⭕️默认session信息在cache为空或者完全为使用状态的时候，通过`OpenSession(csp.slot, pkcs11.CKF_SERIAL_SESSION|pkcs11.CKF_RW_SESSION)`方法来获取session。
方法：(csp *impl) generateECKey(curve asn1.ObjectIdentifier, ephemeral bool) (ski []byte, pubKey *ecdsa.PublicKey, err error)
⭕️该方法主要是通过[]*pkcs11.Attribute来生成对应的公钥和私钥。
公钥pubkey_t主要是通过八个公钥参数参数来生成，私钥prvkey_t通过八个私钥参数参数来生成。然后通过CKA_ID来生成一个ski容器。之后可以通过SetAttributeValue，借助session来写入容器中。
方法： (csp *impl) signP11ECDSA(ski []byte, msg []byte) (R, S *big.Int, err error)
⭕️该方法主要描述了签名过程。首先通过ski来查找到私钥，然后通过一个组件（NewMechanism）以及私钥来进行一个签名的初始化工作（SignInit）。初始化完毕后，我们通过session信息对被签名信息msg来进行签名作业。签名结束后，通过len(sig)/2的信息处理来转换byte数据，最终拿到签名的int信息。
方法： (csp *impl) verifyP11ECDSA(ski []byte, msg []byte, R, S *big.Int, byteSize int) (valid bool, err error)
⭕️该方法主要描述了验证签名的过程。首先通过ski来查找到公钥，然后上述信息处理的反过程来获取签名信息。然后通过一个组件（NewMechanism）以及公钥来进行一个签名验证的初始化工作（VerifyInit），初始化完毕后，我们通过session信息对被签名信息msg来进行验证作业。签名的结果正误及错误信息来作为返回值。
方法： findKeyPairFromSKI(mod *pkcs11.Ctx, session pkcs11.SessionHandle, ski []byte, keyType bool) (*pkcs11.ObjectHandle, error)
⭕️该方法通过输入的Ctx信息、session信息及ski来获取对应的公私钥。
————————————————————————————
## 核心文件2**conf.go**:
配置文件中主要包含两部分内容，一部分是设置安全等级的三个层级实现。另一部分是两个结构体信息。
结构：PKCS11Opts struct
⭕️该方法包括三块，第一块是基本的信息设置安全层级SecLevel及哈希家族选型HashFamily。第二个是密钥库的选型参数Ephemeral、文件密钥库FileKeystore与Dummy文件密钥库DummyKeystore。第三个是PKCS11相关可选参数文件库Library、标签Label、Pin、Sensitive以及SoftVerify等。
方法：(conf *config) setSecurityLevel(securityLevel int, hashFamily string) (err error)
⭕️该方法主要是两个类型SHA2和SHA3。
方法：(conf *config) setSecurityLevel(securityLevel int, hashFamily string) (err error)
⭕️该方法主要是两个类型SHA2和SHA3。
方法：(conf *config) setSecurityLevelSHA2(level int) (err error)
⭕️该方法主要是两个等级选择‘256‘与‘384‘。两者差别主要是体现在ellipticCurve的‘oidNamedCurveP256‘与‘oidNamedCurveP384‘、hashFunction的‘sha256.New‘与‘sha512.New384‘、rsaBitLength的‘2048‘与‘3072‘上。
（PS：源码中setSecurityLevelSHA2方法和setSecurityLevelSHA3方法似乎实现是一样的，这里是bug还是说给后续做预留或者说是为了后续代码拓展而定义的SHA3，这里咱不明白。）
————————————————————————————
## 核心文件3**impl.go**:
该文件主要包含以下这些内容：进行BCCSP实例的方法**new**、通过配置opts生成key的方法**KeyGen**、通过一些配置的数据来使用BCCSP的**key**、通过适当参数进行key的导入方法**KeyImport**、通过csp和ski来获取key的方法**GetKey**，以及使用key来进行sign的实现方法**Sign**及验证方法**Verify**。
另外还有两个带有**TODO**的方法，其中一个是Encrypt与Decrypt，其备注内容提到PKCS11并不需要用到这个方法，当fabric需要此方法的时候会进行便携。而FindPKCS11Lib方法似乎因为还未完全解决配置文件碎片的问题导致该方法还未完成。
核心方法如下：
方法：New(opts PKCS11Opts, keyStore bccsp.KeyStore) (bccsp.BCCSP, error)
⭕️该方法首先初始化配置，设置SecLevel和HashFamily，然后构建sw。通过获取opts来读取lib、pin和label信息，通过这三者信息利用loadlib方法来获取ctx、slot与session。最终通过make和impl的方法获取到csp，即BCCSP的实例。
方法：(csp *impl) KeyGen(opts bccsp.KeyGenOpts) (k bccsp.Key, err error)
⭕️该方法主要是通过读取opts内部type的信息，根据以下三个种类来进行不同的作业。1，ECDSAKeyGenOpts。2，ECDSAP256KeyGenOpts。3，ECDSAP384KeyGenOpts。三者区别在于调用csp的generateECKey代码时，实用的参数不同，依次为：csp.conf.ellipticCurve、oidNamedCurveP256及oidNamedCurveP384。
方法： (csp *impl) KeyDeriv(k bccsp.Key, opts bccsp.KeyDerivOpts) (dk bccsp.Key, err error)
⭕️该方法根据传入参数的公私钥参数的不同来进行重新随机计算过的公私钥。（TODO）该方法比较复杂，可以在后续系列中详细进行解析。
后续其他方法基本是通过CSP的实现来给key使用，这里不再做重点讲解。
————————————————————————————
## 核心文件4**ecdsa.go**:
该文件主要是关于椭圆曲线算法相关的go实现，其包含以下内容：若干结构体和一些方法，包含：序列化椭圆曲线签名方法marshalECDSASignature、非序列化椭圆曲线算法unmarshalECDSASignature、椭圆曲线算法签名signECDSA及椭圆曲线算法验证verifyECDSA。
容器：curveHalfOrders map[elliptic.Curve]*big.Int
⭕️该容器是一个符合curveHalfOrders算法的实现承载容器，包含预计算的曲线组数量减半。它用于确保签名的S值低于或等于曲线组的次数减半，在这里只接受低S签名。由于效率原因，它们是预先计算的。
方法：marshalECDSASignature(r, s *big.Int) ([]byte, error)
⭕️该方法主要是通过调用asn1的序列化编码来实现。
方法：unmarshalECDSASignature(raw []byte) (*big.Int, *big.Int, error)
⭕️该方法主要是通过调用asn1的反序列化编码来实现。
方法：(csp *impl) signECDSA(k ecdsaPrivateKey, digest []byte, opts bccsp.SignerOpts) (signature []byte, err error)
⭕️该签名过程首先是通过ski和字节流digest进行csp的标准加密签名signP11ECDSA。然后通过low-S算法规则进行处理，最终返回序列化椭圆曲线算法签名。
方法：(csp *impl) verifyECDSA(k ecdsaPublicKey, signature, digest []byte, opts bccsp.SignerOpts) (valid bool, err error)
⭕️该验证过程首先是通过对签名信息进行反序列化椭圆曲线签名方法（unmarshalECDSASignature）调用，然后通过对low-S的算法处理，最终返回利用ski、字节流digest、签名参数r和s以及其他相关参数进行csp的验证处理verifyP11ECDSA的计算值。
————————————————————————————
## 核心文件5**ecdsa.go**:
该文件主要是对椭圆曲线相关计算的实现。
方法： (k *ecdsaPrivateKey) Bytes() (raw []byte, err error)
⭕️Bytes将此键转换为其字节表示，如果允许此操作。
方法： (k *ecdsaPrivateKey) SKI() (ski []byte)
⭕️SKI返回该键的主题标识符。
方法：(k *ecdsaPrivateKey) Symmetric() bool
⭕️如果此键是对称密钥，对称性返回true，如果此键不对称，则为false
方法：(k *ecdsaPrivateKey) Private() bool
⭕️如果此密钥是私钥，则私有返回true，否则为false。
方法：(k *ecdsaPrivateKey) PublicKey() (bccsp.Key, error)
⭕️PublicKey返回非对称的公钥/私钥对的相应公钥部分，此方法返回对称密钥方案中的错误。
方法：(k *ecdsaPublicKey) Bytes() (raw []byte, err error)
⭕️如果允许操作的话，字节将此密钥转换为其字节表示。其中用到了序列化公钥方法x509.MarshalPKIXPublicKey(k.pub)。
方法：(k *ecdsaPublicKey) SKI() (ski []byte)
⭕️SKI返回该键的主题标识符。
方法： (k *ecdsaPublicKey) Symmetric() bool
⭕️如果此键是对称密钥，对称性返回true，如果此键不对称，则为false。
方法： (k *ecdsaPublicKey) Private() bool
⭕️如果此密钥是私钥，则私有返回true，否则为false。
方法： (k *ecdsaPublicKey) PublicKey() (bccsp.Key, error)
⭕️PublicKey返回非对称的公钥/私钥对的相应公钥部分。此方法返回对称密钥方案中的错误。
————————————————————————————
## PKCS11加密包小结
整个加密包主要内容是PKCS11标准的实现及RSA椭圆曲线算法中以low-S算法为主导的go实现。同时也通过利用RSA的一些特性和算法，丰富了PKCS11加密实现体系。当然我们也看到了还有一些TODO有待我们在beta阶段进行观测和作业。
## 结束语
觉得好的朋友可以给我大赏一杯咖啡钱缓解通宵梳理环境的疲乏〜！
方式一：支付宝
![这里写图片描述](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
方式二：以太坊
![这里写图片描述](https://img-blog.csdn.net/2018083110244130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[
](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
