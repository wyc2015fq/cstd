# 十分钟接入iOS 12新特性——Siri Shortcuts - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年10月11日 09:15:40[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：25标签：[Siri Shortcuts](https://so.csdn.net/so/search/s.do?q=Siri Shortcuts&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
### 前言
[Xcode 10](https://developer.apple.com/xcode/)已经正式发布，开发者可以接入Siri Shortcuts的iOS 12新特性。
WWDC2018的[Introduction to Siri Shortcuts Session](https://developer.apple.com/videos/play/wwdc2018/211/)介绍了Siri Shortcuts的使用和Swift接入，但没有Objective-C版本的demo。
于是新开此文，介绍如何快速用OC接入iOS 12新特性Siri Shortcuts。
### 正文
快速接入Siri Shortcuts
Siri Shortcuts的API有两大类：NSUserActivity和Intents。
![](https://upload-images.jianshu.io/upload_images/1049769-094c4af1be7b97b0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/746/format/webp)
使用NSUserActivity接入Siri Shortcuts简单高效，只需十分钟。
![](https://upload-images.jianshu.io/upload_images/1049769-96a5897749374c79.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
1、plist添加activity type（Define Shortcut）
注意是array的类型，每个元素是一个字符串，这里定义的是`loying.LearnSiriShortcut.type`；
```
<key>NSUserActivityTypes</key> <array>
<string>loying.LearnSiriShortcut.type</string> </array>
```
2、配置NSUserActivity并通知Siri（Donate Shortcut）
`-initWithActivityType`接口的type参数要和plist一致；
eligibleForPrediction是iOS 12新增API，如需支持老版本要添加 `@available(iOS 12.0, *)`的判断；
userActivity的userInfo属性是透传的参数值；
最后把userActivity赋值给viewController的userActivity属性；
```
- (void)reportSiriShortcuts {
    NSUserActivity *userActivity = [[NSUserActivity alloc] initWithActivityType:@"loying.LearnSiriShortcut.type"];
    userActivity.eligibleForSearch = YES;
    if (@available(iOS 12.0, *)) // 如果要支持老版本，加上判断
    {
        userActivity.eligibleForPrediction = YES;
    }
    userActivity.title = @"测试标题";
    userActivity.userInfo = @{@"testKey" : @"testValue"};
    self.userActivity = userActivity;
}
```
3、在AppDelegate中处理Siri打开APP请求 （Handle Shortcut）
通过userActivity的type值判断是否为Siri Shortcuts呼起，做相应的逻辑处理。
```
- (BOOL)application:(UIApplication *)application continueUserActivity:(NSUserActivity *)userActivity restorationHandler:(void (^)(NSArray<id<UIUserActivityRestoring>> * _Nullable))restorationHandler {
    NSLog(@"continueUserActivity");
    if ([userActivity.activityType isEqualToString:@"loying.LearnSiriShortcut.type"]) {
        // 做自己的业务逻辑
    }
    return YES;
}
```
> 
至此接入Siri Shortcuts完毕，仅需三步！
下面根据WWDC的视频详细介绍Siri Shortcuts的新特性。
Siri Shortcuts的特性解析
Siri Shortcuts提供的能力有两个：
- 1、Suggestion（建议）
	用户重复、固定的行为，通过不断的告知siri，siri会在确定用户习惯后，会发本地推送提醒用户；
	比如说某个用户习惯每天晚上11点的时候用微信读书进行阅读，如果微信读书把用户每次的行为发送给Siri，那么Siri在学习到用户的习惯之后，会在适当时候通过本地推送提示用户，推送的截图如下：
![](https://upload-images.jianshu.io/upload_images/1049769-b28495ddb3245de6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
- 2、Shortcuts（短语快捷方式）
	把APP的常用操作行为与短语挂钩，通过设置好的短语可以快捷使用APP的功能；
	WWDC以点外卖为例，通过设置好的短语`coffee time`，可以呼起APP，也可以直接在Siri界面进行下单操作；
	
![](https://upload-images.jianshu.io/upload_images/1049769-0a1cd6130474397a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/748/format/webp)
coffee time
以上的功能都需要特定的使用场景：
1、App的关键功能；
2、用户有重复性的行为；
3、随时都可以执行；
不满足以上三个要求的功能，苹果不推荐使用Siri Shortcuts。
Siri Shortcuts的使用
iOS 12的开发者选项增加了`Display Recent Shortcuts`，打开之后可以显示最近发送给Siri的Shortcuts；
![](https://upload-images.jianshu.io/upload_images/1049769-8fc1253a45da2459.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/650/format/webp)
如下，向Siri发送了名为“测试标题”的Shortcuts；
![](https://upload-images.jianshu.io/upload_images/1049769-62801c31a85d5ad2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/634/format/webp)
在添加Siri Shortcuts之后，本地搜索时能找到对应的Shortcuts，点击之后会执行定义好的Shortcuts操作；
![](https://upload-images.jianshu.io/upload_images/1049769-adfec028fbbb07c9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/630/format/webp)
在APP添加Shortcuts之后，可以在系统设置中录制Siri Shortcuts的快捷短语：
![](https://upload-images.jianshu.io/upload_images/1049769-692557d3fad90880.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/376/format/webp)
录制短语之后，可以通过Siri快速呼起APP对应的功能：
![](https://upload-images.jianshu.io/upload_images/1049769-5f3d304b94118345.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/376/format/webp)
注意事项
1、Siri语言版本可以设置成中文/英文，需要与短语的语言类型一致；
2、检查项目中是否已存在接口`- (BOOL)application:(UIApplication *)application continueUserActivity:(NSUserActivity *)userActivity restorationHandler:(void (^)(NSArray<id<UIUserActivityRestoring>> * _Nullable))restorationHandler`；（实际开发过程，因为UniversalLink接入，在Category已经实现这个delegate，导致在AppDelegate.m实现的接口没有回调）
### 附录
[苹果开发者文档](https://developer.apple.com/documentation/sirikit/donating_shortcuts)
[苹果官方的demo](https://developer.apple.com/documentation/sirikit/soup_chef_accelerating_app_interactions_with_shortcuts?language=objc)
[本文的demo](https://github.com/loyinglin/LearnSiriShortcuts.git)
作者：落影loyinglin
链接：https://www.jianshu.com/p/edda18023a7b
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
