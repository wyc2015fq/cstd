# iOS数据安全—加密解密「学习总结」 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年03月03日 17:23:51[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3106
**本文来自简书，原文地址:[http://www.jianshu.com/p/1a7d57b60778](http://www.jianshu.com/p/1a7d57b60778)**
写在前面
在写项目中，数据的安全性至关重要，而仅仅用 POST 请求提交用户的隐私数据，还是不能完全解决安全问题。因此：我们经常会用到加密技术，比如说在登录的时候，我们会先把密码用MD5加密再传输给服务器 或者 直接对所有的参数进行加密再POST到服务器。
记得最初接触加密，也不懂，就根据文档，用的是 md5，直接调一方法就OK 了（感觉加密也简单的）-->淡淡的一笑！
相信很多开发者跟我当初一样，只是根据项目经理或者文档指示进行加密，简单会使用 但并不知道加密知识体系和内在原理，于是整理了一份相关资料。时间有限，知识并未全覆盖，有遗漏或者错误，忘指正。
![](http://upload-images.jianshu.io/upload_images/2230763-87dc4b951c54aecc.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
 HTTPS
> 
目录:
- 数据安全介绍
- 常用加密算法
- 常用加密方式
- Base64编码方案
- 加密实现代码
5.1 MD5加密算法
5.2 对称加密算法AES和DES
5.3 非对称加密RSA
- HTTPS基本使用
- 数据安全–加密解密效果
- ~~加密实战应用场景~~（持续更新）
##### 1.数据安全介绍
- 
最基础的是我们发送网络请求时，使用`get`和`post`方式发送请求。两者具体区别就不做解释了，只是引出相关安全性问题
- `get`：将参数暴露在外，（绝对不安全-->明文请求或者傻瓜式请求）。
- `post`：将参数放到请求体body中，（相对于get比较安全-->但是我们可以很容易用一些软件截获请求数据。比如说`Charles（青花瓷）`）
- 
`Charles`（大部分app的数据来源都使用该工具来抓包，并做网络测试）
- 注意：Charles在使用中的乱码问题，可以显示包内容，然后打开info.plist文件，找到java目录下面的VMOptions，在后面添加一项：-Dfile.encoding=UTF-8
- 这里提供一个[青花瓷破解版](http://download.csdn.net/detail/dengw0716/9320153)下载途径，供大家学习使用，商务需求，也请[支持正版](https://www.charlesproxy.com/)。
- 
数据安全的原则
- 在网络上`不允许`传输用户隐私数据的`明文`,（即:App网络传输安全，指对数据从客户端传输到Server中间过程的加密，防止网络世界当中其他节点对数据的窃听）。
- 在本地`不允许`保存用户隐私数据的`明文`,（即:App数据存储安全，主要指在磁盘做数据持久化的时候所做的加密）。
- App`代码安全`,（即:包括代码混淆，加密或者app加壳）。
- 
要想非常安全的传输数据，建议使用https。抓包不可以，但是中间人攻击则有可能。建议双向验证防止中间人攻击，可以参考下文篇章。
##### 2.常用加密算法
|常用加密算法|名称|
|----|----|
|编码方案|Base64|
|哈希(散列)函数|MD5（消息摘要算法）|
||SHA1|
||SHA256|
|对称加密算法|DES|
||AES|
|非对称加密算法|RSA|
|HTTPS|HTTP+SSL协议|
##### 3.常用加密方式
5.要想非常安全的传输数据，建议使用https。抓包不可以，但是中间人攻击则有可能。建议双向验证防止中间人攻击
##### 4.Base64编码方案
- 
Base64简单说明
描述：Base64可以成为密码学的基石，非常重要。
特点：可以将任意的二进制数据进行Base64编码
结果：所有的数据都能被编码为并只用65个字符（A~Z a~z 0~9 + / =）就能表示的文本文件。
注意：对文件进行base64编码后文件数据的变化：编码后的数据~=编码前数据的4/3，会大1/3左右。
- 
Base64编码原理和处理过程
6、如果没有剩下任何数据，就什么都不要加，这样才可以保证资料还原的正确性。
在这里提供几张图结合上面的处理过程，好理解。
![](http://upload-images.jianshu.io/upload_images/2230763-803c71cbc7725637.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Base64 ASCII
- Base64实现代码
`简单方法直接拿走，调用`
```
// 对一个字符串进行base64编码,并且返回
-(NSString *)base64EncodeString:(NSString *)string {
    // 1.先转换为二进制数据
    NSData *data = [string dataUsingEncoding:NSUTF8StringEncoding];
    // 2.对二进制数据进行base64编码,完成之后返回字符串
    return [data base64EncodedStringWithOptions:0];
}
// 对base64编码之后的字符串解码,并且返回
-(NSString *)base64DecodeString:(NSString *)string {
    // 注意:该字符串是base64编码后的字符串
    // 1.转换为二进制数据(完成了解码的过程)
    NSData *data = [[NSData alloc]initWithBase64EncodedString:string options:0];
    // 2.把二进制数据在转换为字符串
    return [[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding];
}
//---------------------------<#我是分割线#>------------------------------//
NSLog(@"%@",[self base64EncodeString:@"A"]);
NSLog(@"%@",[self base64DecodeString:@"QQ=="]);
```
PS.终端执行编码和解码
```
如：
编码：base64 123.png -o 123.txt
解码：base64 123.txt -o test.png -D
```
##### 4.加密实现代码
哈希(散列)函数
特点：
- 算法是公开的
- 对相同的数据加密，得到的结果是一样的"
- 对不同的数据加密，得到的结果是定长的，MD5对不同的数据进行加密，得到的结果都是 32 个字符长度的字符串
- 信息摘要，信息"指纹"，是用来做数据识别的！
- 不能逆推反算(重要)
用途：
- 版权 对文件进行散列判断该文件是否是正版或原版的
- 文件完整性验证 对整个文件进行散列，比较散列值判断文件是否完整或被篡改
- 密码加密，服务器并不需要知道用户真实的密码！
- 搜索：
如：百度搜索-->老司机 皮皮虾 苍老师
或是 【苍老师 老司机 皮皮虾 】
上面两种方式搜索出来的内容是一样的
- 如何判断：对搜索的每个关键字进行三列，得到三个相对应的结果，按位相加结果如果是一样的，那搜索的内容就是一样的！
- 经典加密算法：`MD5、SHA1、SHA512`
###### MD5消息摘要算法
- 
简单介绍:
- MD5：全称是Message Digest Algorithm 5，译为“消息摘要算法第5版”(经MD2、MD3和MD4发展而来)
- 效果：对输入信息生成唯一的128位散列值（32个字符），即 32个16进制的数字。
- 
特点:
- 输入两个不同的明文不会得到相同的输出值
- 根据输出值，不能得到原始的明文，即其过程不可逆(只能加密, 不能解密)
- 
应用:
- 现在的MD5已不再是绝对安全(如：暴力破解的网站)，对此，可以对MD5稍作改进，以增加解密的难度。
- 解决：加盐（Salt）：在明文的固定位置插入随机串，然后再进行MD5（先加密，后乱序：先对明文进行MD5，然后对加密得到的MD5串的字符进行乱序）
- 
注意点:
- 开发中，一定要和后台开发人员约定好，MD5加密的位数是16位还是32位(大多数都是32位的)，16位的可以通过32位的转换得到。
- MD5加密区分 大小写，使用时要和后台约定好。
- MD5公认被破解不代表其可逆，而是一段字符串加密后的密文，可以通过强大运算计算出字符串加密后的密文对应的原始字符串，但也不是绝对的被破解。
- 
PS.暴力破解是指通过将明文和生成的密文进行配对，生成强大的数据库，在数据库中搜索，在这里就可以破解密码。[破解网址 http://www.cmd5.com](http://www.cmd5.com/)
提升MD5加密安全性，解决办法
- 1.先明文加盐，然后再进行MD5。即明文后拼接字符串（此时拼接的字符串要 足够长+足够咸+足够复杂），再进行MD5加密。如：
```
#define
 salt @"1342*&%&shlfhs390(*^^6R%@@KFGKF"
```
- 
2.先加密+乱序
- 
3.乱序|加盐，多次MD5加密等
- 4.使用消息认证机制HMAC：给定一个"秘钥"，对明文进行加密，并且做"两次散列"！-> 得到的结果，还是 32 个字符，相对安全（KEY是服务器传给你的，不是你写死的）。
消息认证机制（HMAC）简单说明
- 原理
- 消息的发送者和接收者有一个共享密钥
- 发送者使用共享密钥对消息加密计算得到MAC值（消息认证码）
- 消息接收者使用共享密钥对消息加密计算得到MAC值
- 比较两个MAC值是否一致
- 使用
- 客户端需要在发送的时候把（消息）+（消息·HMAC）一起发送给服务器
- 服务器接收到数据后，对拿到的消息用共享的KEY进行HMAC，比较是否一致，如果一致则信任
简单示例
```
#pragma mark - md5加密方法
- (NSString *)md5String {
    const char *str = self.UTF8String;
    uint8_t buffer[CC_MD5_DIGEST_LENGTH];
    CC_MD5(str, (CC_LONG)strlen(str), buffer);
    return [self stringFromBytes:buffer length:CC_MD5_DIGEST_LENGTH];
}
#pragma mark - HMACMD5加密方法
- (NSString *)hmacMD5StringWithKey:(NSString *)key {
    const char *keyData = key.UTF8String;
    const char *strData = self.UTF8String;
    uint8_t buffer[CC_MD5_DIGEST_LENGTH];
    CCHmac(kCCHmacAlgMD5, keyData, strlen(keyData), strData, strlen(strData), buffer);
    return [self stringFromBytes:buffer length:CC_MD5_DIGEST_LENGTH];
}
/**
 *  返回二进制 Bytes 流的字符串表示形式
 *  @param bytes  二进制 Bytes 数组
 *  @param length 数组长度
 *  @return 字符串表示形式
 */
- (NSString *)stringFromBytes:(uint8_t *)bytes length:(int)length {
    NSMutableString *strM = [NSMutableString string];
    for (int i = 0; i < length; i++) {
        [strM appendFormat:@"%02x", bytes[i]];
    }
    return [strM copy];
}
//---------------------------<#我是分割线#>------------------------------//
// md5加密调用
NSLog(@"%@",[@"520it" md5String]);
// (明文+加盐)MD5加密调用
NSLog(@"%@",[[@"520it" stringByAppendingString:salt] md5String]);
// hmacMD5加密调用（先加密+乱序）
NSLog(@"%@",[@"520it" hmacMD5StringWithKey:@"xiaomage"]);
```
###### 对称加密算法AES和DES
- 对称加密的特点
- 加密/解密使用相同的密钥
- 加密和解密的过程是可逆的
- 经典算法
- DES 数据加密标准
- AES 高级加密标准
- 提示：
- 加密过程是先加密，再base64编码
- 解密过程是先base64解码，再解密
简单示例
```
/**
 *  加密字符串并返回base64编码字符串
 *
 *  @param string    要加密的字符串
 *  @param keyString 加密密钥
 *  @param iv        初始化向量(8个字节)
 *
 *  @return 返回加密后的base64编码字符串
 */
- (NSString *)encryptString:(NSString *)string keyString:(NSString *)keyString iv:(NSData *)iv;
/**
 *  解密字符串
 *
 *  @param string    加密并base64编码后的字符串
 *  @param keyString 解密密钥
 *  @param iv        初始化向量(8个字节)
 *
 *  @return 返回解密后的字符串
 */
- (NSString *)decryptString:(NSString *)string keyString:(NSString *)keyString iv:(NSData *)iv;
// 调用
EncryptionTools *encrypt = [EncryptionTools sharedEncryptionTools];
NSLog(@"%@",[encrypt encryptString:@"LN123" keyString:@"LN" iv:nil]);
NSLog(@"%@",[encrypt decryptString:@"OPcTMDB5paivqtYo9Fj+hQ==" keyString:@"LN" iv:nil]);
```
###### 非对称加密RSA
- 非对称加密的特点
- 使用 公钥 加密，使用 私钥 解密
- 使用 私钥 加密，使用 公钥 解密（私钥签名，公钥验签）
- 公钥是公开的，私钥保密
- 加密处理安全，但是性能极差
- 经典算法-->RSA
简单示例
```
// 公钥加密时调用类方法：
+ (NSString *)encryptString:(NSString *)str publicKey:(NSString *)pubKey;
+ (NSData *)encryptData:(NSData *)data publicKey:(NSString *)pubKey;
// 私钥解密时调用类方法
+ (NSString *)decryptString:(NSString *)str privateKey:(NSString *)privKey;
+ (NSData *)decryptData:(NSData *)data privateKey:(NSString *)privacy;
/** 调用 */
NSString *str = [RSAUtil encryptString: @"LN" publicKey:RSA_Public_key];
NSLog(@"RSA公钥加密数据-->\n%@",str);
NSString *str1 = [RSAUtil decryptString:str privateKey:RSA_Privite_key];
NSLog(@"RSA私钥解密数据-->%@",str1);
```
MAC上生成公钥、私钥的方法，及使用
```
# MAC上生成公钥、私钥的方法
 @code
 1.打开终端，切换到自己想输出的文件夹下
 2.输入指令:openssl（openssl是生成各种秘钥的工具，mac已经嵌入)
 3.输入指令:genrsa -out rsa_private_key.pem 1024 (生成私钥，java端使用的)
 4.输入指令:rsa -in rsa_private_key.pem -out rsa_public_key.pem -pubout (生成公钥)
 5.输入指令:pkcs8 -topk8 -in rsa_private_key.pem -out pkcs8_rsa_private_key.pem -nocrypt(私钥转格式，在ios端使用私钥解密时用这个私钥)
 注意:在MAC上生成三个.pem格式的文件，一个公钥，两个私钥，都可以在终端通过指令vim xxx.pem 打开，里面是字符串，第三步生成的私钥是java端用来解密数据的，第五步转换格式的私钥iOS端可以用来调试公钥、私钥解密（因为私钥不留在客户端）
 iOS端公钥加密私钥解密、java端公钥加密私钥解密，java端私钥加密公钥解密都容易做到，iOS不能私钥加密公钥解密，只能用于验签
 @endcode
```
##### 5.HTTPS基本使用
- 
https简单说明
- HTTPS（
```
全称：Hyper
 Text Transfer Protocol over Secure Socket Layer
```
），是以安全为目标的HTTP通道，简单讲是HTTP的安全版。
- 在HTTP下加入`SSL层`，HTTPS的安全基础是SSL，因此加密的详细内容就需要SSL。
 它是一个URI scheme（抽象标识符体系），句法类同http:体系。用于安全的HTTP数据传输。
- HTTPS：URL表明它使用了HTTP，但HTTPS存在不同于HTTP的默认端口及一个加密/身份验证层（在HTTP与TCP之间）。
- 
注意
- HTTPS的主要思想是在不安全的网络上创建一安全信道，并可在使用适当的加密包和服务器证书可被验证且可被信任时，对窃听和中间人攻击提供合理的保护。
- HTTPS的信任继承基于预先安装在浏览器中的证书颁发机构（如VeriSign、Microsoft等）（意即“我信任证书颁发机构告诉我应该信任的”）。
- 因此，一个到某网站的HTTPS连接可被信任，如果服务器搭建自己的https 也就是说采用自认证的方式来建立https信道，这样一般在客户端是不被信任的。
- 所以我们一般在浏览器访问一些https站点的时候会有一个提示，问你是否继续。
- 
HTTPS和HTTP区别
- https协议需要到`ca申请证书`，一般免费证书很少，需要交费。
- http是超文本传输协议，信息是明文传输，https 则是具有安全性的`ssl加密传输协议`。
- http和https使用的是完全不同的连接方式，用的端口也不一样，`前者是80`，`后者是443`。
- http的连接很简单，是无状态的；HTTPS协议是由`SSL+HTTP协议`构建的可进行加密传输、身份认证的网络协议，比http协议安全。
- 
实现代码
方案一：如果是自己使用NSURLSession来封装网络请求
```
// 1.创建session
NSURLSession *session = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration] delegate:self delegateQueue:[NSOperationQueue mainQueue]];
// 2.创建Task
NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"https://kyfw.12306.cn/otn"]] completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) { 
    // 3.解析数据
    NSLog(@"%@---%@",[[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding],error);
}];
// 4.执行task
[dataTask resume];
#pragma mark - 遵守<NSURLSessionDataDelegate>
// 如果发送的请求是https的,那么才会调用该方法
-(void)URLSession:(NSURLSession *)session didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition, NSURLCredential * _Nullable))completionHandler {
    /**
     判断服务器传给我们的信任的类型，只有是【服务器信任的时候，才安装证书】
     NSURLSessionAuthChallengeDisposition 如何处理证书
     NSURLAuthenticationMethodServerTrust 服务器信任
     */
    if(![challenge.protectionSpace.authenticationMethod isEqualToString:@"NSURLAuthenticationMethodServerTrust"]) {
        return;
    }
    NSLog(@"%@",challenge.protectionSpace);
    /*
     NSURLCredential 授权信息
     NSURLSessionAuthChallengeUseCredential = 0, 使用该证书 安装该证书
     NSURLSessionAuthChallengePerformDefaultHandling = 1, 默认采用的方式,该证书被忽略
     NSURLSessionAuthChallengeCancelAuthenticationChallenge = 2, 取消请求,证书忽略
     NSURLSessionAuthChallengeRejectProtectionSpace = 3,          拒绝
     */
    NSURLCredential *credential = [[NSURLCredential alloc]initWithTrust:challenge.protectionSpace.serverTrust];
    completionHandler(NSURLSessionAuthChallengeUseCredential,credential);
    // 注意：并不是所有的https的请求都需要安装证书(授权)的，请求一些大型的网站有的是强制安装的，如：苹果官网https://www.apple.com
}
```
方案二：如果使用AFN网络请求
```
AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
// 更改解析方式（请求网页源码应使用原始解析）
manager.responseSerializer = [AFHTTPResponseSerializer serializer];
// 设置对证书的处理方式
// 允许自签名证书，必须的
manager.securityPolicy.allowInvalidCertificates = YES;
// 是否验证域名的CN字段（不是必须的，但是如果写YES，则必须导入证书）
manager.securityPolicy.validatesDomainName = NO;
[manager GET:@"https://kyfw.12306.cn/otn" parameters:nil progress:nil success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
    NSLog(@"success---%@",[[NSString alloc]initWithData:responseObject encoding:NSUTF8StringEncoding]);
} failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
    NSLog(@"error---%@",error);
}];
```
- 
补充`ATS（全称:App Transport Security）`
- iOS9中新增App Transport Security（简称ATS）特性, 让原来请求时候用到的HTTP，全部都转向TLS1.2协议进行传输。
- 这意味着所有的HTTP协议都强制使用了HTTPS协议进行传输。
- 如果我们在iOS9下直接进行HTTP请求是会报错。系统会告诉我们不能直接使用HTTP进行请求，需要在Info.plist中控制ATS的配置。
- "NSAppTransportSecurity"是ATS配置的根节点，配置了节点表示告诉系统要走自定义的ATS设置。
- "NSAllowsAritraryLoads"节点控制是否禁用ATS特性，设置YES就是禁用ATS功能;
- 采用解决方法，修改配置信息。
![](http://upload-images.jianshu.io/upload_images/2230763-4345b1bb3bfc9743.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
http Info.plist 修改配置信息.png
##### 6.数据安全--加密解密效果
![](http://upload-images.jianshu.io/upload_images/2230763-d1c3826956e9f5ee.gif?imageMogr2/auto-orient/strip)
iOS数据安全-->base64、md5、AES、DES、RSA
学习本文之外可以参考
[网络安全——数据的加密与签名,RSA介绍](http://www.cnblogs.com/mddblog/p/5380556.html)
[关于Https安全性问题、双向验证防止中间人攻击问题](http://blog.csdn.net/u010731949/article/details/50538280)
写在最后，
曾经的曾经，
明天的你,一定会感谢今天[拼命](http://upload-images.jianshu.io/upload_images/2230763-a92e6f36d25e44f1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)努力的自己！
附上一张图：
![](http://upload-images.jianshu.io/upload_images/2230763-2266cd55ae7d9339.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
一枚程序员 专用壁纸.png
![](http://upload-images.jianshu.io/upload_images/2230763-df32904650d0c2da.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
脸 在哪~
时间有限，在这里还有好多地方 实战应用，没有总结写上，后续再做补充吧~
附上写的小样([Demo](https://github.com/CustomPBWaters/DataSecurity-Encryption))
方法可直接拿走使用，如果你喜欢或有帮助，可否给个 [Star](http://www.jianshu.com/p/1a7d57b60778)
后续
学习总结-->[GitHub](http://www.jianshu.com/u/fd745d76c816)(现在代码少点，总结好 待上传)、[白开水ln-简书](http://www.jianshu.com/u/fd745d76c816)
不定时、持续更新、一些 学习心得与文章、实战应用~
我也是对所花费时间的一个总结,「学习总结，一劳永逸」。有不足或不对之处，乐听你的槽点(和❤️) ~ 关注菜鸟成长([简书专题](http://www.jianshu.com/c/df53d8188e24))^_^. 我会不定时更新一些学习心得与文章；
