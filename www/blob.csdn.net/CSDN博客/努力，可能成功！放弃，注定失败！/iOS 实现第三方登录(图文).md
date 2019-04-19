# iOS 实现第三方登录(图文) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月16日 19:09:18[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1393
##### 前言: 首先你要明白, 所谓的第三方登录，就是利用用户在第三方平台上已有的账号来快速完成自己应用的登录或者注册的功能。而这里的第三方平台，一般是已经有大量用户的平台，如国内的新浪微博、QQ空间，微信，外国的Facebook、twitter等等。第三方登录不是一个具体的接口，而是一种思想或者一套步骤。这种集成三方的东西, 都非常简单,可能会有坑, 填上就好, 这篇文章以微博为例, 其实都是一样的. 步骤或繁或简单, 习惯就好.
###### 1. 首先在微博开放平台注册成为开发者[http://open.weibo.com](http://open.weibo.com)
###### 2. 创建应用
![](http://upload-images.jianshu.io/upload_images/1506501-f61c254efed426c3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建应用
###### 3. 完善应用信息, 这里需要注意的是Bundle ID需要与应用一致, 同时记录appKey 项目中会用到
###### 4. 在应用信息-高级信息中
![](http://upload-images.jianshu.io/upload_images/1506501-d508cf1e25b7071f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
回调地址
![](http://upload-images.jianshu.io/upload_images/1506501-1ff8768b99d76b01.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
与上方的保持一致---这段代码后面后使用到
###### 5. 填写URL Types
![](http://upload-images.jianshu.io/upload_images/1506501-d13034e728dcb801.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
URL Types
###### 6. 配置info.plist同时加入白名单
![](http://upload-images.jianshu.io/upload_images/1506501-ed445a0e3c5af39a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
info.plist
```xml
<array>
  <string>sinaweibohd</string>
  <string>sinaweibo</string>
  <string>weibosdk</string>
  <string>weibosdk2.5</string>
 </array>
```
###### 7. 导入SDK.
7.1 使用`cocoapods`你可以这样
```php
```php
pod "WeiboSDK", :git => "https://github.com/sinaweibosdk/weibo_ios_sdk.git"
```
```
7.2 或者直接下载拖入工程
###### 8. 代码
```
// AppDelegate中
#import <WeiboSDK.h>
#define WB_iOS_Key @""
@interface AppDelegate ()<WeiboSDKDelegate>
```
```
```
/**
     *  1. 设置WeiboSDK的调试模式
     *  2. 注册
     */
    [WeiboSDK enableDebugMode:YES];
    [WeiboSDK registerApp:WB_iOS_Key];
```
```
```
#pragma mark - WeiboSDKDelegate
- (void)didReceiveWeiboRequest:(WBBaseRequest *)request
{
}
// 根据自己的需求写回调
// 这是微博Demo提供的代码
- (void)didReceiveWeiboResponse:(WBBaseResponse *)response
{
if ([response isKindOfClass:WBSendMessageToWeiboResponse.class])
    {
        NSString *title = NSLocalizedString(@"发送结果", nil);
        NSString *message = [NSString stringWithFormat:@"%@: %d\\n%@: %@\\n%@: %@", NSLocalizedString(@"响应状态", nil), (int)response.statusCode, NSLocalizedString(@"响应UserInfo数据", nil), response.userInfo, NSLocalizedString(@"原请求UserInfo数据", nil),response.requestUserInfo];
        WBSendMessageToWeiboResponse* sendMessageToWeiboResponse = (WBSendMessageToWeiboResponse*)response;
        NSString* accessToken = [sendMessageToWeiboResponse.authResponse accessToken];
        if (accessToken)
        {
            self.wbtoken = accessToken;
        }
        NSString* userID = [sendMessageToWeiboResponse.authResponse userID];
        if (userID) {
            self.wbCurrentUserID = userID;
        }
    }
    else if ([response isKindOfClass:WBAuthorizeResponse.class])
    {
        NSString *title = NSLocalizedString(@"认证结果", nil);
        NSString *message = [NSString stringWithFormat:@"%@: %d\\nresponse.userId: %@\\nresponse.accessToken: %@\\n%@: %@\\n%@: %@", NSLocalizedString(@"响应状态", nil), (int)response.statusCode,[(WBAuthorizeResponse *)response userID], [(WBAuthorizeResponse *)response accessToken],  NSLocalizedString(@"响应UserInfo数据", nil), response.userInfo, NSLocalizedString(@"原请求UserInfo数据", nil), response.requestUserInfo];
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                        message:message
                                                       delegate:nil
                                              cancelButtonTitle:NSLocalizedString(@"确定", nil)
                                              otherButtonTitles:nil];
        self.wbtoken = [(WBAuthorizeResponse *)response accessToken];
        self.wbCurrentUserID = [(WBAuthorizeResponse *)response userID];
        self.wbRefreshToken = [(WBAuthorizeResponse *)response refreshToken];
        [alert show];
    }
}
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    return [WeiboSDK handleOpenURL:url delegate:self];
}
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    return [WeiboSDK handleOpenURL:url delegate:self ];
}
```
###### 9. 写个登陆按钮登陆
```
- (void)loginClick
{
    [[[[UIApplication sharedApplication] delegate] window] makeKeyWindow];
    WBAuthorizeRequest *request = [WBAuthorizeRequest request];
    // 前面提到的
    request.redirectURI = kRedirectURI;
    request.scope = @"all";
    request.userInfo = @{@"SSO_From": @"SendMessageToWeiboViewController",
                         @"Other_Info_1": [NSNumber numberWithInt:123],
                         @"Other_Info_2": @[@"obj1", @"obj2"],
                         @"Other_Info_3": @{@"key1": @"obj1", @"key2": @"obj2"}};
    [WeiboSDK sendRequest:request];
}
```
###### 10. 完成图- 请继续往下看
![](http://upload-images.jianshu.io/upload_images/1506501-c6d39df38984aca9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
webView
![](http://upload-images.jianshu.io/upload_images/1506501-74e199565441f654.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
客户端
![](http://upload-images.jianshu.io/upload_images/1506501-a10687220c53c6f5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
result.png
##### 状态0 代表成功. 如果回忆没有错基本就成功了. O.-
###### 11. 你的应用是否具备独立账户系统？什么意思呢? 就是自带注册登陆功能不? 一般都是有自己的用户系统的.
基本就是要数据，不要功能, 使用第三方登录只是为了拥有更多用户, 大致的思路你可以这样理解
###### 12. 解释
> 
1、用户触发第三方登录事件
2、请求授权用户的资料（这个过程中可能涉及授权操作）
3、如果回调成功代入你应用的Login流程
4、否则错误进行错误处理
5、Login时客户端发送用户资料中的用户ID给服务端
6、服务端判定用户是已注册用户，则引导用户进入系统，否则返回特定错误码
##### 如果文中出现错误, 请联系我, 会及时修正.
[iOS 走进Facebook POP的世界 -- 简书](http://www.jianshu.com/p/0bc2127692e5)
[Swift版本仿网易云音乐播放音乐动画效果-- 简书](http://www.jianshu.com/p/23b0fa948bcf)
[三分钟教你把代码托管到Github -- 简书](http://www.jianshu.com/p/1dab64e7c9ba)
[Swift 很强大的图表库-Charts使用 -- 简书](http://www.jianshu.com/p/0e025931a49a)
[Swift版仿简书App淘宝App很友好弹出view效果 -- 简书](http://www.jianshu.com/p/01a420681ca9)
[iOS UICollectionView高级用法“长按自由移动cell重排 -- 简书](http://www.jianshu.com/p/31d07bf32d62)
[iOS仿微信App添加带箭头的view -- 简书](http://www.jianshu.com/p/260ca284fda0)
[iOS仿美团外卖饿了吗App点餐动画 -- 简书](http://www.jianshu.com/p/d0ae176bc40b)
[你要的Swift资料 -- 简书](http://www.jianshu.com/p/4a1978845eac)
#### 微博@夏天是个大人了欢迎你关注我你还可以加入我创建技术交流群: 498143780 与我交流. 欢迎你加入
文／夏天然后（简书作者）
原文链接：http://www.jianshu.com/p/b8b0e085b074
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
