# AFNetworking（v3.0+）框架学习总结(三框架基本使用) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月19日 16:04:42[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1164
#### 本文来自简书，原文地址:[http://www.jianshu.com/p/228b4b37093e](http://www.jianshu.com/p/228b4b37093e)
#### Write in the first【写在最前】
AFNetWorking 基本是iOS开发中使用网络通信框架的标配，这个框架本身比较庞大，也很复杂，但是使用起来非常非常简单。
在对【AFN 框架学习总结[（一）](https://custompbwaters.github.io/2017/02/16/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.1.0%EF%BC%89%E6%A1%86%E6%9E%B6%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%B8%80%E6%A1%86%E6%9E%B6%E7%BB%93%E6%9E%84)/)[|（二）](https://custompbwaters.github.io/2017/02/17/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.1.0%EF%BC%89%E6%A1%86%E6%9E%B6%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%BA%8C%E5%86%85%E9%83%A8%E9%80%BB%E8%BE%91%E5%A4%84%E7%90%86%E8%BF%87%E7%A8%8B)/)】上两篇文章学习理解的基础上，
本篇文章主要从【AFN 框架基本使用】学习，知其然亦知其所以然，接下来我们就来点干货，实用才是硬道理 ~
时间有限，知识并未全覆盖，有遗漏或者错误，忘指正。
![](http://upload-images.jianshu.io/upload_images/2230763-52f44551d97d1941.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
 AFNetworking
> 
目录：
- AFN GET | POST 请求
- AFN 文件下载 | 上传
- AFN 序列化处理
- AFN 检测网络状态
- AFN 使用技巧
- AFN 数据安全
- AFN 离线断点下载
- 后续【AFN 框架 | 源码 学习总结】
#### Getting Started【开始使用】
#### Examples 【AFN GET | POST 请求】
这里就其他概念不做过多解释，如果你需要可以参考 上面学习总结的（一）、（二）文章，下面就直接来代码了【代码具有详细注释】。
AFN 发送 get 和 post 请求方法，只需把 GET 修改为 POST 即可，
```
// 1.创建会话管理者
AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
// 字典(发送给服务器的数据~参数)
NSDictionary *paramDict = @{
                            @"username":@"520it",
                            @"pwd":@"520it",
                            @"type":@"JSON"
                            };
// 2.发送GET请求
/**
 GET: NSString类型的请求路径，AFN内部会自动将该路径包装为一个url并创建请求对象
 parameters: 请求参数，以字典的方式传递，AFN内部会判断当前是POST请求还是GET请求，
             以选择直接拼接还是转换为NSData放到请求体中传递.
 progress: 进度回调,此处为nil
 success: 请求成功之后回调Block
    task: 请求任务、
    responseObject: 响应体信息（内部已编码处理JSON->OC对象）
 failure: 失败回调（error:错误信息）
 task.response: 响应头
 */
[manager GET:@"http://120.25.226.186:32812/login?username=520it&pwd=520it&type=JSON" parameters:paramDict progress:^(NSProgress * _Nonnull downloadProgress) {
} success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
    NSLog(@"%@\n%@",task.response,responseObject);
} failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
    NSLog(@"%@",error);
}];
```
总结：
这里使用方法其实非常简单和我们之前的使用没有太大的区别，只是这里 AFN 把请求路径给拆分了，把参数单独拿出来，并且用一个字典的方式装载。相信这里大家应该都可以明白为什么作者 把参数单独给拿出来，这样更有利于代码的封装，我们使用起来更加的方便。
注意：
这里关于 AFN（GET | POST 请求）内部业务逻辑是如何处理的，和之前使用 `NSURLSession` 大致是一样的，如果你需要了解
 可以在看一下 [学习总结(二)](https://custompbwaters.github.io/2016/03/21/iOS%E3%80%8CUI%20+%20NET%E3%80%8D/NSURLSession%E8%AF%A6%E8%A7%A3%E4%BD%BF%E7%94%A8/)
#### Examples 【AFN 文件下载 | 上传】
> 
AFN 实现文件下载步骤：
- 创建会话管理者
- 创建下载路径和请求对象
- 创建下载任务
- 执行下载
```
// 1.创建会话管理者
AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
// 2.创建下载路径和请求对象
NSURL *url = [NSURL URLWithString:@"http://120.25.226.186:32812/resources/videos/minion_01.mp4"];
NSURLRequest *request = [NSURLRequest requestWithURL:url];
// 3.创建下载任务
/**
 Request: 请求对象
 progress: 进度回调（监听下载进度）
    completedUnitCount: 已经下载的数据大小
    totalUnitCount: 文件数据的总大小
 destination: 回调,该block需要返回值（NSURL类型）,告诉系统应该把文件剪切到什么地方
    targetPath: 文件的临时保存路径
    response: 响应头信息
 completionHandler: 请求完成后回调
    response：响应头信息
    filePath：文件的保存路径，即destination回调的返回值
    error：错误信息
 */
NSURLSessionDownloadTask *downloadTask = [manager downloadTaskWithRequest:request progress:^(NSProgress * _Nonnull downloadProgress) {
    // 监听下载进度（3.0版本之后的做法）
    NSLog(@"%f",1.0 *downloadProgress.completedUnitCount / downloadProgress.totalUnitCount);    
} destination:^NSURL * _Nonnull(NSURL * _Nonnull targetPath, NSURLResponse * _Nonnull response) {
    // 指定下载路径
    // targetPath临时路径,fullPath存储路径
    NSString *fullPath = [[NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:response.suggestedFilename];
    NSLog(@"%@\n%@",targetPath,fullPath);
    return [NSURL fileURLWithPath:fullPath];
} completionHandler:^(NSURLResponse * _Nonnull response, NSURL * _Nullable filePath, NSError * _Nullable error) {
    NSLog(@"%@\n%@",filePath,error);
}];
// 4.执行下载
[downloadTask resume];
```
注意：
如何监听下载进度，AFN 3.0之后的版本监听下载进度是上面的做法。而AFN 在2.6.3 之前并没有提供 progress 回调给我们，此时要想监听下载进度需要使用KVO，给它添加一位观察者监听内部 progress值的改变。
```
AFHTTPSessionManager *manage  = [AFHTTPSessionManager manager];
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://120.25.226.186:32812/resources/videos/minion_01.mp4"]];
    // 创建下载进度，并监听
    NSProgress *progress = nil;
    NSURLSessionDownloadTask *downloadTask = [manage downloadTaskWithRequest:request progress:&progress destination:^NSURL * _Nonnull(NSURL * _Nonnull targetPath, NSURLResponse * _Nonnull response) {
        // 指定下载路径
        NSString *fullPath = [[NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:response.suggestedFilename];
        NSLog(@"%@\n%@",targetPath,fullPath);
        return [NSURL fileURLWithPath:fullPath];
    } completionHandler:^(NSURLResponse * _Nonnull response, NSURL * _Nonnull filePath, NSError * _Nonnull error) {
        NSLog(@"文件下载完毕---%@",filePath);
    }];
    // 使用KVO监听下载进度
    [progress addObserver:self forKeyPath:@"completedUnitCount" options:NSKeyValueObservingOptionNew context:nil];
    // 启动任务
    [downloadTask resume];
```
```
// 获取并计算当前文件的下载进度
-(void)observeValueForKeyPath:(NSString *)keyPath 
                  ofObject:(NSProgress *)progress 
                  change:(NSDictionary<NSString *,id> *)change 
                  context:(void *)context {
  NSLog(@"%zd--%zd",progress.completedUnitCount,progress.totalUnitCount);
  NSLog(@"%f",1.0 * progress.completedUnitCount/progress.totalUnitCount)
}
- (void)dealloc{
    // 移除(监听)
    [self.person removeObserver:self forKeyPath:@"completedUnitCount"];
}
```
#### AFN 文件上传
```
// 1.创建会话管理者
AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
// 2.发送post请求上传文件
/*
 POST: 请求路径（NSString类型）
 parameters: 非文件参数，以字典的方式传递
 constructingBodyWithBlock: 处理要上传的文件数据(在该回调中拼接文件参数)
 progress: 进度回调
    uploadProgress.completedUnitCount:已经上传的数据大小
    uploadProgress.totalUnitCount：数据的总大小
 success: 成功回调 
    task：上传Task
    responseObject:服务器返回的响应体信息（已经以JSON的方式转换为OC对象）
 failure: 失败回调
 */
[manager POST:@"http://120.25.226.186:32812/upload" parameters:nil constructingBodyWithBlock:^(id<AFMultipartFormData>  _Nonnull formData) {
    // 使用formData来拼接数据
    [formData appendPartWithFileURL:[NSURL fileURLWithPath:@"/Users/sunhui/Desktop/LNnet2.16/LNnet/Assets.xcassets/LN.imageset/LN.png"] name:@"file" error:nil];
} progress:^(NSProgress * _Nonnull uploadProgress) {
    NSLog(@"%f",1.0 * uploadProgress.completedUnitCount / uploadProgress.totalUnitCount);
} success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
    NSLog(@"上传成功---%@",responseObject);
} failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
    NSLog(@"上传失败---%@",error);
}];
```
总结：
这里使用 formData 来拼接数据，共有三种方法如下
```
// 文件上传拼接数据的第一种方式（FileData）
/*
 FileData: 二进制数据 要上传的文件参数
 name: 服务器规定的 @"file"
 fileName: 该文件上传到服务器以什么名称保存
 */
UIImage *image = [UIImage imageNamed:@"LNl.png"];
NSData *imageData = UIImagePNGRepresentation(image);
[formData appendPartWithFileData:data name:@"file" 
fileName:@"xxoo.png" mimeType:@"application/octet-stream"];
// 文件上传拼接数据的第二种方式（FileURL）
[formData appendPartWithFileURL:fileUrl name:@"file" 
fileName:@"xx.png" mimeType:@"application/octet-stream" error:nil];
// 文件上传拼接数据的第三种方式（FileURL）
[formData appendPartWithFileURL:fileUrl name:@"file" error:nil];
```
#### Examples 【AFN 序列化处理】
1、AFN 它内部默认把服务器响应的数据当做 JSON来进行解析，所以如果服务器返回给我的不是JSON数据那么请求报错，这个时候需要设置 AFN 对响应信息的解析方式。AFN提供了三种解析响应信息的方式，分别是：
- 
AFHTTPResponseSerializer（默认二进制响应数据，解析方案）
- 
AFJSONResponseSerializer（返回JSON类型，JSON解析方案.默认）
- 
AFXMLParserResponseSerializer（返回XML类型，XML解析）
2、还有一种情况就是服务器返回给我们的数据格式不太一致（查看：开发者工具Content-Type:text/xml），那么这种情况也有可能请求不成功。解决方法:
- 
直接在源代码中修改，添加相应的Content-Type
- 
拿到这个属性，添加到它的集合中。
返回`JSON、XML、二进制、text/xml` 相关代码
```
// 1.创建请求管理者
AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
/* 知识点1：设置AFN采用什么样的方式来解析服务器返回的数据*/
// 注意：如果返回的是XML，那么告诉AFN，响应的时候使用XML的方式解析
manager.responseSerializer = [AFXMLParserResponseSerializer serializer];
// 注意：如果返回的就是二进制数据，那么采用默认二进制的方式来解析数据
//manager.responseSerializer = [AFHTTPResponseSerializer serializer];
// 注意：如果返回的是JSON，采用JSON的方式来解析数据
//manager.responseSerializer = [AFJSONResponseSerializer serializer];
/*知识点2 告诉AFN，再序列化服务器返回的数据的时候，支持此种类型 */
// [AFJSONResponseSerializer serializer].acceptableContentTypes = [NSSet setWithObject:@"text/xml"];
 // 2.把所有的请求参数通过字典的方式来装载，
 // GET方法内部会自动把所有的键值对取出以&符号拼接并最后用？符号连接在请求路径后面
 NSDictionary *dict = @{
                       @"username":@"223",
                       @"pwd":@"ewr",
                       @"type":@"XML"
                        };
 // 3.发送GET请求
[manager GET:@"http://120.25.226.186:32812/login" parameters:nil progress:^(NSProgress * _Nonnull downloadProgress) {
} success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
    // 4.请求成功的回调block
    NSLog(@"请求成功-\n%@",[responseObject class]);
    // 解析
    NSXMLParser *parser = (NSXMLParser *)responseObject;
    parser.delegate = self;// 遵守<NSXMLParserDelegate>
    [parser parse];// 开始解析
} failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
    // 5.请求失败的回调，可以打印error的值查看错误信息
    NSLog(@"请求失败-%@",error);
}];
```
```
#pragma mark - NSXMLParserDelegate
- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName attributes:(NSDictionary<NSString *,NSString *> *)attributeDict {
    if ([elementName isEqualToString:@"videos"]) { return; }
    NSLog(@"开始解析某个元素%@--%@",elementName,attributeDict);
}
```
#### Examples 【AFN 检测网络状态】
使用 AFN 框架 来检测网络状态的改变
```
// 1.创建检测网络状态管理者
AFNetworkReachabilityManager *manager = [AFNetworkReachabilityManager manager];
// 2.检测网络状态改变
/**
 AFNetworkReachabilityStatusUnknown          = -1, 未知
 AFNetworkReachabilityStatusNotReachable     = 0, 无网络
 AFNetworkReachabilityStatusReachableViaWWAN = 1, 蜂窝网络
 AFNetworkReachabilityStatusReachableViaWiFi = 2, WIFI
 */
[manager setReachabilityStatusChangeBlock:^(AFNetworkReachabilityStatus status) {
    switch (status) {
        case AFNetworkReachabilityStatusReachableViaWWAN:
            NSLog(@"蜂窝网络");
            break;
        case AFNetworkReachabilityStatusReachableViaWiFi:
            NSLog(@"WIFI");
            break;
        case AFNetworkReachabilityStatusNotReachable:
            NSLog(@"没有网络");
            break;
        case AFNetworkReachabilityStatusUnknown:
            NSLog(@"未知");
            break;
        default:
            break;
    }
}];
// 3.开始检测
[manager startMonitoring];
```
#### Encapsulation【AFN 使用技巧】
AFN 使用技巧
- 
在开发的时候可以创建一个工具类，继承自我们的 AFN 中的请求管理者，再控制器中真正发请求的代码使用自己封装的工具类。
- 
这样做的优点是以后如果修改了底层依赖的框架，那么我们修改这个工具类就可以了，而不用再一个一个的去修改。
- 
该工具类一般提供一个单例方法，在该方法中会设置一个基本的请求路径。
- 
该方法通常还会提供对 GET或POST 请求的封装。
- 
在外面的时候通过该工具类来发送请求
单例方法示例：
```
+ (instancetype)shareNetworkTools {
    static LNNetworkTools *instance;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        // 注意: BaseURL中一定要以/结尾
        NSURL *url = [NSURL URLWithString:@"http://120.25.226.186:32812/"];
        instance = [[self alloc] initWithBaseURL:url];
    });
    return instance;
}
```
提示：
这里针对AFN 二次封装，我会单开一文学习总结，如果你感兴趣，可否关注 ~
#### Security【AFN 数据安全】
在写项目中，数据的安全性至关重要，而仅仅用 POST 请求提交用户的隐私数据，还是不能完全解决安全问题。
要想非常安全的传输数据，建议使用https。抓包不可以，但是中间人攻击则有可能。建议双向验证防止中间人攻击。
可先参考 [iOS数据安全—加密解密「学习总结」](http://www.jianshu.com/p/1a7d57b60778)
这部分也正在学习，如果你有比较好的解决方式，可否分享一下，学习学习。
#### Download【AFN 离线断点下载】
AFN 3.0 是基于 `NSURLSession` 的。所以实现原理和`NSURLSession` 差不多。可参考 [NSURLSession
 详解使用](http://www.jianshu.com/p/9d6a244e7ecf) 实现断点下载的方法。
这里使用了 `NSURLSessionDataTask`，以便实现「离线断点下载」。
在这里仅供参考（不必拿走直接用）。
看下代码实现：
1、定义下载文件需要用到的类，这里不需要实现代理
```
//-----------------------AFN 断点下载 属性定义--------------------//
//
/** 文件的总长度 */
@property (nonatomic, assign) NSInteger fileLength;
/** 当前下载长度 */
@property (nonatomic, assign) NSInteger currentLength;
/** 文件句柄对象 */
@property (nonatomic, strong) NSFileHandle *fileHandle;
/** 下载任务 */
@property (nonatomic, strong) NSURLSessionDataTask *dataTask;
/* 会话管理者 */
@property (nonatomic, strong) AFURLSessionManager *manager;
/** 获得文件全路径 */
@property (nonatomic, strong) NSString *fullPath;
```
2、添加全局 `AFURLSessionManager`、`NSURLSessionDataTask` 懒加载代码。这里我把实现
 代码 都放这里了。
```
// manager 懒加载
- (AFURLSessionManager *)manager {
    if (!_manager) {
        // 创建会话管理者
        // Configuration:配置信息（用默认的 defaultSessionConfiguration）
        _manager = [[AFURLSessionManager alloc] initWithSessionConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration]];
    }
    return _manager;
}
// downloadTask 懒加载
- (NSURLSessionDataTask *)dataTask {
    if (!_dataTask) {
        // 1.创建下载URL
        NSURL *url = [NSURL URLWithString:@"http://120.25.226.186:32812/resources/videos/minion_01.mp4"];
        // 2.创建request请求
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        // 3.设置请求头信息,Range告诉服务器请求那一部分数据
        NSString *range = [NSString stringWithFormat:@"bytes=%zd-", self.currentLength];
        [request setValue:range forHTTPHeaderField:@"Range"];
        // 4.创建Task
        __weak typeof(self) weakSelf = self;
        _dataTask = [self.manager dataTaskWithRequest:request uploadProgress:nil downloadProgress:^(NSProgress * _Nonnull downloadProgress) {
        } completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
            NSLog(@"File downloaded to: %@",weakSelf.fullPath);
            // 清空长度
            weakSelf.currentLength = 0;
            weakSelf.fileLength = 0;
            // 关闭文件句柄（创建句柄,要记得在完成方法里 关闭句柄置空）
            [weakSelf.fileHandle closeFile];
            weakSelf.fileHandle = nil;
        }];
        // 1.接收到服务器的响应 它默认会取消该请求
        [self.manager setDataTaskDidReceiveResponseBlock:^NSURLSessionResponseDisposition(NSURLSession * _Nonnull session, NSURLSessionDataTask * _Nonnull dataTask, NSURLResponse * _Nonnull response) {
            //NSLog(@"line = %d, %@-->NSURLSessionResponseDisposition",__LINE__,[NSThread currentThread]);
            // 获得下载文件的总长度：本次请求下载的文件长度 + 当前已经下载的文件长度
            weakSelf.fileLength = response.expectedContentLength + self.currentLength;
            // 创建一个空的文件到沙盒中
            NSFileManager *manager = [NSFileManager defaultManager];
            if (![manager fileExistsAtPath:weakSelf.fullPath]) {
                // 如果没有下载文件的话，就创建一个文件。如果有下载文件的话，则不用重新创建(不然会覆盖掉之前的文件)
                [manager createFileAtPath:weakSelf.fullPath contents:nil attributes:nil];
            }
            // 创建文件句柄
            weakSelf.fileHandle = [NSFileHandle fileHandleForWritingAtPath:weakSelf.fullPath];
            // 允许处理服务器的响应，才会继续接收服务器返回的数据
            return NSURLSessionResponseAllow;
        }];
        // 2.接收到服务器返回的数据（调用多次）
        [self.manager setDataTaskDidReceiveDataBlock:^(NSURLSession * _Nonnull session, NSURLSessionDataTask * _Nonnull dataTask, NSData * _Nonnull data) {
            //NSLog(@"line = %d, %@-->setDataTaskDidReceiveDataBlock",__LINE__,[NSThread currentThread]);
            // 指定数据的写入位置 -- 文件内容的最后面
            [weakSelf.fileHandle seekToEndOfFile];
            // 向沙盒写入数据
            [weakSelf.fileHandle writeData:data];
            // 计算文件的下载进度
            weakSelf.currentLength += data.length;
            NSLog(@"下载进度2-->%f",1.0 * self.currentLength / self.fileLength);
            // 获取主线程，不然无法正确显示进度。
            [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                // 下载进度
                if (weakSelf.fileLength == 0) {
                    weakSelf.progressView.progress = 0.0;
                    weakSelf.progressLabel.text = [NSString stringWithFormat:@"当前下载进度:00.00%%"];
                } else {
                    weakSelf.progressView.progress =  1.0 * weakSelf.currentLength / weakSelf.fileLength;
                    weakSelf.progressLabel.text = [NSString stringWithFormat:@"当前下载进度:%.2f%%",100.0 * weakSelf.currentLength / weakSelf.fileLength];
                }
            }];
        }];
    }
    return _dataTask;
}
```
3、懒加载（方法的独立与抽取）
```
// 获得文件全路径
-(NSString *)fullPath {
    if (!_fullPath) {
        // 拼接文件后的本地名称 FileName @"123.mp4" 或者 [url lastPathComponent] 获取URL最后一个字节命名
        _fullPath = [[NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:FileName];
    }
    return _fullPath;
}
// 获得指定文件路径对应文件的数据大小
-(NSInteger)getFileSize {
    NSDictionary *fileInfoDict = [[NSFileManager defaultManager]attributesOfItemAtPath:self.fullPath error:nil];
    //NSLog(@"%@",fileInfoDict);
    NSInteger currentSize = [fileInfoDict[@"NSFileSize"] integerValue];
    return currentSize;
}
注释：
AFN 实现文件
1.开始下载、暂停下载、取消下载、恢复下载
2.支持后台下载
3.在处理下载任务的时候可以直接把数据下载到磁盘
4.下载的时候﻿是子线程异步处理，效率更高
```
#### 网络请求所有的学习总结效果
![](http://upload-images.jianshu.io/upload_images/2230763-74379c6ddf4b4716.gif?imageMogr2/auto-orient/strip)
iOS网络请求学习总结.gif
#### 相关「所有学习总结都在这里了」
- [AFN 【框架 | 源码 学习总结】](http://www.jianshu.com/p/519611e875cd)
#### 期待
- 
如果在阅读过程中遇到 error，希望你能 Issues 我，谢谢。
- 
如果你想为【本文相关】分享点什么，也希望你能 Issues 我，我非常想为这篇文章增加更多实用的内容，谢谢。
- 
[「博客原文」](https://custompbwaters.github.io/2017/02/18/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.0+%EF%BC%89%E6%A1%86%E6%9E%B6%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%B8%89%E6%A1%86%E6%9E%B6%E5%9F%BA%E6%9C%AC%E4%BD%BF%E7%94%A8)/)，对本文我会【不定时、持续更新、一些
 学习心得与文章、实用才是硬道理】^_^.
- 
