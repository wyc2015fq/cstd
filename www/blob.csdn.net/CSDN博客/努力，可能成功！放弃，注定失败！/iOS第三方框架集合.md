# iOS第三方框架集合 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月18日 10:51:59[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1365
在iOS开发中不可避免的会用到一些第三方类库，它们提供了很多实用的功能，使我们的开发变得更有效率；同时，也可以从它们的源代码中学习到很多有用的东西。
# Reachability 检测网络连接
用来检查网络连接是否可用：包括WIFI和WWAN(3G/EDGE/CDMA等)两种工作模式。
现在有更好的替代品：[https://github.com/tonymillion/Reachability，比Apple提供的兼容性更好，而且更加好用，更具体的使用方法请看它提供的例子。](https://github.com/tonymillion/Reachability%EF%BC%8C%E6%AF%94Apple%E6%8F%90%E4%BE%9B%E7%9A%84%E5%85%BC%E5%AE%B9%E6%80%A7%E6%9B%B4%E5%A5%BD%EF%BC%8C%E8%80%8C%E4%B8%94%E6%9B%B4%E5%8A%A0%E5%A5%BD%E7%94%A8%EF%BC%8C%E6%9B%B4%E5%85%B7%E4%BD%93%E7%9A%84%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95%E8%AF%B7%E7%9C%8B%E5%AE%83%E6%8F%90%E4%BE%9B%E7%9A%84%E4%BE%8B%E5%AD%90%E3%80%82)
> 
Reachability* reach = [Reachability reachabilityWithHostname:@"www.google.com"];reach.reachableBlock = ^(Reachability*reach) { NSLog(@"网络可用!");};reach.unreachableBlock = ^(Reachability*reach) { NSLog(@"网络不可用!");};// 开始监听[reach
 startNotifier];
# ASIHTTPRequest 网络请求
ASIHTTPRequest是对CFNetwork API的一个包装，它提供了一套更加简洁的API，使用起来也更加简单。
官方网站：[http://allseeing-i.com/ASIHTTPRequest/](http://allseeing-i.com/ASIHTTPRequest/)
GitHub：[https://github.com/pokeb/asi-http-request](https://github.com/pokeb/asi-http-request)
它不仅仅支持基本的HTTP请求，而且支持基于REST的服务(GET/POST/PUT/DELETE)。
最让人喜欢的是，它支持block语法：
> 
NSURL *url = [NSURL URLWithString:@"[http://allseeing-i.com](http://allseeing-i.com)"]; __block ASIHTTPRequest*request = [ASIHTTPRequest requestWithURL:url]; [request setCompletionBlock:^{ // Use when fetching text data NSString
*responseString = [request responseString]; // Use when fetching binary data NSData*responseData = [request responseData]; }]; [request setFailedBlock:^{ NSError *error = [request error]; }]; [request startAsynchronous];
它的ASIFormDataRequest子类可以横容易的提交表单数据和文件：
> 
ASIFormDataRequest *request = [ASIFormDataRequest requestWithURL:url];[request setPostValue:@"Ben" forKey:@"first_name"];[request setPostValue:@"Copsey" forKey:@"last_name"];// Upload a file on disk[request setFile:@"/Users/ben/Desktop/ben.jpg"
 withFileName:@"myphoto.jpg" andContentType:@"image/jpeg"forKey:@"photo"]; // Upload an NSData instance[request setData:imageData withFileName:@"myphoto.jpg" andContentType:@"image/jpeg" forKey:@"photo"];
详细的使用方法请下载相应的源代码及例子，或者从官方的使用说明[http://allseeing-i.com/ASIHTTPRequest/How-to-use开始。](http://allseeing-i.com/ASIHTTPRequest/How-to-use%E5%BC%80%E5%A7%8B%E3%80%82)
# MBProgressHUD 提示效果
支持各种状态加载的提示效果，以及带进度的提示效果。
GitHub：[https://github.com/matej/MBProgressHUD](https://github.com/matej/MBProgressHUD)
一般会在.m文件实现MBProgressHUDDelegate协议，并声明HUD变量：
> 
@interface SampleViewController ()<MBProgressHUDDelegate>{ MBProgressHUD
*HUD;}#pragma mark -#pragma mark MBProgressHUDDelegate methods- (void)hudWasHidden:(MBProgressHUD*)hud { // Remove HUD from screen when the HUD was hidded [HUD removeFromSuperview]; HUD = nil;}
在执行某个异步请求时开始调用：
> 
HUD = [MBProgressHUD showHUDAddedTo:self.webView animated:YES]; HUD.labelText = @"正在请求..."; // mode参数可以控制显示的模式 //HUD.mode = MBProgressHUDModeText; HUD.delegate = self;
请求完成时隐藏提示效果：
> 
[HUD hide:YES];
对于同步方法一般都是用showWhileExecuting方法，方法执行完成之后会自动隐藏提示效果：
> 
[HUD showWhileExecuting:@selector(myTask) onTarget:self withObject:nil animated:YES];
# SVProgressHUD 提示效果
GitHub：[https://github.com/samvermette/SVProgressHUD](https://github.com/samvermette/SVProgressHUD)
SVProgressHUD和MBProgressHUD效果差不多，不过不需要使用协议，同时也不需要声明实例。
直接通过类方法进行调用即可：
> 
[SVProgressHUD method]
可以使用以下方法来显示状态：
> - (void)show;+ (void)showWithMaskType:(SVProgressHUDMaskType)maskType;+ (void)showWithStatus:(NSString*)string;+ (void)showWithStatus:(NSString*)string maskType:(SVProgressHUDMaskType)maskType;
如果需要明确的进度，则使用以下方法：
> 
(void)showProgress:(CGFloat)progress;+ (void)showProgress:(CGFloat)progress status:(NSString*)status;+ (void)showProgress:(CGFloat)progress status:(NSString*)status maskType:(SVProgressHUDMaskType)maskType;
通过dismiss方法来隐藏提示：
> 
(void)dismiss;
另外提供了以下方法用于显示状态，并在1秒后自动隐藏提示(使用的图标来源于Glyphish：[http://www.glyphish.com/)：](http://www.glyphish.com/)%EF%BC%9A)
> 
(void)showSuccessWithStatus:(NSString*)string;+ (void)showErrorWithStatus:(NSString*)string;+ (void)showImage:(UIImage*)image status:(NSString*)string; // use 28x28 white pngs
# ZAActivityBar 提示效果
GitHub：[https://github.com/zacaltman/ZAActivityBar](https://github.com/zacaltman/ZAActivityBar)
ZAActivityBar和SVProgressHUD非常相似，它提供了更加简洁的API来显示提示效果。
ZAActivityBar使用的动画效果来源于ZKBounceAnimation([https://github.com/khanlou/SKBounceAnimation)，成功、失败的状态图标来源于Pictos(http://pictos.cc/)。](https://github.com/khanlou/SKBounceAnimation)%EF%BC%8C%E6%88%90%E5%8A%9F%E3%80%81%E5%A4%B1%E8%B4%A5%E7%9A%84%E7%8A%B6%E6%80%81%E5%9B%BE%E6%A0%87%E6%9D%A5%E6%BA%90%E4%BA%8EPictos(http://pictos.cc/)%E3%80%82)
> 
显示加载状态：
[ZAActivityBar showWithStatus:@"加载中..."];
显示成功、失败状态：
[ZAActivityBar showSuccessWithStatus:@"成功!"];[ZAActivityBar showErrorWithStatus:@"失败!"];
隐藏提示：
[ZAActivityBar dismiss];
# SDWebImage 图片异步加载及缓存
SDWebImage用于异步下载网络上的图片，并支持对图片的缓存等。
多数情况下是使用UIImageView+WebCache为UIImageView异步加载图片：
# UIActivityIndicator-for-SDWebImage 为SDWebImage显示加载效果
GitHub：[https://github.com/JJSaccolo/UIActivityIndicator-for-SDWebImage](https://github.com/JJSaccolo/UIActivityIndicator-for-SDWebImage)
用于为SDWebImage在UIImageView加载图片时，显示加载效果(UIActivityIndicatorView实现)
# UIImage+Resize 调整图片大小
GitHub：[https://github.com/coryalder/UIImage_Resize](https://github.com/coryalder/UIImage_Resize)
提供多种方法为图片设置透明度、圆角、裁剪、调整大小等：
> - (UIImage *)imageWithAlpha;- (UIImage *)transparentBorderImage:(NSUInteger)borderSize;- (UIImage
*)roundedCornerImage:(NSInteger)cornerSize borderSize:(NSInteger)borderSize;- (UIImage*)croppedImage:(CGRect)bounds;- (UIImage *)thumbnailImage:(NSInteger)thumbnailSize transparentBorder:(NSUInteger)borderSize cornerRadius:(NSUInteger)cornerRadius interpolationQuality:(CGInterpolationQuality)quality;- (UIImage*)resizedImage:(CGSize)newSize interpolationQuality:(CGInterpolationQuality)quality;- (UIImage *) resizedImageWithContentMode:(UIViewContentMode)contentMode bounds:(CGSize)bounds interpolationQuality:(CGInterpolationQuality)quality;
更详细使用见：[http://vocaro.com/trevor/blog/2009/10/12/resize-a-uiimage-the-right-way/](http://vocaro.com/trevor/blog/2009/10/12/resize-a-uiimage-the-right-way/)
# ImageCacheResize 异步加载图片、缓存及调整大小
GitHub：[https://github.com/toptierlabs/ImageCacheResize](https://github.com/toptierlabs/ImageCacheResize)
整合了SDWebImage和UIImage+Resize的功能，用于图片的异步加载、缓存、以及下载完成后调整大小并显示在UIImageView上。
提供了API用于加载图片以及加载完成后调整图片大小
# EGOTableViewPullRefresh 下拉刷新
GitHub：[https://github.com/enormego/EGOTableViewPullRefresh](https://github.com/enormego/EGOTableViewPullRefresh)
这是最早出现的为UITableView提供下拉刷新功能的类库，使用起来稍显麻烦，需要实现诸多协议(代码取自官方DEMO)：
# PullToRefresh 下拉刷新
GitHub：[https://github.com/leah/PullToRefresh](https://github.com/leah/PullToRefresh)
PullToRefresh提供比EGOTableViewPullRefresh更加简单的使用方法，只要继承自PullRefreshTableViewController，再实现refresh方法即可：
# STableViewController 下拉刷新、上拉加载更多
GitHub：[https://github.com/shiki/STableViewController](https://github.com/shiki/STableViewController)
STableViewController比PullToRefresh多了一个上拉加载更多功能，使用上也差不多简单，需要继承自STableViewController，再实现一些方法
# GitHub：[https://github.com/vicpenap/PrettyKit](https://github.com/vicpenap/PrettyKit)
定制了一些UI组件如UITableViewCell、UINavigationBar、UITabBar、UIToolBar等，比系统自带的更加美观。
# GitHub：[https://github.com/sobri909/MGBox2](https://github.com/sobri909/MGBox2)
提供一些定制的UI组件可以更简单快速的创建表格、网格布局，以及丰富的文本呈现，基于block的事件机制等，包含：MGBox、MGTableBox、MGTableBoxStyled、MGScrollView、MGButton、MGEvents、MGEasyFrame、MGLine等，其中MGBox还支持screenshot方法用于截图。
# GitHub：[https://github.com/jverkoey/nimbus](https://github.com/jverkoey/nimbus)
著名的框架，提供了一套非常丰富的UI组件，可以使开发变得更加简单、有效率。
# GitHub：[https://github.com/Grouper/FlatUIKit](https://github.com/Grouper/FlatUIKit)
扁平化设计的UI组件，类似于WP或者iOS7的风格。
# GitHub：[https://github.com/muccy/MUKMediaGallery](https://github.com/muccy/MUKMediaGallery)
媒体库效果，支持图片、视频及音频。
PTShowcaseViewController
# GitHub: [https://github.com/exalted/PTShowcaseViewController](https://github.com/exalted/PTShowcaseViewController)
同样是一个媒体库效果，支持的格式更多，包括：图片、视频、PDF等.
# GitHub：[https://github.com/mwaterfall/MWPhotoBrowser](https://github.com/mwaterfall/MWPhotoBrowser)
图片展示效果，支持本地及远程的图片，使用也比较简单，只要实现MWPhotoBrowserDelegate协议：
# ios-image-filters
GitHub：[https://github.com/esilverberg/ios-image-filters](https://github.com/esilverberg/ios-image-filters)
提供多种图片滤镜效果。
# PDF Reader Core for iOS
GitHub：[https://github.com/vfr/Reader](https://github.com/vfr/Reader)
PDF阅读器核心。
GitHub：[https://github.com/Cocoanetics/DTCoreText](https://github.com/Cocoanetics/DTCoreText)
支持富文本的显示如HTML。
GitHub：[https://github.com/FuerteInternational/FTCoreText](https://github.com/FuerteInternational/FTCoreText)
富文本视图
GitHub：[https://github.com/akosma/CoreTextWrapper](https://github.com/akosma/CoreTextWrapper)
支持多列的文本视图
GitHub：[https://github.com/nicklockwood/Base64](https://github.com/nicklockwood/Base64)
提供对字符串的Base64编码
GitHub：[https://github.com/rnapier/RNCryptor](https://github.com/rnapier/RNCryptor)
提供AES加密方法
# 1. AFNetworking
在众多iOS开源项目中，AFNetworking可以称得上是最受开发者欢迎的库项目。AFNetworking是一个轻量级的iOS、Mac OS X网络通信类库，现在是GitHub上第三大Objective-C库。它建立在NSURLConnection、NSOperation等类库的基础上，让很多网络通信功能的实现变得十分简单，因此，许多iOS应用开发都会使用到它。
支持HTTP请求和基于REST的网络服务（包括GET、POST、PUT、DELETE等）；
支持ARC；
要求iOS 5.0及以上版本；
有一些插件扩展已有的功能，还有一个功能齐全的API；
从URL中获取JSON特别简单。
详情请参考：AFNetworking官网
# 2. Three20
Three20原本是iPhone版Facebook中所使用的工具库，包括照片查看器等一系列的iPhone UI类集，以及HTTP磁盘缓存等一些通用工具。后来从Facebook iPhone应用中剥离出来，成为了一个深受开发者喜爱的通用框架。
# 3. facebook-ios-sdk
此前在“GitHub上最受欢迎的开源项目”Android系列文章（一）中，我们曾介绍过允许开发者将Facebook集成到Android应用中的Facebook SDK for Android。Facebook SDK for iOS和它一样，可以让开发者将Facebook相关功能集成到自己的iOS App中。
# 4. RestKit
Restkit是一个主要用于iOS上网络通信的开源Objective-C框架，除了发送请求、接受响应这些基本功能外，还附带Core Data，以及将远程JSON映射为本地对象的功能。
主要特点：
可在iOS和Mac OS X的Objective-C中与RESTful Web服务进行简单交互；
包含简单的HTTP Request/Response API；
带有强大的对象映射系统，用于减少代码长度；
RestKit可降低JSON/XML的处理的资源消耗，支持通过SBJSON和YAJL进行JSON解析。
详情请参考：Restkit
# 5. asi-http-request
ASIHTTPRequest是一款极其强劲的HTTP访问开源项目，能够让简单的API完成非常复杂的功能，比如异步请求、队列请求、GZIP压缩、缓存、断点续传、进度跟踪、上传文件、HTTP认证。
ASIHTTPRequest适用于基本的HTTP请求，和基于REST的服务之间的交互。使用Objective-C编写，能够同时用于Mac OS X和iPhone应用中。
详情请参考：all-seeing.ASIHTTPRequest
# 6. GPUImage
GPUImage是一个基于GPU图像和视频处理的开源iOS框架。
主要功能如下：
提供各种各样的图像处理滤镜，并且支持照相机和摄像机的实时滤镜；
GPUImage顾名思义，是基于GPU的图像加速，因此图像处理速度非常快，并且能够自定义图像滤镜；
支持ARC。
# 7. MonoGame
MonoGame是一个Microsoft XNA 4.x Framework的开源跨平台实现。此前在Android开源项目系列文章（一）中我们也进行了详细的介绍。
MonoGame支持平台：
iOS（包括Ritina Display）
Android
Windows（OpenGL）
Mac OS X
Linux
Windows Store Apps（Windows 8、Windows RT）
Windows Phone 8
PlayStation Mobile（目前仅支持2D）
OUYA
# 8. Nimbus
Nimbus是一个开源的iOS框架，比起Three20，Nimbus的文档更为全面、丰富，能够实现很多非常炫的界面特效。因此，开发者可以借助Nimbus来降低项目设计的复杂度。
# 9. cheddar-ios
Cheddar是一个简单即时的任务管理器，Cheddar for iOS是Cheddar的iOS客户端，通用于iPhone和iPad。
开发者可以直接登陆App Store下载该应用。
# 10. ViewDeck
IIViewDeckController能够实现类似于Path 2.0 的视图左右滑动的效果，支持向左或向右顺滑的滑动操作。
点击链接，查看视频演示（常规演示、灵敏性演示），注意，需翻&墙的哟。
# 11. GMGridView
GMGridView是一款开源的iOS（iPhone/iPad）表格视图，允许用户手势对表格单元进行排序，在单元格需要展示时才进行装载，这样极大地提高了表格的效率。其中的伸缩/旋转/平移手势能够让用户改变视图，还能够实现从CellView到全屏的切换。
详情请参考：gmoledina
# 12. QuickDialog
QuickDialog可以帮助开发者快速创建复杂的表单，实现包括登录界面在内的各种样式的TableView输入界面，此外，还可以创建带有多个文本域的表格及项目。
开发者可以直接登陆QuickDialog官方网站，获取更多信息或文档。
# 13. appirater
Appirater是一个可以直接使用到任何iPhone应用（iOS4.0及以上）中的开源类，用于提醒用户在打开App时，对应用进行评论或打分。
详情请参考：Appirater group
# 14. SVProgressHUD
SVProgressHUD能够实现多种HUD效果，多用于程序正在执行耗时较长的任务，需要用户等待。除了显示等待的HUD，还可以显示命令执行成功或者失败的HUD。
# 15. Reader
该项目能够让iOS开发者轻而易举地在iOS设备屏幕上显示PDF文件。代码通用，不需要任何XIB（因为所有UI元素都是代码生成的，具有极大的灵活性），运行于iOS 4.0及其以上版本设备中，同时还支持所有Retina Display设备。
支持：
诸如iBooks等的文档导航；
设备全方位旋转；
对PDF进行加密（密码保护）；
PDF链接（URI及跳转页面）；
PDF旋转页面。
# 16. CocoaAsyncSocket
CocoaAsyncSocket提供了十分强大而又易用的Mac OS X及iOS异步套接库，支持TCP和UDP，其中，AsyncSocket类是支持TCP的，AsyncUdpSocket是支持UDP的。
AsyncSocket是封装了CFSocket和CFSteam的TCP/IP socket网络库，提供异步操作。AsyncUdpSocket是UDP/IP socket网络库，包装自CFSocket。
# 17. SSToolkit
SSToolkit可以说是iOS开发者最常使用的工具类之一，该开源项目提供了一个完整的iOS类库集合，用于解决iOS开发者在开发过程中常碰到的一些问题，比如追踪一款设备是否具有视网膜显示屏或是否能剪裁图片，包含SSCollectionView、SSGradientView、SSSwitch等诸多非常方便的类。
如果想要对SSToolkit作进一步了解，可以在iPad/iPhone上使用SSToolkit的Demo应用SSCatalog。获取更多信息，可直接登陆S.S. TOOLKIT。
# 18 XMPPFramework
XMPPFramework是一个使用Objective-C编写的用于Mac/iOS开发的XMPP框架，它提供了一个RFC-3920（XMPP标准）核心实现，通过某些特定工具读写XML。XMPPFramework有着非常丰富的建立在模块化架构之上的XEP扩展，开发者可以将其直接插入到代码中，使用GCD构成，即使是在非常老旧的iPhone或12核Mac Pro上，它都能高效运转。
# 19. MGSplitViewController
为了方面开发，苹果iOS SDK提供了UISplitViewController用于iPad左右分栏界面显示，但对于开发者而言，UISplitViewController使用起来并不方便，在开发过程中，总会遇到许多问题。于是，天才的iOS开发者Matt Gemmell在GitHub上发起了一个名为MGSplitViewController的开源UISplitView实现，它比苹果官方UI组件所提供的分栏类功能更加丰富。
最初Matt编写MGSplitViewController只是为了方便自己使用，但最终Matt决定将其开源，供所有iOS开发者使用。如果想要了解更多关于MGSplitViewController的信息，可直接登陆Matt Gemmell博客查看，也可点击链接，查看视频演示（需翻&墙）。
# 20. CocoaLumberjack
CocoaLumberjack集快、简单、强大、灵活于一身，和log4j等其他非常受开发者欢迎的日志框架的概念类似，专为Objective-C设计，用于iOS和Mac，拥有多线程、GCD（如果可用）、无锁原子操作和Objective-C运行时的动态特性。
# 21. PSTCollectionView
PSTCollectionView是用于iOS 4.3及其以上版本中的UICollectionView的100%开源API兼容替代品。如果想要使用UICollectionView，但仍然需要支持老版本的iOS，那么这个项目你一定会非常喜欢。如果想要始终使用PSTCollectionView，使用PSTCollectionView作为类名称即可。
当然，如果你想在iOS 4.3/5.x上使用PSTCollectionView，但在iOS 6上使用UICollectionView，可以在所有UICollectionView类上添加PS，以获得对老版iOS的自动支持，这样就可以随心所欲如你所愿地使用PSUICollectionView。
# 22. CocoaHTTPServer
CocoaHTTPServer是一个用于Mac OS X或iOS应用的轻量级、可嵌入的HTTP服务器框架。有时候开发者需要在他们的App中内嵌一个HTTP服务器。无论是具有远程监控功能的服务器应用，还是使用HTTP进行后端通信的桌面应用，或者是提供无线访问文档功能的iOS应用，CocoaHTTPServer都能满足你的开发要求。
CocoaHTTPServer提供如下功能：
支持Bonjour广播内建；
支持IPv4、IPv6；
异步网络使用GCD和标准套接口；
密码保护支持；
SSL/TLS加密支持；
快速、高效；
伸缩性强；
大量注释代码；
非常容易扩展；
还支持WebDAV。
# 23. PushSharp
在应用开发中，给App推送通知是很常见的方式。PushSharp正是这样一个能够实现向iOS（iPhone/iPad APNS）、Android（C2DM和GCM）、Windows Phone、Windows 8、BlackBerry（PAP）和Amazon（ADM）等多种操作系统推送消息通知的开源C#服务器端库。
主要特性：
支持多平台消息推送；
API能够很流畅地为不同平台创建通知；
自适应信息频道（自动增加或减少消息链接）；
使用异步代码，基于Event为导向调用类库；
100%代码托管，兼容Mono框架；
# 24. HockeyKit
此前在GitHub上“最受欢迎的Android开源项目”系列文章（一）中，我们曾对HockeyKit进行了详细的介绍。HockeyKit是一个用于iOS和Android下App更新的开发包，无论是iOS还是Android应用开发，它都深受开发者喜爱。
# 25. JTRevealSidebarDemo
JTRevealSidebarDemo是一个能够实现类似于Facebook客户端和Path应用的视图切换效果的Objective-C库模板，其目的在于为开发者提供一个真正灵活的、可重用的解决方案。
JTRevealSidebarDemo可用于iOS 4.3和5.0应用开发，示例代码使用ARC构建，但该库本身则兼容ARC和非ARC。
# 26. PKRevealController
PKRevealController是对ZUUIRevealController的进化，ZUUIRevealController能够实现类似于Facebook客户端的视图横向切换效果。而PKRevealController则是一个iOS视图控制器的集合，能够让你实现多重控制器切换，非常灵活。
# 27. QuincyKit
QuincyKit是一个为iOS和Mac OS X提供的程序崩溃报告管理解决方案，提供客户端开发包以及基于PHP开发的服务器平台。
主要功能：
（自动）向开发者数据库发送崩溃报告；
由用户决定每次崩溃是否发送数据或总是发送；
用户可对是否选择提供E-mail地址等额外信息进行设置；
如果崩溃是已知的，向用户提供即时反馈，并在接下来的更新中进行修复。
详情请参考：QuincyKit官网
# 28. PSStackedView
PSStackedView能够将你的UIViewControllers堆叠起来，实现了类似于Twitter iPad版应用效果的界面，项目发起人Peter Steinberger表示，PSStackedView的灵感正是来自于Twitter iPad的UI。
为了开发非常受欢迎的奥地利TV应用“3MobileTV”的iPad版，Peter创作了PSStackedView，开发者可以直接登陆App Store下载该应用。PSStackedView可运行在iPad和iPhone上，但相对而言，更适合用在iPad上。
# 29. Chameleon
作为一名iOS开发者，你应该对苹果官方用于开发iPhone、iPod和iPad应用的UIKit框架很熟悉。Chameleon是一个完全独立的可以直接替代UIKit在Mac OS X上运行的框架，和UIKit唯一的共性就是公共类和方法名称。在大多数情况下，开发者不需要改动任何代码，即可直接在Mac上运行所开发的iOS应用。
Chameleon的源代码是基于苹果的文档而来，并没有使用到任何私有API或其他未经Mac App Store批准的技术。如果想要了解更多关于Chameleon的信息，可直接登陆ChameleonProject网站查看。
# 30. DCIntrospect
Introspect是一个很小的iOS工具集，用于帮助开发者对使用UIKit构建的用户界面进行调试。特别适用于动态创建UI布局、改变运行时，还能通过寻找透明视图优化应用性能，并避免不必要的重绘视图。
# 31. PSCollectionView
PSCollectionView是一个能够实现Pinterest风格的瀑布流视图显示效果的iOS滚动组件，可用于类似的UITableView，支持显示图片和文字。
项目发起人Peter Shih在自己的iPhone/iPad应用Lunchbox中使用了PSCollectionView源代码，开发者可以在App Store中免费下载该应用。
# 32. SDURLCache
NSURLCache及其子类提供了很多高级接口用于处理缓存，但如果你想在自己的iPhone应用中通过HTTP请求的方式将缓存存储到磁盘中，推荐使用SDURLCache。Peter Steinberger在GitHub上托管的这个SDURLCache项目是Olivier Poitrey的SDURLCache的一个分支，支持iOS 5以下版本系统。
# 33. FastPdfKit
该开源库包含着FastPdfKit iOS库的一些示例项目。FastPdfKit是一个能够让你在iOS应用中显示PDF文档的开发类库，其最大的特点就是加载PDF文档速度特别快。
详情请参考：FastPdfKit官网
# 34. KKGridView
KKGridView是一个高性能的iOS表格组件，但自WWDC 2012大会上苹果推出了相关的解决方案之后，KKGridView也就逐渐停止使用了。每逢iOS系统更新，iOS开源项目势必也会发生新一轮的改变，至于在颠覆了苹果以往设计观的iOS 7发布之后，iOS开源项目又会发生哪些改变？还要看天才的开发者们！
# 35. MGBox2
MGBox2能够实现非常酷的列表交互效果，使用最少代码，即可快速创建各种布局效果的iOS表格、网格，支持新增和删除网格，动态添加或删除列表，在表格中加入多行文字、图片等，支持手势识别和自定义事件。
# 36下拉刷新列表组件SVPullToRefresh
SVPullToRefresh 允许你通过一行代码把下拉刷新添加至UIScrollView子类别，不需要依赖委托或者子类化UITableViewController。另外，该项目支持简单自定义文本、箭头的外观。
这些UIScrollView类别让下拉刷新和UIScrollView的无限滚动变得非常简单，
SVPullToRefresh使用Objective-C runtime为UIScrollView添加以下3种方法：
- (void)addPullToRefreshWithActionHandler:(void (^)(void))actionHandler;
- (void)addPullToRefreshWithActionHandler:(void (^)(void))actionHandler position:(SVPullToRefreshPosition)position;
- (void)addInfiniteScrollingWithActionHandler:(void (^)(void))actionHandler;
[https://github.com/samvermette/SVPullToRefresh](https://github.com/samvermette/SVPullToRefresh)
# 37网络工具包MKNetWorkKit
MKNetworkKit 是一个完全基于 ARC 的网络工具包。支持把上一次请求进行缓存。MKNetWorkKit吸取了ASIHttpRequest与AFNetWorking的优点，并加入了自己的功能。
最新的代码并未在完全在Mac通过测试，iOS上的测试已经完成。包括Digg在内的多款app使用了该项目。关于更详细的信息和教程，你可以到MKNetWorkKit相关博客上查看。[http://blog.mugunthkumar.com/products/ios-framework-introducing-mknetworkkit/](http://blog.mugunthkumar.com/products/ios-framework-introducing-mknetworkkit/)
网络工具包MKNetWorkKit
MKNetworkKit 是一个完全基于 ARC 的网络工具包。支持把上一次请求进行缓存。MKNetWorkKit吸取了ASIHttpRequest与AFNetWorking的优点，并加入了自己的功能。
最新的代码并未在完全在Mac通过测试，iOS上的测试已经完成。包括Digg在内的多款app使用了该项目。关于更详细的信息和教程，你可以到MKNetWorkKit相关博客上查看。[http://blog.mugunthkumar.com/products/ios-framework-introducing-mknetworkkit/](http://blog.mugunthkumar.com/products/ios-framework-introducing-mknetworkkit/)
UILabel改进版TTTAttributedLabel
TTTAttributedLabel是一个文字视图开源组件，是UILabel的替代元件，可以以简单的方式展现渲染的属性字符串。另外，还支持链接植入，不管是手动还是使用UIDataDetectorTypes自动把电话号码、事件、地址以及其他信息变成链接。
支持NSAttributedString，兼容iOS 4.3以上系统，自动数据检测，手动链接植入等。
还包括先进的段落样式：
verticalAlignment
textInsets
firstLineIndent
leading
lineHeightMultiple
shadowRadius
highlightedShadowRadius / highlightedShadowOffset / highlightedShadowColor
truncationTokenString
网络工具包MKNetWorkKit
MKNetworkKit 是一个完全基于 ARC 的网络工具包。支持把上一次请求进行缓存。MKNetWorkKit吸取了ASIHttpRequest与AFNetWorking的优点，并加入了自己的功能。
最新的代码并未在完全在Mac通过测试，iOS上的测试已经完成。包括Digg在内的多款app使用了该项目。关于更详细的信息和教程，你可以到MKNetWorkKit相关博客上查看。[http://blog.mugunthkumar.com/products/ios-framework-introducing-mknetworkkit/](http://blog.mugunthkumar.com/products/ios-framework-introducing-mknetworkkit/)
转自：[http://www.verydemo.com/demo_c134_i49279.html](http://www.verydemo.com/demo_c134_i49279.html)
