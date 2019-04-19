# iOS 10 适配 ATS（app支持https通过App Store审核） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月12日 10:10:30[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2192
# iOS 10 适配 ATS
## 一. HTTPS
> 
其实HTTPS从最终的数据解析的角度，与HTTP没有任何的区别，HTTPS就是将HTTP协议数据包放到SSL/TSL层加密后，在TCP/IP层组成IP数据报去传输，以此保证传输数据的安全；而对于接收端，在SSL/TSL将接收的数据包解密之后，将数据传给HTTP协议层，就是普通的HTTP数据。HTTP和SSL/TSL都处于OSI模型的应用层。从HTTP切换到HTTPS是一个非常简单的过程，在做具体的切换操作之前，我们需要了解几个概念：
### SSL/TLS
为了保证这些隐私数据能加密传输，于是网景公司设计了SSL（Secure Sockets Layer）协议用于对HTTP协议传输的数据进行加密，从而就诞生了HTTPS。SSL目前的版本是3.0，被IETF（Internet Engineering Task Force）定义在RFC 6101中，之后IETF对SSL 3.0进行了升级，于是出现了TLS（Transport Layer Security） 1.0，定义在RFC 2246。实际上我们现在的HTTPS都是用的TLS协议，但是由于SSL出现的时间比较早，并且依旧被现在浏览器所支持，因此SSL依然是HTTPS的代名词，但无论是TLS还是SSL都是上个世纪的事情，SSL最后一个版本是3.0，今后TLS将会继承SSL优良血统继续为我们进行加密服务。
- 
[SSL/TLS协议运行机制的概述](http://www.ruanyifeng.com/blog/2014/02/ssl_tls.html)
- 
[图解SSL/TLS协议](http://www.ruanyifeng.com/blog/2014/09/illustration-ssl.html)
简单的来说，SSL/TSL通过四次握手。SSL协议的工作流程：
服务器认证阶段：
> 
客户端向服务器发送一个开始信息“Hello”以便开始一个新的会话连接；
服务器根据客户的信息确定是否需要生成新的主密钥，如需要则服务器在响应客户的“Hello”信息时将包含生成主密钥所需的信息；
客户根据收到的服务器响应信息，产生一个主密钥，并用服务器的公开密钥加密后传给服务器；
服务器恢复该主密钥，并返回给客户一个用主密钥认证的信息，以此让客户认证服务器。
用户认证阶段：
> 
在此之前，服务器已经通过了客户认证，这一阶段主要完成对客户的认证。
经认证的服务器发送一个提问给客户，客户则返回（数字）签名后的提问和其公开密钥，从而向服务器提供认证。
## 二. App Transport Security
iOS9中新增App Transport Security（简称ATS）特性, 主要使到原来请求的时候用到的HTTP，都转向TLS1.2协议进行传输。这也意味着所有的HTTP协议都强制使用了HTTPS协议进行传输。在 iOS 9 和 OS X 10.11 中，默认情况下非 HTTPS 的网络访问是被禁止的。当然，因为这样的推进影响面非常广，作为缓冲，我们可以在 Info.plist 中添加
`NSAppTransportSecurity` 字典并且将 `NSAllowsArbitraryLoads` 设置为
`YES` 来禁用 ATS。
不过，WWDC 16 中，Apple 表示将继续在 iOS 10 和 macOS 10.12 里收紧对普通 HTTP 的访问限制。从 2017 年 1 月 1 日起，所有的新提交 app 默认是不允许使用
`NSAllowsArbitraryLoads` 来绕过 ATS 限制的，也就是说，我们最好保证 app 的所有网络请求都是 HTTPS 加密的，否则可能会在应用审核时遇到麻烦。
## 三. iOS10 NSAppTransportSecurity
通过在info.plist中配置这个键，开发者可以自定义网络安全策略。例如：
允许针对个别服务器的不安全访问。
允许不安全的 web 或媒体内容访问，但不影响整个app的ATS策略。
启用新的安全特性，例如Certificate Transparency。
对NSAppTransportSecurity的支持自 iOS9.0，OS X v10.11 开始，适用于 app 和 app extension。
自 iOS10.0，macOS 10.12 开始，增加了对下列子键的支持：
- NSAllowsArbitraryLoadsInMedia
- NSAllowsArbitraryLoadsInWebContent
- NSRequiresCertificateTransparency
- NSAllowsLocalNetworking
### ATS Configuration Basics / ATS 配置基础知识
对于使用 iOS9.0， OS X v10.11 SDK 及以上的 app 来说，ATS（App Transport Security）默认开启，NSAllowsArbitraryLoads是字典NSAppTransportSecurity的根键，默认值NO。
在启用 ATS 的情况下，所有的 HTTP 请求必须为 HTTPS（RFC 2818） 连接。任何不安全的 HTTP 请求都将失败。ATS 使用 TLS（Transport Layer Security）v1.2（RFC 5246）。
下面是字典NSAppTransportSecurity的总体结构，所有键都是非必填项：
```
NSAppTransportSecurity : Dictionary {
    NSAllowsArbitraryLoads : Boolean
    NSAllowsArbitraryLoadsInMedia : Boolean
    NSAllowsArbitraryLoadsInWebContent : Boolean
    NSAllowsLocalNetworking : Boolean
    NSExceptionDomains : Dictionary {
        <domain-name-string> : Dictionary {
            NSIncludesSubdomains : Boolean
            NSExceptionAllowsInsecureHTTPLoads : Boolean
            NSExceptionMinimumTLSVersion : String
            NSExceptionRequiresForwardSecrecy : Boolean   // Default value is YES
            NSRequiresCertificateTransparency : Boolean
        }
    }
}
```
可以看出，所有键可以分为两类：主键，这些键用来定义 app 的总体 ATS 策略；子键，即NSExceptionDomains下面的键，使用这些键针对某个域名单独配置。
主键包括：
- 
NSAllowsArbitraryLoads
设置为 YES，解除整个 app 的 ATS 限制；但是，通过NSExceptionDomains进 行的配置依然有效。默认值为 NO。
注意：设置为 YES，会引发 App Stroe 的审查，开发者必须说明原因。
- 
NSAllowsArbitraryLoadsInMedia
设置为 YES，解除通过 AV Foundation 框架访问媒体内容时的 ATS 限制；启用这个 键，务必确保载入的媒体内容已经被加密，例如受FairPlay保护的文件，或者是安全的 HLS流媒，其中不包含敏感的个人信息。默认为 NO。
- 
NSAllowsArbitraryLoadsInWebContent
设置为 YES，解除通过 web view 发出的网络请求的 ATS 限制。启用这个键，可以使 app 访问任意网页内容，但不影响 app 的总体 ATS 策略。此键值默认为 NO。
- 
NSAllowsLocalNetworking
设置为 YES，使得 app 可以载入任意本地资源，但不影响 app 的总体 ATS 策略。默 认为 NO。
- 
NSExceptionDomains
为一个或多个域名单独配置 ATS。
被单独配置的域名，默认受到完全的 ATS 限制，不管NSAllowsArbitraryLoads的值 如何；需要通过子键，进一步配置
所有的子键都属于NSExceptionDomain。向Info.plist中添加这一主键：
- 创建字典，针对一个或多个域名，以便进行 ATS 配置。
- 这意味着之前使用主键所做的设置，对于这个域名来说，已经无效。
例如，及时之前设置NSAllowsArbitraryLoadsInMedia为 YES，然而NSExceptionDomain所代表的域名依然不能访问不安全的媒体内容。
基于这样的设定，可以针对域名进行 ATS 配置，增加或减少安全措施。例如：
- 将NSExceptionAllowsInsecureHTTPLoads设置为 YES，就 ；这样做会引发 App Store 的审查，详情见App Store Review for ATS。
- 通过配置NSExceptionRequiresForwardSecrecy为 NO，取消正向保密。
- 通过配置NSExceptionMinimumTLSVersion，更改 TLS 最低版本。
NSExceptionDomains字典构成：
- 
<域名字符串>
代表想要配置的特定域名。可以添加多个域名（即添加多个这样的键），为它们统一配置 ATS 策略。这个键对应一个字典，包含以下子键：
- NSIncludesSubdomains
`* 设置为 YES，当前域名的 ATS 策略适用于其所有子域名。默认为 NO。`
- NSExceptionAllowsInsecureHTTPLoads
```
* 设置为 YES，可以同时通过 HTTP 和 HTTPS 访问当前域名。默认为 NO。
注意，配置这个键值，将引发 App Store 的审查，开发者必须说明原因。
```
- NSExceptionMinimumTLSVersion
```
* 指定 TLS 的最低版本，因此可以使用版本较低，有安全漏洞的 TLS 协议。
注意，配置这个键值，将引发 App Store 的审查，开发者必须说明原因。
```
- NSExceptionRequiresForwardSecrecy
`* 设置为 NO，允许针对当前域名使用不支持正向保密的 TLS 加密算法。默认为 YES。`
- NSRequiresCertificateTransparency
`* 设置为 YES，将验证域名服务器证书的Certificate Transparency时间戳 。默认为 NO。`
### Requirements for Connecting Using ATS ／ 使用 ATS 的前提条件
在 ATS 完全开启的情况下，系统要求 app 的 HTTPS 连接必须满足以下要求：
X.509 数字证书必须满足下列标准中的一项：
- 
由操作系统内嵌的根证书颁发机构签发
- 由通过操作系统管理员或用户主动安装的根证书颁发机构签发
- TLS 版本必须为1.2，任何不使用或使用较低版本 TLS / SSL 的连接，都将失败。
- 
连接必须使用 AES-128 或 AES-256 对称加密算法。 TLS 算法套装必须以 ECDSA 密钥交换的形式支持正向保密，加密算法必须为下面之一：
- TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384
- TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
- TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384
- TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA
- TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
- TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA
- TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384
- TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256
- TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384
- TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256
- TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA
- 
服务端的叶证书签名密钥必须为下面之一：
- 至少2048位的 RSA 密钥
- 至少256位的 ECC 密钥
- 此外，服务器证书的哈希算法必须为 SHA-2，其摘要长度至少位256位（即 SHA-256 及以上）。
上面的标准，未来可能会发生变化。但不会影响到 app 二进制包的兼容性。
### App Store Review for ATS / App Store 对于 ATS 相关项的审核
某些对 ATS 的配置会引发 App Store 的审核，开发者必须说明原因。这些键有：
- NSAllowsArbitraryLoads
- NSExceptionAllowsInsecureHTTPLoads
- NSExceptionMinimumTLSVersion
以下是一些原因说明例子，供参考：
- 必须连接由其他机构控制的服务器，其还不支持安全连接。
- 必须支持那些还未升级至可使用安全连接，不得不通过公共域名访问网络的设备。
- 必须通过 web 展示来源不一的各种网络内容，但又不能完全使用NSAllowsArbitraryLoadsInWebContent所管理的类。
向 App Store 提交审核时，开发者应主动提供足够的信息，以便解释 app 无法使用安全连接的原因。
## 四. 实现支持安全ATS策略
<font color=red size=5>ATS相关设置对iOS8及以下系统无效</font>
### 需要解决的问题（iOS 9、iOS10及以上）
#### 1、app内服务器网络请求访问支持https（即一般的请求）
#### 2、webview内支持任意http访问
#### 3、第三方sdk接入与支持http访问
### 主要是围绕info.pilst配置文件作相关的安全ATS策略
Info.plist文件是向操作系统描述应用程序的XML属性列表,是iPhone应用程序文件夹包含所有重要的Info.plist文件
你可能注意到一些关键字像是使用了一些其他关键字中的词但是在前面加上了"ThirdParty"字样，在功能上，这些关键字与不含有"ThirdParty"的关键字有同样的效果。而且实际运行中所调用的代码将会完全忽略是否使用"ThirdParty"关键字。
简单粗暴的方案：
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
NSExceptionDomains 的设置方法如下, 比如我们要将 weibo.com 这个域名排除在 ATS 验证之外，就可以这样：
```xml
key>NSAppTransportSecurity</key>
<dict>
    <key>NSExceptionDomains</key>
    <dict>
        <key>weibo.com</key>
        <dict>
            <key> NSIncludesSubdomains </key>
            <true/>
            <key> NSExceptionRequiresForwardSecrecy </key>
            <false/>
            <key>NSExceptionAllowsInsecureHTTPLoads</key>
            <true/>
        </dict>
    </dict>
    <key>NSAllowsArbitraryLoads</key>
    <true/>
</dict>
```
注意：每个需添加的域都需要设置此三个属性。如果请求的网络图片是HTTP，也是需要设置的图片的域。
注意⚠️这个方案风险较大，有可能被拒绝。“需要访问的域名是第三方服务器，他们没有进行 HTTPS 对应”会是审核时的一个可选理由，但是这应该只需要针对特定域名，而非全面开放。如果访问的是自己的服务器的话，可能这个理由会无法通过。
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
### 实现方案
#### 1、app内服务器网络请求访问支持https
解决方案：
搭建https服务器
搭建https服务器需要ssl证书
- ssl自制证书：称自签名ssl证书，容易被假冒伪造，浏览器不信任。（审核不通过）
- 免费CA证书：部分CA机构提供免费的SSL证书，如wosign，starts等（App Store pass掉不通过）
- 付费CA证书：多指企业级及以上的数字证书。
HTTPS服务器满足ATS默认的条件，而且SSL证书是通过权威的CA机构认证过的，那么我们在使用Xcode开发的时候，对网络的适配什么都不用做，我们也能正常与服务器通信。但是，当我们对安全性有更高的要求时或者我们自建证书时，我们需要本地导入证书来进行验证。
使用AFNetworking来支持https
AFNetworking是iOS/OSX开发最流行的第三方开源库之一,现在iOS oc 代码90%以上都是用这个框架网络请求。AFNetworking已经将上面的逻辑代码封装好，甚至更完善，在AFSecurityPolicy文件中，有兴趣可以阅读这个模块的代码；以下就是在AFNetworking 2.6.0以前版本和3.0.0版本基于支持https的验证方式
步骤：
- 新建一个manager
- 在mainBundle中寻找我们刚才拖进项目中的https.cer, 并且将相关的数据读取出来
- 新建一个AFSecurityPolicy，并进行相应的配置
- 将这个AFSecurityPolicy 实例赋值给manager
代码实现：
```
NSURL * url = [NSURL URLWithString:@"https://www.google.com"];
AFHTTPRequestOperationManager * requestOperationManager = [[AFHTTPRequestOperationManager alloc] initWithBaseURL:url];
dispatch_queue_t requestQueue = dispatch_create_serial_queue_for_name("kRequestCompletionQueue");
requestOperationManager.completionQueue = requestQueue;
AFSecurityPolicy * securityPolicy = [AFSecurityPolicy policyWithPinningMode:AFSSLPinningModeCertificate];
//allowInvalidCertificates 是否允许无效证书（也就是自建的证书），默认为NO
//如果是需要验证自建证书，需要设置为YES
securityPolicy.allowInvalidCertificates = YES;
//validatesDomainName 是否需要验证域名，默认为YES；
//假如证书的域名与你请求的域名不一致，需把该项设置为NO；如设成NO的话，即服务器使用其他可信任机构颁发的证书，也可以建立连接，这个非常危险，建议打开。
//置为NO，主要用于这种情况：客户端请求的是子域名，而证书上的是另外一个域名。因为SSL证书上的域名是独立的，假如证书上注册的域名是www.google.com，那么mail.google.com是无法验证通过的；当然，有钱可以注册通配符的域名*.google.com，但这个还是比较贵的。
//如置为NO，建议自己添加对应域名的校验逻辑。
securityPolicy.validatesDomainName = YES;
//validatesCertificateChain 是否验证整个证书链，默认为YES
//设置为YES，会将服务器返回的Trust Object上的证书链与本地导入的证书进行对比，这就意味着，假如你的证书链是这样的：
//GeoTrust Global CA 
//    Google Internet Authority G2
//        *.google.com
//那么，除了导入*.google.com之外，还需要导入证书链上所有的CA证书（GeoTrust Global CA, Google Internet Authority G2）；
//如是自建证书的时候，可以设置为YES，增强安全性；假如是信任的CA所签发的证书，则建议关闭该验证，因为整个证书链一一比对是完全没有必要（请查看源代码）；
securityPolicy.validatesCertificateChain = NO;
requestOperationManager.securityPolicy = securityPolicy;
```
另afnetworking 3.0.0以上版本用的是AFHTTPSessionManager
```
AFHTTPSessionManager * manager = [AFHTTPSessionManager manager];
    NSString * cerPath = [[NSBundle mainBundle] pathForResource:@"server" ofType:@"cer"];
    NSData * cerData = [NSData dataWithContentsOfFile:cerPath];
    NSLog(@"%@", cerData);
    manager.securityPolicy = [AFSecurityPolicy policyWithPinningMode:AFSSLPinningModeCertificate withPinnedCertificates:[[NSArray alloc] initWithObjects:cerData, nil]];
    manager.securityPolicy.allowInvalidCertificates = YES;
    [manager.securityPolicy setValidatesDomainName:NO];
    manager.requestSerializer = [AFJSONRequestSerializer serializer];
    manager.responseSerializer = [AFJSONResponseSerializer serializer];
    NSDictionary * parameter = @{@"username":self.username, @"password":self.password};
    [manager POST:@"https://192.168.1.4:9777" parameters:parameter success:^(NSURLSessionDataTask * task, id responseObject) {
        NSLog(@"success %@", responseObject);
        }
        failure:^(NSURLSessionDataTask * task, NSError * error) {
            NSLog(@"failure %@", error);
        }]
```
```xml
<key>NSAppTransportSecurity</key>
    <dict>
        <key>NSAllowsArbitraryLoads</key> //设置为 YES，解除整个app的ATS限制；但是通过NSExceptionDomains进行的配置依然有效
        <false/>
        <key>NSAllowsArbitraryLoadsInMedia</key> //设置为 YES，解除通过AVFoundation框架访问媒体内容时的 ATS 限制
        <true/>
        <key>NSAllowsArbitraryLoadsInWebContent</key> //设置为 YES，解除通过webview发出的网络请求的ATS限制
        <true/>
        <key>NSAllowsLocalNetworking</key> //设置为 YES，使得app可以载入任意本地资源，但不影响app的总体 ATS 策略
        <true/>
```
#### 2、webview内支持任意http访问
对于网页浏览和视频播放的行为，iOS 10 中新加入了 NSAllowsArbitraryLoadsInWebContent 键。通过将它设置为 YES，可以让 app 中的 WKWebView 和使用 AVFoundation 播放的在线视频不受 ATS 的限制。这也应该是绝大多数使用了相关特性的 app 的选择。但是坏消息是这个键在 iOS 9 中并不会起作用。
如果app只支持 iOS 10，并且有用户可以自由输入网址进行浏览的功能，或者是在线视频音频播放功能的话，简单地加入 NSAllowsArbitraryLoadsInWebContent，并且将组件换成 WKWebKit 或者 AVFoundation 就可以了。如果你还需要支持 iOS 9，并且需要访问网页和视频的话，可能只能去开启 NSAllowsArbitraryLoads 然后提交时进行说明，并且看 Apple 审核员决定让不让通过了。
另外，当 NSAllowsArbitraryLoads 和 NSAllowsArbitraryLoadsInWebContent 同时存在时，根据系统不同，表现的行为也会不一样。简单说，iOS 9 只看 NSAllowsArbitraryLoads，而 iOS 10 会先看 NSAllowsArbitraryLoadsInWebContent。在 iOS 10 中，要是 NSAllowsArbitraryLoadsInWebContent 存在的话，就忽略掉 NSAllowsArbitraryLoads，如果它不存在，则遵循
 NSAllowsArbitraryLoads 的设定
UIWebView 在 NSAllowsArbitraryLoadsInWebContent 为 YES 时访问 HTTP，无效。WKWebView 在 NSAllowsArbitraryLoadsInWebContent 为 YES 时在iOS 10 中访问 HTTP，有效，iOS 9无效。如果用WkWebView替换UIWebView，iOS 7 将无法使用WkWebView，可做适配加载，没有特殊的什么需求的话，尽早将 UIWebView 全部换为 WkWebView 会比较好。所以为了能让WebView在所有版本都能访问非https内容，只能把NSAllowsArbitraryLoads设置为YES。
解决方案一：
开启 NSAllowsArbitraryLoads 为 YES，然后提交时进行说明
解决方案二：
设置 NSExceptionDomains 属性来访问指定域名，然后提交时进行说明
#### 3、第三方sdk接入与支持http访问
但是按照国内的现状，关闭这个限制也许是更实际的做法。至于原因就太多了，第三方SDK（几乎都是访问http），合作伙伴接入（不能要求它们一定要支持https）
第三方sdk，同样需要遵守ATS规则，即第三方sdk也有被ATS过滤的风险，微信，qq，分享，登陆功能都能正常，微博登陆不能正常通过。另在网上找到了一些可能存在有问题的sdk，目前已知的有：
- 友盟 （已经有最新的v1.4.0版本sdk，支持https，待验证）
- 百度地图 
解决方案一：
更新最新sdk，接入并测试
解决方案二：
可以设置 NSExceptionDomains属性来将需要排除强制验证的域名写进来：
## 五. 总结
开启 NSAllowsArbitraryLoads 为 YES
对第三方访问的服务器设置NSExceptionDomains方式添加白名单
提交审核说明：
- 
必须连接由其他机构控制的服务器，其还不支持安全连接。
- 
必须通过 web 展示来源不一的各种网络内容，但又不能完全使用NSAllowsArbitraryLoadsInWebContent所管理的类。
文／SN_Simon（简书作者）
原文链接：http://www.jianshu.com/p/36ddc5b009a7
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
