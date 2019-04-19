# HTTPS（2）- keytool - 定义域 - CSDN博客
置顶2015年10月28日 18:02:12[定义域](https://me.csdn.net/u010278882)阅读数：1179
·# keytool 参考手册
> 
[http://docs.oracle.com/javase/6/docs/technotes/tools/windows/keytool.html](http://docs.oracle.com/javase/6/docs/technotes/tools/windows/keytool.html)
keytool 是一个JDK自带的秘钥和证书管理工具。
## 命令
keytool // 密钥和证书管理工具 
(keytool == keytool help)
-help  
    keytool -help 列出所有可以执行的命令 
    keytool *-command-name* -help 列出所有命令的执行参数
## 子命令
keytool [COMMAND]
[COMMAND] 
-list           列出密钥库中的条目 
-importkeystore 从其他密钥库导入一个或所有条目 
-delete         删除条目 
-changealias    更改条目别名 
-keypasswd      更改条目的密钥口令 
-storepasswd    更改密钥库的存储口令
-genseckey      生成密钥 
-genkeypair     生成密钥对
-printcert      打印证书内容 
-printcertreq   打印证书请求内容 
-printcrl       打印CRL文件内容 
-certreq        生成证书请求 
-gencert        根据证书请求生成证书 
-exportcert     导出证书 
-importcert     导入证书
> 
逻辑概念
keystore 即密钥库，用于存储密钥和证书。 
密钥库中存储的是密钥库实体(keystore entity)，密钥库实体有不同的类型，keytool工具使用的密钥库实体类型有两种：密钥实体(key entity)和受信证书实体(trusted certficate entity)，分别存放密钥信息和证书信息。密钥信息有两类：对称加密算法使用的密钥和非对称加密算法使用公/私钥。
> 
物理实现
keystore 的类型和数据存储格式与密钥服务提供者类型相关（参考 KeystoreSpi）。不同的密钥库类型是不相互兼容的。
JDK中提供了三种用户访问和管理密钥库的工具，两种命令行工具`keytool`和`jarsigner`和图形界面工具`Policy Tool`。 
三种工具对 keystore 的访问和管理能力不同。从能够操纵的密钥库的文件格式来看，keytool只能操作基于文件的密钥库，即密钥库是文件。在使用 keytool 处理密钥库时，在命令行中传入密钥库文件的路径，然后 keytool 通过FileInputStream 加载信息。jarsigner 和 Policy Tool 能够操作的数据格式更加广泛，只要提供 URL 作为密钥库资源路径即可。从能够操作的密钥库实体类型来看，keytool 能够操作密钥实体和受信证书实体，并且支持两种密钥实体。而 jarsigner 只支持公/私钥密钥实体。
// TODO KeystoreSpi KeyStore 为应用程序自定义安全密钥管理工具
上面的命令主要分为三类：用于操作密钥库实体的、操作密钥实体的和操作受信证书实体的。 
利用 -list -importkeystore -delete -changealias 可以用于查看密钥库中全部密钥库实体、导入密钥库实体、删除密钥库实体和改变密钥实体的名称，利用 -keypassword 和 -storepassword 可以分别用于更改密钥库实体和密钥库的访问密码。
> 
密钥库实体密码 vs. 密钥库密码 
  比如，前者对密钥实体加密，防止密钥被非法访问；后者，通过加密，保证密钥库的安全性。
第二组命令，-genseckey 和 -genkeypair 可分别用于生成对称加密算法加密的密钥和非对称加密算法加密的密钥对。 
第三组命令，用于操作证书。
> 
证书内容和格式
X.509是证书的标准，其规定了证书的内容和格式。具体分别使用 ASN.1/DER 标准定义。 
X.509 标注有三个版本，v1、v2、v3，目前广泛使用的是 96 年发布的 v3版，该版本添加了对证书内容的扩展特性（用户可以扩展证书的内容）和添加X.500可区分名特新（原先的标准只能添加一个X.500 distinguish name, dname，现在可以添加不只一个，可以添加 DNS、Email、IP作为X.500 dname）。
keytool 工具支持X.509标准，但是对于证书的导入和导出支持 v1~v3 三个版本，对证书的生成仅支持 v3 版，所以使用 keytool 生成的证书都是 v3版的。
证书的格式（编码）没有使用二进制编码，而是使用 RFC 1421 标注，即使用 Base64 这种有可打印码构成的编码。可以通过为 -printcert 子命令添加参数的方式，指定其是以何种方式打印证书内容或签名，通过 -list 子命令，默认打印证书的 MD5 指纹；通过添加参数 -rfc 将打印证书的Base64 编码的内容。打印内容在下面的分隔符中： 
—–BEGIN CERTIFICATE—– 
证书内容 Base64 
—–END CERTIFICATE—–
## 生成密钥对
keytool -genkeypair [OPTION] 
keytool -gensecpair [OPTION]
[OPTION] 
-alias *alias*      要处理的条目的别名 
-keyalg *keyalg*    密钥算法名称 
-keysize *keysize*  密钥位大小 
-keypass    *arg*       密钥口令
-sigalg * sigalg*   签名算法名称 
-destalias *destalias* 目标别名 
-dname *dname* 与alias相关的X.500可区分名 
-startdate *startdate*  证书有效期开始日期/时间 
-ext *ext*          X.509扩展 
-validity   *valDays*   有效天数
-keystore   *keystore*  密钥库名称（文件名） 
-storepass  *arg*       密钥库口令 
-storetype  *storetype* 密钥库类型
-providername   *providername* 提供方名称 
-providerclass  *providerclass* 提供方类名 
-providerarg    *arg*       提供方参数 
-providerpath   *pathlist*  提供方类路径
-v              详细输出 
-protected      通过受保护的机制的口令
生成密钥对（公钥和私钥），将公钥包装到一个X.509 v3自签名证书中，该证书以单一证书链的形式保存。证书链和私钥被保存到一个新的keystore entry中，通过别名访问。
> 
命令参数的默认值
-alias mykey 
-keyalg 当 -genkeypair 时，使用 DSA 
        当 -genseckey 时，使用 DES 
-keysize 当 -genkeypair 时，1024 
         当 -genseckey 且 -keyalg 是 DES 时，56 
         当 -genseckey 且 -keyalg 是 DESded 时，是128 
-sigalg 【需要确认的地方】对于-genkeypair，如果使用RSA算法，则实际使用MD5withRSA算法；如果使用DSA算法，则实际使用SHA1withDSA算法。 
-validatity 90天 
-keystore 默认是用户目录下的.keystore文件 
-storetype 从JVM的密钥库配置文件中取，JRE_HOME/lib/security/java.security属性文件的 keystore.type 属性，该属性默认值是jsk。因为操作该密钥库的服务提供者是Sun公司的JavaSecurityKeytore类，所以简称”jsk”（详见自定义KeyStore）。 
-proected false
> 
证书别名
密钥库中的密钥库实体只能通过别名进行引用，所以在密钥实体创建的过程中需要指定并记录别名。别名非常重要，因为在创建或导入密钥库实体后，所有操作均要通过别名引用。如果没有在命令行中指定别名，命令行会提示输入。【别名是否区分大小写？】可以通过 -changealias 来改变别名。
```
// 创建密钥并指定别名为 duke
keytool -genkeypair -alias duke -keypass dukepasswd
// 处于安全考虑，所以强制要求设置 storepass
// 密钥库使用的密码密码必须是不少于6个字符
// 由于创建非对称密钥，所以会生成用于存放公钥的证书，所以需要提供证书信息，即X.509规定的证书内容项
// 由于没有指定密钥库文件的存放路径（-keystore），因此使用了默认值，默认存放在用户主目录中并命名为.keystore
// 修改密钥库中别名为 duke 的密钥实体密码为 newpasswd
keytool -keypasswd -alias duke -keypass dukepasswd -new newpasswd
```
> 
证书内容
X.509 规定的证书内容包含如下项目，此外 v3 版的证书还可以扩展条目
- 版本号：该证书使用的 X.509 协议版本号
- 序列号：用于区分同一个签署人签署的不同证书，即同一个签署人签署的证书的唯一标识
- 加密算法：证书的加密算法，通常是 RSA
- 签署人：通常是颁发该证书的CA机构
- 提供人：即证书的创建人
- 有效期：开始和结束日期和时间
- 公钥：证书创建人的公钥
![支付宝证书](https://img-blog.csdn.net/20151028180108330)
> 
X.500 Distiguish Name
X.500 dname 用于标识证书的创建人，在创建证书时通过 -dname 与 -alias 绑定。 
dname 包含如下的内容，在创建 dname 时，填写的值要按照如下的顺序进行，但不要求提供全部字段的值。 
如果在创建证书时，比如创建公/私钥对时（默认创建公钥包装入证书中，所以会附带创建证书），没有指定 dname，命令行会给出提示让用户提供分别提供这些字段的值。
`CN=cnName,OU=ouName,O=oName,L=lName,S=sName,C=cName`
- Comman Name(CN):个人名称
- Organization Unit(OU):小组织名称，比如”Developemnt Dept”
- Organizaiton Name(O):大组织名称，比如”Paibo Software Co.Ltd”
- Location(L):区域名称，比如城市名
- State(S):省或州名
- Country(C):国家字母代码，两个字母构成，不区分大小写
```
// 这次创建上面的示例，但同时指定 dname
keytool -genkeypair -alias duke -dname CN="Js",OU="DD",O="PS",L="SH",S="SH",C="CN" -keypass dukepasswd -storepass admin1
// 如果 dname 中包含逗号(,)，则要使用转移字符对其转义\,
keytool -genkeypair -alias duke -dname CN="Js\," -keypass dukepasswd -storepass andmin1
```
> 
genseckey
因为公钥加密需要证书用于存放公钥，而对称加密不需要，所以，该指令除了没有证书相关的命令参数（第2组参数），其他参数的使用是相同的。
## 查看密钥库条目信息
```
// 查看密钥库信息
keytool -list -alias duke -keypass dukepasswd -keystore ./.keystore -storepass storepasswd
// 输出为证书的SHA1指纹
// duke, 2015-10-29, PrivateKeyEntry,
// 证书指纹 (SHA1): E9:A9:C2:91:7B:94:2C:58:E6:71:7B:C7:B7:C6:8F:6E:61:0F:B9:F5
// 查看证书的详细内容
keytool -list -alias duke -keypass dukepasswd -keystore ./.keystore -storepass storepasswd -v
// 使用 -v 查看更详细信息
// 别名: duke
// 创建日期: 2015-10-29
// 条目类型: PrivateKeyEntry
// 证书链长度: 1
// 证书[1]:
// 所有者: CN=Jasper, OU=Dev Dept, O=PS, L=Shanghai, ST=Shanghai, C=CN
// 发布者: CN=Jasper, OU=Dev Dept, O=PS, L=Shanghai, ST=Shanghai, C=CN
// 序列号: a7109d0
// 有效期开始日期: Thu Oct 29 09:33:07 CST 2015, 截止日期: Wed Jan 27 09:33:07 CST 2016
// 证书指纹:
//   MD5: 30:EA:58:8F:F3:10:97:BD:CC:5A:67:B1:4D:A6:4B:BB
//   SHA1: E9:A9:C2:91:7B:94:2C:58:E6:71:7B:C7:B7:C6:8F:6E:61:0F:B9:F5
//   SHA256: A9:E3:C6:6C:21:EA:01:DD:6F:E8:3D:D9:B4:05:A6:BE:81:F8:57:EB:C5:79:38:97:5A:93:18:08:F6:DB:D6:7C
//   签名算法名称: SHA1withDSA
//   版本: 3
// 
// 扩展: 
// 
// #1: ObjectId: 2.5.29.14 Criticality=false
// SubjectKeyIdentifier [
// KeyIdentifier [
// 0000: 51 67 60 05 61 39 1C CC   1F DB 58 55 D4 53 AC 0D  Qg`.a9....XU.S..
// 0010: 91 C6 93 E3                                        ....
// ]
// ]
// 查看证书内容
keytool -list -alias duke -keypass dukepasswd -keystore ./.keystore -storepass storepasswd -rfc
// 输出为证书的Base 64编码内容
// 别名：duke
// 创建日期：2015年10月29日
// 条目类型：PrivateKeyEntry
// 证书链长度：1
// 证书[1]:
// -----BEGIN CERTIFICATE-----
// MIIDJDCCAuKgAwIBAgIECnEJ0DALBgcqhkjOOAQDBQAwZDELMAkGA1UEBhMCQ04xETAPBgNVBAgT
// CFNoYW5naGFpMREwDwYDVQQHEwhTaGFuZ2hhaTELMAkGA1UEChMCUFMxETAPBgNVBAsTCERldiBE
// ZXB0MQ8wDQYDVQQDEwZKYXNwZXIwHhcNMTUxMDI5MDEzMzA3WhcNMTYwMTI3MDEzMzA3WjBkMQsw
// CQYDVQQGEwJDTjERMA8GA1UECBMIU2hhbmdoYWkxETAPBgNVBAcTCFNoYW5naGFpMQswCQYDVQQK
// EwJQUzERMA8GA1UECxMIRGV2IERlcHQxDzANBgNVBAMTBkphc3BlcjCCAbcwggEsBgcqhkjOOAQB
// MIIBHwKBgQD9f1OBHXUSKVLfSpwu7OTn9hG3UjzvRADDHj+AtlEmaUVdQCJR+1k9jVj6v8X1ujD2
// y5tVbNeBO4AdNG/yZmC3a5lQpaSfn+gEexAiwk+7qdf+t8Yb+DtX58aophUPBPuD9tPFHsMCNVQT
// WhaRMvZ1864rYdcq7/IiAxmd0UgBxwIVAJdgUI8VIwvMspK5gqLrhAvwWBz1AoGBAPfhoIXWmz3e
// y7yrXDa4V7l5lK+7+jrqgvlXTAs9B4JnUVlXjrrUWU/mcQcQgYC0SRZxI+hMKBYTt88JMozIpuE8
// FnqLVHyNKOCjrh4rs6Z1kW6jfwv6ITVi8ftiegEkO8yk8b6oUZCJqIPf4VrlnwaSi2ZegHtVJWQB
// TDv+z0kqA4GEAAKBgH7Lg3cY+90BR+qkZV5TLF+rZiayGdaIzncL8FLHh+YRxHUgki5j66dFzukZ
// LxpscKR4XtqMiQ/ZzC9aKw8frz0Rv239ehDxX+l8uOz0TZWj6iqFWvkwD07OEVnv5A4WpsSk2N77
// /ZMUvQTzfsTCe9KSpJ+6+VH+XegSfAJtgIBpoyEwHzAdBgNVHQ4EFgQUUWdgBWE5HMwf21hV1FOs
// DZHGk+MwCwYHKoZIzjgEAwUAAy8AMCwCFB5wa8LJSn/Tlhs5qb4KJZGbZDhOAhROQJw+241e59FZ
// ibL6YLAzan5v2w==
// -----END CERTIFICATE-----
```
## 创建一个CSR
CSR(Certificate Signing Request)，认证签名请求。请求文件扩展名为.csr。 
创建请求后将文件提交给CA，请他们帮着授权。申请通过后会返回被他们签名了证书。 
该证书可能是CA签署的证书或者证书链的形式。
全球主要的公共CA包括，VeriSign、Thawte、Entrust等。
```
keytool -certreq -alias duke -keypass dukepass -keystore ./.keystore -storepass storepasswd -file my.csr
// 将在当前目录下创建 my.csr 认证签名请求
```
申请通过后， CA会返回.cer文件，将该文件中的证书(链)导入keystore即可
`keytool -importcert -trustcacerts -file VeriSignReply.cer`
![Windows7中默认的根CA](https://img-blog.csdn.net/20151029101626826)
## 术语
- keystore 密钥库，用于存储密钥和证书信息；在 `keytool` 工具中采用文件形式存储。
- keystore entrites 密钥库实体，密钥库中信息的存在方式
- key entries 密钥实体，密钥库实体的一种，存储密钥信息
- trusted certficate entries 受信证书实体，密钥库实体的一种，存储证书信息
- keystore aliases 密钥库别名，密钥实体在加入到密钥库后只能通过密钥库别名访问
- certficate 证书/公钥证书，证书签署人（参考 issuer）为证书拥有者（参考 subject）签署的文件，其中包含证书拥有者的公钥信息
- issuer 证书签署人，提供证书认证的机构或组织，即CA
- subject 证书拥有者，需要被被服务人信任的组织或机构
- X.509 标准，定义证书的数据内容和格式，分别被 ASN.1/DER 标准定义
- X.500 可区名，subject 的标识信息，用于区分不同的证书拥有者
- RFC 1421，证书编码格式定义，证书使用 Base64编码
## 认识
### 证书的认证过程
- 环境：根CA信息被添加到系统中，比如JVM中的cacerts属性文件定义了JVM所信赖的CA
- 新CA 
- 组织或个人向CA申请认证，或者个人创建自签名证书
- CA申请通过返回被CA签名了证书
- 用户访问服务，获取特定组织的证书，验证证书链，如果信任，则认证成功 
或者用户添加自己的自签名证书的签名信息环境中
### 证书信任逻辑
我是用户，我有证书 
我相信证书的签署人（CA） 
证书的签署人对证书拥有者提供认证 
我就相信证书的拥有者
> 
下面的内容待整理（文档的部分译文）
# 待整理
-file 读取时使用标准输入，写入时使用标注输出 
-v 可以用于除 -help 选项之外的所有命令选项。如果使用 -v，表示keytool处于详细模式，将会有更多的信息输出。
## KeyStore
keystore是一个用于存储加密密钥和证书的存储媒介。
### KeyStore Entries(密钥库实体)
密钥库有不同的实体类型。keytool工具主要使用的有两种：
- Key Entries（密钥实体）
每个实体包含了非常敏感的加密密钥信息，这些信息以一种受保护的格式来保存以防止非授权访问。 
最常见的情况时，保存在该类型密钥库实体中的密钥是对称加密的密钥或非对称加密的私钥（对应着证书链中相应的公钥）。 
keytool工具能够处理这两种类型的密钥库实体，但是jarsigner工具只能够处理非对称密钥中的私钥的实体。
- Trusted certificate Entries(受信证书实体)
每个受信证书实体包含一个公钥证书。之所以称为”受信证书”，因为密钥库的拥有者信任该密钥库中的受信证书实体中的公钥。公钥表明了证书的拥有者是谁。而证书的签署人通过签署该证书保证了对证书拥有者的信任。
### KeyStroe Aliases(密钥库别名)
所有的密钥库实体都是通过在密钥库中唯一的别名来标识的。
别名在想密钥库添加密钥实体的过程中指定，通过使用 -genseckey 命令用于生成对称加密密钥，或者使用 -genkeypair 用于生成非对称密钥对（公钥和私钥），或者通过 -importcert 命令添加一个证书或者一个证书链到信任证书列表中。keytool工具的子命令也必须使用该别名来访问密钥实体。
比如，生成一个公钥/私钥对，为其起别名为duke，同时将公钥包装到自签名证书中：
keytool -genkeypair -alias duke -keypass dukekeypasswd
这里为私钥指定了密钥密码为 dukekeypasswd，如果只有要访问该私钥需要通过其别名 duke来进行。如果想值修修改别名为 duke的私钥的密钥密码，需要使用功能如下的命令：
keytool -keypasswd -alias duke -keypass dukekeypasswd -new newpass
这里将私钥的密钥密码改为 newpass.
> 
注意：除非在你认为是安全的系统山，否则不要在命令航中输入密钥密码或者通过在脚本中使用密钥密码。
### KeyStore实现
可以通过 java.security.KeyStore 来访问和修改密钥库中的信息。其是接口，需要实现。可以通过提供具体的实现来实现访问不同类型的密钥库。
JDK中自带了的两个命令行工具 keytool 和 jarsign 以及一个图形界面工具 Policy Tool实现了该接口。 
此外还有一个Sun公司提供的JDK内建的KeyStore实现。它将密钥库实现为一个文件，使用私有的密钥库类型（格式）称为”JKS”。 
这种格式中，每个私钥有自己的独立密钥面貌，同时通过为密钥库指定一个密钥库密码来保证密钥库实体的完整性。
KeyStore的实现是基于提供者的。具体来说，应用程序通过实现一个”服务提供接口”来支持KeyStore。在java.security包中存在一个抽象类 KeytstoreSpi，其定义了服务提供者接口方法，提供者需要实现该接口的定义（提供者只的是一个包或者一组包，其实现了具体服务的子集，这些服务能够被Java 安全API访问）。因此，为了提供一个KeyStore的实现，必须还要实现一个提供者，以及一个KeyStoreSpi的子类实现，参考《如何为Java加密架构实现一个提供者》
应用程序可以根据不同的提供者类型选择不同的KeyStore实现类型，通过getInstance… 
KeyStore定义了密钥库信息的数据格式，用于保护私钥/密钥的算法以及密钥库自身完整性的方法。不同类型的KeyStore实现是不兼容的。
keytool 工具对所有基于文件的KeyStroe实现都可以正常工作。该工具将通过命令行中传入的参数作为keystore文件的路径，并将其转换成一个文件输入流，加载密钥库的信息。 而 jarsigner 和 policytool 工具，能够从任意位置（通过URL指定）读取密钥库。
keytool 和 jarsigner 工具可以指定使用的密钥哭的类型，通过命令行（-storetype）。Policy Tool，工具可以在 KeyStore菜单中指定密钥哭的类型。
如果没有指定 -storetype那么工具会使用默认值，默认值在JRE\lib\security\keystore.type属性文件中定义，定义的属性是 java.security。工具在获取 keystore.type 值之后检查当前安全的全部可用服务提供者，知道找到一个实现了这种密钥库类型的提供者。然后就使用该提供者提供的密钥库实现。
KeyStore#getDefaultType可以获取keystore.type文件中的值。
KeyStore keyStore = KeyStore.getInstance(KeyStore.getDefaultType());
keystore.type默认值是 jks，Sun的实现。
如果你有一个提供者包，支持类型为”pkcs12”密钥库的密钥库实现，就可以更改keystore.type的属性值（不区分大小写）。
## 证书
证书也称为公钥证书，是签署主体（签署人）颁发的数字签名文件，也就是说每个签署人的公钥是不同的。
### 证书术语
- 
公钥 是与某个主体联系的数字序列，用来让主体被每个需要对主体信任的一方提供。公钥用于验证签名。
- 
数字签名 如果某些数据经过数字签名，那么这些数据中就带有了主体的标识信息，并且签名能够证明主体知道这些数据的存在。数据被主体的私钥签名。
- 
标识 一种标识主体的方式。在某些系统中标识是公钥，在另外一些系统中它可能是Unix UID、或者X.509 可区分名中使用的电子邮箱地址。
- 
签名 签名通过使用主体的私钥在数据上生成 
- 
私钥 数字序列，每个私钥仅被拥有该私钥的特定主体所致（也就是说，私钥是被安全保管的）。在所有的公钥加密体系中，私钥和公钥都是成对存在的。通常的公钥加密系统，比如DSA，一个私钥对应一个公钥。私钥用于计算签名。
- 
主体 主体是一个人、组织、程序、计算机、商业机构、银行，或者其他任何你能够在某种程序上信任的事物。主体是签署人，在证书的例子中也是证书的发布人。
- 
基本上来说，公钥加密体系需要用户能够访问公钥。在大规模网路环境下，不可能保证通信双方已经建立了安全的连接。也不能对所有公钥都信任。所以发明了证书，用于解决公钥的分发问题。现在，认证机构（CA）饰演了受信的第三方的角色。CA门是一些主体，比如，，一些商业主体，他们发对其他主体的证书进行签名。【！】假设CA们只创建经过验证的可信任的证书，因为验证的过程是需要遵循相关法律信息。常见的公共CA包括 VeriSign\Thawte\Entrus。当然，也可以使用来自微软认证服务器来提供你自己的认证。
试音keytool，可以显示、导入和导出帧数。还可以用于生成自签名的证书。
当前keytool工具兼容X.509证书。
### X.509认证
X.509标准定义了哪些信息能够成为认证，描述了如何记录认证（也就是认证的数据格式）。认证中的素有数据都是使用两种先关的标准进行编码的，这个标准叫做 ASN.1/DER。抽象语法标记1描述数据。定义编码规则描述存储和传输这类数据的方式，并且只有一种方式。 
所有的 X.509证书除了签名外，都包含如下的数据：
版本
表示使用的X.509是哪个版本的，不同版本的指定了可以指定的哪些信息。目前为止，有三个版本。keytool工具在导入和导出证书时可以使用v1,v2,v3版本。在生成证书时使用v3（生成的证书都是v3的）。
v1 1988年发布，应用最广泛
v2 引入subject和issuer 唯一标识的概念来处理subject或签署名重用的问题。大多数证书文档都强烈建议不要重用名称。v2 没有广泛使用
v3 1996 年发布，支持标注的扩展，这样任何人都能够定义扩展，并将其加入到帧数中。今天还在坏死用的通用扩展包括：keyUsage和AlternativeNames。前者限制了密钥的使用目的，比如，只能用于签名。后者其他的标识也能够被关联到公钥，比如 DNS名、哟想第一或者IP地址。
序列号
用于区分主体签署的其他证书。如果该序列号被废弃，那么就会从主体的认列表中移除。
签名算法
签署人名称
X.500可区分名 CA
有效期
主体名称 X.500 可区分名
主体公钥信息
### 证书链
自签名的证书的签署人和证书拥有者是同一方。 issuer(signne)==subject
只要新生成公/私钥，就会将同月放入自签名证书中。
之后，在证书签署请求生成(-certreq)后，并发往CA后，CA认证审核通过后，返回的响应将被导入(-importcert)到密钥库中，自签名的证书会被获得证书链替代。回复的证书的证书链的底部证书是被CA签署的主体公钥，下一层是CA的公钥。
证书的下一个认证机构是
### JDK认可的CA认证机构列表存储在 cacerts文件
JRE目录下的\lib\security\cacerts.properties文件(一个keystore文件)存储了JVM允许的CA证书。 
系统管理员可以对其进行配置(通过keytool命令)，同时指定 -storekey=”jks”。 
该文件保存了一些根CA证书，这些证书的别名和X.500可区分名如下所示：
```
*Alias: thawtepersonalfreemailca
 Owner DN: EmailAddress=person-freemail@thawte.com,
 CN=Thawte Personal Freemail CA,
 OU=Certification Service Division,
 O=Thawte Consulting,
 L=Cape Town,
 ST=Western Cape,
 C=ZA
```
完整列表参见文档
cacerts文件（是一个keystore文件），它的初始密钥库口令是”changit”。系统管理员应该对该密钥库的口令进行修改，在安装SDK之后。
### 互联网 RFC 1421 证书编码标准
证书不使用二进制编码，而是遵循 RFC1421标准使用可打印码来编码。 
使用这种证书的格式(也称作Base64 编码)为证书导出到其他应用程序提供了便利。
证书可以通过 -importcert 和 -printcert 指令来读取，读取时可以使用Base64编码，也可以使用二进制编码。 
-exportxcert 导出证书时，默认使用二进制编码；使用 -rfc 参数可以导出Base64编码格式的证书。
-list 默认会打印证书的 MD5 签名，如果同时使用 -v参数，打印内容将是可读的；如果使用 -rfc 参数，将会打印输出打印编码格式。
在打印码格式输出时，打印码的边界通过如下的形式进行限定：
—–BEGIN CERTIFICATE—– 
xxx 
—–END CERTIFICATE—–
### X.500可区分名
X.500 可区分名用于标识entity，…. 
keytool 支持下面的子端： 
* commonName            个人名称，比如”老王” 
* organizationUnit      小型组织/部门/分支机构的名称，比如”采购部” 
* organizationName      大型组织名称，比如”ABC有限公司” 
* localityName          城市名/地区名，比如”上海” 
* stateName             省名/州名，比如”加利福尼亚” 
* country               两个字母的国家大妈，比如“CN”，不区分大小写
使用 keytool -genkeypair -dname 来添加可区分名，比如按照如下的顺序提供子字段，但是可以不包含全部子字段。 
如果子字段的值包含逗号(,)，可以使用 , 来转义。
CN=Jasper, OU=”Development Department”, O=”Paibo Software Co.Ltd”, L=”Shanghai”, S=”Shanghai”, C=”CN”
通常情况下不强制要求提供 X.500 可区分名。但是如果使用的命令需要你提供而你没有体用的话，会有命令行提示让你输入，提示会分字段进行，每次提示一个子字段，然后你填写回车后提示输入下一个子字段，因此，在这种情况下，如果子字段值中包含逗号，则不再需要转移字符对逗号进行转义。
