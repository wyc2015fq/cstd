# App-鉴权说明

### 描述

在调用App-GetBizToken以及App-Verify等API接口时，为了保障密钥（api_secret）不被泄露，因此，这里引入签名机制来确保服务器之间的通信安全。

### 签名算法

拼接字符串：a=[api_key]&b=[expire_time]&c=[current_time]&d=[random]

字段含义：

| 字段 | 对应项       | 备注                                                         |
| :--- | :----------- | :----------------------------------------------------------- |
| a    | api_key      | 相当于用户名，在FaceID 后台中获取                            |
| b    | expire_time  | 签名的有效期。是一个符合 UNIX Epoch 时间戳规范的数值，单位为秒。 |
| c    | current_time | 生成签名时的时间戳，单位为秒。在设置签名时，current_time需要小于expire_time。 |
| d    | random       | 无符号 10 进制整数，用户需自行生成，最长 10 位               |

**注：**有效期的设置是为了允许签名在一段时间内可以多次使用。

### 生成签名

使用 HMAC-SHA1 算法对请求进行加密。

**sign生成过程如下：**

- 通过各个字段拼接字符串产生一个raw
- 使用api_secret通过HMAC-SHA1 算法对raw进行签名
- 将生成的签名和raw进行拼接，然后再进行Base64编码最终产生一个sign

**公式如下：**

- raw = "a={}&b={}&c={}&d={}".format(api_key, expire_time, current_time, random)
- sign_tmp = HMAC-SHA1(api_secret, raw)
- sign = Base64(''.join(sign_tmp, raw))

**注意：**

- 此处使用的是标准的 Base64 编码，不是 urlsafe 的 Base64 编码。 
- api_secret 必须是和api_key配套使用的，可通过FaceID后台中获取到。

### 示例代码

Python 代码示例

```
import time
import hashlib
import base64
import random
import hmac
api_key = "你的api_key"
api_secret = "你的api_secret"
valid_durtion = 100 # 有效时间100秒
current_time = int(time.time())
expire_time = current_time + valid_durtion
rdm = ''.join(random.choice("0123456789") for i in range(10))
raw = "a={}&b={}&c={}&d={}".format(api_key, expire_time, current_time, rdm)
sign_tmp = hmac.new(api_secret, raw, hashlib.sha1).digest()
sign = base64.b64encode(sign_tmp + raw)
```

Java 代码示例

```
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.util.Base64;
import java.util.Random;
 
public class HmacSha1Sign {
 
    /**
     * 生成签名字段
     *
     * @param apiKey
     * @param secretKey
     * @param expired
     * @return
     * @throws Exception
     */
    public static String genSign(String apiKey,  String secretKey, long expired) throws Exception {
        long now = System.currentTimeMillis() / 1000;
        int rdm = Math.abs(new Random().nextInt());
        String plainText = String.format("a=%s&b=%d&c=%d&d=%d", apiKey, now + expired, now, rdm);
        byte[] hmacDigest = HmacSha1(plainText, secretKey);
        byte[] signContent = new byte[hmacDigest.length + plainText.getBytes().length];
        System.arraycopy(hmacDigest, 0, signContent, 0, hmacDigest.length);
        System.arraycopy(plainText.getBytes(), 0, signContent, hmacDigest.length,
                plainText.getBytes().length);
        return encodeToBase64(signContent).replaceAll("[\\s*\t\n\r]", "");
    }
 
    /**
     * 生成 base64 编码
     *
     * @param binaryData
     * @return
     */
    public static String encodeToBase64(byte[] binaryData) {
        String encodedStr = Base64.getEncoder().encodeToString(binaryData);
        return encodedStr;
    }
 
    /**
     * 生成 hmacsha1 签名
     *
     * @param binaryData
     * @param key
     * @return
     * @throws Exception
     */
    public static byte[] HmacSha1(byte[] binaryData, String key) throws Exception {
        Mac mac = Mac.getInstance("HmacSHA1");
        SecretKeySpec secretKey = new SecretKeySpec(key.getBytes(), "HmacSHA1");
        mac.init(secretKey);
        byte[] HmacSha1Digest = mac.doFinal(binaryData);
        return HmacSha1Digest;
    }
 
    /**
     * 生成 hmacsha1 签名
     *
     * @param plainText
     * @param key
     * @return
     * @throws Exception
     */
    public static byte[] HmacSha1(String plainText, String key) throws Exception {
        return HmacSha1(plainText.getBytes(), key);
    }
}
```

Objective-C 代码示例

```
#import "ViewController.h"
#import <CommonCrypto/CommonHMAC.h>
#import <CommonCrypto/CommonCryptor.h>
#import <math.h>
#define api_key @"你的api_key"
#define api_secret @"你的api_secret"

@interface ViewController ()
@end

@implementation ViewController
- (void)viewDidLoad {
 [super viewDidLoad];
 NSString* sign = [self getSignStr];
 NSLog(@"sign = %@",sign);
}

- (NSString *)getSignStr {
    int valid_durtion = 10000;
    long int current_time = [[NSDate date] timeIntervalSince1970];
    long int expire_time = current_time + valid_durtion;
    long random = abs(arc4random() % 100000000000);
    NSString* str = [NSString stringWithFormat:@"a=%@&b=%ld&c=%ld&d=%ld", api_key, expire_time, current_time, random];
    NSData* sign_tmp = [self hmac_sha1:api_secret text:str];
    NSData* sign_raw_data = [str dataUsingEncoding:NSUTF8StringEncoding];
    NSMutableData* data = [[NSMutableData alloc] initWithData:sign_tmp];
    [data appendData:sign_raw_data];
    NSString* sign = [data base64EncodedStringWithOptions:0];
    return sign;
}
- (NSData *)hmac_sha1:(NSString *)key text:(NSString *)text{
    const char *cKey  = [key cStringUsingEncoding:NSUTF8StringEncoding];
    const char *cData = [text cStringUsingEncoding:NSUTF8StringEncoding];
    char cHMAC[CC_SHA1_DIGEST_LENGTH];
    CCHmac(kCCHmacAlgSHA1, cKey, strlen(cKey), cData, strlen(cData), cHMAC);
    
    NSData *HMAC = [[NSData alloc] initWithBytes:cHMAC length:sizeof(cHMAC)];
    return HMAC;
}

@end
```

PHP 示例代码

```
<?php
 
function gen_sign($apiKey, $apiSecret, $expired){
    $rdm = rand();
    $current_time = time();
    $expired_time = $current_time + $expired;
    $srcStr = "a=%s&b=%d&c=%d&d=%d";
    $srcStr = sprintf($srcStr, $apiKey, $expired_time, $current_time, $rdm);
    $sign = base64_encode(hash_hmac('SHA1', $srcStr, $apiSecret, true).$srcStr);
    return $sign;
}
```