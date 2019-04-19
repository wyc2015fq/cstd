# AFNetworking（v3.0+）框架学习总结(二内部逻辑处理过程) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月19日 16:02:55[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1587
#### 本文来自简书，原文地址:[http://www.jianshu.com/p/4f1818568bc4](http://www.jianshu.com/p/4f1818568bc4)
#### Write in the first【写在最前】
AFNetWorking 基本是iOS开发中使用网络通信框架的标配，这个框架本身比较庞大，也很复杂，但是使用起来非常非常简单。
本篇文章主要从【AFN 内部逻辑处理过程】以Get请求为例，学习总结，
时间有限，知识并未全覆盖，有遗漏或者错误，忘指正。
![](http://upload-images.jianshu.io/upload_images/2230763-52f44551d97d1941.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
 AFNetworking
> 
目录：
- AFN GET内部逻辑处理
- AFN POST内部逻辑处理
- 总结
- 后续【AFN 框架 | 源码 学习总结】
#### Business logic【AFN GET 内部逻辑处理】
这是 AFNetworking 发起一个 Get 请求的流程图，大概可以分为这几个步骤，下面会逐个解读这个流程。
![](http://upload-images.jianshu.io/upload_images/2230763-50cd9ac10597d305.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
AFN-->GET业务逻辑处理.png
#### 1. AFHTTPSessionManager 发起GET请求
![](http://upload-images.jianshu.io/upload_images/2230763-ce63bee95c1191a1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
manager-->GET请求
这个方法是 AFN 的 Get请求 的起点，其他 Get 请求的方法也都是直接或者间接调用这个方法来发起 Get 请求。这个方法的代码量很少也很直观，就是调用其他方法生成 `NSURLSessionDataTask`对象的实例，然后调用 `NSURLSessionDataTask` 的 `resume` 方法发起请求。
#### 2. 创建 NSURLSessionDataTask
![](http://upload-images.jianshu.io/upload_images/2230763-5283f20a36d3446e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建-->NSURLSessionDataTask
这个方法是创建 `NSURLSessionDataTask` 对象实例并返回这个实例。首先创建一个 `NSMutableURLRequest` 对象的实例，然后配置。之后是使用 `NSMutableURLRequest` 对象的实例创建`NSURLSessionDataTask` 对象实例，然后配置，可以选择性地传入各类`Block`回调，用于监听网络请求的进度比如上传进度，下载进度，请求成功，请求失败。
#### 3. 配置 NSMutableURLRequest对象
![](http://upload-images.jianshu.io/upload_images/2230763-e08acd0c3f8ddc99.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
配置-->NSMutableURLRequest对象
在这个方法中先使用了 url 创建了一个 `NSMutableURLRequest` 对象的实例,并且设置了 `HTTPMethod` 为 `Get` 方法（如果是Post方法，那么这里就是设置Post方法）然后使用KVC的方法设置了 `NSMutableURLRequest` 的一些属性。
```
// 设置 NSMutableURLRequest 的属性
static NSArray * AFHTTPRequestSerializerObservedKeyPaths() {
    static NSArray *_AFHTTPRequestSerializerObservedKeyPaths = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        //allowsCellularAccess 允许使用数据流量
        //cachePolicy 缓存策略
        //HTTPShouldHandleCookies 处理Cookie
        //HTTPShouldUsePipelining 批量请求
        //networkServiceType 网络状态
        //timeoutInterval 超时
        _AFHTTPRequestSerializerObservedKeyPaths = @[NSStringFromSelector(@selector(allowsCellularAccess)), NSStringFromSelector(@selector(cachePolicy)), NSStringFromSelector(@selector(HTTPShouldHandleCookies)), NSStringFromSelector(@selector(HTTPShouldUsePipelining)), NSStringFromSelector(@selector(networkServiceType)), NSStringFromSelector(@selector(timeoutInterval))];
    });
    return _AFHTTPRequestSerializerObservedKeyPaths;
}
```
![](http://upload-images.jianshu.io/upload_images/2230763-60291964bad44b91.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
配置-->NSMutableURLRequest对象
先设置 HTTP header，之后格式化请求参数，设置参数的编码类型。这个是这个方法的基本操作流程。对于Get操作来说，参数是直接拼接在请求地址后面。
#### 4. 配置 NSURLSessionDataTask对象
![](http://upload-images.jianshu.io/upload_images/2230763-b23c86c80e838520.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
配置-->NSURLSessionDataTask对象
之后配置 `NSMutableURLRequest` 对象就需要配置 `NSURLSessionDataTask` 对象了。主要分为2个步骤，第一个步骤是创建 `NSURLSessionDataTask` 对象实例，第二个步骤是给`NSURLSessionDataTask` 对象实例设置 `Delegate`。用于实时了解网络请求的过程。
![](http://upload-images.jianshu.io/upload_images/2230763-457c5cdc1218d891.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
给NSURLSessionDataTask对象实例设置Delegate.png
AFN 的代理统一使用 `AFURLSessionManagerTaskDelegate` 对象来管理，使用 `AFURLSessionManagerTaskDelegate` 对象来接管`NSURLSessionTask` 网络请求过程中的回调，然后再传入 `AFN` 内部进行管理。
```
@interface AFURLSessionManagerTaskDelegate : NSObject <NSURLSessionTaskDelegate, 
NSURLSessionDataDelegate, NSURLSessionDownloadDelegate>
```
如代码所示 `AFURLSessionManagerTaskDelegate` 接管了`NSURLSessionTaskDelegate`，`NSURLSessionDataDelegate`，`NSURLSessionDownloadDelegate`的各种回调，然后做内部处理。这也是第三方网络请求框架的重点，让网络请求更加易用，好用。
```
// 通过 task 的标识符管理代理
- (void)setDelegate:(AFURLSessionManagerTaskDelegate *)delegate
            forTask:(NSURLSessionTask *)task
{
    NSParameterAssert(task);
    NSParameterAssert(delegate);
    [self.lock lock];
    // 将task和代理类绑定，task的taskIdentifier作为字典的key,delegate作为字典的value
    self.mutableTaskDelegatesKeyedByTaskIdentifier[@(task.taskIdentifier)] = delegate;
    // 给该task添加两个KVO事件（Resume 和 Suspend）
    [self addNotificationObserverForTask:task];
    [self.lock unlock];
}
```
通过`NSURLSessionTask`的`taskIdentifier`标识符对`delegate`进行管理，只要是用于识别该`NSURLSessionTask`的代理。
![](http://upload-images.jianshu.io/upload_images/2230763-bf27849f4cfa33ed.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
NSURLSessionTask 设置进度回调
设置各类回调 Block,给 NSURLSessionTask 使用 KVO 进行各种过程进度监听。
```
#pragma mark -
// 给task添加暂停和恢复的通知
- (void)addNotificationObserverForTask:(NSURLSessionTask *)task {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(taskDidResume:) name:AFNSURLSessionTaskDidResumeNotification object:task];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(taskDidSuspend:) name:AFNSURLSessionTaskDidSuspendNotification object:task];
}
```
监听 `NSURLSessionTask` 被挂起 和 恢复的通知。
#### 5. 网络请求开始
```
// 发送GET请求
/**
 GET: 请求路径(不包含参数),url
 parameters: 字典(发送给服务器的数据~参数)
 progress: 进度回调
 success: 成功回调（task:请求任务、responseObject:响应体信息JSON->OC对象）
 failure: 失败回调（error:错误信息）
 task.response: 响应头
 */
- (NSURLSessionDataTask *)GET:(NSString *)URLString
                   parameters:(id)parameters
                     progress:(void (^)(NSProgress * _Nonnull))downloadProgress
                      success:(void (^)(NSURLSessionDataTask * _Nonnull, id _Nullable))success
                      failure:(void (^)(NSURLSessionDataTask * _Nullable, NSError * _Nonnull))failure
{
    NSURLSessionDataTask *dataTask = [self dataTaskWithHTTPMethod:@"GET"
                                                        URLString:URLString
                                                       parameters:parameters
                                                   uploadProgress:nil
                                                 downloadProgress:downloadProgress
                                                          success:success
                                                          failure:failure];
    [dataTask resume];
    return dataTask;
}
```
当 `NSURLSessionTask` 创建和配置完毕之后，它并不会主动执行，而是需要我们主动调用 `resume` 方法，`NSURLSessionTask` 才会开始执行。
#### 6. 网络请求回调
![](http://upload-images.jianshu.io/upload_images/2230763-6db56d320d5b1814.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
NSURLSessionDelegate 方法
![](http://upload-images.jianshu.io/upload_images/2230763-71c4e889d1e81a66.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
NSURLSessionTaskDelegate 方法
AFN 里面有关 `NSURLSessionDelegate` 的回调方法非常的多，这里我们只说和 `NSURLSessionTask` 相关的部分方法和 `KVO` 处理来进行说明，其他的大家可以参考源码细看。
![](http://upload-images.jianshu.io/upload_images/2230763-7eeef076feb91c74.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
KVO监听请求过程.png
![](http://upload-images.jianshu.io/upload_images/2230763-d1e3bd92a9398065.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
收到响应数据.png
![](http://upload-images.jianshu.io/upload_images/2230763-a9a82b1054d5fe85.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
请求完成.png
对于我们的 Get请求 来说，我们最关注的莫过于关注请求过程进度，收到响应数据和请求完成这2个回调。
KVO监听的属性值发生变化：
```
// KVO监听的属性值发生变化
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSString *,id> *)change context:(void *)context {
    if ([object isKindOfClass:[NSURLSessionTask class]] || [object isKindOfClass:[NSURLSessionDownloadTask class]]) {
        if ([keyPath isEqualToString:NSStringFromSelector(@selector(countOfBytesReceived))]) {
            NSLog(@"countOfBytesReceived");
            // 这个是在Get请求下，网络响应过程中已经收到的数据量
            // 已经收到
            self.downloadProgress.completedUnitCount = [change[NSKeyValueChangeNewKey] longLongValue];
        } else if ([keyPath isEqualToString:NSStringFromSelector(@selector(countOfBytesExpectedToReceive))]) {
            NSLog(@"countOfBytesExpectedToReceive");
            // 这个是在Get请求下，网络响应过程中期待收到的数据量
            // 期待收到
            self.downloadProgress.totalUnitCount = [change[NSKeyValueChangeNewKey] longLongValue];
        } else if ([keyPath isEqualToString:NSStringFromSelector(@selector(countOfBytesSent))]) {
            NSLog(@"countOfBytesSent");
            // 已经发送
            self.uploadProgress.completedUnitCount = [change[NSKeyValueChangeNewKey] longLongValue];
        } else if ([keyPath isEqualToString:NSStringFromSelector(@selector(countOfBytesExpectedToSend))]) {
            NSLog(@"countOfBytesExpectedToSend");
            // 期待发送
            self.uploadProgress.totalUnitCount = [change[NSKeyValueChangeNewKey] longLongValue];
        }
    }
    else if ([object isEqual:self.downloadProgress]) {
        // 下载进度变化
        if (self.downloadProgressBlock) {
            self.downloadProgressBlock(object);
        }
    }
    else if ([object isEqual:self.uploadProgress]) {
        // 上传进度变化
        if (self.uploadProgressBlock) {
            self.uploadProgressBlock(object);
        }
    }
}
```
收到请求响应：
```
// 收到请求响应
- (void)URLSession:(NSURLSession *)session
          dataTask:(NSURLSessionDataTask *)dataTask
          didReceiveResponse:(NSURLResponse *)response
          completionHandler:(void (^)(NSURLSessionResponseDisposition disposition))completionHandler {
    NSLog(@"收到请求响应");
    // 允许处理服务器的响应，才会继续接收服务器返回的数据
    NSURLSessionResponseDisposition disposition = NSURLSessionResponseAllow;
    // 是否有收到请求响应的回调Block
    if (self.dataTaskDidReceiveResponse) {
        // 若有调用该Block
        disposition = self.dataTaskDidReceiveResponse(session, dataTask, response);
    }
    // 是否有请求响应完成的回调Block
    if (completionHandler) {
        // 若有调用该Block
        completionHandler(disposition);
    }
}
```
请求完成：
```
// 请求完成
- (void)URLSession:(NSURLSession *)session
              task:(NSURLSessionTask *)task
              didCompleteWithError:(NSError *)error {
    NSLog(@"请求完成");
    // 取出该NSURLSessionTask的代理对象
    AFURLSessionManagerTaskDelegate *delegate = [self delegateForTask:task];
    // delegate may be nil when completing a task in the background
    if (delegate) {
        // 若是该代理对象存在，那么将对应数据转给该代理对象处理
        [delegate URLSession:session task:task didCompleteWithError:error];
        // NSURLSessionTask任务完成之后，移除该NSURLSessionTask的代理对象
        [self removeDelegateForTask:task];
    }
    // 是否有请求完成的回调Block
    if (self.taskDidComplete) {
        // 若有调用改Block
        self.taskDidComplete(session, task, error);
    }
}
```
因为在配置 NSURLSessionDataTask 对象的时候我们有给 NSURLSessionTask 做了一系列配置，那么当 NSURLSessionDataTask 任务完成之后，我们需要将该 NSURLSessionDataTask 的一系列配置全部清理掉。
这个是我们的配置过程：
```
// 通过task的标识符管理代理
- (void)setDelegate:(AFURLSessionManagerTaskDelegate *)delegate
            forTask:(NSURLSessionTask *)task
{
    NSParameterAssert(task);
    NSParameterAssert(delegate);
    [self.lock lock];
    self.mutableTaskDelegatesKeyedByTaskIdentifier[@(task.taskIdentifier)] = delegate;
    [delegate setupProgressForTask:task];
    [self addNotificationObserverForTask:task];
    [self.lock unlock];
}
```
那么对应的清理过程是这样的，就是设置过程中做了什么，在清理过程中就需要去掉什么。
```
// 给task移除delegate
- (void)removeDelegateForTask:(NSURLSessionTask *)task {
    NSParameterAssert(task);
    AFURLSessionManagerTaskDelegate *delegate = [self delegateForTask:task];
    [self.lock lock];
    [delegate cleanUpProgressForTask:task];
    [self removeNotificationObserverForTask:task];
    [self.mutableTaskDelegatesKeyedByTaskIdentifier removeObjectForKey:@(task.taskIdentifier)];
    [self.lock unlock];
}
```
![](http://upload-images.jianshu.io/upload_images/2230763-81bdec38d280ee1c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
cleanUpProgressForTask.png
![](http://upload-images.jianshu.io/upload_images/2230763-ec45f15cbc7cf786.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
removeNotificationObserverForTask.png
#### Business logic【AFN POST内部逻辑处理】
![](http://upload-images.jianshu.io/upload_images/2230763-ecb59ae9e6d82be3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
请求序列化方法
```
#pragma mark - AFURLRequestSerialization
// 设置Header和请求参数
- (NSURLRequest *)requestBySerializingRequest:(NSURLRequest *)request
                               withParameters:(id)parameters
                                        error:(NSError *__autoreleasing *)error
{
    NSParameterAssert(request);
    NSMutableURLRequest *mutableRequest = [request mutableCopy];
    [self.HTTPRequestHeaders enumerateKeysAndObjectsUsingBlock:^(id field, id value, BOOL * __unused stop) {
        // 判断header的field是否存在,如果不存在则设置，存在则跳过
        if (![request valueForHTTPHeaderField:field]) {
            // 设置 header
            [mutableRequest setValue:value forHTTPHeaderField:field];
        }
    }];
    NSString *query = nil;
    if (parameters) {
        // 用传进来的自定义block格式化请求参数
        if (self.queryStringSerialization) {
            NSError *serializationError;
            query = self.queryStringSerialization(request, parameters, &serializationError);
            if (serializationError) {
                if (error) {
                    *error = serializationError;
                }
                return nil;
            }
        } else {
            switch (self.queryStringSerializationStyle) {
                case AFHTTPRequestQueryStringDefaultStyle:
                    // 默认的格式化方式
                    query = AFQueryStringFromParameters(parameters);
                    break;
            }
        }
    }
    // 判断是否是GET/HEAD/DELETE方法， 对于GET/HEAD/DELETE方法，把参数加到URL后面
    if ([self.HTTPMethodsEncodingParametersInURI containsObject:[[request HTTPMethod] uppercaseString]]) {
        // 判断是否有参数
        if (query && query.length > 0) {
            // 拼接请求参数
            NSLog(@"query-->%@",query);
            mutableRequest.URL = [NSURL URLWithString:[[mutableRequest.URL absoluteString] stringByAppendingFormat:mutableRequest.URL.query ? @"&%@" : @"?%@", query]];
        }
    } else {
        // #2864: an empty string is a valid x-www-form-urlencoded payload
        if (!query) {
            query = @"";
        }
        // 参数带在body上，大多是POST PUT
        if (![mutableRequest valueForHTTPHeaderField:@"Content-Type"]) {
            // 设置Content-Type HTTP头，告诉服务端body的参数编码类型
            [mutableRequest setValue:@"application/x-www-form-urlencoded" forHTTPHeaderField:@"Content-Type"];
        }
        [mutableRequest setHTTPBody:[query  dataUsingEncoding:self.stringEncoding]];
    }
    return mutableRequest;
}
```
如果是 Post 请求，那么请求参数是没有拼接在 URL 上面，而是放在 `body` 上，这是 Post
 和 Get 请求的最大区别了，其他过程和Get 请求并没有太多区别。
#### 总结
AFN发起Get请求主要分为以下步骤：
- 
1.创建`NSURLSessionDataTask`
- 
2.配置`NSURLSessionDataTask`
- 
3.设置`NSURLSessionDataTask的Delegate`
- 
4.调用`NSURLSessionDataTask`的`resume`方法开始请求
- 
5.在`Delegate`的方法里面处理网络请求的各个过程
- 
6.清理`NSURLSessionDataTask`的配置
其实也就是使用 `NSURLSessionDataTask` 的步骤，AFN在这几个步骤加了一些封装，让我们的使用更简单。
【转载原著】[iOS网络框架－AFNetworking3.1.0源码解读](http://www.jianshu.com/p/c36159094e24)
#### 相关「所有学习总结都在这里了」
- [AFN 【框架 | 源码 学习总结】](http://www.jianshu.com/p/519611e875cd)
#### 期待
- 
如果在阅读过程中遇到 error，希望你能 Issues 我，谢谢。
- 
如果你想为【本文相关】分享点什么，也希望你能 Issues 我，我非常想为这篇文章增加更多实用的内容，谢谢。
- 
[「博客原文」](https://custompbwaters.github.io/2017/02/17/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.1.0%EF%BC%89%E6%A1%86%E6%9E%B6%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%BA%8C%E5%86%85%E9%83%A8%E9%80%BB%E8%BE%91%E5%A4%84%E7%90%86%E8%BF%87%E7%A8%8B)/)，对本文我会【不定时、持续更新、一些
 学习心得与文章、实用才是硬道理】^_^.
