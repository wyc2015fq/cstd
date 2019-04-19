# 玩转Swift(一) - 三少GG - CSDN博客
2016年01月16日 22:19:58[三少GG](https://me.csdn.net/scut1135)阅读数：1737
[The
 Swift Programming Language 中文版](http://wiki.jikexueyuan.com/project/swift/)
[http://wiki.jikexueyuan.com/project/swift/](http://wiki.jikexueyuan.com/project/swift/)
学习路径：
入门先看liuyubobobo的《玩转Swift》了解swift这个语言的特性    [http://www.imooc.com/learn/127](http://www.imooc.com/learn/127)
进阶看sundy的《iOS开发玩转界面》了解Cocoa touch 的各个UIKit组件   [http://www.maiziedu.com/course/ios/18-349/](http://www.maiziedu.com/course/ios/18-349/)
另外再推荐看网易公开课的《斯坦福大学公开课-IOS8开发》
剩下的就是看各个教程做出不同的小程序、小游戏。
熟悉Alamofire之类的第三方开源项目，基本上你可以做个各种类型的app了，后面就是不断累积的过程。
**Swift 学完后如何上手 UIKit 框架及其他常用类库？**建议路径：
1. 忘掉界面编辑器
2. 弄清楚AppDelegate是怎么成为app入口的，怎么通过self.window.rootViewController赋值启动第一个主窗口
3. 弄清楚UIView的控件堆砌/去除方法addChild和removeFromParent之类，稍微了解一下View和Controller的关系
4. 弄清楚UIView控制外观位置大小的一般盒模型frame/bounds/anchorPoint/position，注意有些属性在uiview.layer里，虽然我多半都在用frame。以及放缩旋转必须用transform，没现成的属性可以用。
5. 一般的非容器具体控件具体属性自己看文档，只是建议留神一下UIButton的title和image都要用set方法，没有直接的=方法可以用来赋值，就是为了在不同状态下设置不同的东西，以及控制Button上面的image大小写法特别扭曲（要设置inset值），很多时候不如直接addChild一个UIImageView
6. 容器控件UIScrollerVie和UITableView要了解设置代理，填充数据，响应滚动事件的写法。UITableView的古怪之处是要给它一个类让他可以用那个类新建单行控件。
7. 弄清楚UINavigationController的Push/Pop怎么用，怎么手写代码向Navigation上增加按钮，UiTabbarController作为界面容器其实也差不多但现在用的不多了。
8. 现在可以学一下UIView animateWithDuration方法的妙处了，各种华丽精美的，特别苹果范儿的小动画都要靠这玩意实现。
http://www.maiziedu.com/course/ios/16-237/
###开源###
http://dev.swiftguide.cn (同样https://github.com/ipader/SwiftGuide）
http://www.swifttoolbox.io
/////////////////
所有库的太长不看列表，快速访问：
1. [DZNEmptyDataSet]() [UI, 空table view解决方案]
2. [PDTSimpleCalendar]() [UI, 可放入日历组件]
3. [MagicalRecord]() [Core Data帮手实现活动记录模式]
4. [Chameleon]() [UI, 颜色框架]
5. [Alamofire]() [Swift网络]
6. [TextFieldEffects]() [UI, 自定义外观text fields]
7. [GPUImage]() [快速图像处理]
8. [iRate]() [获得用户评价]
9. [GameCenterManager]() [轻松管理Game Center]
10. [PKRevealController]() [UI, 滑动边栏]
11. [SlackTextViewController]() [UI, 高度自定义text field]
12. [RETableViewManager]() [用代码动态创建table view]
13. [PermissionScope]() [UI, 巧妙的提前问用户要系统许可]
14. [SVProgressHUD]() [UI, 自定义等待菊花]
15. [FontAwesomeKit]() [轻松地添加酷字体到你的app中]
16. [SnapKit]() [用代码轻松auto layout]
17. [MGSwipeTableCell]() [UI, 可滑动的table view cells]
18. [Quick]() [Swift 单元测试框架]
19. [IAPHelper]() [app内购封装帮手]
20. [ReactiveCocoa]() [FRP框架]
21. [SwiftyJSON]() [Swift JSON库]
22. [Spring]() [动画框架]
23. [FontBlaster]() [轻松在app中加载自定义字体]
24. [TAPromotee]() [在你的app中交叉提示，置入界面]
25. [Concorde]() [下载和解码进度化JPEGs]
26. [KeychainAccess]() [轻松管理钥匙串]
27. [iOS-charts]() [漂亮的图表库]
http://www.cocoachina.com/special/swift/
######
实践
怎样在Swift中使用CocoaPods [http://www.cocoachina.com/swift/20150630/12305.html](http://www.cocoachina.com/swift/20150630/12305.html)
###################
Swift VS Objective-C
著作权归作者所有。
商业转载请联系作者获得授权，非商业转载请注明出处。
作者：阮超
链接：https://www.zhihu.com/question/24123236/answer/26751939
来源：知乎
最早的回答是 Swift 刚刚诞生的时候写下的，从现在 (2015.12.13 ) Swift 从很多方面都已经非常适合新人学习，语言的规范程度，文档和社区的成熟，已经 Swift 本身的开源，都标志着Swift 开发的新时代的到来，少年们，学习 Swift 吧
https://www.zhihu.com/question/24004835
https://www.zhihu.com/question/24304009
Swift 刚发布的时候有人说「这下所有人都回到零起点了」，实际上是外行的一个常见的误解。Objective-C / Swift 和 iOS 开发是两个不同的概念，这点希望你清楚。前者仅仅是一门编程语言，而后者则包括了编程思想、Cocoa 框架的使用，甚至交互设计规范、界面设计规范等 iOS 开发所需的一整套东西，并且绝大部分是**和语言无关的**。从学习的角度看，大部分时间你将花在学习**后者**上，而具体语言和语法只占很小的比例。Objective-C
 和 Swift 虽然语法上差异很大，但是背后的思想以及所依托的类库都是相同的。关于 iOS 应用开发你能想到的绝大多数问题，例如：
- 一个应用的代码是从哪里开始执行的？
- 如何建立一个按钮，并实现用户点击这个按钮之后调用一个函数？
- 如何修改一个标签的文字？
- 如何控制系统发出声音或者播放视频？
- 如何检测设备的朝向？
它们的答案都取决于 Cocoa 框架如何使用，而非具体采用的语言是什么——你关注的应该是背后的「我该做什么，为什么要这么做」，而不是简单的「具体怎么去写」——不同语言仅仅是一个写法不同而已，手册和网络上都可以查到，用多了自然掌握。
因此，对于一个已经掌握了 Objective-C 和 iOS 开发的人，迁移到 Swift 是几天就可以搞定的；而对于一个掌握了 Swift 和 iOS 开发的人，迁移到 Objective-C 也不会很困难。
