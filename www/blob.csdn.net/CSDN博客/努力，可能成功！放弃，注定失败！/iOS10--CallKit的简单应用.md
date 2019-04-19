# iOS10--CallKit的简单应用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月09日 17:18:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4404
> 
CallKit 这个开发框架，能够让语音或视讯电话的开发者将 UI 界面整合在 iPhone 原生的电话 App 中.将允许开发者将通讯 App 的功能内建在电话 App 的“常用联络资讯”，以及“通话记录”，方便用户透过原生电话 App，就能直接取用这些第三方功能;允许用户在通知中心就能直接浏览并回覆来电，来电的画面也将整合在 iOS 原生的 UI 里，总体来说，等于让 iOS 原本单纯用来打电信电话的“电话”功能，能够结合众多第三方语音通讯软件，具备更完整的数码电话潜力。CallKit 也拓展了在
 iOS 8 就出现的 App Extensions 功能，可以让用户在接收来电时，在原生电话 App 中就透过第三方 App 辨识骚扰电话（例如诈骗）.
#### 如何创建一个call项目？
![](http://upload-images.jianshu.io/upload_images/1109379-964b45562525bd78.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1109379-b07714dc5a209fb4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1109379-136bd297938a1118.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1109379-6ce882e54664ffba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
出现下图结构说明创建成功：
![](http://upload-images.jianshu.io/upload_images/1109379-e958e50dd0a59d47.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
`注意：`cccccccccalldemo.xcdatamodeld必须创建；
#### 代码的实现：
- sms：或者是sms://:发送短信；
- tel： 或者是tel://:打电话
- telprompt:或者是 telprompt://: 打电话；
- mailto:发送邮件；
- http：或者是 http://: 浏览网址；
##### 打电话的按钮
```
NSMutableString * str=[[NSMutableString alloc] initWithFormat:@"tel:%@",self.noTextField.text];
    [self.callWebview loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:str]]];
    if (!self.callWebview.subviews) {
        [self.view addSubview:_callWebview];
    }
```
##### 检查授权:
```
CXCallDirectoryManager *manager = [CXCallDirectoryManager sharedInstance];
    // 获取权限状态
    [manager getEnabledStatusForExtensionWithIdentifier:@"com.tq.cccccccccalldemo.CallDirectoryExtension" completionHandler:^(CXCallDirectoryEnabledStatus enabledStatus, NSError * _Nullable error) {
        if (!error) {
            NSString *title = nil;
            if (enabledStatus == CXCallDirectoryEnabledStatusDisabled) {
                /*
                 CXCallDirectoryEnabledStatusUnknown = 0,
                 CXCallDirectoryEnabledStatusDisabled = 1,
                 CXCallDirectoryEnabledStatusEnabled = 2,
                 */
                title = @"未授权，请在设置->电话授权相关权限";
            }else if (enabledStatus == CXCallDirectoryEnabledStatusEnabled) {
                title = @"授权";
            }else if (enabledStatus == CXCallDirectoryEnabledStatusUnknown) {
                title = @"不知道";
            }
            UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"提示"
                                                                           message:title
                                                                    preferredStyle:UIAlertControllerStyleAlert];
            UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
                                                                  handler:^(UIAlertAction * action) {}];
            [alert addAction:defaultAction];
            [self presentViewController:alert animated:YES completion:nil];
        }else{
            UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"提示"
                                                                           message:@"有错误"
                                                                    preferredStyle:UIAlertControllerStyleAlert];
            UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
                                                                  handler:^(UIAlertAction * action) {}];
            [alert addAction:defaultAction];
            [self presentViewController:alert animated:YES completion:nil];
        }
    }];
```
##### CallDirectoryHandler文件的实现方法：
![](http://upload-images.jianshu.io/upload_images/1109379-5bbd998c1698e0c7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
自动生成的方法
`注意：`电话号码前要加区号：+86；
```
/**
 添加黑名单：无法接通
 */
- (BOOL)addBlockingPhoneNumbersToContext:(CXCallDirectoryExtensionContext *)context
/**
 添加信息标识
 */
- (BOOL)addIdentificationPhoneNumbersToContext:(CXCallDirectoryExtensionContext *)context
```
`说明：`将项目运行到真机之后，还需要在“设置->电话”设置应用的权限；
##### 判断是否有权限：
```
CXCallDirectoryManager *manager = [CXCallDirectoryManager sharedInstance];
    // 获取权限状态
    [manager getEnabledStatusForExtensionWithIdentifier:@"com.tq.cccccccccalldemo.CallDirectoryExtension" completionHandler:^(CXCallDirectoryEnabledStatus enabledStatus, NSError * _Nullable error) {
        if (!error) {
            NSString *title = nil;
            if (enabledStatus == CXCallDirectoryEnabledStatusDisabled) {
                /*
                 CXCallDirectoryEnabledStatusUnknown = 0,
                 CXCallDirectoryEnabledStatusDisabled = 1,
                 CXCallDirectoryEnabledStatusEnabled = 2,
                 */
                title = @"未授权，请在设置->电话授权相关权限";
            }else if (enabledStatus == CXCallDirectoryEnabledStatusEnabled) {
                title = @"授权";
            }else if (enabledStatus == CXCallDirectoryEnabledStatusUnknown) {
                title = @"不知道";
            }
            UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"提示"
                                                                           message:title
                                                                    preferredStyle:UIAlertControllerStyleAlert];
            UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
                                                                  handler:^(UIAlertAction * action) {}];
            [alert addAction:defaultAction];
            [self presentViewController:alert animated:YES completion:nil];
        }else{
            UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"提示"
                                                                           message:@"有错误"
                                                                    preferredStyle:UIAlertControllerStyleAlert];
            UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
                                                                  handler:^(UIAlertAction * action) {}];
            [alert addAction:defaultAction];
            [self presentViewController:alert animated:YES completion:nil];
        }
    }];
```
项目基本可以达到需求了！黑名单的号码打进来是在通话中，标记的号码显示标记的名字；
![](http://upload-images.jianshu.io/upload_images/1109379-8fa6c28506247606.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
标记的号码
> 
这时你会发现你只有第一次运行项目的号码设置才起作用，或者是去设置里面重新授权；显然这是不行的；我们需要实时更新号码：
```
CXCallDirectoryManager *manager = [CXCallDirectoryManager sharedInstance];
    [manager reloadExtensionWithIdentifier:@"com.tq.cccccccccalldemo.CallDirectoryExtension" completionHandler:^(NSError * _Nullable error) {
        if (error == nil) {
            UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"提示"
                                                                           message:@"更新成功"
                                                                    preferredStyle:UIAlertControllerStyleAlert];
            UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
                                                                  handler:^(UIAlertAction * action) {}];
            [alert addAction:defaultAction];
            [self presentViewController:alert animated:YES completion:nil];
        }else{
            UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"提示"
                                                                           message:@"更新失败"
                                                                    preferredStyle:UIAlertControllerStyleAlert];
            UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
                                                                  handler:^(UIAlertAction * action) {}];
            [alert addAction:defaultAction];
            [self presentViewController:alert animated:YES completion:nil];
        }
    }];
```
![](http://upload-images.jianshu.io/upload_images/1109379-585de43ef965487c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
文／芝麻绿豆（简书作者）
原文链接：http://www.jianshu.com/p/4acc7c02dc93
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
