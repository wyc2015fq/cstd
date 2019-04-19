# iOS MD5加密 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月18日 09:53:36[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1590
###### 简介
MD5（单向散列算法）的全称是Message-Digest Algorithm 5（信息-摘要算法）,经过MD5处理后看不到原文，是一种加密算法。
###### MD5的特点
输入两个不同的明文不会得到相同的输出值
根据输出值，不能得到原始的明文，即其过程不可逆
##### MD5的应用
由于MD5加密算法具有较好的安全性，而且免费，因此该加密算法被广泛使用
主要运用在数字签名、文件完整性验证以及口令加密等方面
- 比如发送一个电子文档，发送前，我先得到MD5的输出结果a。然后在对方收到电子文档后，对方也得到一个MD5的输出结果b。如果a与b一样就代表中途未被篡改。
- 比如我提供文件下载，为了防止不法分子在安装程序中添加木马，我可以在网站上公布由安装文件得到的MD5输出结果。
**MD5解密网站：[http://www.cmd5.com](http://www.cmd5.com)**
#### 为了方便使用，我们可以把md5写成一个分类。
##### 头文件
```
#import <Foundation/Foundation.h>
@interface NSString (MD5)
- (NSString *)md5_32bit;
- (NSString *)MD5_32BIT;
@end
```
##### 实现文件
```
#import "NSString+MD5.h"
#import <CommonCrypto/CommonDigest.h>
@implementation NSString (MD5)
- (NSString *)md5_32bit {
    const char *cStr = [self UTF8String];
    unsigned char digest[CC_MD5_DIGEST_LENGTH];
    CC_MD5( cStr, (CC_LONG)self.length, digest );
    NSMutableString *result = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
    for(int i = 0; i < CC_MD5_DIGEST_LENGTH; i++)
        [result appendFormat:@"%02x", digest[i]];
    return result;
}
- (NSString *)MD5_32BIT {
    const char *cStr = [self UTF8String];
    unsigned char digest[CC_MD5_DIGEST_LENGTH];
    CC_MD5( cStr, (CC_LONG)self.length, digest );
    NSMutableString *result = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
    for(int i = 0; i < CC_MD5_DIGEST_LENGTH; i++)
        [result appendFormat:@"%02X", digest[i]];
    return result;
}
@end
```
文／梦亦趣（简书作者）
原文链接：http://www.jianshu.com/p/8898b0bb3c94
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
