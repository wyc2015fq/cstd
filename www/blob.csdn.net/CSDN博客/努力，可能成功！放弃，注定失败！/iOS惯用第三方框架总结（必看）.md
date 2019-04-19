# iOS惯用第三方框架总结（必看） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年04月02日 16:46:59[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：166

写iOS 程序的时候往往需要很多第三方框架的支持，可以大大减少工作量，讲重点放在软件本身的逻辑实现上。
GitHub 里面有大量优秀的第三方框架，而且 License 对商业很友好。一下摘录一下几乎每个项目都想集成的几个框架。
SDWebImageView
**1. Mantle**
Mantle 让我们能简化 Cocoa 和 Cocoa Touch 应用的 model 层。简单点说，程序中经常要进行网络请求，请求到得一般是 json 字符串，我们一般会建一个 Model 类来存放这些数据。这就要求我们编写一系列的序列化代码，来把 json 转换为 Model 。这很费时间，容易错，不容易修改。 Mantle 很好的解决了这个问题，而且更易用。
- 
GitHub ： [https://github.com/Mantle/Mantle](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FMantle%2FMantle)
- 
参考：
[工具篇： Mantle](https://link.jianshu.com?t=http%3A%2F%2Fsouthpeak.github.io%2Fblog%2F2015%2F01%2F11%2Fgong-ju-pian-%3Amantle%2F)
[源码篇： Mantle](https://link.jianshu.com?t=http%3A%2F%2Fsouthpeak.github.io%2Fblog%2F2015%2F01%2F11%2Fyuan-ma-pian-%3Amantle%2F)
- 
CocoaPod 集成： pod 'Mantle', '~> 1.5.4'
**2. Masonry**
IB 时代，如果你还在用代码绝对布局就太 low 了。随着苹果发布 iPhone6 、 iPhone 6 plus 。 iOS 设备将会出现越来越丰富的屏幕尺寸，我们不可能根据每个尺寸做一套布局。所以，使用 autolayout 就很有必要了。在 storyboard 中，可以非常方便的使用 autolayout ，但是为了更好的协作开发，有些公司依然在手写布局，令人沮丧的是苹果提供的 autolayout 语法晦涩难懂，非常影响效率（你可以在 [这里](https://link.jianshu.com?t=http%3A%2F%2Fconstraints.icodeforlove.com%2F)动态查看 autolayout 的语法）。 Masonry 就是设计来解决复杂的手写 autolayout 。如何优雅的使用 autolayout ，且看 Masonry 。
- GitHub ： [https://github.com/Masonry/Masonry](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FMasonry%2FMasonry)
- 参考：使用方法，请看 README
- CocoaPod 集成： pod 'Masonry', '~> 0.6.1'
**3. Reachability**
移动互联网时代，应该很少有应用是不需要网络连接的吧。监测网络连接状态几乎是必不可少的一部分。 Reachability 可以完美的完成这一任务
- GitHub ： [https://github.com/tonymillion/Reachability](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Ftonymillion%2FReachability)
- 参考：使用方法非常简单，请看 README
- CocoaPod 集成： pod 'Reachability', '~> 3.2'
**4. BlocksKit**
BlocksKit绝对是 Objective-C 的知心伴侣，它为 OC 常用类提供了强大的 Block 语法支持，使得编写 OC 代码变得舒适、快速、优雅。反正我是绝对离不开它。
- GitHub ： [https://github.com/zwaldowski/BlocksKit](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fzwaldowski%2FBlocksKit)
- 参考： [block 使用小结、在 arc 中使用 block 、如何防止循环引用 (zz)](https://link.jianshu.com?t=http%3A%2F%2Fblog.sina.com.cn%2Fs%2Fblog_8c87ba3b0101m599.html)
- CocoaPod 集成： pod 'BlocksKit', '~> 2.2.5'
**5. KVOController**
如果你在项目中有使用 KVO ，那么 KVOController 绝对是个好选择。它是 facebook 开源的一个 KVO 增强框架。有以下几个特性：
- 使用  Blocks 、自定义  Actions  或者  NSKeyValueObserving  回调进行通知 .
- 观测者移除时无异常
- 控制器  dealloc  时隐式的观测者移除
- 提升使用  NSKeyValueObservingInitial 的性能
- 线程安全并提供在观测者恢复时额外的保护
还有什么理由不使用 KVOController 呢？
- GitHub ： [https://github.com/facebook/KVOController](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Ffacebook%2FKVOController)
- 参考： [KVOController ： facebook 开源的  KVO （ Key-value Observing ）工具](https://link.jianshu.com?t=http%3A%2F%2Fsegmentfault.com%2Fa%2F1190000000420162)
- CocoaPod ： pod 'KVOController', '~> 1.0.3'
**6. MBProgressHUD**
一个老牌、经典的通知组件，如果你们美工没有专门设计等待和通知视图，那就用它吧！
- GitHub ： [https://github.com/jdg/MBProgressHUD](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fjdg%2FMBProgressHUD)
- 参考
- CocoaPod ： pod 'MBProgressHUD', '~> 0.9'
**7. ODRefreshControl**
很多公司都自己设计下拉刷新视图，比如网易新闻 iOS 客户端，下拉的时候会有广告出现。如果你只是需要一个下拉刷新，那么可以考虑 ODRefreshControl ，它是原 iOS6 上的橡皮糖刷新样式，很有意思。现在也很多大的 App 在用，比如虾米音乐和 QQ 客户端。
- GitHub ： [https://github.com/Sephiroth87/ODRefreshControl](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FSephiroth87%2FODRefreshControl)
- 参考
- CocoaPod ： pod 'ODRefreshControl', '~> 1.1.0'
**8. pop**
又是 Facebook 开源的。大名鼎鼎的 pop ，做动画的不二之选。收下它吧。
- GitHub ： [https://github.com/facebook/pop](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Ffacebook%2Fpop)
- 参考： [Facebook Pop  使用指南](https://link.jianshu.com?t=http%3A%2F%2Fwww.cocoachina.com%2Findustry%2F20140527%2F8565.html)[popping(pop 的使用实例 )](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fschneiderandre%2Fpopping)
- CocoaPod ： pod 'pop', '~> 1.0.7'
**9. AFNetworking**
Objective-C下网络请求库。
- GitHub ： [https://github.com/AFNetworking/AFNetworking](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FAFNetworking%2FAFNetworking)
- 参考
- CocoaPod ： pod 'AFNetworking', '~> 2.5.0'
##### 作为一个开发者，有一个学习的氛围跟一个交流圈子特别重要，这是一个我的iOS交流群：462699214，不管你是小白还是大牛欢迎入驻 ，分享面试题、面试经验，讨论技术， 大家一起交流学习成长！
## SVProgressHUD 提示效果
GitHub：[https://github.com/samvermette/SVProgressHUD](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fsamvermette%2FSVProgressHUD)
SVProgressHUD和MBProgressHUD效果差不多，不过不需要使用协议，同时也不需要声明实例。
直接通过类方法进行调用即可：
```
[SVProgressHUD method]
```
可以使用以下方法来显示状态：
```cpp
```cpp
+ (void)show;+ (void)showWithMaskType:(SVProgressHUDMaskType)maskType;+ (void)showWithStatus:(NSString*)string;+ (void)showWithStatus:(NSString*)string maskType:(SVProgressHUDMaskType)maskType;
```
```
如果需要明确的进度，则使用以下方法：
```
+ (void)showProgress:(CGFloat)progress;+ (void)showProgress:(CGFloat)progress status:(NSString*)status;+ (void)showProgress:(CGFloat)progress status:(NSString*)status maskType:(SVProgressHUDMaskType)maskType;
```
通过dismiss方法来隐藏提示：
```cpp
```cpp
+ (void)dismiss;
```
```
另外提供了以下方法用于显示状态，并在1秒后自动隐藏提示(使用的图标来源于Glyphish：[http://www.glyphish.com/](https://link.jianshu.com?t=http%3A%2F%2Fwww.glyphish.com%2F))：
```
+ (void)showSuccessWithStatus:(NSString*)string;+ (void)showErrorWithStatus:(NSString *)string;+ (void)showImage:(UIImage*)image status:(NSString*)string; // use 28x28 white pngs
```
### ZAActivityBar 提示效果
GitHub：[https://github.com/zacaltman/ZAActivityBar](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fzacaltman%2FZAActivityBar)
ZAActivityBar和SVProgressHUD非常相似，它提供了更加简洁的API来显示提示效果。
ZAActivityBar使用的动画效果来源于ZKBounceAnimation([https://github.com/khanlou/SKBounceAnimation](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fkhanlou%2FSKBounceAnimation))，成功、失败的状态图标来源于Pictos([http://pictos.cc/](https://link.jianshu.com?t=http%3A%2F%2Fpictos.cc%2F))。
显示加载状态：
```bash
[ZAActivityBar showWithStatus:@"加载中..."];
```
显示成功、失败状态：
```bash
[ZAActivityBar showSuccessWithStatus:@"成功!"];[ZAActivityBar showErrorWithStatus:@"失败!"];
```
隐藏提示：
```
```
[ZAActivityBar dismiss];
```
```
官方： [http://sbjson.org/](https://link.jianshu.com?t=http%3A%2F%2Fsbjson.org%2F)
GitHub：[https://github.com/stig/json-framework](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fstig%2Fjson-framework)
### UIActivityIndicator-for-SDWebImage 为SDWebImage显示加载效果
GitHub：[https://github.com/JJSaccolo/UIActivityIndicator-for-SDWebImage](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FJJSaccolo%2FUIActivityIndicator-for-SDWebImage)
用于为SDWebImage在UIImageView加载图片时，显示加载效果(UIActivityIndicatorView实现)，它提供以下方法：
```
```
- (void)setImageWithURL:(NSURL *)url usingActivityIndicatorStyle:(UIActivityIndicatorViewStyle)activityStyle;- (void)setImageWithURL:(NSURL *)url placeholderImage:(UIImage *)placeholder usingActivityIndicatorStyle:(UIActivityIndicatorViewStyle)activityStyle;- (void)setImageWithURL:(NSURL *)url placeholderImage:(UIImage *)placeholder options:(SDWebImageOptions)options usingActivityIndicatorStyle:(UIActivityIndicatorViewStyle)activityStyle;- (void)setImageWithURL:(NSURL *)url completed:(SDWebImageCompletedBlock)completedBlock usingActivityIndicatorStyle:(UIActivityIndicatorViewStyle)activityStyle;- (void)setImageWithURL:(NSURL *)url placeholderImage:(UIImage *)placeholder completed:(SDWebImageCompletedBlock)completedBlock usingActivityIndicatorStyle:(UIActivityIndicatorViewStyle)activityStyle;- (void)setImageWithURL:(NSURL *)url placeholderImage:(UIImage *)placeholder options:(SDWebImageOptions)options completed:(SDWebImageCompletedBlock)completedBlock usingActivityIndicatorStyle:(UIActivityIndicatorViewStyle)activityStyle;- (void)setImageWithURL:(NSURL *)url placeholderImage:(UIImage *)placeholder options:(SDWebImageOptions)options progress:(SDWebImageDownloaderProgressBlock)progressBlock completed:(SDWebImageCompletedBlock)completedBlock usingActivityIndicatorStyle:(UIActivityIndicatorViewStyle)activityStyle;
```
```
### UIImage+Resize 调整图片大小
GitHub：[https://github.com/coryalder/UIImage_Resize](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fcoryalder%2FUIImage_Resize)
提供多种方法为图片设置透明度、圆角、裁剪、调整大小等：
```
- (UIImage *)imageWithAlpha;- (UIImage *)transparentBorderImage:(NSUInteger)borderSize;- (UIImage *)roundedCornerImage:(NSInteger)cornerSize borderSize:(NSInteger)borderSize;- (UIImage *)croppedImage:(CGRect)bounds;- (UIImage *)thumbnailImage:(NSInteger)thumbnailSize          transparentBorder:(NSUInteger)borderSize               cornerRadius:(NSUInteger)cornerRadius       interpolationQuality:(CGInterpolationQuality)quality;- (UIImage *)resizedImage:(CGSize)newSize     interpolationQuality:(CGInterpolationQuality)quality;- (UIImage *)  resizedImageWithContentMode:(UIViewContentMode)contentMode                       bounds:(CGSize)bounds         interpolationQuality:(CGInterpolationQuality)quality;
```
更详细使用见：[http://vocaro.com/trevor/blog/2009/10/12/resize-a-uiimage-the-right-way/](https://link.jianshu.com?t=http%3A%2F%2Fvocaro.com%2Ftrevor%2Fblog%2F2009%2F10%2F12%2Fresize-a-uiimage-the-right-way%2F)
### ImageCacheResize 异步加载图片、缓存及调整大小
GitHub：[https://github.com/toptierlabs/ImageCacheResize](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Ftoptierlabs%2FImageCacheResize)
整合了SDWebImage和UIImage+Resize的功能，用于图片的异步加载、缓存、以及下载完成后调整大小并显示在UIImageView上。
提供了以下API用于加载图片以及加载完成后调整图片大小：
```
- (void)setImageWithURL:(NSURL *)url andCropToBounds:(CGRect)bounds;- (void)setImageWithURL:(NSURL *)url andResize:(CGSize)size withContentMode:(UIViewContentMode)mode;- (void)setImageWithURL:(NSURL *)url placeholderImage:(UIImage *)placeholder andCropToBounds:(CGRect)bounds;- (void)setImageWithURL:(NSURL *)url placeholderImage:(UIImage *)placeholder options:(SDWebImageOptions)options  andResize:(CGSize)size;- (void)setImageWithURL:(NSURL *)url placeholderImage:(UIImage *)placeholder options:(SDWebImageOptions)options  andResize:(CGSize)size withContentMode:(UIViewContentMode)mode;- (void)setImageWithURL:(NSURL *)url placeholderImage:(UIImage *)placeholder options:(SDWebImageOptions)options  andCropToBounds:(CGRect)bounds;
```
使用方法和SDWebImage一样简单，如以下官方例子：
```
[imageview setImageWithURL:[NSURL URLWithString:@"http://t0.gstatic.com/images?q=tbn:ANd9GcQfraHpiabjEY8iDdBe9OUQYHMtwfuAv9ZRR0RYKuoVF_EpE8Fp5A"] andResize:CGSizeMake(30, 30) withContentMode:UIViewContentModeScaleAspectFit]; // 按比例缩放[imageview setImageWithURL:[NSURL URLWithString:@"http://t0.gstatic.com/images?q=tbn:ANd9GcQfraHpiabjEY8iDdBe9OUQYHMtwfuAv9ZRR0RYKuoVF_EpE8Fp5A"] andCropToBounds:CGRectMake(0, 0, 100, 100)]; // 裁剪成100x100大小
```
### STableViewController  下拉刷新、上拉加载更多
GitHub：[https://github.com/shiki/STableViewController](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fshiki%2FSTableViewController)
STableViewController比PullToRefresh多了一个上拉加载更多功能，使用上也差不多简单，需要继承自STableViewController，再实现一些方法：
```
```
- (void) viewDidLoad{  [super viewDidLoad];    self.title = @"STableViewController Demo";  [self.tableView setBackgroundColor:[UIColor lightGrayColor]];    // 需要创建两个自定义视图用于显示"下拉刷新"、"上拉加载更多"  self.headerView = headerView;    self.footerView = footerView;  }#pragma mark - Pull to Refresh- (void) pinHeaderView{  [super pinHeaderView];    // 下拉刷新视图显示一些加载动画}- (void) unpinHeaderView{  [super unpinHeaderView];    // 下拉刷新视图停止动画}- (void) headerViewDidScroll:(BOOL)willRefreshOnRelease scrollView:(UIScrollView *)scrollView{  // 下拉刷新视图显示状态信息  if (willRefreshOnRelease)    //hv.title.text = @"松开后刷新...";  else    //hv.title.text = @"下拉刷新...";}- (BOOL) refresh{  if (![super refresh])    return NO;    // 下拉刷新加载数据  [self performSelector:@selector(addItemsOnTop) withObject:nil afterDelay:2.0];  return YES;}#pragma mark - Load More- (void) willBeginLoadingMore{  // 上拉加载更多视图加载动画}- (void) loadMoreCompleted{  [super loadMoreCompleted];  // 上拉加载更多视图停止动画    if (!self.canLoadMore) {    //没有更多数据的时候执行代码...  }}- (BOOL) loadMore{  if (![super loadMore])    return NO;    // 上拉加载更多数据  [self performSelector:@selector(addItemsOnBottom) withObject:nil afterDelay:2.0];  return YES;}// - (void) addItemsOnTop{  // 加载数据...    [self.tableView reloadData];    // 数据加载完成通知上拉视图  [self refreshCompleted];}- (void) addItemsOnBottom{  // 加载更多数据...  [self.tableView reloadData];    // 通过判断设置是否可以加载更多  //self.canLoadMore = NO;    // 数据加载完成通知下拉视图  [self loadMoreCompleted];}
```
```
### SVPullToRefresh 下拉刷新、上拉加载更多
GitHub：[https://github.com/samvermette/SVPullToRefresh](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fsamvermette%2FSVPullToRefresh)
包含SVPullToRefresh + SVInfiniteScrolling为UITableView提供下拉刷新、上拉加载更多功能。
使用起来也相当简单，只要在UITableViewController里实现以下方法：
```
- (void)viewDidLoad {    [super viewDidLoad];    __weak SVViewController *weakSelf = self;        // 设置下拉刷新    [self.tableView addPullToRefreshWithActionHandler:^{        [weakSelf insertRowAtTop];    }];            // 设置上拉加载更多    [self.tableView addInfiniteScrollingWithActionHandler:^{        [weakSelf insertRowAtBottom];    }];}- (void)viewDidAppear:(BOOL)animated {    [tableView triggerPullToRefresh];}- (void)insertRowAtTop {    // 获取数据....        // 停止动画    [self.tableView.pullToRefreshView stopAnimating];}- (void)insertRowAtBottom {    // 获取数据....        // 停止动画    [weakSelf.tableView.infiniteScrollingView stopAnimating];}
```
### CMPopTipView 提示信息
GitHub：[https://github.com/chrismiles/CMPopTipView](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fchrismiles%2FCMPopTipView)
CMPopTipView用于在一些视图上显示提示信息：
```
```
self.tipView = [[CMPopTipView alloc] initWithMessage:@"提示消息"];self.tipView.delegate = self;[self.tipView presentPointingAtView:anyButton inView:self.view animated:YES]; // 点击按钮显示[self.tipView presentPointingAtBarButtonItem:barButtonItem animated:YES]; // 点击导航栏按钮显示    #pragma mark CMPopTipViewDelegate methods- (void)popTipViewWasDismissedByUser:(CMPopTipView *)popTipView {  // 清理资源  self.tipView = nil;}
```
```
GitHub：[https://github.com/vicpenap/PrettyKit](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fvicpenap%2FPrettyKit)
定制了一些UI组件如UITableViewCell、UINavigationBar、UITabBar、UIToolBar等，比系统自带的更加美观。
GitHub：[https://github.com/sobri909/MGBox2](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fsobri909%2FMGBox2)
提供一些定制的UI组件可以更简单快速的创建表格、网格布局，以及丰富的文本呈现，基于block的事件机制等，包含：MGBox、MGTableBox、MGTableBoxStyled、MGScrollView、MGButton、MGEvents、MGEasyFrame、MGLine等，其中MGBox还支持screenshot方法用于截图。
GitHub：[https://github.com/jverkoey/nimbus](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fjverkoey%2Fnimbus)
著名的框架，提供了一套非常丰富的UI组件，可以使开发变得更加简单、有效率。
GitHub：[https://github.com/Grouper/FlatUIKit](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FGrouper%2FFlatUIKit)
扁平化设计的UI组件，类似于WP或者iOS7的风格。
GitHub：[https://github.com/muccy/MUKMediaGallery](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fmuccy%2FMUKMediaGallery)
媒体库效果，支持图片、视频及音频。
## PTShowcaseViewController
GitHub：[https://github.com/exalted/PTShowcaseViewController](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fexalted%2FPTShowcaseViewController)
同样是一个媒体库效果，支持的格式更多，包括：图片、视频、PDF等.
GitHub：[https://github.com/mwaterfall/MWPhotoBrowser](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fmwaterfall%2FMWPhotoBrowser)
图片展示效果，支持本地及远程的图片，使用也比较简单，只要实现MWPhotoBrowserDelegate协议：
```
@interface TestViewController ()<MWPhotoBrowserDelegate>{    NSArray *_photos;}-(void) doAction {        NSMutableArray *photos = [[NSMutableArray alloc] init];        for (...) {            MWPhoto* photo = [MWPhoto photoWithURL:[NSURL URLWithString:url]]; // 设置图片地址            photo.caption = description; // 设置描述            [photos addObject:photo];        }        _photos = photos;        MWPhotoBrowser *browser = [[MWPhotoBrowser alloc] initWithDelegate:self];        browser.displayActionButton = YES;                UINavigationController *nc = [[UINavigationController alloc] initWithRootViewController:browser];        nc.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;        [self presentModalViewController:nc animated:YES];}#pragma mark - MWPhotoBrowserDelegate- (NSUInteger)numberOfPhotosInPhotoBrowser:(MWPhotoBrowser *)photoBrowser {    return _photos.count;}- (MWPhoto *)photoBrowser:(MWPhotoBrowser *)photoBrowser photoAtIndex:(NSUInteger)index {    if (index < _photos.count)        return [_photos objectAtIndex:index];    return nil;}
```
## ios-image-filters
GitHub：[https://github.com/esilverberg/ios-image-filters](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fesilverberg%2Fios-image-filters)
提供多种图片滤镜效果。
## PDF Reader Core for iOS
GitHub：[https://github.com/vfr/Reader](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fvfr%2FReader)
PDF阅读器核心。
GitHub：[https://github.com/Cocoanetics/DTCoreText](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FCocoanetics%2FDTCoreText)
支持富文本的显示如HTML。
GitHub：[https://github.com/FuerteInternational/FTCoreText](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FFuerteInternational%2FFTCoreText)
富文本视图
GitHub：[https://github.com/akosma/CoreTextWrapper](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fakosma%2FCoreTextWrapper)
支持多列的文本视图
GitHub：[https://github.com/nicklockwood/Base64](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fnicklockwood%2FBase64)
提供对字符串的Base64编码
GitHub：[https://github.com/rnapier/RNCryptor](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Frnapier%2FRNCryptor)
### 开发几个常用的开源类库及下载地址:
1.[json](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fjson-framework)json编码解码
2.[GTMBase64](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fgoogle-toolbox-for-mac%2Fsource%2Fbrowse%2Ftrunk%2FFoundation%2F%3Fr%3D87)  base64编码解码
3.[TouchXML](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fschwa%2FTouchXML) 解析
4.[SFHFKeychainUtils](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fldandersen%2Fscifihifi-iphone)  安全保存用户密码到keychain中
5.[MBProgressHUD](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fjdg%2FMBProgressHUD)很棒的一个加载等待特效框架
6.[ASIHTTPRequest](https://link.jianshu.com?t=http%3A%2F%2Fallseeing-i.com%2FASIHTTPRequest) 等相关协议封装
7.[EGORefreshTableHeaderView](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fenormego%2FEGOTableViewPullRefresh) 下拉刷新代码
8.[AsyncImageView](https://link.jianshu.com?t=http%3A%2F%2Fwww.markj.net%2Fiphone-asynchronous-table-image%2F) 异步加载图片并缓存代码
9.[类似setting的竖立也分栏程序](https://link.jianshu.com?t=http%3A%2F%2Fblog.blackwhale.at%2F)
10.[MBProgressHUD](https://link.jianshu.com?t=http%3A%2F%2Fgithub.com%2Fjdg%2FMBProgressHUD)——进展指示符库
11.[Flurry](https://link.jianshu.com?t=http%3A%2F%2Fwww.flurry.com%2Fproduct%2Fanalytics%2Findex.html)——详尽的使用统计
12.[CorePlot](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fcore-plot%2F)——2D图形绘图仪
13.[GData client](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fgdata-objectivec-client%2F)——iPhone上所有Google相关服务的类库
14.[SDWebImage](https://link.jianshu.com?t=http%3A%2F%2Fgithub.com%2Frs%2FSDWebImage)——简化网络图片处理
15.[RegexKitLite](https://link.jianshu.com?t=http%3A%2F%2Fregexkit.sourceforge.net%2FRegexKitLite%2F)——正则表达式支持
可能会用到的库：
１.exif：svn checkout [http://iphone-exif.google.code.com/svn/trunk/ iphone-exif](https://link.jianshu.com?t=http%3A%2F%2Fiphone-exif.google.code.com%2Fsvn%2Ftrunk%2F%2520iphone-exif)
２.图像处理：svn checkout [http://simple-iphone-image-processing.googlecode.com/svn/trunk/ simple-iphone-image-processing-read-only](https://link.jianshu.com?t=http%3A%2F%2Fsimple-iphone-image-processing.googlecode.com%2Fsvn%2Ftrunk%2F%2520simple-iphone-image-processing-read-only)
AppStore软件排名相关,工欲善其事，必先利其器。
1.下载排名相关:[appannie](https://link.jianshu.com?t=http%3A%2F%2Fwww.appannie.com%2F)
2.用户行为分析:[flurry](https://link.jianshu.com?t=http%3A%2F%2Fwww.flurry.com%2F)
3.[majicrank](https://link.jianshu.com?t=http%3A%2F%2Fmajicjungle.com%2Fmajicrank.html)-各国排名查询工具 (推荐)
4.[AppViz](https://link.jianshu.com?t=http%3A%2F%2Fwww.cocoachina.com%2Fappstore%2Fsales%2F2010%2F0331%2F890.html)-App销售统计软件 (推荐)
5.[PodViz](https://link.jianshu.com?t=http%3A%2F%2Fwww.ideaswarm.com%2Fproducts%2Fappviz%2F)-用户评论等查看工具
6.[appfigures](https://link.jianshu.com?t=https%3A%2F%2Fwww.appfigures.com%2F)-报表统计分析工具
开发几个常用的开源类库及下载地址:引用1.json json编码解码2.GTMBase64 base64编码解码3.TouchXML xml解析4.SFHFKeychainUtils 安全保存用户密码到keychain中5.MBProgressHUD 很棒的一个加载等待特效框架6.ASIHTTPRequest http等相关协议封装7.EGORefreshTableHeaderView 下拉刷新代码8.AsyncImageView 异步加载图片并缓存代码9.类似setting的竖立也分栏程序
扫描wifi信息：
[http://code.google.com/p/uwecaugmentedrealityproject/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fuwecaugmentedrealityproject%2F)
[http://code.google.com/p/iphone-wireless/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fiphone-wireless%2F)
条形码扫描：[http://zbar.sourceforge.net/iphone/sdkdoc/install.html](https://link.jianshu.com?t=http%3A%2F%2Fzbar.sourceforge.net%2Fiphone%2Fsdkdoc%2Finstall.html)
tcp/ip的通讯协议：[http://code.google.com/p/cocoaasyncsocket/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fcocoaasyncsocket%2F)
voip/sip：
[http://code.google.com/p/siphon/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fsiphon%2F)
[http://code.google.com/p/asterisk-voicemail-for-iphone/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fasterisk-voicemail-for-iphone%2F)
[http://code.google.com/p/voiphone/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fvoiphone%2F)
three20：[https://github.com/facebook/three20](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Ffacebook%2Fthree20)
google gdata[：http://code.google.com/p/gdata-objectivec-client/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fgdata-objectivec-client%2F)
720全景显示panoramagl：[http://code.google.com/p/panoramagl/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fpanoramagl%2F)
jabber client：[http://code.google.com/p/ichabber/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fichabber%2F)
PLBlocks[：http://code.google.com/p/plblocks/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fplblocks%2F)
image processing
[http://code.google.com/p/simple-iphone-image-processing/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fsimple-iphone-image-processing%2F)
json编码解码:[http://code.google.com/p/json-framework](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fjson-framework)
base64编码解码:[http://code.google.com/p/google-toolbox-for-mac/source/browse/trunk/Foundation/?r=87](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Fgoogle-toolbox-for-mac%2Fsource%2Fbrowse%2Ftrunk%2FFoundation%2F%3Fr%3D87)
xml解析:[https://github.com/schwa/TouchXML](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fschwa%2FTouchXML)
安全保存用户密码到keychain中:[https://github.com/ldandersen/scifihifi-iphone](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fldandersen%2Fscifihifi-iphone)
加载等待特效框架(private api):[https://github.com/jdg/MBProgressHUD](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fjdg%2FMBProgressHUD)
http等相关协议封装:[http://allseeing-i.com/ASIHTTPRequest](https://link.jianshu.com?t=http%3A%2F%2Fallseeing-i.com%2FASIHTTPRequest)
下拉刷新代码:[https://github.com/enormego/EGOTableViewPullRefresh](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fenormego%2FEGOTableViewPullRefresh)
异步加载图片并缓存代码:[http://www.markj.net/iphone-asynchronous-table-image/](https://link.jianshu.com?t=http%3A%2F%2Fwww.markj.net%2Fiphone-asynchronous-table-image%2F)
iphone TTS:[https://bitbucket.org/sfoster/iphone-tts](https://link.jianshu.com?t=https%3A%2F%2Fbitbucket.org%2Fsfoster%2Fiphone-tts)
iphone cook book 源码:[https://github.com/erica/iphone-3.0-cookbook-](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Ferica%2Fiphone-3.0-cookbook-)
iphone  正则表达式:[http://regexkit.sourceforge.net/RegexKitLite/](https://link.jianshu.com?t=http%3A%2F%2Fregexkit.sourceforge.net%2FRegexKitLite%2F)
OAuth认证: [http://code.google.com/p/oauth/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Foauth%2F)
[http://code.google.com/p/oauthconsumer/](https://link.jianshu.com?t=http%3A%2F%2Fcode.google.com%2Fp%2Foauthconsumer%2F)
作者：唐三_iOS
链接：https://www.jianshu.com/p/ed17a42b4805
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
