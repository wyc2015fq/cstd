# 在app内拿到用户安装app前信息的解决方案（支持iOS11） - weixin_33985507的博客 - CSDN博客
2017年08月28日 16:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
> 
如何让用户更顺滑的完成“由外而内”的“激活过程”
iOS11前后的Cookie共享和深度链接：SFSafariViewController/SFAuthenticationSession + Associated Domains
2017/10/23更新：
随着逐渐在项目里把这一部分东西具体化，发现之前写的内容不够系统化，这次把相关的内容都体系化一下，算是一套方案吧。
大致流程如下图：
![2161270-d60e76caba549a7e.jpeg](https://upload-images.jianshu.io/upload_images/2161270-d60e76caba549a7e.jpeg)
流程图
常规的从外进入App内部，有很成熟且简单的方案：私有协议，但使用私有协议的前提是你当前的设备上已经安装了你自己的App。
本文要讨论就是，**将用户的激活/信息收集过程变成一个单向的线性流程**，当前设备是否安装了App不再是障碍，也不需要用户手动返回进行重复操作。
- 
通过Associated Domains(也就是首版内容中的Universal Links)来判断当前设备是否安装了App，也就是，有App时就直接跳转到App内部了，如果没有的话，使用Associated Domains的web页面加载完成后，立即跳转到新的页面，提示用户在Safari中打开（因为在微信中存储的Cookie无法与我们自身的App共享）；
- 
用户下载好App后，打开App，我们以是否为第一次打开App为判断标准，如果是的话，就与Safari共享Cookie，检测Cookie数据，这样就获取到了**app存在前产生的信息**。
如何获取到呢？这里有版本的区别：
**iOS11以后的版本**：使用SFAuthenticationSession这个类，这也是更新前这篇文章所主要阐述的；
**iOS11之前的版本（iOS9+）**：使用SFSafariViewController这个类，这个类的使用方法网上有很多前辈写过，我也不做赘述了。
这里格外强调一下他们之间的差异：从第4步和第7步也可以看出来，SFAuthenticationSession的
```
- (instancetype)initWithURL:(NSURL *)URL callbackURLScheme:(nullable NSString *)callbackURLScheme completionHandler:(SFAuthenticationCompletionHandler)completionHandler;
```
方法有个回调的block，其中的参数callbackURL实际上就是当前打开的web页面调用私有协议的URL，这里就可以让web带上Cookie中的值通过私有协议传递给我们；
而SFSafariViewController同样是打开指定的web页面，web同样发起私有协议，但是此时的私有协议会即时“生效”，也就是直接调用了AppDelegate中的
```
-(BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options；
```
或
```
-(BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation；
```
方法，我们就可以从这两个方法中拿到所需的参数，也就是和从外部调用私有协议的处理过程是一致的，也就是从app内调用app自己（这里我最开始绕了一段时间才整明白🤦🏻‍♂️）。
对于SFAuthenticationSession的处理还有一个需要注意的点：SFSafariViewController在加载完成后，有代理方法可以把它dismiss掉，但是SFAuthenticationSession只有在产生回调时才会自动dismiss，也就是说，如果用户在系统Safari中没有所需的Cookie并且还同意打开SFAuthenticationSession中的页面的话，那么用户就必须在手动把页面在关上，而且这一系列过程都不会对用户带来任何有益的结果，那酱紫对用户就很不友好了~因此，建议让web同学在没有Cookie的时候，也调用私有协议，然后带上指定的标识，那么我们就可以在接收到指定标识时继续常规的流程，而无需麻烦用户。
（对了，我之前提问的关于“SFSafariViewController如何隐性展示”的问题，我最后使用的解决方案是直接present出来，然后加载完成后dismiss掉，因为我们的验证页面几乎没有需要加载的元素，只是逻辑判断的代码，加载速度会很快，几乎是一闪而过的。但是相信还是有童鞋用更隐性的方式完成了的，还请赐教！）
- 这里之所以采用Associated Domains，是出于以下两点优势：
- web同学没法准确判断当前设备是否安装了app，如果强行调用私有协议的话，在未安装的情况下，Safari上会出现无法执行的提示，用户体验很不友好；
- 突破微信的限制：现今“由外而内”的方式一般都是以二维码为起点，而且大多数用户都在使用微信扫码，但是微信会对自己不支持的私有协议进行拦截，而Associated Domains则可以完全突破他的限制，直接有效的进入app；
除了更新前说的一些内容，这里我想格外强调一下的就是**跨域问题**。这个是指：如果你希望app在从A页面跳转到B页面的URL时被唤起，那么，你就需要让A、B两个页面**不在同一个域**，例如，a.xtshow.com和b.xtshow.com就是跨域的两个URL（这一点在一些教程中没有太明确，多亏一篇文章中一个同学的回复才帮我绕出去，鸣谢！）。
除了指定技术点上的注意之外，还希望大家能够注意一下ViewController层级结构的问题，以免造成相互覆盖，或者无法present、push的情况，具体情况还要看各位的工程特点。
分割线（首版）
- 
# 前言
项目中新增了一个扫描二维码 **激活账号+指定商品** 的功能，加完之后发现不能过审。。。才发现发现原来二维码功能并不是能随便加的（[官方文档](https://developer.apple.com/app-store/review/guidelines/cn/#content-codes)）。。。但是因为推广渠道特殊，二维码的功能一定要有，因此考虑将App内的全部二维码扫描功能移除，换成从外部扫描跳转App的方式。
又进一步考虑用户体验的问题，希望能够让用户只扫描一次二维码就线性完成从下载到打开激活全过程，然后就有了这片文章~
- 
# 开始
常规的custom scheme浏览器跳转什么的就不在此处赘述了。此处对我来说的新鲜点应该是**Deep Link**相关实现。
**Deep Link**：顾名思义，就是深度链接。链接很好理解，就是把一些东西串起来，那何为深度呢？这就要针对iOS系统的特性来说了，iOS下App绝大多数情况下都是从App Store上下载下来的。刚下载的app是完全“初始化”的，而且出于Apple对用户隐私的保护，App是无法得知当前用户是谁。但我们又希望通过某些方式能够在第一次用户开启App时，不需要用户手动的告诉我们他是谁（此处的“谁”不是客观的是谁，而是对于我们来说“用户是谁”）。
#### SFSafariViewController
通过一番Google~发现了SFSafariViewController这么个好玩意儿！SFSafariViewController的核心好处就是：**共享系统Safari的Cookie**。
也就是说，通过SFSafariViewController可以拿到你在Safari浏览器中打开的某个网页的Cookie。Cookie中就可以存储一些你需要的信息：例如激活码，用户的登录信息等。而这个web页面一般就是在三方app，例如微信中扫码或者点击分享等操作打开的，再在Safari中打开，而Cookie则是web小伙伴根据二维码中的信息进行处理后写入web页面中的，因此，这个是需要web小伙伴的配合的哦。
这样，在第一次打开App时，检测一下Safari中指定页面的Cookie，就可以拿到我们所需的信息。对应到我现在的项目，也就是可以知道用户在安装App之前，扫得二维码的信息，也就不要用再让用户退出app，回到微信扫第二次码了~做到了“**一码两用**”-既能下载又能传值！
具体的实践方案，我在这就不再赘述了，直接在Google中查SFSafariViewController就会有一大票前辈们的文章可供参考。
- 
# 重点
弄好之后，无意之间在stackoverflow上发现一个哥们说：SFSafariViewController在iOS11上要被封了（此处的被封不是DEPRECATED，而是Cookie共享的机制没有了）！虽然只有一个赞，但也宁可信其有不可啊！万一废了半天劲弄的东西最后半个来月之后就不能用了那岂不是闹心死了所以又去官网确认了下。。。果然。。。
![2161270-5f69605e2ecd4a41.png](https://upload-images.jianshu.io/upload_images/2161270-5f69605e2ecd4a41.png)
WX20170828-142216.png
[官方原文地址](https://developer.apple.com/documentation/safariservices/sfsafariviewcontroller?language=objc)
但是还好，柳暗花明又一村，下面出来个instead的~ 就说吗~ 不应该封了一条路之后，不开条新路出来嘛~
#### SFAuthenticationSession
这个是iOS11新加入的API，因此需要**Xcode9 + iOS11**的环境。
需要引入SafariServices框架：
```
#import <SafariServices/SafariServices.h>
```
这个玩意儿的使用方式比SFSafariViewController还要简单，只有一个可用的初始化方法：
```
- (instancetype)initWithURL:(NSURL *)URL callbackURLScheme:(nullable NSString *)callbackURLScheme completionHandler:(SFAuthenticationCompletionHandler)completionHandler;
```
（*很奇怪，其实这个初始化方法前面还有个初始化方法：*
```
- (instancetype)init NS_UNAVAILABLE;
```
但是直接就给不可用了。~~可能由于现在还是beta版吧，估计正式版之后会直接删掉吧。~~
）
这个方法有三个参数：URL、callbackURLScheme、completionHandler。
官方库中的解释很详细，但我还是啰嗦两句吧：
```
URL：需要同步Cookie的网站的URL；
callbackURLScheme：App期望在回调URL中出现的自定义协议；
completionHandler：处理完成后的回调。
```
**callbackURLScheme**，有两种设置方法，除了在此处直接设置外，还可以在Targets->Info->URL Types中设置。
**completionHandler**，在请求用户权限时，如果点击了取消，也会调用此回调，error code = 1；而且在出现任何回调时，无论是出现error还是callbackURL，都会自动dismiss刚弹出的web页面。
示例代码1：
```
SFAuthenticationSession *auth = [[SFAuthenticationSession alloc] initWithURL:[NSURL URLWithString:@"https://www.baidu.com"] callbackURLScheme:@"xtshow" completionHandler:^(NSURL * _Nullable callbackURL, NSError * _Nullable error) {
        //用户点击取消时，会出现error：SFAuthenticationErrorCanceledLogin
        NSLog(@"%@---%@",callbackURL,error);
}];
```
实例化一个SFAuthenticationSession后，**一定要使用strong属性全局强持有这个auth**,不然的话会出现申请权限alert一闪而过的情况。~~`很好奇为什么oc中会出现这类不strong强引用就会马上消失掉的对象？是因为单例吗？单例如果不强引用就会拿上release掉以防止影响其他对象持有吗？有木有大神给解个惑~谢谢了~~！`~~如果不强引用的话，离开作用域（一般就是当前代码块）时，对象就会被释放掉。
示例代码2：
```
self.auth = auth;
```
最后启用auth就可以了。
示例代码3：
```
[self.auth start]
```
启用后就会present一个很类似于SFSafariViewController的网页。
SFAuthenticationSession类中还有最后一个方法：
```
- (void)cancel;
```
用来手动dismiss web页面。
还有一个好消息就是，由于对web的逻辑是一致的，所以web小伙伴的代码都不需要改变，用一套就可以了！
用户可见的部分，最大的区别就是，在调用SFAuthenticationSession的时候，会出现一个系统的alert,请求用户的权限允许
![2161270-0feebbe6155d3dde.PNG](https://upload-images.jianshu.io/upload_images/2161270-0feebbe6155d3dde.PNG)
调用SFAuthenticationSession后提示用户的alert
嗯。。。。。web页面的逻辑我还是啰嗦两句吧：
web判断是否有Cookie：
没有的话，应该是出现在Safari中的情况，web会把url中的参数写入到Cookie中；
有的话，则应该是出现在App内的情况，web页面带着需要传递参数请求私有协议，例如：
```
xtshow://name=cool&password=bugaosuni
```
那么此时我们的app就能拿到所需的参数。
#### Universal Links
之前在闲逛的时候，见过这个东东，这是一种可以突破微信限制，直接从微信跳转到我们自己的App中的技术，这次也用上了。具体操作可以Google前辈们的文章~（这里有个小坑，很多文章中会提到一个Apple官方的[验证网站](https://search.developer.apple.com/appsearch-validation-tool/),验证是否实现了Universal Links，但是我在这个网站上一直验证不通过，实际上各种特性，记事本长按啊、web页面下拉app内打开什么的特性都是有的~所以我个人怀疑这个网站的验证不是绝对正确的，还是建议记事本长按和web页面下拉app内打开等方式验证。）
总而言之，custom scheme、SFSafariViewController、SFAuthenticationSession、Universal Links这些技术，在合适的时机搭配使用，可以很好的提升用户在使用app的顺畅感。
还有一个问题，想请教一下使用过SFSafariViewController并已经过审的大神们~想要让用户无感知的实现SFSafariViewController，看到有的大神是设置size为(0.5,0.5)，还有大神是设置alpha=0.05，但是由于Guideline 5.1.1(iv)里写得不是那么清楚，到底怎么做能够平稳过审呢？谢谢！
###### 这是本人的第一篇技术博客，Markdown也是边用边查的[手动捂脸]。。。之所以写这篇文章是因为之前找SFAuthenticationSession方面的资料的时候，发现全网好像都是英文的，中文版的也只有一篇湾湾同行的繁体中文版（可能是太简单了各位大神觉得不需要写一篇文章吧），因此就想自己写一篇~ 第一次写，语言组织啊、排版啊什么的都很粗糙，希望各位大神能够多多指教~ 不喜勿喷哈~
