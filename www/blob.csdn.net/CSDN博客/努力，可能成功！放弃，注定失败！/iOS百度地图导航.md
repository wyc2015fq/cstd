# iOS百度地图导航 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月09日 15:42:54[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2878
# Come on !  手把手教你实现百度地图导航功能！
**一 ：将工程中导入百度地图库**
** 1.首先将百度地图SDK导入到工程里，这里我就不演示用手动导了，直接Cocopods导，首先打开终端，终端输入cd /  +你的工程     路径，如下图：**
![](https://img-blog.csdn.net/20160909091633476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2.在你工程目录下创建podfile文件，在终端输入touch podfile 创建完成后你可以手动打开工程查看podfile文件是否创 建成功也可以在****终端输入open ./查看如下图：**
![](https://img-blog.csdn.net/20160909155339856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**3.在终端输入pod search'BaiduMapKit' 搜索百度地图库，搜索完成如下图：**
**![](https://img-blog.csdn.net/20160909155751124?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**4.复制搜索到的百度地图库版本号：**** pod 'BaiduMapKit', '~> 3.0.0'  , 在终端输入vim podfile  进入到podfile文件中然后输入**
**i 键编辑podfile文件，将百度地图版本号pod 'BaiduMapKit', '~> 3.0.0'  粘贴到podfile文件中，按下ESC键，再按shift+：键输入wq 保存退出podfile文件，如下图所示：**
**![](https://img-blog.csdn.net/20160909160408815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**5.用终端将百度SDK导入到工程中,在终端输入pod install接下来会等一段时间，因为cocopods需要下载百度地图库导入到你的工程中，如果cocopods导库成功,终端会有如下显示，如图:**
**![](https://img-blog.csdn.net/20160909160832686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**也可以进入到工程中打开文件夹但看是否导入成功，如下图所示：**
**![](https://img-blog.csdn.net/20160909161227223?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**二 ：进入到工程中配置工程文件：**
**1、静态库中采用ObjectC++实现，因此需要您保证您工程中至少有一个.mm后缀的源文件(您可以将任意一个.m后缀的文件改名为.mm)，或者在工程属性中指定编译方式，即在Xcode的Project -> Edit Active Target -> Build Setting 中找到 Compile Sources As，并将其设置为"Objective-C++ 我回为比较懒所以经常用第一种方法：**
![](https://img-blog.csdn.net/20160909161616023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**2、如果您只在Xib文件中使用了BMKMapView，没有在代码中使用BMKMapView，编译器在链接时不会链接对应符号，需要在工程属性中显式设定：在Xcode的Project -> Edit Active Target -> Build Setting -> Other Linker Flags中添加-ObjC**
**3、授权Key的申请：新、旧Key之间不可通用，即新Key只可以使用在v2.0.2及后续版本的SDK中，旧的Key只适用于v2.0.1及之前版本的SDK；如果还没有授权Key，请 [申请密钥](http://developer.baidu.com/map/ios-mobile-apply-key.htm)**
** 进入到百度地图API官方申请APPKEY  如下图所示：**
![](https://img-blog.csdn.net/20160909161746183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4、由于iOS9改用更安全的https，为了能够在iOS9中正常使用地图SDK，请在"Info.plist"中进行如下配置，否则影响SDK的使用。
<key>NSAppTransportSecurity</key><dict><key>NSAllowsArbitraryLoads</key><true/></dict>
5、如果在iOS9中使用了调起百度地图客户端功能，必须在"Info.plist"中进行如下配置，否则不能调起百度地图客户端。
<key>LSApplicationQueriesSchemes</key><array><string>baidumap</string></array>
6、管理地图的生命周期：自2.0.0起，BMKMapView新增viewWillAppear、viewWillDisappear方法来控制BMKMapView的生命周期，并且在一个时刻只能有一个BMKMapView接受回调消息，因此在使用BMKMapView的viewController中需要在viewWillAppear、viewWillDisappear方法中调用BMKMapView的对应的方法，并处理delegate，代码如下：
<pre name="code" class="objc">(void)viewWillAppear:(BOOL)animated      
{      
    [_mapView viewWillAppear];      
    _mapView.delegate = self; // 此处记得不用的时候需要置nil，否则影响内存的释放      
}      
-(void)viewWillDisappear:(BOOL)animated      
{      
        [_mapView viewWillDisappear];      
      _mapView.delegate = nil; // 不用时，置nil      
}
7、自[iOS](http://lib.csdn.net/base/1) SDK v2.5.0起，为了对iOS8的定位能力做兼容，做了相应的修改，开发者在使用过程中注意事项如下：需要在info.plist里添加（以下二选一，两个都添加默认使用NSLocationWhenInUseUsageDescription）：
NSLocationWhenInUseUsageDescription ，允许在前台使用时获取GPS的描述
NSLocationAlwaysUsageDescription ，允许永久使用GPS的描述
8、在使用Xcode6进行SDK开发过程中，需要在info.plist中添加：Bundle display name ，且其值不能为空（Xcode6新建的项目没有此配置，若没有会造成manager start failed）
9、百度地图iOS SDK v2.5.0起，对arm64进行了支持适配，开发包体积有所增加。但根据开发者在研发过程中的选择，最终生成的APP体积并不会发生较大的变化。
10、确认项目中添加mapapi.bundle文件以及添加方法正确，不能删除或随意更改其中files文件夹下的内容：
注：mapapi.bundle中存储了定位、默认大头针标注View及路线关键点的资源图片，还存储了矢量地图绘制必需的资源文件。如果您不需要使用内置的图片显示功能，则可以删除bundle文件中的image文件夹。您也可以根据具体需求任意替换或删除该bundle中image文件夹的图片文件。添加方式：将mapapi.bundle拷贝到您的工程目录，直接将该bundle文件托拽至Xcode工程左侧的Groups&Files中即可。若您需要替换定位、指南针的图标，请保留原文件名称，否则不显示替换的新图片，默认大头针标注与路线关键点的新图片名称可自定义名称。
11、注意BMKManager对象的生命周期管理，在使用地图SDK期间不能释放该对象，尤其在arc情况下注意避免提前被自动释放，否则，该对象一旦被释放，网络模块将不可用，地图无法加载，检索失败。 
由于是cocopods导入的地图SDK  .framework包和系统 库就不需要在去手动配置了。
三 ：下载导航SDK包，进入到工程中配置导航SDK所需文件：
下面进入到导航SDK配置，由于百度地图SDK和百度地图导航SDK是分开的  且百度地图导航SDKcocopos没有开放，所以无法用cocopods引入到我们的工程中去，那么我们只有手动去导咯！
**1.下载百度地图导航SDK包，下载完成后解压文件，如下图：**
![](https://img-blog.csdn.net/20160909102945385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## SDK组成
**百度iOS导航SDK由三部分组成：baiduNaviSDK.bundle资源包，libbaiduNaviSDK.a静态库，头文件。baiduNaviSDK.bundle资源包由导航所需的配置数据、基础数据，以及导航功能所需的图片资源组成。libbaiduNaviSDK.a静态库，提供导航功能、线径规划等功能。头文件是libbaiduNaviSDK.a静态库提供给第三方开发者的接口。**
## 将SDK和Framework添加进工程
**把baiduNaviSDK文件夹添加到工程中，把AudioToolbox.framework、ImageIO.framework、CoreMotion.framework、CoreLocation.framework、CoreTelephony.framework、MediaPlayer.framework、AVFoundation.framework、SystemConfiguration.framework、JavaScriptCore.framework、Security.framework 、OpenGLES.framework 、GLKit.framework 、libstdc++6.0.9.dylib、libsqlite3.0.tbd、libz.1.2.5.tbd这几个framework添加到工程中，添加方法为在Xcode中单击工程文件，选择Build Phrases选项，点击Link Binary with Libraries下的“+”逐个添加，如下所示：**
![HelloWorld-3-1.png](http://wiki.lbsyun.baidu.com/cms/iosnav/img/HelloWorld-3-1.png)
## 修改Build Settings设置项
**在Build Settings中， “Other Linker Flags”添加“-ObjC” 标识**
![helloworld4-1.png](http://wiki.lbsyun.baidu.com/cms/iosnav/img/helloworld4-1.png)
## 配置.plist文件
**设置 “Required background modes”、 “App Transport Security Settings”、 ”NSLocationAlwaysUsageDescription”、 ”NSLocationWhenInUseUsageDescription”、 ”View controller-based status bar appearance”、” LSApplicationQueriesSchemes”这6项，如下图:**

![HelloWorld-5.png](http://wiki.lbsyun.baidu.com/cms/iosnav/img/HelloWorld-5.png)

**也可复制下面源码：**
**<key>NSAppTransportSecurity</key>**
**<dict>**
**<key>NSAllowsArbitraryLoads</key>**
**<true/>**
**</dict>**
**<key>NSLocationAlwaysUsageDescription</key>**
**<string>只有开启定位功能才能正常使用百度导航</string>**
**<key>NSLocationWhenInUseUsageDescription</key>**
**<string>只有开启定位功能才能正常使用百度导航</string>**
**<key>UIBackgroundModes</key>**
**<array>**
**<string>audio</string>**
**<string>location</string>**
**</array>**
**<key>UIViewControllerBasedStatusBarAppearance</key>**
**<false/>**
**<key>LSApplicationQueriesSchemes</key>**
**<array>**
**<string>baidumap</string>**
**</array>**
**2.SDK，工程文件 配置完成后，下面配置语音播报功能**
语音播报
iOS导航SDK
## 简述
**SDK内置百度TTS语音播报功能，需要对应用进行授权验证才能够使用，因此需要主动注册应用相关信息。**
## 申请TTS授权
**可以在[导航TTS平台](http://app.navi.baidu.com/ttsregister/appinfo)中进行注册申请。点击“创建应用”跳转至如下页面：**

![ios-voice-step1.png](http://wiki.lbsyun.baidu.com/cms/iosguide/ios-voice-step1.png)
**注册成功后的邮箱如下图所示：**

![ios-voice-step2.png](http://wiki.lbsyun.baidu.com/cms/iosguide/ios-voice-step2.png)
**3.以上都配置成功后，下面进入到工程里面进行代码书写：**
**在您的AppDelegate.h文件中添加BMKMapManager的定义**
```
#import <UIKit/UIKit.h>
#import <BaiduMapAPI_Base/BMKMapManager.h>
@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
    BMKMapManager *_mapmanager;
}
@property (strong, nonatomic) UIWindow *window;
@end
```

**在您的AppDelegate.m文件中添加对BMKMapManager的初始化，并填入您申请的授权Key，示例如下:**
```
//
//  AppDelegate.m
//  TextMapNavigation
//
//  Created by 007 on 16/9/9.
//  Copyright © 2016年 xu.lei. All rights reserved.
//
#import "AppDelegate.h"
#import "ViewController.h"
#import "BNCoreServices.h"
@interface AppDelegate ()<BMKGeneralDelegate>
@end
@implementation AppDelegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    _mapmanager = [[BMKMapManager alloc]init];
    BOOL ret = [_mapmanager start:@"输入APPKEY"  generalDelegate:self];
    if (!ret) {
        NSLog(@"manager start failed!");
    }
    //配置
    [BNCoreServices_Instance initServices:@"输入APPKEY"];
    [BNCoreServices_Instance startServicesAsyn:nil fail:nil];
    
    
    ViewController *vc = [[ViewController alloc]init];
    UINavigationController *nav = [[UINavigationController alloc]initWithRootViewController:vc];
    self.window.rootViewController = nav;
    return YES;
}
- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}
- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
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
#pragma mark ---BMKGeneralDelegate---
/**
 *返回网络错误
 *@param iError 错误号
 */
- (void)onGetNetworkState:(int)iError
{
    if (0==iError) {
        NSLog(@"联网成功");
    }
    else
    {
        NSLog(@"onGetNetworkState:%d",iError);
    }
}
/**
 *返回授权验证错误
 *@param iError 错误号 : 为0时验证通过，具体参加BMKPermissionCheckResultCode
 */
- (void)onGetPermissionState:(int)iError
{
    if (0 == iError) {
        NSLog(@"授权成功");
    }else
    {
        NSLog(@"onGetNetworkState:%d",iError);
    }
}
@end
```

**在您的ViewController.m文件中添加BMKMapView的创建代码，示例如下:**
**地图显示和导航实现功能代码：**
```
//
//  ViewController.m
//  TextMapNavigation
//
//  Created by 007 on 16/9/9.
//  Copyright © 2016年 xu.lei. All rights reserved.
//
#import "ViewController.h"
#import <BaiduMapAPI_Map/BMKMapView.h>
#import <BaiduMapAPI_Location/BMKLocationService.h>
#import "BNRoutePlanModel.h"
#import "BNCoreServices.h"
#define SCREEN_HEIGHT   [UIScreen mainScreen].bounds.size.height
#define SCREEN_WIDTH [UIScreen mainScreen].bounds.size.width
@interface ViewController ()<BMKLocationServiceDelegate,BNNaviRoutePlanDelegate,BNNaviUIManagerDelegate>
{
    BMKMapView *_mapview;
    BMKLocationService *_locationSerVice;
}
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    [self configUI];
    [self createMapview];
   
}
//设置视图
- (void)configUI
{
    //设置导航颜色
    self.navigationController.navigationBar.barTintColor = [UIColor yellowColor];
    //设置导航右边按钮
    UIButton *navigationBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    navigationBtn.frame = CGRectMake(0, 7,50, 30);
    [navigationBtn setTitle:@"导航" forState:UIControlStateNormal];
    [navigationBtn setTitleColor:[UIColor greenColor] forState:UIControlStateNormal];
    [navigationBtn addTarget:self action:@selector(navigationAction:) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *rightItem = [[UIBarButtonItem alloc]initWithCustomView:navigationBtn];
    self.navigationItem.rightBarButtonItem = rightItem;
}
- (void)navigationAction:(id)sender
{
    [self startNavi];
}
//设置地图
- (void)createMapview
{
    _mapview = [[BMKMapView alloc]initWithFrame:CGRectMake(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)];
    _mapview.showsUserLocation = YES;
    [self.view addSubview:_mapview];
     
    _locationSerVice = [[BMKLocationService alloc]init];
    //打开定位
    [_locationSerVice startUserLocationService];
}
- (void)viewWillAppear:(BOOL)animated
{
    [_mapview viewWillAppear];
    _locationSerVice.delegate =self;
}
- (void)viewWillDisappear:(BOOL)animated
{
    [_mapview viewWillDisappear];
    _locationSerVice.delegate =nil;
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
  
}
#pragma mark ---BMKLocationServiceDelegate---
/**
 *用户方向更新后，会调用此函数
 *@param userLocation 新的用户位置
 */
- (void)didUpdateUserHeading:(BMKUserLocation *)userLocation
{
    
}
/**
 *用户位置更新后，会调用此函数
 *@param userLocation 新的用户位置
 */
- (void)didUpdateBMKUserLocation:(BMKUserLocation *)userLocation
{
    CLLocationCoordinate2D coor = CLLocationCoordinate2DMake(userLocation.location.coordinate.latitude, userLocation.location.coordinate.longitude);
    _mapview.centerCoordinate = coor;//设置地图中心点
    [_mapview updateLocationData:userLocation];
}
//发起导航
- (void)startNavi
{
    //节点数组
    NSMutableArray *nodesArray = [[NSMutableArray alloc]    initWithCapacity:2];
    
    //起点
    BNRoutePlanNode *startNode = [[BNRoutePlanNode alloc] init];
    startNode.pos = [[BNPosition alloc] init];
    startNode.pos.x = _locationSerVice.userLocation.location.coordinate.longitude;
    startNode.pos.y = _locationSerVice.userLocation.location.coordinate.latitude;
    startNode.pos.eType = BNCoordinate_BaiduMapSDK;
    [nodesArray addObject:startNode];
    
    //终点
    BNRoutePlanNode *endNode = [[BNRoutePlanNode alloc] init];
    endNode.pos = [[BNPosition alloc] init];
    endNode.pos.x = 114.077075;
    endNode.pos.y = 22.543634;
    endNode.pos.eType = BNCoordinate_BaiduMapSDK;
    [nodesArray addObject:endNode];
    //发起路径规划
    [BNCoreServices_RoutePlan startNaviRoutePlan:BNRoutePlanMode_Recommend naviNodes:nodesArray time:nil delegete:self userInfo:nil];
}
#pragma mark ---BNNaviUIManagerDelegate---
//算路成功后，在回调函数中发起导航，如下：
//算路成功回调
-(void)routePlanDidFinished:(NSDictionary *)userInfo
{
    NSLog(@"算路成功");
    
    //路径规划成功，开始导航
    [BNCoreServices_UI showPage:BNaviUI_NormalNavi delegate:self extParams:nil];
}
@end
```

注：如果手机本身装有百度地图APP那么使用导航功能时，会首先调用百度地图APP发起导航，如果手机本身没有装百度地图APP，那么使用导航功能时会调用百度地图网页端导航，具体如下图所示：
![](https://img-blog.csdn.net/20160909114617365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160909114644693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160909114713888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160909114736935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**好吧，大功告成！如果大家有什么不懂的地方可以联系我。**
**Demo太大github传不上去,给一个百度云盘的下载地址吧！**
**Demo下载地址:[https://pan.baidu.com/s/1eS9JCWi](https://pan.baidu.com/s/1eS9JCWi)**
