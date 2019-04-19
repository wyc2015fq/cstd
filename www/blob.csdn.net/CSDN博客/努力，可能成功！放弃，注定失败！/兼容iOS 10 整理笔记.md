# 兼容iOS 10 整理笔记 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月26日 22:36:50[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1160
升级完 Xcode8 和 iOS10后, 在开发当中遇到的问题, 现整理自己的问题解决记录.
## 一.Xcode8开发iOS10推送打开后问题
我的好奇心打开推送通知后,目前就发现了两个问题,说大不大说小不小,但是真心很恼人!真心建议,如若项目中没有推送相关,则不要打开这个!
![](http://upload-images.jianshu.io/upload_images/576025-05b075eca116ced7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1.1 工程增加了Notification Content和Notification Service两个Extension，Signing默认是自动模式.
![](http://upload-images.jianshu.io/upload_images/576025-42a25b023ac5f0ad.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
如果想使用原始的手动配置，不仅要把Automatically manage signing去掉，还要在Key Chains钥匙串里把新生成的证书删掉，然后在Web端生成授权文件。使用Automatically manage signing最好把每个Target都使用相同的方式.
重新 Edit 配置文件 有黄变绿成为Active时,重新下载并打开.
![](http://upload-images.jianshu.io/upload_images/576025-3a7e27847e8213b3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/576025-d92d9464c863a836.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这个时候真机测试时如果出现如下图这个问题:
![](http://upload-images.jianshu.io/upload_images/576025-a33ccbb35718aa9b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这个时候请先检查设备上是不是之前测试装过该软件,如果有删掉重新运行即可!
`如果想了解 Xcode8开发iOS10推送通知过程 请移步,作者: zhao0`
[http://www.jianshu.com/p/133b535a4e90](http://www.jianshu.com/p/133b535a4e90)
1.2 打开推送通知后,xib 文件里布局约束丢失及视图变为空白;
这个时候我们先Update Frames, 如果还是不行再需要重新添加整体 view布局, 先从外到内添加约束,因为后面的一般不再爆红而是报黄,我们可以用提示一键解决的.
![](http://upload-images.jianshu.io/upload_images/576025-e8ff59e018cb3b07.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 二. Xcode8打印许多无关紧要的内容
刚升级到 Xcode8,发现控制台打印了好多东西.例如:**subsystem: com.apple.network, category: , enable_level: 0, persist_level: 0, default_ttl: 0, info_ttl: 0, debug_ttl: 0, generate_symptoms: 0, enable_oversize: 0, privacy_setting: 2, enable_private_data: 0**
解决方案:
![](http://upload-images.jianshu.io/upload_images/576025-f8393e8e9aa16ae2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/576025-a1c8800bc5d7217c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
OS_ACTIVITY_MODE disable .png
当然目前Xcode8比较事多,你懂得,如果有时候失灵了,重新勾选对号✅即可.
## 三. Xcode8不能真机打印log问题
```cpp
```cpp
#ifdef DEBUG //测试 处于开发阶段
// 真机打印
#define ZLString [NSString stringWithFormat:@"%s", __FILE__].lastPathComponent
#define ZLLog(...) printf("%s 第%d行: %s\n\n", [ZLString UTF8String] ,__LINE__, [[NSString stringWithFormat:__VA_ARGS__] UTF8String]);
#else //发布 处于发布阶段
#define ZLLog(...)
#endif
```
```
## 四. 去除 Nav 上黑线
使用 iOS10之后, 之前使用的方法去除的 navi 的黑线又回来了.那么怎么进去处理呢.
```
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    // 移除nav上黑线 (这个时机viewDidAppear)
    [self removeNavBlackLine:self.navigationBar];
}
- (void)removeNavBlackLine:(UIView *)view {
    for (UIView *subView in view.subviews) {
        if (subView.subviews.count) {
            [self removeNavBlackLine:subView];
        } else {
            if (subView.frame.size.height <= 1) {
                [subView removeFromSuperview];
            }
        }
    }
}
```
如果你的 nav 不需要背景颜色 当然可以有更简单的方法.
```
/**
 * 当第一次使用这个类的时候会调用一次
 */
+ (void)initialize {
  // 移除nav上黑线
  [UINavigationBar appearance].clipsToBounds = YES;
}
```
![](http://upload-images.jianshu.io/upload_images/576025-bbe6d481799f2aba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
nav.png
## 五. 去除 TabBar 上黑线且换背景
```
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    // 移除tabBar上黑线(时机很重要!!!)
    [self removeTabBarBlackLine:self.tabBar];
}
- (void)removeTabBarBlackLine:(UIView *)view {
    for (UIView *subView in view.subviews) {
        if (subView.subviews.count) {
            [self removeTabBarBlackLine:subView];
        } else {
            if (subView.frame.size.height <= 1) {
                [subView removeFromSuperview];
            }
        }
    }
}
```
```
// 因为这个tabBar 是透明底,如果改变背景颜色发现根本没变色,但是可以通过设置背景图片去改变背景色.
+ (void)initialize {
    // tabbar相关背景设置
    UITabBar *tabbar = [UITabBar appearance];
    tabbar.backgroundImage = [UIImage imageNamed:@"tabbar"];
}
```
![](http://upload-images.jianshu.io/upload_images/576025-bbf37adab2e14d88.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
tabBar.png
`深层定制 tabBar. 请移步: 作者:明月钓无痕`
[http://www.jianshu.com/p/dd3475fb3960](http://www.jianshu.com/p/dd3475fb3960)
## 六. 隐私权限的问题
iOS10 对于隐私权限更加严格了.在调用相机,相册,定位,蓝牙等等时,都需要在 info.plist 中进行配置添加.
info.plist NSContactsUsageDescription 
![](http://upload-images.jianshu.io/upload_images/576025-026679705a00ace4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
P 要大写!!!.png
## 七. ATS的问题
- 在iOS 9的时候, 默认HTTP的网络是被禁止的当然我们也可以把 NSAllowsArbitraryLoads 设置为YES禁用ATS。
- 不过iOS 10从2017年1月1日起, 所有新提交的 App苹果 默认不允许通过这个方法来绕过ATS的限制,也就是说强制我们用HTTPS，如果不这样的话提交App可能会被拒绝。
- ***但是我们可以选择使用NSExceptionDomains设置白名单的方式来针对特定的域名开放HTTP通过审核。***
- 在 iOS 10 中 info.plist 文件新加入NSAllowsArbitraryLoadsInWebContent键, 允许任意 web 页面加载,同事苹果会用 ATS 来保护你的 App.
- 安全传输不再支持 SSLv3, 建议尽快停用 SHA1 和 3DES 算法.
## 八. iOS 10 判断系统版本
建议用第三个*[[UIDevice currentDevice] systemVersion]*这个方法！
```
// 值为 1
[[[[UIDevice currentDevice] systemVersion] substringToIndex:1] integerValue]
// 值为10.000000
[[UIDevice currentDevice] systemVersion].floatValue,
// 值为10.0
[[UIDevice currentDevice] systemVersion]
```
## 九. iOS10随着手机系统字体而改变
当我们手机系统字体改变了之后，字体会随着手机系统而改变, 我们App的label可能导致部分文字隐藏显示不全问题, 这需要我们写很多代码来进一步处理才能实现，但是iOS 10 提供了这样的属性`adjustsFontForContentSizeCategory`来设置。
解决方案: 为 UILabel写一个扩展类
```
@implementation UILabel (Extend)
- (void)lableAdaptIOS10{
    CGFloat iOS10 = [[UIDevice currentDevice] systemVersion] >= 10.0;
    if (iOS10) {
//        self    UIFont 的preferredFontForTextStyle: 意思是指定一个样式，并让字体大小符合用户设定的字体大小。
        [self sizeToFit];
//      self.font =[UIFont preferredFontForTextStyle: UIFontTextStyleHeadline]; 
        //是否更新字体的变化
//        self.adjustsFontForContentSizeCategory = YES;
    }
```
## 十. Xcode8提交后不显示构建版本
后面的描述信息切记要写!!!!否则很有可能出现构建版本失败的情况.构建版本旁边的➕不会出现.当然历史版本里也看不到.几乎是没有提示.当再提交下一个版本时,发现1.1.1是失败的(这里拿1.1.1版本介绍).
![](http://upload-images.jianshu.io/upload_images/576025-6dcef40d3fd6e314.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
`想了解更多相关兼容iOS 10 资料整理笔记, 请移步,作者: 判若两人丶`[http://www.jianshu.com/p/0cc7aad638d9](http://www.jianshu.com/p/0cc7aad638d9)
本人感觉这篇相当不错,再次感谢~~
后续接着整理自己遇到的问题, 未完待续~~
文／smile丽语（简书作者）
原文链接：http://www.jianshu.com/p/7c3719448d26
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
