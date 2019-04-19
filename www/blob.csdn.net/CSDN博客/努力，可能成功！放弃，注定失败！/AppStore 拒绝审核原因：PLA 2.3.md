# AppStore 拒绝审核原因：PLA 2.3 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年10月11日 09:14:31[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：84
### 1、前言
最近更新了一下之前开源的一些iOS App，适配了一下iPhone X / Xs / XR / Xs Max，然后收到这样的拒审信：
> 
2018年9月24日 上午1:08
发件人 Apple
- 10.0 Before You Submit: Program License Agreement
	PLA 2.3
Your app or its metadata contains references to a pre-release version of Apple software or hardware. Apps with compatibility references to a pre-GM version of iOS SDK or pre-released Apple hardware are not in compliance with the Apple Developer Program License Agreement.
Specifically, section 2.3 states:
"Apple may provide You with pre-release versions of the Apple Software or related services that constitute Apple Confidential Information and are subject to the confidentiality obligations of this Agreement."
Next Steps
Please remove all references to pre-release versions of Apple software and hardware from your app and its metadata.
> 
Since your App Store Connect status is Metadata Rejected, we do NOT require a new binary. To revise the metadata, visit App Store Connect to select your app and revise the desired metadata values. Once you’ve completed all changes, reply to this message in Resolution Center and we will continue the review.
### 2、解决
刚开始以为自己使用了 Xcode GM 版本提交应用，但反复检查就是正式的Xcode Version 10.0 (10A255)!
最后，还是通过网络搜索找到答案：
> 
此版本的新增内容:
- v5.0.0 适配iPhoneX、XR、Xmax ；
- 修复新闻列表闪退问题；
- 优化一些功能细节体验
引用网友的话：
> 
你的发布介绍里提到了苹果没开卖的iPadPro，删掉重新审核就行，不用重新上传包。回复大概这意思
然后搜索iPhone Xs上市时间?
> 
将在9月14日周五开启预售，9月21日正式上市
没毛病！！！今天已经2018年9月24号啦！
最后，还是发现，字母拼写错啦！！！
> - v5.0.0 适配 iPhone X、XR、XS Max；
- 修复新闻列表闪退问题；
- 优化一些功能细节体验
- 最后发现，还是被坑了一波：
> 
iPhone X🅁 即将到来iPhone XR 即将到来
北京时间 10 月 19 日下午 3:01 起开始预购。
苹果拒审信：
> 
Next Steps
Please remove all references to pre-release versions of Apple hardware from your app’s metadata such as iPhone XR.
最后苹果友善回复：
> 
2018年9月24日 下午8:49
发件人 Apple
0 . 10.0 Before You Submit: Program License Agreement
Hello,
Thank you for making these changes. We will continue the review, and we will notify you if there are any further issues.
Best regards,
App Store Review
### 3、总结
还是第一次遇到这样的情况，苹果不允许发布无上线的硬件或软件产品，也就是说，不能写错名字！！！确实，这样的要求，完全合理！！！
经过这事，苹果的审核确实还是存在很多问题，经过这几年，审核速度可以给予肯定，从原来一个星期，变快到1~2天，但是“审核质量” 却一直没有得到提升，导致开发者和审核员一直来回被拒来拒绝，相互怀疑和猜测，导致效率并没有真正的提高。希望，后续苹果能提供更优质的审核机制和流程标准。像谷歌play那样的机器审核也未必不行，后续在抽查，发现违反的，直接下架。这样对于开发者，都没有什么要向老板交待，直接了当！
### 参考
- [最近被拒，不知道什么原因，内容是 PLA 2.3 - Swift中国](http://swiftcn.io/topics/48?reply=185)
- [购买 iPhone XR - Apple (中国)](https://www.apple.com/cn/shop/buy-iphone/iphone-xr)
- 如有疑问，欢迎在评论区一起讨论！
- 如有不正确的地方，欢迎指导！
> 
注：本文首发于 [iHTCboy's blog](https://iHTCboy.com)，如若转载，请注来源
作者：iHTCboy
链接：https://www.jianshu.com/p/bf6ece6bb0df
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
