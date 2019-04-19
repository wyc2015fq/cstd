# Flutter 1.0 正式版： Google 的便携 UI 工具包 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年12月08日 11:57:10[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：115
Flutter 1.0 正式版: Google 的便携 UI 工具包
文 / Tim Sneath，Google Dart & Flutter 产品组产品经理
Flutter 是 Google 打造的 UI 工具包，通过一套代码同时在 iOS 和 Android 上构建媲美原生体验的精美应用！
如今，移动开发者们在同时兼容 iOS 和 Android 时有两种选择：走原生开发路线，把界面和逻辑在不同平台分别实现；抑或用同一套代码兼容多个平台，但这往往意味着运行速度和产品保真度的损失。我们坚信，Flutter 为您提供了一套两全其美的解决方案——既能用原生 ARM 代码直接调用的方式来加速图形渲染和 UI 绘制，又能同时运行在两大主流移动操作系统上。
Flutter介绍
Flutter 并不是要替代原生应用开发模式，您可以用 Flutter 开始一个全新的应用，也可以把 Flutter 理解为应用内置的一个引擎，把这个引擎引入到现有的工程里。
让我来从以下四个特点为您介绍 Flutter: 
- Flutter 可以帮助您构建界面精美的应用。我们希望设计师们都能充分发挥自己的想象，尽情施展精美绝伦的创意，而不受框架局限性的干扰。通过 Flutter，您可以在屏幕上有像素级的掌控。Flutter 强大的图像组合能力让您近乎可以不受限制的堆叠任何图形、视频、文本和控件。Flutter 内置了一系列的 widgets (在 Flutter 里，几乎所有内容都是 widget)，这些 widgets 可以帮助您在 iOS 和 Android 上实现极致的视觉体验。Flutter 集成并有最大程度的实现了 Material Design —— 这是 Google 为数字体验所创作的开放的设计系统。
- Flutter 的另一个特点的速度快。它基于 Skia 2D 硬件加速图形引擎，该引擎也同样用在了 Chrome 和 Android 平台。媲美原生应用的速度，这是我们在构建 Flutter 时的初心。Flutter 的代码基于 Dart 平台，它可以被编译成 iOS 和 Android 平台上 32 位和 64 位的 ARM 代码。
- Flutter 非常高效。Flutter 引入了 Stateful Hot Reload（保持应用状态的热重载），这个革命性的新特性可以让移动开发者和设计师们实时迭代应用程序。通过 Stateful Hot Reload，无需重新启动应用，你就可以在程序运行的时候直接看到代码修改之后的效果，Stateful Hot Reload 改变了开发者们编写应用的方式。在我们的用户反馈中，开发者们表示该特性使得开发效率提升了三倍。
- 最后，Flutter 是开放的。Flutter 是一个基于 BSD-style 许可的开源项目，全球数百位开发者在为其贡献代码。Flutter 的插件生态系统平台也充满活力。有数千款插件已经发布，避免了重复造轮子。由于 Flutter 应用程序使用标准的 Android 和 iOS 的编译打包工具 (build tools)，因此它的开放还体现在您可以使用原生开发资源。比如，您同样可以在 Android 上使用 Kotlin 或者 Java，在 iOS 上使用 Swift 或者 Objective-C 来写逻辑或者界面。
结合以上这些，再加上您顺手的开发工具，比如 Visual Studio Code、Android Studio、IntelliJ 或其他您青睐开发者编辑器，您就可以开始使用 Flutter 体验一套代码同时在 iOS 和 Android上实现原生应用效果的乐趣了。
![](http://note.youdao.com/yws/res/109625/D382B22DE6D94B3FA8ADD25AFC335655)
Flutter 势头正旺
在 10 个月前的世界移动大会 (MWC) 上，我们发布了 Flutter 的[第一个 Beta 版本](http://mp.weixin.qq.com/s?__biz=MzAwODY4OTk2Mg==&mid=2652046552&idx=1&sn=ec4db0a0f43e5062a81745bd140b985f&chksm=808ca49db7fb2d8be6809ddb77f2ae43df6a436e3d7d797118fd5f8e49d7558ca55af02a7739&scene=21#wechat_redirect)，在正式版本尚未推出之前，已经有成百上千的基于 Flutter 开发的应用在 Apple Store 和 Google Play 上架，很显然，广大的开发者们也已经准备好迎接这种全新的 UI 开发方式了。
在 Google 内部，Flutter 已经被广泛用于多个产品，比如 Google Ads 已经将其产品的 iOS 版本和 Android 版本转向使用 Flutter。在正式版本之前，全世界已经有多个公司开始使用 Flutter 来开发应用，包括 Abbey Road Studios、阿里巴巴、Capital One、Groupon、Hamilton、京东、Philips Hue、Reflectly 以及腾讯等。
Capital One 团队的高级工程总监 Michael Jones 对于 Flutter 提到了如下的使用心得: 
“我们对于 Flutter 所带来的高效的跨平台开发体验感到十分兴奋。我们的工程师对其中的快速开发和 hot reload 给予了极高的评价。而且在过去的一年里，我们也看到了整体平台巨大的变化，尤其是和原生系统深度集成的效果。”
“Flutter 使 Capital One 研发团队摆脱了 iOS 优先还是 Android 优先的固定思维，而是真正的移动端优先的开发模式。非常高兴能看到 Flutter 1.0 的发布，并且也对 Flutter 如此快速的发展和开发者社区所迸发的热情所感动。”
*注: Capital One 是美国十大银行之一，《财富》世界 500 强企业。
在今天的 Flutter Live 活动中，知名的支付服务商 Square 正式公布了两个全新的 Flutter SDK。它们可以更加简化基于 Flutter 的支付操作和服务，无论是使用 Square 支付识读器直接支付还是使用手机应用进行支付。Square 演示了一款基于它们的支付 SDK 所开发的应用。该应用来自 Collins Family Orchards，这是一家在太平洋西北部种植和销售水果的家族农场。
- Flutter SDK: 
https://squareup.com/flutter
![](http://note.youdao.com/yws/res/109624/768B70AADAD84316A7BAD1C2C770BAAC)
Collins Family Orchards 应用的开发者 Dean Papastrat 认为: 
“应用在生产环境下的动画流畅度和页面的切换速度真的让我大开眼界。作为一个 Web 开发者，上手使用 Flutter 非常简单。真的没想到能够在短短的一周时间就完成了一个能够实现完整功能的应用。”
同样是在今天的 Flutter Live 活动上，2Dimensions 宣布了一款全新的工具 Flare，该工具可以将矢量动画直接嵌入到 Flutter 应用中，并且用代码来控制。Flare 打破了传统的模式，无需再单独开发应用和动画，然后再转换成设备资源和代码。
- Flare: 
https://medium.com/2dimensions/flare-launch-d524067d34d8
![](http://note.youdao.com/yws/res/109626/E7BBE75CC5F64FDFA4E61AACBB81A1B7)
使用 Flare 实现的动画可以作为 widget 嵌入到已有的 Flutter 应用中。使其能够融入整体的界面中，和文本、图形渲染层以及其它 UI widget 叠加在一起。通过这种集成方式，可以将动画从其他平台的 “黑盒” 局限性解放出来，持续促进开发者与设计师协作完成应用。Flutter 和 Flare 的紧密整合激发了设计师和动画师的热情来构建设计精美的移动应用。
为您介绍另外一位 Flutter 的合作伙伴 Nevercode，这是一家快速成长的，为移动应用提供持续集成和推送工具的提供商。今天，他们正式发布了 Codemagic。该工具针对 Flutter 设计，简化了 Android 和 iOS 平台上的应用编译和打包的处理过程。现在已经发布 beta 版本。使用 Codemagic，开发者只需要在 GitHub 里选择一个含有 Flutter 项目的仓库，仅需简单的几个点击即可生成一整套处理流程，包括执行测试，生成应用软件包并上传至 Apple Store 和 Google Play。
- Codemagic:
https://codemagic.io/
这里我们整理了一个短视频，为大家展示开发者们基于 Flutter 的 Beta 版本所实现的应用，长按下方☟二维码☟可直接观看：
![](http://note.youdao.com/yws/res/109627/8E6B5C29195B40ED8E54B62A1789ED22)
1.0 版本中引入的新特性
自[首个 Beta 版](http://mp.weixin.qq.com/s?__biz=MzAwODY4OTk2Mg==&mid=2652046552&idx=1&sn=ec4db0a0f43e5062a81745bd140b985f&chksm=808ca49db7fb2d8be6809ddb77f2ae43df6a436e3d7d797118fd5f8e49d7558ca55af02a7739&scene=21#wechat_redirect)发布以来，我们持续为 Flutter 添砖加瓦，精雕细琢。尤其是今年 [PR2 版本](http://mp.weixin.qq.com/s?__biz=MzAwODY4OTk2Mg==&mid=2652047108&idx=2&sn=242ef51c15fef1f386ebb2ac322e490c&chksm=808ca741b7fb2e5723a16b633f12cacb124b60bd37b3c59929aae3610628e86326885275467f&scene=21#wechat_redirect)的发布，我们为 iOS 风格提供了全新的 widgets，接入了近 20 种 Firebase 服务，以及不断优化性能、压缩由 Flutter 打包的应用大小。此外，我们也悉心听取 Flutter 社区反馈，解决了大量的问题。
此时，Flutter 正式开始使用 Dart 平台新的 2.1 版本，这是对 Dart 2 的一次更新。它不仅生成更小的代码，还能够提供更快的类型检查以及更有用的类型错误提示信息。Dart 2.1 也带来了新的语言特性，让开发构建过程事半功倍。这是来自开发者们的好评声，他们在切换到新的引擎后感受到了显著的速度提升: 
![](http://note.youdao.com/yws/res/109628/B2A4D28750074295BBBC0815BB406AFA)
1.0 正式版的主要目的是修复 Bug 和提高稳定性，但我们也非常高兴地宣布：将有两个新功能随 1.0 正式版的预览模式提供给广大开发者。它们将会于 2019 年二月的季度更新中正式推出，两个特性分别是:
引入 Flutter 到现有工程 (Add to App)
起初在构思 Flutter 的时候，我们主要关注于从零开始构建应用的场景。但事实上受制于很多因素，并不是所有开发者都能从零构建。显然，他们更需要能够在现有的应用中用 Flutter 添加新的功能，或者分步骤、分阶段将现有的应用程序迁移到 Flutter。
Flutter 的架构非常适合于这样的场景，毕竟每个 Flutter 应用都包含了一个用于托管 Flutter 的 Android 或 iOS 容器。但精益求精方能上一层楼，我们在不断努力改善渐进式接入 Flutter 的体验，包括更新模板、优化工具、完善指南等等。现在，将素材在 Flutter 与宿主代码之间共享变得非常简单了。另一方面，Flutter 的工具现在能让您轻松在不使用调试器启动应用的情况下介入和监测现有的 Flutter 进程。
我们会持续不断地改善这一体验。虽然已经有不少客户已经成功实践了我们针对“应用引入”提供的指南，但绝不会就此止步，我们会一如既往增加更多优秀的案例，并为更加复杂的场景进行延伸。同时，您可以在 Wiki 页面查看关于将 Flutter 引入现有应用的文章，也可以在这个 GitHub 的项目看板上关注我们的进展。
- 关于将 Flutter 引入现有应用的文章:
https://github.com/flutter/flutter/wiki/Add-Flutter-to-existing-apps
- GitHub 的项目看板: 
https://github.com/flutter/flutter/projects/28
平台级视图 (Platform Views)
上面提到的 Add to App 功能非常适合于逐渐引入 Flutter 到现有应用中，但有时候我们反倒需要将 Android 或 iPhone 平台的控件嵌入到 Flutter 应用当中。
我们引入了 AndroidView 和 UiKitView 这两个平台级视图的 widget 到 Flutter，您可以将它们分别嵌入到指定的平台中。我们已经在 Android 平台上预览这一功能数个月了，现在是时候将这一特性推广到 iOS 平台了。此外，像 Google Maps 和 WebView 这样的插件已经可以享受到这一特性带来的种种便利了。
- Google Maps: 
https://github.com/flutter/plugins/tree/master/packages/google_maps_flutter
- WebView:
https://github.com/flutter/plugins/tree/master/packages/webview_flutter
![](http://note.youdao.com/yws/res/109629/2ADC8774203D4268B5489C6B68D92D68)
与其他组件一样，平台级视图 widget 也属于 UI 组合模型的一部分。这意味着您可以将它与其他的 Flutter 组件结合使用。举个例子，上面的截图中，右下角的浮动按钮是一个 50% 透明度的 Flutter widget。这也充分展示了 Flutter 这一独特架构的优势。
当前这一功能已经开放给开发者们尝鲜了，而我们仍需要不断提升性能和改善设备兼容性。所以我们建议您在构建依赖 Platform Views 的应用时要谨慎小心。我们会坚持完善这一特性，并且期待能够在下一次季度更新的时候将这一成果正式发布给所有用户。
不止步于移动平台
就目前而言，Flutter 的首要目标平台是 iOS 和 Android，但我们也在不断探索将 Flutter 拓展到手机端以外的更多平台上。事实上，Flutter 的设计理念就是希望它可以作为一个灵活且便携的 UI 工具包，以适应各种需要绘制屏幕内容的平台。
一些进展已经公布，Flutter Desktop Embedding 就是其中的一个，这是一个使 Flutter 运行于 macOS、Linux 和 Windows 等桌面操作系统的项目。前不久，我们尝试在树莓派平台运行了 Flutter 应用，以非正式和探索的形式向用户展示 Flutter 是有可能运行在一些没有完整桌面环境的小型设备中的。
- Flutter Desktop Embedding: 
https://github.com/google/flutter-desktop-embedding
同时在本周，我们在内部预览了一个还处于实验室开发阶段实验性项目，未来它将极大拓展 Flutter 的运行平台。
![](http://note.youdao.com/yws/res/109630/26329561200A4F97B3719DFAD12FA419)
Hummingbird 是一个基于 Web 实现的 Flutter 运行时环境。它利用了 Dart 语言能被编译成 JavaScript 的特性。这个项目让 Flutter 应用程序能够无需改动地运行在标准 Web 平台。
![](http://note.youdao.com/yws/res/109636/3106ED2915AC4C3E9BBAC65FFC151219)
我们在 Medium 博客上发布了一篇关于 Hummingbird 的技术实现文章，更多的内容会在 2019 年的 I/O 大会分享给大家。
- Medium 博客: 
https://medium.com/flutter-io/hummingbird-building-flutter-for-the-web-e687c2a023a8
当然，移动平台一直都会是我们的首要目标。接下来的几个月，我们会不断公布 Flutter 在这些核心平台上取得的成果。
总结一下
伴随着 Flutter 1.0 正式发布，我们会在当前的 beta、dev 和 master 通道之外新增一个叫 stable 的稳定版通道。它的更新不如其他通道那么频繁，预计将以季度更新的形式将每一阶段的稳定更新收纳进去。
请前往 https://flutter.io 下载 Flutter 1.0 版本。我们的网站还提供了丰富的文档帮您从其他的框架中迁移过来，也提供了 codelabs 帮助您更好理解开发 Flutter 应用的过程。另外我们的 cookbook 收录了常见案例的指南及大量的技术性视频也一定能帮助您认识与了解 Flutter。
我们深知，那些从一开始就伴随我们踏上征程的开发者们是如此令人敬佩。你们向我们提交了大量的反馈建议，帮助找到了诸多问题，产出了大量内容，更和我们一起让 Flutter 项目越来越好。Flutter 社区是我们最宝贵的财富，在这里，我们认识了很多来自不同地区和不同开发生态的积极热情、乐于奉献的你们。你们因为爱而为这个开源项目无私的奉献着，我们真诚地感谢你们！
![](http://note.youdao.com/yws/res/109631/A34F0EDF61A04525B2D272A05034C511)
最后，为 Flutter 吟诗一首：今朝东风拂（Flutter）大地，问君几时扬帆去何方？
