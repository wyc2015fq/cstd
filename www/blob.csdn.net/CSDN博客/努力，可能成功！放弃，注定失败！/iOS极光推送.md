# iOS极光推送 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年01月21日 09:24:20[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5783
**        昨天花了一下午的时间研究了下极光推送，也前也是没做过，不知道从何下手！才开始的时候一看官方的SDK感觉好难，不过经过一系列的捣鼓之后，手机收到了推送信息，感觉其实并没有那么难！**
## 1.配置开发证书（得有开发者账号，个人，企业的都可以）
#### 开发环境测试
在对 JPush iOS 开发环境进行测试前，请确保 3 个统一：
- App 是开发环境打包（开发证书 Development）
- 上传了开发证书并验证通过
- Portal 上的应用设置为开发环境
#### 发布环境测试
在对 JPush iOS 生产环境进行测试前，请确保 3 个统一：
- App 是 ad-hoc 打包或者App Store 版本（发布证书 Production）
- 上传了发布证书并验证通过
- Portal 上的应用设置为生产环境
- 
### iOS 证书 设置指南
#### 创建应用程序ID
- 登陆 [iOS Dev Center](https://developer.apple.com/devcenter/ios/index.action) 选择进入iOS Provisioning Portal。
![jpush_ios](http://docs.jpush.io/client/image/login.png)
- 在 [iOS
 Provisioning Portal](https://daw.apple.com/cgi-bin/WebObjects/DSAuthWeb.woa/wa/login?&appIdKey=891bd3417a7776362562d2197f89480a8547b108fd934911bcbea0110d07f757&path=%2F%2Faccount%2Findex.action)中，点击App IDs进入App ID列表。
![](http://docs.jpush.io/client/image/appid.png)
- 创建 App ID，如果 ID 已经存在可以直接跳过此步骤
![jpush_ios](http://docs.jpush.io/client/image/appid2.png)
- 为 App 开启 Push Notification 功能。如果是已经创建的 App ID 也可以通过设置开启 Push Notification 功能。
![jpush_ios](http://docs.jpush.io/client/image/appservice.png)
根据实际情况完善 App ID 信息并提交,注意此处需要指定具体的 Bundle ID 不要使用通配符。
![jpush_ios](http://docs.jpush.io/client/image/appid3.png)
#### 配置和下载证书
- 如果你之前没有创建过 Push 证书或者是要重新创建一个新的，请在证书列表下面新建。
![](http://docs.jpush.io/client/image/cer0.png)
- 新建证书需要注意选择证书种类（开发证书用于开发和调试使用，生产证书用于 App Store 发布）
![](http://docs.jpush.io/client/image/cer1.png)
- 点击 Continue 后选择证书对应的应用ID，然后继续会出现“About Creating a Certificate Signing Request (CSR)”。
![](http://docs.jpush.io/client/image/cer2.png)
- 根据它的说明创建打开KeychainAccess 创建 Certificate Signing Request。
![](http://docs.jpush.io/client/image/Screenshot_13-4-1_5_22.png)
- 填写“User Email Address”和“Common Name” 后选择 Saved to disk 进行保存 。
![](http://docs.jpush.io/client/image/Snip20140122_7.png)
- 继续返回Apple developer 网站点击 Continue ，上传刚刚生成的 .certSigningRequest 文件生成 APNs Push Certificate。
- 下载并双击打开证书，证书打开时会启动“钥匙串访问”工具。
- 在“钥匙串访问”中你的证书会显示在“我的证书”中，注意选择“My Certificates” 和"login"
![jpush_ios](http://docs.jpush.io/client/image/keychain_cert.png)
#### 导出 .p12 证书文件
```
注意要选“login”和“My Certificates” 导出证书时要选中证书文件，不要展开private key。
```
![jpush_ios](http://docs.jpush.io/client/image/export_p12.png)
- 将文件保存为Personal Information Exchange (.p12)格式。
![](http://docs.jpush.io/client/image/export_filename.png)
- 将文件保存为Personal Information Exchange (.p12)格式。
#### 上传证书
在 [JPush 管理 Portal 上](https://www.jpush.cn/apps/)，针对某应用程序，上传上面步骤得到 .p12 证书文件。这是 iOS SDK 能够接收到 JPush 推送消息的必要步骤。
#### Provisioning Profile的创建
- 创建Provisioning Profile的前提，已在Apple Developer网站创建待发布应用所使用的Bundle ID的App ID，且为该App ID创建了APNs证书，如下图:
![jpush_ios](http://docs.jpush.io/client/image/appidcer.png)
- 
创建App ID、APN证书和p12证书的导出的具体步骤请看 :[iOS 证书 设置指南](http://docs.jpush.io/client/ios_tutorials/#ios_1)
- 
在[苹果开发者账号的Provisioning Profile](https://developer.apple.com/account/ios/profile/profileList.action)页面点击下图按钮，创建Provisioning Profile
![jpush_ios](http://docs.jpush.io/client/image/provision_profile.png)
- 选择此Provisioning Profile的环境后点击[Continue]：
![jpush_ios](http://docs.jpush.io/client/image/create_pp_type.png)
- 选择要创建Provisioning Profile的App ID后点击[Continue]：
![jpush_ios](http://docs.jpush.io/client/image/pp_appid_new.png)
- 选择所属的开发者证书，（这里创建了多个开发者证书，建议只创建一个，方便管理）为了方便，选择了[Select All]，再点击[Continue]进入下一步：
![jpush_ios](http://docs.jpush.io/client/image/select_cer.png)
- 为该Provisioning Profile选择将要安装的设备（一般选择[Select All]），点击[Continue]:
![jpush_ios](http://docs.jpush.io/client/image/select_devices.png)
- 给该Provisioning Profile填写Profile Name，点击[generate]完成创建。
![jpush_ios](http://docs.jpush.io/client/image/pp_name.png)
- 填写完Profile Name后点击[generate]完成创建，之后点击[DownLoad]下载Provisioning Profile
![jpush_ios](http://docs.jpush.io/client/image/download_pp.png)
- 双击下载下来的Provisioning Profile，添加到xcode。

## 2. Xcode的证书配置教程
参照[iOS SDK 集成指南](http://docs.jpush.io/guideline/ios_guide/)集成JPush SDK 和上传了推送用到的p12证书后在编译运行前需要先配置一下证书，步骤如下：
- 打开xxx-info.plist的Bundle identifier项把上传到JPush 控制台的bundle id填写进去：
![jpush_ios](http://docs.jpush.io/client/image/xcode_bundle.png)
- 点击项目，选择目标TARGETS后进入Build Setting 界面，搜索“Code signing”，按照下图配置
![jpush_ios](http://docs.jpush.io/client/image/xcode_buildsettings_cs.png)
#### 客户端设置
##### 开启Remote notifications
需要在Xcode 中修改应用的 Capabilities 开启Remote notifications，请参考下图：
![](http://docs.jpush.io/client/image/Snip20131119_1.png)
## 3.SDK集成步骤
### 集成压缩包内容
包名为JPush-iOS-SDK-{版本号}
- lib文件夹：包含头文件 JPUSHService.h，静态库文件jpush-ios-x.x.x.a ，支持的iOS版本为 5.0 及以上版本。（请注意：模拟器不支持APNs）
- pdf文件：集成指南
- demo文件夹：示例
### 开发环境
- 使用Xcode 6及以上版本可以使用新版Push SDK，XCode 5环境下需要运行旧版本SDK(1.7.4)
### 1、在JPush Portal上创建应用
- 在 JPush的管理Portal 上创建应用并上传APNs证书。如果对APNs证书不太了解 请参考： [iOS 证书设置指南](http://docs.jpush.io/client/ios_tutorials/#ios_1)
![jpush_ios](http://docs.jpush.io/guideline/image/create_ios_app.jpg)
- 创建成功后自动生成 AppKey 用以标识该应用。
![jpush_ios](http://docs.jpush.io/guideline/image/Screenshot_13-4_2_create.jpg)
### 2、导入API开发包到应用程序项目
- 将SDK包解压，在XCode中选择“Add files to 'Your project name'...”，将解压后的lib子文件夹（包含JPUSHService.h、jpush-ios-x.x.x.a）添加到你的工程目录中。
### 3、必要的框架
- CFNetwork.framework
- CoreFoundation.framework
- CoreTelephony.framework
- SystemConfiguration.framework
- CoreGraphics.framework
- Foundation.framework
- UIKit.framework
- Security.framework
- Xcode7需要的是libz.tbd；Xcode7以下版本是libz.dylib
### 4、Build Settings
如果你的工程需要支持小于7.0的iOS系统，请到Build Settings 关闭 bitCode 选项，否则将无法正常编译通过。
- 设置 Search Paths 下的 User Header Search Paths 和 Library Search Paths，比如SDK文件夹（默认为lib）与工程文件在同一级目录下，则都设置为"$(SRCROOT)/{静态库所在文件夹名称}"即可。
### 5、创建并配置PushConfig.plist文件
2.1.0 版本开始，新增了带参数的setupWithOption初始化方法，可通过此方法等参数传入AppKey等信息。1.8.8及之前版本的 JPush SDK只能通过PushConfig.plist配置AppKey等信息。
在你的工程中创建一个新的Property List文件，并将其命名为PushConfig.plist，文件所含字段如下：
- CHANNEL
- 指明应用程序包的下载渠道，为方便分渠道统计，具体值由你自行定义，如：App Store。
- APP_KEY
- 填写[管理Portal上创建应用](https://www.jpush.cn/apps/new)后自动生成的AppKey值。请确保应用内配置的 AppKey 与第1步在 Portal 上创建应用后生成的 AppKey 一致。
- APS_FOR_PRODUCTION
- 1.3.1版本新增，用于标识当前应用所使用的APNs证书环境。
- 0 (默认值)表示采用的是开发证书，1 表示采用生产证书发布应用。
- 注：此字段的值要与Build Settings的Code Signing配置的证书环境一致。
- 在1.2.2或之前版本的配置文件中，有 TEST_MODE 这个键，新版的SDK不再使用，可以将它删除。
PushConfig.plist文件示例图:
![jpush_ios](http://docs.jpush.io/guideline/image/Screenshot_13-4-15_3_31.png)
### 6、添加代码
2.1.0版本开始,API类名为JPUSHService，不再使用原先的APService。
#### 允许XCode7支持Http传输方法
如果用的是Xcode7时，需要在App项目的plist手动加入以下key和值以支持http传输:
```
<key>NSAppTransportSecurity</key> 
      <dict> 
  <key>NSAllowsArbitraryLoads</key> 
        <true/> 
    </dict>
```
#### 集成所需API
APIs 主要集中在 JPUSHService 接口类里。
- 
初始化JPush方法分为两个：
- 1.8.8及以下版本使用的是已过期的初始化方法。升级到2.1.0的老用户仍可继续使用旧的初始化方法。
- 2.1.0版本开始提供带appkey等参数的新初始化方法。使用此方法无需再添加PushConfig.plist配置JPush的AppKey等字段。
## 4.项目代码：
** AppDelegate.h里面的代码：**
**#import<UIKit/UIKit.h>**
**staticNSString *appKey = @"4fd48a0712a3fde75eb1c7423";//申请应用成功以后官方会提供给你**
**staticNSString *channel = @"Publish channel";**
**staticBOOL isProduction = FALSE;**
**@interface AppDelegate :UIResponder <UIApplicationDelegate>**
**@property (strong,nonatomic) UIWindow *window;**
**@end**
#import
"AppDelegate.m"中的代码
#import "AppDelegate.h"
#import "JPUSHService.h"
@interfaceAppDelegate ()
@end
@implementation AppDelegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary
 *)launchOptions {
// Override point for customization after application launch.
self.window.backgroundColor = [UIColorwhiteColor];
    [self.windowmakeKeyAndVisible];
if ([[UIDevicecurrentDevice].systemVersionfloatValue] >= 8.0) {
//可以添加自定义categories
        [JPUSHServiceregisterForRemoteNotificationTypes:(UIUserNotificationTypeBadge |
UIUserNotificationTypeSound |
UIUserNotificationTypeAlert)
categories:nil];
    } else {
//categories 必须为nil
        [JPUSHServiceregisterForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge |
UIRemoteNotificationTypeSound |
UIRemoteNotificationTypeAlert)
categories:nil];
    }
    [JPUSHServicesetupWithOption:launchOptions appKey:appKey
channel:channelapsForProduction:NO];
returnYES;
}
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData
 *)deviceToken {
// Required
    [JPUSHServiceregisterDeviceToken:deviceToken];
}
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary
 *)userInfo {
// Required,For systems with less than or equal to iOS6
    [JPUSHServicehandleRemoteNotification:userInfo];
}
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary
 *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
// IOS 7 Support Required
    [JPUSHServicehandleRemoteNotification:userInfo];
    completionHandler(UIBackgroundFetchResultNewData);
}
- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError
 *)error {
//Optional
NSLog(@"did Fail To Register For Remote Notifications With Error: %@", error);
}

- (void)applicationWillResignActive:(UIApplication *)application {
// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user
 quits the application and it begins the transition to the background state.
// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}
- (void)applicationDidEnterBackground:(UIApplication *)application {
// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated
 later.
// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}
- (void)applicationWillEnterForeground:(UIApplication *)application {
// Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}
- (void)applicationDidBecomeActive:(UIApplication *)application {
// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}
- (void)applicationWillTerminate:(UIApplication *)application {
// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}
@end

## 4.真机测试：
登陆到极光推送官网:![](https://img-blog.csdn.net/20160121094908262?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击控制台：
![](https://img-blog.csdn.net/20160121095009264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
选择应用，也就是你创建测试的项目：我的项目是aa.TestPush
![](https://img-blog.csdn.net/20160121095232110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
选中以后点击进入到下面这里：
![](https://img-blog.csdn.net/20160121095342133?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击上面的推送：
![](https://img-blog.csdn.net/20160121095441144?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击左边的发送通知，也可以选择自定义消息(这里我选择的发送通知)：
![](https://img-blog.csdn.net/20160121095633322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里我随便写点什么用我手机测试：
![](https://img-blog.csdn.net/20160121100042495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20160121100058402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击立即发送：
![](https://img-blog.csdn.net/20160121100124621?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20160121100205234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
手机上的显示：
![](https://img-blog.csdn.net/20160121100443956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

