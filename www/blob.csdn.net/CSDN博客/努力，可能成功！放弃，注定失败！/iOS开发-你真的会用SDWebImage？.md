# iOS开发-你真的会用SDWebImage？ - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月14日 16:38:09[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：456
#### SDWebImage作为目前最受欢迎的图片下载第三方框架，使用率很高。但是你真的会用吗？本文接下来将通过例子分析如何合理使用SDWebImage。
#### 使用场景：自定义的UITableViewCell上有图片需要显示，要求网络网络状态为WiFi时，显示图片高清图；网络状态为蜂窝移动网络时，显示图片缩略图。如下图样例：
本文来自简书，原文地址:[http://www.jianshu.com/p/dabc0c6d083e](http://www.jianshu.com/p/dabc0c6d083e)
![](http://upload-images.jianshu.io/upload_images/1353118-c335652e795beb0c?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图中显示的图片符合根据网络状态下载要求
- 由于要监听网络状态，在这里笔者推荐使用AFNetWorking。
- 在GitHub或者利用cocoaPod给项目导入第三方框架AFNetWorking。
- 在AppDelegate.m文件中的application:didFinishLaunchingWithOptions:方法中监听网络状态。
```
// AppDelegate.m 文件中
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions 
{
    // 监控网络状态
    [[AFNetworkReachabilityManager sharedManager] startMonitoring];
}
// 以下代码在需要监听网络状态的方法中使用
AFNetworkReachabilityManager *mgr = [AFNetworkReachabilityManager sharedManager];
    if (mgr.isReachableViaWiFi)     { // 在使用Wifi, 下载原图
    } else     { // 其他，下载小图
    }
 }
```
- 这时就会有iOS学习者开始抱怨：这不是很简单吗？于是三下五除二写完了以下代码。
```
// 利用MVC，在设置cell的模型属性时候，下载图片
- setItem:(CustomItem *)item
{
    _item = item;
    UIImage *placeholder = [UIImage imageNamed:@"placeholderImage"];
    AFNetworkReachabilityManager *mgr = [AFNetworkReachabilityManager sharedManager];
    if (mgr.isReachableViaWiFi) { // 在使用Wifi, 下载原图
        [self.imageView sd_setImageWithURL:[NSURL URLWithString:item.originalImage] placeholderImage:placeholder];
    } else { // 其他，下载小图
        [self.imageView sd_setImageWithURL:[NSURL URLWithString:item.thumbnailImage] placeholderImage:placeholder];
    }
}
```
- 
此时，确实能完成基本的按照当前网络状态下载对应的图片，但是真实开发中，这样其实是不合理的。以下是需要注意的细节：
- SDWebImage会自动帮助开发者缓存图片（包括内存缓存，沙盒缓存），所以我们需要设置用户在`WiFi`环境下下载的`高清图`，下次在`蜂窝网络状态`下打开应用也应显示`高清图`，而不是去下载缩略图。
- 许多应用设置模块带有一个功能：`移动网络环境下仍然显示高清图`。这个功能其实是将设置记录在沙盒中，关于数据保存到本地，可以查看本人另一篇简书首页文章
[iOS本地数据存取，看这里就够了](http://www.jianshu.com/p/a3eeae99e902)
- 当用户处于离线状态时候，无法合理处理业务。
- 
于是，开始加以改进。为了让读者你更容易理解，我先贴出伪代码：
```
- setItem:(CustomItem *)item
{
    _item = item;
    if (缓存中有原图) 
    {
        self.imageView.image = 原图;
    } else 
    {
        if (Wifi环境) 
        {
            下载显示原图
        } else if (手机自带网络) 
        {
            if (3G\4G环境下仍然下载原图) 
            {
                下载显示原图
            } else 
            {
                下载显示小图
            }
        } else 
        {
            if (缓存中有小图) 
            {
                self.imageView.image = 小图;
            } else  // 处理离线状态
            {
                self.imageView.image = 占位图片;
            }
        }
    }
}
```
- 实现上面的伪代码：读者可以一一对应上面的伪代码。练习的时候推荐`先写伪代码`，再写真实代码
- 多多注意`注释`解释。
```
- setItem:(CustomItem *)item
{
    _item = item;
     // 占位图片
    UIImage *placeholder = [UIImage imageNamed:@"placeholderImage"];
    // 从内存\沙盒缓存中获得原图，
    UIImage *originalImage = [[SDImageCache sharedImageCache] imageFromDiskCacheForKey:item.originalImage];
    if (originalImage) { // 如果内存\沙盒缓存有原图，那么就直接显示原图（不管现在是什么网络状态）
        self.imageView.image = originalImage;
    } else { // 内存\沙盒缓存没有原图
        AFNetworkReachabilityManager *mgr = [AFNetworkReachabilityManager sharedManager];
        if (mgr.isReachableViaWiFi) { // 在使用Wifi, 下载原图
            [self.imageView sd_setImageWithURL:[NSURL URLWithString:item.originalImage] placeholderImage:placeholder];
        } else if (mgr.isReachableViaWWAN) { // 在使用手机自带网络
            //     用户的配置项假设利用NSUserDefaults存储到了沙盒中
            //    [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"alwaysDownloadOriginalImage"];
            //    [[NSUserDefaults standardUserDefaults] synchronize];
#warning 从沙盒中读取用户的配置项：在3G\4G环境是否仍然下载原图
            BOOL alwaysDownloadOriginalImage = [[NSUserDefaults standardUserDefaults] boolForKey:@"alwaysDownloadOriginalImage"];
            if (alwaysDownloadOriginalImage) { // 下载原图
                [self.imageView sd_setImageWithURL:[NSURL URLWithString:item.originalImage] placeholderImage:placeholder];
            } else { // 下载小图
                [self.imageView sd_setImageWithURL:[NSURL URLWithString:item.thumbnailImage] placeholderImage:placeholder];
            }
        } else { // 没有网络
            UIImage *thumbnailImage = [[SDImageCache sharedImageCache] imageFromDiskCacheForKey:item.thumbnailImage];
            if (thumbnailImage) { // 内存\沙盒缓存中有小图
                self.imageView.image = thumbnailImage;
            } else { // 处理离线状态，而且有没有缓存时的情况
                self.imageView.image = placeholder;
            }
        }
    }
}
```
#### 解决了吗？真正的坑才刚刚开始。
- 在表述上述代码的坑之前，我们先来分析一下UITableViewCell的缓存机制。
- 请看下图：有一个tableView正在`同时`显示三个UITableViewCell，每个`tableViewCell`包含一个imageView的子控件，而且每个cell都有一个`对应的模型属性`用来设置imageView的图片内容。
- 
`注意`：由于没有cell被推出屏幕，此时缓存池为空。
![](http://upload-images.jianshu.io/upload_images/1353118-c86dd6b6ab05e229?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
cell还没有被推入缓存池
- 
当有一个cell被推到屏幕之外时，系统会自动将这个cell放入自动缓存池。`注意`:cell对应的UIImage图片数据模型并没有清空！还是指向上一个使用的cell。
![](http://upload-images.jianshu.io/upload_images/1353118-d0c90e08bb2416bd?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
cell被放入缓存池
- 
当下一个cell进入屏幕，系统会根据tableView注册的标识找到对应的cell，拿来应用。上述进入缓存池的cell被`重新`添加进tableView，在tableView的Data
 Source方法`tableView: cellForRowAtIndexPath:`中设置改cell`对应的模型`数据，此时cell对应的如图：
![](http://upload-images.jianshu.io/upload_images/1353118-008b06a22a88fa7f?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
cell被放入缓存池
- 以上就是tableView重用机制的简单介绍。
#### 重新回来，那么上面所说的真正的坑在哪呢？
- 用一个场景来描述一下吧：当用户所处环境WiFi网速`不够快`(不能立即将图片下载完毕)，而在上述代码，在WiFi环境下又是下载高清大图。所以需要`一定的时间`来完成下载。而就在此时，用户不愿等，想看看`上次打开`App时显示的图片，此时用户会滑到处于`下面的cell`来查看。注意：`此时，上面的cell下载图片操作并没有暂停，还在处于下载图片状态中`。当用户在查看`上次打开App`的显示图片时（上次打开App下载完成的图片，SDWebImage会帮我们缓存，不用下载），而正好需要显示`上次打开App时的图片`的cell是利用tableView`重用机制`而从缓存池中拿出来的cell，等到处于`上面的cell`的高清大图已经下载好了的时候，SDWebImage默认做法是，立马把下载好的图片设置给ImageView，所以我们这时候会在底下的显示的cell显示上面的图片，造成`数据错乱`，这是非常严重的BUG。
#### 那么该如何解决这个棘手的问题呢？
- 如果我们能在cell被从缓存池中拿出来使用的时候，将这个cell放入缓存池之前的`下载操作`移除，那么就不会出现数据错乱了。
- 这时候你可能会问我：怎么移除下载操作？下载操作不是SDWebImage帮我们做的吗？
- 说的没错，确实是SDWebImage帮我们下载图片的，我们来扒一扒SDWebImage源码，看看他是怎么完成的。
```
- (void)sd_setImageWithURL:(NSURL *)url placeholderImage:(UIImage *)placeholder options:(SDWebImageOptions)options progress:(SDWebImageDownloaderProgressBlock)progressBlock completed:(SDWebImageCompletionBlock)completedBlock {
    // 关闭当前图片的下载操作
    [self sd_cancelCurrentImageLoad];
    objc_setAssociatedObject(self, &imageURLKey, url, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    if (!(options & SDWebImageDelayPlaceholder)) {
        dispatch_main_async_safe(^{
            self.image = placeholder;
        });
    }
    if (url) {
        // check if activityView is enabled or not
        if ([self showActivityIndicatorView]) {
            [self addActivityIndicator];
        }
        __weak __typeof(self)wself = self;
        id <SDWebImageOperation> operation = [SDWebImageManager.sharedManager downloadImageWithURL:url options:options progress:progressBlock completed:^(UIImage *image, NSError *error, SDImageCacheType cacheType, BOOL finished, NSURL *imageURL) {
            [wself removeActivityIndicator];
            if (!wself) return;
            dispatch_main_sync_safe(^{
                if (!wself) return;
                if (image && (options & SDWebImageAvoidAutoSetImage) && completedBlock)
                {
                    completedBlock(image, error, cacheType, url);
                    return;
                }
                else if (image) {
                    wself.image = image;
                    [wself setNeedsLayout];
                } else {
                    if ((options & SDWebImageDelayPlaceholder)) {
                        wself.image = placeholder;
                        [wself setNeedsLayout];
                    }
                }
                if (completedBlock && finished) {
                    completedBlock(image, error, cacheType, url);
                }
            });
        }];
        [self sd_setImageLoadOperation:operation forKey:@"UIImageViewImageLoad"];
    } else {
        dispatch_main_async_safe(^{
            [self removeActivityIndicator];
            if (completedBlock) {
                NSError *error = [NSError errorWithDomain:SDWebImageErrorDomain code:-1 userInfo:@{NSLocalizedDescriptionKey : @"Trying to load a nil url"}];
                completedBlock(nil, error, SDImageCacheTypeNone, url);
            }
        });
    }
}
```
#### 我们惊奇的发现，原来SDWebImage在下载图片时，第一件事就是关闭imageView当前的下载操作！
- 是不是开始感叹SDWebImage多么神奇了？没错，我们只需要把我们写的那段代码所有的直接访问本地缓存代码利用SDWebImage进行设置就OK了！
- 下面就是完成版代码。
```
- setItem:(CustomItem *)item
{
    _item = item;
  // 占位图片
    UIImage *placeholder = [UIImage imageNamed:@"placeholderImage"];
    // 从内存\沙盒缓存中获得原图
    UIImage *originalImage = [[SDImageCache sharedImageCache] imageFromDiskCacheForKey:item.originalImage];
    if (originalImage) { // 如果内存\沙盒缓存有原图，那么就直接显示原图（不管现在是什么网络状态）
        [self.imageView sd_setImageWithURL:[NSURL URLWithString:item.originalImage] placeholderImage:placeholder];
    } else { // 内存\沙盒缓存没有原图
        AFNetworkReachabilityManager *mgr = [AFNetworkReachabilityManager sharedManager];
        if (mgr.isReachableViaWiFi) { // 在使用Wifi, 下载原图
            [self.imageView sd_setImageWithURL:[NSURL URLWithString:item.originalImage] placeholderImage:placeholder];
        } else if (mgr.isReachableViaWWAN) { // 在使用手机自带网络
            //     用户的配置项假设利用NSUserDefaults存储到了沙盒中
            //    [[NSUserDefaults standardUserDefaults] setBool:NO forKey:@"alwaysDownloadOriginalImage"];
            //    [[NSUserDefaults standardUserDefaults] synchronize];
#warning 从沙盒中读取用户的配置项：在3G\4G环境是否仍然下载原图
            BOOL alwaysDownloadOriginalImage = [[NSUserDefaults standardUserDefaults] boolForKey:@"alwaysDownloadOriginalImage"];
            if (alwaysDownloadOriginalImage) { // 下载原图
                [self.imageView sd_setImageWithURL:[NSURL URLWithString:item.originalImage] placeholderImage:placeholder];
            } else { // 下载小图
                [self.imageView sd_setImageWithURL:[NSURL URLWithString:item.thumbnailImage] placeholderImage:placeholder];
            }
        } else { // 没有网络
            UIImage *thumbnailImage = [[SDImageCache sharedImageCache] imageFromDiskCacheForKey:item.thumbnailImage];
            if (thumbnailImage) { // 内存\沙盒缓存中有小图
                [self.imageView sd_setImageWithURL:[NSURL URLWithString:item.thumbnailImage] placeholderImage:placeholder];
            } else {
                [self.imageView sd_setImageWithURL:nil placeholderImage:placeholder];
            }
        }
    }
}
```
## 本文就介绍到这里，如有疑问或错误，欢迎指出。喜欢就点个关注吧。
