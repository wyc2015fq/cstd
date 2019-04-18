# iOS开发 实现数据缓存 - weixin_33985507的博客 - CSDN博客
2018年12月05日 17:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
### 一 、数据缓存的过程：
从服务器请求到数据之后，需要做以下步骤：
> 
（1） 使用请求到的数据，比如进行解析、显示到界面等
（2）将请求到的数据缓存到硬盘（沙盒）
再次使用到该数据就会有两种情况
> 
（1） 如果程序没有被关闭，一直在运行，那么此时内存缓存和硬盘缓存中都有数据，就直接使用内存缓存中的数据
（2） 如果程序重新启动，那么内存缓存就被释放了，硬盘缓存的的数据还在，就使用硬盘缓存中的数据，并且会将数据缓存到内存中
### 二 、缓存的实现
- 说明：
由于GET请求一般用来查询数据，POST请求一般是发大量数据给服务处理，变动性比较大，因此开发中一般只对GET请求进行缓存，不对POST请求进行缓存
在iOS中，可以使用NSURLCache类缓存数据
iOS5之前，只支持内存缓存。iOS5开始，同时支持内存缓存和硬盘缓存
- 
NSURLCache
iOS中的缓存技术用到了NSURLCache类
缓存原理：一个NSURLRequest对应一个NSCachedURLResponse
缓存技术：把缓存的数据都保存到数据库中
- 
NSURLCache的常见用法
```
// (1) 获得全局缓存对象（没必要手动创建）
NSURLCache *cache = [NSURLCache sharedURLCache];
// (2)设置内存缓存的最大容量（字节为单位，默认为512KB）
- (void)setMemoryCapacity:(NSUInteger)memoryCapacity;
// (3) 设置硬盘缓存的最大容量（字节为单位，默认为10M） 
– (void)setDiskCapacity:(NSUInteger)diskCapacity;
// (4) 硬盘缓存的位置：
沙盒/Library/Caches
// (5) 取得某个请求的缓存
– (NSCachedURLResponse *)cachedResponseForRequest:(NSURLRequest *)request;
// (6) 清除某个请求的缓存  
– (void)removeCachedResponseForRequest:(NSURLRequest *)request;
// (7) 清除所有缓存 
 – (void)removeAllCachedResponses;
```
- 缓存GET请求
```
// 创建请求
NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
// 设置缓存策略（只要设置了缓存策略，系统会自动利用NSURLCache进行数据缓存）
request.cachePolicy = NSURLRequestReturnCacheDataElseLoad;
```
- iOS对NSURLRequest提供了7种缓存策略：（能用的只有4种）
> 
(1) NSURLRequestUseProtocolCachePolicy // 默认的缓存策略（取决于协议）
(2) NSURLRequestReloadIgnoringLocalCacheData// 忽略缓存，重新请求
(3) NSURLRequestReloadIgnoringLocalAndRemoteCacheData // 未实现
(4) NSURLRequestReloadIgnoringCacheData = NSURLRequestReloadIgnoringLocalCacheData // 忽略缓存，重新请求
(5) NSURLRequestReturnCacheDataElseLoad// 有缓存就用缓存，没有缓存就重新请求
(6) NSURLRequestReturnCacheDataDontLoad// 有缓存就用缓存，没有缓存就不发请求，当做请求出错处理（用于离线模式）
(7) NSURLRequestReloadRevalidatingCacheData // 未实现
- 常用的属性
```
// 文件是否已经被缓存
BOOL fileHasBeenCached = NO;
//缓存是否过期
BOOL cachedFileHasExpired = NO;
// 缓存文件是否存在
BOOL cachedFileExists = NO;
// 缓存文件能否被加载
BOOL cachedFileDataCanBeLoaded = NO;
// 缓存文件数据
NSData *cachedFileData = nil;
// 缓存文件是否完全下载
BOOL cachedFileIsFullyDownloaded = NO;
// 缓存文件是否已经下载
BOOL cachedFileIsBeingDownloaded = NO;
// 过期时间
NSDate *expiryDate = nil;
// 下载结束时间
NSDate *downloadEndDate = nil;
// 下载开始时间
NSDate *downloadStartDate = nil;
// 本地缓存路径
NSString *localURL = nil;
// 有效时间
NSNumber *expiresInSeconds = nil;
```
- 缓存的注意事项
缓存的设置需要根据具体情况考虑，如果请求某个URL的返回数据：
（1） 经常更新：不能用缓存
（2） 一成不变：果断用缓存
（3） 偶尔更新：可以定期更改缓存策略或者清除缓存
提示：如果大量使用缓存，会越积越大，建议定期清除缓存
三 、缓存代码示例
```
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
 {
     // 1.创建请求
     NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
     
     // 2.设置缓存策略(有缓存就用缓存，没有缓存就重新请求)
     request.cachePolicy = NSURLRequestReturnCacheDataElseLoad;
     
    // 3.发送请求
    [NSURLConnection sendAsynchronousRequest:request queue:[NSOperationQueue mainQueue] completionHandler:^(NSURLResponse *response, NSData *data, NSError *connectionError) {
        if (data) {
            NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableLeaves error:nil];
            
            NSLog(@"%@", dict);
        }
    }];
}
/**
 // 定期处理缓存
 //    if (缓存没有达到7天) {
 //        request.cachePolicy = NSURLRequestReturnCacheDataElseLoad;
 //    }
 // 获得全局的缓存对象
 NSURLCache *cache = [NSURLCache sharedURLCache];
 //    if (缓存达到7天) {
 //        [cache removeCachedResponseForRequest:request];
 //    }
 
 // lastCacheDate = 2014-06-30 11:04:30
 
 NSCachedURLResponse *response = [cache cachedResponseForRequest:request];
 if (response) {
 NSLog(@"---这个请求已经存在缓存");
 } else {
 NSLog(@"---这个请求没有缓存");
 }
 */
```
