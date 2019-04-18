# 集成 jpush-react-native 常见问题汇总 （iOS 篇） - weixin_33985507的博客 - CSDN博客
2017年04月17日 08:57:36[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
给 iOS 应用添加推送功能是一件比较麻烦的事情，本篇文章收集了集成 jpush-react-native 的常见问题，目的是为了帮助用户更好的排查问题
**1.收不到推送**
- 
确保是在真机上测试，而不是在模拟器
- 
自己的应用已经在 Apple developer 给应用配置推送功能，创建推送证书 （并且保证
bundle id 与 Apple developer 上的是一致的）如果之前没有接触过推送证书建议看视频来 ? [官方集成视频](https://community.jiguang.cn/t/jpush-ios-sdk/4247)
- 
能够获取deviceToken 但是收不到推送， 如果是使用 xcode 8，检查 (Project -> Target ->
Capabilities ) Push Notification 选项是否已经点开，如果没有需要点开
**2.收到一条通知 JS 层会有多个 ReceiveNotification 事件**
- 
升级 jpush-react-native 到最新版本
- 
有没有在合适的地方 remove ReceiveNotification
  事件，如果多次监听该事件会导致重复接收
**3.如何消除 应用 icon 的角标（badge）**
调用 JPushModule.setBadge 方法 如果设为 0，则表示情况角标
```
JPushModule.setBadge(5, (badgeNumber) => {
  console.log(badgeNumber)
});
```
**4.开发环境能够收到推送，生产环境收不到推送**
- 
如果是通过控制台发送的推送，检查环境选择是否正确
![clipboard.png](https://image-static.segmentfault.com/232/225/2322251946-58f42cd4c61e1_articlex)
- 
如果是调用 REST api 发送通知，请检查推送参数
- 
手机应用是否是仍然是 开发环境， 如果不是请打包 Ad Hoc 来测试
**5.自定义消息事件无法触发**
- 
如果通过控制台发送的自定义消息，确保选择的是自定义消息，而不是通知
![clipboard.png](https://image-static.segmentfault.com/334/862/3348621260-58f42d210ae8b_articlex)
- 
检查是否监听了 networkDidReceiveMessage 事件
**6.点击推送唤起应用的时候 OpenNotification 不调用**
- 
升级 jpush-react-native 到最新版本（旧版本还没做缓存处理，新版本已修复）
- 
检查监听 OpenNotification
  的操作是否异步调用，有用户应为在异步回调中才执行监听操作，导致事件已经上抛到 js 层但 js 层还没执行监听操作的问题
**7.通知在前台展示问题**
- 
iOS 10 开始允许通知在前台展示这个地方在 Native 代码中控制
```
// iOS 10 Support
- (void)jpushNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(NSInteger))completionHandler {
  // Required
  NSDictionary * userInfo = notification.request.content.userInfo;
  if([notification.request.trigger isKindOfClass:[UNPushNotificationTrigger class]]) {
    [JPUSHService handleRemoteNotification:userInfo];
    [[NSNotificationCenter defaultCenter] postNotificationName:kJPFDidReceiveRemoteNotification object:userInfo];
  }
completionHandler(UNNotificationPresentationOptionAlert);  // 需要执行这个方法，选择是否提醒用户，有Badge、Sound、Alert三种类型可以选择设置
}
```
- 
iOS 10 之前是系统控制，应用在前台推送不展示
**8.找不到头文件 RCTJPushModule.h**
- 
在 iOS 工程中如果找不到头文件可能要在 TARGETS-> BUILD SETTINGS -> Search Paths ->
  Header Search Paths 添加如下如路径
`$(SRCROOT)/../node_modules/jpush-react-native/ios/RCTJPushModule/RCTJPushModule`
RN 在 0.40.0 之后 不会自动加入这个路径了，要手动加一下
**9. 在 iphone 5 的模拟器编译不通过**
JPush SDK 在 3.0.0 及以后版本不再支持 i386 的模拟器了血药在 iphone 5s 以上版本中测试
**10.找不到 jcore-react-native**
在 jpush-react-native 1.4.4 以后版本 需要通知安装 jcore-react-native 才能正常使用
