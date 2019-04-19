# 27个iOS开源库，让你的开发坐上火箭吧 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月18日 22:17:04[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：280标签：[iOS学习资料																[iOS 资源大全																[iOS常用方法																[库](https://so.csdn.net/so/search/s.do?q=库&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
本文翻译自Medium，原作者是Paweł Białecki，[原文](https://medium.com/app-coder-io/27-ios-open-source-libraries-to-skyrocket-your-development-301b67d3124c)
# 27个iOS开源库，让你的开发坐上火箭吧
你不会想错过他们，真的。
***我爱开源。***
并且我喜欢开发者们，把他们宝贵的私人时间用来创造神奇的东西，然后他们会和其他人分享并且不求回报。**开源作者和贡献者，你们是最帅的。**感谢你们一直以来的工作。
**所以，因为[我是一个分类整理狂](http://iosappsweekly.com/)，这是我从iOS开源库中选出的[最喜欢的](https://github.com/stars/ecler)。**这些项目的顺序都是随机的，全部都很酷。
绝大部分库是支持[CocoaPods](https://cocoapods.org/)的，所以把它们添加到你的Xcode项目中轻而易举。
**文章的尾部你会看到一个太长不看的版本**——一个简单的列表，只有标题和到项目的链接。如果你发现这篇文章是有用的，**把它和你的iOS开发者兄弟们分享。好东西需要被传播。**
## DZNEmptyDataSet
这本应该是iOS中一个标准、内置的解决空table和collection view的方式。默认的如果你的table view是空的，屏幕就是空的。但这不是你能提供的最好的用户体验。
用了这个库，你只需要遵循一系列协议，iOS会优雅地接管你的collection view并且会正确、好看地显示给用户信息。很明显，每个iOS项目都应该采用。
![](http://upload-images.jianshu.io/upload_images/861914-af27c8ff96c227a5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这是完全可自定义的。
### CocoaPods：
pod ‘DZNEmptyDataSet’
## [GitHub](https://github.com/dzenbot/DZNEmptyDataSet)
## 2. PDTSimpleCalendar
你的app是否需要一个简单、好看并且有效的日历组件呢？
现在你有了——PDTSimpleCalendar很有可能是最棒的iOS日历组件。有很多方式来自定义它，逻辑有效而且好看。
![](http://upload-images.jianshu.io/upload_images/861914-ebee361378a2aae9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### CocoaPods：
pod 'PDTSimpleCalendar'
## [GitHub](https://github.com/jivesoftware/PDTSimpleCalendar)
## 3. MagicalRecord
他们说，Core Data很简单。他们说，它很好很简单。哈哈，你是认真的吗，苹果？一顿陈词滥调的代码被添加到每个项目里，这真的不够优雅和简单。更不用说添加、移除和更新很多实体，保存上下文，为不同的环境创建不同的Core Data栈，等等等等。我当然很喜欢Core Data，但是苹果真的可以通过一个简单的好方法来简化它——MagicalRecord方法。
MagicalRecord的工作就像一个Core Data的包装，并且向开发者隐藏了所有无关的东西。如果你曾经用过活跃纪录模式（例如Ruby on Rails），那你已经掌握它了。如果你在app里用Core Data的话真的真心推荐这个库。
## CocoaPods：
pod 'MagicalRecord'
## [GitHub](https://github.com/magicalpanda/MagicalRecord)
## 4. Chameleon
如果你读到了这一点，你是一个很好的程序员，而不是一个设计师。这就是为你准备的。
![](http://upload-images.jianshu.io/upload_images/861914-e4b3a251abcfb829.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Chameleon是iOS的一个颜色框架。它用好看、摩登的扁平化颜色扩展了UIColor。它也给了我们能力来创建调色板，里面是我们自己定义的颜色。它可以做很多其他事，探索readme文件。如果你想要好看的应用程序，快把这个库加到你的项目里吧。
![](http://upload-images.jianshu.io/upload_images/861914-ae8e84d56f1dee43.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Chameleon基础扁平化颜色
### CocoaPods：
pod 'ChameleonFramework'
### [GitHub](https://github.com/ViccAlexander/Chameleon)
## 5. Alamofire
Alamofire是一个用Swift写的优雅的网络库。你曾经用过AFNetworking吗？ALamofire是它的弟弟。年轻也更有才华，当然啦（AFNetworking是用Objective-C写的）。
![](http://upload-images.jianshu.io/upload_images/861914-2b844c334358151f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
需要做网络相关比如下载、上传、获取JSON等等？Alamofire是为你准备的。GitHub上8000人的选择不会错。
### CocoaPods：
pod 'MagicalRecord'
### [GitHub](https://github.com/Alamofire/Alamofire)
## 6. TextFieldEffects
你不觉得标准的UITextField有一点无聊吗？我也是——所以对TextFieldEffects说hello吧！我不会写太多，我只会展示你一些这个库可以做的例子：
![](http://upload-images.jianshu.io/upload_images/861914-b8a06fcacf854990.gif?imageMogr2/auto-orient/strip)
![](http://upload-images.jianshu.io/upload_images/861914-39e9f3837da205ec.gif?imageMogr2/auto-orient/strip)
![](http://upload-images.jianshu.io/upload_images/861914-53c7ac1a55c85913.gif?imageMogr2/auto-orient/strip)
是的，**这些就是简单的易用的控制器。**你甚至可以用storyboard里的IBDesignable！
不幸的是这个库不支持CocoaPods（如果你来自未来，而这在一些时间之前改变了的话，[请在Twitter上让我知道](https://twitter.com/pawel_bialecki)），但它支持Carthage。你也可以简单的从GitHub下载项目，并且把它添加到你的workspace里。
### Carthage：
github "raulriera/TextFieldEffects"
### [GitHub](https://github.com/raulriera/TextFieldEffects)
## 7. GPUImage
你曾经创建过一个摄像机app吗？**如果没有，看完这个库你就肯定会的。**
![](http://upload-images.jianshu.io/upload_images/861914-9d701a51ce365975.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
GPUImage可能性
GPUImage提供我们一个GPU-accelerated摄像头效果（图像和视频都可以），熊熊燃烧般的速度。App Store里有几百个app使用这个库——其中就有我的一个：
![](http://upload-images.jianshu.io/upload_images/861914-8de2b71fcfb34698.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
我的一个app中用的GPUImage
GitHub上8869个收藏并且还在持续增加。
### CocoaPods：
pod 'GPUImage'
### [GitHub](http://github.com/BradLarson/GPUImage)
## 8. iRate
在App Store获得更多评价最好的方式是什么？我没有明确的数据来回答这个问题，但如果要我来猜测的话，我会说只要**简单的询问用户**就可以了。也许这是一个老套的方式——大部分开发者现在创建自定义的app内置提醒——但如果你没有时间或者不想所有事都从头做起，用iRate比不用要好。并且这就是iRate——**一个小库，你包含在你的项目里并且忘记询问用户去评价了——iRate会自动替你完成，在合适的时间里。**
### **CocoaPods：**
pod 'iRate'
### [GitHub](https://github.com/nicklockwood/iRate)
## 9. GameCenterManager
喜欢或讨厌一个人，在这种情况下管理Game Center非常简单，只需要一点我们最著名的反模式的帮助（你的游戏里只有Game Center，对吧？）
![](http://upload-images.jianshu.io/upload_images/861914-894ebfa1ebd5fa6c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
诚实地说，在iOS里香草管理Game Center并不是那么困难，**但用这个库就是简单和快速。**更好的是好的敌人。
![](http://upload-images.jianshu.io/upload_images/861914-29186e49d73c75bb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
我的一个游戏里就用了它，真是一个愉悦的体验。
### CocoaPods：
pod 'GameCenterManager'
### [GitHub](https://github.com/nihalahmed/GameCenterManager)
## 10. PKRevealController 2
这是一个真正的宝石，**我最喜欢的iOS控件之一。**PKRevealController是一个可滑动的侧边栏（向左、向右或者都可以），用你的手指来滑动（或者只通过点击按钮，但这样就没有滑动酷了）。
![](http://upload-images.jianshu.io/upload_images/861914-62a54d9e488396b1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
我使用过一些其它库，提供这种空间，PKRevealController是最好的。设置起来很简单，高度可自定义，手势识别非常非常好。它可以在iOS SDK中包含作为一个标准空间，真的。
### CocoaPods：
pod 'PKRevealController'
### [GitHub](https://github.com/pkluz/PKRevealController)
## 11. SlackTextViewController
你曾经用过Slack iOS app吗？如果你在一个更大的软件公司工作的话，很有可能回答是。对于没有这么回答的人——Slack很坚硬。Slack的iOS app也是，特别是对于很好的、自定义的文本输入控件…就是你现在有的——你的app里可以用的代码！
**自增长文本域？对的。手势识别，自动填充，多媒体粘贴？对的。简单的易用的？对的。**你还可能需要什么？
### CocoaPods：
pod 'SlackTextViewController'
### **[GitHub](https://github.com/slackhq/SlackTextViewController)**
## 12. RETableViewManager
RETableViewManager会帮助你动态创建和管理你的table view，都用代码。它提供我们预先定义好的cell（布尔型、文本、日期等等。——看下面的截图），你也可以创建你的自定义视图，和默认的一起使用。
![](http://upload-images.jianshu.io/upload_images/861914-e03b920c0376370b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
左侧截图好老套！
这些你都可以在storybard里做而不需要这个库的帮忙，但有的时候代码比可视化编辑器更简单。
### CocoaPods：
pod 'RETableViewManager'
### [GitHub](https://github.com/romaonthego/RETableViewManager)
### 13. PermissionScope
有用的库，通过在询问用户**之前**提示用户需要系统许可带来更好的用户体验。高度可接受程度->更多用户活动使用app->更好的留存->更好的数据->更多的下载。被高度推荐的pod。
![](http://upload-images.jianshu.io/upload_images/861914-ee80c097dad4e660.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### CocoaPods：
pod 'PermissionScope'
### [GitHub](https://github.com/nickoneill/PermissionScope)
## 14. SVProgressHUD
这个图片正在被正确的加载，不会等很久也不需要刷新页面。**这就是SVProgressHUD在你的app里看起来的样子。**如果你需要自定义等待指示，这里就有一个（很有可能就是最好的）。
![](http://upload-images.jianshu.io/upload_images/861914-8c9e0f980e040354.gif?imageMogr2/auto-orient/strip)
### CocoaPods：
pod 'SVProgressHUD'
### [GitHub](https://github.com/TransitApp/SVProgressHUD)
## 15. FontAwesomeKit
**Font Awesome很酷**，用这个库你可以轻松地添加字体到你的项目里，并且在多种方式里使用它。
![](http://upload-images.jianshu.io/upload_images/861914-4f6f218e82209ca1.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### CocoaPods：
pod 'FontAwesomeKit'
### [GitHub](https://github.com/PrideChung/FontAwesomeKit)
## 16. SnapKit
喜欢auto layout？你应该！
至少在storyboard里创建的时候。
用代码创建constraints是痛苦的，如果没有帮助的话，但幸运的是SnapKit在这里，使用它你可以轻松写你的constraints，声明方式。看看吧。
![](http://upload-images.jianshu.io/upload_images/861914-244dd474b5c09563.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### CocoaPods：
pod 'SnapKit'
### [GitHub](https://github.com/SnapKit/SnapKit)
## 17. MGSwipeTableCell
另一个UI组件，在很多app里都常见，苹果应该考虑在iOS标准库里加入类似的东西。**可滑动的table cell**，这是这个pod最好的描述。最好的一个。
![](http://upload-images.jianshu.io/upload_images/861914-e89287c195c838da.gif?imageMogr2/auto-orient/strip)
![](http://upload-images.jianshu.io/upload_images/861914-4d356408526b4bb7.gif?imageMogr2/auto-orient/strip)
![](http://upload-images.jianshu.io/upload_images/861914-142a679117603fed.gif?imageMogr2/auto-orient/strip)
这些只是3个动画种类，还有很多。探索readme文件。
### CocoaPods：
pod 'MGSwipeTableCell'
### [GitHub](https://github.com/MortimerGoro/MGSwipeTableCell)
## 18. Quick
Swift里的单元测试，给Swift（好吧，要给Objective-C），和Xcode融为一体。如果你是一个Objective-C粉，我会推荐你Specta而不是这个，但对于Swift Quick很有可能是最好的。
![](http://upload-images.jianshu.io/upload_images/861914-d7b409c6f56dbab8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-3dfc2f36c8d6a899.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### CocoaPods：
pod 'Quick'
### [GitHub](https://github.com/Quick/Quick)
## 19. IAPHelper
app内购带给我们很多陈词滥调的代码，用这个库就不需要了，把最有关交易金钱的普遍的任务从iOS用户到你的（或者你的公司的）钱包简单包装起来。
### CocoaPods：
pod 'IAPHelper'
### [GitHub](https://github.com/saturngod/IAPHelper)
## 20. ReactiveCocoa
好吧，这里我们有一个小怪兽。
![](http://upload-images.jianshu.io/upload_images/861914-6c7ab0453e80829a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
ReactiveCocoa并不是很小、易用的项目，就像这个列表里其它项目一样。**ReactiveCocoa带给我们一种完全不同的编程方式/结构，基于值的信号和流。**这是完全的头脑风暴，首先你需要忘记你曾经学习的来理解它是如何工作的。这不是一个简单的任务，但是是有回报的。
这不是一个合适的地方来教你使用ReactiveCocoa，但我会给你一些好的资源，如果你感兴趣的话。
### [Getting Started with ReactiveCocoa](http://www.teehanlax.com/blog/getting-started-with-reactivecocoa/)
### [ReactiveCocoa](http://nshipster.com/reactivecocoa/)
### [ReactiveCocoa Tutorial - The Definitive Introduction: Part 1/2](http://www.raywenderlich.com/62699/reactivecocoa-tutorial-pt1)
### CocoaPods：
pod 'ReactiveCocoa'
### [GitHub](https://github.com/ReactiveCocoa/ReactiveCocoa)
## 21. SwiftyJSON
让Swift里的JSON解析更简单。
### CocoaPods：
pod 'SwiftyJSON'
### [GitHub](https://github.com/SwiftyJSON/SwiftyJSON)
## 22. Spring
做动画更简单、可链和声明。
![](http://upload-images.jianshu.io/upload_images/861914-32e5f0336c9a9e10.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### CocoaPods：
pod 'Spring'
### [GitHub](https://github.com/MengTo/Spring)
## 23. FontBlaster
在app里加载自定义字体更简单。
### CocoaPods：
pod 'FontBlaster'
### [GitHub](https://github.com/ArtSabintsev/FontBlaster)
## 24. TAPromotee
交叉推销你的app是最棒的销售策略之一，你可以在它们中免费实现。用这个库非常简单，你不再能为自己辩解为什么不用它——增加TAPromotee到你的podfile，配置、然后免费享受更多下载。
![](http://upload-images.jianshu.io/upload_images/861914-4e25ff785aadd75b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### CocoaPods:
pod 'TAPromotee'
### [GitHub](https://github.com/JanC/TAPromotee)
## 25. Concorde
在你的app加载很多JPEG吗？用Concorde你可以加载地更好看。有进度的方式。
![](http://upload-images.jianshu.io/upload_images/861914-0482228ba815f6b6.gif?imageMogr2/auto-orient/strip)
### CocoaPods:
pod 'Concorde'
### [GitHub](https://github.com/contentful-labs/Concorde)
## 26. KeychainAccess
管理钥匙串权限的小帮手库。
![](http://upload-images.jianshu.io/upload_images/861914-0dceea138c8956c0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-9b3a2606d46f43c7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-d3f08d23e9a43a87.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### CocoaPods:
pod 'KeychainAccess'
### [GitHub](https://github.com/kishikawakatsumi/KeychainAccess)
### 27. iOS-charts
最后但重要的——iOS图表库！很简单和有效，我不会在这儿写太多——滑**动到下面去看在你的app用它可以干嘛。**
![](http://upload-images.jianshu.io/upload_images/861914-0c374a3e924540d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-ed772e9dfa73598d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-dc6d80251d1c5341.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-bd66a6dae3e7c811.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-3d04e22674183ee5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-af78c57b2ce2e698.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-12c451ad246d01c8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-aab25e8afd68e2d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-24b84e17c53d3b1d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-0ee28b50739b2b1d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-573804d25589adc3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-054d2a7dede014ad.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-219f1c200abfeb3e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/861914-7ab5ed589df30357.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
是的，所有东西都是可用的，作为一个可放进去（好吧，也许是“可写进去”）的组件。
不幸的是目前还没有CocoaPods支持，所以你需要手动拖动项目到你的Xcode workspace中。
## 所有库的太长不看列表，快速访问：
1. [DZNEmptyDataSet](https://github.com/dzenbot/DZNEmptyDataSet) [UI, 空table view解决方案]
2. [PDTSimpleCalendar](https://github.com/jivesoftware/PDTSimpleCalendar) [UI, 可放入日历组件]
3. [MagicalRecord](https://github.com/magicalpanda/MagicalRecord) [Core Data帮手实现活动记录模式]
4. [Chameleon](https://github.com/ViccAlexander/Chameleon) [UI, 颜色框架]
5. [Alamofire](https://github.com/Alamofire/Alamofire) [Swift网络]
6. [TextFieldEffects](https://github.com/raulriera/TextFieldEffects) [UI, 自定义外观text fields]
7. [GPUImage](https://github.com/BradLarson/GPUImage) [快速图像处理]
8. [iRate](https://github.com/nicklockwood/iRate) [获得用户评价]
9. [GameCenterManager](https://github.com/nihalahmed/GameCenterManager) [轻松管理Game Center]
10. [PKRevealController](https://github.com/pkluz/PKRevealController) [UI, 滑动边栏]
11. [SlackTextViewController](https://github.com/slackhq/SlackTextViewController) [UI, 高度自定义text field]
12. [RETableViewManager](https://github.com/romaonthego/RETableViewManager) [用代码动态创建table view]
13. [PermissionScope](https://github.com/nickoneill/PermissionScope) [UI, 巧妙的提前问用户要系统许可]
14. [SVProgressHUD](https://github.com/TransitApp/SVProgressHUD) [UI, 自定义等待菊花]
15. [FontAwesomeKit](https://github.com/PrideChung/FontAwesomeKit) [轻松地添加酷字体到你的app中]
16. [SnapKit](https://github.com/SnapKit/SnapKit) [用代码轻松auto layout]
17. [MGSwipeTableCell](https://github.com/MortimerGoro/MGSwipeTableCell) [UI, 可滑动的table view cells]
18. [Quick](https://github.com/Quick/Quick) [Swift 单元测试框架]
19. [IAPHelper](https://github.com/saturngod/IAPHelper) [app内购封装帮手]
20. [ReactiveCocoa](https://github.com/ReactiveCocoa/ReactiveCocoa) [FRP框架]
21. [SwiftyJSON](https://github.com/SwiftyJSON/SwiftyJSON) [Swift JSON库]
22. [Spring](https://github.com/MengTo/Spring) [动画框架]
23. [FontBlaster](https://github.com/ArtSabintsev/FontBlaster) [轻松在app中加载自定义字体]
24. [TAPromotee](https://github.com/JanC/TAPromotee) [在你的app中交叉提示，置入界面]
25. [Concorde](https://github.com/contentful-labs/Concorde) [下载和解码进度化JPEGs]
26. [KeychainAccess](https://github.com/kishikawakatsumi/KeychainAccess) [轻松管理钥匙串]
27. [iOS-charts](https://github.com/danielgindi/ios-charts) [漂亮的图表库]
感谢阅读，真是一个长列表！如果你认为创建是有价值的，**请分享它，通过点击文章下方的分享按钮**——更多人会从中受益。并且如果你是一个Meduim用户，**请点击推荐按钮**——它会鼓舞我创建更多iOS开发文章！
**One more thing**——如果你在读这篇文章，你很有可能是个**iOS开发者。**很多iOS开发者也是iPhone用户——所以我有一些你可能感兴趣的东西。
**我运行着一个[每周一次手选最好的iOS app和游戏](http://iosappsweekly.com/)——看看吧**，我关注高质量并且只挑选酷的app。连接也会很酷！
![](http://upload-images.jianshu.io/upload_images/861914-f8dd01b052db3a9c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
你也可以在[推特上follow我](https://twitter.com/pawel_bialecki)，我最经常讨论iOS开发的地方。
#### 推荐阅读：
### [27 places to learn iOS development. Best ones.](https://medium.com/p/b1bcfb48efab)
### [52 people every iOS developer should follow on Twitter](https://medium.com/p/25ca8915369a)
文／张嘉夫（简书作者）
原文链接：http://www.jianshu.com/p/228535226656
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
