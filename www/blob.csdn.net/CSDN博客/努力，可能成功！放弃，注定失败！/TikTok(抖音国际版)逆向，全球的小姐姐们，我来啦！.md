# TikTok(抖音国际版)逆向，全球的小姐姐们，我来啦！ - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年02月21日 14:31:59[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：946标签：[TikTok(抖音国际版)逆向，全球的小姐姐们，我来啦！](https://so.csdn.net/so/search/s.do?q=TikTok(抖音国际版)逆向，全球的小姐姐们，我来啦！&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
## 开源地址
首先抛出GitHub地址吧~多多支持指点，谢谢。
[AYTikTokPod](https://github.com/AYJk/AYTikTokPod)
## 简述
iOS逆向工程指的是软件层面上进行逆向分析的过程。
在一般的软件开发流程中，都是过程导向结果。在逆向中，你首先拿到的是结果，然后是去分析实现这个结果的过程。理清过程之后，才开始进行逆向的代码编写，在整个流程中，分析过程的占比是90%，代码书写的过程只占10%。所以本篇更多的讲的是一个思路，代码其实很日常
## 前期准备
- 一台Mac
- 一台iPhone
- [frida-ios-dump](https://github.com/AloneMonkey/frida-ios-dump)
- [Hopper Disassembler](https://www.hopperapp.com/)
- [class-dump](https://github.com/nygard/class-dump)
- [MonkeyDev](https://github.com/AloneMonkey/MonkeyDev)
- [Reveal](https://revealapp.com/)
### frida-ios-dump
用于脱壳，脱壳是逆向的第一步。直接AppStore上下载的应用都有带壳，导致我们无法对他进行任何操作。脱壳的ipa文件，也可以直接去一些越狱商店下载，但是可能版本上比较旧。
如果有一台已越狱的机器，按照frida-ios-dump的wiki来操作很简单。
### Hopper Disassembler
`Hopper Disassembler`是Mac上的一款二进制反汇编器，基本上满足了工作上的反汇编的需要，包括伪代码以及控制流图(Control Flow Graph)，支持ARM指令集并针对Objective-C的做了优化。
### class-dump
`class-dump`是一款可以导出头文件的命令行工具，改程序用于检查Objective-C运行时信息存储在Mach-O文件，它生成类的声明,类别和协议。
### MonkeyDev
`MonkeyDev`的前身是`iOSOpenDev`，在`iOSOpenDev`的基础上增加`CaptainHook Tweak`、`Logos Tweak`、`Command-line Tool`。
MonkeyDev为我们做的事情：
- 创建dylib，通过hook修改类的属性或方法
- 将dylib注入到App中
- 重签名ipa文件
## 静态分析
### 前期准备
拿到TikTok的脱壳ipa文件
由于自己的6s痛失越狱环境，于是脱壳这一步，是拜托了我的好哥们完成的。
只要有越狱手机，砸壳并不复杂，按照网上的教程步骤来就行。
![](https://upload-images.jianshu.io/upload_images/2049519-3dbe20b93945b970.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
已脱壳的可执行文件
class-dump导出头文件
通过命令
```
class-dump -H XXX.app -o /DumpHeaderClass
```
- -H后跟的是脱壳后的app文件路径
- -o是头文件输出的文件夹路径
如图所示为class-dump之后的项目中所有头文件，单从这里，我们就能看出，TikTok项目中，使用的几个第三方库：`AFNetWorking`、`YYKit`、`FaceBook的SDK`。
![](https://upload-images.jianshu.io/upload_images/2049519-42e02e6940304754.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
class-dump后的头文件
> 
tips: 快速搜索对应的头文件或方法，可以新建个工程，将头文件文件夹拖入项目中。有什么工具能比Xcode检索更方便检索代码呢？
Hopper静态分析
直接将脱壳后的二进制可执行文件拖入Hopper，等待一段时间后，Hopper会完成反编译。
![](https://upload-images.jianshu.io/upload_images/2049519-d020e5c1c4d3462d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Hopper反编译之后的结果
左边的展示的是对应的类和方法列表，通过搜索框可以快速定位到方法。
> 
红色框框起来的是模式切换：分别是`汇编模式`、`控制流图模式`、`伪代码模式`、`十六进制模式`
通常我们用的最多的就是`控制流图`和`伪代码`。
Reveal查看界面
`MonkeyDev`会为我们自动注入`RevealService.framework`。`RevealService.framework`需要和对应版本`Reveal`使用。否则请更新替换注入的`RevealService.framework`。
![](https://upload-images.jianshu.io/upload_images/2049519-2b73ff2e3abe3d70.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Reveal展示的TikTok界面布局
`Reveal`能让我们快速定位到我们需要的控制器或视图。
如图，首页的ViewController就是`AWEFeedTableViewController`。
## 问题&处理问题
### Question1
Q1：
发现从其他区的AppStore下载的TikTok打开后什么都没有？
T1：
初步怀疑是网络问题。
A1：
全局代理之后打开还是一片漆黑，基本排除是网络的问题导致的。
### Question2
Q2：
如果不是网络问题，那问题会不会出现在请求参数上？
T2：
使用`Charles`抓包看看
A2：
刷新feed，拿到url
```
/aweme/v1/feed/?version_code=4.3.0&language=zh&pass-region=1&app_name=trill&vid=B196D171-B020-453E-A19C-9AAD845151BE&app_version=4.3.0&carrier_region=CN&is_my_cn=1&channel=App%20Store&mcc_mnc=46001&device_id=6631689375623284225&tz_offset=28800&account_region=&sys_region=CN&aid=1180&screen_width=750&openudid=63ceee2a26c0fd4501ebcf1f47a2311c5551f6e0&os_api=18&ac=WIFI&os_version=12.0&app_language=en&tz_name=Asia/Shanghai&device_platform=iphone&build_number=43004&device_type=iPhone8,1&iid=6635504889546049282&idfa=BFBB2BCA-9743-451B-95CC-F01292FC02F6&ad_user_agent=Mozilla%2F5.0%20%28iPhone%3B%20CPU%20iPhone%20OS%2012_0%20like%20Mac%20OS%20X%29%20AppleWebKit%2F605.1.15%20%28KHTML%2C%20like%20Gecko%29%20Mobile%2F16A366&count=6&feed_style=0&filter_warn=0&max_cursor=0&min_cursor=0&pull_type=1&type=0&volume=0.25&mas=01050af0364af36a45501b82b389f379ef3f8bda89739cc55924e8&as=a1157001fc8b2c2ce65057&ts=1544948924
```
其中有几个字段引起了怀疑：
|key|value|
|----|----|
|is_my_cn|1|
|language|zh|
|account_region|CN|
|carrier_region|CN|
|mcc_mnc|46001|
|tz_name|Asia/Shanghai|
|sys_region|CN|
**分析：**
- `is_my_cn`字面意思，是否是中国，很可能通过标记来判断是否是国内用户。
- `language`语言类型，通过这个来判断可能性比较低，误伤几率很高。外区也可以设置语言中文，但你不可能去影响他使用吧。这么做，是不合理的。
- `account_region`、`carrier_region`、`sys_region`，账号、运营商和系统的地区，可能通过所属地区来进行封锁。
- `mcc_mnc`，`mcc`指的是`移动国家码`，`mnc`指的是`移动网络码`。
- `tz_name`时区。
**验证：**
使用`Charles`的**Rewrite**或者**Breakpoints**来改变URL中传递的`params`。
![](https://upload-images.jianshu.io/upload_images/2049519-7f10ae12a7f91d00.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Rewrite Settings
**结果：**
通过各种组合实验，发现真正产生作用的
|key|value|
|----|----|
|carrier_region|CN|
**这里得到了第一个结论：说明TikTok服务器，是通过运营商来封锁用户的。既然是运营商，那就把`mcc_mnc`这个字段也一起处理。**
|key|value|
|----|----|
|mcc_mnc|46001|
### Question3
Q3：
怎么处理`carrier_region`和`mcc_mnc`?
T3：
上面是通过`Charles`完成了，可以正常观看TikTok的视频，勉强算是完成了部分修改，但局限性很大。
比如：
- 无法评论、关注等操作，因为只`Rewrite`了部分接口，其他接口没有`Rewrite`。
- 离开特定的WiFi就无法观看，无法通过蜂窝网观看视频。(PS:可以通过Thor这个软件的拦截器实现，和Charles的原理一致)
- 如果后续更新添加了接口签名校验，那这种方法就会失效。
A3：
**方案一：**
通过Hook第三方网络库`AFNetWorking`或内部封装的NetService类来修改`carrier_region`字段。
这个方案基本可行，通过Hook`AFHTTPRequestSerializer`类的`requestWithMethod: URLString: parameters: error:`方法。获取`parameters`，然后修改`carrier_region`的值。
> 
优点：
- 方案简单，不需要过多的内部实现分析。
- 能完成所有接口的Hook。
> 
缺点：
- 遇到接口签名校验将失效。
- 所有网络接口都被Hook，如果Hook函数里存在复杂耗时的操作，会严重影响性能。
**方案二：**
iOS系统的`CoreTelephony.framework`的`CTCarrier`类提供了`carrier_region`、`mnc`和`mcc`的获取。通过Hook他们来实现土突破地区限制。
```
/*
 * isoCountryCode
 *
 * Discussion:
 *   Returns an NSString object that contains country code for
 *   the subscriber's cellular service provider, represented as an ISO 3166-1
 *   country code string
 */
@property (nonatomic, readonly, retain, nullable) NSString* isoCountryCode __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_4_0);
/*
 * mobileCountryCode
 *
 * Discussion:
 *   An NSString containing the mobile country code for the subscriber's 
 *   cellular service provider, in its numeric representation
 */
@property (nonatomic, readonly, retain, nullable) NSString *mobileCountryCode __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_4_0);
/*
 * mobileNetworkCode
 *
 * Discussion:
 *   An NSString containing the  mobile network code for the subscriber's 
 *   cellular service provider, in its numeric representation
 */
@property (nonatomic, readonly, retain, nullable) NSString *mobileNetworkCode __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_4_0);
```
**代码编写：**
```
// MARK: - Hook CTCarrier
CHDeclareClass(CTCarrier)
CHMethod0(NSString *, CTCarrier, isoCountryCode) {
    NSDictionary *areaDic = [UserDefaults valueForKey:HookArea];
    NSString *code = [areaDic objectForKey:@"code"];
    return code;
}
CHMethod0(NSString *, CTCarrier, mobileCountryCode) {
    NSDictionary *areaDic = [UserDefaults valueForKey:HookArea];
    NSString *mcc = [areaDic objectForKey:@"mcc"];
    return mcc;
}
CHMethod0(NSString *, CTCarrier, mobileNetworkCode) {
    NSDictionary *areaDic = [UserDefaults valueForKey:HookArea];
    NSString *mnc = [areaDic objectForKey:@"mnc"];
    return mnc;
}
CHConstructor {
    CHLoadLateClass(CTCarrier);
    CHHook0(CTCarrier, isoCountryCode);
    CHHook0(CTCarrier, mobileCountryCode);
    CHHook0(CTCarrier, mobileNetworkCode);
}
```
[CaptainHook](https://github.com/rpetrich/CaptainHook)为我们提供了完善的Hook宏。
- `CHDeclareClass`作用是声明需要Hook的类
- `CHMethod`作用是对应的方法Hook的实现
- `CHConstructor`作用是用于加载Hook的方法和所在的类
- `CHLoadLateClass`加载Hook类
- `CHHook`注册Hook方法
这个framework底层通过`runtime`接口实现对应功能，比如
```
class_getInstanceMethod(Class _Nullable cls, SEL _Nonnull name)
method_setImplementation(Method _Nonnull m, IMP _Nonnull imp)
method_getImplementation(Method _Nonnull m)
method_getTypeEncoding(Method _Nonnull m) 
class_addMethod(Class _Nullable cls, SEL _Nonnull name, IMP _Nonnull imp, 
                const char * _Nullable types)
```
**结果：**
![](https://upload-images.jianshu.io/upload_images/2049519-724aeab1f04c4a76.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
Hook之后的效果
到这里区域限制的突破已经完成了。
### Question4
Q4:
使用过程中发现其他地区TikTok都能下载视频，日区TikTok不能😓
![](https://upload-images.jianshu.io/upload_images/2049519-a21bc5a3677c7400.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
Error 提示
T4:
使用的是同一部手机，只Hook了`carrier_region`和`mcc_mnc`，出现了下载限制问题，那肯定是地区版权策略导致的(11区对版权的重视，佩服了)。
A4:
点开分享按钮
![](https://upload-images.jianshu.io/upload_images/2049519-2b2b7cb9120a89e7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
分享界面
发现判断是否有下载权限是发生在按钮点击之前的。考虑是在请求返回的JSON数据中存储的flag，然后把这个flag传给`AWEAwemeShareViewController`。
使用`Reveal`对界面分析，发现TableView的Cell类名是`AWEFeedViewCell`，然后查找class-dump出的`AWEFeedViewCell.h`，有一个可疑的方法是`- (void)configWithModel:(id)arg1;`
使用`MDMethodTrace`进行方法跟踪，确认了方法被调用，同时`arg1`的类型是`AWEAwemeModel`，这个Model里又发现了可疑的属性`@property(nonatomic, assign) BOOL preventDownload;`，意思是**禁止下载**。
**代码编写：**
```
// MARK: - AWEAwemeModel
CHDeclareClass(AWEAwemeModel)
CHMethod1(void, AWEAwemeModel, setPreventDownload, BOOL, arg1) {
    arg1 = ![UserDefaults boolForKey:HookDownLoad];
    CHSuper1(AWEAwemeModel, setPreventDownload, arg1);
}
CHConstructor {
    CHLoadLateClass(AWEAwemeModel);
    CHHook1(AWEAwemeModel, setPreventDownload);
}
```
**效果：**
![](https://upload-images.jianshu.io/upload_images/2049519-932696752fbe3c62.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
Hook preventDownload
下载按钮没被禁用了！怀着激动的心情点下去！
![](https://upload-images.jianshu.io/upload_images/2049519-36da113cea725692.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
下载出错
WTF !!!
![](https://upload-images.jianshu.io/upload_images/2049519-39446992ad1949b4.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/391/format/webp)
image
**继续:**
对比日区和其他区的`AWEAwemeModel`。发现`AWEAwemeModel`的某一个数据结构是这个样的
```
@interface AWEURLModel
@property(retain, nonatomic) NSArray *originURLList;
@end
@interface AWEVideoModel
@property(readonly, nonatomic) AWEURLModel *playURL;
@property(readonly, nonatomic) AWEURLModel *downloadURL;
@end
@interface AWEAwemeModel
@property(nonatomic, assign) BOOL preventDownload;
@property(retain, nonatomic) AWEVideoModel *video;
@end
```
一顿分析得到日区的`downloadURL`只有两个接口，不包含视频地址。其他能下载的地区，`downloadURL`有四个接口，前两个为视频地址。进一步发现`playURL`和`downloadURL`的参数一直。直接尝试将`playURL`赋值给`downloadURL`。
**代码编写：**
```
// MARK: - AWEAwemeModel
CHDeclareClass(AWEAwemeModel)
CHMethod1(void, AWEAwemeModel, setPreventDownload, BOOL, arg1) {
    arg1 = ![UserDefaults boolForKey:HookDownLoad];
    CHSuper1(AWEAwemeModel, setPreventDownload, arg1);
}
CHMethod1(void, AWEAwemeModel, setVideo, AWEVideoModel *, arg1) {
    BOOL isHookDownLoad = [UserDefaults boolForKey:HookDownLoad];
    if (isHookDownLoad) {
        arg1.downloadURL.originURLList = arg1.playURL.originURLList;
    }
    CHSuper1(AWEAwemeModel, setVideo, arg1);
}
CHConstructor {
    CHLoadLateClass(AWEAwemeModel);
    CHHook1(AWEAwemeModel, setPreventDownload);
    CHHook1(AWEAwemeModel, setVideo);
}
```
再次运行，成功下载日区TikTok视频。
### Question5
Q5:
视频down下来发现有水印？
T5:
对比原地址，发现原视频是没有水印的，那么水印就是在下载完成后添加了的。
![](https://upload-images.jianshu.io/upload_images/2049519-5743494e875046b7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
TargetApp
目录搜索watermark，验证了猜想。
在头文件中，发现了带watermark名称的类。
![](https://upload-images.jianshu.io/upload_images/2049519-a927cac63f0677fc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/686/format/webp)
WaterMark
最终发现`AWEDynamicWaterMarkExporter`这个类的`+ (id)watermarkLogoImageArray;`返回了对应的水印图片。
**代码编写**
```
#pragma mark WaterMark
CHDeclareClass(AWEDynamicWaterMarkExporter)
CHOptimizedClassMethod0(self, NSArray *, AWEDynamicWaterMarkExporter, watermarkLogoImageArray) {
    BOOL isHookWaterMark = [UserDefaults boolForKey:HookWaterMark];
    if (isHookWaterMark) {
        return @[];
    }
    return CHSuper0(AWEDynamicWaterMarkExporter, watermarkLogoImageArray);
}
CHConstructor {
    CHLoadLateClass(AWEDynamicWaterMarkExporter);
    CHClassHook0(AWEDynamicWaterMarkExporter, watermarkLogoImageArray);
}
```
### 总结
整个逆向过程中，完整的Hook代码并不复杂，开发工作也是站在巨人的肩膀上完成的，草草几行就能完成功能逆向。
他是令人振奋，因为最终证明了你的逆向想法是对的，通往成功的路不只有一条，切入点可能不一样，思路可能不一样，方法可能不一样，但是都能成功。
> 
作者：酱油白切鸡
链接：https://www.jianshu.com/p/9778959a8c24
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
