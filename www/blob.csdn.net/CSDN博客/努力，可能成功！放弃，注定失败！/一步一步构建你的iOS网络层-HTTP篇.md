# 一步一步构建你的iOS网络层 - HTTP篇 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年02月17日 11:45:55[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：583
# 本文来自简书，原文地址:[http://www.jianshu.com/p/f9b4ada163ab](http://www.jianshu.com/p/f9b4ada163ab)
# 缘起
经常跑步的人一定知道, 每次跑步时间过了15分钟以后, 脑子里通常就开始出现: 青楼花坊, 白衣神王, 世界未亡, 誓不投降之类的中二遐想. 然而年假过后, 这些想法却突然销声匿迹, 一些iOS知识点和往期项目经验却开始浮上心头, 莫名其妙居然自己排列组合出了七八篇文章. 这些奇怪的想法渐渐不满足于在跑步时叨扰大脑, 更是在公交地铁商业街也跑出来嗡嗡细吟: 写下来吧, 写下来嘛...
这想法就像大二兼职时偶遇的小姐姐, 越是努力不去回忆, 她就愈发美丽动人......
你想写, 那便写罢.
## 前言
本文参考casatwy先生的网络层架构设计从网络请求的构建到请求结果的处理为你概述如何构建一个方便易用的iOS网络层, 全文约8千字, 预计花费阅读时间20 - 30分钟.
## 目录
- 
网络请求的构建
- 
网络请求的派发
1.请求的派发与取消
2.多服务器的切换
- 
合理的使用请求派发器
1.协议还是配置对象?
2.简单的请求结果缓存器
3.请求结果的格式化
4.两个小玩意儿
### 一.网络请求的构建
网络请求的构建很简单, 根据一个请求需要的条件如URL, 请求方式, 请求参数, 请求头等定义请求生成的接口即可. 定义如下:
```
@interface HHURLRequestGenerator : NSObject
+ (instancetype)sharedInstance;
- (void)switchService;
- (void)switchToService:(HHServiceType)serviceType;
- (NSMutableURLRequest *)generateRequestWithUrlPath:(NSString *)urlPath
                                           useHttps:(BOOL)useHttps
                                             method:(NSString *)method
                                             params:(NSDictionary *)params
                                             header:(NSDictionary *)header;
- (NSMutableURLRequest *)generateUploadRequestUrlPath:(NSString *)urlPath
                                             useHttps:(BOOL)useHttps
                                               params:(NSDictionary *)params
                                             contents:(NSArray<HHUploadFile *> *)contents
                                               header:(NSDictionary *)header;
@end
```
可以看到方法参数都是生成请求基本组成部分, 当然, 这里的参数比较少, 因为在我的项目中像请求超时时间都是一样的, 类似这些公用的设置我都偷懒直接写在请求配置文件里面了. 我们看看请求接口的具体实现, 以数据请求为例:
```
- (NSMutableURLRequest *)generateRequestWithUrlPath:(NSString *)urlPath useHttps:(BOOL)useHttps method:(NSString *)method params:(NSDictionary *)params header:(NSDictionary *)header {
    NSString *urlString = [self urlStringWithPath:urlPath useHttps:useHttps];
    NSMutableURLRequest *request = [self.requestSerialize requestWithMethod:method URLString:urlString parameters:params error:nil];
    request.timeoutInterval = RequestTimeoutInterval;
    [self setCookies];//设置cookie
    [self setCommonRequestHeaderForRequest:request];// 在这里做公用请求头的设置
    [header enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull value, BOOL * _Nonnull stop) {
        [request setValue:value forHTTPHeaderField:key];
    }];
    return request;
}
```
```
- (NSString *)urlStringWithPath:(NSString *)path useHttps:(BOOL)useHttps {
    if ([path hasPrefix:@"http"]) {
        return path;
    } else {
        NSString *baseUrlString = [HHService currentService].baseUrl;
        if (useHttps && baseUrlString.length > 4) {
            NSMutableString *mString = [NSMutableString stringWithString:baseUrlString];
            [mString insertString:@"s" atIndex:4];
            baseUrlString = [mString copy];
        }
        return [NSString stringWithFormat:@"%@%@", baseUrlString, path];
    }
}
```
代码很简单, 接口根据参数调用urlStringWithPath:useHttps:通过BaseURL和URLPath拼装出完整的URL, 然后用这个URL和其他参数生成一个URLRequest, 然后调用setCommonRequestHeaderForRequest:设置公用请求, 最后返回这个URLRequest.
BaseURL来自HHService, HHService对外暴露各个环境(测试/开发/发布)下的baseURL和切换服务器的接口, 内部走工厂生成当前的服务器, 我的设置是默认连接第一个服务器且APP关闭后恢复此设置, APP运行中可根据需要调用switchService切换服务器.
HHService定义如下:
```
@protocol HHService <NSObject>
@optional
- (NSString *)testEnvironmentBaseUrl;
- (NSString *)developEnvironmentBaseUrl;
- (NSString *)releaseEnvironmentBaseUrl;
@end
@interface HHService : NSObject<HHService>
+ (HHService *)currentService;
+ (void)switchService;
+ (void)switchToService:(HHServiceType)serviceType;
- (NSString *)baseUrl;
- (HHServiceEnvironment)environment;
@end
```
```
#import "HHService.h"
@interface HHService ()
@property (assign, nonatomic) HHServiceType type;
@property (assign, nonatomic) HHServiceEnvironment environment;
@end
@interface HHServiceX : HHService
@end
@interface HHServiceY : HHService
@end
@interface HHServiceZ : HHService
@end
@implementation HHService
#pragma mark - Interface
static HHService *currentService;
static dispatch_semaphore_t lock;
+ (HHService *)currentService {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        lock = dispatch_semaphore_create(1);
        currentService = [HHService serviceWithType:HHService0];
    });
    return currentService;
}
+ (void)switchService {
    [self switchToService:self.currentService.type + 1];
}
+ (void)switchToService:(HHServiceType)serviceType {
    dispatch_semaphore_wait(lock, DISPATCH_TIME_FOREVER);
    currentService = [HHService serviceWithType:(serviceType % ServiceCount)];
    dispatch_semaphore_signal(lock);
}
+ (HHService *)serviceWithType:(HHServiceType)type {
    HHService *service;
    switch (type) {
        case HHService0: service = [HHServiceX new];  break;
        case HHService1: service = [HHServiceY new];  break;
        case HHService2: service = [HHServiceZ new];  break;
    }
    service.type = type;
    service.environment = BulidServiceEnvironment;
    return service;
}
- (NSString *)baseUrl {
    switch (self.environment) {
        case HHServiceEnvironmentTest: return [self testEnvironmentBaseUrl];
        case HHServiceEnvironmentDevelop: return [self developEnvironmentBaseUrl];
        case HHServiceEnvironmentRelease: return [self releaseEnvironmentBaseUrl];
    }
}
@end
```
### 2.网络请求的派发
请求的派发是通过一个单例HHNetworkClient来实现的, 如果把请求比作炮弹的话, 那么这个单例就是发射炮弹的炮台, 使用炮台的人只需要告诉炮台需要发射什么样的炮弹和炮弹的打击目标便可发射了. 另外, 应该提供取消打击的功能以处理不必要的打击的情况, 那么, 根据炮台的作用.
HHNetworkClient定义如下:
```
@interface HHNetworkClient : NSObject
+ (instancetype)sharedInstance;
- (NSURLSessionDataTask *)dataTaskWithUrlPath:(NSString *)urlPath
                                     useHttps:(BOOL)useHttps
                                  requestType:(HHNetworkRequestType)requestType
                                       params:(NSDictionary *)params
                                       header:(NSDictionary *)header
                            completionHandler:(void (^)(NSURLResponse *response,id responseObject,NSError *error))completionHandler;
- (NSNumber *)dispatchTaskWithUrlPath:(NSString *)urlPath
                             useHttps:(BOOL)useHttps
                          requestType:(HHNetworkRequestType)requestType
                               params:(NSDictionary *)params
                               header:(NSDictionary *)header
                    completionHandler:(void (^)(NSURLResponse *response,id responseObject,NSError *error))completionHandler;
- (NSNumber *)dispatchTask:(NSURLSessionTask *)task;
- (NSNumber *)uploadDataWithUrlPath:(NSString *)urlPath
                           useHttps:(BOOL)useHttps
                             params:(NSDictionary *)params
                           contents:(NSArray<HHUploadFile *> *)contents
                             header:(NSDictionary *)header
                    progressHandler:(void(^)(NSProgress *))progressHandler
                  completionHandler:(void (^)(NSURLResponse *response,id responseObject,NSError *error))completionHandler;
- (void)cancelAllTask;
- (void)cancelTaskWithTaskIdentifier:(NSNumber *)taskIdentifier;
@end
```
```
@interface HHNetworkClient ()
@property (strong, nonatomic) AFHTTPSessionManager *sessionManager;
@property (strong, nonatomic) NSMutableDictionary<NSNumber *, NSURLSessionTask *> *dispathTable;
@property (assign, nonatomic) CGFloat totalTaskCount;
@property (assign, nonatomic) CGFloat errorTaskCount;
@end
```
1.请求的派发与取消
外部暴露数据请求和文件上传的接口, 参数为构建请求所需的必要参数, 返回值为此次请求任务的taskIdentifier, 调用方可以通过taskIdentifier取消正在执行的请求任务.
内部声明一个dispathTable保持着此时正在执行的任务, 并在任务执行完成或者任务取消时移除任务的引用, 以数据请求为例, 具体实现如下:
```
- (NSURLSessionDataTask *)dataTaskWithUrlPath:(NSString *)urlPath useHttps:(BOOL)useHttps requestType:(HHNetworkRequestType)requestType params:(NSDictionary *)params header:(NSDictionary *)header completionHandler:(void (^)(NSURLResponse *, id, NSError *))completionHandler {
    NSString *method = (requestType == HHNetworkRequestTypeGet ? @"GET" : @"POST");
    NSMutableURLRequest *request = [[HHURLRequestGenerator sharedInstance] generateRequestWithUrlPath:urlPath useHttps:useHttps method:method params:params header:header];
    NSMutableArray *taskIdentifier = [NSMutableArray arrayWithObject:@-1];
    NSURLSessionDataTask *task = [self.sessionManager dataTaskWithRequest:request completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
        dispatch_semaphore_wait(lock, DISPATCH_TIME_FOREVER);
        [self checkSeriveWithTaskError:error];
        [self.dispathTable removeObjectForKey:taskIdentifier.firstObject];
        dispatch_semaphore_signal(lock);
        completionHandler ? completionHandler(response, responseObject, error) : nil;
    }];
    taskIdentifier[0] = @(task.taskIdentifier);
    return task;
}
- (NSNumber *)dispatchTaskWithUrlPath:(NSString *)urlPath useHttps:(BOOL)useHttps requestType:(HHNetworkRequestType)requestType params:(NSDictionary *)params header:(NSDictionary *)header completionHandler:(void (^)(NSURLResponse *, id, NSError *))completionHandler {
    return [self dispatchTask:[self dataTaskWithUrlPath:urlPath useHttps:useHttps requestType:requestType params:params header:header completionHandler:completionHandler]];
}
- (NSNumber *)dispatchTask:(NSURLSessionDataTask *)task {
    if (task == nil) { return @-1; }
    dispatch_semaphore_wait(lock, DISPATCH_TIME_FOREVER);
    self.totalTaskCount += 1;
    [self.dispathTable setObject:task forKey:@(task.taskIdentifier)];
    dispatch_semaphore_signal(lock);
    [task resume];
    return @(task.taskIdentifier);
}
```
代码很简单, 通过参数生成URLRequest, 然后通过AFHTTPSessionManager执行任务, 在任务执行前我们以task.taskIdentifier为key保持一下执行的任务, 然后在任务执行后我们移除这个任务, 当然, 外部也可以在必要的时候通过我们返回的task.taskIdentifier手动移除任务.
注意我们先声明一个NSMutableArray来标志taskIdentifier, 然后在任务生成后设置taskIdentifier[0]为task. taskIdentifier, 最后在任务完成的回调block中使用taskIdentifier[0]来移除这个已经完成的任务.
可能有人会有疑问为什么不直接使用task.taskIdentifier, block不是可以捕获task吗? 下面解释一下为什么这样写:
我们知道block之于函数最大的区别就在于它可以捕获自身作用域外的对象, 并在block执行的时候访问被捕获的对象, 具体的, 对于值类型对象block会生成一份此对象的拷贝, 对于引用类型对象block会生成一个此对象的引用并使该对象的引用计数+1(这里我们只描述非__block修饰的情况). 那么代入到上面的代码, 我们来一步一步分析:
- 
直接捕获task的写法
```
NSURLSessionDataTask *task = [self.sessionManager dataTaskWithRequest:request completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
...略
     [self.dispathTable removeObjectForKey:@(task.taskIdentifier)];
...略
 }];
[self.dispathTable setObject:task forKey:@(task.taskIdentifier)];
```
我们把它拆开来看:
```
NSURLSessionDataTask *task; 
NSURLSessionDataTask *returnTask = [self.sessionManager dataTaskWithRequest:request completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
...略
     [self.dispathTable removeObjectForKey:@(task.taskIdentifier)];
...略
 }];
task =  returnTask;
[self.dispathTable setObject:task forKey:@(task.taskIdentifier)];
```
可以看到returnTask是我们实际存储的任务, 而task只是一个临时变量, 此时task指向nil, 那我们生成returnTask的block此时捕获到的task也就是nil, 所以在任务完成的时候我们的task.taskIdentifier一定是0, 这样写的结果就是dispathTable只会添加不会删除(系统的taskIdentifier是从0开始依次递增的), 当然, 因为进行中的returnTask我们是做了存储的, 所以在任务未完成的时候我们还是可以做取消的.
- 
如果一开始给task一个占位对象呢不让它为nil可以吗?
```
NSURLSessionDataTask *task = [NSObject new]; //1.suspend
NSURLSessionDataTask *returnTask = [self.sessionManager dataTaskWithRequest:request completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
...略
      [self.dispathTable removeObjectForKey:@(task.taskIdentifier)];//3.completed
...略
  }];//2.alloc
task =  returnTask;
[self.dispathTable setObject:task forKey:@(task.taskIdentifier)];
```
这样其实就是一个简单的引用变换题了, 我们来看看各个指针的指向情况:
suspend: pTask->NSObject block.pTask->nil pReturnTask->nil
alloc: pTask-> NSObject block.pTask->NSObject pReturnTask->returnTask
completed: pTask->returnTask block.pTask->NSObject pReturnTask->returnTask
可以看到在任务执行完成时我们访问block.pTask时也不过是我们一开始的占位对象, 所以这个方案也不行, 当然, 取消任务依然可用
事实上block.pTask确实是捕获了占位对象, 只是我们在那之后没有替换block.pTask指向到returnTask, 然而block.pTask我们是访问不了的, 所以这个方案行不通.
- 
如果我们的占位对象是一个容器呢?
```
NSMutableArray *taskIdentifier = [NSMutableArray arrayWithObject:@-1];
NSURLSessionDataTask *returnTask = [self.sessionManager dataTaskWithRequest:request completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
...略
      [self.dispathTable removeObjectForKey:@(taskIdentifier.firstObject)];
...略
  }];
taskIdentifier[0] = @(returnTask.taskIdentifier);
[self.dispathTable setObject:task forKey:@(task.taskIdentifier)];
```
既然我们访问不了block.pTask那就访问block.pTask指向的对象嘛, 更改这个对象的内容不就相当于更改了block.pTask么, 大家照着2的思路走一下应该很容易就能想通, 我就不多说了.
2.多服务器的切换
关于多服务器其实我也没有实际的经验, 公司正在部署第二台服务器, 具体需求是如果访问第一台服务器总是超时或者出错, 那就切换到第二台服务器, 基于此需求我简单的实现一下:
```
- (NSNumber *)dispatchTask:(NSURLSessionDataTask *)task {
    ...略
    dispatch_semaphore_wait(lock, DISPATCH_TIME_FOREVER);
    self.totalTaskCount += 1;
    [self.dispathTable setObject:task forKey:@(task.taskIdentifier)];
    dispatch_semaphore_signal(lock);
    ...略
}
```
```
- (NSURLSessionDataTask *)dataTaskWithUrlPath:(NSString *)urlPath useHttps:(BOOL)useHttps requestType:(HHNetworkRequestType)requestType params:(NSDictionary *)params header:(NSDictionary *)header completionHandler:(void (^)(NSURLResponse *, id, NSError *))completionHandler {
    NSString *method = (requestType == HHNetworkRequestTypeGet ? @"GET" : @"POST");
    ...略
    NSURLSessionDataTask *task = [self.sessionManager dataTaskWithRequest:request completionHandler:^(NSURLResponse * _Nonnull response, id  _Nullable responseObject, NSError * _Nullable error) {
       ...略
        [self checkSeriveWithTaskError:error];
       ...略
    }];
    ...略
}
```
```
- (void)checkSeriveWithTaskError:(NSError *)error {
    if ([HHAppContext sharedInstance].isReachable) {
        switch (error.code) {
            case NSURLErrorUnknown:
            case NSURLErrorTimedOut:
            case NSURLErrorCannotConnectToHost: {
                self.errorTaskCount += 1;
            }
            default:break;
        }
        if (self.totalTaskCount >= 40 && (self.errorTaskCount / self.totalTaskCount) == 0.1) {
            self.totalTaskCount = self.errorTaskCount = 0;
            [[HHURLRequestGenerator sharedInstance] switchService];
        }
    }
}
```
```
- (void)didReceivedSwitchSeriveNotification:(NSNotification *)notif {
    dispatch_semaphore_wait(lock, DISPATCH_TIME_FOREVER);
    self.totalTaskCount = self.errorTaskCount = 0;
    dispatch_semaphore_signal(lock);
    [[HHURLRequestGenerator sharedInstance] switchToService:[notif.userInfo[@"service"] integerValue]];
}
```
假设认为APP在此次使用过程中网络任务的错误率达到10%那就应该切换一下服务器, 我们在任务派发前将任务总数+1, 然后在任务结束后判断任务是否成功, 失败的话将任务失败总数+1再判断是否到达最大错误率, 进而切换到另一台服务器.
另外还有一种情况是大部分服务器都挂了, 后台直接走APNS推送可用的服务器序号过来, 就不用挨个挨个切换了.
### 三.合理的使用请求派发器
OK, 炮弹有了, 炮台也就绪了, 接下来看看如何使用这个炮台.
```
#pragma mark - HHAPIConfiguration
typedef void(^HHNetworkTaskProgressHandler)(CGFloat progress);
typedef void(^HHNetworkTaskCompletionHander)(NSError *error, id result);
@interface HHAPIConfiguration : NSObject
@property (copy, nonatomic) NSString *urlPath;
@property (strong, nonatomic) NSDictionary *requestParameters;
@property (assign, nonatomic) BOOL useHttps;
@property (strong, nonatomic) NSDictionary *requestHeader;
@property (assign, nonatomic) HHNetworkRequestType requestType;
@end
@interface HHDataAPIConfiguration : HHAPIConfiguration
@property (assign, nonatomic) NSTimeInterval cacheValidTimeInterval;
@end
@interface HHUploadAPIConfiguration : HHAPIConfiguration
@property (strong, nonatomic) NSArray<HHUploadFile *> * uploadContents;
@end
#pragma mark - HHAPIManager
@interface HHAPIManager : NSObject
- (void)cancelAllTask;
- (void)cancelTaskWithtaskIdentifier:(NSNumber *)taskIdentifier;
+ (void)cancelTaskWithtaskIdentifier:(NSNumber *)taskIdentifier;
+ (void)cancelTasksWithtaskIdentifiers:(NSArray *)taskIdentifiers;
- (NSURLSessionDataTask *)dataTaskWithConfiguration:(HHDataAPIConfiguration *)config completionHandler:(HHNetworkTaskCompletionHander)completionHandler;
- (NSNumber *)dispatchDataTaskWithConfiguration:(HHDataAPIConfiguration *)config completionHandler:(HHNetworkTaskCompletionHander)completionHandler;
- (NSNumber *)dispatchUploadTaskWithConfiguration:(HHUploadAPIConfiguration *)config progressHandler:(HHNetworkTaskProgressHandler)progressHandler completionHandler:(HHNetworkTaskCompletionHander)completionHandler;
@end
```
```
- (void)cancelAllTask {
    for (NSNumber *taskIdentifier in self.loadingTaskIdentifies) {
        [[HHNetworkClient sharedInstance] cancelTaskWithTaskIdentifier:taskIdentifier];
    }
    [self.loadingTaskIdentifies removeAllObjects];
}
- (void)cancelTaskWithtaskIdentifier:(NSNumber *)taskIdentifier {
    [[HHNetworkClient sharedInstance] cancelTaskWithTaskIdentifier:taskIdentifier];
    [self.loadingTaskIdentifies removeObject:taskIdentifier];
}
+ (void)cancelTaskWithtaskIdentifier:(NSNumber *)taskIdentifier {
    [[HHNetworkClient sharedInstance] cancelTaskWithTaskIdentifier:taskIdentifier];
}
+ (void)cancelTasksWithtaskIdentifiers:(NSArray *)taskIdentifiers {
    for (NSNumber *taskIdentifier in taskIdentifiers) {
        [[HHNetworkClient sharedInstance] cancelTaskWithTaskIdentifier:taskIdentifier];
    }
}
- (NSURLSessionDataTask *)dataTaskWithConfiguration:(HHDataAPIConfiguration *)config completionHandler:(HHNetworkTaskCompletionHander)completionHandler {
    return [[HHNetworkClient sharedInstance] dataTaskWithUrlPath:config.urlPath useHttps:config.useHttps requestType:config.requestType params:config.requestParameters header:config.requestHeader completionHandler:^(NSURLResponse *response, id responseObject, NSError *error) {
        completionHandler ? completionHandler([self formatError:error], responseObject) : nil;
    }];
}
```
HHAPIManager对外提供数据请求和取消的接口, 内部调用HHNetworkClient进行实际的请求操作.
1.协议还是配置对象?
HHAPIManager的接口我们并没有像之前一样提供多个参数, 而是将多个参数组合为一个配置对象, 下面说一下为什么这样做:
- 为什么多个参数的接口方式不好?
一个APP中调用的API通常都是数以百计甚至千计, 如果有一天需要对已成型的所有的API都追加一个参数, 此时的改动之多, 足使男程序员沉默, 女程序员流泪.
举个例子: APP1.0已经上线, 1.1版本总监突然要求对数据请求加上缓存, 操作请求不用加缓存, 如果是参数接口的形式一般就是这样写:
```
//老接口
- (NSNumber *)dispatchTaskWithUrlPath:(NSString *)urlPath
                             useHttps:(BOOL)useHttps
                               method:(NSString *)method
                               params:(NSDictionary *)params
                               header:(NSDictionary *)header;
//新接口
- (NSNumber *)dispatchTaskWithUrlPath:(NSString *)urlPath
                             useHttps:(BOOL)useHttps
                               method:(NSString *)method
                               params:(NSDictionary *)params
                               header:(NSDictionary *)header
                          shouldCache:(BOOL)shouldCache;
```
然后原来的老接口全都调用新接口shouldCache默认传NO, 不需要缓存的API不用做改动, 而需要缓存的API都得改调用新接口然后shouldCache传YES.
这样能暂时解决问题, 工作量也会小一些, 然后过了两天总监过来说, 为什么没有对API区分缓存时间? 还有, 我们又有新需求了. 呵呵!
- 使用协议提升拓展性
```
@protocol HHAPIManager <NSObject>
@required
- (BOOL)useHttps;
- (NSString *)urlPath;
- (NSDictionary *)parameters;
- (OTSNetworkRequestType)requestType;
@optional
- (BOOL)checkParametersIsValid;
- (NSTimeInterval)cacheValidTimeInterval;
- (NSArray<OTSUploadFile *> *)uploadContents;
@end
```
```
@interface HHAPIManager : NSObject<HHAPIManager>
...略
- (NSNumber *)dispatchTaskWithCompletionHandler:(OTSNetworkTaskCompletionHander)completionHandler;
...略
@end
```
其实最初的设计是走协议的, HHAPIManager遵守这个协议, 内部给上默认参数, dispatchTaskWithCompletionHandler:会去挨个获取这些参数, 各个子类自行实现自己自定义的部分, 这样以后就算有任何拓展, 只需要在协议里面加个方法基类给上默认值, 有需要的子类API重写一下就行了.
- 替换协议为配置对象
```
- (NSURLSessionDataTask *)dataTaskWithConfiguration:(HHDataAPIConfiguration *)config completionHandler:(HHNetworkTaskCompletionHander)completionHandler;
- (NSNumber *)dispatchDataTaskWithConfiguration:(HHDataAPIConfiguration *)config completionHandler:(HHNetworkTaskCompletionHander)completionHandler;
- (NSNumber *)dispatchUploadTaskWithConfiguration:(HHUploadAPIConfiguration *)config progressHandler:(HHNetworkTaskProgressHandler)progressHandler completionHandler:(HHNetworkTaskCompletionHander)completionHandler;
```
协议的方案其实很好, 也是我想要的设计. 但是协议是针对类而言的, 这意味着今后的每添加一个API就需要新建一个HHAPIManager的子类, 很容易就有了几百个API类文件, 维护起来很麻烦, 找起来很麻烦(以上是同事要求替换协议的理由, 我仍然支持协议, 但是他们人多). 所以将协议替换为配置对象, 然后API以模块功能划分, 每个模块一个类文件给出多个API接口 ,内部每个API搭上合适的配置对象, 这样一来只需要十几个类文件.
总之, 考虑到配置对象既可以实现单个API单个类的设计, 也可以满足同事的需求, 协议被换成了配置对象.
另外, 所有的block参数都不写在配置对象里, 而是直接在接口处声明, 看着别扭写着方便(block做参数和做属性哪个写起来简单大家都懂的).
2.简单的请求结果缓存器
上面简单提到了请求缓存, 其实我们是没有做缓存的, 因为我司HTTP的API现在基本上都被废弃了, 全是走TCP, 然而TCP的缓存又是另一个故事了.但是还是简单实现一下吧:
```
#define HHCacheManager [HHNetworkCacheManager sharedManager]
@interface HHNetworkCache : NSObject
+ (instancetype)cacheWithData:(id)data;
+ (instancetype)cacheWithData:(id)data validTimeInterval:(NSUInteger)interterval;
- (id)data;
- (BOOL)isValid;
@end
@interface HHNetworkCacheManager : NSObject
+ (instancetype)sharedManager;
- (void)removeObejectForKey:(id)key;
- (void)setObjcet:(HHNetworkCache *)object forKey:(id)key;
- (HHNetworkCache *)objcetForKey:(id)key;
@end
```
```
#define ValidTimeInterval 60
@implementation HHNetworkCache
+ (instancetype)cacheWithData:(id)data {
    return [self cacheWithData:data validTimeInterval:ValidTimeInterval];
}
+ (instancetype)cacheWithData:(id)data validTimeInterval:(NSUInteger)interterval {
    HHNetworkCache *cache = [HHNetworkCache new];
    cache.data = data;
    cache.cacheTime = [[NSDate date] timeIntervalSince1970];
    cache.validTimeInterval = interterval > 0 ? interterval : ValidTimeInterval;
    return cache;
}
- (BOOL)isValid {
    if (self.data) {
        return [[NSDate date] timeIntervalSince1970] - self.cacheTime < self.validTimeInterval;
    }
    return NO;
}
@end
#pragma mark - HHNetworkCacheManager
@interface HHNetworkCacheManager ()
@property (strong, nonatomic) NSCache *cache;
@end
@implementation HHNetworkCacheManager
+ (instancetype)sharedManager {
    static HHNetworkCacheManager *sharedManager;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedManager = [[super allocWithZone:NULL] init];
        [sharedManager configuration];
    });
    return sharedManager;
}
+ (instancetype)allocWithZone:(struct _NSZone *)zone {
    return [self sharedManager];
}
- (void)configuration {
    self.cache = [NSCache new];
    self.cache.totalCostLimit = 1024 * 1024 * 20;
}
#pragma mark - Interface
- (void)setObjcet:(HHNetworkCache *)object forKey:(id)key {
    [self.cache setObject:object forKey:key];
}
- (void)removeObejectForKey:(id)key {
    [self.cache removeObjectForKey:key];
}
- (HHNetworkCache *)objcetForKey:(id)key {
    return [self.cache objectForKey:key];
}
@end
```
```
- (NSNumber *)dispatchDataTaskWithConfiguration:(HHDataAPIConfiguration *)config completionHandler:(HHNetworkTaskCompletionHander)completionHandler{
    NSString *cacheKey;
    if (config.cacheValidTimeInterval > 0) {
        NSMutableString *mString = [NSMutableString stringWithString:config.urlPath];
        [config.requestParameters enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
            [mString appendFormat:@"&%@=%@",key, obj];
        }];
        cacheKey = [self md5WithString:[mString copy]];
        HHNetworkCache *cache = [HHCacheManager objcetForKey:cacheKey];
        if (!cache.isValid) {
            [HHCacheManager removeObejectForKey:cacheKey];
        } else {
            completionHandler ? completionHandler(nil, cache.data) : nil;
            return @-1;
        }
    }
    NSMutableArray *taskIdentifier = [NSMutableArray arrayWithObject:@-1];
    taskIdentifier[0] = [[HHNetworkClient sharedInstance] dispatchTaskWithUrlPath:config.urlPath useHttps:config.useHttps requestType:config.requestType params:config.requestParameters header:config.requestHeader completionHandler:^(NSURLResponse *response, id responseObject, NSError *error) {
        if (!error && config.cacheValidTimeInterval > 0) {
            HHNetworkCache *cache = [HHNetworkCache cacheWithData:responseObject validTimeInterval:config.cacheValidTimeInterval];
            [HHCacheManager setObjcet:cache forKey:cacheKey];
        }
        [self.loadingTaskIdentifies removeObject:taskIdentifier.firstObject];
        completionHandler ? completionHandler([self formatError:error], responseObject) : nil;
    }];
    [self.loadingTaskIdentifies addObject:taskIdentifier.firstObject];
    return taskIdentifier.firstObject;
```
简单定义一个HHCache对象, 存放缓存数据, 缓存时间, 缓存时效, 然后HHNetworkCacheManager单例对象内部用NSCache存储缓存对象, 因为NSCache自带线程安全特效, 连锁都不用.
在任务发起之前我们检查一下是否有可用缓存, 有可用缓存直接返回, 没有就走网络, 网络任务成功后存一下请求数据即可.
3.请求结果的格式化
网络任务完成后带回的数据以什么样的形式返回给调用方, 分两种情况: 任务成功和任务失败.这里我们定义一下任务成功和失败, 成功表示网络请求成功且带回了可用数据, 失败表示未获取到可用数据.
举个例子: 获取一个话题列表, 用户希望看到的看到是一排排彩色头像, 如果你调用API拿不到这一堆数据那对于用户来说就是失败的. 那么没拿到数据可能是网络出错了, 或者网络没有问题只是用户没有关注过任何话题, 那么相应的展示网络错误提示或者推荐话题提示.
任务成功的话很简单, 直接做相应JSON解析正常返回就行, 如果某个XXXAPI有特殊需求那就新加一个XXXAPIConfig继承APIConfig基类, 在里面添加属性或者方法描述一下你有什么特殊需求, XXXAPI负责格式好返回就行了(所以还是一个API一个类好, 干净).
任务失败的话就麻烦一点, 我希望任何API都能友好的返回错误提示, 具体的, 如果有错误发生了, 那么返回给调用方的error.code一定是可读的枚举而不是301之类的需要比对文档的错误码(必须), error.domain通常就是错误提示语(可选), 这就要求程序员写每个API时都定义好错误枚举(所以还是一个API一个类好, 干净)和相应的错误提示.大概是这样子:
```
//HHNetworkTaskError.h 通用错误
typedef enum : NSUInteger {
    HHNetworkTaskErrorTimeOut = 101,
    HHNetworkTaskErrorCannotConnectedToInternet = 102,
    HHNetworkTaskErrorCanceled = 103,
    HHNetworkTaskErrorDefault = 104,
    HHNetworkTaskErrorNoData = 105,
    HHNetworkTaskErrorNoMoreData = 106
} HHNetworkTaskError;
static NSError *HHError(NSString *domain, int code) {
    return [NSError errorWithDomain:domain code:code userInfo:nil];
}
static NSString *HHNoDataErrorNotice = @"这里什么也没有~";
static NSString *HHNetworkErrorNotice = @"当前网络差, 请检查网络设置~";
static NSString *HHTimeoutErrorNotice = @"请求超时了~";
static NSString *HHDefaultErrorNotice = @"请求失败了~";
static NSString *HHNoMoreDataErrorNotice = @"没有更多了~";
```
```
- (NSError *)formatError:(NSError *)error {
    if (error != nil) {
        switch (error.code) {
            case NSURLErrorCancelled: {
                error = HHError(HHDefaultErrorNotice, HHNetworkTaskErrorCanceled);
            }   break;
            case NSURLErrorTimedOut: {
                error = HHError(HHTimeoutErrorNotice, HHNetworkTaskErrorTimeOut);
            }   break;
            case NSURLErrorCannotFindHost:
            case NSURLErrorCannotConnectToHost:
            case NSURLErrorNotConnectedToInternet: {//应产品要求, 所有连不上服务器都是用户网络的问题
                error = HHError(HHNetworkErrorNotice, HHNetworkTaskErrorCannotConnectedToInternet);
            }   break;
            default: {
                error = HHError(HHNoDataErrorNotice, HHNetworkTaskErrorDefault);
            }   break;
        }
    }
    return error;
}
```
通用的错误枚举和提示语定义在一个.h中, 以后有新增通用描述都在这里添加, 便于管理. HHAPIManager基类会先格式好某些通用错误, 然后各个子类定义自己特有的错误枚举(不可和通用描述冲突)和错误描述, 像这样:
```
//HHTopicAPIManager.h
typedef enum : NSUInteger {
    HHUserInfoTaskErrorNotExistUserId = 1001,//用户不存在
    HHUserInfoTaskError1,//瞎写的, 意思到就行
    HHUserInfoTaskError2
} HHUserInfoTaskError;
typedef enum : NSUInteger {
    HHUserFriendListTaskError0 = 1001,
    HHUserFriendListTaskError1,
    HHUserFriendListTaskError2,
} HHTopicListTaskError;
```
```
//HHTopicAPIManager.m
- (NSNumber *)fetchUserInfoWithUserId:(NSUInteger)userId completionHandler:(HHNetworkTaskCompletionHander)completionHandler {
    HHDataAPIConfiguration *config = [HHDataAPIConfiguration new];
    config.urlPath = @"fetchUserInfoWithUserIdPath";
    config.requestParameters = nil;
    return [super dispatchDataTaskWithConfiguration:config completionHandler:^(NSError *error, id result) {
        if (!error) {//通用错误基类已经处理好, 做好自己的数据格式就行
            switch ([result[@"code"] integerValue]) {
                case 200: {
                    //                    请求数据无误做相应解析
                    //                    result = [HHUser objectWithKeyValues:result[@"data"]];
                }   break;
                case 301: {
                    error = HHError(@"用户不存在", HHUserInfoTaskErrorNotExistUserId);
                }  break;
                case 302: {
                    error = HHError(@"xxx错误", HHUserInfoTaskError1);
                }   break;
                case 303: {
                    error = HHError(@"yyy错误", HHUserInfoTaskError2);
                }   break;
                default:break;
            }
        }
        completionHandler ? completionHandler(error, result) : nil;
    }];
}
```
然后调用方一般情况下只需要这样:
```
[[HHTopicAPIManager new] fetchUserInfoWithUserId:123 completionHandler:^(NSError *error, id result) {
       error ? [self showToastWithText:error.domain] : [self reloadTableViewWithNames:result];
    }];
```
当然, 情况复杂的话只能这样, 代码多一点, 但是有枚举读起来也不麻烦:
```
[[HHTopicAPIManager new] fetchUserInfoWithUserId:123 completionHandler:^(NSError *error, id result) {
        error ? [self showErrorViewWithError:error] : [self reloadTableViewWithNames:result];
    }];
- (void)showErrorViewWithError:(NSError *)error {
    switch (error.code) {//如果情况复杂就自己switch
                case HHNetworkTaskErrorTimeOut: {
                    //                    展示请求超时错误页面
                }   break;
                case HHNetworkTaskErrorCannotConnectedToInternet: {
                    //                    展示网络错误页面
                }
                case HHUserInfoTaskErrorNotExistUserId: {
                    //                    ...
                }
                    //                    ...
                default:break;
            }
}
```
这里多扯两句, 请求的回调我是以(error, id)的形式返回的, 而不是像AFN那样分别给出successBlock和failBlock. 其实我本身是很支持AFN的做法的, 区分成功和错误强行让两种业务的代码出现在两个不同的部分, 这很好, 不同的业务处理就该在不同函数/方法里面. 但是实际开发中有很多成功和失败都会执行的操作, 典型的例子就是HUD, 两个block的话我需要在两个地方都加上[HUD hide], 这样的代码写的多了就会很烦, 而我又懒, 所以就成功失败都在一个回调返回了.
但是! 你也应该区分不同的业务写出两个不同方法(像上面那样做), 至于公用的部分就只写一次就够了.像这样:
```
[hud show:YES];
[[HHTopicAPIManager new] fetchUserInfoWithUserId:123 completionHandler:^(NSError *error, id result) {
      [hud hide:YES];
       error ? [self showToastWithText:error.domain] : [self reloadTableViewWithNames:result];
    }];
```
再说一句, 即使你比我还懒, 不声明两个方法那也应该将较短的逻辑写在前面, 较长的写在后面, 易读, 像这样:
```
if (!error) {
            ...短
            ...短
        } else {
            switch (error.code) {//如果情况复杂就自己switch
                case HHNetworkTaskErrorTimeOut: {
                    //                    展示请求超时错误页面
                }   break;
                case HHNetworkTaskErrorCannotConnectedToInternet: {
                    //                    展示网络错误页面
                }
                case HHUserInfoTaskErrorNotExistUserId: {
                    //                    ...长
                }
                    //                    ...长
                default:break;
            }
        }
    }
```
4.两个小玩意儿
文章到这基本上这个网络层该说的都说的差不多了, 各位可以根据自己的需求改动改动就能用了, 最后简单介绍下两个和它相关的小玩意儿就结尾吧:
- HHNetworkTaskGroup
```
@protocol HHNetworkTask <NSObject>
- (void)cancel;
- (void)resume;
@end
@interface HHNetworkTaskGroup : NSObject
- (void)addTaskWithMessgeType:(NSInteger)type message:(id)message completionHandler:(HHNetworkTaskCompletionHander)completionHandler;
- (void)addTask:(id<HHNetworkTask>)task;
- (void)cancel;
- (void)dispatchWithNotifHandler:(void(^)(void))notifHandler;
@end
```
```
@interface HHNetworkTaskGroup ()
@property (copy, nonatomic) void(^notifHandler)(void);
@property (assign, nonatomic) NSInteger signal;
@property (strong, nonatomic) NSMutableSet *tasks;
@property (strong, nonatomic) dispatch_semaphore_t lock;
@property (strong, nonatomic) id keeper;
@end
@implementation HHNetworkTaskGroup
//- (void)addTaskWithMessgeType:(HHSocketMessageType)type message:(PBGeneratedMessage *)message completionHandler:(HHNetworkCompletionHandler)completionHandler {
//    
//    HHSocketTask *task = [[HHSocketManager sharedManager] taskWithMessgeType:type message:message completionHandler:completionHandler];
//    [self addTask:task];
//}
- (void)addTask:(id<HHNetworkTask>)task {
    if ([task respondsToSelector:@selector(cancel)] &&
        [task respondsToSelector:@selector(resume)] &&
        ![self.tasks containsObject:task]) {
        [self.tasks addObject:task];
        [(id)task addObserver:self forKeyPath:NSStringFromSelector(@selector(state)) options:NSKeyValueObservingOptionOld | NSKeyValueObservingOptionNew context:nil];
    }
}
- (void)dispatchWithNotifHandler:(void (^)(void))notifHandler {
    if (self.tasks.count == 0) {
        dispatch_async(dispatch_get_main_queue(), ^{
            notifHandler ? notifHandler() : nil;
        });
        return;
    }
    self.lock = dispatch_semaphore_create(1);
    self.keeper = self;
    self.signal = self.tasks.count;
    self.notifHandler = notifHandler;
    for (id<HHNetworkTask> task in self.tasks.allObjects) {
        [task resume];
    }
}
- (void)cancel {
    for (id<HHNetworkTask> task in self.tasks.allObjects) {
        if ([(id)task state] < NSURLSessionTaskStateCanceling) {
            [(id)task removeObserver:self forKeyPath:NSStringFromSelector(@selector(state))];
            [task cancel];
        }
    }
    [self.tasks removeAllObjects];
    self.keeper = nil;
}
#pragma mark - KVO
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context {
    if ([keyPath isEqualToString:NSStringFromSelector(@selector(state))]) {
        NSURLSessionTaskState oldState = [change[NSKeyValueChangeOldKey] integerValue];
        NSURLSessionTaskState newState = [change[NSKeyValueChangeNewKey] integerValue];
        if (oldState != newState && newState >= NSURLSessionTaskStateCanceling) {
            [object removeObserver:self forKeyPath:NSStringFromSelector(@selector(state))];
            dispatch_semaphore_wait(self.lock, DISPATCH_TIME_FOREVER);
            self.signal--;
            dispatch_semaphore_signal(self.lock);
            if (self.signal == 0) {
                dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                    self.notifHandler ? self.notifHandler() : nil;
                    [self.tasks removeAllObjects];
                    self.keeper = nil;
                });
            }
        }
    }
}
#pragma mark - Getter
- (NSMutableSet *)tasks {
    if (!_tasks) {
        _tasks = [NSMutableSet set];
    }
    return _tasks;
}
@end
```
看名字应该就知道这个是和dispatch_group_notif差不多的东西, 不过是派发的对象不是dispatch_block_t而是id<HHNetworkTask>. 代码很简单, 说说思路就行了.
- 
keeper
系统大部分带有Block的API都有一个特性就是只需要生成不需要持有, 也不用担心Block持有我们的对象而造成循环引用, 例如:dispatch_async, dataTaskWithURL:completionHandler:等等, 其实具体的实现就是先循环引用再破除循环引用, 比如dispatch_async的queue和block会循环引用, 这样在block执行期间双方都不会释放, 然后等到block执行完成后再将queue.block置nil破除循环引用, block没了, 那它捕获的queue和其他对象计数都能-1,也就都能正常释放了.代码里面的keeper就是来制造这个循环引用的.
- 
signal和tasks
signal其实就是tasks.count, 为什么我们不直接在task完成后直接tasks.remove然后判断tasks.count == 0而是要间接给一个signal来做这事儿?
原因很简单: forin过程中是不能改变容器对象的. 当我们forin派发task的时候, task是异步执行的, 有可能在task执行完成触发KVO的时候我们的forin还在遍历, 此时直接remove就会crash. 如果不用forin, 而是用while或者for(;;)就会漏发. 所以就声明一个signal来做计数了. 另外addObserve和removeObserve必须成对出现, 控制好就行.
- 
dispatch_after
在所有任务执行完成后并没有马上执行notif(), 而是等待0.1秒以后再执行notif(), 这是因为task.state的设置会在task.completionHandler之前执行, 所以我们需要等一下, 确认completionHandler执行后在走我们的notif().
- 
如何使用
```
HHNetworkTaskGroup *group = [HHNetworkTaskGroup new];
  HHTopicAPIManager *manager = [HHTopicAPIManager new];
  for (int i = 1; i < 6; i++) {
      NSURLSessionDataTask *task = [manager topicListDataTaskWithPage:i pageSize:20 completionHandler:^(NSError *error, id result) {
          //...completionHandler... i
      }];
      [group addTask:(id)task];
  }
  [group dispatchWithNotifHandler:^{
      //notifHandler
  }];
```
强调一下, 绝对不应该直接调用HHNetworkClient或者HHAPIManger的dataTaskxxx...这些通用接口来生成task, 应该在该task所属的API暴露接口生成task, 简单说就是不要跨层访问. 每个API的参数甚至签名规则都是不一样的, API的调用方应该只提供生成task的相应参数而不应该也不需要知道这些参数具体的拼装逻辑.
- 
HHNetworkAPIRecorder
```
@interface HHNetworkAPIRecorder : NSObject
@property (strong, nonatomic) id rawValue;
@property (assign, nonatomic) int pageSize;
@property (assign, nonatomic) int currentPage;
@property (assign, nonatomic) NSInteger itemsCount;
@property (assign, nonatomic) NSInteger lastRequestTime;
- (void)reset;
- (BOOL)hasMoreData;
- (NSInteger)maxPage;
@end
```
日常请求中有很多接口涉及到分页, 然而毫无疑问分页的逻辑在每个页面都是一模一样的, 但是却需要每个调用页面都保持一下currentPage然后调用逻辑都写一次, 其实直接在API内部实现一下分页的逻辑, 然后对外暴露第一页和下一页的接口就不用声明currentPage和重复这些无聊的逻辑了. 像这样:
```
//XXXAPI.h
- (NSNumber *)refreshTopicListWithCompletionHandler:(HHNetworkTaskCompletionHander)completionHandler;//第一页
- (NSNumber *)loadmoreTopicListWithCompletionHandler:(HHNetworkTaskCompletionHander)completionHandler;//当前页的下一页
- (NSNumber *)fetchTopicListWithPage:(NSInteger)page completionHandler:(HHNetworkTaskCompletionHander)completionHandler;//指定页(一般外部用不到, 看情况暴露)
```
```
//XXXAPI.m
- (NSNumber *)refreshTopicListWithCompletionHandler:(HHNetworkTaskCompletionHander)completionHandler {
    [self.topicListAPIRecorder reset];
    return [self fetchTopicListWithPage:self.topicListAPIRecorder.currentPage completionHandler:completionHandler];
}
- (NSNumber *)loadmoreTopicListWithCompletionHandler:(HHNetworkTaskCompletionHander)completionHandler {
    self.topicListAPIRecorder.currentPage++;
    return [self fetchTopicListWithPage:self.topicListAPIRecorder.currentPage completionHandler:completionHandler];
}
```
```
//SomeViewController
self.topicAPIManager = [HHTopicAPIManager new];
...
self.tableView.header = [MJRefreshNormalHeader headerWithRefreshingBlock:^{//下拉刷新
        [weakSelf.topicAPIManager refreshTopicListWithCompletionHandler:^(NSError *error, id result) {
                ...
        }];
    }];
self.tableView.footer = [MJRefreshAutoNormalFooter footerWithRefreshingBlock:^{//上拉加载
        [weakSelf.topicAPIManager loadmoreTopicListWithCompletionHandler:^(NSError *error, id result) {
                ...
        }];
    }];
```
### 总结
HHURLRequestGenerator: 网络请求的生成器, 公用的请求头, cookie都在此设置.
HHNetworkClient: 网络请求的派发器, 这里会记录每一个服役中的请求, 并在必要的时候切换服务器.
HHAPIManager: 网络请求派发器的调用者, 这里对请求的结果做相应的数据格式化后返回给API调用方, 提供请求模块的拓展性支持, 并提供合理的Task供TaskGroup派发.
## 写在最后
本来想来一段洋洋洒洒的写作感言感动一下自己的, 手机过来一条推送, 一看才知道今天是情人节, 那就祝各位节日快乐吧......
[本文附带的demo地址](https://github.com/HeiHuaBaiHua/TAFNetworking)
