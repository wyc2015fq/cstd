# iOS下的实际网络连接状态检测 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年02月29日 09:29:21[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：339标签：[ios开发																[iOS																[iOS网络连接状态检测																[iOS网络连接](https://so.csdn.net/so/search/s.do?q=iOS网络连接&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS网络连接状态检测&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)](https://so.csdn.net/so/search/s.do?q=ios开发&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
序言
网络连接状态检测对于我们的iOS app开发来说是一个非常通用的需求。为了更好的用户体验，我们会在无网络时展现本地或者缓存的内容，并对用户进行合适的提示。对绝大部分iOS开发者来说，从苹果示例代码改变而来的各种Reachablity框架是实现这个需求的普遍选择,比如这个库。但事实上，基于此方案的所有实现，都无法帮助我们检测真正的网络连接状态，它们能检测的只是本地连接状态；这种情况包括但不限于如下场景：
1.现在很流行的公用wifi，需要网页鉴权，鉴权之前无法上网，但本地连接已经建立；
2.存在了本地网络连接，但信号很差，实际无法连接到服务器；
3.iOS连接的路由设备本身没有连接外网。
CocoaChina上已有很多网友对此进行提问和吐槽，比如：
- 
[如何判断设备是否真正连上互联网？而不是只有网络连接](http://www.cocoachina.com/bbs/read.php?tid=283498)
- 
[[Reachability reachabilityWithHostName:]完全没用！](http://www.cocoachina.com/bbs/read.php?tid=40989)
[苹果的Reachability示例](https://developer.apple.com/library/ios/samplecode/Reachability/Introduction/Intro.html)中有如下说明，告诉我们其能力受限于此：
"Reachability cannot tell your application if you can connect to a particular host, only that an interface is available that might allow a connection, and whether that interface is the WWAN."
而苹果的[SCNetworkReachability
 API](https://developer.apple.com/library/mac/documentation/SystemConfiguration/Reference/SCNetworkReachabilityRef/#//apple_ref/doc/uid/TP40007260-CH101-SW4)则告诉了我们更多： "Reachability does not guarantee that the data packet will actually be received by the host. "
Reachability相关的框架在底层都是通过SCNetworkReachability来实现网络检测的，所以无法检测实际网络连接情况。
有鉴于此，笔者希望打造一个通用、简单、可靠的实际网络连接状态检测框架，于是[RealReachability](https://github.com/dustturtle/RealReachability)诞生了。
**RealReachability简单介绍**
RealReachability是笔者1个月之前发布到github的开源库，项目地址如下： https://github.com/dustturtle/RealReachability。 短短1个月时间收获了100多个star，其中主要来自于外国友人，最近还上了github的[OC板块趋势排行榜](https://github.com/trending/objective-c)，着实令笔者相当惊喜。
 此框架开发的初衷来源于项目实际需求，离线模式对网络连接状态的要求比较苛刻，且实际场景经常会遇到“伪连接”的情况，Reachability面对此场景力不从心。多方研究后引入了ping能力（此方案流量开销最小，也最简单），实现了简单的实际网络连接监测；后面经过提炼和优化，就有了这个框架。可以告诉大家的是，这个框架在appstore上架应用中已经经受了考验，且经过了长时间的测试，可以放心使用。
**集成和使用介绍**
- 
集成
最简便的集成方法当属pod: pod 'RealReachability'。
手动集成：将RealReachability文件夹加入到工程即可。
依赖：Xcode5.0+，支持ARC, iOS6+.项目需要引入SystemConfiguration.framework. 
- 
使用介绍
其接口的设计和调用方法和Reachability非常相似，大家可以无缝上手，非常方便。 开启网络监听：
```
`   ``[GLobalRealReachability startNotifier];`
`    ``[[NSNotificationCenter defaultCenter] addObserver:self`
`                                             ``selector:@selector(networkChanged:)`
`                                                 ``name:kRealReachabilityChangedNotification`
`                                               ``object:nil];`
```
回调代码示例：
```
`- (void)networkChanged:(NSNotification *)notification`
`{`
`    ``RealReachability *reachability = (RealReachability *)notification.object;`
`    ``ReachabilityStatus status = [reachability currentReachabilityStatus];`
`    ``NSLog(@``"currentStatus:%@"``,@(status));`
`}`
```
触发实时网络状态查询代码示例：
```
`[GLobalRealReachability reachabilityWithBlock:^(ReachabilityStatus status) {`
`        ``switch``(status)`
`        ``{`
`            ``case``NotReachable:`
`            ``{`
`            ``//  case NotReachable handler`
`                ``break``;`
`            ``}`
`            `
`            ``case``ReachableViaWiFi:`
`            ``{`
`            ``//  case ReachableViaWiFi handler`
`                ``break``;`
`            ``}`
`            `
`            ``case``ReachableViaWWAN:`
`            ``{`
`            ``//  case ReachableViaWWAN handler`
`                ``break``;`
`            ``}`
`            ``default``:`
`                ``break``;`
`        ``}`
`    ``}];`
```
查询当前实际网络连接状态：
```
`ReachabilityStatus status = [reachability currentReachabilityStatus];`
```
**Demo:**
我们在github的repository中已经包含了简单的Demo工程，直接下载运行即可。相关的Api调用也可以参考demo中的实现。 demo截图： 
![45.png](http://cc.cocimg.com/api/uploads/20160223/1456215404428152.png)
**RealReachability的实现原理**
RealReachability架构图：
![111.png](http://cc.cocimg.com/api/uploads/20160223/1456212358863328.png)
realReachability架构概要图RealReachability主要包含3大模块：connection、ping、FSM； 其中Ping模块通过对同样是苹果提供的ping样例代码进行了封装，connection模块实现了基于SCNetworkReachability的本地状态检测，FSM模块是有限状态机。通过FSM的状态管理控制connection模块和Ping模块协同工作，并通过可配置的定时策略等业务逻辑优化，最终得到了我们的实现。
PS:其中connection模块和ping模块也可独立使用，分别提供本地网络检测和ping的能力，感兴趣的读者也可以尝试（调用方式请参考RealReachability开源代码）。
**结束语**
希望这个框架能够帮助到大家的iOS开发！ 遇到任何疑问或者使用上的问题，[都可以联系我](http://163.com/)。
对此框架我也会持续进行维护和优化，更希望感兴趣的朋友可以到github上pull request! 开源有你更精彩！[https://github.com/dustturtle/RealReachability](https://github.com/dustturtle/RealReachability) 觉得有用就点个star喔！
