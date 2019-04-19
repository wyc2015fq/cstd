# iOS百度推送的基本使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年01月27日 15:56:42[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1864
## 一.iOS证书指导
在 iOS App 中加入消息推送功能时，必须要在 Apple 的开发者中心网站上申请推送证书，每一个 App 需要申请两个证书，一个在开发测试环境下使用，另一个用于上线到 AppStore 后的生产环境。
### 7.1 为你的 App 创建 App ID
iOS 中每个 App 都需要对应一个 App ID，同一个公司可能会使用类似于 `com.example.*` 这样通用的 App ID，但是如果要在 App 中加入消息推送功能，那么是不能使用通用 ID 的，需要为之单独创建一个。
首先登陆 [iOS Dev Center](https://developer.apple.com/) ，然后进入 Member Center，然后选择 Certificates，Identifiers & profiles，如下图：
![Member Center](http://push.baidu.com/static/md/cert/adx_member_center.png)
然后点击下图红框中的任意条目，进入证书界面，如下图:
![certifiates select](http://push.baidu.com/static/md/cert/adx_cert_select.png)
在进入证书界面后，在左边的Identifiers选择中选定App IDs，点右上角加号创建Appid，如下图：
![Create App ID](http://push.baidu.com/static/md/cert/adx_create_appid.png)
在创建 App ID 的过程中，需要勾选 Push 服务，如下图:
![select push notification](http://push.baidu.com/static/md/cert/adx_select_push.png)
进入提交页面，push服务处于configurable状态，如下图:
![submit push notification](http://push.baidu.com/static/md/cert/adx_push_configurable.png)
点击submit后到确认页面，如下图:
![done push notification](http://push.baidu.com/static/md/cert/adx_push_done.png)
点击done后到初始页面，然后再次选择自己创建的appid，如下图:
![choose push addid](http://push.baidu.com/static/md/cert/adx_push_choose_appid.png)
在下图中选择edit按钮，配置推送的环境，如图:
![edit push mode](http://push.baidu.com/static/md/cert/adx_edit_push_mode.png)
然后配置好对应的推送环境，个人版和企业版的开发环境都是选择创建Development SSL Certificate类型的。个人版和企业版的发布环境。发布环境分以下三种：1. in-house必须是企业开发账户（企业内）（299美金） 2.ad-hoc个人账户或公司Company账户（99美金），但只用于内部测试（总共100个设备）.3.上线Appstore只能是个人账户或公司Company账户（99美金））如下图:
![done push mode](http://push.baidu.com/static/md/cert/adx_done_push_mode.png)
如果你是为已有的 App 增加消息推送功能，那么打开原有的 App ID，开启 Push Notification 选项即可。流程跟上面的一样。
### 7.2 创建及下载证书
点击 Create Certificate按钮后会出现“About Creating a Certificate Signing Request (CSR)”，如下图：
![Select Certificate Signing Request](http://push.baidu.com/static/md/cert/adx_select_csr.png)
到了这里，需要停下制作 CSR 文件，制作过程比较简单，下面是制作的过程。打开 Mac 系统软件'钥匙串访问'，选择 '证书助理' 及 '从证书颁发机构请求证书'，制作 CSR 文件，如下图:
![Open keyChain](http://push.baidu.com/static/md/cert/adx_open_keychain.png)
![Open keyChain](http://push.baidu.com/static/md/cert/adx_create_ca.png)
生成证书后，返回到 “About Creating a Certificate Signing Request (CSR)” 的界面，点击 continue，然后在 “Choose File” 选择生成的CSR文件，最后点击 Generate，生成证书。如下图:
![generate certifate](http://push.baidu.com/static/md/cert/adx_generate_cert.png) 现在证书制作已经完成。下载并双击用“钥匙串访问” 程序打开后，在左边一栏，上面选择登录，下面选择证书，然后选择刚刚打开的证书，切记不要展开它，直接右击导出p12，如下图：
![open certifate](http://push.baidu.com/static/md/cert/adx_open_cer.png)
将文件保存为 .p12 格式，输入密码，如图所示：
![save as p12](http://push.baidu.com/static/md/cert/adx_save_p12.png)
最后进入终端，到证书目录下，运行以下命令将p12文件转换为pem证书文件：
`openssl pkcs12 -in MyApnsCert.p12 -out MyApnsCert.pem -nodes`
提示需要输入密码，输入刚才导出 p12 时的密码即可。
Provisioning Profile的创建 点击下图的+按钮开始创建profile
![save as p12](http://push.baidu.com/static/md/cert/adx_provisioning_profile.png)
选择profile的环境
![save as p12](http://push.baidu.com/static/md/cert/adx_provisioning_profile_ev.png)
选择创建profile的appid和开发者证书，并选择设备，最后生成profile
![save as p12](http://push.baidu.com/static/md/cert/adx_provisioning_profile_device.png)
最后下载profile配置到xcode中进行开发测试
最后得到的证书
![](https://img-blog.csdn.net/20160127161035555?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2.创建工程：**
点击进入新建或已有的工程，将会显示出应用的基本信息，其中 API key 和 appid 需要在 Demo 中使用，如图所示：
![](http://push.baidu.com/static/md/cert/proj_basic_info.png)
进入左边导航栏，选择“云推送”功能，首次选择时，需要进行【推送设置】，如图所示：
![](http://push.baidu.com/static/md/cert/proj_push_setting.png)
说明（iOS）：
开发证书：需上传推送证书的“开发版本” 的pem文件。
生产证书：需上传推送证书的“生产版本” 的pem文件。
部署状态：开发测试期间选择 【开发状态】，待 App 上线完成后可更改为【生产状态】。
这里选 择了什么状态就要上传什么证书要一一对应
![](https://img-blog.csdn.net/20160127161928646?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**3.用百度Demo进行测试 （一般选择开发测试）**
**打开下载好的百度推送SDK选择开发测试Demo：**
![](https://img-blog.csdn.net/20160127161335941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
###  修改工程设置
选择相应的Demo分开发Demo和发布Demo打开.xcodeproj 工程，首先需要修改 Bundle Identifier ，修改为在创建证书时所选择的 Bundle ID，如下图:
![Demo 配置参数](http://push.baidu.com/static/md/cert/set_bundle_id.png)
修改下面方法中的apikey的值为自己的apikey,并配置为自己的证书，如下图:
![Demo 配置参数](http://push.baidu.com/static/md/cert/set_apikey.png)
### 3.5 运行Demo应用
若以上步骤均无误，即可对 Demo 进行真机测试(推送通知必须在真机环境下进行测试)。如下图所示 ![Demo 运行截图](http://push.baidu.com/static/md/cert/demo_screen.png)
### 3.6 Demo客户端查看消息
打开Demo后，可以绑定、解绑以及添加删除tag，Demo界面会显示服务器返回数据。在绑定成功后，使用百度开放服务平台推送消息，进入指定的应用，选择云推送，将显示以下界面，如图所示：
![](http://push.baidu.com/static/md/cert/proj_push_control.png)
**4.自己创建工程实现百度推送：**
第一步：
### 添加SDK到工程中
![](https://img-blog.csdn.net/20160127162559211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
添加到SDK到工程中的步骤如下：
• 将libBPush.a和BPush.h添加到自己的工程下，添加时需要注意勾选当前Target
2.添加依赖库：
• SDK需要以下库：Foundation.framework、CoreTelephony.framework、libz.dylib、SystemConfiguration.framework，请在工程中添加。
![](https://img-blog.csdn.net/20160127162818384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**3.开启Remote notifications：**
客户端设置开启Remote notifications,需要在Xcode 中修改应用的 Capabilities 开启Remote notifications，(红框内的必须要做哦)请参考下图：
![](http://push.baidu.com/static/md/cert/backgroundset.png)
ios7之后如果像上面一样设置好了，并且服务端aps字段中添加content-available字段为1的话，了那么收到远程通知后，应用在后台的话会在下面的方法中接受到远程通知，对应程序中的代码是（注意，iOS7之后没有开启后台的话也可以通过点击通知调起下面的方法只是不能在后台状态下运行一段代码）：
`- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler`
```
```
**4.写代码：**
AppDelegate.m中
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary
 *)launchOptions {
// Override point for customization after application launch.
ViewController *vc = [[ViewControlleralloc]init];
UINavigationController *nav = [[UINavigationControlleralloc]initWithRootViewController:vc];
self.window.rootViewController = nav;
// iOS8下需要使用新的 API
if ([[[UIDevicecurrentDevice]systemVersion]floatValue]
 >=8.0) {
UIUserNotificationType myTypes =UIUserNotificationTypeBadge |UIUserNotificationTypeSound
 | UIUserNotificationTypeAlert;
UIUserNotificationSettings *settings = [UIUserNotificationSettingssettingsForTypes:myTypescategories:nil];
        [[UIApplicationsharedApplication]registerUserNotificationSettings:settings];
    }else {
UIRemoteNotificationType myTypes =UIRemoteNotificationTypeBadge|UIRemoteNotificationTypeAlert|UIRemoteNotificationTypeSound;
        [[UIApplicationsharedApplication]registerForRemoteNotificationTypes:myTypes];
    }
    [BPushregisterChannel:launchOptionsapiKey:@"0KLGrI7nYN3WOcHDTYGwobn3"pushMode:BPushModeDevelopmentwithFirstAction:nilwithSecondAction:nilwithCategory:nilisDebug:YES];
// App是用户点击推送消息启动
NSDictionary *userInfo = [launchOptionsobjectForKey:UIApplicationLaunchOptionsRemoteNotificationKey];
if (userInfo) {
NSLog(@"从消息启动:%@",userInfo);
        [BPushhandleNotification:userInfo];
    }
#if TARGET_IPHONE_SIMULATOR
Byte dt[32] = {0xc6,0x1e,0x5a,0x13,0x2d,0x04,0x83,0x82,0x12,0x4c,0x26,0xcd,0x0c,0x16,0xf6,0x7c,0x74,0x78,0xb3,0x5f,0x6b,0x37,0x0a,0x42,0x4f,0xe7,0x97,0xdc,0x9f,0x3a,0x54,0x10};
    [selfapplication:applicationdidRegisterForRemoteNotificationsWithDeviceToken:[NSDatadataWithBytes:dtlength:32]];
#endif
//角标清0
    [[UIApplicationsharedApplication]setApplicationIconBadgeNumber:0];
returnYES;
}
- (void)application:(UIApplication *)application didRegisterUserNotificationSettings:(UIUserNotificationSettings
 *)notificationSettings
{
    [application registerForRemoteNotifications];
}
- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification
 *)notification
{
NSLog(@"接收本地通知啦！！！");
    [BPushshowLocalNotificationAtFront:notificationidentifierKey:nil];
}
- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError
 *)error
{
NSLog(@"DeviceToken获取失败，原因：%@",error);
}
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData
 *)deviceToken
{
NSLog(@"test:%@",deviceToken);
    [BPushregisterDeviceToken:deviceToken];
    [BPushbindChannelWithCompleteHandler:^(id result,NSError
 *error) {
//需要在绑定成功后进行 settag listtag deletetag unbind操作否则会失败
if (result) {
            [BPushsetTag:@"Mytag"withCompleteHandler:^(id
 result,NSError *error) {
if (result) {
NSLog(@"设置tag成功");
                }
            }];
        }
    }];
}
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary
 *)userInfo
{
// App收到推送的通知
    [BPushhandleNotification:userInfo];
NSLog(@"********** ios7.0之前 **********");
//应用在前台或者后台开启状态下，不跳转页面，让用户选择。
if (application.applicationState ==UIApplicationStateActive
 || application.applicationState ==UIApplicationStateBackground) {
NSLog(@"acitve or background");
UIAlertView *alertView =[[UIAlertViewalloc]initWithTitle:@"收到一条消息"message:userInfo[@"aps"][@"alert"]delegate:selfcancelButtonTitle:@"取消"otherButtonTitles:@"确定",nil];
        [alertView show];
    }
NSLog(@"%@",userInfo);
}
5.真机测试:由于推送功能在模拟器上无法调试，所以只能在真机上进行测试
![](https://img-blog.csdn.net/20160127164210942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以根据打印日志里面的
**"channel_id"  实现单播（对指定的手机发送通知）**
**![](https://img-blog.csdn.net/20160127164523453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
确定发送，手机收到通知：
![](https://img-blog.csdn.net/20160127164703391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
当发送广播时，所有的设备都可以收到通知 ：
![](https://img-blog.csdn.net/20160127165010500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160127165037469?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
手机收到通知：
![](https://img-blog.csdn.net/20160127165213112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
