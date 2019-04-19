# 手把手教你如何搭建iOS项目基本框架 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月28日 09:37:14[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1117
今天我们来谈谈如何搭建框架，框架需要做一些什么。
> 
第一步：找到我们的目标
我们的目标是让其他开发人员拿到手后即可写页面，不再需要考虑其他的问题。
> 
第二步：我们需要做哪些东西
各位跟着我一步一步来进行。
假定我们即将要写的项目是一个tabbar+navgation的项目结构。
# 1. 新建工程
#### 1.1 删除不必要类目
选择Single ViewApplication，命名为HomeHome
删除选中的三项。
![](http://upload-images.jianshu.io/upload_images/1694866-44bce298bd2887e2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
删除选中的三项
#### 1.2 修改info.plist
1.删除info.plist main.storyboard字段
![](http://upload-images.jianshu.io/upload_images/1694866-a4d1a717781ec512.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
main.storyboard
2.添加字段
info.plist 中添加以下
Bundle display name --- 对应应用名
开启http访问，不添加该字段导致不能访问http，只能访问https。
![](http://upload-images.jianshu.io/upload_images/1694866-27df6e7db25ddcdd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
开启http
3.添加应用白名单
iOS9之后分享等必须添加白名单后才可以打开相关应用。字段值为LSApplicationQueriesSchemes
- 添加URL Types
用于分享到QQ，微信等平台或者打开支付宝等，已经设置当前APP的Url shesmes
#### 1.3 修改项目配置
- 关闭bitCode
build setting中搜索bitCode 改为NO
# 2.配置项目
#### 2.1 建立目录
![](http://upload-images.jianshu.io/upload_images/1694866-f24ac1ebadcf3cdd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
项目目录
#### 2.2 拖入各大依赖库
我在这里主要依赖了以下库：
> 
[AFNetWorking 网络请求](https://github.com/AFNetworking/AFNetworking)
[SDWebImage 图片加载](https://github.com/rs/SDWebImage)
[MWPhotoBrowser 图片浏览器](https://github.com/mwaterfall/MWPhotoBrowser)
[MJRefresh 刷新加载控件](https://github.com/CoderMJLee/MJRefresh)
[RDVTabBarController 一个很好用的tabbar控件](https://github.com/robbdimitrov/RDVTabBarController)
[MBProgressHUD 菊花转圈](https://github.com/jdg/MBProgressHUD)
[NJKWebViewProgress webview的加载进度条](https://github.com/ninjinkun/NJKWebViewProgress)
[Masonry 用于适配](https://github.com/SnapKit/Masonry)
### 2.3 依赖常用第三方服务
通常集成了：
> 
[友盟分享](http://www.umeng.com/)
[极光推送](https://www.jpush.cn/)
[腾讯bugly](http://bugly.qq.com/)
[Mob验证码服务](http://www.mob.com/#/)
均根据官方文档安装
# 3. 编写代码
#### 3.1 建立pch
[xcode6之后建立pch 参考该网址](http://www.cnblogs.com/YouXianMing/p/3989155.html)
#### 3.1 建立Api.h文件
该声明文件用于查询接口等
#### 3.2 建立Config.h文件
该声明文件用于编写某些全局配置参数
如以下：
```cpp
```cpp
#define kPushPhotoBrowserNotifitationName @"PushPhotoBrowser"
#define kPresentVideoPlayerNotifitationName @"playCallBackVideo"
#define APPICONIMAGE [UIImage imageNamed:[[[[NSBundle mainBundle] infoDictionary] valueForKeyPath:@"CFBundleIcons.CFBundlePrimaryIcon.CFBundleIconFiles"] lastObject]]
#define APPNAME [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleDisplayName"]
#define Main_Color [UIColor colorWithRed:(3)/255.0 green:(160)/255.0 blue:(235)/255.0 alpha:1.0]
#define Main2_Color [UIColor colorWithRed:(135)/255.0 green:(202)/255.0 blue:(231)/255.0 alpha:1.0]
#define VTColor(r, g, b) [UIColor colorWithRed:(r)/255.0 green:(g)/255.0 blue:(b)/255.0 alpha:1.0]
#define Text_Color [UIColor colorWithRed:(51)/255.0 green:(71)/255.0 blue:(113)/255.0 alpha:1.0]
#define BackGround_Color [UIColor colorWithRed:(235)/255.0 green:(235)/255.0 blue:(241)/255.0 alpha:1.0]
#define Default_Person_Image [UIImage imageNamed:@"default_parents"]
#define Default_General_Image [UIImage imageNamed:@"default_general"]
#define kScreenW [UIScreen mainScreen].bounds.size.width
#define kScreenH [UIScreen mainScreen].bounds.size.height
//以及各种第三方服务商的appId或者App key
```
```
#### 3.3 开始编写项目结构
我们分别建立了3个AppDelegate的类别
> 
HHAppDelegate+AppService //app的服务管理
HHAppDelegate+AppLifeCircle//app的生命周期管理
HHAppDelegate+RootController//app的跟视图控制器实例
先看看HHAppDelegate+RootController
> 
声明文件
```
/**
 *  首次启动轮播图
 */
- (void)createLoadingScrollView;
/**
 *  tabbar实例
 */
- (void)setTabbarController;
/**
 *  window实例
 */
- (void)setAppWindows;
/**
 *  设置根视图
 */
- (void)setRootViewController;
```
> 
实现：
```
- (void)setRoot
{
    UINavigationController * navc = [[UINavigationController alloc] initWithRootViewController:self.viewController];
    navc.navigationBar.barTintColor = Main_Color;
    navc.navigationBar.shadowImage = [[UIImage alloc] init];
    [navc.navigationBar setTranslucent:NO];
    [[UINavigationBar appearance] setTitleTextAttributes:@{NSForegroundColorAttributeName:[UIColor whiteColor]}];
    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
    [navc.navigationBar setTitleTextAttributes:@{NSFontAttributeName:[UIFont systemFontOfSize:19],NSForegroundColorAttributeName:[UIColor whiteColor]}];
    navc.navigationBar.tintColor = [UIColor whiteColor];
    self.window.rootViewController = navc;
}
- (void)setTabbarController
{
    HomePageViewController *school = [[HomePageViewController alloc]init];
   AboutChildViewController *child  = [[AboutChildViewController alloc]init];
    CommuntiyViewController *edu = [[CommuntiyViewController alloc]init];
    SZCourseListViewController *courseList = [[SZCourseListViewController alloc]init];
    AboutMeViewController *about = [[AboutMeViewController alloc]init];
    RDVTabBarController *tabBarController = [[RDVTabBarController alloc] init];
    [tabBarController setViewControllers:@[school,edu,child,courseList,about]];
    self.viewController = tabBarController;
    tabBarController.delegate = self;
    [self customizeTabBarForController:tabBarController];
}
- (void)setAppWindows
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.backgroundColor = [UIColor whiteColor];
    [[UIApplication sharedApplication]setStatusBarHidden:NO];
    [[UIApplication sharedApplication]setStatusBarStyle:UIStatusBarStyleLightContent];
    [[UINavigationBar appearance] setTitleTextAttributes:@{NSForegroundColorAttributeName: [UIColor whiteColor]}];
}
- (void)createLoadingScrollView
{
  //引导页实例
}
```
> 
注意：这里将Navgation作为根视图控制器，tabbar作为Navgation的rootViewController。
如此全局只有一个跟导航控制器。页面Controller与Tabbar处于平级状态。这与接下来我们弹出各种页面均由关联
自此一个大致的结构就出来了，一个Tabbar+Navgation的格式
#### 3.4 编写AppService
> 
这类信息大都跟需求有关，我们在这里处理项目相关信息。如可能会由以下信息
```
- (void)registerBugly;
/**
 *  基本配置
 */
- (void)configurationLaunchUserOption;
/**
 *  友盟注册
 */
- (void)registerUmeng;
/**
 *  Mob注册
 */
- (void)registerMob;
/**
 *  检查更新
 */
- (void)checkAppUpDataWithshowOption:(BOOL)showOption;
/**
 *  上传用户设备信息
 */
- (void)upLoadMessageAboutUser;
/**
 *  检查黑名单用户
 */
-(void)checkBlack;
```
#### 3.5 App结构整理
> 
以上我们处理了跟视图，服务，app声明周期等方法实现
现在我们开始调用这些方法。
```
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [self setAppWindows];
    [self setTabbarController];
    [self setRootViewController];
    [self configurationLaunchUserOption];
    [self registerBugly];
    [self registerMob];
    [self registerUmeng];
    [VTJpushTools setupWithOptions:launchOptions];
    [self upLoadMessageAboutUser];
    [self checkAppUpDataWithshowOption:NO];
    [SZNotificationCenter addObserver:self selector:@selector(phontoBroserPush:) name:kPushPhotoBrowserNotifitationName object:nil];
    [SZNotificationCenter addObserver:self selector:@selector(playCallBackVideo:) name:kPresentVideoPlayerNotifitationName object:nil];
    [self.window makeKeyAndVisible];
    return YES;
}
```
好了，我们着重说一下这里两个通知。
分别是收到图片查看的通知、视频播放的通知
这里以查看图片举列。我们认为查看图片是push到一个新页面中去。
> 
因此我们需要找到导航控制器
根据之前所说，当前结构中只有一个导航控制器就是Root
```
+ (UINavigationController *)rootNavigationController
{
    HHAppDelegate *app = (HHAppDelegate *)[UIApplication sharedApplication].delegate;
    return (UINavigationController *)app.window.rootViewController;
}
```
找到控制器后就变得简单的多了。
我们开始实现：
> 
这里我们对传来的数据格式由一定要求，类似于这样的一个格式。
`@{@"imageInfo":@[HHImageInfo*info,HHImageInfo *info2],@"index":@3};`
> 
原因：图片一般有高清图片和非高清图片两种格式。以及图片有可能带有文字描述。因此引入ImageInfo。
```
- (void)phontoBroserPush:(NSNotification *)note
{
    self.imageArr = [NSMutableArray arrayWithCapacity:0];
    [self.imageArr removeAllObjects];
    NSDictionary *dic = note.object;
    NSArray *arr = dic[@"imageInfo"];
    NSInteger index = [dic[@"index"] integerValue];
    self.imageIndex = index;
    for(HHImageInfo *info in arr)
    {
        //[self.imageArr addObject:info.url];
        MWPhoto *photo = [MWPhoto photoWithURL:[NSURL URLWithString:info.url]];
        photo.caption = info.desc;
        [self.imageArr addObject:photo];
    }
    MWPhotoBrowser *browser = [[MWPhotoBrowser alloc]initWithDelegate:self];
    browser.zoomPhotosToFill = YES;
    browser.displayNavArrows = YES;
    browser.displayActionButton = NO;
    browser.alwaysShowControls = NO;
    browser.autoPlayOnAppear = YES;
    [browser setCurrentPhotoIndex:index];
    [[HHAppDelegate rootNavigationController]pushViewController:browser animated:YES];
}
```
> 
到这里，其他开发人员在进行模块开发的时候需要图片浏览器的时候直接发送通知即可集成。
HHImageInfo中
```
@property (nonatomic,strong)NSString *desc;
@property (nonatomic,strong)NSString *url;
//@property (nonatomic,assign)NSInteger imageIndex;
+ (NSString *)getHUDImageUrl:(NSString *)smallImageUrl;
```
实现：
```
+ (NSString *)getHUDImageUrl:(NSString *)smallImageUrl
{
    NSMutableString *str = [NSMutableString stringWithString:smallImageUrl];
    NSRange substr = [str rangeOfString:@"_thu"];
    while (substr.location != NSNotFound)
    {
        [str replaceCharactersInRange:substr withString:@""];
        substr = [str rangeOfString:@"_thu"];
    }
    [str insertString:@"" atIndex:0];
    return str;
}
```
# 4. 其他配置
#### 4.1 rootViewController -- 所有页面均继承该控制器
> 
我通常在RootViewController实现以下方法以供调用
```
/**
 *  显示没有数据页面
 */
-(void)showNoDataImage;
/**
 *  移除无数据页面
 */
-(void)removeNoDataImage;
/**
 *  需要登录
 */
- (void)showShouldLoginPoint;
/**
 *  加载视图
 */
- (void)showLoadingAnimation;
/**
 *  停止加载
 */
- (void)stopLoadingAnimation;
/**
 *  分享页面
 *
 *  @param url   url
 *  @param title 标题
 */
- (void)shareUrl:(NSString *)url andTitle:(NSString *)title;
- (void)goLogin;
/**
 *  状态栏
 */
- (void)initStatusBar;
- (void)showStatusBarWithTitle:(NSString *)title;
- (void)changeStatusBarTitle:(NSString *)title;
- (void)hiddenStatusBar;
```
具体实现方法请看demo
[Demo下载](http://7xiiiu.com1.z0.glb.clouddn.com/HomeHomeDemo.zip)
有点乱七八糟，但是你明白我的意思对不？
文／五阿哥永琪（简书作者）
原文链接：http://www.jianshu.com/p/0c6f3f4b3b34
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
