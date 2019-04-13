
# [区块链]Hyperledger Fabric源代码（基于v1.0 beta版本）阅读之乐扣老师解读系列 （五）BSSCP包之SW加密包 - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

置顶2017年06月16日 10:31:52[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：1829所属专栏：[区块链](https://blog.csdn.net/column/details/20660.html)



三，核心包之BSSCP（区块链加密服务提供者）包之SW加密包
包内容简介：整个ws包中根据文件类型来看，基本包含了一下几块：AES加密模块go文件、RSA加密模块go文件、椭圆曲线加密模块go文件、dummy签名文件模块go文件、基于既存文件地址的签名文件模块go文件、哈希模块go文件、签名模块go文件、验证模块go文件以及一些配置模块go文件、实现模块文件等等。
## 核心文件1
## impl.go
## :
主要是关于sw（可插拔式共同软件实现加密算法）代码的实现。其文件内主要包含了新建new方法、key生成器方法KeyGen、key的使用KeyDeriv方法、密钥导入方法KeyImport、获取key的方法GetKey、哈希算法Hash、获取哈希算法GetHash、签名方法Sign、验证方法Verify、加密方法Encrypt及解密方法Decrypt。整个过程的方法调用与前一讲相对类似，因此这里只关注核心方法或者差异较大方法。
方法： New(securityLevel int, hashFamily string, keyStore bccsp.KeyStore) (bccsp.BCCSP, error)
⭕️该方法首先初始化配置，设置SecLevel和HashFamily，然后进行各种初始化操作包括初始化config文件、检测密钥库（keystore）是否存在、设置加密模块encryptors、设置解密模块decryptors、设置签名调用者signers、设置签名验证者verifiers、设置哈希参数hashers、设置密钥生成器generators、设置密钥key的调用行为（源代码中这里的注释写错了应该是// Set the key keyDerivers）以及最后的设置密钥导入的部分。
方法： (csp *impl) Hash(msg []byte, opts bccsp.HashOpts) (digest []byte, err error)
⭕️该方法对比pkcs11包中，这里是有独立的方法，整个方法主要过程还是来自于调用csp的hashers，通过反射获取opts的可选参数type，然后再调用其hash方法，来获取digest的字节流信息。
方法： (csp *impl) Hash(msg []byte, opts bccsp.HashOpts) (digest []byte, err error)
⭕️该方法对比pkcs11包中，这里是有独立的方法，整个方法主要过程还是来自于调用csp的hashers，通过反射获取opts的可选参数type，然后再调用其hash方法，来获取digest的字节流信息。
方法：(csp *impl) Encrypt(k bccsp.Key, plaintext []byte, opts bccsp.EncrypterOpts) (ciphertext []byte, err error)
⭕️该方法加密使用密钥K加密明文。通过使用csp的encryptors来反射获取key的加密者。之后再用加密方法，通过密钥key、明文和可选参数进行加密操作得到密文。
方法：(csp *impl) Decrypt(k bccsp.Key, ciphertext []byte, opts bccsp.DecrypterOpts) (plaintext []byte, err error)
⭕️该方法用密钥K解密密文。通过使用csp的decryptors来反射获取key的解密者。之后再用解密方法，通过密钥key、密文ciphertext和可选参数进行解密获取明文。
————————————————————————————
## 核心文件2
## aes.go
## :
主要是关于sw（可插拔式共同软件实现加密算法）代码的aes算法实现。pkcs11中并不需要aes来实现，其只用到了椭圆曲线算法，因此aes可以理解下其如何通过go来实现。总体来看，该文件中主要包含了以下几个核心方法：通过给定长度参数来获取随机字节流方法GetRandomBytes、基于PKCS的256位标准填充模式方法pkcs7Padding、基于PKCS的256位标准反填充模式方法pkcs7UnPadding、CBC模式的加密方法aesCBCEncrypt、CBC模式的解密方法aesCBCDecrypt、结合了PKCS7与CBC模式的加密方法AESCBCPKCS7Encrypt、结合了PKCS7与CBC模式的解密方法AESCBCPKCS7Decrypt以及最终这个aes中采用结合了PKCS7与CBC模式的入口方法Encrypt与Decrypt。
都是现成的第三方提供包，调用关系相对简单，这里不再个别分析。
————————————————————————————
## 核心文件3
## rsa.go
## :
该文件主要是用来为RSA公钥加密算法进行实现的内容。其由签名方法sign和公钥签名验证及私钥签名验证Verify组成。
都是现成的第三方提供包，调用关系相对简单，这里不再个别分析。
————————————————————————————
## 核心文件4
## ecdsa.go
## :
该文件主要是对椭圆曲线相关计算的实现与上一讲的内容大致相同，因此这里也不再做
都是现成的第三方提供包，调用关系相对简单，这里不再个别分析。
————————————————————————————
## 核心文件5
## keyderiv.go
## :
## 核心文件6
## keyderiv.go
## :
## 核心文件7
## keyimport.go
## :
三个文件均与上一讲类似，但在结构上这里进行了拆分，毕竟考虑可插拔特性，这里独立出来。实现方式与上一讲中的没有太大差别。更多还是利用了椭圆曲线加密签名算法的特性（ecdsa），因此这里不再详细分析。
————————————————————————————
## 核心文件8
## dummyks.go
## :
newdummykeystore是一个实例化了的虚拟密钥库。该库实现了几种方法，包括：本身不能进行加载（load）或者存储（store）密钥容器、只读容器和可以进行读写的容器。
————————————————————————————
## 核心文件9
## fileks.go
## :
NewFileBasedKeyStore是一个实例化了的虚拟密钥存储库。该库与上述区别在于是可以读取现有的密钥库地址来加载。
结构：fileBasedKeyStore struct
⭕️该结构filebasedkeystore是一个基于文件夹的密钥库。每一个键都存储在一个分散的文件中，文件的名字包含密钥的SKI。其标志标识密钥类型的标志。所有的密钥都存储在初始化时提供路径的文件夹中。密钥库可以用密码初始化，同时这个密码也可以用于加密和解密存储密钥的文件。
密钥库为了避免键覆盖可以设置为只读。其包含参数地址path、只读设置readonly、是否开放isOpen、密钥字节pwd以及同步设置m。
方法：NewFileBasedKeyStore(pwd []byte, path string, readOnly bool) (bccsp.KeyStore, error)
⭕️该方法newfilebasedkeystore实例化在一个给定的位置读取一个基于文件的钥存储容器。如果其内容非空，则密钥存储库可以加密。
也可以将其设置为只读，在这种情况下，任何存储操作将被禁止。
方法：(ks *fileBasedKeyStore) Init(pwd []byte, path string, readOnly bool) error
⭕️该方法主要内容如下： init初始化需要通过密码和地址来进行生成文件夹操作，其用来存储在密钥，同时设置只读标记。每一个键都存储在一个分散的文件中，文件的名字包含密钥的SKI。和标识密钥类型的标志。
密钥库可以用密码初始化，同时这个密码也可以用于加密和解密存储密钥的文件。如果该密钥库不需要加密，那么pwd参数可以为空。如果密钥存储库未经密码初始化，那么从密钥库检索密钥会失败。密钥库为了避免键覆盖可以设置为只读。
方法： (ks *fileBasedKeyStore) GetKey(ski []byte) (k bccsp.Key, err error)
⭕️该方法主要是通过SKI来获取对应的key，这里有三个参数可以供选择‘key’、‘sk’、‘pk’，分别表示读取一个key字符串、读取一个私有key和读取一个公有key。
方法：(ks *fileBasedKeyStore) StoreKey(k bccsp.Key) (err error)
⭕️该方法的storekey 将密钥K存储在这个密钥库中。如果这个密钥库是只读的那么将存储失败。其中存储的类型分为通过椭圆曲线签名来存储私钥ecdsaPrivateKey、通过椭圆曲线签名来存储公钥ecdsaPublicKey、RSA存储私钥rsaPrivateKey、RSA存储公钥rsaPublicKey以及AES系列AES存储私钥aesPrivateKey、AES存储公钥aesPublicKey。
方法：(ks *fileBasedKeyStore) searchKeystoreForSKI(ski []byte) (k bccsp.Key, err error)
⭕️该方法主要是描述通过SKI来检索密钥，主要借助工具ioutil的ReadDir方法来读取文件地址(ks.path)，然后读取文件ReadFile，进而读取对应的私钥信息PEMtoPrivateKey。该私钥进行类型辨分为ecdsa或者rsa的类型，最终返回其密钥k。
方法：(ks *fileBasedKeyStore) createKeyStore() error
⭕️该方法在根目录不存在的情况下创建密钥库，主要是利用os的MkdirAll方法。
方法： (ks *fileBasedKeyStore) createKeyStoreIfNotExists() error
⭕️该方法与上述差别在于先判定在预设的目录下判定是否为空（利utils包的DirMissingOrEmpty方法），之后再利用ks的createKeyStore进行密钥库的创建。
方法：(ks *fileBasedKeyStore) createKeyStore() error
⭕️该方法在根目录不存在的情况下创建密钥库，主要是利用os的MkdirAll方法。
方法： (ks *fileBasedKeyStore) openKeyStore() error
⭕️该方法主要是利用ks的isOpen 方法进行对密钥库打开状态的判定。
其他的方法相对较简单且常规，这里不做分析。
————————————————————————————
SW加密包小结
整个加密包主要内容围绕加密界主流的技术如RSA、AES、ECDSA等成熟算法进行go语言实现。同时在这个包中充分体现了可插拔，对模块化进行了拆分。虽然大体功能和上一讲较类似，但本包的可取之处在于清晰的模块划分。当然其特点和PKCS11不同的是，对两种密钥库进行了详细的实现。
## 结束语
觉得好的朋友可以给我大赏一杯咖啡钱缓解通宵梳理环境的疲乏〜！
方式一：支付宝
![这里写图片描述](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
方式二：以太坊
![这里写图片描述](https://img-blog.csdn.net/2018083110244130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[
](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
