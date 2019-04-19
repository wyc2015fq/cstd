# URL加解密 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月01日 09:27:45[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2186
# URL加解密
## 背景介绍
- iOS 下URL加解密，项目使用`AFNetworking`
- 虽然是使用HTTPS，但是从安全方面考虑，在很多情况下还是需要对url的参数进行加密的。
- 接口如 `https://192.160.0.123:92/frame/webInteface.do?(method=addNew&uid=123&pass=131)`
- 不管是get请求还是post请求，都可以对后边的参数进行加密
- get请求比较简单，这里直接说post请求吧
## 加密方式
- 加密：首先对字符串记性AES128加密，然后进行base64加密（主要是为了去除特殊字符）
- 解密：先base64解密，然后在AES128解密即可还原数据
- 其中base64加解密使用 `GTMBase64`添加两个方法
```
+ (NSString*)encodeBase64Data:(NSData *)data {
    data = [GTMBase64 encodeData:data];
    NSString *base64String = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    return base64String;
}
+ (NSData*)decodeBase64String:(NSString * )input {
    NSData *data = [input dataUsingEncoding:NSUTF8StringEncoding allowLossyConversion:YES];
    data = [GTMBase64 decodeData:data];
    return data;
}
```
- AES128使用系统`CommonCrypto/CommonCryptor.h`实现 //用于AES
- 添加`NSData`分类，增加两个方法
```
#pragma mark - AES128位加解密
- (NSData *)AES128EncryptWithKey:(NSString *)key {
    char keyPtr[kCCKeySizeAES128 + 1];
    memset(keyPtr, 0, sizeof(keyPtr));
    [key getCString:keyPtr maxLength:sizeof(keyPtr) encoding:NSUTF8StringEncoding];
    NSUInteger dataLength = [self length];
    size_t bufferSize = dataLength + kCCBlockSizeAES128;
    void *buffer = malloc(bufferSize);
    size_t numBytesEncrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCEncrypt,
                                          kCCAlgorithmAES,
                                          kCCOptionPKCS7Padding|kCCOptionECBMode,
                                          keyPtr,
                                          kCCBlockSizeAES128,
                                          NULL /* initialization vector (optional) */,
                                          [self bytes],
                                          dataLength, /* input */
                                          buffer,
                                          bufferSize, /* output */
                                          &numBytesEncrypted);
    if (cryptStatus == kCCSuccess) {
        //the returned NSData takes ownership of the buffer and will free it on deallocation
        return [NSData dataWithBytesNoCopy:buffer length:numBytesEncrypted];
    }
    free(buffer); //free the buffer;
    return nil;
}
- (NSData *)AES128DecryptWithKey:(NSString *)key {
    // 'key' should be 32 bytes for AES256, will be null-padded otherwise
    char keyPtr[kCCKeySizeAES128+1]; // room for terminator (unused)
    bzero(keyPtr, sizeof(keyPtr)); // fill with zeroes (for padding)
    // fetch key data
    [key getCString:keyPtr maxLength:sizeof(keyPtr) encoding:NSUTF8StringEncoding];
    NSUInteger dataLength = [self length];
    //See the doc: For block ciphers, the output size will always be less than or
    //equal to the input size plus the size of one block.
    //That's why we need to add the size of one block here
    size_t bufferSize = dataLength + kCCBlockSizeAES128;
    void *buffer = malloc(bufferSize);
    size_t numBytesDecrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCDecrypt,
                                          kCCAlgorithmAES128,
                                          kCCOptionPKCS7Padding|kCCOptionECBMode,
                                          keyPtr,
                                          kCCBlockSizeAES128,
                                          NULL /* initialization vector (optional) */,
                                          [self bytes],
                                          dataLength, /* input */
                                          buffer,
                                          bufferSize, /* output */
                                          &numBytesDecrypted);
    if (cryptStatus == kCCSuccess) {
        //the returned NSData takes ownership of the buffer and will free it on deallocation
        return [NSData dataWithBytesNoCopy:buffer length:numBytesDecrypted];
    }
    free(buffer); //free the buffer;
    return nil;
}
```
## 直奔主题
- `AFNetworking` 的post请求如下
```
- (AFHTTPRequestOperation *)POST:(NSString *)URLString
                      parameters:(id)parameters
       constructingBodyWithBlock:(void (^)(id <AFMultipartFormData> formData))block
                         success:(void (^)(AFHTTPRequestOperation *operation, id responseObject))success
                         failure:(void (^)(AFHTTPRequestOperation *operation, NSError *error))failure
```
- 其中URLString可以作为最基础的，不需要加密
- parameters 就是我们需要加密的地方，这是一个字典,因为AFN会对这个parameters进行解析，所以对这个参数集合进行一次包装，拼接成一个字符串。然后对字符串进行加密。
### 原来的代码是这样请求数据的
```
NSMutableDictionary *para = [NSMutableDictionary dictionary];
    para[@"method"] = @"securityAdd";
    para[@"userId"] = userId;
    para[@"userPsw"] = userPsw;
    para[@"content"] = @"ddddd123891237";
          NSString *url = [NSString stringWithFormat:@"https://%@:82/frame/webInteface.do?", NHBaseURL];
    AFHTTPRequestOperation *operation = [NetWorkInst POST:url parameters:dict constructingBodyWithBlock:^(id<AFMultipartFormData> formData) {       
    } success:^(AFHTTPRequestOperation *operation, id responseObject) {
    }];
```
### 加密后代码是这样的
```
NSMutableDictionary *para = [NSMutableDictionary dictionary];
    para[@"method"] = @"securityAdd";
    para[@"userId"] = userId;
    para[@"userPsw"] = userPsw;
    para[@"content"] = @"ddddd123891237";
    // 开始加密****************************
    NSString *str = [NSString stringWithFormat:@"'method':'securityAdd','userId':'%@','userPsw':'%@','content':'%@'",userId,userPsw,content];
    NSLog(@"原始数据：%@",str);
    NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
    NSData *aaa = [data AES128EncryptWithKey:@"song.com"]; // aes加密
    NSLog(@"加密AES128后：%@",aaa);
    NSString *bbb = [PublicMethod encodeBase64Data:aaa];//base64加密
    NSLog(@"base64加密后：%@",bbb);
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    dict[@"info"] = bbb;
    // 开始解密****************************
    NSData *da = [PublicMethod decodeBase64String:bbb]; //base64解密
    NSString *ccc = [[NSString alloc] initWithData:da encoding:NSUTF8StringEncoding];
    NSLog(@"base64解密后：%@",ccc);
    NSData *ddd = [da AES128DecryptWithKey:@"song.com"];// aes解密
    NSString *eee = [[NSString alloc] initWithData:ddd encoding:NSUTF8StringEncoding];
    NSLog(@"解密AES128后：%@",eee);
    NSString *url = [NSString stringWithFormat:@"https://%@:82/frame/webInteface.do?", NHBaseURL];
    AFHTTPRequestOperation *operation = [NetWorkInst POST:url parameters:dict constructingBodyWithBlock:^(id<AFMultipartFormData> formData) {       
    } success:^(AFHTTPRequestOperation *operation, id responseObject) {
    }];
```
文／MrSong（简书作者）
原文链接：http://www.jianshu.com/p/2cee02d50f30
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
