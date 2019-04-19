# AFNetworking（v3.1.0）框架学习总结(一框架结构) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月19日 15:54:00[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：960

#### 本文来自简书，原文地址:[http://www.jianshu.com/p/519611e875cd](http://www.jianshu.com/p/519611e875cd)
#### Write in the first【写在最前】
AFNetWorking 基本是iOS开发中使用网络通信框架的标配，这个框架本身比较庞大，也很复杂，但是使用起来非常非常简单。极大地提高了开发效率，让我们更加专注于业务逻辑的实现。
像这种经常使用用又比较重要的内容，我觉得要做到 知其然亦知其所以然，然后梳理总结常用方法，到最后夯实基础、活学活用。
为此，本着好好学习，了解权威的目的，决定从 AFNetworking 框架入手学习，按功能模块一点一点剖析它的各个模块，所以写了【AFN框架学习总结】+【AFN源码学习总结】系列。
本篇文章主要从【AFN 框架结构】学习总结，
时间有限，知识并未全覆盖，有遗漏或者错误，忘指正。
![](http://upload-images.jianshu.io/upload_images/2230763-52f44551d97d1941.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
 AFNetworking
> 
目录：
- AFN 需求 | 版本区别
- AFN 内部体系结构
- AFN 工程目录
- AFN 后续【框架 | 源码 学习总结】
- 期待 & About me
首先来看一下，官方介绍 [AFNetworking](https://github.com/AFNetworking/AFNetworking)，如下：
> 
AFNetworking is a delightful networking library for iOS and Mac OS X. It's built on top of the [Foundation
 URL Loading System](https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/URLLoadingSystem/URLLoadingSystem.html), extending the powerful high-level networking abstractions built into Cocoa. It has a modular architecture with well-designed, feature-rich APIs that are a joy to use.
Perhaps the most important feature of all, however, is the amazing community of developers who use and contribute to AFNetworking every day. AFNetworking powers some of the most popular and critically-acclaimed apps on the iPhone, iPad, and Mac.
Choose AFNetworking for your next project, or migrate over your existing projects—you'll be happy you did!
翻译过来简单点说就是
AFNetworking 是一个适用于 iOS 和 Mac OS X 两个平台的网络库，它是基于 [Foundation
 URL Loading System](https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/URLLoadingSystem/URLLoadingSystem.html) 上进行了一套封装，并且提供了丰富且优美的API接口给使用者使用。
#### Requirements【需求 | 版本区别】
![](http://upload-images.jianshu.io/upload_images/2230763-a0f1b08410453f35.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
AFN --> 需求 | 版本区别
- 
1.x 版本，内部底层是基于 NSURLConnection 的，是对 NSURLConnection 一次封装。
- 
在13年，苹果推出 NSURLSession 也就是会话管理者，后来 2.x AFN框架又对 NSURLSession 进行一次封装，其实在 2.0-2.6.3 AFN内部是使用两套系统，一套是基于 NSURLConnection 的，一套是基于 NSURLSession的。
- 版本升级到3.0之后，AFN 就不在支持 NSURLConnection 了，把有关 URLConnection 的代码已全部移除。
## Architecture【AFN 内部体系结构】
- 
`NSURLSession`【管理者】
- `AFURLSessionManager`
- `AFHTTPSessionManager`（封装了常用的
 HTTP 方法）
- `GET`
- `POST`
- `UIKit + AFNetworking` 分类
- `NSProgress`：利用KVO
- 
`Serialization`【序列化】
- `AFURLRequestSerialization`【请求序列化】
- `AFHTTPRequestSerializer`（请求的数据格式，默认二进制）
- `AFJSONRequestSerializer`
- `AFPropertyListRequestSerializer`
- `AFURLResponseSerialization`【响应者序列化】
- `AFHTTPResponseSerializer`（默认二进制响应数据，解析方案）
- `AFJSONResponseSerializer`（返回JSON类型，JSON解析方案.默认）
- `AFXMLParserResponseSerializer`（返回XML类型，XML解析方案）
- 
`Additional Functionality`【额外的功能】
- `AFSecurityPolicy`【安全策略相关】
- `HTTPS`（HTTP+SSL加密协议）
- `AFNetworkReachabilityManager`【网络状态监听管理者】
- 
`UIKit`【类库扩展与工具类】
- `UIKit+AFNetworking`
- 
~~`NSURLConnection（iOS9.0废弃）`~~
- `AFURLConnectionOperation`
- `AFHTTPRequestOperation`
- `AFHTTPRequestOperationManager`（封装了常用的
 HTTP 方法）
- 属性
- `baseURL`：AFN建议开发者针对 `AFHTTPRequestOperationManager` 自定义个一个单例子类，设置 `baseURL`,
 所有的网络访问，都只使用相对路径即可
- `requestSerializer`：请求数据格式/默认是二进制的
 HTTP
- `responseSerializer`：响应的数据格式/默认是
 JSON 格式
- `operationQueue`：操作队列
- `reachabilityManager`：网络状态检测管理者
- 方法
- `manager` :方便创建管理器的类方法
- `HTTPRequestOperationWithRequest` :在访问服务器时，如果
 要告诉服务器一些附加信息，都需要在 `Request` 中设置
- `GET`
- `POST`
#### Directory【AFN 工程目录】
目前版本是 3.1.0，我通过 CocoaPods 导入的 AFNetworking，导入后目录如下
![](http://upload-images.jianshu.io/upload_images/2230763-273639fd84d28ab3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
CocoaPods-->AFN 3.1.0工程目录
使用 CocoaPods 导入后可以看到目录很清晰主要是在五个文件夹下， 除去 Support Files，可以看到AF分为如下5个功能模块：
- 
`NSURLSession`（网络通信模块）
- 
`ReachAbility`（网络状态监听模块）
- 
`Security`（网络通信安全策略模块）
- 
`Serialization`（网络通信信息序列化/反序列化模块）
- 
`UIKit`（UIKit库的扩展）
其核心当然是网络通信模块，其余的四个模块，均是为了配合网络通信或对已有 UIKit 的一个扩展及工具包。
这五个模块所对应的类的结构关系图如下所示：
![](http://upload-images.jianshu.io/upload_images/2230763-ce4a7d0f7d8106e2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
AFN 功能模块-->关系
可以看到，AFN 的核心是 `AFURLSessionManager` 类，`AFHTTPSessionManager` 继承于 `AFURLSessionManager`，
 针对HTTP协议传输做了特化。而 `AFURLResponseSerialization`，`AFSecurityPolicy`，`AFNetworkReachabilityManager`则被`AFURLSessionManager`所用。
其次，还可以看到一个单独的UIKit 包提供了对 iOS UIKit 类库的扩展与工具类。
建议：
可以学习下AFN对 UIKit 做了一些分类，对自己能力提升是非常有帮助的。
当下载好，手动导入的时候，显示两个文件夹，如下
![](http://upload-images.jianshu.io/upload_images/2230763-6c4dd19dff5b6d6f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
手动导入-->AFN 3.1.0工程目录
很明显第一个文件夹里边是跟网络请求相关的，第二个是跟UI相关的。
#### Subsequent【后续】
这个框架有太多的东西值得我们去学习推敲，方方面面都透露着作者以及众多贡献者的智慧，我们读起来也一定会受益良多。
那么，我们的【AFN 框架 | 源码 学习总结】之路，
框架：
- 
[AFN（v3.0+）框架学习总结(一框架结构)](https://custompbwaters.github.io/2017/02/16/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.1.0%EF%BC%89%E6%A1%86%E6%9E%B6%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%B8%80%E6%A1%86%E6%9E%B6%E7%BB%93%E6%9E%84)/)
- 
[AFN（v3.0+）框架学习总结(二内部逻辑处理过程)](https://custompbwaters.github.io/2017/02/17/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.1.0%EF%BC%89%E6%A1%86%E6%9E%B6%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%BA%8C%E5%86%85%E9%83%A8%E9%80%BB%E8%BE%91%E5%A4%84%E7%90%86%E8%BF%87%E7%A8%8B)/)
- 
[AFN（v3.0+）框架学习总结(三框架基本使用)](https://custompbwaters.github.io/2017/02/18/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.0+%EF%BC%89%E6%A1%86%E6%9E%B6%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%B8%89%E6%A1%86%E6%9E%B6%E5%9F%BA%E6%9C%AC%E4%BD%BF%E7%94%A8)/)
- 
[AFN（v3.0+）框架学习总结(四二次封装)](https://custompbwaters.github.io/2017/02/19/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.0+%EF%BC%89%E6%A1%86%E6%9E%B6%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E5%9B%9B%E6%A1%86%E6%9E%B6%E4%BA%8C%E6%AC%A1%E5%B0%81%E8%A3%85)/)
源码：将按照 AFN 5大功能模块，依次为
- 
[网络通信核心
 AFURLSessionManager 源码学习总结](https://custompbwaters.github.io/2017/02/25/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.0+%EF%BC%89%E6%BA%90%E7%A0%81%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%B8%80)%E2%80%94AFURLSessionManager/)
- 
[HTTP网络通信核心
 AFHTTPSessionManager 源码学习总结](https://custompbwaters.github.io/2017/02/25/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.0+%EF%BC%89%E6%BA%90%E7%A0%81%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%BA%8C)%E2%80%94AFHTTPSessionManager/)
- 
[网络通信信息序列化/反序列化模块
 AFURL[Request / Response]Serialization源码学习总结](https://custompbwaters.github.io/2017/02/27/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.0+%EF%BC%89%E6%BA%90%E7%A0%81%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%B8%89)%E2%80%94%20Serialization%20%E5%BA%8F%E5%88%97%E5%8C%96/)
- 
[网络状态监听模块
 AFNetworkReachabilityManager 源码学习总结](https://custompbwaters.github.io/2017/02/27/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.0+%EF%BC%89%E6%BA%90%E7%A0%81%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E5%9B%9B)%E2%80%94%20ReachAbility%20%E7%BD%91%E7%BB%9C%E7%8A%B6%E6%80%81%E7%9B%91%E5%90%AC/)
- 
[网络安全策略
 AFSecurityPolicy 源码学习总结](https://custompbwaters.github.io/2017/03/01/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.0+%EF%BC%89%E6%BA%90%E7%A0%81%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%BA%94)%E2%80%94%20Security%20%E7%BD%91%E7%BB%9C%E9%80%9A%E4%BF%A1%E5%AE%89%E5%85%A8%E7%AD%96%E7%95%A5/)
- 
[AFN
 提供的工具包 UIKit 的功能类学习总结](https://custompbwaters.github.io/2017/03/01/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.0+%EF%BC%89%E6%BA%90%E7%A0%81%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E5%85%AD)%E2%80%94%20UIKit+AFN%E5%B7%A5%E5%85%B7%E7%B1%BB/)
#### 补充
1、可参考 AFNetworking 提供的 Example 学习
- [AFNetworking 源码阅读系列【Example】](http://www.cnblogs.com/polobymulberry/category/785705.html)
![](http://upload-images.jianshu.io/upload_images/2230763-f211fa6ac64276c9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
 AFN-->【Example】
2、「HTTPS 相关」
- 
[AFNetworking之于https认证](http://www.jianshu.com/p/a84237b07611)
- 
[HTTPS为什么安全 &分析 HTTPS 连接建立全过程](http://www.jianshu.com/p/0d8575b132a8)
- 
[知识 | HTTPS到底是啥？](http://www.jianshu.com/p/6d95a8665935)
#### 期待
- 
如果在阅读过程中遇到 error，希望你能 Issues 我，谢谢。
- 
如果你想为【本文相关】分享点什么，也希望你能 Issues 我，我非常想为这篇文章增加更多实用的内容，谢谢。
- 
[「博客原文」](https://custompbwaters.github.io/2017/02/16/%E6%BA%90%E7%A0%81%E6%B3%A8%E8%A7%A3+%E6%B4%BB%E7%94%A8/AFN%EF%BC%88v3.1.0%EF%BC%89%E6%A1%86%E6%9E%B6%E5%AD%A6%E4%B9%A0%E6%80%BB%E7%BB%93(%E4%B8%80%E6%A1%86%E6%9E%B6%E7%BB%93%E6%9E%84)/)，对本文我会【不定时、持续更新、一些
 学习心得与文章、实用才是硬道理】^_^.
