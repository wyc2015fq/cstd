# AFNetworking 3 批量上传图片的 3 种方法 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月16日 11:26:09[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2685

AFNetworking 在去年年底升级到了 3.0。这个版本更新想必有很多好处，然而让我吃惊的是，它并没有 batch request 接口。之前的 1.x 版本、2.x 版本都实现了这个很常见的需求，不知道作者为何选择在 3.x 中去掉它。
在 AFNetworking 2 中，我们只需一行代码就能解决批量上传的问题：
```
[AFURLConnectionOperation batchOfRequestOperations:operations progressBlock:^(NSUInteger numberOfFinishedOperations, NSUInteger totalNumberOfOperations) {
    NSLog(@"%lu 上传完成，共 %lu", (long)numberOfFinishedOperations, (long)totalNumberOfOperations);
} completionBlock:^(NSArray *operations) {
    NSLog(@"上传完毕");   
}];
```
但 AFNetworking 3 用的是`NSURLSession`，而不是用`NSOperation`来包装`NSURLConnection`，所以把整个`AFURLConnectionOperation`类都干掉了。上面的方法不能再用，并且也没有给出替代品。因此，我们只能自己动手了。
实现这个功能，有几个要点：
- 异步上传。批量请求里的每个请求都应该在不同线程，可以同时上传。
- 在所有请求都完成之后，再通知回调。
- 尽管异步请求的返回先后顺序没有一定，很可能后发出的请求先返回；但是最后回调的时候，请求返回的结果必须要按请求发出的顺序排列。比如，一个很常见的处理是，上传图片的接口返回该图片的 url；那么回调结果里的 url 顺序显然需要跟上传的图片顺序一一对应上。
- 最好传完每张图片也能有一个回调，方便我们告诉用户上传的进度。
同时满足以上要点，主要有3种思路：GCD、NSOperation 以及 promise。这个需求也是示例多线程用法的一个很好的例子，所以我写了这篇比较详细的文章供大家参考。
下面的代码以图片上传为例。测试数据配置了 3 张图片，其中第 2 张图片尺寸最小，往往先上传完毕，用来测试请求发出顺序与返回顺序不一致的情况。
![](http://upload-images.jianshu.io/upload_images/227290-49c6bd2c7d2a877f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
测试图片之一
## 方法一：GCD dispatch group
我们知道，GCD dispatch 是多线程处理最简单的方法。全部请求完成后再通知回调的需求，很适合利用 dispatch group 来完成。至于保证返回结果的顺序，我们只好自己来做了。
首先需要一个方法，对于每张图片生成一个上传请求。
```
- (NSURLSessionUploadTask*)uploadTaskWithImage:(UIImage*)image completion:(void (^)(NSURLResponse *response, id responseObject, NSError *error))completionBlock {
    // 构造 NSURLRequest
    NSError* error = NULL;
    NSMutableURLRequest *request = [[AFHTTPRequestSerializer serializer] multipartFormRequestWithMethod:@"POST" URLString:[self uploadUrl] parameters:nil constructingBodyWithBlock:^(id<AFMultipartFormData> formData) {
        NSData* imageData = UIImageJPEGRepresentation(image, 1.0);
        [formData appendPartWithFileData:imageData name:@"file" fileName:@"someFileName" mimeType:@"multipart/form-data"];
    } error:&error];
    // 可在此处配置验证信息
    // 将 NSURLRequest 与 completionBlock 包装为 NSURLSessionUploadTask
    AFURLSessionManager *manager = [[AFURLSessionManager alloc] initWithSessionConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration]];
    NSURLSessionUploadTask *uploadTask = [manager uploadTaskWithStreamedRequest:request progress:^(NSProgress * _Nonnull uploadProgress) {
    } completionHandler:completionBlock];
    return uploadTask;
}
```
在这个方法里，我们首先用`UIImageJPEGRepresentation`把`UIImage`变为`NSData`。然后用`AFHTTPRequestSerializer`来生成`NSMutableURLRequest`，`[self uploadUrl]`是上传接口的地址。为安全考虑，一般上传的接口都有身份验证的需求，比如在请求 header 中加入 auth 信息，可以在此配置`NSMutableURLRequest`的
 header。最后，我们用 `AFURLSessionManager` 把 `NSURLRequest` 和回调 block 包装成一个`NSURLSessionUploadTask`。
有了生成请求的方法，批量发出请求的方法如下：
```
- (IBAction)runDispatchTest:(id)sender {
    // 需要上传的数据
    NSArray* images = [self images];
    // 准备保存结果的数组，元素个数与上传的图片个数相同，先用 NSNull 占位
    NSMutableArray* result = [NSMutableArray array];
    for (UIImage* image in images) {
        [result addObject:[NSNull null]];
    }
    dispatch_group_t group = dispatch_group_create();
    for (NSInteger i = 0; i < images.count; i++) {
        dispatch_group_enter(group);
        NSURLSessionUploadTask* uploadTask = [self uploadTaskWithImage:images[i] completion:^(NSURLResponse *response, NSDictionary* responseObject, NSError *error) {
            if (error) {
                NSLog(@"第 %d 张图片上传失败: %@", (int)i + 1, error);
                dispatch_group_leave(group);
            } else {
                NSLog(@"第 %d 张图片上传成功: %@", (int)i + 1, responseObject);
                @synchronized (result) { // NSMutableArray 是线程不安全的，所以加个同步锁
                    result[i] = responseObject;
                }
                dispatch_group_leave(group);
            }
        }];
        [uploadTask resume];
    }
    dispatch_group_notify(group, dispatch_get_main_queue(), ^{
        NSLog(@"上传完成!");
        for (id response in result) {
            NSLog(@"%@", response);
        }
    });
}
```
可以看到，我们把所有请求放在一个 dispatch_group 里。首先用`dispatch_group_create()`来创建这个 group。然后，对于每一个 uploadTask，在创建之前先执行`dispatch_group_enter(group)`，在结束回调的 block里执行`dispatch_group_leave(group)`。结束回调的代码放在`dispatch_group_notify`里即可。
实际执行中，首先是所有 task 都进入 group，同时开始上传；上传完成之后依次离开 group；最后 group 空了会自动调用传入`group_notify`的回调，整个过程完成。
那么如何把回调数据排成正确的顺序呢？借助 block 会保存自动变量的特点，我们让每个 task 的回调 block 都自动带上标志请求次序的变量 i，只需把返回结果填入数组的第 i 位即可。所以在开始请求之前，先创建好保存返回结果的数组，元素个数与请求个数相等，每个位置上用`[NSNull null]`占位。每个请求返回之后，把自己那个位置上的`[NSNull null]`替换成返回结果。全部请求返回之后，数组里保存的自然是按请求顺序排列的回调数据。
这里注意，因为 NSMutableArray 是线程不安全的，而每个请求返回时是在不同线程操作同一个数组，所以我用`@synchronized`把操作数组的代码锁住了，锁的对象就用这个数组即可。这样保证所有线程执行到这一句都得串行，避免线程安全问题。
一次测试结果如下：
```cpp
```cpp
2016-05-13 15:49:43.042 HAMAFNetworkingBatchRequestDemo[23102:5717076] 第 2 张图片上传成功: {
    imageBucket = test;
    imageKey = "331eb245-741f-4fdc-8769-fdfb9e646da7";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/331eb245-741f-4fdc-8769-fdfb9e646da7?imageMogr2/thumbnail/640x";
}
2016-05-13 15:49:43.098 HAMAFNetworkingBatchRequestDemo[23102:5717076] 第 1 张图片上传成功: {
    imageBucket = test;
    imageKey = "d08f5370-c8b6-4912-b4e5-c73ea3134637";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/d08f5370-c8b6-4912-b4e5-c73ea3134637?imageMogr2/thumbnail/640x";
}
2016-05-13 15:49:43.120 HAMAFNetworkingBatchRequestDemo[23102:5717076] 第 3 张图片上传成功: {
    imageBucket = test;
    imageKey = "bdf13097-8128-4f04-bcbc-462bd2a728ab";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/bdf13097-8128-4f04-bcbc-462bd2a728ab?imageMogr2/thumbnail/640x";
}
2016-05-13 15:49:43.120 HAMAFNetworkingBatchRequestDemo[23102:5717076] 上传完成!
2016-05-13 15:49:43.121 HAMAFNetworkingBatchRequestDemo[23102:5717076] {
    imageBucket = test;
    imageKey = "d08f5370-c8b6-4912-b4e5-c73ea3134637";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/d08f5370-c8b6-4912-b4e5-c73ea3134637?imageMogr2/thumbnail/640x";
}
2016-05-13 15:49:43.121 HAMAFNetworkingBatchRequestDemo[23102:5717076] {
    imageBucket = test;
    imageKey = "331eb245-741f-4fdc-8769-fdfb9e646da7";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/331eb245-741f-4fdc-8769-fdfb9e646da7?imageMogr2/thumbnail/640x";
}
2016-05-13 15:49:43.124 HAMAFNetworkingBatchRequestDemo[23102:5717076] {
    imageBucket = test;
    imageKey = "bdf13097-8128-4f04-bcbc-462bd2a728ab";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/bdf13097-8128-4f04-bcbc-462bd2a728ab?imageMogr2/thumbnail/640x";
}
```
```
可以看到，尽管第 2 张图片尺寸最小、最先传完，第 1 张图片后传完，但最后的结果顺序还是正确的。
## 方法二：NSOperationQueue
能用 dispatch 实现的功能，自然也可以用`NSOperationQueue`。NSOperation 这一套比 dispatch 写起来要麻烦一些，不过有几个优点：
- `NSOperation`是对象，不像 dispatch 是 c 函数。这就意味着你可以继承它，可以给它加 category，在执行过程中也可以始终管理它，访问到它，查看它的状态等，不像 dispatch 是一撒手就够不着了。
- 用`NSOperation`执行的任务，执行过程中可以随时取消。dispatch 一经发出是无法取消的。
- `NSOperationQueue`可以限制最大并发数。假如队列里真有 100 个文件要传，开出 100 个线程反而会严重影响性能。`NSOperationQueue`可以很方便地设置`maxConcurrentOperationCount`。dispatch 也可以限制最大并发数（[参考苹果的文档](https://developer.apple.com/library/ios/documentation/General/Conceptual/ConcurrencyProgrammingGuide/OperationQueues/OperationQueues.html#//apple_ref/doc/uid/TP40008091-CH102-SW24)）不过写起来麻烦很多。
就我们的需求而言，用 NSOperation 有一个很方便的特点：dispatch 里的任务各自为政，而`NSOperation`之前是可以有依赖关系的。我们就可以利用这一点，来发起所有任务上传完成后的回调：把这个完成回调也做成一个`NSOperation`，让这个`NSOperation`前置依赖所有上传的`NSOperation`，这样等到所有上传的`NSOperation`完成之后，这个回调`NSOperation`才会开始执行。
然而，用`NSOperation`也有一个很不方便的特点：`NSOperationQueue`是用 KVO 观察`NSOperation`状态来判断任务是否已结束的。而我们请求用的`NSURLSessionTask`，它长得很像一个`NSOperation`，但却并不是`NSOperation`的子类。所以，这一套方法最麻烦的地方就在于我们需要写一个自定义的`NSOperation`子类，只是为了跟踪`NSURLSessionTask`的状态。
自定义的`NSOperation`代码如下：
#### HAMURLSessionWrapperOperation.h
```
#import <Foundation/Foundation.h>
@interface HAMURLSessionWrapperOperation : NSOperation
+ (instancetype)operationWithURLSessionTask:(NSURLSessionTask*)task;
@end
```
#### HAMURLSessionWrapperOperation.m
```
#import "HAMURLSessionWrapperOperation.h"
@interface HAMURLSessionWrapperOperation () {
    BOOL executing;  // 系统的 finished 是只读的，不能修改，所以只能重写一个。
    BOOL finished;
}
@property (nonatomic, strong) NSURLSessionTask* task;
@property (nonatomic, assign) BOOL isObserving;
@end
@implementation HAMURLSessionWrapperOperation
#pragma mark - Observe Task
+ (instancetype)operationWithURLSessionTask:(NSURLSessionTask*)task {
    HAMURLSessionWrapperOperation* operation = [HAMURLSessionWrapperOperation new];
    operation.task = task;
    return operation;
}
- (void)dealloc {
    [self stopObservingTask];
}
- (void)startObservingTask {
    @synchronized (self) {
        if (_isObserving) {
            return;
        }
        [_task addObserver:self forKeyPath:@"state" options:NSKeyValueObservingOptionNew context:nil];
        _isObserving = YES;
    }
}
- (void)stopObservingTask { // 因为要在 dealloc 调，所以用下划线不用点语法
    @synchronized (self) {
        if (!_isObserving) {
            return;
        }
        _isObserving = NO;
        [_task removeObserver:self forKeyPath:@"state"];
    }
}
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSString *,id> *)change context:(void *)context {
    if (self.task.state == NSURLSessionTaskStateCanceling || self.task.state == NSURLSessionTaskStateCompleted) {
        [self stopObservingTask];
        [self completeOperation];
    }
}
#pragma mark - NSOperation methods
- (void)start {
    // Always check for cancellation before launching the task.
    if ([self isCancelled])
    {
        // Must move the operation to the finished state if it is canceled.
        [self willChangeValueForKey:@"isFinished"];
        finished = YES;
        [self didChangeValueForKey:@"isFinished"];
        return;
    }
    // If the operation is not canceled, begin executing the task.
    [self willChangeValueForKey:@"isExecuting"];
    [NSThread detachNewThreadSelector:@selector(main) toTarget:self withObject:nil];
    executing = YES;
    [self didChangeValueForKey:@"isExecuting"];
}
- (void)main {
    @try {
        [self startObservingTask];
        [self.task resume];
    }
    @catch (NSException * e) {
        NSLog(@"Exception %@", e);
    }
}
- (void)completeOperation {
    [self willChangeValueForKey:@"isFinished"];
    [self willChangeValueForKey:@"isExecuting"];
    executing = NO;
    finished = YES;
    [self didChangeValueForKey:@"isExecuting"];
    [self didChangeValueForKey:@"isFinished"];
}
- (BOOL)isAsynchronous {
    return YES;
}
- (BOOL)isExecuting {
    return executing;
}
- (BOOL)isFinished {
    return finished;
}
@end
```
代码有点长，但没办法。我们的目标是对每个`NSURLSessionTask`都包装出一个`HAMURLSessionWrapperOperation`，这个`NSOperation`完全随着`NSURLSessionTask`的状态而动，在 Task 结束之后发出 KVO 的通知，通知`NSOperationQueue`这个任务结束。
系统`NSOperation`的`finished`属性是只读的，不能修改；为了记录值和发出 KVO 的通知，我们只能在旁再定义一个`finished`的成员变量，通过重写`- (BOOL)isFinished`等 getter 方法，盖掉原来的`finished`属性。现在几乎全用 property，这种成员变量的写法好久没看见过了，没想到还有这种用处，这种特殊的写法还是从苹果文档学来的（[参考这里](https://developer.apple.com/library/ios/documentation/General/Conceptual/ConcurrencyProgrammingGuide/OperationObjects/OperationObjects.html#//apple_ref/doc/uid/TP40008091-CH101-SW16)）。
这里 start 方法照抄苹果文档，在新线程调起 main 方法。main 方法里就两件事：开始 KVO 观察上传 task 的 state 属性，然后启动 task。一旦 task 完成（或失败），接到 KVO 的通知，我们停止对 task 的观察，然后发出自己的 KVO 通知去通知`NSOperationQueue`。这里我们手动调起了`[self willChangeValueForKey:@"isFinished"];`和`[self didChangeValueForKey:@"isFinished"];`，又重写了
```
-
 (BOOL)isFinished
```
方法，就把只读的`finished`属性偷天换日变成我们自己定义的`finished`成员变量了。
自定义`NSOperation`说完了，下面我们来看看怎么使用这个类。我们同样要利用上面 dispatch 一节写的那个`uploadTaskWithImage:completion`方法，根据图片生成请求。发出请求的代码如下：
```
- (IBAction)runNSOperationTest:(id)sender {
    // 需要上传的数据
    NSArray* images = [self images];
    // 准备保存结果的数组，元素个数与上传的图片个数相同，先用 NSNull 占位
    NSMutableArray* result = [NSMutableArray array];
    for (UIImage* image in images) {
        [result addObject:[NSNull null]];
    }
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    queue.maxConcurrentOperationCount = 5;
    NSBlockOperation *completionOperation = [NSBlockOperation blockOperationWithBlock:^{
        [[NSOperationQueue mainQueue] addOperationWithBlock:^{ // 回到主线程执行，方便更新 UI 等
            NSLog(@"上传完成!");
            for (id response in result) {
                NSLog(@"%@", response);
            }
        }];
    }];
    for (NSInteger i = 0; i < images.count; i++) {
        NSURLSessionUploadTask* uploadTask = [self uploadTaskWithImage:images[i] completion:^(NSURLResponse *response, NSDictionary* responseObject, NSError *error) {
            if (error) {
                NSLog(@"第 %d 张图片上传失败: %@", (int)i + 1, error);
            } else {
                NSLog(@"第 %d 张图片上传成功: %@", (int)i + 1, responseObject);
                @synchronized (result) { // NSMutableArray 是线程不安全的，所以加个同步锁
                    result[i] = responseObject;
                }
            }
        }];
        HAMURLSessionWrapperOperation *uploadOperation = [HAMURLSessionWrapperOperation operationWithURLSessionTask:uploadTask];
        [completionOperation addDependency:uploadOperation];
        [queue addOperation:uploadOperation];
    }
    [queue addOperation:completionOperation];
}
```
保持结果顺序的方法与 dispatch 相同，都是我们自己完成的。我们把`maxConcurrentOperationCount`定成 5，避免并发过多竞争资源。先创建结束回调的 operation，再让它依赖后面创建的每一个上传 operation。因为一般回调都要涉及到更新 UI，所以让它回到主线程执行。后面根据每张图片逐一创建 task、包装成 operation。创建好之后，加进 operationQueue 里就开始跑了。
一次测试结果如下：
```cpp
```cpp
2016-05-13 15:50:06.269 HAMAFNetworkingBatchRequestDemo[23102:5717076] 第 2 张图片上传成功: {
    imageBucket = test;
    imageKey = "cc60ab02-7745-4c60-8697-8bae1501768b";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/cc60ab02-7745-4c60-8697-8bae1501768b?imageMogr2/thumbnail/640x";
}
2016-05-13 15:50:06.365 HAMAFNetworkingBatchRequestDemo[23102:5717076] 第 1 张图片上传成功: {
    imageBucket = test;
    imageKey = "ee9c1492-a8f1-441c-9bd4-c90756841266";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/ee9c1492-a8f1-441c-9bd4-c90756841266?imageMogr2/thumbnail/640x";
}
2016-05-13 15:50:06.413 HAMAFNetworkingBatchRequestDemo[23102:5717076] 第 3 张图片上传成功: {
    imageBucket = test;
    imageKey = "6fe8197a-4638-4706-afe1-3aca203cf73f";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/6fe8197a-4638-4706-afe1-3aca203cf73f?imageMogr2/thumbnail/640x";
}
2016-05-13 15:50:06.414 HAMAFNetworkingBatchRequestDemo[23102:5717076] 上传完成!
2016-05-13 15:50:06.414 HAMAFNetworkingBatchRequestDemo[23102:5717076] {
    imageBucket = test;
    imageKey = "ee9c1492-a8f1-441c-9bd4-c90756841266";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/ee9c1492-a8f1-441c-9bd4-c90756841266?imageMogr2/thumbnail/640x";
}
2016-05-13 15:50:06.415 HAMAFNetworkingBatchRequestDemo[23102:5717076] {
    imageBucket = test;
    imageKey = "cc60ab02-7745-4c60-8697-8bae1501768b";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/cc60ab02-7745-4c60-8697-8bae1501768b?imageMogr2/thumbnail/640x";
}
2016-05-13 15:50:06.415 HAMAFNetworkingBatchRequestDemo[23102:5717076] {
    imageBucket = test;
    imageKey = "6fe8197a-4638-4706-afe1-3aca203cf73f";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/6fe8197a-4638-4706-afe1-3aca203cf73f?imageMogr2/thumbnail/640x";
}
```
```
结果也是正确的。
## 方法三：promise
上面的两种方法，我们都是自己用数组、占位、逐位替换的方法，自己写代码保证返回数据顺序正确的。其实这种需要多个线程执行、全部结束后回调、结果顺序保证正确的需求，一般最适合用 promise 来做。各个语言都有自己的 promise 实现，iOS 也有好几种。这里我们试用一下 iOS 最著名的实现
[PromiseKit](https://github.com/mxcl/PromiseKit)。
在 github 上 5000 多个 star，这个 lib 是 Objective-C 、Swift 通用的，两套代码都有。在网络请求方面，它要依赖同一个作者写的另一个库
[OMGHTTPURLRQ](https://github.com/mxcl/OMGHTTPURLRQ)，导入的时候小费周折。PromiseKit 这一套方法与 AFNetworking 库就没关系了，可能有些离题，但是用起来是最为方便的。
这里我们不再需要上面那个生成`NSURLSessionTask`的方法了，现在我们需要把`NSURLRequest`包装成`AnyPromise`：
```
- (AnyPromise *)uploadPromiseWithImage:(UIImage *)image completion:(id (^)(id))completionBlock {
    NSString* url = [self uploadUrl];
    NSData* imageData = UIImageJPEGRepresentation(image, 1.0);
    OMGMultipartFormData *multipartFormData = [OMGMultipartFormData new];
    [multipartFormData addFile:imageData parameterName:@"file" filename:@"someFileName" contentType:@"multipart/form-data"];
    NSMutableURLRequest* request = [OMGHTTPURLRQ POST:url :multipartFormData error:nil];
    // 可在此处配置验证信息
    if (completionBlock) {
        return [NSURLConnection promise:request].then(completionBlock);
    } else {
        return [NSURLConnection promise:request];
    }
}
```
这里可以看到 promise 的`.then`语法。它是一个 C 函数，传进的参数是这项 promise 完成之后下一步需要执行的 block，返回值仍然是`AnyPromise`，所以可以一直`.then().then()……`这样链式调用下去。我们在这里让它上传完单张图片之后执行单张图片的回调，把回调 block『附身』在上传的 promise 之后。
上面就是创建 promise 的过程。那么执行 promise 的代码怎么写呢？
```
- (IBAction)runPromiseTest:(id)sender {
    // 需要上传的数据
    NSArray* images = [self images];
    NSMutableArray* promises = [NSMutableArray array];
    for (NSInteger i = 0; i < images.count; i++) {
        UIImage* image = images[i];
        [promises addObject:[self uploadPromiseWithImage:image completion:^(id resultImageUrl){
            NSLog(@"第 %d 张图片上传成功: %@", (int)i + 1, resultImageUrl);
            return resultImageUrl;
        }]];
    }
    PMKWhen(promises).then(^(NSArray *results) {
        NSLog(@"上传完成!");
        NSLog(@"%@", results);
    }).catch(^{
        NSLog(@"图片上传失败");
    });
}
```
可以看到代码非常简洁，可读性又好，比前两种方法都省去不少代码，这是 promise 的一大优势。我们只需把针对每张图片创建一个 promise ，放进一个 promises 数组，然后`PMKWhen(promises).then()`就能帮我们搞定一切了——是不是很神奇呢？每个任务单开线程、等待全部任务执行完、结果正确排序等诸多工序，全都由这一行代码搞定了。看看测试结果：
```cpp
```cpp
2016-05-13 15:30:45.447 HAMAFNetworkingBatchRequestDemo[23093:5713564] 第 2 张图片上传成功: {
    imageBucket = test;
    imageKey = "5d50cdd3-2272-4d3b-bbb1-054d1d08e682";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/5d50cdd3-2272-4d3b-bbb1-054d1d08e682?imageMogr2/thumbnail/640x";
}
2016-05-13 15:30:45.595 HAMAFNetworkingBatchRequestDemo[23093:5713564] 第 1 张图片上传成功: {
    imageBucket = test;
    imageKey = "ff3874d2-8477-4ceb-a49f-1938168b0456";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/ff3874d2-8477-4ceb-a49f-1938168b0456?imageMogr2/thumbnail/640x";
}
2016-05-13 15:30:46.127 HAMAFNetworkingBatchRequestDemo[23093:5713564] 第 3 张图片上传成功: {
    imageBucket = test;
    imageKey = "2b8b0175-1274-4de9-b809-7d88809ef606";
    imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/2b8b0175-1274-4de9-b809-7d88809ef606?imageMogr2/thumbnail/640x";
}
2016-05-13 15:30:46.130 HAMAFNetworkingBatchRequestDemo[23093:5713564] 上传完成!
2016-05-13 15:30:46.130 HAMAFNetworkingBatchRequestDemo[23093:5713564] (
        {
        imageBucket = test;
        imageKey = "ff3874d2-8477-4ceb-a49f-1938168b0456";
        imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/ff3874d2-8477-4ceb-a49f-1938168b0456?imageMogr2/thumbnail/640x";
    },
        {
        imageBucket = test;
        imageKey = "5d50cdd3-2272-4d3b-bbb1-054d1d08e682";
        imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/5d50cdd3-2272-4d3b-bbb1-054d1d08e682?imageMogr2/thumbnail/640x";
    },
        {
        imageBucket = test;
        imageKey = "2b8b0175-1274-4de9-b809-7d88809ef606";
        imageUrl = "http://7xnpgs.com1.z0.glb.clouddn.com/2b8b0175-1274-4de9-b809-7d88809ef606?imageMogr2/thumbnail/640x";
    }
)
```
```
同样是正确的。
所以看起来用 promise 还是非常方便的。不过这是我第一次尝试用它，还不知道在工程中实际应用会有什么缺点。
以上就是多线程批量上传图片的 3 种方法。思路最初来自 stackoverflow 的这个问题[How to send batch request by using AFNetworking 3.0](http://stackoverflow.com/questions/36081460/how-to-send-batch-request-by-using-afnetworking-3-0)，感谢这位回答的大神~
文／戴仓薯（简书作者）
原文链接：http://www.jianshu.com/p/2cb9136c837a
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
