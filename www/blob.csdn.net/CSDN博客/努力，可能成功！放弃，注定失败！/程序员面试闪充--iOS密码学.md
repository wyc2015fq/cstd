# 程序员面试闪充--iOS密码学 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年06月22日 11:38:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：81

但凡一个有点追求的iOS开发，总得会点加密技术，要不然用户信息就有可能被其他人获取用来做一些对我们不利的事情。
视频地址： [密码学](https://link.jianshu.com?t=https%3A%2F%2Fv.qq.com%2Fx%2Fpage%2Fh0558q5sg26.html)
## 一、base64
base64是一种完全可反编译的编码方式，因为编码算法完全公开，所以分分钟就会被破解，所以这个一定不能用于密码的“加密”，一些不需要特别加密的，例如用户名，我们可以用base64进行编码，让人不是一眼就能看出来是什么。
Base64 是网络上最常见的用于传输8Bit 字节代码的编码方式之一，Base64 要求把每三个8Bit 的字节转换为四个6Bit 的字节（3*8 = 4*6 = 24 ），然后把6Bit 再添两个高位0 ，组成四个8Bit 的字节，也就是说，转换后的字符串理论上将要比原来的长1/3 。
比如说：
转换前 aaaaaabb ccccdddd eeffffff
转换后 00aaaaaa 00bbcccc 00ddddee 00ffffff
上面的三个字节是原文，下面的四个字节是转换后的Base64 编码，其前两位均为0。
转换后，我们用一个码表来得到我们想要的字符串（也就是最终的Base64 编码），这个表是这样的：
|字符 值|字符 值|字符 值|字符 值|
|----|----|----|----|
|0 A|17 R|34 i|51 z|
|1 B|18 S|35 j|52 0|
|2 C|19 T|36 k|53 1|
|3 D|20 U|37 l|54 2|
|4 E|21 V|38 m|55 3|
|5 F|22 W|39 n|56 4|
|6 G|23 X|40 o|57 5|
|7 H|24 Y|41 p|58 6|
|8 I|25 Z|42 q|59 7|
|9 J|26 a|43 r|60 8|
|10 K|27 b|44 s|61 9|
|11 L|28 c|45 t|62 +|
|12 M|29 d|46 u|63 /|
|13 N|30 e|47 v|=|
|14 O|31 f|48 w||
|15 p|32 g|49 x||
|16 Q|33 h|50 y||
让我们再来看一个实际的例子，加深印象！
转换前 10101101 10111010 01110110
转换后 00101011 00011011 00101001 00110110
在用二进制表示的字符串前面补0得到转换后的字符串，再对应十进制的码表就能得到最后的结果啦
十进制 43 27 41 54
对应码表中的值是rbp2
可是等等…… 好像有疑惑，原文的字节数量应该是3 的倍数啊，如果这个条件不能满足的话，那该怎么办呢？
其实可以这样解决：原文的字节不够的地方可以用全0 来补足，转换时Base64 编码用= 号来代替。这就是为什么有些Base64 编码会以一个或两个等号结束的原因，但等号最多只有两个。因为：
`余数 = 原文字节数 MOD 3`
所以余数任何情况下都只可能是0 ，1 ，2 这三个数中的一个。如果余数是0 的话，就表示原文字节数正好是3的倍数。如果是1 的话，为了让Base64 编码是4的倍数，就要补2个等号；同理，如果是2 的话，就要补1 个等号。
以 Hello!! 为例，其转换过程为：
![](https://upload-images.jianshu.io/upload_images/2237676-b15f0efc24f78ff2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
图中蓝色背景的二进制的0值是额外补充的。Hello!! Base64编码的结果为 SGVsbG8hIQAA 。最后2个零值只是为了Base64编码而补充的，在原始字符中并没有对应的字符，那么Base64编码结果中的最后两个字符 AA 实际不带有效信息，所以需要特殊处理，以免解码错误。
标准Base64编码通常用 = 字符来替换最后的 A，即编码结果为 SGVsbG8hIQ==。因为 = 字符并不在Base64编码索引表中，其意义在于结束符号，在Base64解码时遇到 = 时即可知道一个Base64编码字符串结束。
```
- (NSString *)base64EncodedString;
{
NSData *data = [self dataUsingEncoding:NSUTF8StringEncoding];
return [data base64EncodedStringWithOptions:0];
}
- (NSString *)base64DecodedString
{
NSData *data = [[NSData alloc]initWithBase64EncodedString:self options:0];
return [[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding];
}
```
## 二、MD5
全称是Message Digest Algorithm 5，译为“消息摘要算法第5版”。它能对输入信息生成唯一的128位散列值（32个字符）。输入两个不同的明文不会得到相同的输出值，根据输出值，不能得到原始的明文，即其过程不可逆。
由于MD5加密算法具有较好的安全性，而且免费，因此该加密算法被广泛使用。主要运用在数字签名、文件完整性验证以及口令加密等方面。
![](https://upload-images.jianshu.io/upload_images/2237676-50634ee2acd8c66f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/685)
当用户进行注册、登录操作时，我们可以直接对密码进行MD5操作，用户Login的时候，系统是把用户输入的密码计算成MD5值，然后再去和系统中保存的MD5值进行比较，而系统并"不知道"用户的密码是什么。
MD5特点:
- 压缩性 : 任意长度的数据,算出的MD5值长度都是固定的。
- 容易计算 : 从原数据计算出MD5值很容易。
- 抗修改性 : 对原数据进行任何改动，哪怕只修改一个字节，所得到的MD5值都有很大区别。
- 弱抗碰撞 : 已知原数据和其MD5值，想找到一个具有相同MD5值的数据（即伪造数据）是非常困难的。
- 强抗碰撞 : 想找到两个不同数据，使他们具有相同的MD5值，是非常困难的。
```
/****************************MD5.m类实现文件内容****************************/
//对字符串数据进行MD5的签名
+ (NSString *)md5SignWithString:(NSString *)string
{
    const char *object = [string UTF8String];
    unsigned char result[CC_MD5_DIGEST_LENGTH];
    CC_MD5(object,(CC_LONG)strlen(object),result);
    NSMutableString *hash = [NSMutableString string];
    for (int i = 0; i < 16; i ++) {
        [hash appendFormat:@"%02X", result[i]];
    }
    return [hash lowercaseString];
}
//对二进制数据进行MD5的签名
+ (NSData *)md5SignWithData:(NSData *)data
{
    Byte byte[CC_MD5_DIGEST_LENGTH];    //定义一个字节数组来接收结果
    CC_MD5((const void*)([data bytes]), (CC_LONG)[data length], byte);
    return [NSData dataWithBytes:byte length:CC_MD5_DIGEST_LENGTH];
}
/******************************************************************************/
//使用MD5文件进行MD5加密和验签
/*********************************使用MD5类*********************************/
//使用MD5执行加密操作
NSString *string2 = @"abcdefghijklmnopqrstuvwxyz";
NSString *encodeString2 = [MD5 md5SignWithString:string2];
NSLog(@"encodeString2 : %@", encodeString2);
//MD5为不可逆的操作，使用MD5执行验签操作
NSString *verifyString2 = [MD5 md5SignWithString:string2];
NSLog(@"verifyString2 : %@", verifyString2);
if ([verifyString2 isEqualToString:encodeString2]) {
    NSLog(@"md5 verify sign success");
} else {
    NSLog(@"md5 verify sign failed");
}
```
然而现在的MD5已不再是绝对安全，因为网上有一套数据库[http://cmd5.com](https://link.jianshu.com?t=http%3A%2F%2Fcmd5.com)可以查到许多MD5的词条。对此，可以对MD5稍作改进，以增加解密的难度。
加盐（Salt）：在明文的固定位置插入随机串，然后再进行MD5
先加密，后乱序：先对明文进行MD5，然后对加密得到的MD5串的字符进行乱序。总之宗旨就是：黑客就算攻破了数据库，也无法解密出正确的明文。
## 三、HMAC加密
此加密方法需要先生成密钥，然后再对密码进行MD5和HMAC加密，数据库中需要存放当时使用的密钥和密码加密后的密文
在用户登陆时 再次对填入的密码用密钥进行加密 并且还要加上当前时间（精确到分钟） 再次HMAC加密，服务器里也会拿出以前存放的密文加上时间再次加密。所以就算黑客在中途截取了密码的密文 也在能在1分钟只能破译才能有效，大大加强了安全性。服务器为了考虑到网络的延迟一般会多算一种答案，如23分过来的密码 他会把23分和22分的都算一下和用户匹配只要对上一个就允许登陆。
## 四、AES加密
高级加密标准Advanced Encryption Standard简称：AES。是美国联邦政府采用的一种区块加密标准。AES设计有三个密钥长度:128、192、256位，相对而言，AES的128密钥比DES的56密钥强1021倍。AES算法主要包括三个方面：轮变化、圈数和密钥扩展。
AES加解密特点：
- AES强安全性、高性能、高效率、易用和灵活。
- 在软件及硬件上都能快速地加解密且只需要很少的存储资源。
AES的使用：除了类库本身只有Config类里面包含AESCrpt.h，只有两个方法用到。一个是保存用户名和密码，密码使用了AES加密，另一个是解密密码后再返回这个密码；保存用户名和密码是将用户名和密码放到了本地的一个沙盒之中，获取的时候直接从本地读取加密后的文件，经过解密和服务器上用户数据进行比较。
```
-(void)saveUserNameAndPwd:(NSString *)userName andPwd:(NSString *)pwd  
{  
    NSUserDefaults * settings = [NSUserDefaults standardUserDefaults];  
    [settings removeObjectForKey:@"UserName"];  
    [settings removeObjectForKey:@"Password"];  
    [settings setObject:userName forKey:@"UserName"];  
      
    pwd = [AESCrypt encrypt:pwd password:@"pwd"];  
      
    [settings setObject:pwd forKey:@"Password"];  
    [settings synchronize];  
}
```
```
-(NSString *)getPwd  
{  
    NSUserDefaults * settings = [NSUserDefaults standardUserDefaults];  
    NSString * temp = [settings objectForKey:@"Password"];  
    return [AESCrypt decrypt:temp password:@"pwd"];  
}
```
正如官方给出示例用法一样，AES的使用非常简单，首先要添加头文件 #import "AESCrypt.h"，使用示例如下：
```
NSString *pwdKey = @"新风作浪";  
NSString *password = @"duxinfeng123456";  
  
NSString *encryptedPWD = [AESCrypt encrypt:password password:pwdKey];  
NSString *decryptedPWD = [AESCrypt decrypt:encryptedPWD password:pwdKey];  
  
NSLog(@"加密后密码:%@  解密后密码: %@",encryptedPWD,decryptedPWD);
```
## 五、RSA加密
RSA非对称加密算法
非对称加密算法需要两个密钥：公开密钥（publickey）和私有密钥（privatekey）
公开密钥与私有密钥是一对，如果用公开密钥对数据进行加密，只有用对应的私有密钥才能解密；如果用私有密钥对数据进行加密，那么只有用对应的公开密钥才能解密
特点：
非对称密码体制的特点：算法强度复杂、安全性依赖于算法与密钥但是由于其算法复杂，而使得加密解密速度没有对称加密解密的速度快
对称密码体制中只有一种密钥，并且是非公开的，如果要解密就得让对方知道密钥。所以保证其安全性就是保证密钥的安全，而非对称密钥体制有两种密钥，其中一个是公开的，这样就可以不需要像对称密码那样传输对方的密钥了。
基本加密原理：
(1)找出两个“很大”的质数：P & Q
(2)N = P * Q
(3)M = (P – 1) * (Q – 1)
(4)找出整数E，E与M互质，即除了1之外，没有其他公约数
(5)找出整数D，使得E*D除以M余1，即 (E * D) % M = 1
经过上述准备工作之后，可以得到：
E是公钥，负责加密
D是私钥，负责解密
N负责公钥和私钥之间的联系
加密算法，假定对X进行加密
(X ^ E) % N = Y
n根据费尔马小定义，根据以下公式可以完成解密操作
(Y ^ D) % N = X
但是RSA加密算法效率较差，对大型数据加密时间很长，一般用于小数据。
## 六、指纹识别
指纹识别功能是 iphone 5S之后推出的.SDK是 iOS 8.0 推出，它的出现简化移动支付环节,迅速占领了移动支付市场。
使用步骤：
导入框架
`#import <LocalAuthentication/LocalAuthentication.h>`
获得当前系统版本号
```
```float version = [UIDevice currentDevice].systemVersion.floatValue;
if (version < 8.0 ) // 判断当前系统版本 {
    NSLog(@"系统版本太低,请升级至最新系统");
    return;
}
```
实例化指纹识别对象,判断当前设备是否支持指纹识别功能(是否带有TouchID)
```
// 1> 实例化指纹识别对象
    LAContext *laCtx = [[LAContext alloc] init];
    // 2> 判断当前设备是否支持指纹识别功能.
    if (![laCtx canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:NULL]) {
        // 如果设备不支持指纹识别功能
        NSLog(@"该设备不支持指纹识别功能");
        return;
    };
```
指纹登录(默认是异步方法)
```
[laCtx evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics localizedReason:@"指纹登陆" reply:^(BOOL success, NSError *error) {
        // 如果成功,表示指纹输入正确.
        if (success) {
            NSLog(@"指纹识别成功!");
        } else {
            NSLog(@"指纹识别错误,请再次尝试");
        }
    }];
```
## 七、面试题
1、你会如何存储用户的一些敏感信息，如登录的token？
使用keychain来存储,也就是钥匙串,使用keychain需要导入Security框架
iOS的keychain服务提供了一种安全的保存私密信息（密码，序列号，证书等）的方式，每个ios程序都有一个独立的keychain存储。相对于 NSUserDefaults、文件保存等一般方式，keychain保存更为安全，而且keychain里保存的信息不会因App被删除而丢失，所以在 重装App后，keychain里的数据还能使用。从ios 3。0开始，跨程序分享keychain变得可行。
如何需要在应用里使 用使用keyChain，需要导入Security.framework ，keychain的操作接口声明在头文件SecItem.h里。直接使用SecItem.h里方法操作keychain，需要写的代码较为复杂，可以使用一些已经封装好了的工具类KeychainItemWrapper和 SFHFKeychainUtils。
2、说说你加密算法常用在什么地方？
- 远程推送：用户设备与苹果的APNS服务器形成长连接时，苹果服务器会加密生成一个deviceToken给用户设备。
- 对信息保密：将明文通过接收人的公钥加密，传输给接受人时使用非对称算法可以防止中间人攻击。
- 身份验证和防止篡改
- 登录注册
- 支付宝支付
3、OSI七层模型中，哪一层用于数据格式转化和数据加密呢？（B）
A、应用层 B、表示层  C、会话层 D 传输层
解释： 表示层提供各种用于应用层数据的编码和转换功能,确保一个系统的应用层发送的数据能被另一个系统的应用层识别。如果必要，该层可提供一种标准表示形式，用于将计算机内部的多种数据格式转换成通信中采用的标准表示形式。数据压缩和加密也是表示层可提供的转换功能之一。
4、逆向分析最常用的有三种方法是什么？
- 网络分析
通过分析和篡改接口数据，可以有效的破解通过接口数据来控制客户端行为的app，常用的抓包工具有Tcpdump, WireShark, Charles等，windows平台有fidller
- 静态分析
通过砸壳、反汇编、classdump头文件等技术来分析app行为，通过这种方式可以有效的分析出app实用的一些第三方库，甚至分析出app的架构等内容，常用的工具有dumpdecrypted（砸壳）、hopper disassembler（反汇编）、class_dump（导头文件）
- 动态分析
动态分析指的是通过分析app的运行时数据，来定位注入点或者获取关键数据，常用的工具有cycript（运行时控制台）、 lldb+debugserver（远程断点调试）、logify（追踪）
5、（阿里三面）怎样防止反编译？
1.本地数据加密
iOS应用防反编译加密技术之一：对NSUserDefaults，sqlite存储文件数据加密，保护帐号和关键信息
2.URL编码加密
iOS应用防反编译加密技术之二：对程序中出现的URL进行编码加密，防止URL被静态分析
3.网络传输数据加密
iOS应用防反编译加密技术之三：对客户端传输数据提供加密方案，有效防止通过网络接口的拦截获取数据
4.方法体，方法名高级混淆
iOS应用防反编译加密技术之四：对应用程序的方法名和方法体进行混淆，保证源码被逆向后无法解析代码
5.程序结构混排加密
iOS应用防反编译加密技术之五：对应用程序逻辑结构进行打乱混排，保证源码可读性降到最低
6.借助第三方APP加固，例如：网易云易盾
作者：阡陌有客
链接：https://www.jianshu.com/p/0b4cd8bcc1b4
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
