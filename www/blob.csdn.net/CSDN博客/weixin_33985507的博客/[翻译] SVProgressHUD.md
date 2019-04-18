# [翻译] SVProgressHUD - weixin_33985507的博客 - CSDN博客
2014年08月02日 16:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
SVProgressHUD
![](https://images0.cnblogs.com/i/607542/201408/012126368213154.gif)
https://github.com/TransitApp/SVProgressHUD
SVProgressHUD is a clean and easy-to-use HUD meant to display the progress of an ongoing task.
SVProgressHUD 是一个清新易用的指示器，用来指示正在执行的任务。
![](https://images0.cnblogs.com/i/607542/201408/012126586181194.gif)
Installation
[](https://github.com/TransitApp/SVProgressHUD#from-cocoapods)From CocoaPods CocoaPods安装
I'm not a big fan of CocoaPods, so tend to not keep it updated. If you really want to use SVProgressHUD with CocoaPods, I suggest you use `pod 'SVProgressHUD', :head` to pull from the `master` branch directly. I'm usually careful about what I push there and is the version I use myself in all my projects.
[](https://github.com/TransitApp/SVProgressHUD#manually)Manually 手动安装
- Drag the `SVProgressHUD/SVProgressHUD` folder into your project.
- Add the **QuartzCore** framework to your project.
[](https://github.com/TransitApp/SVProgressHUD#usage)Usage 使用
(see sample Xcode project in `/Demo`)
SVProgressHUD is created as a singleton (i.e. it doesn't need to be explicitly allocated and instantiated; you directly call `[SVProgressHUD method]`).
SVProgressHUD 是单例模式（也就是说你只需要调用[SVProgressHUD method]即可）
**Use SVProgressHUD wisely! Only use it if you absolutely need to perform a task before taking the user forward. Bad use case examples: pull to refresh, infinite scrolling, sending message.**
请明智的使用SVProgressHUD！仅仅当你确保绝对要执行一项任务了。以下是不当的使用情形：下拉刷新，发送消息，无限滚动
Using SVProgressHUD in your app will usually look as simple as this (using Grand Central Dispatch):
请以以下的方式使用SVProgressHUD
[SVProgressHUD show];
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 
[](https://github.com/TransitApp/SVProgressHUD#showing-the-hud)Showing the HUD 显示指示器
You can show the status of indeterminate tasks using one of the following:
你可以用以下的方式来指示你在做什么：
+ (void)show;
+ (void)showWithMaskType:(SVProgressHUDMaskType)maskType;
+ (void)showWithStatus:(NSString*)string;
+ (void)showWithStatus:(NSString*)string maskType:(SVProgressHUDMaskType)maskType;
If you'd like the HUD to reflect the progress of a task, use one of these:
如果你想显示任务的进度情况，用下面的方法：
+ (void)showProgress:(CGFloat)progress;
+ (void)showProgress:(CGFloat)progress status:(NSString*)status;
+ (void)showProgress:(CGFloat)progress status:(NSString*)status maskType:(SVProgressHUDMaskType)maskType;
[](https://github.com/TransitApp/SVProgressHUD#dismissing-the-hud)Dismissing the HUD 隐藏指示器
It can be dismissed right away using:
你可以用以下方法隐藏指示器：
+ (void)dismiss;
If you'd like to stack HUDs, you can balance out every show call using:
如果你想用栈的方式存储按照顺序来显示，你可以依次调用以下方法：
+ (void)popActivity;
The HUD will get dismissed once the `popActivity` calls will match the number of show calls.
这个指示器会依照顺序依次消失
Or show a confirmation glyph before before getting dismissed 1 second later using:
或者按照标志位延时1s钟消失：
+ (void)showSuccessWithStatus:(NSString*)string;
+ (void)showErrorWithStatus:(NSString *)string;
+ (void)showImage:(UIImage*)image status:(NSString*)string; // use 28x28 pngs
[](https://github.com/TransitApp/SVProgressHUD#customization)Customization
SVProgressHUD can be customized via the following methods:
SVProgressHUD 可以用以下方式定制：
+ (void)setBackgroundColor:(UIColor*)color; // default is [UIColor whiteColor]
+ (void)setForegroundColor:(UIColor*)color; // default is [UIColor blackColor]
+ (void)setRingThickness:(CGFloat)width; // default is 4 pt
+ (void)setFont:(UIFont*)font; // default is [UIFont preferredFontForTextStyle:UIFontTextStyleSubheadline]
+ (void)setSuccessImage:(UIImage*)image; // default is bundled success image from Glyphish
+ (void)setErrorImage:(UIImage*)image; // default is bundled error image from Glyphish
[](https://github.com/TransitApp/SVProgressHUD#notifications)Notifications
`SVProgressHUD` posts four notifications via `NSNotificationCenter` in response to being shown/dismissed:
SVProgressHUD 通过通知中心发送4种通知来显示/隐藏：
- `SVProgressHUDWillAppearNotification` when the show animation starts
- `SVProgressHUDDidAppearNotification` when the show animation completes
- `SVProgressHUDWillDisappearNotification` when the dismiss animation starts
- `SVProgressHUDDidDisappearNotification` when the dismiss animation completes
Each notification passes a `userInfo` dictionary holding the HUD's status string (if any), retrievable via`SVProgressHUDStatusUserInfoKey`.
每一个通知都传递一个userInfo字典来装载HUD状态值，其值可以通过via`SVProgressHUDStatusUserInfoKey来获取。`
`SVProgressHUD` also posts `SVProgressHUDDidReceiveTouchEventNotification` when users touch on the screen. For this notification `userInfo` is not passed but the object parameter contains the `UIEvent` that related to the touch.
SVProgressHUD当用户触摸屏幕的时候，也能发送`SVProgressHUDDidReceiveTouchEventNotification。这种通知方式呢，并没有把userInfo当做参数来传递，取而代之的是object对象，包含了这个触摸的事件。`
