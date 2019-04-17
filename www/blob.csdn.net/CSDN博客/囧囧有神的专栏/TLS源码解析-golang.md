# TLS源码解析-golang - 囧囧有神的专栏 - CSDN博客





2018年03月24日 19:13:41[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：616标签：[tls 加密																[源码](https://so.csdn.net/so/search/s.do?q=源码&t=blog)](https://so.csdn.net/so/search/s.do?q=tls 加密&t=blog)
个人分类：[linux																[network](https://blog.csdn.net/liujiyong7/article/category/1098490)](https://blog.csdn.net/liujiyong7/article/category/1098485)











# 概述

以golang 1.8.1版本为准。

源码目录：src/crypto/tls

# RECORD协议


record类型




|```cppconst``````cpp(``````cpp``````cpprecordTypeChangeCipherSpec recordType = 20``````cpp// changecipherspec表明发送端已取得用以生成连接参数的足够信息。内容随密码套件不同``````cpp``````cpprecordTypeAlert            recordType = 21``````cpp// alert协议类型``````cpp``````cpprecordTypeHandshake        recordType = 22``````cpp// 握手协议类型``````cpp``````cpprecordTypeApplicationData  recordType = 23``````cpp// 应用数据协议类型``````cpp)```|
|----|





record head长5个字节

|--record type--|--version--|--data length--|




record type： 1个字节

version：        2个字节

data length：  2个字节  最大值16KB

一条record的长度是可变的， 参考： https://blog.csdn.net/liujiyong7/article/details/65632819





record是TLS 加解密的最小单元，至少要收到一个完整的record，才能加解密。

record 协议会执行以下动作

1. 分片，将应用层的数据进行分片

2. 生成序列号，防重放

3. 压缩， 可选，握手协议会进行协商，但是不建议压缩

4. 算HMAC， 计算数据HMAC，防止篡改，伪造

5. 发给tcp/ip,    下层协议，也可以是udp

# HANDSHAKE协议

第一次Read或Write方法会自动调用Handshake()方法

## 完整的握手

![](http://wiki.intra.xiaojukeji.com/download/attachments/110509120/tls.png?version=1&modificationDate=1504719378000&api=v2)



### ClientHello




|```cpptype clientHelloMsg``````cppstruct``````cpp{``````cpp``````cppraw                          []byte``````cpp// 原始数据``````cpp``````cppvers                         uint16``````cpp// 协议版本，指示客户端支持的最大协议版本``````cpp``````cpprandom                       []byte``````cpp// 随机数 32字节``````cpp``````cppsessionId                    []byte``````cpp// 会话ID，第一次为空，服务端借助会话ID恢复会话，需要服务器端缓存。``````cpp``````cppcipherSuites                 []uint16``````cpp// 客户端支持的加密套件列表``````cpp``````cppcompressionMethods           []uint8``````cpp// 客户端支持的压缩方法，默认为null``````cpp``````cppnextProtoNeg``````cppbool``````cpp// 扩展NPN 是否支持次协议协商``````cpp``````cppserverName                   string``````cpp// 扩展SNI 服务器名称，通常为域名，默认为目标地址主机名。支持SNI扩展需要的字段。``````cpp``````cppocspStapling``````cppbool``````cpp// 扩展status_request 是否支持ocsp staping。全称在线证书状态检查协议 (rfc6960)，用来向 CA 站点查询证书状态``````cpp``````cppscts``````cppbool``````cpp// 扩展SCT。是否支持SCT``````cpp``````cppsupportedCurves              []CurveID``````cpp// 扩展ellipic curve 列出支持的椭圆曲线名称  [https://tools.ietf.org/html/rfc4492#section-5.5.1](https://tools.ietf.org/html/rfc4492#section-5.5.1)``````cpp``````cppsupportedPoints              []uint8``````cpp// 扩展Point Formats 对椭圆曲线顶点进行可选压缩  [https://tools.ietf.org/html/rfc4492#section-5.5.2](https://tools.ietf.org/html/rfc4492#section-5.5.2) 默认不压缩``````cpp``````cppticketSupported``````cppbool``````cpp// 扩展Sessionticket.是否支持会话ticket``````cpp``````cppsessionTicket                []uint8``````cpp// 扩展Sessionticket 会话ticket，区别于sessionId的新的会话恢复机制，这种机制不需要服务器端缓存。``````cpp``````cppsignatureAndHashes           []signatureAndHash``````cpp// 扩展SignatureAlgorithms 签名和散列算法``````cpp``````cppsecureRenegotiation          []byte``````cpp// 扩展RenegotiationInfo 如果请求重新协商，就会发起一次新的握手。``````cpp``````cppsecureRenegotiationSupported``````cppbool``````cpp// 扩展RenegotiationInfo 是否支持renegotiation_info扩展 安全重新协商``````cpp``````cppalpnProtocols                []string``````cpp// 扩展ALPN 应用层协议协商。``````cpp}```|
|----|



**常用的扩展**

支持ECC需要两个扩展 ellipic curve，point formats



SNI扩展用来实现安全虚拟主机。单个服务器可能配有多个证书，服务端使用SNI来区分请求使用的是哪个证书。



SPDY使用NPN扩展协商使用何种应用层协议

HTTP2的协议协商过程使用ALPN扩展

ALPN是由客户端给服务器发送一个协议清单，由服务器来选择一个。NPN正好相反

**加密套件**

使用openssl ciphers -V | column -t 查看本机支持的cipher suite列表。如下所示：




|```java0xC0``````java,``````java0x30``````java-  ECDHE-RSA-AES256-GCM-SHA384    TLSv1.``````java2``````javaKx=ECDH        Au=RSA    Enc=AESGCM(``````java256``````java)    Mac=AEAD``````java0xC0``````java,``````java0x2C``````java-  ECDHE-ECDSA-AES256-GCM-SHA384  TLSv1.``````java2``````javaKx=ECDH        Au=ECDSA  Enc=AESGCM(``````java256``````java)    Mac=AEAD``````java0xC0``````java,``````java0x28``````java-  ECDHE-RSA-AES256-SHA384        TLSv1.``````java2``````javaKx=ECDH        Au=RSA    Enc=AES(``````java256``````java)       Mac=SHA384``````java0xC0``````java,``````java0x24``````java-  ECDHE-ECDSA-AES256-SHA384      TLSv1.``````java2``````javaKx=ECDH        Au=ECDSA  Enc=AES(``````java256``````java)       Mac=SHA384```|
|----|



Kx表示密钥交换算法 Au表示认证算法 Enc表示加密算法 Mac表示消息认证码算法

每一种cipher suite由一个uint16整数标示

golang的默认cipher suites (如果监测到AES-GSM硬件，会优先提供aes-gcm的加密套件)




|```javatopCipherSuites = []uint16{``````java``````javaTLS_ECDHE_RSA_WITH_CHACHA20_POLY1305,``````java``````javaTLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305,``````java``````javaTLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,``````java``````javaTLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,``````java``````javaTLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,``````java``````javaTLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,``````java}```|
|----|






**EC参数填充**
supportedCurves的值默认填充以下参数：var defaultCurvePreferences = []CurveID{X25519, CurveP256, CurveP384, CurveP521} // 23 24 25 29
[https://www.iana.org/assignments/tls-parameters/tls-parameters.xml#tls-parameters-8](https://www.iana.org/assignments/tls-parameters/tls-parameters.xml#tls-parameters-8) 从中可查到是secp256r1 secp384r1 secp521r1  x25519


supportedPoints 默认为0 不压缩
### ServerHello

消息的意义是将服务器选择的参数传回客户端，结构与clientHello相似，每个字段只包含一个选项




|```cpptype serverHelloMsg``````cppstruct``````cpp{``````cpp``````cppraw                          []byte``````cpp// 原始数据``````cpp``````cppvers                         uint16``````cpp// 服务端选择的版本号 通常为client server都支持的版本中最高的。 common.go:mutualVersion()``````cpp``````cpprandom                       []byte``````cpp// 服务端生成的随机数  32字节``````cpp``````cppsessionId                    []byte``````cpp//``````cpp``````cppcipherSuite                  uint16``````cpp// 服务端选择的加密套件  通常为client server都支持的套件中，最靠前的。所以套件的顺序是有讲究的。``````cpp``````cppcompressionMethod            uint8``````cpp// 选择的压缩方法 只会选择不压缩。如果客户端不支持不压缩，会报错``````cpp``````cppnextProtoNeg``````cppbool``````cpp// client支持，则server支持``````cpp``````cppnextProtos                   []string``````cpp// 服务端支持的应用层协议``````cpp``````cppocspStapling``````cppbool``````cpp//``````cpp``````cppscts                         [][]byte``````cpp// 签名的证书时间戳？``````cpp``````cppticketSupported``````cppbool``````cpp//``````cpp``````cppsecureRenegotiation          []byte``````cpp//``````cpp``````cppsecureRenegotiationSupported``````cppbool``````cpp// 与client一致``````cpp``````cppalpnProtocol                 string``````cpp// 服务端选择的应用层协议，client server都支持的协议中，最靠前的。如果client端为空，则填充为服务器支持的协议``````cpp}```|
|----|





根据ClientHello，填充ServerHello消息，主要的代码在 handshake_server.go:readClientHello()

EC参数填充




|```javapreferredCurves := c.config.curvePreferences()``````javafor``````java_, curve := range hs.clientHello.supportedCurves {``````java``````javafor``````java_, supported := range preferredCurves {``````java// 默认X25519, CurveP256, CurveP384, CurveP521，``````java``````javaif``````javasupported == curve {``````java``````javasupportedCurve =``````javatrue``````java``````javabreak``````javaCurves``````java``````java}``````java``````java}``````java}``````java......``````java``````javavar preferenceList, supportedList []uint16``````java// c.config.PreferServerCipherSuites本字段控制服务端是选择客户端最期望的密码组合还是服务端最期望的密码组合。``````java// 如果本字段为真，服务端会优先选择CipherSuites字段中靠前的密码组合使用。``````javaif``````javac.config.PreferServerCipherSuites {``````java``````javapreferenceList = c.config.cipherSuites()``````java``````javasupportedList = hs.clientHello.cipherSuites``````java}``````javaelse``````java{``````java``````javapreferenceList = hs.clientHello.cipherSuites``````java``````javasupportedList = c.config.cipherSuites()``````java}``````javafor``````java_, id := range preferenceList {``````java``````javaif``````javahs.setCipherSuite(id, supportedList, c.vers) {``````java//选择一个优先的ciphersuite``````java``````javabreak``````java``````java}``````java}``````java```|
|----|





### Certificate

可选。携带X.509证书链，证书链以ASN.1 DSR编码的一系列证书。主证书必须第一个发送，中间证书按照正确的顺序跟在后面，根证书需省略。

证书需与选择的算法套件一致。




|```cpptype certificateMsg``````cppstruct``````cpp{``````cpp``````cppraw          []byte``````cpp``````cppcertificates [][]byte``````cpp}```|
|----|



ServerKeyExchange

携带密钥交换的额外数据，消息内容对不同的协商算法套件存在差异。某些场景下不需要发送




|```cpptype serverKeyExchangeMsg``````cppstruct``````cpp{``````cpp``````cppraw []byte``````cpp// 对于ECDHE， 填充serverECDHParams``````cpp``````cppkey []byte``````cpp}```|
|----|










|```cppfunc (ka *ecdheKeyAgreement) generateServerKeyExchange(config *Config, cert *Certificate, clientHello *clientHelloMsg, hello *serverHelloMsg) (*serverKeyExchangeMsg, error) {``````cpp``````cpp``````cppif``````cppka.curveid == X25519 {``````cpp// 选择的椭圆曲线是X25519``````cpp``````cppvar scalar,``````cpppublic``````cpp[32]byte``````cpp``````cppif``````cpp_, err := io.ReadFull(config.``````cpprand``````cpp(), scalar[:]); err != nil {``````cpp``````cppreturn``````cppnil, err``````cpp``````cpp}``````cpp``````cppcurve25519.ScalarBaseMult(&``````cpppublic``````cpp, &scalar)``````cpp// 生成公钥 私钥``````cpp``````cppka.privateKey = scalar[:]``````cpp// 私钥自己保存``````cpp``````cppecdhePublic =``````cpppublic``````cpp[:]``````cpp// 公钥填充进ECDHParams参数``````cpp``````cpp}``````cppelse``````cpp{``````cpp// 选择其他的椭圆曲线``````cpp``````cppcurve, ok := curveForCurveID(ka.curveid)``````cpp``````cppif``````cpp!ok {``````cpp``````cppreturn``````cppnil, errors.New(``````cpp"tls: preferredCurves includes unsupported curve"``````cpp)``````cpp``````cpp}``````cpp``````cppvar x, y *big.Int``````cpp``````cppvar err error``````cpp``````cppka.privateKey, x, y, err = elliptic.GenerateKey(curve, config.``````cpprand``````cpp())``````cpp// 生成私钥 公钥对``````cpp``````cppif``````cpperr != nil {``````cpp``````cppreturn``````cppnil, err``````cpp``````cpp}``````cpp``````cppecdhePublic = elliptic.Marshal(curve, x, y)``````cpp``````cpp}``````cpp``````cpp``````cpp// [http://tools.ietf.org/html/rfc4492#section-5.4](http://tools.ietf.org/html/rfc4492#section-5.4)``````cpp``````cppserverECDHParams := make([]byte, 1+2+1+len(ecdhePublic))``````cpp// 计算长度，分配内存``````cpp``````cppserverECDHParams[0] = 3``````cpp// named curve                     // 选择的椭圆曲线``````cpp``````cppserverECDHParams[1] = byte(ka.curveid >> 8)``````cpp// 保存椭圆曲线高8位``````cpp``````cppserverECDHParams[2] = byte(ka.curveid)``````cpp// 保存椭圆曲线低8位``````cpp``````cppserverECDHParams[3] = byte(len(ecdhePublic))``````cpp// 公钥 ecdhePublic 长度``````cpp``````cppcopy(serverECDHParams[4:], ecdhePublic)``````cpp// 公钥``````cpp...``````cpp``````cppskx :=``````cppnew``````cpp(serverKeyExchangeMsg)``````cpp``````cppsigAndHashLen := 0``````cpp``````cppif``````cppka.version >= VersionTLS12 {``````cpp``````cppsigAndHashLen = 2``````cpp``````cpp}``````cpp``````cppskx.key = make([]byte, len(serverECDHParams)+sigAndHashLen+2+len(sig))``````cpp``````cppcopy(skx.key, serverECDHParams)``````cpp``````cppk := skx.key[len(serverECDHParams):]``````cpp``````cppif``````cppka.version >= VersionTLS12 {``````cpp``````cppk[0] = sigAndHash.hash``````cpp// hash``````cpp``````cppk[1] = sigAndHash.signature``````cpp// 签名``````cpp``````cppk = k[2:]``````cpp``````cpp}``````cpp``````cppk[0] = byte(len(sig) >> 8)``````cpp// 签名长度高8位``````cpp``````cppk[1] = byte(len(sig))``````cpp// 签名长度低8位``````cpp``````cppcopy(k[2:], sig)``````cpp// 签名``````cpp}```|
|----|





### ServerHelloDone

表明服务器已经将所有握手消息发送完毕




|```cpptype serverHelloDoneMsg``````cppstruct``````cpp{}```|
|----|




ClientKeyExchange

携带客户端为密钥交换提供的所有信息。内容随不同的密码套件会不同。




|```cpptype clientKeyExchangeMsg``````cppstruct``````cpp{``````cpp``````cppraw        []byte``````cpp``````cppciphertext []byte``````cpp}```|
|----|



Finished




|```cpptype finishedMsg``````cppstruct``````cpp{``````cpp``````cppraw        []byte``````cpp``````cppverifyData []byte``````cpp}```|
|----|





## 客户端身份验证

### CertificateRequest

服务器使用本消息对客户端进行身份认证




|```cpptype certificateRequestMsg``````cppstruct``````cpp{``````cpp``````cppraw []byte``````cpp``````cpp// hasSignatureAndHash indicates whether this message includes a list``````cpp``````cpp// of signature and hash functions. This change was introduced with TLS``````cpp``````cpp// 1.2.``````cpp``````cpphasSignatureAndHash``````cppbool``````cpp``````cppcertificateTypes       []byte``````cpp``````cppsignatureAndHashes     []signatureAndHash``````cpp``````cppcertificateAuthorities [][]byte``````cpp}```|
|----|





### CertificateVerify

客户端使用本消息证明自己的私钥与之前发送的客户端证书中的公钥对应




|```cpptype certificateVerifyMsg``````cppstruct``````cpp{``````cpp``````cppraw                 []byte``````cpp``````cpphasSignatureAndHash``````cppbool``````cpp``````cppsignatureAndHash    signatureAndHash``````cpp``````cppsignature           []byte``````cpp}```|
|----|





# 密钥交换

## ECDHE

Elliptic curve Diffie-Hellman 椭圆曲线密钥交换，如名称所示，基于椭圆曲线加密。

### 数学原理

安全性建立在离散对数计算很困难的基础上

**Diffie-Hellman**

两个基本概念：
- 本原根：如果整数 a 是素数 p 的本原根，则 a, a^2, …, a^(p-1) 在 mod p 下都不相同。
- 离散对数：对任意整数 b 和素数 p 的本原根 a，存在唯一的指数 i 满足: b ≡ a^i mod p (0≤i≤p-1)

则称 i 是 b 的以 a 为底的模 p 的离散对数

a,p是公开的，已知i，求b比较容易。已知b，求i非常困难



示例：

假设 client 和 server 需要协商密钥，p=2579，则本原根 a = 2。

1，  Client 选择随机数 Kc = 123 做为自己的私钥，计算 Yc = a^Kc  mod p = 2^123 mod 2579 = 2400，把 Yc 作为公钥发送给 server。

2，  Server 选择随机数 Ks = 293 作为私钥，计算 Ys = a^Ks  mod p = s^293 mod 2579 = 968，把 Ys 作为公钥发送给 client。

3，  Client 计算共享密钥：secrect =  Ys^Kc mod (p) = 968^123  mod(2579) = 434

4，  Server 计算共享密钥：secrect = Yc^Ks mod(p) =2400^293 mod(2579) =434

上述公式中的 Ys,Yc,P, a, 都是公开信息，可以被中间者查看，只有 Ks,Kc 作为私钥没有公开，当私钥较小时，通过穷举攻击能够计算出共享密钥，但是当私钥非常大时，穷举攻击肯定是不可行的。



**ECC**

**[http://www.8btc.com/introduction](http://www.8btc.com/introduction)**

椭圆曲线简化定义为 y^2 = x^3+ax+b 

![](http://wiki.intra.xiaojukeji.com/download/attachments/110509120/elli.png?version=1&modificationDate=1504754972000&api=v2)

观察以上椭圆曲线图中的四个点，定义一个阿贝尔群

• 群中的元素是一条椭圆曲线上的点;

• 单位元为无穷远点O;

• 点P的逆元是其关于x-轴的对称点;

• 加法，满足以下规则: 对于3个处在同一直线上的非零点 P, Q 和 R, 它们的和 P + Q + R = 0.





![](http://wiki.intra.xiaojukeji.com/download/attachments/110509120/ecc.jpeg?version=1&modificationDate=1504755624000&api=v2)

考虑上图，在切线的情况下， P+Q+R =0,  切点P=Q, 所以P+P=-R

定义乘法

nP = P+P+P...+P n个P相加

简写为Q=nP

已知n,P求解Q，比较容易

已知Q,P,求解n，非常困难



加密使用的椭圆曲线公式 y^2modp=(x^3+ax+b)modp  

1.挑选基点 G = (x, y)，G 的阶为 n。n 为满足 nG = 0 的最小正整数。

2.Client 选择私钥 Kc (0 <Kc<n )，产生公钥 Yc =Kc *G

3.server 选择私钥 Ks 并产生公钥 Ys =Ks*G

4.client 计算共享密钥 K = Kc*Ys   ，server 端计算共享密钥 Ks*Yc ，这两者的结果是一样的

Kc*Ys=Kc*(Ks*G)=Ks*(Kc*G)=Ks*Yc




### 协商过程

tls利用两个扩展elliptic_curves, point_formats 实现EC功能

elliptic_curves在clientHello消息中列出支持的椭圆曲线名称

![](http://wiki.intra.xiaojukeji.com/download/attachments/110509120/ecdhe.jpg?version=1&modificationDate=1504723234000&api=v2)



1，  浏览器发送 client_hello，包含一个随机数 random1，同时需要有 2 个扩展：

a)      Elliptic_curves：客户端支持的曲线类型和有限域参数。现在使用最多的是 256 位的素数域，参数定义如上节所述。

b)      Ec_point_formats：支持的曲线点格式，默认都是 uncompressed。

2，  服务端回复 server_hello，包含一个随机数 random2 及 ECC 扩展。

3，  服务端回复 certificate，携带了证书公钥。

4，  服务端生成 ECDH 临时公钥，同时回复 server_key_exchange，包含三部分重要内容：

a)       ECC 相关的参数。

b)       ECDH 临时公钥。

c)       ECC 参数和公钥生成的签名值，用于客户端校验。

5，  浏览器接收 server_key_exchange 之后，使用证书公钥进行签名解密和校验，获取服务器端的 ECDH 临时公钥，生成会话所需要的共享密钥。

至此，浏览器端完成了密钥协商。

6，  浏览器生成 ECDH 临时公钥和 client_key_exchange 消息，跟 RSA 密钥协商不同的是，这个消息不需要加密了。

7，  服务器处理 client_key_exchang 消息，获取客户端 ECDH 临时公钥。

8，  服务器生成会话所需要的共享密钥。

9，  Server 端密钥协商过程结束。



GOLANG实现




|```cppfunc (hs *serverHandshakeState) establishKeys() error {``````cpp``````cppc := hs.c``````cpp``````cpp// 生成密钥协商需要的密钥，这6个值，在加密过程中会使用``````cpp``````cppclientMAC, serverMAC, clientKey, serverKey, clientIV, serverIV :=``````cpp``````cppkeysFromMasterSecret(c.vers, hs.suite, hs.masterSecret, hs.clientHello.random, hs.hello.random, hs.suite.macLen, hs.suite.keyLen, hs.suite.ivLen)``````cpp``````cppvar clientCipher, serverCipher interface{}``````cpp``````cppvar clientHash, serverHash macFunction``````cpp``````cppif``````cpphs.suite.aead == nil {``````cpp``````cppclientCipher = hs.suite.cipher(clientKey, clientIV,``````cpptrue``````cpp/* for reading */``````cpp)``````cpp``````cppclientHash = hs.suite.mac(c.vers, clientMAC)``````cpp``````cppserverCipher = hs.suite.cipher(serverKey, serverIV,``````cppfalse``````cpp/* not for reading */``````cpp)``````cpp``````cppserverHash = hs.suite.mac(c.vers, serverMAC)``````cpp``````cpp}``````cppelse``````cpp{``````cpp``````cppclientCipher = hs.suite.aead(clientKey, clientIV)``````cpp``````cppserverCipher = hs.suite.aead(serverKey, serverIV)``````cpp``````cpp}``````cpp``````cppc.in.prepareCipherSpec(c.vers, clientCipher, clientHash)``````cpp``````cppc.out.prepareCipherSpec(c.vers, serverCipher, serverHash)``````cpp``````cppreturn``````cppnil``````cpp}```|
|----|



c.vers                                                     tls version  handshake_server.go readClientHello() 142 协商最大的版本号

hs.suite                                                  加密套件 handshake_server.go readClientHello() 265
hs.masterSecret              主密钥  handshake_server.go doFullHandshake 498 hs.clientHello.random        clientHello传进来的随机数hs.hello.random              serverHello 随机数hs.suite.macLen              echde_ecdsa_aes128_gcm_sha256 0     macLen keyLen ivLen都在cipher_suite.go中定义hs.suite.keyLen              echde_ecdsa_aes128_gcm_sha256 16hs.suite.ivLen               echde_ecdsa_aes128_gcm_sha256 4


# ALERT协议

以简单的通知机制告知对端通信出现异常。通常会携带close_notify异常，在连接关闭时使用。



需要关闭从client往server发送ALERT，以避免server端过多timeout



# 加密

## AES128-GCM-SHA256

分组加密




|```cpptype serverHandshakeState``````cppstruct``````cpp{``````cpp``````cppc                     *Conn``````cpp// tls连接``````cpp``````cppclientHello           *clientHelloMsg``````cpp// ClientHello消息``````cpp``````cpphello                 *serverHelloMsg``````cpp// ServeHello 消息``````cpp``````cppsuite                 *cipherSuite``````cpp// 加密套件``````cpp``````cppellipticOk``````cppbool``````cpp//``````cpp``````cppecdsaOk``````cppbool``````cpp//``````cpp``````cpprsaDecryptOk``````cppbool``````cpp//``````cpp``````cpprsaSignOk``````cppbool``````cpp//``````cpp``````cppsessionState          *sessionState``````cpp//``````cpp``````cppfinishedHash          finishedHash``````cpp//``````cpp``````cppmasterSecret          []byte``````cpp// 主密钥``````cpp``````cppcertsFromClient       [][]byte``````cpp// 客户端证书``````cpp``````cppcert                  *Certificate``````cpp// 证书``````cpp``````cppcachedClientHelloInfo *ClientHelloInfo``````cpp//``````cpp}```|
|----|







## chacha20-poly1305

流加密






















# 参考资料

1.[https://github.com/WeMobileDev/article](https://github.com/WeMobileDev/article)

2.[https://blog.helong.info/blog/2015/09/07/tls-protocol-analysis-and-crypto-protocol-design/](https://blog.helong.info/blog/2015/09/07/tls-protocol-analysis-and-crypto-protocol-design/)

3.[http://www.vuln.cn/6521](http://www.vuln.cn/6521) TLS扩展

4.[http://blog.csdn.net/u010129119/article/details/54090814](http://blog.csdn.net/u010129119/article/details/54090814) tls 1.3规范

5.[https://tlswg.github.io/tls13-spec/draft-ietf-tls-tls13.html](https://tlswg.github.io/tls13-spec/draft-ietf-tls-tls13.html)  tls 1.3最新草案

7.https://tools.ietf.org/html/rfc5246   tls 1.2 rfc


8.[http://blog.csdn.net/t0mato_/article/details/53160772](http://blog.csdn.net/t0mato_/article/details/53160772) AES-GCM

9.[http://blog.csdn.net/fw0124/article/details/8472560](http://blog.csdn.net/fw0124/article/details/8472560) 分组加密的四种模式









