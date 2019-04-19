# iOS App内评分 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年01月17日 11:41:27[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：132
App内评分
苹果在iOS 10.3中增加了App评分的新API，用户可以直接在App内评分，而不用跳转到App Store。
使用非常简单，只有一行代码，但也说明了它没有任何可定制性。
此API使用前需要 `#import <StoreKit/StoreKit.h>`:
```
if (@available(iOS 10.3, *)) {
            [SKStoreReviewController requestReview];
    }
```
显示效果如下图：
![](https://upload-images.jianshu.io/upload_images/2710049-dfd7aa2508d748e9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/361/format/webp)
App内评分
点击星星即可评分，评分之后会显示提交按钮：
![](https://upload-images.jianshu.io/upload_images/2710049-92050b29bf38947a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/363/format/webp)
准备提交
“提交”按钮仅在生产环境（上传App Store的包）才是有效的，其余均为上图中的灰色禁用状态。
**注意：**
苹果官方说不能骚扰用户，因此使用此API有非常严格的限制。
按苹果规定，**每台设备、每个App、每年只能弹出3次**该提示窗口，如果超过这个次数，则什么都不会显示。(╯‵□′)╯︵┻━┻
虽然规定严格，但它非常方便，用户随手一点就可以给App评分了。再想想以前的方式：先离开App跳转到App Store，等待网络加载，在那么长的页面中找到评分入口。。。破坏用户使用App的整体性、影响用户心情、浪费用户时间、增加用户学习成本。。。可以说毫无优点。即便我自己作为一个开发者，都很少跳转到应用商店给App评分，但如果我发现哪个App使用了这个新API，我一般至少给个4星好评！举手之劳，何乐而不为呢？(～￣▽￣)～
因此，我们必须利用好每年的这3次机会。苹果官方的意思是，你应该在用户**对你的App有了整体的了解，且对你的App感到满意**的时候弹出窗口，请用户给予评分。
这么玄学的状态怎么把握呢？举个栗子，你的App是一款游戏，用户正在打关底boss，你肯定不想在这个时候弹出窗口让用户评分吧（保证他给你一星）？等他成功通关的时候，这时他正心满意足，肯定不介意赏你几颗星星。
新API虽然方便又好用，但是次数实在太少，而且弹出了用户可能也会点击“取消”，关键这也会使次数-1！而且，这个功能是有可能被用户关闭的，在系统设置->iTunes Store与App Store中可以找到“App内评分及评论”的开关，如果用户关闭了这个开关，则上面的窗口也不会弹出。
所以，我们还是需要有个跳转到App Store的方法做保障，在次数不足时（App中无法获取到“App内评分及评论”开关的状态）使用。
Deep Link
大家应该都知道跳转到App Store的方法：
```
NSString *appURL = @"https://itunes.apple.com/cn/app/idAppID";
[[UIApplication sharedApplication] openURL:[NSURL URLWithString:appURL]];
```
这样可以跳转到自己的APP在应用商店的地址，但是有可能用户找不到在哪评分？（要相信，真的有这样的用户存在）
因此，下面介绍一种可以直接跳转到评分页面的方法。这个方法其实在上述新API的注释中苹果已经提到，他们称为“Deep Link”。使用方式与上面的方法一致，只是在URL后面拼接一个query params：`action=write-review`。
下面是完整代码：
```
NSString *appURL = @"https://itunes.apple.com/cn/app/idAppID?action=write-review";
[[UIApplication sharedApplication] openURL:[NSURL URLWithString:appURL]];
```
它在App Store中的显示效果如下：
![](https://upload-images.jianshu.io/upload_images/2710049-c0095c15399bd7ab.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Deep Link
怎么找到自己的AppID？
- 如果你的App已经上线，那么在App Store中找到你的App，点击分享->拷贝链接->在别处粘贴，就可以在链接末尾找到类似于`id123456789`的字符串，这串数字就是你的AppID了。
- 不管你的App是否已经上线，在[App Store Connect](https://appstoreconnect.apple.com)（以前叫iTunes Connect）中找到你的App，它的位置在下图中的两处红框中可以找到：
	
![](https://upload-images.jianshu.io/upload_images/2710049-5ab308ab36b38b70.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
QQ20180914-142518.png
作者：乌鸢
链接：https://www.jianshu.com/p/4f9fe2e1879e
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
