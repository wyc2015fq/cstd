# App Store 狠抓精神文明建设，JSPatch要亡了？ - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年03月09日 09:24:12[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：519
![](http://upload-images.jianshu.io/upload_images/225849-2d3be8182d8b54c1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
本文来自简书，原文地址:[http://www.jianshu.com/p/39af67a58355](http://www.jianshu.com/p/39af67a58355)
> 
据悉，近日美国苹果公司响应党的号召，为了实现了社区文明和谐，各项事业全面发展，狠抓精神文明建设。将形成组织领导、定期引导、精准对标、适时检查、总结提高等五项机制，确保创建工作规范化、科学化、常态化。 ---来自老司机的前线报道
事情的主要起因在 App Store Review Guide Line 的 2.5.2 这条：
> 
2.5.2 Apps should be self-contained in their bundles, and may not read or write data outside the designated container area, nor may they download, install, or execute code, including other iOS, watchOS, macOS, or tvOS apps.
这条是在16年WWDC之后更新上去的。这条规则也很容易理解，所有被执行的代码都应该包含在App里，不能下载代码到本地执行。下发的无论是OC还是JS或者其他形式的代码，都可以被认为违反了这条规则。但是苹果一直没有严格“执法”，直到今天才开始给大批有类似嫌疑的开发者发了警告邮件，或者纷纷被拒。
当然这没有什么可怪的，这种执行力度全靠心情，毕竟爸爸从来不用讲道理。不时也会父爱如山。
![](http://upload-images.jianshu.io/upload_images/225849-672b60553458ae84.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/500)
父爱如山
根据看到的反馈，目前苹果判断的依据主要有两条。
一种是扫描特定的 API ，比如：
> 
dlopen(), dlsym(), respondsToSelector:, performSelector:, method_exchangeImplementations()
但是这里并不是完全禁止使用这些 API ，只是有个规则会检查这些 API 的参数是不是可能是外部引入的。
比如这么写是没问题的：
```
if([self.delegate respondsToSelector: @selector(myDelegateMethod)]) {
   [self.delegate performSelector: @selector(myDelegateMethod)];
}
```
这么写就可能被打回：
```
NSString *remotelyLoadedString = .... (download from your backend)
[self performSelector: NSSelectorFromString(remotelyLoadedString)];
```
另外会检查特定的类名，比如大家都知道的JSPatch和Rollout，发现APP里带了这样有潜在威胁的库就可能打回。但是这个方式似乎通过混淆就能过关。
![](http://upload-images.jianshu.io/upload_images/225849-984f48aa899b4f4f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/600)
不懂为什么提到JSPatch我就想发个表情，印象中过去某个时间段也有被这样提示被下架，后来又放松了。内心 OS ：“皇上！我是被冤枉的！。”
![](http://upload-images.jianshu.io/upload_images/225849-aa16cf8c747b029f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/400)
紫薇，快救我
# 对未来的判断
苹果是百分百不愿意代码绕过审核被下发的。无论是修bug，还是一个小功能，高冷的资本主义企业怎么能让你乱蹭，万一你进去了呢。其实我国国情特殊，真要是什么违法的东西上了苹果也遭不住，搞不好App Store都要下线。
道高一尺魔高一丈，程序执行起来总是要有漏洞。没错，是可以和官方博弈，但是你想一下下发代码这件事是不是真的影响到App运行非做不可？如果代价提高呢？比如被发现一次直接封掉你的Apple ID，觉得还是非做不可？
聪明的人已经在如何提APP稳定性的道路上努力了。忘了HotPatch这件事吧。
![](http://upload-images.jianshu.io/upload_images/225849-234a8f445316e071.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/500)
# RN&Weex 还能续一轮命
从各方的反馈来看，苹果这轮狠抓的是下发代码，而不是针对用 JS 代码编写业务逻辑。如果觉得这个有问题，就会把 JSCore 的一些核心 API 改为私有 API 了。只是在这个风口，基于这个技术的框架刚好很容易就提供了下发代码执行逻辑的能力，所以被推到灯光下。
我之前有次也问过Weex的负责人，他明确表示 Weex 不是为了解决热更新的问题存在的。只是恰巧有这个能力。
苹果在 iOS 7中引入新的 JavaScriptCore 框架，必然是认为在 APP 里执行 JS 对于整个 iOS 生态是有益的，否则何必多此一举。所以目前只是要解决掉引入这个框架后带来的滥用问题。 所以我认为这一轮的整治里，JS2Native 的框架肯定可以全身而退。
# Swift ？
有人说有没可能是苹果在为 Swift 下一步很大的棋。为了确认此事某好事网友咨询了 Swift 之父CL：
![](http://upload-images.jianshu.io/upload_images/225849-7d9c4530e109d597.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/800)
爸爸口风很紧表示：
![](http://upload-images.jianshu.io/upload_images/225849-36642178b724a689.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/500)
# 共建和谐社会
来，让我们一起表态：我志愿与美方一道建设和谐的 App Store 生态。
![](http://upload-images.jianshu.io/upload_images/225849-884e4128cfc553f2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/500)
