# iOS9AdaptationTips - game3108的专栏 - CSDN博客
2016年12月07日 19:43:25[game3108](https://me.csdn.net/game3108)阅读数：804
个人分类：[iOS																[转载](https://blog.csdn.net/game3108/article/category/2926393)](https://blog.csdn.net/game3108/article/category/2844789)
非常好的ios9网络适配文章，这边记录一下：
https://github.com/ChenYilong/iOS9AdaptationTips#why%E4%BB%A5%E5%89%8D%E7%9A%84http%E4%B8%8D%E6%98%AF%E4%B9%9F%E8%83%BD%E7%94%A8%E5%90%97%E4%B8%BA%E4%BB%80%E4%B9%88%E8%A6%81%E7%94%A8ssltlsapple%E6%98%AF%E4%B8%8D%E6%98%AF%E5%8F%88%E5%9C%A8%E5%8F%8D%E4%BA%BA%E7%B1%BB
iOS9适配系列教程【中文在[页面下方](https://github.com/ChenYilong/iOS9AdaptationTips#1-demo1_ios9%E7%BD%91%E7%BB%9C%E9%80%82%E9%85%8D_ats%E6%94%B9%E7%94%A8%E6%9B%B4%E5%AE%89%E5%85%A8%E7%9A%84https)】
相关链接：[iOS10适配系列教程](https://github.com/ChenYilong/iOS10AdaptationTips)。
（截至2016年4月17日共有12篇，后续还将持续更新。更多iOS开发干货，欢迎关注 [微博@iOS程序犭袁](http://weibo.com/luohanchenyilong/) ）
# [](https://github.com/ChenYilong/iOS9AdaptationTips#中文快速导航)中文快速导航：
- [iOS9网络适配_ATS：改用更安全的HTTPS（见Demo1）](https://github.com/ChenYilong/iOS9AdaptationTips#1-demo1_ios9%E7%BD%91%E7%BB%9C%E9%80%82%E9%85%8D_ats%E6%94%B9%E7%94%A8%E6%9B%B4%E5%AE%89%E5%85%A8%E7%9A%84https)- [WHAT（什么是SSL/TLS？跟HTTP和HTTPS有什么关系）](https://github.com/ChenYilong/iOS9AdaptationTips#what%E4%BB%80%E4%B9%88%E6%98%AFssltls%E8%B7%9Fhttp%E5%92%8Chttps%E6%9C%89%E4%BB%80%E4%B9%88%E5%85%B3%E7%B3%BB)
- [WHY（以前的HTTP不是也能用吗？为什么要用SSL/TLS？Apple是不是又在反人类？）](https://github.com/ChenYilong/iOS9AdaptationTips#why%E4%BB%A5%E5%89%8D%E7%9A%84http%E4%B8%8D%E6%98%AF%E4%B9%9F%E8%83%BD%E7%94%A8%E5%90%97%E4%B8%BA%E4%BB%80%E4%B9%88%E8%A6%81%E7%94%A8ssltlsapple%E6%98%AF%E4%B8%8D%E6%98%AF%E5%8F%88%E5%9C%A8%E5%8F%8D%E4%BA%BA%E7%B1%BB)
- [HOW（如何适配？---弱弱地问下：加班要多久？）](https://github.com/ChenYilong/iOS9AdaptationTips#how%E5%A6%82%E4%BD%95%E9%80%82%E9%85%8D---%E5%BC%B1%E5%BC%B1%E5%9C%B0%E9%97%AE%E4%B8%8B%E5%8A%A0%E7%8F%AD%E8%A6%81%E5%A4%9A%E4%B9%85)- [第1种情况：HTTPS
 Only （只有HTTPS，所有情况下都使用ATS）](https://github.com/ChenYilong/iOS9AdaptationTips#1https-only-%E5%8F%AA%E6%9C%89https%E6%89%80%E6%9C%89%E6%83%85%E5%86%B5%E4%B8%8B%E9%83%BD%E4%BD%BF%E7%94%A8ats)
- [第2种情况：Mix & Match（混合）](https://github.com/ChenYilong/iOS9AdaptationTips#2mix--match%E6%B7%B7%E5%90%88)
- [第3种情况：Opt Out（禁用ATS）](https://github.com/ChenYilong/iOS9AdaptationTips#3-opt-out%E7%A6%81%E7%94%A8ats)
- [第4种情况：Opt
 Out With Exceptions（除特殊情况外，都不使用ATS）](https://github.com/ChenYilong/iOS9AdaptationTips#4-opt-out-with-exceptions%E9%99%A4%E7%89%B9%E6%AE%8A%E6%83%85%E5%86%B5%E5%A4%96%E9%83%BD%E4%B8%8D%E4%BD%BF%E7%94%A8ats)
- [Certificate Transparency](https://github.com/ChenYilong/iOS9AdaptationTips#certificate-transparency)
- [Q-A](https://github.com/ChenYilong/iOS9AdaptationTips#q-a)
- [iOS9新特性_更灵活的后台定位（见Demo2）](https://github.com/ChenYilong/iOS9AdaptationTips#2demo2_ios9%E6%96%B0%E7%89%B9%E6%80%A7_%E6%9B%B4%E7%81%B5%E6%B4%BB%E7%9A%84%E5%90%8E%E5%8F%B0%E5%AE%9A%E4%BD%8D)
- [企业级分发](https://github.com/ChenYilong/iOS9AdaptationTips#3%E4%BC%81%E4%B8%9A%E7%BA%A7%E5%88%86%E5%8F%91)- [iOS9以后，企业级分发ipa包将遭到与Mac上dmg安装包一样的待遇：默认不能安装，也不再出现“信任按钮”](https://github.com/ChenYilong/iOS9AdaptationTips#1-ios9%E4%BB%A5%E5%90%8E%E4%BC%81%E4%B8%9A%E7%BA%A7%E5%88%86%E5%8F%91ipa%E5%8C%85%E5%B0%86%E9%81%AD%E5%88%B0%E4%B8%8Emac%E4%B8%8Admg%E5%AE%89%E8%A3%85%E5%8C%85%E4%B8%80%E6%A0%B7%E7%9A%84%E5%BE%85%E9%81%87%E9%BB%98%E8%AE%A4%E4%B8%8D%E8%83%BD%E5%AE%89%E8%A3%85%E4%B9%9F%E4%B8%8D%E5%86%8D%E5%87%BA%E7%8E%B0%E4%BF%A1%E4%BB%BB%E6%8C%89%E9%92%AE)
- [iOS9以后，企业分发时可能存在：下载的ipa包与网页两者的
 bundle ID 无法匹配而导致下载失败的情况](https://github.com/ChenYilong/iOS9AdaptationTips#2-ios9%E4%BB%A5%E5%90%8E%E4%BC%81%E4%B8%9A%E5%88%86%E5%8F%91%E6%97%B6%E5%8F%AF%E8%83%BD%E5%AD%98%E5%9C%A8%E4%B8%8B%E8%BD%BD%E7%9A%84ipa%E5%8C%85%E4%B8%8E%E7%BD%91%E9%A1%B5%E4%B8%A4%E8%80%85%E7%9A%84-bundle-id-%E6%97%A0%E6%B3%95%E5%8C%B9%E9%85%8D%E8%80%8C%E5%AF%BC%E8%87%B4%E4%B8%8B%E8%BD%BD%E5%A4%B1%E8%B4%A5%E7%9A%84%E6%83%85%E5%86%B5)
- [iOS9以后，企业APP安装之后，在网络情况为Wi-Fi环境的时候，可能会出现无法验证应用的情况](https://github.com/ChenYilong/iOS9AdaptationTips#3-%E4%BC%81%E4%B8%9Aapp%E5%AE%89%E8%A3%85%E4%B9%8B%E5%90%8E%E5%9C%A8%E7%BD%91%E7%BB%9C%E6%83%85%E5%86%B5%E4%B8%BAwi-fi%E7%8E%AF%E5%A2%83%E7%9A%84%E6%97%B6%E5%80%99%E5%8F%AF%E8%83%BD%E4%BC%9A%E5%87%BA%E7%8E%B0%E6%97%A0%E6%B3%95%E9%AA%8C%E8%AF%81%E5%BA%94%E7%94%A8%E7%9A%84%E6%83%85%E5%86%B5%E5%87%BA%E7%8E%B0%E4%BB%A5%E4%B8%8B%E6%8F%90%E7%A4%BA)
- [Bitcode](https://github.com/ChenYilong/iOS9AdaptationTips#4bitcode)
- [iOS9
 URL Scheme 适配_引入白名单概念（见Demo3）](https://github.com/ChenYilong/iOS9AdaptationTips#5demo3---ios9-url-scheme-%E9%80%82%E9%85%8D_%E5%BC%95%E5%85%A5%E7%99%BD%E5%90%8D%E5%8D%95%E6%A6%82%E5%BF%B5)- [常见 URL Scheme](https://github.com/ChenYilong/iOS9AdaptationTips#%E5%B8%B8%E8%A7%81-url-scheme)
- [Q-A](https://github.com/ChenYilong/iOS9AdaptationTips#q-a-1)
- [iPad适配Slide Over
 和 Split View](https://github.com/ChenYilong/iOS9AdaptationTips#6-ipad%E9%80%82%E9%85%8Dslide-over-%E5%92%8C-split-view)
- [字体间隙变大导致
 UI 显示异常](https://github.com/ChenYilong/iOS9AdaptationTips#7%E5%AD%97%E4%BD%93%E9%97%B4%E9%9A%99%E5%8F%98%E5%A4%A7%E5%AF%BC%E8%87%B4-ui-%E6%98%BE%E7%A4%BA%E5%BC%82%E5%B8%B8)
- [升级
 Xcode7 后的崩溃与警告](https://github.com/ChenYilong/iOS9AdaptationTips#8%E5%8D%87%E7%BA%A7-xcode7-%E5%90%8E%E7%9A%84%E5%B4%A9%E6%BA%83%E4%B8%8E%E8%AD%A6%E5%91%8A)- [iOS9
 下使用 Masonry 会引起崩溃的一种情况](https://github.com/ChenYilong/iOS9AdaptationTips#ios9-%E4%B8%8B%E4%BD%BF%E7%94%A8-masonry-%E4%BC%9A%E5%BC%95%E8%B5%B7%E5%B4%A9%E6%BA%83%E7%9A%84%E4%B8%80%E7%A7%8D%E6%83%85%E5%86%B5)
- [Xcode
 升级后，旧的状态栏的样式设置方式会引起警告](https://github.com/ChenYilong/iOS9AdaptationTips#xcode-%E5%8D%87%E7%BA%A7%E5%90%8E%E6%97%A7%E7%9A%84%E7%8A%B6%E6%80%81%E6%A0%8F%E7%9A%84%E6%A0%B7%E5%BC%8F%E8%AE%BE%E7%BD%AE%E6%96%B9%E5%BC%8F%E4%BC%9A%E5%BC%95%E8%B5%B7%E8%AD%A6%E5%91%8A)- [Demo4---navigationController状态栏样式新的设置方法](https://github.com/ChenYilong/iOS9AdaptationTips#demo4---navigationcontroller%E7%8A%B6%E6%80%81%E6%A0%8F%E6%A0%B7%E5%BC%8F%E6%96%B0%E7%9A%84%E8%AE%BE%E7%BD%AE%E6%96%B9%E6%B3%95)
- [Xcode7
 在 debug 状态下也生成 .dSYM 文件引起的警告](https://github.com/ChenYilong/iOS9AdaptationTips#xcode7-%E5%9C%A8-debug-%E7%8A%B6%E6%80%81%E4%B8%8B%E4%B9%9F%E7%94%9F%E6%88%90-dsym-%E6%96%87%E4%BB%B6%E5%BC%95%E8%B5%B7%E7%9A%84%E8%AD%A6%E5%91%8A)
- [Xcode7
 无法使用 8.x 系统的设备调试，一运行就报错 there is an intenal API error](https://github.com/ChenYilong/iOS9AdaptationTips#xcode7-%E6%97%A0%E6%B3%95%E4%BD%BF%E7%94%A8-8x-%E7%B3%BB%E7%BB%9F%E7%9A%84%E8%AE%BE%E5%A4%87%E8%B0%83%E8%AF%95%E4%B8%80%E8%BF%90%E8%A1%8C%E5%B0%B1%E6%8A%A5%E9%94%99-there-is-an-intenal-api-error)
- [使用了
 HTML 的 iframe 元素可能导致无法从 Safari 跳转至 App](https://github.com/ChenYilong/iOS9AdaptationTips#%E4%BD%BF%E7%94%A8%E4%BA%86-html-%E7%9A%84-iframe-%E5%85%83%E7%B4%A0%E5%8F%AF%E8%83%BD%E5%AF%BC%E8%87%B4%E6%97%A0%E6%B3%95%E4%BB%8E-safari-%E8%B7%B3%E8%BD%AC%E8%87%B3-app)
- [iOS9锁屏控制台会打印警告](https://github.com/ChenYilong/iOS9AdaptationTips#ios9%E9%94%81%E5%B1%8F%E6%8E%A7%E5%88%B6%E5%8F%B0%E4%BC%9A%E6%89%93%E5%8D%B0%E8%AD%A6%E5%91%8A)
- [Xcode7 上传应用时提示
 ITMS-90535 Unable to publish iOS app with xxx SDK 的问题](http://stackoverflow.com/questions/32622899/itms-90535-unable-to-publish-ios-app-with-latest-google-signin-sdk)
- [在didFinishLaunchingWithOptions结束后还没有设置window的rootViewController会导致崩溃](https://github.com/ChenYilong/iOS9AdaptationTips#%E5%9C%A8didfinishlaunchingwithoptions%E7%BB%93%E6%9D%9F%E5%90%8E%E8%BF%98%E6%B2%A1%E6%9C%89%E8%AE%BE%E7%BD%AEwindow%E7%9A%84rootview%0Acontroller%E4%BC%9A%E5%AF%BC%E8%87%B4%E5%B4%A9%E6%BA%83)
- [Demo5、Demo6--- 搜索 API](https://github.com/ChenYilong/iOS9AdaptationTips#9demo5demo6----%E6%90%9C%E7%B4%A2-api)
- [iOS国际化问题：当前设备语言字符串返回有变化](https://github.com/ChenYilong/iOS9AdaptationTips#10ios%E5%9B%BD%E9%99%85%E5%8C%96%E9%97%AE%E9%A2%98%E5%BD%93%E5%89%8D%E8%AE%BE%E5%A4%87%E8%AF%AD%E8%A8%80%E5%AD%97%E7%AC%A6%E4%B8%B2%E8%BF%94%E5%9B%9E%E6%9C%89%E5%8F%98%E5%8C%96)
- [UITableView显示异常](https://github.com/ChenYilong/iOS9AdaptationTips#11uitableview%E6%98%BE%E7%A4%BA%E5%BC%82%E5%B8%B8)- [代码创建的
 tableView 无法隐藏 cell 分割线](https://github.com/ChenYilong/iOS9AdaptationTips#%E4%BB%A3%E7%A0%81%E5%88%9B%E5%BB%BA%E7%9A%84-tableview-%E6%97%A0%E6%B3%95%E9%9A%90%E8%97%8F-cell-%E5%88%86%E5%89%B2%E7%BA%BF)
- [reloadData 刷新失效](https://github.com/ChenYilong/iOS9AdaptationTips#reloaddata-%E5%88%B7%E6%96%B0%E5%A4%B1%E6%95%88)
- [基于
 HTTP/2 的新 APNs 协议](https://github.com/ChenYilong/iOS9AdaptationTips/blob/master/%E5%9F%BA%E4%BA%8EHTTP2%E7%9A%84%E5%85%A8%E6%96%B0APNs%E5%8D%8F%E8%AE%AE/%E5%9F%BA%E4%BA%8EHTTP2%E7%9A%84%E5%85%A8%E6%96%B0APNs%E5%8D%8F%E8%AE%AE.md#%E5%A6%82%E4%BD%95%E5%88%9B%E5%BB%BA-universal-push-notification-client-ssl-%E8%AF%81%E4%B9%A6)
# [](https://github.com/ChenYilong/iOS9AdaptationTips#english️️)English⬇️⬇️
For more infomation ，welcome to follow [my twitter](https://twitter.com/stevechen1010)
Reference：[iOS 10 Adaptation Tips](https://github.com/ChenYilong/iOS10AdaptationTips)。
## [](https://github.com/ChenYilong/iOS9AdaptationTips#1-demo1_youd-better-convert-http-to-https)1. Demo1_You'd Better Convert HTTP to HTTPS
How to deal with the SSL in iOS9，One solution is as follows:
![enter image description here](https://github.com/ChenYilong/iOS9AdaptationTips/raw/master/Demo1_iOS9%E7%BD%91%E7%BB%9C%E9%80%82%E9%85%8D_%E6%94%B9%E7%94%A8%E6%9B%B4%E5%AE%89%E5%85%A8%E7%9A%84HTTPS/%E5%BE%AE%E5%8D%9A%40iOS%E7%A8%8B%E5%BA%8F%E7%8A%AD%E8%A2%81/http%E9%97%AE%E9%A2%98.gif)
As [Apple](https://developer.apple.com/library/prerelease/ios/releasenotes/General/WhatsNewIniOS/Articles/iOS9.html#//apple_ref/doc/uid/TP40016198-DontLinkElementID_13) say
 :
![enter image description here](https://camo.githubusercontent.com/68960ab280c3183cf473b82fe658cf8b52a8a56e/68747470733a2f2f692e696d6775722e636f6d2f65546753485a592e706e67)
![enter image description here](https://camo.githubusercontent.com/6739f70037a4ce87972d0eb8a13527546cdcf936/68747470733a2f2f692e696d6775722e636f6d2f546330665336702e6a7067)
![enter image description here](https://camo.githubusercontent.com/41dfdf6f2f8adeb0c91e8ac56d4683ab47f929d5/68747470733a2f2f692e696d6775722e636f6d2f763254736b77682e6a7067)
iOS 9 and OSX 10.11 require TLSv1.2 SSL for all hosts you plan to request data from, unless you specify exception domains in your app's Info.plist file.
The syntax for the Info.plist configuration looks like this:
```
<key>NSAppTransportSecurity</key>
<dict>
  <key>NSExceptionDomains</key>
  <dict>
    <key>yourserver.com</key>
    <dict>
      <!--Include to allow subdomains-->
      <key>NSIncludesSubdomains</key>
      <true/>
      <!--Include to allow insecure HTTP requests-->
      <key>NSTemporaryExceptionAllowsInsecureHTTPLoads</key>
      <true/>
      <!--Include to specify minimum TLS version-->
      <key>NSTemporaryExceptionMinimumTLSVersion</key>
      <string>TLSv1.1</string>
    </dict>
  </dict>
</dict>
```
If your application (a third-party web browser, for instance) needs to connect to arbitrary hosts, you can configure it like this:
```
<key>NSAppTransportSecurity</key>
<dict>
    <!--Connect to anything (this is probably BAD)-->
    <key>NSAllowsArbitraryLoads</key>
    <true/>
</dict>
```
If you have to do this, it's probably best to update your servers to support TLSv1.2 and SSL. This should be considered a temporary workaround.
As of today, the prerelease documentation makes no mention of any of these configuration options in any specific way. Once it does, I'll update the answer to link to the relevant documentation.
Even though your sever supports TLSv1.2, you may also have to follow what I wrote above to ensure connecting successfully in iOS 9:
After some discussion with Apple Support, the issue is due to the self signed certificate.
ATS only trusts certificate signed by a well known CA, all others are rejected. As a consequence, the only solution with a Self signed certificate is to set an exception with NSExceptionDomains.
## [](https://github.com/ChenYilong/iOS9AdaptationTips#2demo2_ios9-new-feature--in-corelocation--backgroud-when-needed)2.Demo2_iOS9 new feature in CoreLocation : backgroud when needed
If you're using CoreLocation framework in your app in Xcode7(pre-released), you may notice that there is a newly added property called allowsBackgroundLocationUpdates in CLLocationManager class.
This new property is explained in the WWDC session ["What's New in Core Location"](https://developer.apple.com/videos/wwdc/2015/?id=714).![enter image description here](https://camo.githubusercontent.com/434fd48a31e360cb0607f881fa5332f548265ef7/68747470733a2f2f692e696d6775722e636f6d2f707656683166782e706e67)
The default value is `NO` if you link against iOS 9.
If your app uses location in the background without showing the blue status bar, you have to set `allowsBackgroundLocationUpdates` to `YES` in
 addition to setting the background mode capability in Info.plist. Otherwise location updates are only delivered in foreground. The advantage is that, in the same app, you can now have some of the location managers update locations in background, while others
 at foreground. You can also reset the value to `NO` to change the behavior.
The documentation makes it pretty clear:
> 
By default, this is NO for applications linked against iOS 9.0 or later, regardless of minimum deployment target.
With UIBackgroundModes set to include "location" in Info.plist, you must also set this property to YES at runtime whenever calling -startUpdatingLocation with the intent to continue in the background.
Setting this property to YES when UIBackgroundModes does not include "location" is a fatal error.
Resetting this property to NO is equivalent to omitting "location" from the UIBackgroundModes value. Access to location is still permitted whenever the application is running (ie not suspended), and has sufficient
 authorization (ie it has WhenInUse authorization and is in use, or it has Always authorization). However, the app will still be subject to the usual task suspension rules.
See -requestWhenInUseAuthorization and -requestAlwaysAuthorization for more details on possible authorization values.
Set Info.plist like this： ![enter image description here](https://camo.githubusercontent.com/0fa5c00013a32b4a0d37b53912296a21ec175cce/68747470733a2f2f692e696d6775722e636f6d2f4d416f4b6255652e706e67)
The syntax for the Info.plist configuration looks like this:
```
<key>NSLocationAlwaysUsageDescription</key>
<string>微博@iOS程序犭袁 请求后台定位权限</string>
<key>UIBackgroundModes</key>
<array>
    <string>location</string>
</array>
```
Use like:
```
_locationManager = [[CLLocationManager alloc] init];
_locationManager.delegate = self;
[_locationManager setDesiredAccuracy:kCLLocationAccuracyBest];
if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8) {
    [_locationManager requestAlwaysAuthorization];
}
if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 9) {
    _locationManager.allowsBackgroundLocationUpdates = YES;
}
[_locationManager startUpdatingLocation];
```
## [](https://github.com/ChenYilong/iOS9AdaptationTips#3ios-9-dealing-with-untrusted-enterprise-developer)3.iOS 9 Dealing With Untrusted Enterprise Developer
Since iOS 9, there is no more “trust” option for an enterprise build.
Before iOS 9, there used to have an alert popped like this:
![enter image description here](https://camo.githubusercontent.com/0f5aec3b1cf687600612627c0e85edbd66ba15b6/687474703a2f2f692e737461636b2e696d6775722e636f6d2f57774637362e706e67)
Now:
![enter image description here](https://camo.githubusercontent.com/66b08f5cd050af14d388f5e14b2720ef3304da2c/687474703a2f2f6935382e74696e797069632e636f6d2f327a65636d38332e6a7067)
Users have to do the configuration themselves: Go to Settings - General - Profiles - tap on your Profile - tap on Trust button.
![enter image description here](https://camo.githubusercontent.com/c8b9d10ca5e851e6ee20daa8e85d56833463f830/68747470733a2f2f692e696d6775722e636f6d2f4164474e5948652e676966)
## [](https://github.com/ChenYilong/iOS9AdaptationTips#4bitcode-option)4.bitcode option
After Xcode 7, bitcode option is set enabled by default. If your library is complied without bitcode while the bitcode option is enabled in your project setting, you can
> - Update your library with bit code, or you'll get warnings like:
(null): URGENT: all bitcode will be dropped because '/Users/myname/Library/Mobile Documents/com~apple~CloudDocs/foldername/appname/GoogleMobileAds.framework/GoogleMobileAds(GADSlot+AdEvents.o)' was built without
 bitcode. You must rebuild it with bitcode enabled (Xcode setting ENABLE_BITCODE), obtain an updated library from the vendor, or disable bitcode for this target. Note: This will be an error in the future.
- 
Say NO to Enable Bitcode in your target Build Settings
![enter image description here](https://camo.githubusercontent.com/4bbe93bae6c3edb9fb965e0e5e33b18e57e6cafd/68747470733a2f2f692e696d6775722e636f6d2f4f6f4f6f6755652e676966)
and the Library Build Settings to remove the warnings
For more information,go to [documentation
 of bitcode in developer library](https://developer.apple.com/library/prerelease/watchos/documentation/IDEs/Conceptual/AppDistributionGuide/AppThinning/AppThinning.html#//apple_ref/doc/uid/TP40012582-CH35-SW2)
,and WWDC 2015 Session 102: ["Platforms State of the Union"](https://developer.apple.com/videos/wwdc/2015/?id=102)
![enter image description here](https://camo.githubusercontent.com/44664c879bb780d18f3984ae94ea4bce73b6c35f/687474703a2f2f6d6f62696c65666f72776172642e6e65742f77702d636f6e74656e742f75706c6f6164732f323031352f30362f53637265656e2d53686f742d323031352d30362d31322d61742d362e35372e35342d504d2d363937783335312e706e67)
## [](https://github.com/ChenYilong/iOS9AdaptationTips#5privacy-and-your-appurl-scheme-changes)5.Privacy and Your App【URL scheme changes】
iOS 9 has made a small change to the handling of URL scheme. You must whitelist the url's that your app will call out by using the `LSApplicationQueriesSchemes` key
 in your `Info.plist`.
Please see the post here: [http://awkwardhare.com/post/121196006730/quick-take-on-ios-9-url-scheme-changes](http://awkwardhare.com/post/121196006730/quick-take-on-ios-9-url-scheme-changes)
The main conclusion is that:
> 
If you call the “canOpenURL” method on a URL that is not in your whitelist, it will return “NO”, even if there is an app installed that has registered to handle this scheme. A “This app is not allowed to query for
 scheme xxx” syslog entry will appear.
If you call the “openURL” method on a URL that is not in your whitelist, it will fail silently. A “This app is not allowed to query for scheme xxx” syslog entry will appear.
The author also speculates that this is a bug with the OS and Apple will fix this in a subsequent release.
This is a new security feature of iOS 9. Watch [WWDC 2015 Session 703](https://developer.apple.com/videos/wwdc/2015/?id=703) for more information.
![enter image description here](https://camo.githubusercontent.com/c94995d0a6d10b903949f129075e39bd79b2fafd/68747470733a2f2f692e696d6775722e636f6d2f324878575171712e706e67)
Any app built with SDK 9 needs to provide an `LSApplicationQueriesSchemes` entry in its plist file, declaring which scheme it
 attempts to query.
```
<key>LSApplicationQueriesSchemes</key>
<array>
 <string>urlscheme</string>
 <string>urlscheme2</string>
 <string>urlscheme3</string>
 <string>urlscheme4</string>
</array>
```
Assuming that we have two apps: Test A and Test B. TestB wants to check whether TestA is installed or not. "TestA" defines the following URL scheme in its info.plist file:
```
<key>CFBundleURLTypes</key>
<array>
    <dict>
        <key>CFBundleURLSchemes</key>
        <array>
            <string>testA</string>
        </array>
    </dict>
</array>
```
The second app "TestB" tries to find out if "TestA" is installed by calling:
```
[[UIApplication sharedApplication]
                    canOpenURL:[NSURL URLWithString:@"TestA://"]];
```
But this will generally return NO in iOS9, because "TestA" needs to be added to the LSApplicationQueriesSchemes entry in TestB's info.plist file. This is done by adding the following code to TestB's info.plist file:
```
<key>LSApplicationQueriesSchemes</key>
<array>
    <string>TestA</string>
</array>
```
A working implementation can be found here: [https://github.com/gatzsche/LSApplicationQueriesSchemes-Working-Example](https://github.com/gatzsche/LSApplicationQueriesSchemes-Working-Example)
## [](https://github.com/ChenYilong/iOS9AdaptationTips#6-support-slide-over-and-split-view-of-ios-9)6. Support Slide Over and Split View of iOS 9
![enter image description here](https://camo.githubusercontent.com/b1dce6c06ef1e6b3298e71b7b21b4f9e46d464fa/687474703a2f2f63646e312e746e7763646e2e636f6d2f77702d636f6e74656e742f626c6f67732e6469722f312f66696c65732f323031352f30362f65772d2e676966) How
 to make an old project support Slide Over and Split View in iOS 9? You may find all the demo projects was written by storyboard or xib, but the older project's UI is written by code！
I would suggest switching to storyboards to make your life easy.
I would highly recommend you to watch the following WWDC videos and then think about what exactly you need to do in order to support multi tasking.
- 
[Mysteries of Auto Layout, Part 1](https://developer.apple.com/videos/wwdc/2015/?id=218)
- 
[What's New in Storyboards](https://developer.apple.com/videos/wwdc/2015/?id=215)
- 
[Implementing UI Designs in Interface Builder](https://developer.apple.com/videos/wwdc/2015/?id=407)
- 
[Getting Started with Multitasking on iPad in iOS
 9](https://developer.apple.com/videos/wwdc/2015/?id=205)
- 
[Optimizing Your App for Multitasking on iPad in iOS](https://developer.apple.com/videos/wwdc/2015/?id=212)
## [](https://github.com/ChenYilong/iOS9AdaptationTips#7-ui-display-problem-due-to-enlarged-character-space)￼7. UI Display Problem Due to Enlarged Character Space
iOS 9 introduced a new font of Chinese characters. The enlarged character space may cause display problems of text labels, especially for fixed label width. To avoid this, we prefer to use ‘sizeToFit’ and ‘ceilf’ methods to calculate label size dynamically.
## [](https://github.com/ChenYilong/iOS9AdaptationTips#8-crash-and-warnings)8. Crash and Warnings
Sina SDK crashes on iOS 9 with the following information: Solution: update SDK. A case of Masonry crashing on iOS 9 left & leading, trailing & right: not equivalent any more. Some other modifications to make: Old way of setting status bar style lead to errors.
 Solution: delete ‘View controller-based status bar appearance’ key in ‘Info.plist’, apply new methods to set status bay style. Demo4 — set status bar style of navigationController If it still does not work after using the above method, the problem is probably
 on your rootViewController. Xcode 7 generates .dSYM file under debug circumstance and causes warnings. Solution: change debug setting. Prevent Xcode from generating .dSYM. Xcode 7 cannot debugging on 8.x devices. Solution: ensure the project name not including
 Chinese character. Problem of jumping between Safari and other apps. This is probably caused by using ‘iframe’ element in HTML. Warning when locking screen while executing tasks in apps. Haven’t found a solution. Crash when rootViewController not set in didFinishLaunchingWithOptions
 Solution: initialize rootViewController in didFinishLaunchingWithOptions and replace it afterwards.
## [](https://github.com/ChenYilong/iOS9AdaptationTips#9-search-api)9. Search API
Import related frameworks, and configure ‘search elements’ just as configuring tableview cells: Demo 6 shows how to combine CoreSpotlightSearch and tableView:
## [](https://github.com/ChenYilong/iOS9AdaptationTips#10-change-of-device-language-return-string)10. Change of Device Language Return String
Before iOS 9: the above code returns language string code (e.g. “zh-Hans”). ￼iOS 9: returns language string code + area code (e.g. “zh-Hans-US"). Be careful when checking current language.
## [](https://github.com/ChenYilong/iOS9AdaptationTips#11-uitableview-display-problem)11. UITableView Display Problem
A project running well on Xcode 6 may encounter the following problems on Xcode 7:
- Tableview created by code cannot hide cell separators.
- reloadData does not work. For cell separators, we provide two solutions (the first one preferred): As for reloadData, we assume this may conflict with some new features. We can use local reload as an alternative:
## [](https://github.com/ChenYilong/iOS9AdaptationTips#license)License
Posted by [微博@iOS程序犭袁](http://weibo.com/luohanchenyilong/)
[Creative Commons BY-NC-ND 3.0](http://creativecommons.org/licenses/by-nc-nd/3.0/deed.zh)
> 
中文
## [](https://github.com/ChenYilong/iOS9AdaptationTips#1-demo1_ios9网络适配_ats改用更安全的https)1. Demo1_iOS9网络适配_ATS：改用更安全的HTTPS
[摘要]为了强制增强数据访问安全， iOS9 默认会把 ~~所有的http请求~~ 所有从`NSURLConnection` 、 `CFURL` 、 `NSURLSession`发出的
 HTTP 请求，都改为 HTTPS 请求：iOS9.x-SDK编译时，默认会让所有从`NSURLConnection` 、 `CFURL` 、 `NSURLSession`发出的
 HTTP 请求统一采用TLS 1.2 协议。因为 AFNetworking 现在的版本底层使用了 `NSURLConnection` ，众多App将被影响（基于iOS8.x-SDK的App不受影响）。服务器因此需要更新，以解析相关数据。如不更新，可通过在
 Info.plist 中声明，倒退回不安全的网络请求。而这一做法，官方文档称为ATS，全称为App Transport Security，是iOS9的一个新特性。
一个符合 ATS 要求的 HTTPS，应该满足如下条件：
- Transport Layer Security协议版本要求TLS1.2以上
- 服务的Ciphers配置要求支持Forward Secrecy等
- 证书签名算法符合ATS要求等
官方文档 [App
 Transport Security Technote](https://developer.apple.com/library/prerelease/ios/technotes/App-Transport-Security-Technote/index.html#//apple_ref/doc/uid/TP40016240)对ATS 的介绍：
![enter image description here](https://camo.githubusercontent.com/e0031e8e3fd61674d054e374341d0ee8f25c295b/687474703a2f2f6935382e74696e797069632e636f6d2f616a7366306a2e6a7067)
注：有童鞋反映：服务器已支持TLS 1.2 SSL ，但iOS9上还是不行，还要进行本文提出的适配操作。
那是因为：要注意 App Transport Security 要求 TLS 1.2，而且它要求站点使用支持forward secrecy协议的密码。证书也要求是符合ATS规格的，ATS只信任知名CA颁发的证书，小公司所使用的 self signed certificate，还是会被ATS拦截。。因此慎重检查与你的应用交互的服务器是不是符合ATS的要求非常重要。对此，建议使用下文中给出的NSExceptionDomains，并将你们公司的域名挂在下面。下文也会详细描述该问题。
官方文档 [App
 Transport Security Technote](https://developer.apple.com/library/prerelease/ios/technotes/App-Transport-Security-Technote/index.html#//apple_ref/doc/uid/TP40016240)对CA颁发的证书要求：
> 
Certificates must be signed using a SHA256 or better signature hash algorithm, with either a 2048 bit or greater RSA key or a 256 bit or greater Elliptic-Curve (ECC) key. Invalid certificates result in a hard failure
 and no connection
在讨论之前，跟往常一样，先说下iOS程序猿们最关心的问题：
### [](https://github.com/ChenYilong/iOS9AdaptationTips#跟我有毛关系需要我加班吗)跟我有毛关系？需要我加班吗？！
首先咱们来看下业内对Apple这一做法的评论：
![enter image description here](https://camo.githubusercontent.com/aad5eb55aa716b2c9437823104206d11a5dd1112/68747470733a2f2f692e696d6775722e636f6d2f513137514447302e706e67)
这是某社交App上讨论，看来业内还是吐槽声和肯定声同在。
结论是：
> 
跟你很有关系，加班吧，少年！
书归正传【严肃脸】，我们正式讨论下 WHAT，WHY，HOW：
- WHAT（什么是SSL/TLS？跟HTTP和HTTPS有什么关系）
- WHY（以前的HTTP不是也能用吗？为什么要用SSL/TLS？！Apple是不是又在反人类？）
- HOW（如何适配？---弱弱地问下：加班要多久？）
### [](https://github.com/ChenYilong/iOS9AdaptationTips#what什么是ssltls跟http和https有什么关系)WHAT（什么是SSL/TLS？跟HTTP和HTTPS有什么关系）
什么是SSL/TLS？ SSL你一定知道，在此不做赘述。主要说下什么是TLS，还有跟HTTP和HTTPS有什么关系。
TLS 是 SSL 新的别称：
“TLS1.0”之于“SSL3.1”，犹“公元2015”之于“民国104”，“一千克”之于“一公斤”：称呼不同，意思相同。
SSL 3.0版本之后的迭代版本被重新命名为TLS 1.0：TLS 1.0＝SSL 3.1。所以我们平常也经常见到 “SSL/TLS” 这种说法。
目前，应用最广泛的是TLS 1.0，接下来是SSL 3.0。目前主流浏览器都已经实现了TLS 1.2的支持。
常用的有下面这些：
- SSL 2.0
- SSL 3.0
- TLS 1.0 (SSL 3.1)
- TLS 1.1 (SSL 3.1)
- TLS 1.2 (SSL 3.1)
那为什么标题是“使用HTTPS”而没有提及SSL和TLS什么事？ “SSL/TLS”跟HTTP和HTTPS有什么关系？
要理解这个，要看下他们之间的关系：
> 
HTTP+SSL/TLS+TCP = HTTPS
![HTTP+SSL/TLS+TCP](https://camo.githubusercontent.com/8e0b122edcfa71a076122f5daf6975d059ca5249/687474703a2f2f7777772e7a79747261782e636f6d2f746563682f737572766976616c2f73736c2d6c61796572732e676966)
或者
> 
HTTPS = “HTTP over SSL”
也就是说：
> 
Apple让你的HTTP采用SSL/TLS协议，就是让你从HTTP转到HTTPS。而这一做法，官方文档称为ATS，全称为App Transport Security。
### [](https://github.com/ChenYilong/iOS9AdaptationTips#why以前的http不是也能用吗为什么要用ssltlsapple是不是又在反人类)WHY（以前的HTTP不是也能用吗？为什么要用SSL/TLS？Apple是不是又在反人类？）
> 
不使用SSL/TLS的HTTP通信，就是不加密的通信！
不使用SSL/TLS的HTTP通信，所有信息明文传播，带来了三大风险：
- 窃听风险（eavesdropping）：第三方可以获知通信内容。
- 篡改风险（tampering）：第三方可以修改通信内容。
- 冒充风险（pretending）：第三方可以冒充他人身份参与通信。
SSL/TLS协议是为了解决这三大风险而设计的，希望达到：
- 所有信息都是加密传播，第三方无法窃听。
- 具有校验机制，一旦被篡改，通信双方会立刻发现。
- 配备身份证书，防止身份被冒充。
SSL/TLS的作用，打个比方来讲：
如果原来的 HTTP 是塑料水管，容易被戳破；那么如今新设计的 HTTPS 就像是在原有的塑料水管之外，再包一层金属水管（SSL/TLS协议）。一来，原有的塑料水管照样运行；二来，用金属加固了之后，不容易被戳破。
### [](https://github.com/ChenYilong/iOS9AdaptationTips#how如何适配---弱弱地问下加班要多久)HOW（如何适配？---弱弱地问下：加班要多久？）
正如文章开头所说：
> 
TLS 1.2 协议 强制增强数据访问安全 系统 Foundation 框架下的“相关网络请求”将不再默认使用 HTTP 等不安全的网络协议，而默认采用 TLS 1.2。服务器因此需要更新，以解析相关数据。如不更新，可通过在 Info.plist 中声明，倒退回不安全的网络请求。
总之：
> 
要么咱们iOS程序猿加班，要么后台加班：
方案一：立即让公司的服务端升级使用TLS 1.2，以解析相关数据。
方案二：虽Apple不建议，但可通过在 Info.plist 中声明，倒退回不安全的网络请求依然能让App访问指定http，甚至任意的http，具体做法见gif图，示例Demo见 [Demo1](https://github.com/ChenYilong/iOS9AdaptationTips)
![enter image description here](https://github.com/ChenYilong/iOS9AdaptationTips/raw/master/Demo1_iOS9%E7%BD%91%E7%BB%9C%E9%80%82%E9%85%8D_%E6%94%B9%E7%94%A8%E6%9B%B4%E5%AE%89%E5%85%A8%E7%9A%84HTTPS/%E5%BE%AE%E5%8D%9A%40iOS%E7%A8%8B%E5%BA%8F%E7%8A%AD%E8%A2%81/http%E9%97%AE%E9%A2%98.gif)
这也是官方文档和WWDC给出的解决方案：
- 
[Apple官方文档](https://developer.apple.com/library/prerelease/ios/releasenotes/General/WhatsNewIniOS/Articles/iOS9.html#//apple_ref/doc/uid/TP40016198-DontLinkElementID_13)![enter image description here](https://camo.githubusercontent.com/68960ab280c3183cf473b82fe658cf8b52a8a56e/68747470733a2f2f692e696d6775722e636f6d2f65546753485a592e706e67)
- 
[WWDC Session： "Networking with NSURLSession" session（
 【WWDC 2015 session 703, “Privacy and Your App” O网页链接 】, 时间在30:18左右）](https://developer.apple.com/videos/wwdc/2015/?id=703)
![enter image description here](https://camo.githubusercontent.com/6739f70037a4ce87972d0eb8a13527546cdcf936/68747470733a2f2f692e696d6775722e636f6d2f546330665336702e6a7067)
![enter image description here](https://camo.githubusercontent.com/41dfdf6f2f8adeb0c91e8ac56d4683ab47f929d5/68747470733a2f2f692e696d6775722e636f6d2f763254736b77682e6a7067)
![enter image description here](https://camo.githubusercontent.com/044a969436b0cab3bd73d4f23af2439bb66c0ae1/68747470733a2f2f63646e2d696d616765732d312e6d656469756d2e636f6d2f6d61782f3830302f312a392d566552585535534149366c4c5a65574c493068512e706e67)
即使你的应用使用的是：你没有权限控制的CDN (Content Delivery Network)，而且它不支持HTTPS！
也别担心，Apple都替你考虑好了：
![enter image description here](https://camo.githubusercontent.com/190b05de620acadc477de07111e348d1fb3dc064/687474703a2f2f6936312e74696e797069632e636f6d2f61653974676a2e6a7067)
正如你在上图中看到的：苹果官方提供了一些可选配置项来决定是否开启ATS模式，也就是可以选择开启或者不开启。
开发者可以针对某些确定的URL不使用ATS，这需要在工程中的info.plist中标记NSExceptionDomains。在NSExceptionDomains字典中，可以显式的指定一些不使用ATS的URL。这些你可以使用的例子可以是:
- 
NSIncludesSubdomains
- 
NSExceptionAllowInsecureHTTPLoads
- 
NSExceptionRequiresForwardSecrecy
- 
NSExceptionMinimumTLSVersion
- 
NSThirdPartyExceptionAllowsInsecureHTTPLoads
- 
NSThirdPartyExceptionMinimumTLSVersion
- 
NSThirdPartyExceptionRequiresForwardSecrecy
这些关键字使我们可以更加细致的设置针对不使用ATS的域名情况下禁用ATS或者一些特殊的ATS选项。
你可能注意到一些关键字像是使用了一些其他关键字中的词但是在前面加上了"ThirdParty"字样，比如列表里最后三个：
- 
NSThirdPartyExceptionAllowsInsecureHTTPLoads
- 
NSThirdPartyExceptionMinimumTLSVersion
- 
NSThirdPartyExceptionRequiresForwardSecrecy
在功能上，这些关键字与不含有"ThirdParty"的关键字有同样的效果。而且实际运行中所调用的代码将会完全忽略是否使用"ThirdParty"关键字。你应该使用适用于你的场景的关键字而不必过多考虑这些。
关于App Transport Security，每个应用都属于4个大类当中的一类。我们来看看每一个大类都是怎样影响应用的。
|--|分类名|解释|
|----|----|----|
|1.|HTTPS Only （只有HTTPS，所有情况下都使用ATS）|如果你的应用只基于支持HTTPS的服务器，你的应用不需要做任何改变。但是，注意App Transport Security要求TLS 1.2，而且它要求站点使用支持forward secrecy协议的密码。证书也要求是符合ATS规格的。因此慎重检查与你的应用交互的服务器是不是符合ATS的要求。|
|2.|Mix & Match（混合）|如果你的服务器不符合ATS要求，你需要在你的应用的 Info.plist 文件中说明哪些地址是不符合ATS要求的。|
|3.|Opt Out（禁用ATS）|如果你在创建一个网页浏览器，因为你不能确定用户将要访问哪个网页，也就不可能指明这些网页是否支持ATS要求且在HTTPS上传输。在这种情况下，只能配置为禁用ATS。|
|4.|Opt Out With Exceptions（除特殊情况外，都不使用ATS）|如果想禁用ATS的同时又想定义一些例外。这个应用场景是当你的应用需要从很多不符合ATS要求的服务器上取数据，但是也要与一个你可控的API(符合ATS要求)交互。在这种情况下，需要在应用的 Info.plist 文件中配置为允许所有请求，但是你也指定了一个或多个例外来表明哪些请求是必须符合ATS的要求。|
下面分别做一下介绍：
#### [](https://github.com/ChenYilong/iOS9AdaptationTips#1https-only-只有https所有情况下都使用ats)1.HTTPS Only （只有HTTPS，所有情况下都使用ATS）
如果你的应用只基于支持HTTPS的服务器，那么你太幸运了。你的应用不需要做任何改变。
唯一需要做的事情就是使用 `NSURLSession` 。如果你的开发目标是iOS 9或者 OS X EI Capitan之后，ATS 的最佳实践将会应用到所有基于 `NSURLSession` 的网络。
但也有人遇到过这样的疑惑：服务器已支持TLS 1.2 SSL ，但iOS9上还是不行，还要进行本文提出的适配操作。
那是因为：要注意 App Transport Security 要求 TLS 1.2，而且它要求站点使用支持forward secrecy协议的密码。证书也要求是符合ATS规格的，ATS只信任知名CA颁发的证书，小公司所使用的 self signed certificate，还是会被ATS拦截。因此慎重检查与你的应用交互的服务器是不是符合ATS的要求非常重要。对此，建议使用下文中给出的NSExceptionDomains，并将你们公司的域名挂在下面。
官方文档 [App
 Transport Security Technote](https://developer.apple.com/library/prerelease/ios/technotes/App-Transport-Security-Technote/index.html#//apple_ref/doc/uid/TP40016240)对CA颁发的证书要求：
> 
Certificates must be signed using a SHA256 or better signature hash algorithm, with either a 2048 bit or greater RSA key or a 256 bit or greater Elliptic-Curve (ECC) key. Invalid certificates result in a hard failure
 and no connection
#### [](https://github.com/ChenYilong/iOS9AdaptationTips#2mix--match混合)2.Mix & Match（混合）
如果你的服务器不符合ATS要求。
比如当你遇到以下三个不符合 ATS 要求的服务器的域名时：
- api.insecuredomain.com
- cdn.domain.com
- thatotherdomain.com
你可以分别设置如下：
- 
api.insecuredomain.com
Info.plist 配置中的XML源码如下所示:
  <key>NSAppTransportSecurity</key>
  <dict>
      <key>NSExceptionDomains</key>
      <dict>
          <key>api.insecuredomain.com</key>
          <dict>
              <!--允许App进行不安全的HTTP请求-->
              <key>NSExceptionAllowsInsecureHTTPLoads</key>
              <true/>
              <!--适用于这个特定域名下的所有子域-->
              <key>NSIncludesSubdomains</key>
              <true/>
          </dict>
      </dict>
  </dict>
在 plist 文件里显示如下：
![enter image description here](https://camo.githubusercontent.com/a45cc829d85c2a08788bd2a676148aef6c5f439a/687474703a2f2f6935392e74696e797069632e636f6d2f6678746b306a2e6a7067)
我们定义的第一个“例外”（Exception）告诉ATS当与这个子域交互的时候撤销了必须使用HTTPS的要求。注意这个仅仅针对在“例外”（Exception）中声明了的子域。非常重要的一点是要理解NSExceptionAllowsInsecureHTTPLoads关键字并不仅仅只是与使用HTTPS相关。这个“例外”（Exception）指明了对于那个域名，所有的App Transport Security的要求都被撤销了。
- 
cdn.domain.com Info.plist 配置中的XML源码如下所示:
  <key>NSAppTransportSecurity</key>
  <dict>
      <key>NSExceptionDomains</key>
      <dict>
          <key>cdn.somedomain.com</key>
          <dict>
              <key>NSThirdPartyExceptionMinimumTLSVersion</key>
              <string>TLSv1.1</string>
          </dict>
      </dict>
  </dict>
在 plist 文件里显示如下：
![enter image description here](https://camo.githubusercontent.com/1f2c5ee1274daece38e0f7da7e322f79d81e1b70/687474703a2f2f6935382e74696e797069632e636f6d2f323961746d356b2e6a7067)
很可能你的应用是与一个支持HTTPS传输数据的服务器交互，但是并没有使用TLS 1.2或更高。在这种情况下，你定义一个“例外”（Exception），它指明应该使用的最小的TLS的版本。这比完全撤销那个域名的App Transport Security要更好更安全。
- 
thatotherdomain.com
Info.plist 配置中的XML源码如下所示:
     <key>NSAppTransportSecurity</key>
      <dict>
          <key>NSExceptionDomains</key>
          <dict>
              <key>thatotherdomain.com</key>
              <dict>
                  <!--适用于这个特定域名下的所有子域-->
                  <key>NSIncludesSubdomains</key>
                  <true/>
                  <!--扩展可接受的密码列表：这个域名可以使用不支持 forward secrecy 协议的密码-->
                  <key>NSExceptionRequiresForwardSecrecy</key>
                  <false/>
                  <!--允许App进行不安全的HTTP请求-->
                  <key>NSExceptionAllowsInsecureHTTPLoads</key>
                  <true/>
                  <!--在这里声明所支持的 TLS 最低版本-->
                  <key>NSExceptionMinimumTLSVersion</key>
                  <string>TLSv1.1</string>
              </dict>
          </dict>
      </dict>
在 plist 文件里显示如下：
![enter image description here](https://camo.githubusercontent.com/3f3c2443c8ccc5fabe5728fc3b911e4b2898748d/687474703a2f2f6936312e74696e797069632e636f6d2f7736786e34332e6a7067)
`NSIncludesSubdomains` 关键字告诉 App Transport Security 这个“例外”（Exception）适用于这个特定域名的所有子域。这个“例外”（Exception）还进一步通过扩展可接受的密码列表来定义这个域名可以使用不支持forward
 secrecy( `NSExceptionRequiresForwardSecrecy` ) 协议的密码。想了解更多关于forward secrecy的信息，推荐去看官方文档[Apple's
 technote](https://developer.apple.com/library/prerelease/mac/technotes/App-Transport-Security-Technote/index.html)。
如果你的App中同时用到了这三个域名，那么应该是这样：
     <key>NSAppTransportSecurity</key>
        <dict>
            <key>NSExceptionDomains</key>
            <dict>
                <key>api.insecuredomain.com</key>
                <dict>
                    <key>NSExceptionAllowsInsecureHTTPLoads</key>
                    <false/>
                </dict>
                <key>cdn.somedomain.com</key>
                <dict>
                    <key>NSThirdPartyExceptionMinimumTLSVersion</key>
                    <string>TLSv1.1</string>
                </dict>
                <key>thatotherdomain.com</key>
                <dict>
                    <key>NSIncludesSubdomains</key>
                    <true/>
                    <key>NSExceptionRequiresForwardSecrecy</key>
                    <false/>
                </dict>
            </dict>
        </dict>
![enter image description here](https://camo.githubusercontent.com/22d65c0f6f142cc9df8f0062d2276bde58348e0e/687474703a2f2f6936312e74696e797069632e636f6d2f3133796e67676b2e6a7067)
#### [](https://github.com/ChenYilong/iOS9AdaptationTips#3-opt-out禁用ats)3. Opt Out（禁用ATS）
上面是比较严谨的做法，指定了能访问哪些特定的HTTP。当然也有暴力的做法： 彻底倒退回不安全的HTTP网络请求，能任意进行HTTP请求，比如你在开发一款浏览器App，或者你想偷懒，或者后台想偷懒，或者公司不给你升级服务器。。。
你可以在Info.plist 配置中改用下面的XML源码：
    <key>NSAppTransportSecurity</key>
    <dict>
        <!--彻底倒退回不安全的HTTP网络请求，能任意进行HTTP请求 (不建议这样做)-->
        <key>NSAllowsArbitraryLoads</key>
        <true/>
    </dict>
在 plist 文件里显示如下：
![enter image description here](https://camo.githubusercontent.com/0319de82a394f9d6d6a5c24c80c1f4534a0118e2/687474703a2f2f6935372e74696e797069632e636f6d2f3975713263372e6a7067)
#### [](https://github.com/ChenYilong/iOS9AdaptationTips#4-opt-out-with-exceptions除特殊情况外都不使用ats)4. Opt Out With Exceptions（除特殊情况外，都不使用ATS）
上面已经介绍了三种情景，还有一种可能你也会遇到：
当你禁用ATS的同时又想定义一些“例外”（Exception）。这个应用场景是当你的应用需要从很多不符合ATS要求的服务器上取数据，但是也要与一个你可控的API(符合ATS要求)交互。在这种情况下，在应用的Info.plist文件中配置为允许所有请求，但是你也指定了一个或多个“例外”（Exception）来表明哪些地址是必须符合 App Transport Security 要求的。下面是Info.plist文件应该会有的内容：
<key>NSAppTransportSecurity</key>
        <dict>
            <key>NSAllowsArbitraryLoads</key>
            <true/>
            <key>NSExceptionDomains</key>
            <dict>
                <key>api.tutsplus.com</key>
                <dict>
                    <key>NSExceptionAllowsInsecureHTTPLoads</key>
                    <false/>
                </dict>
            </dict>
        </dict>
在 plist 文件里显示如下：
![enter image description here](https://camo.githubusercontent.com/179b5719d5906b0b474b282431e73fd9c3733187/687474703a2f2f6936322e74696e797069632e636f6d2f6465317277392e6a7067)
~~【注：以上在Info.plist配置中的做法已经验证可行，但目前Apple的prerelease版本的官方文档并未提及Info.plist中配置的代码，我将密切关注官方文档，如有提及，再来更新[本文](https://github.com/ChenYilong/iOS9AdaptationTips) .你若发现官方文档有提及了，也可在[微博@iOS程序犭袁](http://weibo.com/luohanchenyilong/)通知下我。】（官方文档已经有阐述）~~
#### [](https://github.com/ChenYilong/iOS9AdaptationTips#certificate-transparency)Certificate Transparency
虽然ATS大多数安全特性都是默认可用的，Certificate Transparency 是必须设置的。如果你有支持Certificate Transparency的证书，你可以检查NSRequiresCertificateTransparency关键字来使用Certificate Transparency。再次强调，如果你的证书不支持Certificate Transparency，此项需要设置为不可用。
如果需要调试一些由于采用了ATS而产生的问题，需要设置CFNETWORK_DIAGNOSTICS为1，这样就会打印出包含被访问的URL和ATS错误在内的NSURLSession错误信息。要确保处理了遇到的所有的错误消息，这样才能使ATS易于提高可靠性和扩展性。
#### [](https://github.com/ChenYilong/iOS9AdaptationTips#q-a)Q-A
Q：我用xcode7编译的app，如果不在plist里面加关键字说明，ios9下不能进行网络请求，因为我们服务器并不支持 TLS 1.2 ，我要是直接下载app store上的，什么也没有做，也是能正常网络请求。
A：本文中所罗列的新特性，多数情况下指的是 iOS9.X-SDK 新特性，AppStore 的版本是基于 iOS8.X-SDK或 iOS7.X-SDK，所以并不受 iOS9新特性约束。也就是说：Xcode7给iOS8打设备包可以请求到网络，Xcode7给iOS9设备打的包请求不到网络，Xcode7和iOS9缺一不可，才需要网络适配ATS。
那么，如何确认自己项目所使用的 SDK？在Targets->Build Setting-->Architectures
![enter image description here](https://camo.githubusercontent.com/203388f8139ffcf2332becd6fc6c359c35456a46/687474703a2f2f6935382e74696e797069632e636f6d2f616d736139752e6a7067)
Q：服务器已支持TLS 1.2 SSL ，但iOS9上还是不行，还要进行本文提出的适配操作。
A：那是因为：要注意 App Transport Security 要求 TLS 1.2，而且它要求站点使用支持forward secrecy协议的密码。证书也要求是符合ATS规格的，ATS只信任知名CA颁发的证书，小公司所使用的 self signed certificate，还是会被ATS拦截。。因此慎重检查与你的应用交互的服务器是不是符合ATS的要求非常重要。对此，建议使用下文中给出的NSExceptionDomains，并将你们公司的域名挂在下面。
官方文档 [App
 Transport Security Technote](https://developer.apple.com/library/prerelease/ios/technotes/App-Transport-Security-Technote/index.html#//apple_ref/doc/uid/TP40016240)对CA颁发的证书要求：
> 
Certificates must be signed using a SHA256 or better signature hash algorithm, with either a 2048 bit or greater RSA key or a 256 bit or greater Elliptic-Curve (ECC) key. Invalid certificates result in a hard failure
 and no connection
Q：我使用的是第三方的网络框架，比如 AFNetworking 、ASIHTTPRequest、CFSocket 等，这个有影响没有？
A： AFNetworking 有影响，其它没影响。
ATS 是只针对 `NSURLConnection` 、 `CFURL` 、 `NSURLSession` ，如果底层涉及到这三个类就会有影响。
现在的 AFNetworking 的 AFHTTPRequestOperationManager 实现是使用的 `NSURLConnection` 。
但 AFNetworking 也有更新计划，移除 `NSURLConnection` 相关API，迁移到 AFHTTPSessionManager ，但还未执行，详情见：[https://github.com/AFNetworking/AFNetworking/issues/2806](https://github.com/AFNetworking/AFNetworking/issues/2806)。
Q：试了一下禁用 ATS 的方法 但是还是无法联网 仍然提示要使用https?
> 
App Transport Security has blocked a cleartext HTTP (http://) resource load since it is insecure. Temporary exceptions can be configured via your app's Info.plist file.
The resource could not be loaded because the App Transport Security policy requires the use of a secure connection.
A：遇到这类问题，90%是出现在“一个 Project 多 Target ”的情况下，所以 请确保你修改的，确实是你的 Target 所属的 Info.plist ！
如何确认？请前往这里，确认你 Target 所属的 Info.plist 究竟是哪个：
Project -> Your Target -> Build Settings -> Info.plist File
![enter image description here](https://camo.githubusercontent.com/290158c0eb7271ce2d2afe41ba61458c7118c6fc/687474703a2f2f6936302e74696e797069632e636f6d2f73627266726c2e6a7067)
或者更直截了当一点，直接修改：
Project -> Your Target —>info－> Custom iOS target properties－> 添加禁用 ATS 的属性
![enter image description here](https://camo.githubusercontent.com/3e9de4aa4e7a8d77ef94c194d1503c0a928187e9/687474703a2f2f6936302e74696e797069632e636f6d2f7a76627437622e6a7067)
还有一种可能性是：禁用 ATS 的代码粘贴进 plist 时，位置不对，可以尝试放在第5行.
Q：我的项目是“一个 Project 多 Target ”，按照本文禁用 ATS 的方法，是不是每个 Info.plist 都要修改？
A：不需要，用到哪个 Target 修改哪个的 Info.plist ，Target 是独立的，不受其他 Target 的影响，也不会影响其他 Target。
Q：如何检测我们公司 HTTPS 是否符合 ATS 的要求？
A： 如果你的 App 的服务也在升级以适配ATS要求，可以使用如下的方式进行校验：
在OS X EI Capitan系统的终端中通过nscurl命令来诊断检查你的HTTPS服务配置是否满足Apple的ATS要求:
 $ nscurl --verbose --ats-diagnostics https://<your_server_domain>
当然，你也可以让公司服务端的同事参考Apple提供官方指南App Transport Security Technote进行服务的升级配置以满足ATS的要求：
一个符合 ATS 要求的 HTTPS，应该满足如下条件：
- Transport Layer Security协议版本要求TLS1.2以上
- 服务的Ciphers配置要求支持Forward Secrecy等
- 证书签名算法符合ATS要求等
## [](https://github.com/ChenYilong/iOS9AdaptationTips#2demo2_ios9新特性_更灵活的后台定位)2.Demo2_iOS9新特性_更灵活的后台定位
【iOS9在定位的问题上，有一个坏消息一个好消息】坏消息：如果不适配iOS9，就不能偷偷在后台定位（不带蓝条，见图）！好消息：将允许出现这种场景：同一App中的多个location manager：一些只能在前台定位，另一些可在后台定位，并可随时开启或者关闭特定location manager的后台定位。
如果没有请求后台定位的权限，也是可以在后台定位的，不过会带蓝条：![enter image description here](https://camo.githubusercontent.com/92d0b5a946d4d89cad7db00027ccd6addfcab595/68747470733a2f2f692e696d6775722e636f6d2f556f7147486c472e706e67)
如何偷偷在后台定位：请求后台定位权限：
```
// 1. 实例化定位管理器
_locationManager = [[CLLocationManager alloc] init];
// 2. 设置代理
_locationManager.delegate = self;
// 3. 定位精度
[_locationManager setDesiredAccuracy:kCLLocationAccuracyBest];
// 4.请求用户权限：分为：⓵只在前台开启定位⓶在后台也可定位，
//注意：建议只请求⓵和⓶中的一个，如果两个权限都需要，只请求⓶即可，
//⓵⓶这样的顺序，将导致bug：第一次启动程序后，系统将只请求⓵的权限，⓶的权限系统不会请求，只会在下一次启动应用时请求⓶
if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8) {
    //[_locationManager requestWhenInUseAuthorization];//⓵只在前台开启定位
    [_locationManager requestAlwaysAuthorization];//⓶在后台也可定位
}
// 5.iOS9新特性：将允许出现这种场景：同一app中多个location manager：一些只能在前台定位，另一些可在后台定位（并可随时禁止其后台定位）。
if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 9) {
    _locationManager.allowsBackgroundLocationUpdates = YES;
}
// 6. 更新用户位置
[_locationManager startUpdatingLocation];
```
但是如果照着这种方式尝试，而没有配置Info.plist，100%你的程序会崩溃掉，并报错：
> 
*** Assertion failure in -[CLLocationManager setAllowsBackgroundLocationUpdates:], /BuildRoot/Library/Caches/com.apple.xbs/Sources/CoreLocationFramework_Sim/CoreLocation-1808.1.5/Framework/CoreLocation/CLLocationManager.m:593
这个问题，有两种方式可以解决：
第一种：
要将 Info.plist 配置如下： ![enter image description here](https://camo.githubusercontent.com/0fa5c00013a32b4a0d37b53912296a21ec175cce/68747470733a2f2f692e696d6775722e636f6d2f4d416f4b6255652e706e67)
对应的 Info.plist 的XML源码是：
```
<key>NSLocationAlwaysUsageDescription</key>
<string>微博@iOS程序犭袁 请求后台定位权限</string>
<key>UIBackgroundModes</key>
<array>
    <string>location</string>
</array>
```
第二种：
在对应 target 的 Capabilities -> Background Modes -> 开启 Location Updates
![enter image description here](https://camo.githubusercontent.com/77ea43af9dc61ea1c404ecfb3dda55f4c7a7edb1/687474703a2f2f63646e322e72617977656e6465726c6963682e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031342f31322f6261636b67726f756e645f6d6f6465732e706e67)
## [](https://github.com/ChenYilong/iOS9AdaptationTips#3企业级分发)3.企业级分发
有两处变化：
- iOS9以后，企业级分发ipa包将遭到与Mac上dmg安装包一样的待遇：默认不能安装，也不再出现“信任按钮”
- iOS9以后，企业分发时可能存在：下载的ipa包与网页两者的 bundle ID 无法匹配而导致下载失败的情况
### [](https://github.com/ChenYilong/iOS9AdaptationTips#1-ios9以后企业级分发ipa包将遭到与mac上dmg安装包一样的待遇默认不能安装也不再出现信任按钮)1. iOS9以后，企业级分发ipa包将遭到与Mac上dmg安装包一样的待遇：默认不能安装，也不再出现“信任按钮”
iOS9之前，企业级分发十分方便：点击App出现“信任按钮”，
![enter image description here](https://camo.githubusercontent.com/751bf95170a6a8051a7431e2f68098e0c4eacc3c/68747470733a2f2f692e696d6775722e636f6d2f61536d4d38626b2e706e67)
iOS9以后，企业级分发ipa包将遭到与Mac上dmg安装包一样的待遇：默认不能安装，也不再出现“信任按钮”
![enter image description here](https://camo.githubusercontent.com/66b08f5cd050af14d388f5e14b2720ef3304da2c/687474703a2f2f6935382e74696e797069632e636f6d2f327a65636d38332e6a7067)
必须让用户进行gif图中的设置：
![enter image description here](https://camo.githubusercontent.com/84384d3101e995df2dd5a6f4c8945c52b72739f7/68747470733a2f2f692e696d6775722e636f6d2f50584d3233354c2e676966)
### [](https://github.com/ChenYilong/iOS9AdaptationTips#2-ios9以后企业分发时可能存在下载的ipa包与网页两者的-bundle-id-无法匹配而导致下载失败的情况)2. iOS9以后，企业分发时可能存在：下载的ipa包与网页两者的 bundle ID 无法匹配而导致下载失败的情况
iOS9升级后众多企业分发的 app 已经出现了不能安装的情况，而iOS8或更早的系统不受影响。那是因为从iOS9以后，系统会在 ipa 包下载完之后，拿ipa包中的 bundle ID 与网页中的 plist 文件中的 bundle ID 进行比对，不一致不允许安装。
错误提示如下：
![enter image description here](https://camo.githubusercontent.com/6253d72e5ab716c09cc9138d794656ed0922f13c/687474703a2f2f6935372e74696e797069632e636f6d2f32386a636b75732e6a7067)
网页中的 plist 文件中的 bundle ID 的作用可参考 [《iOS:苹果企业证书通过网页分发安装app》](http://blog.sina.com.cn/s/blog_6afb7d800101fa16.html) 。
正如这篇文章提到的，“网页中的 plist 文件”是习惯的叫法，也有人称作“manifest文件”，比如 [这篇文章](http://gknops.github.io/adHocGenerate/)。
而iOS9之前，苹果不会检查这一项，因此iOS9之前可以安装。
导致这一错误的原因除了粗心，还有开发者是故意设置不一致，据开发者说：
> 
当初服务器 plist 的 bundle id 上故意做成成不一致。是为了解决一些人安装不上的问题。
详情可参考： [《升级到ios 9，企业版发布现在无法安装成功了，有人遇到了这种问题吗？》](http://www.cocoachina.com/bbs/read.php?tid-324230-fpage-2-page-1.html)
如何知道是因为 bundle id 不一致造成的无法安装？
通过查看设备上的日志信息：有一个 itunesstored 进程提示安装信息：
```
itunesstored →  <Warning>: [Download]: Download task did finish: 8 for download: 2325728577585828282
  itunesstored →  <Warning>: [ApplicationWorkspace] Installing download: 2325728577585828282 with step(s): Install
  itunesstored →  <Warning>: [ApplicationWorkspace]: Installing software package with bundleID: com.***.***: bundleVersion: 1.01 path: /var/mobile/Media/Downloads/2325728577585828282/-1925357977307433048
  itunesstored →  <Warning>: BundleValidator: Failed bundleIdentifier: com.***.**** does not match expected bundleIdentifier: com.***.*********
  itunesstored →  <Warning>: [ApplicationWorkspace]: Bundle validated for bundleIdentifier: com.****.******success: 0
  itunesstored →  <Warning>: LaunchServices: Uninstalling placeholder for app <LSApplicationProxy: 0x12677be70> com.****.*******(Placeholder) <file:///private/var/mobile/Containers/Bundle/Application/B62D8EA3-2052-4393-8A7E-3FD27228BFC2/2325728577585828282.app>
  itunesstored →  <Warning>: LaunchServices: Uninstalling app <LSApplicationProxy: 0x12677be70> com.****.*****(Placeholder) <file:///private/var/mobile/Containers/Bundle/Application/B62D8EA3-2052-4393-8A7E-3FD27228BFC2/2325728577585828282.app>
```
其中的这一句很重要：
```
itunesstored →  <Warning>: BundleValidator: Failed bundleIdentifier: com.***.**** does not match expected bundleIdentifier: com.***.*********
```
经过核对，果然是.ipa文件中真实的Bundle ID和manifest文件中配置的信息不匹配，然后测试发现：
> 
iOS 9是校验bundle-identifier值的，而iOS 9以下版本是不校验，一旦iOS 9发现bundle-identifier不匹配，即使下载成功了，也会 Uninstall(日志中提示的)app的。
适配方法：
- 
两者的 bundle id 修改一致
一旦出现iOS9能够安装企业版本APP，iOS9以下版本不能安装，一定先查看安装日志，然后核对每个参数配置。
manifest文件的参考配置。
<!DOCTYPEplist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
"http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plistversion="1.0">
<dict>
<!-- array of downloads. -->
<key>items</key>
<array>
   <dict>
       <!-- an array of assets to download -->
       <key>assets</key>
       <array>
           <!-- software-package: the ipa to install. -->
           <dict>
               <!-- required.  the asset kind. -->
               <key>kind</key>
               <string>software-package</string>
               <!-- optional.  md5 every n bytes.  --><!-- will restart a chunk if md5 fails. -->
               <key>md5-size</key>
               <integer>10485760</integer>
               <!-- optional.  array of md5 hashes -->
               <key>md5s</key>
               <array>
                   <string>41fa64bb7a7cae5a46bfb45821ac8bba</string>
                   <string>51fa64bb7a7cae5a46bfb45821ac8bba</string>
               </array>
               <!-- required.  the URL of the file to download. -->
               <key>url</key>
               <string>http://www.example.com/apps/foo.ipa</string>
           </dict>
           <!-- display-image: the icon to display during download. -->
           <dict>
               <key>kind</key>
               <string>display-image</string>
               <!-- optional. icon needs shine effect applied. -->
               <key>needs-shine</key>
               <true/>
               <key>url</key>
               <string>http://www.example.com/image.57×57.png</string>
           </dict>
           <!-- full-size-image: the large 512×512 icon used by iTunes. -->
           <dict>
               <key>kind</key>
               <string>full-size-image</string>
               <!-- optional.  one md5 hash for the entire file. -->
               <key>md5</key>
               <string>61fa64bb7a7cae5a46bfb45821ac8bba</string>
               <key>needs-shine</key>
               <true/>
               <key>url</key>
               <string>http://www.example.com/image.512×512.jpg</string>
           </dict>
       </array><key>metadata</key>
       <dict>
           <!-- required -->
           <key>bundle-identifier</key>
           <string>com.example.fooapp</string>
           <!-- optional (software only) -->
           <key>bundle-version</key>
           <string>1.0</string>
           <!-- required.  the download kind. -->
           <key>kind</key>
           <string>software</string>
           <!-- optional. displayed during download; --><!-- typically company name -->
           <key>subtitle</key>
           <string>Apple</string>
           <!-- required.  the title to display during the download. -->
           <key>title</key>
           <string>Example Corporate App</string>
       </dict>
   </dict>
</array>
</dict>
</plist>
- 
使用fir.im等第三方分发平台：上述“ bundle id 不一致导致下载失败”这种情况只会出现在企业自己搭建网页分发的情形下，事实证明第三方的分发平台更加专业，已经很好地规避了该情况的发生。
### [](https://github.com/ChenYilong/iOS9AdaptationTips#3-企业app安装之后在网络情况为wi-fi环境的时候可能会出现无法验证应用的情况出现以下提示)3. 企业APP安装之后，在网络情况为Wi-Fi环境的时候，可能会出现无法验证应用的情况。出现以下提示：
无法验证"** Co.,Ltd"应用，需要网络连接以在这台iPhone上验证"**** Co.,Ltd"应用。接入互联网并重试。**
![](https://camo.githubusercontent.com/3b09bf6e91b8e515c0040e495e1e739b06bde650/687474703a2f2f6936332e74696e797069632e636f6d2f3130686f3835772e6a7067)
![](https://camo.githubusercontent.com/a4d9bc496ae8b9aba03f879e75aa9bc705aac0f6/687474703a2f2f6936362e74696e797069632e636f6d2f3330756372756f2e6a7067)
![](https://camo.githubusercontent.com/83f770fcc1dfdad136d708c763d6809f8c2e52a1/687474703a2f2f6936362e74696e797069632e636f6d2f7731347169312e6a7067)
而此时，Wi-Fi网络是接入互联网的。如果多次验证不通过的话，我们需要切换到非Wi-Fi网络环境下才能解决这个问题。
### [](https://github.com/ChenYilong/iOS9AdaptationTips#q-a-1)Q-A
Q：企业分发，企业版证书在iOS9上安装应用报 `Ignore manifest download, already have bundleID: com.mycom.MyApp`只有我的手机无法安装，别人 iOS9 都可以安装
A：这并非 iOS9的问题，iOS8及以前的系统也会出现，和缓存有关系，请尝试关机重启手机，然后就可以安装了。
Q：为什么用微信扫描二维码不能直接下载？
A：部分市场支持直接扫描二维码下载（如蒲公英，fir.im等）,但是用微信扫描二维码不能直接。因为安装app所在页面必须要符合苹果`itms-services`协议，在不符合该协议的页面点击安装会没有反应。比如使用微信、QQ扫描点击安装会没有反应。
此时需要在微信或QQ安装页面点击右上角按钮，在弹出框里找到使用Safari浏览器打开选项，点击跳转到系统浏览器Safari里后再点击“安装”按钮进行安装。
部分第三分浏览器支持`itms-services`协议，可以直接点击安装。
## [](https://github.com/ChenYilong/iOS9AdaptationTips#4bitcode)4.Bitcode
【前言】未来， Watch 应用必须包含 bitcode ，iOS不强制，Mac OS不支持。 但最坑的一点是： Xcode7 及以上版本会默认开启 bitcode 。
什么是 bitcode ？
通俗解释：在线版安卓ART模式。
Apple 官方文档--[App
 Distribution Guide – App Thinning (iOS, watchOS)](https://developer.apple.com/library/prerelease/ios/documentation/IDEs/Conceptual/AppDistributionGuide/AppThinning/AppThinning.html#//apple_ref/doc/uid/TP40012582-CH35)是这样定义的：
> 
Bitcode is an intermediate representation of a compiled program. Apps you upload to iTunes Connect that contain bitcode will be compiled and linked on the App Store. Including bitcode will allow Apple to re-optimize
 your app binary in the future without the need to submit a new version of your app to the store.
翻译过来就是：
> 
bitcode 是被编译程序的一种中间形式的代码。包含 bitcode 配置的程序将会在 App Store 上被编译和链接。 bitcode 允许苹果在后期重新优化我们程序的二进制文件，而不需要我们重新提交一个新的版本到 App Store 上。
在 Xcode简介--- [What’s
 New in Xcode-New Features in Xcode 7](https://developer.apple.com/library/prerelease/ios/documentation/DeveloperTools/Conceptual/WhatsNewXcode/Articles/xcode_7_0.html)中这样描述：
> 
Bitcode. When you archive for submission to the App Store, Xcode will compile your app into an intermediate representation. The App Store will then compile the bitcode down into the 64 or 32 bit executables as necessary.
也就是
> 
当我们提交程序到 App Store上时， Xcode 会将程序编译为一个中间表现形式( bitcode )。然后 App store 会再将这个 bitcode 编译为可执行的64位或32位程序。
再看看这两段描述都是放在App Thinning(App瘦身)一节中，可以看出其与包的优化有关了。
打个比方，没有 bitcode 的 AppStore 里所提供的 App，类似在新华书店里卖捆绑销售的《四大名著丛书--精装版》，要买只能全买走，有了 bitcode 就好比这套四大名著每本都可以单卖，顾客就能按需购买。我们开发者在这个过程中扮演的角色是图书出版商的角色，应该照顾那些没钱一次买四本的顾客。（不要做不珍惜用户流量和存储空间的奸商。。）
那为什么第三方的 SDK 不支持 bitcode，我的 app 也就不能支持？打个比方，《四大名著丛书》只要有一本是可以单卖的，那么你很难再卖捆绑销售款的《四大名著丛书》了，所以干脆全都可以单卖，这大概就是 Apple 的逻辑。
App Thinning 官方文档解释如下：
> 
The App Store and operating system optimize the installation of iOS and watchOS apps by tailoring app delivery to the capabilities of the user’s particular device, with minimal footprint. This optimization, called
 app thinning, lets you create apps that use the most device features, occupy minimum disk space, and accommodate future updates that can be applied by Apple. Faster downloads and more space for other apps and content provides a better user experience.
开发者都知道，当前 iOS App 的编译打包方式是把适配兼容多个设备的执行文件及资源文件合并一个文件，上传和下载的文件则包含了所有的这些文件，导致占用较多的存储空间。
App Thinning是一个关于节省iOS设备存储空间的功能，它可以让iOS设备在安装、更新及运行App等场景中仅下载所需的资源，减少App的占用空间，从而节省设备的存储空间。
根据Apple官方文档的介绍，App Thinning主要有三个机制：
- 
Slicing
开发者把App安装包上传到AppStore后，Apple服务会自动对安装包切割为不同的应用变体(App variant)，当用户下载安装包时，系统会根据设备型号下载安装对应的单个应用变体。
- 
On-Demand Resources
ORD(随需资源)是指开发者对资源添加标签上传后，系统会根据App运行的情况，动态下载并加载所需资源，而在存储空间不足时，自动删除这类资源。
- 
Bitcode 开启Bitcode编译后，可以使得开发者上传App时只需上传Intermediate Representation(中间件)，而非最终的可执行二进制文件。 在用户下载App之前，AppStore会自动编译中间件，产生设备所需的执行文件供用户下载安装。
（喵大(@onevcat)在其博客 [《开发者所需要知道的 iOS 9 SDK 新特性》](http://onevcat.com/2015/06/ios9-sdk/) 中也描述了iOS 9中苹果在App瘦身中所做的一些改进，大家可以转场到那去研读一下。）
其中，Bitcode的机制可以支持动态的进行App Slicing，而对于Apple未来进行硬件升级的措施，此机制可以保证在开发者不重新发布版本的情况下而兼容新的设备。
Bitcode 是一种中间代码，那它是什么格式的呢？ LLVM 官方文档有介绍这种文件的格式： [LLVM
 Bitcode File Format](http://llvm.org/docs/BitCodeFormat.html#llvm-bitcode-file-format)。
如果你的应用也准备启用 Bitcode 编译机制，就需要注意以下几点：
- Xcode 7默认开启 Bitcode ，如果应用开启 Bitcode，那么其集成的其他第三方库也需要是 Bitcode 编译的包才能真正进行 Bitcode 编译
- 开启 Bitcode 编译后，编译产生的 `.app` 体积会变大(中间代码，不是用户下载的包)，且 `.dSYM` 文件不能用来崩溃日志的符号化(用户下载的包是
 Apple 服务重新编译产生的，有产生新的符号文件)
- 通过 Archive 方式上传 AppStore 的包，可以在Xcode的Organizer工具中下载对应安装包的新的符号文件
如何适配？
在上面的错误提示中，提到了如何处理我们遇到的问题：
> 
You must rebuild it with bitcode enabled (Xcode setting ENABLE_BITCODE), obtain an updated library from the vendor, or disable bitcode for this target. for architecture arm64
正如开头所说的：
> 
未来， Watch 应用必须包含 Bitcode ，iOS不强制，Mac OS不支持。 但最坑的一点是： Xcode7 及以上版本会默认开启 Bitcode 。
Xcode 7 + 会开启 Bitcode。
也就是说，也两种方法适配：
方法一：更新 library 使包含 Bitcode ，否则会出现以下中的警告；
> 
(null): URGENT: all bitcode will be dropped because '/Users/myname/Library/Mobile Documents/com~apple~CloudDocs/foldername/appname/GoogleMobileAds.framework/GoogleMobileAds(GADSlot+AdEvents.o)' was built without
 bitcode. You must rebuild it with bitcode enabled (Xcode setting ENABLE_BITCODE), obtain an updated library from the vendor, or disable bitcode for this target. Note: This will be an error in the future.
甚至有的会报错误，无法通过编译：
> 
ld: ‘/Users//Framework/SDKs/PolymerPay/Library/mobStat/libSDK.a(**ForSDK.o)’ does not contain bitcode. You must rebuild it with bitcode enabled (Xcode setting ENABLE_BITCODE),
 obtain an updated library from the vendor, or disable bitcode for this target. for architecture arm64
或：
> 
ld: -undefined and -bitcode_bundle (Xcode setting `ENABLE_BITCODE` =YES) cannot be
 used together clang: error: linker command failed with exit code 1 (use -v to see invocation)
![enter image description here](https://camo.githubusercontent.com/9fdcc19dea820bf715c27b40f6eb225b6856f536/687474703a2f2f6936322e74696e797069632e636f6d2f333330766875672e6a7067)
无论是警告还是错误，得到的信息是：我们引入的一个第三方库不包含bitcode。
方法二：关闭Bitcode，方法见下图
> 
![enter image description here](https://camo.githubusercontent.com/4bbe93bae6c3edb9fb965e0e5e33b18e57e6cafd/68747470733a2f2f692e696d6775722e636f6d2f4f6f4f6f6755652e676966)
我们可以在”Build Settings”->”Enable Bitcode”选项中看到：
用 Xcode 7+ 新建一个 iOS 程序时， bitcode 选项默认是设置为YES的。现在需要改成NO。
如果我们开启了 bitcode ，在提交包时，下面这个界面也会有个 bitcode 选项：
![enter image description here](https://camo.githubusercontent.com/c2da48f01fef3bb51e209acecc376da707536481/687474703a2f2f6936302e74696e797069632e636f6d2f35623271376d2e6a7067)
这里有一个坑，目前 Xcode 处理 Embedded Binaries 时还有些问题，解决办法是，上图中左下角的两个选项不要同时勾选：
相关讨论见： [Missing BCSymbolMap for AppStore Submission - Xcode7B5](https://forums.developer.apple.com/thread/14729)
用 AD_HOC 打测试包时，也有相应的 bitcode 选项：
![enter image description here](https://camo.githubusercontent.com/94a3159d5779b67d37faeadc2333b7aafbf2b41d/687474703a2f2f6936342e74696e797069632e636f6d2f38766f7370692e6a7067)
那么 SDK 厂商如何支持 bitcode 呢？需要在 Xcode7上重新编译，确保默认开启的 bitcode 没有去主动关闭。
但是如果仅仅是编译一下，则会出现下类似的如下警告：
![enter image description here](https://camo.githubusercontent.com/54acf75a862a7fdc8facb2adab8c1eb1990c1e03/687474703a2f2f696d61676531372d632e706f636f2e636e2f6d79706f636f2f6d7970686f746f2f32303135303932382f31372f313733333838373234323031353039323831373134333130362e6a70673f31343632783132305f313230)
> 
ld: warning: full bitcode bundle could not be generated because 'Lookback(Lookback.o)' was built only with bitcode marker. The library must be generated from Xcode archive build with bitcode enabled (Xcode setting
 ENABLE_BITCODE)
警告的消除步骤：
模拟器、真机分开打包，SDK在build的时候，让模拟器与真机分开build，模拟器不设置bitcode的参数，真机的加上，然后再合起来。（“合起来”指的是指令集，好比 x86_64 i386 跟 armv7 arm64合起来。）用命令行打包的话 加上这个参数OTHER_CFLAGS=“-fembed-bitcode”。
![enter image description here](https://camo.githubusercontent.com/ccb8fa89b3ae07c00ff8711b79ea129951d121be/68747470733a2f2f63646e2d696d616765732d312e6d656469756d2e636f6d2f6d61782f3830302f312a52484c75373451795434444e714b592d376c6f5f36672e706e67)
详情可移步：[How do I xcodebuild a static library with Bitcode enabled?](http://stackoverflow.com/a/31486233/3395008)
同时切记，为 release 状态设置 `BITCODE_GENERATION_MODE=bitcode` ，开启 
```
full
 bitcode
```
 模式，否则会报错误：`Failed to verify bitcode in XXX.framework` :
![enter image description here](https://camo.githubusercontent.com/14e21ac9756618e3be71f1235ee4afa43607acd6/687474703a2f2f6936342e74696e797069632e636f6d2f326378756b69782e6a7067)
![enter image description here](https://camo.githubusercontent.com/5d4aa4546fd4f840b59d46dc1d9e25a9914296ed/68747470733a2f2f63646e2d696d616765732d312e6d656469756d2e636f6d2f6d61782f3830302f312a6364396d764a37424a435735317a5853676c776842512e706e67)
详见：
- [Static Libraries,
 Frameworks, and Bitcode](https://medium.com/@heitorburger/static-libraries-frameworks-and-bitcode-6d8f784478a9#.eq7rl2msf) 。
- [Failed to verify bitcode
 in Alamofire](https://github.com/Alamofire/Alamofire/issues/835)
- 用 xcodebuild 添加 bitcode 支持 [How
 do I xcodebuild a static library with Bitcode enabled?](http://stackoverflow.com/a/31486233/3395008)
更多信息，请移步
- 
[bitcode
 苹果官方文档](https://developer.apple.com/library/prerelease/watchos/documentation/IDEs/Conceptual/AppDistributionGuide/AppThinning/AppThinning.html#//apple_ref/doc/uid/TP40012582-CH35-SW2)
- 
WWDC 2015 Session 102: ["Platforms State of the Union"](https://developer.apple.com/videos/wwdc/2015/?id=102)
![enter image description here](https://camo.githubusercontent.com/44664c879bb780d18f3984ae94ea4bce73b6c35f/687474703a2f2f6d6f62696c65666f72776172642e6e65742f77702d636f6e74656e742f75706c6f6164732f323031352f30362f53637265656e2d53686f742d323031352d30362d31322d61742d362e35372e35342d504d2d363937783335312e706e67)
## [](https://github.com/ChenYilong/iOS9AdaptationTips#5demo3---ios9-url-scheme-适配_引入白名单概念)5.Demo3---iOS9 URL Scheme 适配_引入白名单概念
[WWDC 2015 Session 703: "Privacy and Your App](https://developer.apple.com/videos/wwdc/2015/?id=703)（
 时间在30：18左右）关于 `URL scheme` 的介绍，指出：
![enter image description here](https://camo.githubusercontent.com/c94995d0a6d10b903949f129075e39bd79b2fafd/68747470733a2f2f692e696d6775722e636f6d2f324878575171712e706e67)
也就是说：在iOS9中，如果使用 `canOpenURL:` 方法，该方法所涉及到的 
```
URL
 scheme
```
 必须在"Info.plist"中将它们列为白名单，否则不能使用。key叫做LSApplicationQueriesSchemes ，键值内容是
```
<key>LSApplicationQueriesSchemes</key>
<array>
 <string>urlscheme</string>
 <string>urlscheme2</string>
 <string>urlscheme3</string>
 <string>urlscheme4</string>
</array>
```
白名单上限是50个：
[WWDC 2015 Session 703: "Privacy and Your App](https://developer.apple.com/videos/wwdc/2015/?id=703)）有说明：
> 
“So for apps that are linked before iOS 9 and are running on iOS 9, they will be given 50 distinct URL schemes.” -- WWDC 2015 session 703 Privacy and Your App
~~然而，我们却发现了一件意外的事： 当我们在 iOS9-beta（截至本文发布时，iOS9正式版还未发布）中，使用 `openURL:` 方法时，不在白名单中的 URL 会报错 > “This app is not allowed to query for scheme xxx” 。 无论是官方文档还是 WWDC 的视频中都没有提及 `openURL:` 方法的这一变动，所以猜测这是 beta 版本一个 bug ，截至本文发布时，iOS9正式版还未发布，期望在正式版中能得以修复。在此之前，可通过将 `openURL:` 用到的 ```URL scheme``` 列入白名单来解决这个 bug 。（经测试：iOS9 beta5中已经修复）~~
iOS9中 `openURL:` 方法没有什么实质性的变化，仅仅多了一个确认动作：
![enter image description here](https://camo.githubusercontent.com/8e96a2b0a4ce66dbf8a29a17bd0c6eac09fe6720/687474703a2f2f6935372e74696e797069632e636f6d2f387a6a6833352e6a7067)
苹果为什么要这么做？
在 iOS9 之前，你可以使用 `canOpenURL:` 监测用户手机里到底装没装微信，装没装微博。但是也有一些别有用心的 App ，这些 App 有一张常用 App 的 
```
URL
 scheme
```
，然后他们会多次调用`canOpenURL:` 遍历该表，来监测用户手机都装了什么 App ，比如这个用户装了叫“大姨妈”的App，你就可以知道这个用户是女性，你就可以只推给这个用户女性用品的广告。这是侵犯用户隐私的行为。
这也许就是原因。
本项目中给出了一个演示用的 Demo ，仓库的文件夹叫“Demo3_iOS9URLScheme适配_引入白名单概念”，Demo引用自[LSApplicationQueriesSchemes-Working-Example](https://github.com/gatzsche/LSApplicationQueriesSchemes-Working-Example)
Demo结构如下：
![enter image description here](https://camo.githubusercontent.com/2eab2f42b107e98ef97cd9cabc431e71829319f0/687474703a2f2f6936312e74696e797069632e636f6d2f326879797571762e6a7067)
主要演示的情景是这样的：
假设有两个App： weixin(微信) and 我的App. 我的App 想监测 weixin(微信) 是否被安装了. "weixin(微信)" 在 info.plist 中定义了 URL scheme :
```
<key>CFBundleURLTypes</key>
<array>
    <dict>
        <key>CFBundleURLSchemes</key>
        <array>
            <string>weixin</string>
        </array>
    </dict>
</array>
```
我的App 想监测 weixin(微信) 是否被安装了 ：
```
[[UIApplication sharedApplication]
                    canOpenURL:[NSURL URLWithString:@"weixin(微信)://"]];
```
即使你安装了微信，在iOS9中，这有可能会返回NO：
因为你需要将 "weixin(微信)" 添加到 “我的App” 的 info.plist 文件中：
```
<key>LSApplicationQueriesSchemes</key>
<array>
    <string>weixin</string>
</array>
```
（以上只是为了演示，实际开发中，你不仅需要添加“weixin”还需要“wechat”这两个。具体下文给出表格）
~~关于 `openURL:` 这个问题，可在 Demo3 中自行测试，如果该 bug 修复了的话，请私信[微博@iOS程序犭袁](http://weibo.com/luohanchenyilong/)，我再来更新本文。（经测试：iOS9 beta5中已经修复）~~
另外，推荐一篇[博文](http://awkwardhare.com/post/121196006730/quick-take-on-ios-9-url-scheme-changes)，其中最关键的是以下部分：
> 
If you call the “canOpenURL” method on a URL that is not in your whitelist, it will return “NO”, even if there is an app installed that has registered to handle this scheme. A “This app is not allowed to query for
 scheme xxx” syslog entry will appear.
~~> If you call the “openURL” method on a URL that is not in your whitelist, it will fail silently. A “This app is not allowed to query for scheme xxx” syslog entry will appear.~~
### [](https://github.com/ChenYilong/iOS9AdaptationTips#常见-url-scheme)常见 URL Scheme
如果想一次性集成最常用的微信、新浪微博、QQ、支付宝四者的白名单，则配置如下：
 <key>LSApplicationQueriesSchemes</key>
<array>
    <!-- 微信 URL Scheme 白名单-->
    <string>wechat</string>
    <string>weixin</string>
    <!-- 新浪微博 URL Scheme 白名单-->
    <string>sinaweibohd</string>
    <string>sinaweibo</string>
    <string>sinaweibosso</string>
    <string>weibosdk</string>
    <string>weibosdk2.5</string>
    <!-- QQ、Qzone URL Scheme 白名单-->
    <string>mqqapi</string>
    <string>mqq</string>
    <string>mqqOpensdkSSoLogin</string>
    <string>mqqconnect</string>
    <string>mqqopensdkdataline</string>
    <string>mqqopensdkgrouptribeshare</string>
    <string>mqqopensdkfriend</string>
    <string>mqqopensdkapi</string>
    <string>mqqopensdkapiV2</string>
    <string>mqqopensdkapiV3</string>
    <string>mqzoneopensdk</string>
    <string>wtloginmqq</string>
    <string>wtloginmqq2</string>
    <string>mqqwpa</string>
    <string>mqzone</string>
    <string>mqzonev2</string>
    <string>mqzoneshare</string>
    <string>wtloginqzone</string>
    <string>mqzonewx</string>
    <string>mqzoneopensdkapiV2</string>
    <string>mqzoneopensdkapi19</string>
    <string>mqzoneopensdkapi</string>
    <string>mqzoneopensdk</string>
    <!-- 支付宝  URL Scheme 白名单-->
    <string>alipay</string>
    <string>alipayshare</string>
</array>
plist 文件看起来会是这样的：
![enter image description here](https://camo.githubusercontent.com/127b04db3d6a52590e8794d39444d29851419be3/687474703a2f2f6935382e74696e797069632e636f6d2f6535707965652e6a7067)
其他平台可在下面的列表中查询： 各平台OpenURL白名单说明
|平台名称|URL Schem|补充说明|
|----|----|----|
|微信|wechat,weixin||
|支付宝|alipay,alipayshare||
|QQ|mqqOpensdkSSoLogin,mqqopensdkapiV2,mqqopensdkapiV3,wtloginmqq2,mqq,mqqapi||
|QZONE|mqzoneopensdk,mqzoneopensdkapi,mqzoneopensdkapi19,mqzoneopensdkapiV2,mqqOpensdkSSoLogin,mqqopensdkapiV2,mqqopensdkapiV3,wtloginmqq2,mqqapi,mqqwpa，mqzone，mqq|[注:若同时使用QQ和QZONE,则直接添加本格即可]|
|新浪微博|sinaweibo,sinaweibohd,sinaweibosso,sinaweibohdsso,weibosdk,weibosdk2.5|[后两个若导入新浪SDK则需要]|
|豆瓣|无需配置||
|开心网|无需配置||
|易信|yixin,yixinopenapi||
|Google+|googlechrome,googlechrome-x-callback,hasgplus4,com.google.gppconsent,com.google.gppconsent.2.2.0,com.google.gppconsent.2.3.0,com.google.gppconsent.2.4.0,com.google.gppconsent.2.4.1||
|人人网|renrenapi,renrenios,renreniphone,renren,||
|Facebook|见下文||
|Twitter|无需配置||
|Pocket|pocket-oauth-v1||
|Pinterest|pinit||
|Instagram|instagram||
|WhatsApp|whatsapp||
|Line|line||
|KakaoTalk|kakaolink||
|KaokaoStory|storylink||
|LinkedIn|无需配置||
|Tumblr|无需配置||
|非平台类|无需配置|( 如短信，复制，邮件等)|
另外， Facebook 的URL Scheme白名单需要注意：
如果 SDK 版本低于 4.5 应补充
```
<key>LSApplicationQueriesSchemes</key>
<array>
    <string>fbapi</string>
    <string>fbapi20130214</string>
    <string>fbapi20130410</string>
    <string>fbapi20130702</string>
    <string>fbapi20131010</string>
    <string>fbapi20131219</string>    
    <string>fbapi20140410</string>
    <string>fbapi20140116</string>
    <string>fbapi20150313</string>
    <string>fbapi20150629</string>
    <string>fbauth</string>
    <string>fbauth2</string>
    <string>fb-messenger-api20140430</string>
</array>
```
如果使用 FBSDKMessengerShareKit，还要加上
```
<string>fb-messenger-platform-20150128</string>
<string>fb-messenger-platform-20150218</string>
<string>fb-messenger-platform-20150305</string>
```
如果使用SDK版本高于4.6，则只需要加上
```
<key>LSApplicationQueriesSchemes</key>
<array>
        <string>fbapi</string>
        <string>fb-messenger-api</string>
        <string>fbauth2</string>
        <string>fbshareextension</string>
</array>
```
[参考链接](https://developers.facebook.com/docs/ios/ios9) 。
### [](https://github.com/ChenYilong/iOS9AdaptationTips#q-a-2)Q-A
Q：我用xcode7编译的app，如果不在plist里面加scheme，ios9下qq就会不显示，因为我用了qqsdk里的判断是否安装qq的方法，我要是直接下载app store上的，没有加scheme，qq也是能显示。
A：本文中所罗列的新特性，多数情况下指的是 iOS9.X-SDK 新特性，AppStore 的版本是基于 iOS8.X-SDK或 iOS7.X-SDK，所以并不受 iOS9新特性约束。也就是说：Xcode7给iOS8打设备包不需要白名单也能调用“canOpenURL” ，Xcode7给iOS9设备打的包则不然，Xcode7和iOS9缺一不可，才需要适配URL Scheme。
那么，如何确认自己项目所使用的 SDK？在Targets->Build Setting-->Architectures
![enter image description here](https://camo.githubusercontent.com/203388f8139ffcf2332becd6fc6c359c35456a46/687474703a2f2f6935382e74696e797069632e636f6d2f616d736139752e6a7067)
Q：我们自己的应用跳到微信、支付宝、微博等的URLScheme是固定几个，但是从微信、支付宝、微博跳回到我们的应用的URLScheme可能是成千上万个，那他们那些大厂是如何做这个白名单？
A：白名单策略影响的仅仅是 canOpenURL: 接口，OpenURL: 不受影响，这些大厂只调用 openURL: 所以不受 iOS9 的影响。
Q：文中提到了设置白名单的原因，然而，如果这些别有用心的APP在它自己的白名单列出它关心的APP, 然后依次调用canOpenURL来检测，照样可以监控用户都安装了哪些APP啊？所以我依然不明白苹果这样做得原因。
A：白名单的数目上限是50个。苹果这样子做，使得最多只能检测50个App。
Q：按照文中的适配方法，error原因就没有了的确没问题了，但是还是会打印如下信息：
 -canOpenURL: failed for URL: "XXXXXXXXXX" - error: "(null)"。
A：这个模拟器的一个 bug，无论使用iOS9的真机还是模拟器均出现该问题，估计 Xcode 后续的升级中会修复掉。
那如何判断日志究竟是 Xcode bug 造成的还是没有适配造成的？看error的值，如果是null，则是 bug。（2015-09-21更）
## [](https://github.com/ChenYilong/iOS9AdaptationTips#6-ipad适配slide-over-和-split-view)6. iPad适配Slide Over 和 Split View
![enter image description here](https://camo.githubusercontent.com/b1dce6c06ef1e6b3298e71b7b21b4f9e46d464fa/687474703a2f2f63646e312e746e7763646e2e636f6d2f77702d636f6e74656e742f626c6f67732e6469722f312f66696c65732f323031352f30362f65772d2e676966)
【iPad适配Slide Over 和 Split View】 若想适配multi tasking特性，唯一的建议：弃纯代码，改用storyboard、xib，纵观苹果WWDC所有Demo均是如此：
- 
[Mysteries of Auto Layout, Part 1](https://developer.apple.com/videos/wwdc/2015/?id=218)
- 
[What's New in Storyboards](https://developer.apple.com/videos/wwdc/2015/?id=215)
- 
[Implementing UI Designs in Interface Builder](https://developer.apple.com/videos/wwdc/2015/?id=407)
- 
[Getting Started with Multitasking on iPad in iOS
 9](https://developer.apple.com/videos/wwdc/2015/?id=205)
- 
[Optimizing Your App for Multitasking on iPad in iOS](https://developer.apple.com/videos/wwdc/2015/?id=212)
## [](https://github.com/ChenYilong/iOS9AdaptationTips#7字体间隙变大导致-ui-显示异常)7.字体间隙变大导致 UI 显示异常
iOS8中，字体是Helvetica，中文的字体有点类似于“华文细黑”。只是苹果手机自带渲染，所以看上去可能比普通的华文细黑要美观。iOS9中，中文系统字体变为了专为中国设计的“苹方” 有点类似于一种word字体“幼圆”。字体有轻微的加粗效果，并且最关键的是字体间隙变大了！
所以很多原本写死了width的label可能会出现“...”的情况：
|情况|显示|解释|
|----|----|----|
|XIB|将 label 的 width 写死|下面这两张图也可以直观的看出同一个界面，同一个label的变化。|
|iOS8|![enter image description here](https://camo.githubusercontent.com/4cbd1a8a37329561d33f8151997f81b2dfcc18fb/687474703a2f2f696d61676573323031352e636e626c6f67732e636f6d2f626c6f672f3731373830392f3230313530392f3731373830392d32303135303931393232333930333437362d3137363834343631392e706e67)|正常|
|iOS9|![enter image description here](https://camo.githubusercontent.com/a69ad197c175b6edd5f66ae993696a7bdf3348fa/687474703a2f2f696d61676573323031352e636e626c6f67732e636f6d2f626c6f672f3731373830392f3230313530392f3731373830392d32303135303931393232333931383130312d313931373731373134342e706e67)|最后四位数字、、、|
如果不将 label 的 width 写死，仅仅添加左端约束则右端的四个数字会越界
|情况|显示|解释|
|----|----|----|
|XIB|![enter image description here](https://camo.githubusercontent.com/113a6013302a4aa93234bdedec84318eea3fcfc4/687474703a2f2f6936302e74696e797069632e636f6d2f323932723432382e6a7067)|如果仅仅添加左端约束|
|iOS8|![enter image description here](https://camo.githubusercontent.com/d4f97f344034cff949883850400892e755d821f2/687474703a2f2f6935382e74696e797069632e636f6d2f32766a3932626e2e6a7067)|正常|
|iOS9|![enter image description here](https://camo.githubusercontent.com/4b86ab2ed1e2a99602522e0a54485a1dc2a0f743/687474703a2f2f6936322e74696e797069632e636f6d2f32637a617131762e6a7067)|“3199”这四个数字越界了|
所以为了在界面显示上不出错，就算是固定长度的文字也还是建议使用sizetofit 或者ios向上取整 ceilf() 或者提前计算：
CGSize size = [title sizeWithAttributes:@{NSFontAttributeName: [UIFont systemFontOfSize:14.0f]}];
CGSize adjustedSize = CGSizeMake(ceilf(size.width), ceilf(size.height));
## [](https://github.com/ChenYilong/iOS9AdaptationTips#8升级-xcode7-后的崩溃与警告)8.升级 Xcode7 后的崩溃与警告
### [](https://github.com/ChenYilong/iOS9AdaptationTips#旧版本新浪微博-sdk-在-ios9-上会导致的-crash)旧版本新浪微博 SDK 在 iOS9 上会导致的 Crash
 app was compiled with optimization - stepping may behave oddly; variables may not be available
打印出来这句话，然后崩溃。多是启动的过程中程序就崩溃。
在iOS9下，新浪微博SDK里面使用的 JSONKit 在部分机型可能导致崩溃。崩溃信息如下图。
![enter image description here](https://camo.githubusercontent.com/a7b70b2ec7126e54aa4e94d66b2c819fb4f19bff/687474703a2f2f77696b692e6d6f622e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031352f30392f34303632313330432d313133382d343335322d383941462d4535313846313839413835312e706e67)
解决：更新新浪微博SDK，新浪的SDK最新版做了对iOS9兼容。
### [](https://github.com/ChenYilong/iOS9AdaptationTips#ios9-下使用-masonry-会引起崩溃的一种情况)iOS9 下使用 Masonry 会引起崩溃的一种情况
在 iOS8（及以前）我们有这样的经验：
> 
`leading 与 left` 、 
```
trailing
 与 right
```
 在正常情况下是等价的 但是当一些布局是从右至左时(比如阿拉伯文?没有类似的经验) 则会对调，换句话说就是基本可以不理不用，用left和right就好了
（摘自 [《Masonry介绍与使用实践(快速上手Autolayout)》](http://adad184.com/2014/09/28/use-masonry-to-quick-solve-autolayout/) ）
但在概念里，还是一直将 leading 与 left 划为等号，这样做在 iOS8（及以前）上是正常的，但在 iOS9 上这样的观念可能会引起崩溃，比如：
 make.left.equalTo(self.mas_leading).offset(15);
应该为：
 make.left.equalTo(self.mas_left).offset(15);
同理 mas_training 也需要改为right
同时也有人反馈说也需要作如下调整否则也会崩溃：
toplayoutGuide 替换成 mas_toplayoutguide bottomlayoutguide 替换成 mas_bottomlayoutguide
而且使用类似 `make.top.equalTo(topView.mas_baseline).with.offset(5);` 涉及 `mas_baseline` 的语句也会引起崩溃。
暂时的解决方案是
使用 `make.top.equalTo(self.mas_topLayoutGuide).with.offset(5);` 来替换原来的`self.topLayoutGuide.mas_baseline` 反正效果是一样的
原来的代码：
[self.headerView mas_makeConstraints:^(MASConstraintMaker *make) {
    UIView *topView = (UIView *)self.topLayoutGuide;
    make.top.equalTo(topView.mas_baseline).with.offset(5);
    make.leading.equalTo(self.view.mas_leading).with.offset(10);
    make.right.equalTo(self.view.mas_right).with.offset(-10);
    make.height.equalTo(@34);
}];
修改后：
[self.headerView mas_makeConstraints:^(MASConstraintMaker *make) {
    make.top.equalTo(self.mas_topLayoutGuide).with.offset(5);
    make.left.equalTo(self.view.mas_left).with.offset(10);
    make.right.equalTo(self.view.mas_right).with.offset(-10);
    make.height.equalTo(@34);
}];
### [](https://github.com/ChenYilong/iOS9AdaptationTips#xcode-升级后旧的状态栏的样式设置方式会引起警告)Xcode 升级后，旧的状态栏的样式设置方式会引起警告
<Error>: CGContextSaveGState: invalid context 0x0. If you want to see the backtrace, please set CG_CONTEXT_SHOW_BACKTRACE environmental variable.
<Error>: CGContextTranslateCTM: invalid context 0x0. If you want to see the backtrace, please set CG_CONTEXT_SHOW_BACKTRACE environmental variable.
<Error>: CGContextRestoreGState: invalid context 0x0. If you want to see the backtrace, please set CG_CONTEXT_SHOW_BACKTRACE environmental variable.
出错原因：设置 app 的状态栏样式的时候，使用了旧的方式，在 info.plist 里面的 `View controller-based status bar appearance` 默认会为 YES，即使不设置也是 YES，但一般 iOS6 的时候为了设置状态栏样式，需要将其设为NO，iOS7，8也兼容，但是到了iOS9
 就会报警告。
解决办法：
删除原先的设置代码，通常老的设置方式是这样的：
//设置状态栏的白色
    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
删除的原因见下：
// Setting the statusBarStyle does nothing if your application is using the default UIViewController-based status bar system.
@property(readwrite, nonatomic) UIStatusBarStyle statusBarStyle NS_DEPRECATED_IOS(2_0, 9_0, "Use -[UIViewController preferredStatusBarStyle]");
- (void)setStatusBarStyle:(UIStatusBarStyle)statusBarStyle animated:(BOOL)animated NS_DEPRECATED_IOS(2_0, 9_0, "Use -[UIViewController preferredStatusBarStyle]");
修改方式是在 `Info.plist` 文件中做如下修改：
将 `View controller-based status bar appearance` 删除（默认为 YES），或设置为YES：
对应的 plist 里的 XML源码：
 <key>UIViewControllerBasedStatusBarAppearance</key>
    <true/>
看起来长这样：
![enter image description here](https://camo.githubusercontent.com/0da5feace4f8a429a18dfa96fbe068942cdcbeec/687474703a2f2f6936312e74696e797069632e636f6d2f6a72736a6e642e6a7067)
然后使用新的方式来实现状态栏的样式：
- (UIStatusBarStyle)preferredStatusBarStyle;
- (UIViewController *)childViewControllerForStatusBarStyle;
- (void)setNeedsStatusBarAppearanceUpdate
比如，你想将状态栏设置为白色，就可以这样写：
//设置状态栏的白色
 -(UIStatusBarStyle)preferredStatusBarStyle
{
    return UIStatusBarStyleLightContent;
}
记得要 clean 下或者删除应用程序重新运行
参考链接： [How to change Status Bar text color in iOS 7](http://stackoverflow.com/a/17768797/3395008)
#### [](https://github.com/ChenYilong/iOS9AdaptationTips#demo4---navigationcontroller状态栏样式新的设置方法)Demo4---navigationController状态栏样式新的设置方法
如果你按照上面的方法设置了，但还是不行。八成是 rootViewController 设置的问题，你必须设置 rootViewController，编译器才会去 rootViewController 中重载 preferredStatusBarStyle 方法。
另外当你在 appdelegate 中将 navigationController 设为 rootViewController 的时候：
     self.window.rootViewController = self.navigationController;
因为 rootViewController 变为了 navigationController，你在 ViewController 里重写 preferredStatusBarStyle 方法是不会起作用的。所以最好的方法是：
 - (void)viewDidLoad
{
    [superviewDidLoad];
    self.title = @"微博@iOS程序犭袁";
    self.navigationController.navigationBar.barStyle = UIBarStyleBlack;
}
如果你还是想重写 preferredStatusBarStyle 方法来达到作用，那最好使用分类来解决：
第二种方法：
.h文件：
////  UINavigationController+StatusBarStyle.h//  微博@iOS程序犭袁////  Created by  https://github.com/ChenYilong/iOS9AdaptationTips/ on 15/6/8.//  Copyright (c) 2015年   http://weibo.com/luohanchenyilong/  . All rights reserved.//
#import<UIKit/UIKit.h>@interfaceUINavigationController (StatusBarStyle)
@end
.m文件：
////  UINavigationController+StatusBarStyle.m//  微博@iOS程序犭袁////  Created by  https://github.com/ChenYilong/iOS9AdaptationTips/ on 15/6/8.//  Copyright (c) 2015年   http://weibo.com/luohanchenyilong/  . All rights reserved.//
#import"UINavigationController+StatusBarStyle.h"@implementationUINavigationController (StatusBarStyle)
- (UIStatusBarStyle)preferredStatusBarStyle
{
    //also you may add any fancy condition-based code herereturn UIStatusBarStyleLightContent;
}
@end
但最好不要通过 Category 重写 `preferredStatusBarStyle` 的方式来指定 status bar 样式。按照苹果官方的解释：
> 
If the name of a method declared in a category is the same as a method in the original class, or a method in another category on the same class (or even a superclass), the behavior is undefined as to which method
 implementation is used at runtime. This is less likely to be an issue if you’re using categories with your own classes, but can cause problems when using categories to add methods to standard Cocoa or Cocoa Touch classes.
所以推荐第一种的方法，不推荐第二种。
我在仓库里给出了 navigation 的两种设置方法，见Demo4。
第三种方法：
 - (UIViewController *)childViewControllerForStatusBarStyle;
按照苹果官方的解释：
> 
If your container view controller derives its status bar style from one of its child view controllers, implement this method and return that child view controller. If you return nil or do not override this method,
 the status bar style for self is used. If the return value from this method changes, call the setNeedsStatusBarAppearanceUpdate method.
调用 `setNeedsStatusBarAppearanceUpdate` 时，系统默认会去调用application.rootViewController的 `preferredStatusBarStyle` 方法，所以这时候当前自己的
 viewController 的 `preferredStatusBarStyle` 方法根本不会被调用。
这个接口很重要，这种情况下 `childViewControllerForStatusBarStyle` 就有用了。一般我们常用 navigationController 作为 rootViewController，利用此接口便可以很方便自订各个 viewController
 的 statusBarStyle。 子类化一个 navigationController，并且 override `childViewControllerForStatusBarStyle`
 - (UIViewController * _Nullable)childViewControllerForStatusBarStyle {
     return self.topViewController;
 }
意思就是说不要调用我自己 `application.rootViewController（navigationController）` 的 `preferredStatusBarStyle`方法，去调用｀childViewControllerForStatusBarStyle｀
 回传的 UIViewController 的 `preferredStatusBarStyle`。这里回传 self.topViewController 就可以保证当前显示的 viewController 的 `preferredStatusBarStyle` 会被系统调用且正确的显示。
参考链接：
- [preferredStatusBarStyle isn't called--For anyone using a UINavigationController:](http://stackoverflow.com/a/19513714/3395008)
- [How to hide iOS status bar](http://stackoverflow.com/a/18980833/3395008)
### [](https://github.com/ChenYilong/iOS9AdaptationTips#xcode7-在-debug-状态下也生成-dsym-文件引起的警告)Xcode7 在 debug 状态下也生成 .dSYM 文件引起的警告
Xcode6 的工程升级到 Xcode7上来，会报警告：
![enter image description here](https://camo.githubusercontent.com/5f98d66a10ef190cd00b2d5d9dccde61ad2d9f1c/687474703a2f2f6935372e74696e797069632e636f6d2f3261357a7569612e6a7067)
这是 debug 编译时导出符号文件出现的告警，
然而新建的Xcode7工程不会有该问题。
解决方法是让 debug 编译的时候不生成符号文件：
![enter image description here](https://camo.githubusercontent.com/cb0c5a5dc923b52f6ef85dfdeee0418f93b633ac/687474703a2f2f6936302e74696e797069632e636f6d2f326532337179702e6a7067)
### [](https://github.com/ChenYilong/iOS9AdaptationTips#xcode7-无法使用-8x-系统的设备调试一运行就报错-there-is-an-intenal-api-error)Xcode7 无法使用 8.x 系统的设备调试，一运行就报错 `there is an intenal API error`
![enter image description here](https://camo.githubusercontent.com/41e5211203a62e4b8dbd3c9511a96e20ad418ad2/687474703a2f2f63646e2e636f63696d672e636f6d2f6262732f6174746163686d656e742f4669645f32312f32315f3239363330355f3932303934643661373165353837612e706e67)
`Xcode7` 调试 `iOS8.x` 的真机，需要确保项目名改为英文，中间含有中文会报错 
```
there
 is an intenal API error
```
按照下面的步骤检查：
bulid settings -> packaging -> product name
### [](https://github.com/ChenYilong/iOS9AdaptationTips#使用了-html-的-iframe-元素可能导致无法从-safari-跳转至-app)使用了 HTML 的 iframe 元素可能导致无法从 Safari 跳转至 App
我们都知道，从网易新闻分享一条新闻到QQ，然后从QQ中打开链接再用safari打开链接，在iOS8上，这个时候会跳转到网易新闻App。但是现在（2015年09月23日）版本的网易新闻在 iOS9 就不能正常跳转，会跳转到 App Store 页面并提示要不要打开 App Store。
这是很可能是因为使用了 HTML 的 iframe 元素，并将自定义的链接放进了该元素中
举例说明：
![enter image description here](https://camo.githubusercontent.com/533bc8eaef7fd45ecac0c1637562c283358feec1/687474703a2f2f6936312e74696e797069632e636f6d2f327762766f6b382e6a7067)
我之前写的一个 Demo： [模仿 《简书 App》 的效果:在html中跳转到App中的对应页面,并能从App跳转到原来的网址](https://github.com/ChenYilong/CYLExternalURL)，在例子中直接调用自定义链接在 iOS9上是可以跳转到 App 中的，然而，如果用
 iframe 元素包起来就会变不可用。
参考链接：
- [HTML 的iframe 标签](http://www.w3school.com.cn/tags/tag_iframe.asp)
- [iOS 9 safari iframe
 src with custom url scheme not working](http://stackoverflow.com/questions/31891777/ios-9-safari-iframe-src-with-custom-url-scheme-not-working)
### [](https://github.com/ChenYilong/iOS9AdaptationTips#ios9锁屏控制台会打印警告)iOS9锁屏控制台会打印警告
加入运行如下示例代码：
- (void)viewDidLoad {
    [superviewDidLoad];
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_async(queue, ^(void) {
        //在这个10秒内锁屏NSLog(@"准备休眠");
        sleep(10);
        NSLog(@"打印成功");
    });
}
应用运行过程中锁屏，总是会出现以下提示：
** -[UIApplication _handleNonLaunchSpecificActions:forScene:withTransitionContext:completion:] ** unhandled action -> <FBSSceneSnapshotAction: 0x16da76c0> {
    handler = remote;
    info = <BSSettings: 0x16d80e50> {
        (1) = 5;
    };
}
当应用处于空闲状态时（无网络请求）锁屏对于用户而言并无较大影响，
但是当应用在执行某个异步任务时（比如下拉刷新一下列表）锁屏，重新解锁进入就可能会发现异步任务失败，控制台也会提示 Error 信息：
** -[UIApplication _handleNonLaunchSpecificActions:forScene:withTransitionContext:completion:] ** unhandled action -> <FBSSceneSnapshotAction: 0x16da76c0> {
    handler = remote;
    info = <BSSettings: 0x16d80e50> {
        (1) = 5;
    };
}
error in __connection_block_invoke_2: Connection interrupted
以上情况不易复现，但确有发生。
在 iOS8 系统下测试并未发现此问题。
对此并未找到合理的解释和对应的解决办法，如果你有解决方法，欢迎提 PR !
### [](https://github.com/ChenYilong/iOS9AdaptationTips#在didfinishlaunchingwithoptions结束后还没有设置window的rootviewcontroller会导致崩溃)在`didFinishLaunchingWithOptions`结束后还没有设置window的`rootViewController`会导致崩溃
iOS9 不允许在 `didFinishLaunchingWithOptions` 结束了之后，还没有设置 window 的 `rootViewController` 。
 也许是 Xcode7 的编译器本身就不支持。
崩溃时的控制台日志提示：
*** Assertion failure in -[UIApplication _runWithMainScene:transitionContext:completion:], /BuildRoot/Library/Caches/com.apple.xbs/Sources/UIKit_Sim/UIKit-3505.16/UIApplication.m:3294
***  Terminating app due to uncaught exception 'NSInternalInconsistencyException', reason: 'Application windows are expected to have a root view controller at the end of application launch'
*** First throw call stack:
/*省略*/
libc++abi.dylib: terminating with uncaught exception of type NSException
(lldb) 
解决的方法是先设初始化个值，之后再赋值替换掉：
UIWindow *window = [[UIWindow alloc] initWithFrame:[UIScreenmainScreen].bounds];
window.rootViewController = [[UIViewController alloc] init];
尤其注意一种情况，在 iOS8以前，我们有时候会通过在 AppDelegate 中添加另一个 UIWindow ，并修改其 Level 来达到 addSubview 的效果，因而也不设置 window 的 `rootViewController` ，而是把它直接以视图的形式展示了，则在
 iOS8 上是警告，在 iOS9 上就崩溃了。
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.backgroundColor = [UIColor yellowColor];
    [self.window makeKeyAndVisible];
    UIWindow *normalWindow = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    normalWindow.backgroundColor = [UIColor blueColor];
    normalWindow.windowLevel = UIWindowLevelAlert;
    [normalWindow makeKeyAndVisible];
    returnYES;
}
这种情况，在 `didFinishLaunchingWithOptions` 需要修改原来的策略，将第二个 window 类型改为其他类型，比如 viewController 类型、navigation 类型、tabbarController 类型等。
## [](https://github.com/ChenYilong/iOS9AdaptationTips#9demo5demo6----搜索-api)9.Demo5、Demo6--- 搜索 API
导入两个 framework，
然后像设置tableView 的 cell 一样设置下每一个“搜索元素”，搜索元素的组成如下：
![enter image description here](https://camo.githubusercontent.com/2d043ff8596ea7676fbd15e2a2c375ed349afbfc/687474703a2f2f6935372e74696e797069632e636f6d2f313434623232772e6a7067)
详情见 Demo6 代码。
![enter image description here](https://camo.githubusercontent.com/a1c63ca25854417bb1211a280747653e1d8d74a1/687474703a2f2f696d61676531372d632e706f636f2e636e2f6d79706f636f2f6d7970686f746f2f32303135303932332f32312f31373333383837323432303135303932333231343733303031302e6769663f333730783638365f313130)
既然刚才说了搜索元素与 tableView 的 cell 非常相似：那么我们就展示一下如何让 tableView 与 CoreSpotlightSearch 进行结合：
详见 Demo6，Demo6 与 Demo5 的主要差异在于：在点击搜索结果跳转到 App 后，还会进一步根据搜索的内容 push 到相应的详情页中：
![enter image description here](https://camo.githubusercontent.com/f6ea20dc685f3590baee86082dcd06a61f4f81a1/687474703a2f2f696d61676531372d632e706f636f2e636e2f6d79706f636f2f6d7970686f746f2f32303135303932342f30302f31373333383837323432303135303932343030313334303033352e6769663f333036783537325f313130)
## [](https://github.com/ChenYilong/iOS9AdaptationTips#10ios国际化问题当前设备语言字符串返回有变化)10.iOS国际化问题：当前设备语言字符串返回有变化。
```
NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
NSArray *allLanguage = [defaults objectForKey:@"AppleLanguages"];
NSString *currentLanguage = [allLanguage objectAtIndex:0];
NSLog(@"The current language is : %@", currentLanguage);
```
iOS 9 之前：以上返回结果：语言字符串代码。例如："zh-Hans"
iOS 9:以上返回结果：语言字符串代码 + 地区代码。例如："zh-Hans-US"
备注：
1.请注意判断当前语言类型，不要用以下形式的代码了，不然在iOS9上就会遇到坑。
```
if ([currentLanguage isEqualToString:@"zh-Hans"])
```
可以使用：
```
if ([currentLanguage hasPrefix:@"zh-Hans"])
```
另外：对于中文，语言有：
- 简体中文:zh-Hans
- 繁体中文:zh-Hant
- 香港中文:zh-HK
- 澳门中文:zh-MO
- 台湾中文:zh-TW
- 新加坡中文:zh-SG
备注：以上iOS9 当前语言字符串返回结果：语言字符串代码 + 地区代码。在某些情况下不是这样，本人手机型号：大陆版电信iPhone5S/A1533/16GB测试结果：zh-HK/zh-TW，在地区为"中国"、"中国香港"、"中国台湾"的时候，显示的还是zh-HK/zh-TW，一旦切换到其它地区，设备语言会自动的切换到中文繁体。请开发人员注意中文的问题！
## [](https://github.com/ChenYilong/iOS9AdaptationTips#11uitableview显示异常)11.UITableView显示异常
原本在 Xcode6 上完好的项目，在 Xcode7 上一编译， `tableView` 出了两个问题 ：
- 代码创建的 `tableView` 无法隐藏 cell 分割线
- `reloadData` 刷新失效；
### [](https://github.com/ChenYilong/iOS9AdaptationTips#代码创建的-tableview-无法隐藏-cell-分割线)代码创建的 `tableView` 无法隐藏 cell 分割线
iOS9 里面用到 tableView 突然跑出来了很多 cell 的分割线， 但是在用xib创建的 tableview，就不存在这个问题
解决方法是将设置分割线隐藏的方法 `self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;` 写在 `-layoutSubviews` 中：
-(void)layoutSubviews{
    [superlayoutSubviews];
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
}
也有人发现另一种方法，就是每次 reloadData 之前都进行一次设置：设置分割线隐藏，这样也可以解决：
    self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
   [self.tableView reloadData]
虽然也可以解决但是不推荐，这样写会给其他人造成困扰：不知所云。
### [](https://github.com/ChenYilong/iOS9AdaptationTips#reloaddata-刷新失效)`reloadData` 刷新失效
现象： `[tableView reloadData]` 无效，有一行 cell 明明改变了但是刷新不出来。
感觉可能是这个方法和某种新加的特性冲突了，猜测可能是 `reloadData` 的操作被推迟到下一个 `RunLoop` 执行最终失效。
解决的方法是，注释 `[tableView reloadData]` ，改用局部刷新：
[self.tableView reloadSections:[NSIndexSetindexSetWithIndex:0] withRowAnimation:UITableViewRowAnimationNone];
这两个推测均属 Xcode7 的bug，将来 Apple 肯定会修复。
### [](https://github.com/ChenYilong/iOS9AdaptationTips#基于http2的全新apns协议)基于HTTP/2的全新APNs协议
文章较长，单独成篇: [《基于HTTP/2的全新APNs协议》](https://github.com/ChenYilong/iOS9AdaptationTips/blob/master/%E5%9F%BA%E4%BA%8EHTTP2%E7%9A%84%E5%85%A8%E6%96%B0APNs%E5%8D%8F%E8%AE%AE/%E5%9F%BA%E4%BA%8EHTTP2%E7%9A%84%E5%85%A8%E6%96%B0APNs%E5%8D%8F%E8%AE%AE.md#%E5%A6%82%E4%BD%95%E5%88%9B%E5%BB%BA-universal-push-notification-client-ssl-%E8%AF%81%E4%B9%A6) 。
# [](https://github.com/ChenYilong/iOS9AdaptationTips#结束语)结束语
如果你在开发中遇到什么新的 iOS9 的坑，或者有什么适配细节本文没有提及，欢迎给本仓库提 pull request。也欢迎在[微博@iOS程序犭袁](http://weibo.com/luohanchenyilong/) 或在“iOS9开发学习交流群：515295083”中交流。
疏漏之处，可前往阅读下[这个网站](http://asciiwwdc.com/)，这里有每年 WWDC 演讲的英文记录。
Posted by [微博@iOS程序犭袁](http://weibo.com/luohanchenyilong/)
原创文章，版权声明：自由转载-非商用-非衍生-保持署名 | [Creative Commons BY-NC-ND 3.0](http://creativecommons.org/licenses/by-nc-nd/3.0/deed.zh)
