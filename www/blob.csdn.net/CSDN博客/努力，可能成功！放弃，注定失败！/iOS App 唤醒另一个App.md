# iOS App 唤醒另一个App - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月13日 11:05:16[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：755
网上也有讲这块的，感觉讲得都不是很好。而且有一些细节根本没有讲清楚。这里重写整理一下相关知识点。
## 主要内容
- URL Scheme 是什么？
- 项目中关键的配置
- 注意事项
## URL Scheme 是什么？
`iOS`有个特性就是应用将其自身”绑定”到一个自定义 `URL scheme` 上，该 `scheme`用于从浏览器或其他应用中启动本应用。常见的分享到第三方之间的跳转都是基于`Scheme`的。
**通过对比网页链接来理解`iOS` 上的 `URL Schemes`，应该就容易多了。**
- `URL`，我们都很清楚，`http://www.apple.com`就是个`URL`，我们也叫它链接或网址；
- `Schemes`，表示的是一个 `UR`L 中的一个位置——最初始的位置，即 `://`之前的那段字符。比如
`http://www.apple.com`这个网址的`Schemes`是 **http**。
根据我们上面对`URL Schemes`的使用，我们可以很轻易地理解，在以本地应用为主的 iOS 上，我们可以像定位一个网页一样，用一种特殊的
`URL` 来定位一个应用甚至应用里某个具体的功能。而定位这个应用的，就应该这个应用的`URL 的 Schemes` 部分，也就是开头儿那部分。比如短信，就是
`sms:`
你可以完全按照理解一个网页的 **URL ——也就是它的网址——的方式来理解一个 iOS 应用的 URL，拿苹果的网站和 iOS 上的微信来做个简单对比：**
||网页（苹果）|iOS 应用（微信）|
|----|----|----|
|网站首页/打开应用|[http://www.apple.com](http://www.apple.com)|weixin://|
|子页面/具体功能|[http://www.apple.com/mac/（Mac页面）](http://www.apple.com/mac/%EF%BC%88Mac%E9%A1%B5%E9%9D%A2%EF%BC%89)|weixin://dl/moments（朋友圈）|
**关于基础概念性的就讲这么多**
## 项目中关键的配置
### 在项目`Info`的`Url Type`中配置（**被唤起端**）
> 
`Url Type`,表示可以被哪些带前缀Url打开,而不是可以打开以这些前缀开头的app。
![](http://upload-images.jianshu.io/upload_images/664334-1e373d2a61fad1d7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 说明
- URL identifier只是一个标示符，随意填写，建议写成：`com.*.*`反转域名的方法保证该名字的唯一性。
- URL Scheme就是你用来通信的命令前缀,用来定位一个应用。
### 在`Plist`文件中配置
![](http://upload-images.jianshu.io/upload_images/664334-b76eec163e50ba46.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 注意：`URL Schemes` 是一个数组，允许应用定义多个 `URL schemes`。
### 接收到唤起如何处理
在代理方法`- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation`中判断唤起的来源`source app`,根据`Url`所携带的参数进行不同的操作。比如跳转到制定的页面，相关的逻辑处理等等.
```
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url 
        sourceApplication:(NSString *)sourceApplication annotation:(id)annotation 
{ 
  NSLog(@"Calling Application Bundle ID: %@", sourceApplication); 
  NSLog(@"URL scheme:%@", [url scheme]); 
  NSLog(@"URL query: %@", [url query]); 
// Customer Code
  return YES; 
}
```
> 
以上配置是在被唤起应用中配置的。
### 唤起端
一般情况下，唤起端可以直接调用`appDelegate`的代理方法去唤醒其他应用。
```
- (void)awakeOtherApp
{ 
  NSString *customURL = @"otherApp://"; 
  if ([[UIApplication sharedApplication] 
    canOpenURL:[NSURL URLWithString:customURL]]) 
  { 
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:customURL]]; 
  } 
  else 
  { 
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"URL error" 
                          message:[NSString stringWithFormat: 
                            @"No custom URL defined for %@", customURL] 
                          delegate:self cancelButtonTitle:@"Ok" 
                          otherButtonTitles:nil]; 
    [alert show]; 
  } 
}
```
- 这里需要注意一下在`iOS9`以后，唤起端需要配置一下
> 
`LSApplicationQueriesSchemes`.`iOS9`之后需要，`iOS9`之后提高了`app`的安全性，需要给出一个类似白名单的东西，在白名单里面的才能打开`app`。不然报错：** -canOpenURL: failed for URL: "OpenAppTest://mark?id=007" - error: "This app is not allowed to query for scheme OpenAppTest"**
![](http://upload-images.jianshu.io/upload_images/664334-a6a13ba493d4b249.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 注意事项
通过上面的方法可以唤醒其他的应用，简单总结下一些注意事项。
- URL Scheme 其实就是一个app应用的唯一标志。通过它来确定打开那个应用。
- 一定要分清哪些配置在哪方配置，被唤醒与唤醒。
- iOS9之后需要在唤起端加入`LSApplicationQueriesSchemes`千万不能忘。
- 还有一个问题还没解决，如何再次回到唤醒应用的界面。这个需要参考下官方的`XCallbackURL`。听说有些复杂，有空再看看。
文／纸简书生（简书作者）
原文链接：http://www.jianshu.com/p/7a308f5b89fc
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
