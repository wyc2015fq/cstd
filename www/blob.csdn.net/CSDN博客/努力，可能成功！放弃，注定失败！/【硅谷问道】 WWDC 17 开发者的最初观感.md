# 【硅谷问道】 WWDC 17: 开发者的最初观感 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月07日 10:18:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：230
![](http://upload-images.jianshu.io/upload_images/1721232-845ea8502ecec70e.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 前言
本文来自简书，原文地址:[http://www.jianshu.com/p/01f7ca48ecef](http://www.jianshu.com/p/01f7ca48ecef)
每年的 WWDC 都是 iOS 开发者集体高潮的时刻。第一天的 WWDC 带来了全新的 iOS 11、MacOS、tvOS 和 watchOS，革命性的 AR 和 ML 应用，以及更强的 iPad、“硬件神兽” iMac Pro 和智能音箱 HomePod。我想从开发者的角度，聊聊这次大会给我的印象和感受。欢迎大家在评论中谈谈自己的看法。
# 机器学习
恐怕没有哪个词的频率高过 Machine Learning，它也是贯穿各个平台的关键词汇：watchOS 的表盘，macOS 上照片流的分类，Siri 的上下文理解、翻译、预测，手写笔记的直接识别和搜索支持，甚至 Mac 系列芯片和 Metal 的更新都表示支持机器学习。iPhone 上人脸识别甚至比 Google Pixel 快出6倍。可以看出机器学习已经完完全全得融入到了 Apple 的全平台、全应用、全硬件。
而作为开发者，最令我激动的是 MLKit 的发布。苹果带来了自己的模型，你只要不停的运用数据喂养它即可。这意味着我们今后的 App 将变得更加动态化 -- 随着用户使用的增多，App 将越来越智能和强大。
原来的路线是 App 发布 -> 用户反馈 -> 线下产品经理分析 -> 程序员优化 -> App 更新；今后的路线可能是 App 发布 -> 用户反馈 -> 机器学习 -> App 更新。这次 Apple 还推出了急速审核（40分钟过审，摩拜单车团队已验证）和灰度发布的两大功能，也许就是为了给机器学习能自主的更新 App 铺路。目前苹果公布的 API 主要支持视觉识别、自然语言以及游戏 AI。
### 相关链接：
- [Build more intelligent apps with machine learning](https://developer.apple.com/machine-learning/)
- [Machine Learning API (Core ML)](https://developer.apple.com/documentation/coreml)
- [Introducing Core ML](https://developer.apple.com/videos/play/wwdc2017/703/)
![](http://upload-images.jianshu.io/upload_images/1721232-1ea43b2fa54d909d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 虚拟现实和增强现实
这些年虚拟现实遭遇了过山车式的变化。先是遭到资本疯狂追逐，大厂如三星、HTC 重金投入，索尼也趁势推出了 PlayStation VR；然后是 Pokemon Go 的大火，仿佛虚拟现实和增强现实的元年已经到来；突然，Facebook 的 Oculus 线下200家体验店关门；乐视的虚拟现实部门倒闭走人；直到上个月的 Google IO，Day Dream 的更新也表现平平。大家都认为，VR/AR 只是个概念炒作，充其量在游戏上起到锦上添花的效果。
直到今天。如同当年普鲁士一统德意志，苹果姗姗来迟，却在一出场就技惊四座。
Metal 2 十倍于当前性能的图像处理技术，芯片的升级和“硬件神兽” iMac Pro 的出现，让苹果在 AR/VR 上的雄心彰显无遗。现场的星球大战 VR 以及 iOS 11 即时战场 AR 展示完全颠覆了我对 AR 的认知 -- 要知道，这可是无需沉重的头套、专用的手机，直接用 iPad 即可完成的事情。
同样，Apple 推出了全新的 ARKit 来供开发者使用。配合 iPhone 和 iPad 上强大的传感器以及全新的 GPU、CPU，我们开发者可以使用 ARSession 这个共享单例来在摄像头上实现 2D 和 3D 的宏大场景构建。
### 相关链接：
- [Metal 2](https://developer.apple.com/metal/)
- [Introducing ARKit](https://developer.apple.com/arkit/)
- [Augmented Reality API (ARKit)](https://developer.apple.com/documentation/arkit)
- [VR with Metal 2](https://developer.apple.com/videos/play/wwdc2017/603/)
- [Introducing ARKit: Augmented Reality for iOS](https://developer.apple.com/videos/play/wwdc2017/602/)
![](http://upload-images.jianshu.io/upload_images/1721232-ad751eab597c7634.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# Swift 和 Xcode 9
Swift 毫无疑问是开发者关心的头等大事。作为苹果的野心，Swift 引入了很多非常先进的思路。然而令我有点失望的是，这次 Keynote 却对 Swift 和 Xcode 9 只字未提。
不过从现场看来，Swift 这次的更新主要是 API 的优化上，对现有的标准库进行了11项更新和1处新增，主要集中在基本数据类型和文件系统上。
Xcode 上，目前发现的更新主要是多个模拟器同时运行（鸡肋，估计会卡），本地化，runtime debug 工具，持续集成，和源码管理。特别致敬一下喵神，他的VVDocumenter 被集成在 Xcode 9 里面了。
我们只能期待秋季的苹果发布会上 Swift 4 进一步的更新了。有意思的是，之前 Swift 之父 Christ Lattner 写了篇总结文：[Chris Lattner 对 Swift 3 的总结与对 Swift 4 的展望](https://segmentfault.com/a/1190000006909760)。另外大家如果关心 [swift-evolution](https://github.com/apple/swift-evolution) 这个开源库，Swift
 4 的新功能应该可见一斑。
### 相关链接
- [Introducing Swift 4](https://developer.apple.com/swift/)
- [Localizing with Xcode 9](https://developer.apple.com/videos/play/wwdc2017/401/)
- [What's New in Signing for Xcode and Xcode Server](https://developer.apple.com/wwdc17/403)
- [Debugging with Xcode 9](https://developer.apple.com/videos/play/wwdc2017/404/)
- [Finding Bugs Using Xcode Runtime Tools](https://developer.apple.com/videos/play/wwdc2017/406/)
# iOS 11
这次 iOS 11 更新可以说是全面彻底。从基本的功能升级到 App 新功能添加，再到 iPad 平台 UI 重构。主要集中在这几个方面。
### iPad
为 iPad 全新打造的 iOS 系统，多任务、分屏 UI，Docker 工具栏。虚拟现实和增强现实的引入，以及对机器学习的支持。总体来说，消费者很开心，开发者又有新的东西可玩了。同时我觉得 iPad 越来越向 Macbook 看齐了。
### App
- App Store 全新设计
- Files 文件管理软件放出
- 开放 Apple Music
- Apple Pay 新的转账功能，注意 NFC API 接口开放，是不是为了 Apple Pay 更方便接入其它 App？
- Apple 地图继续追赶 Google 地图
- iMessage 花里胡哨扩展中
- HomeKit 追加音响（HomePod）支持
### 新功能
- Drag and Drop (梦想成真！)
- 拍照和截屏（扫描文件这个功能真是又重创了一大波 App）
- 中国特殊加成：二维码和上海话识别
### 相关 Session
- [Updating Your App for iOS 11](https://developer.apple.com/videos/play/wwdc2017/204/)
- [Building Great Document-based Apps in iOS 11](https://developer.apple.com/videos/play/wwdc2017/229/)
![](http://upload-images.jianshu.io/upload_images/1721232-a1bacdfaa000930b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# tvOS, watchOS 4, MacOS
这三个平台的更新可以说是泛善可陈（除了 Machine Learning 和 AR）。基本上就是，更多的内容（Amazon 的加入）、更炫更方便的 UI（运动界面和音乐界面）、以及更拽的名字（对，新 MacOS 叫 High）。
值得一提的是 Apple File System。其实这个文件系统去年就宣布了，不过并不在 Keynote 之中。今年苹果用它全面替代了古老的 HFS+，取而代之的是更高的性能、更好的安全性、更多的空间。这主要是 copy-on-write 和空间共享技术的运用。
Apple 同时开放了 [FileProvider API](https://developer.apple.com/documentation/fileprovider)，开发者可以用它在新的 Apple File System 基础上实现对文件各种基本操作，还能通过它对应的 extension 实现诸如
 drag and drop 这样跨 App 的功能。
![](http://upload-images.jianshu.io/upload_images/1721232-f0ec2785b47023e1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### 相关链接
- [What's new in Apple File System](https://developer.apple.com/videos/play/wwdc2017/715/)
- [File Provider Enhancements](https://developer.apple.com/videos/play/wwdc2017/243/)
# 总结
这次的 WWDC，苹果可以说是打了一个漂亮的翻身仗：在人工智能方面挑战 Google，在虚拟、增强现实方面加入战局震慑 Facebook，并重夺工业界顶峰的宝座。 watchOS 和 tvOS 的丰富，虽然进步并不明显，但好歹补足了短板。iOS 11 让 iPad 这个日趋边缘的产品逐渐焕发出了生机。后面的几天，应该会有更多精彩的 Session，给开发上带来更多的启示。
