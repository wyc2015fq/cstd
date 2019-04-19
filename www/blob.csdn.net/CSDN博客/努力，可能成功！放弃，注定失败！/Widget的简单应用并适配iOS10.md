# Widget的简单应用并适配iOS10 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月24日 08:46:56[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：6938
widget这个小插件不知道有多少人习惯使用？又或者有多少使用iphone手机的用户知道这东西的存在？好了，不说废话了；既然公司有这需求，小编也只能去研究了！下面来和大家介绍小编研究成果！查询了网上的相关内容，没有发现什么特别全面详细的文章！只能自己查官方文档喽！
官方对widget的解释：[App Extension Programming Guide: Today](https://developer.apple.com/library/content/documentation/General/Conceptual/ExtensibilityPG/Today.html#//apple_ref/doc/uid/TP40014214-CH11-SW1)
> 
People view Today widgets in the Today area of Notification Center. Because people configure the Today area so that it displays the information they value most, it works well to approach the design of your widget with the goal of earning a place among the
 user’s most important items.
extension是iOS8新开放的一种对几个固定系统区域的扩展机制，extension并不是一个独立的app，它有一个包含在app bundle中的独立bundle，extension的bundle后缀名是.appex；需要依赖于containning app。
![](http://upload-images.jianshu.io/upload_images/1109379-5036409a3b3192d6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iOS 10 widget
点击“编辑”可是添加其他app的widget。
#### 如何创建widget？
- 创建一个工程，在该工程里添加targets:
![](http://upload-images.jianshu.io/upload_images/1109379-651007d93e537c11.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建widget
- 创建成功之后的项目结构：
![](http://upload-images.jianshu.io/upload_images/1109379-ca8c6ea720d63533.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
项目结构
- iOS 10和iOS 10之前的界面对比：
![](http://upload-images.jianshu.io/upload_images/1109379-261bed459cccfd2a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iOS 10
![](http://upload-images.jianshu.io/upload_images/1109379-d736d5d7a826ca52.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iOS 10 之前
#### 搭建简单的交互界面
- 文件配置：
系统生成的info.plist文件默认是使用Storyboard 实现的界面；如果你想使用代码实现是界面的搭建，需更改这个配置文件：
> 
The Xcode Today template provides default header and implementation files for the principal class (named TodayViewController), an Info.plist file, and an interface file (that is, a storyboard or xib file).
By default, the Today template supplies the following Info.plist keys and values (shown here for an OS X target):
<key>NSExtension</key>
```xml
<dict>
      <key>NSExtensionPointIdentifier</key>
      <string>com.apple.widget-extension</string>
      <key>NSExtensionPrincipalClass</key>
      <string>TodayViewController</string>
  </dict>
```
If you use a custom view controller subclass, use the custom class name to replace the TodayViewController value for the NSExtensionPrincipalClass key.
- 
NSExtensionMainStoryboard：MainInterface(你Storyboard的名称)
![](http://upload-images.jianshu.io/upload_images/1109379-8a660b1f0cde793b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
NSExtensionPrincipalClass:TodayViewController（你widget控制器的名称）
![](http://upload-images.jianshu.io/upload_images/1109379-877cb458a7c640e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
NSExtensionAttributes：这是一个描述扩展点具体属性的字典，就像照片编辑扩展中的PHSupportedMediaTypes一样。
NSExtensionPrincipalClass：这是扩展模板创建的主体视图控制器类，比如TodayViewController。当载体应用程序（host app）调用扩展时，扩展点会实例化这个类。
NSExtensionMainStoryboard（只适用于iOS）：扩展默认的Storyboard文件，一般名为MainInterface。
`注：`本文以代码为例！
#### 方法实现：
- 界面的搭建（<iOS 10）：
```
-(void)viewDidLoad {
  [super viewDidLoad];
  WGHeaderView *headerView = [[WGHeaderView alloc] initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, 110)];
  headerView.delegate_ = self;
  [self.view addSubview: headerView];
  self.headerView = headerView;
}
// 设置界面的高度
-(void)viewWillAppear:(BOOL)animated{
  [super viewWillAppear:animated];
  self.preferredContentSize = CGSizeMake([UIScreen mainScreen].bounds.size.width, 110);
}
```
- 更新widget视图：（demo目前只是按钮 不需要更新）
```
-(void)widgetPerformUpdateWithCompletionHandler:(void (^)(NCUpdateResult))completionHandler {
//    NCUpdateResultNewData   新的内容需要重新绘制视图
//    NCUpdateResultNoData    部件不需要更新
//    NCUpdateResultFailed    更新过程中发生错误
  completionHandler(NCUpdateResultNoData);
}
```
运行程序：
![](http://upload-images.jianshu.io/upload_images/1109379-9b1c44a675fe25a7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
运行效果图
因此还需要实现：
```cpp
```cpp
-(UIEdgeInsets)widgetMarginInsetsForProposedMarginInsets:(UIEdgeInsets)defaultMarginInsets{
  return UIEdgeInsetsMake(0, 10, 0, 10);
}
```
```
`注意：`该方法在iOS 10之后就被淘汰了！
- 
实现点击按钮跳转：
配置URL schemes：
![](http://upload-images.jianshu.io/upload_images/1109379-c0ad09d68592ad17.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
在按钮点击的方法里实现：（此处小编用的代理）
```
NSString *urlStr = [NSString stringWithFormat:@"medicalWdget://%li",index];
  NSURL *url = [NSURL URLWithString:urlStr];
  [self.extensionContext openURL:url completionHandler:^(BOOL success) {
  }];
```
目前就可以跳转到app了！当然是四个按钮，你需要再AppDelegate里面进行处理：
```
-(BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation{
  if ([url.scheme isEqualToString:@"medicalWdget"]) {
      [[NSNotificationCenter defaultCenter] postNotificationName:@"ExtenicationNotification" object:url.host];
  }
  return YES;
}
// ios9 之后
-(BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<NSString *,id> *)options{
  if ([url.scheme isEqualToString:@"medicalWdget"]) {
      [[NSNotificationCenter defaultCenter] postNotificationName:@"ExtenicationNotification" object:url.host];
  }
![72BBF230-83EB-4650-BE27-EE0FBBAAD35F.png](http://upload-images.jianshu.io/upload_images/1109379-2530dc667076faee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
  return YES;
}
```
然后处理相应的通知，判断url.host点击的第几个按钮！
#### 适配iOS 10：
- 
NCWidgetDisplayMode
```
NCWidgetDisplayModeCompact, // Fixed height
  NCWidgetDisplayModeExpanded, // Variable height
```
需要再设置高度之前设置该属性；
```
-(void)viewWillAppear:(BOOL)animated{
  [super viewWillAppear:animated];
  if ([[UIDevice currentDevice].systemVersion doubleValue] >= 10.0) {
      self.extensionContext.widgetLargestAvailableDisplayMode = NCWidgetDisplayModeCompact;
  }
  self.preferredContentSize = CGSizeMake([UIScreen mainScreen].bounds.size.width, 110);
}
```
模式改变后会执行下面这个方法：
```
-(void)widgetActiveDisplayModeDidChange:(NCWidgetDisplayMode)activeDisplayMode withMaximumSize:(CGSize)maxSize{
  if (activeDisplayMode == NCWidgetDisplayModeCompact) {
      NSLog(@"maxSize-%@",NSStringFromCGSize(maxSize));// maxSize-{359, 110}
  }else{
      NSLog(@"maxSize-%@",NSStringFromCGSize(maxSize));// maxSize-{359, 616}
  }
}
```
两种设计风格：
![](http://upload-images.jianshu.io/upload_images/1109379-c2673b8b668adcef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPhone 7的界面效果
![](http://upload-images.jianshu.io/upload_images/1109379-6532baa71f30c78f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iPhone 6（iOS8.4）的界面效果
#### 扩展：
![](http://upload-images.jianshu.io/upload_images/1109379-7814ea36bcdff948.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
`注意：`有人看到这种界面可能会想到tableview实现，开始小编也是这样想的。可是实现起来发现布局都是乱的（具体原因没有找出来，如哪位研究出来了，麻烦交流一下）！所以建议大家使用UIView就可以了！
网络请求数据：
```
@try {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            NSMutableDictionary *param = [[NSMutableDictionary alloc] init];
            [param setObject:@"gettopics" forKey:@"action"];
            [param setObject:@"135644452322" forKey:@"EchoToken"];
            [param setValue:@"data" forKeyPath:@"type"];
            [param setValue:[NSString stringWithFormat:@"%i",widgetCount] forKeyPath:@"pagesize"];
            [param setValue:@"1" forKeyPath:@"pageindex"];
            [param setValue:@"" forKeyPath:@"username"];
            [param setValue:@"cF54141DC1FA8E736B45244428874CE46==" forKeyPath:@"token"];
            NSDictionary *headers = @{@"Content-Type":@"application/json; charset=utf-8",
                                      @"Accept":@"application/json"
                                      };
            NSURLSession *session = [NSURLSession sharedSession];
            NSURL *url = [NSURL URLWithString:[@"v1/api.ashx?action=" stringByAppendingString:@"gettopics"]];
            NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:10.0];
            request.HTTPBody = [[NSString stringWithFormat:@"type=data&pagesize=%i&pageindex=1",widgetCount ] dataUsingEncoding:NSUTF8StringEncoding];
            request.allHTTPHeaderFields = headers;
            request.HTTPMethod = @"POST";
            NSError *error = nil;
            NSData *jsonData = [NSJSONSerialization dataWithJSONObject:param options:NSJSONWritingPrettyPrinted error:&error];
            request.HTTPBody = jsonData;
            NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
                NSError *jsonError = nil;
                NSDictionary *jsonDict = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingAllowFragments error:&jsonError];
                if (!jsonError) {
                    NSMutableArray *mutArr = [NSMutableArray array];
                    for (NSDictionary *dict in jsonDict[@"list"]) {
                        WGTopic *topic = [[WGTopic alloc] initWithDict:dict];
                        [mutArr addObject:topic];
                    }
                    self.hotTopicArr = mutArr;
                }
            }];
            // 启动任务
            [dataTask resume];
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.view layoutIfNeeded];
            });
        });
    } @catch (NSException *exception) {
    } @finally {
    }
```
如果是http请求需要配置info.plist文件：
![](http://upload-images.jianshu.io/upload_images/1109379-d60a35c13c2a9fa2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
`注意：`从2017年1月起所有请求需要时https 的，否则审核就会被拒！web连接可以是http的，但是也需要配置！
#### APP数据互通：
![](http://upload-images.jianshu.io/upload_images/1109379-1db1ed81441b4f3c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
通过一下沙盒存储方法存储数据和获取数据！
#### 与app使用相同的方法文件：
将公共的文件打包成framework之后，进行相关的配置；
#### 调试：
选择不同的项目运行，在相应的项目里的断点就会起作用！
![](http://upload-images.jianshu.io/upload_images/1109379-0bffeafcadc310e6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
本文[demo](https://github.com/dreamGrilCode/MyWidgetDemo)
#### 补充：widget的上线也是需要单独申请APP ID的 需要配置证书和Provisioning Profiles文件！
﻿没有配置相关证书时：
![](http://upload-images.jianshu.io/upload_images/1109379-f541070c4dc52602.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
配置证书及描述文件：（列举一些）
![](http://upload-images.jianshu.io/upload_images/1109379-4b77d6102aee3d2c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1109379-b4babaffbc15ffdf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
证书与描述文件配置好之后：
![](http://upload-images.jianshu.io/upload_images/1109379-b3235e1abb5c84ef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
感谢您的阅读，希望对您有所帮助！
文／芝麻绿豆（简书作者）
原文链接：http://www.jianshu.com/p/42516ee26a45
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
