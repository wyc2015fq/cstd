# YYWebImage工作原理介绍-----下载单张图片 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月25日 13:20:43[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5049
# YYWebImage简介
YYWebImage是由ibireme开发的YYkit其中的一个库，专门处理图像。github传送门：[YYWebImage](https://github.com/ibireme/YYWebImage)。他的出现是为了替代 SDWebImage、PINRemoteImage、FLAnimatedImage 等开源框架。在YYImage出来之前，我们的图片下载和动态图片展示需要SD和FL两个库去处理。现在YYImage的出现使得统一图像库成为可能。这篇文章会根据YYImage下载单张图的生命周期为主线简单的介绍一下，YYImage各个模块是如何协同合作的。
# YYWebImage使用
先引入头文件：
` #import <YYWebImage/YYWebImage.h>`
然后创建一个UIImageVIew，YYWebImage有对 UIImageVIew 的Category 方法支持。所以只要一句话：
`imvAd.cbd_imageURL=[NSURL URLWithString:@"http://ss.bdimg.com/static/superman/img/logo/bd_logo1_31bdc765.png"];`
这样图片就下载下来并展示出来，api相当简单。接下来我们就深入源代码，看看YYimage到底做了些什么。
# UIImageView+YYWebImage类
当我们点进方法以后，发现这是一个Category 方法，里面提供了很多方法，单都是基于一个方法扩展的：
```
- (void)setYy_imageURL:(NSURL *)imageURL {
    [self yy_setImageWithURL:imageURL
                 placeholder:nil
                     options:kNilOptions
                     manager:nil
                    progress:nil
                   transform:nil
                  completion:nil];
}
```
里面可以传图片的url，默认图片，动画效果类型，下载类型，下载管理器，提供的block：进度条回调，完成回调。
这个方法是整个Category的核心，代码比较长，我们分开看。
首先是初始化一个YYWebImageManager，然后动态的添加_YYWebImageSetter属性，为的是管控整个YYImage的下载，查找有没有相同的url在下载，如果有的话就要取消操作，确保同一个url只有一个队列在下载处理：
```
_YYWebImageSetter *setter = objc_getAssociatedObject(self, &_YYWebImageSetterKey);
    if (!setter) {
        setter = [_YYWebImageSetter new];
        objc_setAssociatedObject(self, &_YYWebImageSetterKey, setter, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    }
    int32_t sentinel = [setter cancelWithNewURL:imageURL];
```
紧接着YYImage会切到主线程中，做一些配置，设置一下动画，设置一下默认图片，根据刚才的options从不同的地方获取图片：
```
if ((options & YYWebImageOptionSetImageWithFadeAnimation) &&
            !(options & YYWebImageOptionAvoidSetImage)) {
            if (!self.highlighted) {
                [self.layer removeAnimationForKey:_YYWebImageFadeAnimationKey];
            }
        }
        if (!imageURL) {
            if (!(options & YYWebImageOptionIgnorePlaceHolder)) {
                self.image = placeholder;
            }
            return;
        }
        // get the image from memory as quickly as possible
        UIImage *imageFromMemory = nil;
        if (manager.cache &&
            !(options & YYWebImageOptionUseNSURLCache) &&
            !(options & YYWebImageOptionRefreshImageCache)) {
            imageFromMemory = [manager.cache getImageForKey:[manager cacheKeyForURL:imageURL] withType:YYImageCacheTypeMemory];
        }
        if (imageFromMemory) {
            if (!(options & YYWebImageOptionAvoidSetImage)) {
                self.image = imageFromMemory;
            }
            if(completion) completion(imageFromMemory, imageURL, YYWebImageFromMemoryCacheFast, YYWebImageStageFinished, nil);
            return;
        }
        if (!(options & YYWebImageOptionIgnorePlaceHolder)) {
            self.image = placeholder;
        }
```
这里要注意的是他会在YYImageCacheMemory中寻找一次图片，也就是他默认先在内存中找一下。但是默认的方法中，我们的图片是存在YYImageCacheDisk中的。我们点开路径可以看到，他是在Library/Caches/com.ibireme.yykit/images中的数据库里，我们查看一下数据库发现：
![](http://upload-images.jianshu.io/upload_images/1460494-0b2c4572388542ab.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
已经存好了。（因为我这里已经下载过这张图片了）
所以现在YYImage并没有找到图片，继续往执行，他开出一个异步线程进行下载：
```
dispatch_async([_YYWebImageSetter setterQueue], ^{
            YYWebImageProgressBlock _progress = nil;
            if (progress) _progress = ^(NSInteger receivedSize, NSInteger expectedSize) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    progress(receivedSize, expectedSize);
                });
            };
            __block int32_t newSentinel = 0;
            __block __weak typeof(setter) weakSetter = nil;
            YYWebImageCompletionBlock _completion = ^(UIImage *image, NSURL *url, YYWebImageFromType from, YYWebImageStage stage, NSError *error) {
                __strong typeof(_self) self = _self;
                BOOL setImage = (stage == YYWebImageStageFinished || stage == YYWebImageStageProgress) && image && !(options & YYWebImageOptionAvoidSetImage);
                dispatch_async(dispatch_get_main_queue(), ^{
                    BOOL sentinelChanged = weakSetter && weakSetter.sentinel != newSentinel;
                    if (setImage && self && !sentinelChanged) {
                        BOOL showFade = ((options & YYWebImageOptionSetImageWithFadeAnimation) && !self.highlighted);
                        if (showFade) {
                            CATransition *transition = [CATransition animation];
                            transition.duration = stage == YYWebImageStageFinished ? _YYWebImageFadeTime : _YYWebImageProgressiveFadeTime;
                            transition.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
                            transition.type = kCATransitionFade;
                            [self.layer addAnimation:transition forKey:_YYWebImageFadeAnimationKey];
                        }
                        self.image = image;
                    }
                    if (completion) {
                        if (sentinelChanged) {
                            completion(nil, url, YYWebImageFromNone, YYWebImageStageCancelled, nil);
                        } else {
                            completion(image, url, from, stage, error);
                        }
                    }
                });
            };
            newSentinel = [setter setOperationWithSentinel:sentinel url:imageURL options:options manager:manager progress:_progress transform:transform completion:_completion];
            weakSetter = setter;
        });
```
这里的核心是：
> 
newSentinel = [setter setOperationWithSentinel:sentinel url:imageURL options:options manager:manager progress:_progress transform:transform completion:_completion];
# YYWebImageOperation.h类
点进去以后看，发现又是一坨，仔细看会发现，大部分是赋值和一些判断，最关键的创建一个请求去获取图片：
> 
NSOperation *operation = [manager requestImageWithURL:imageURL options:options progress:progress transform:transform completion:completion];
作者对NSOperation进行了自定义，他创建了一个YYWebImageOperation，这是YYImage请求的核心。首先他重写了
start：
```
- (void)start {
    @autoreleasepool {
        [_lock lock];
        self.started = YES;
        if ([self isCancelled]) {
            [self performSelector:@selector(_cancelOperation) onThread:[[self class] _networkThread] withObject:nil waitUntilDone:NO modes:@[NSDefaultRunLoopMode]];
            self.finished = YES;
        } else if ([self isReady] && ![self isFinished] && ![self isExecuting]) {
            if (!_request) {
                self.finished = YES;
                if (_completion) {
                    NSError *error = [NSError errorWithDomain:NSURLErrorDomain code:NSURLErrorFileDoesNotExist userInfo:@{NSLocalizedDescriptionKey:@"request in nil"}];
                    _completion(nil, _request.URL, YYWebImageFromNone, YYWebImageStageFinished, error);
                }
            } else {
                self.executing = YES;
                [self performSelector:@selector(_startOperation) onThread:[[self class] _networkThread] withObject:nil waitUntilDone:NO modes:@[NSDefaultRunLoopMode]];
                if ((_options & YYWebImageOptionAllowBackgroundTask) && _YYSharedApplication()) {
                    __weak __typeof__ (self) _self = self;
                    if (_taskID == UIBackgroundTaskInvalid) {
                        _taskID = [_YYSharedApplication() beginBackgroundTaskWithExpirationHandler:^{
                            __strong __typeof (_self) self = _self;
                            if (self) {
                                [self cancel];
                                self.finished = YES;
                            }
                        }];
                    }
                }
            }
        }
        [_lock unlock];
    }
}
```
这个start方法主要是对队列的一些控制，他会再分出一个网络线程networkThread去做网络请求工作。我们顺着往下看，他进入了：
> 
-(void)_startOperation 
这里还没进入请求，YYImage会去查看硬盘内存中的图片，如果有就会执行：
> 
UIImage *image = [self.cache getImageForKey:self.cacheKey withType:YYImageCacheTypeDisk];
如果没有的话，就会去执行_startRequest方法，然后我们就看到了最重要的一段代码：
```
if (![self isCancelled]) {
            _connection = [[NSURLConnection alloc] initWithRequest:_request delegate:[_YYWebImageWeakProxy proxyWithTarget:self]];
            if (![_request.URL isFileURL] && (_options & YYWebImageOptionShowNetworkActivity)) {
                [YYWebImageManager incrementNetworkActivityCount];
            }
        }
```
这里请求还是用的快要淘汰的NSURLConnection，- -让人潸然泪下....接下来就是一些回调了。整个流程结束！
整个流程图如下：
![](http://upload-images.jianshu.io/upload_images/1460494-9c08b49475c69216.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 总结
由此看出YYImage使用非常简单，但内部构造比较复杂。主要的核心模块是：_YYWebImageSetter，YYImageCache，YYWebImageManager，YYWebImageOperation。分工有序，结构清晰，美中不足的是请求方式还是基于快要淘汰的NSURLConnection。不过要修改起来也比较容易，之后我会对YYImage的其他功能做介绍分析，如果你感兴趣可以关注我。如果有任何问题可以留言我。
# 感谢
YYImage：[https://github.com/ibireme/YYWebImage](https://github.com/ibireme/YYWebImage)
文／NiubilityLeo（简书作者）
原文链接：http://www.jianshu.com/p/6c8bb7c1183f
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
