# iOS超全开源框架、项目和学习资料汇总（4）数据库、缓存处理、图像浏览、摄像照相视频音频篇 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月25日 09:19:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：483
> 
系统的整理iOS开发常用的第三方库、学习资源等，如果有更好的三方库、没有整理到、可以在评论留言，我会及时更新的，还不赶快mark～
**[iOS超全开源框架、项目和学习资料汇总（1）UI篇](http://www.jianshu.com/p/01b98f69bd66)[iOS超全开源框架、项目和学习资料汇总（2）动画篇](http://www.jianshu.com/p/8ba2751e925b)[iOS超全开源框架、项目和学习资料汇总（3）网络和Model篇](http://www.jianshu.com/p/c59698c249e5)**
#### 数据库
**1. [FMDB](https://github.com/ccgus/fmdb)**– sqlite的工具， [多线程FMDatabaseQueue实例](https://github.com/tangqiaoboy/FmdbSample)，[FMDB数据库的使用演示和封装工具类](https://github.com/liuchunlao/LVDatabaseDemo)。[GXDatabaseUtils](https://github.com/Gerry1218/GXDatabaseUtils) – 在FMDB基础上的工具。
**2. [realm-cocoa](https://github.com/realm/realm-cocoa)**– Realm是一个真正为移动设备打造的数据库，同时支持Objective-C和Swfit。Realm宣称其相比Sqlite，在移动设备上有着更好的性能表现。
**3. [Breeze](https://github.com/andrelind/Breeze)**– 用Swift写的一个轻量级的CoreData管理工具，并且还支持iCloud 。
**4. [AlecrimCoreData](https://github.com/Alecrim/AlecrimCoreData)**– Swift，更容易地访问 CoreData 对象封装类库。除了 CRUD，还提供指针定位，强大的排序、筛选，异步数据获取，以及独立线程后台存取数据。
**5. [CoreStore](https://github.com/JohnEstropia/CoreStore)** – Core Data 管理类库。 其中事务管理及查询是其比较大的亮点，整套 API 功能完整。
**6. [Presentation](https://github.com/hyperoslo/Presentation)**– 重量级好项目 Presentation，它可以方便你制作定制的动画式教程、Release Notes、个性化演讲稿等。
**7. [CoreValue](https://github.com/terhechte/CoreValue)** – Swift 2 版 Core Data 封装库。相比另外两个 [SwiftRecord](https://github.com/arkverse/SwiftRecord)和 [CoreStore](https://github.com/JohnEstropia/CoreStore)更轻量。
#### 缓存处理
**1. [YTKKeyValueStore](https://github.com/yuantiku/YTKKeyValueStore)** – Key-Value存储工具类，[说明](http://tangqiaoboy.gitcafe.io/blog/2014/10/03/opensouce-a-key-value-storage-tool/)。
**2. [TMCache](https://github.com/tumblr/TMCache)**– TMCache 是 Tumblr 开源的一个基于 key/value 的数据缓存类库,可以用于缓存一些临时数据或者需要频繁加载的数据,比如某些下载的数据或者一些临时处理结果。
**3. [JLKeychain](https://github.com/jl322137/JLKeychain)**– 快捷使用keychain存储数据的类，使keychain像NSUserDefaults一样工作。
**4. [sskeychain](https://github.com/soffes/sskeychain)**– SSKeyChains对苹果安全框架API进行了简单封装,支持对存储在钥匙串中密码、账户进行访问,包括读取、删除和设置。
**5. [KeychainAccess](https://github.com/kishikawakatsumi/KeychainAccess)**– 管理Keychain接入的小助手。
#### 图像浏览及处理
**1. [FLAnimatedImage](https://github.com/liric28/FLAnimatedImage)** – gif播放处理的工具。
**2. [CLImageEditor](https://github.com/yackle/CLImageEditor)** – 超强的图片编辑库，快速帮你实现旋转，防缩，滤镜等等一系列麻烦的事情。
**3. [VIPhotoView](https://github.com/vitoziv/VIPhotoView)**– 用于展示图片的工具类，因为是个 View，所以你可以放在任何地方显示。支持旋转，双击指定位置放大等。
**4. [MWPhotoBrowser](https://github.com/mwaterfall/MWPhotoBrowser)**– 一个非常不错的照片浏览器，在github的star接近3000个，[解决MWPhotoBrowser中的SDWebImage加载大图导致的内存警告问题](http://www.superqq.com/blog/2015/01/22/jie-jue-mwphotobrowserzhong-de-sdwebimagejia-zai-da-tu-dao-zhi-de-nei-cun-jing-gao-wen-ti/)。
**5. [core-image-explorer](https://github.com/objcio/issue-21-core-image-explorer)** – Core Image 滤镜处理图片– swift ，[Core Image 介绍](http://objccn.io/issue-21-6/)。
**6. [CoreImageShop](https://github.com/rFlex/CoreImageShop)**– CoreImageShop图片滤镜处理– Mac app that let you create a complete Core Image Filter usable on iOS using SCRecorder。
**7. [GPUImage](https://github.com/BradLarson/GPUImage)** – 处理图片效果。
**8. [RSKImageCropper](https://github.com/ruslanskorb/RSKImageCropper)** – 适用于iOS的图片裁剪器，类似Contacts app，可上下左右移动图片选取最合适的区域。
**9. [WZRecyclePhotoStackView](http://code.cocoachina.com/detail/232156)**– 删除照片交互–WZRecyclePhotoStackView，就是模拟生活中是删除或保留犹豫不决的情形而产生的。 在上滑，下滑的部分，借鉴了[TinderSimpleSwipeCards](https://github.com/cwRichardKim/TinderSimpleSwipeCards)。
**10. [TimingFunctionEditor](https://github.com/schwa/TimingFunctionEditor)**– TimingFunctionEditor用swift编写， 贝塞尔曲线编辑器，编辑后可以预览或拷贝代码片段直接使用。P.S. 该项目采用更简单的依赖管理器
[Carthage](https://github.com/Carthage/Carthage) ，而非常用的 CocoaPods。[Carthage介绍中文](http://www.cocoachina.com/ios/20141204/10528.html)。
**11. [AAFaceDetection](https://github.com/aaronabentheuer/AAFaceDetection)**– AAFaceDetection–swift，简单、实用的面部识别封装库。虽然该技术从 iOS 5 发展，不过真正有趣的应用还不多。。
[PhotoTweaks](https://github.com/itouch2/PhotoTweaks) – 这个库挺赞的，正好是对图像操作的。
**12. [Concorde](https://github.com/contentful-labs/Concorde)** – swift, Concorde, 一个可用于下载和解码渐进式 JPEG 的库, 可用来改善应用的用户体验。
**13. [ZoomTransition](https://github.com/tristanhimmelman/ZoomTransition)**– swift, 通过手势操控图片的放大、缩小、旋转等自由变化效果的组件及示例。
**14. [AFImageHelper](https://github.com/melvitax/AFImageHelper)**– swift,一套针对 UIImage 和 UIImageView 的实用扩展库，功能包含填色和渐变、裁剪、缩放以及具有缓存机制的在线图片获取。
**15. [PinterestSwift](https://github.com/demon1105/PinterestSwift) –** swift,Pinterest 风格图片缩放、切换示例。
**16. [KYElegantPhotoGallery](https://github.com/KittenYang/KYElegantPhotoGallery)**– 一个优雅的图片浏览库。
**17. [SDCycleScrollView](https://github.com/gsdios/SDCycleScrollView)**– 无限循环自动图片轮播器(一步设置即可使用)。
**18. [SDPhotoBrowser](https://github.com/gsdios/SDPhotoBrowser)**– 仿新浪动感图片浏览器,非常简单易用的图片浏览器，模仿微博图片浏览器动感效果，综合了图片展示和存储等多项功能。
**19. [HZPhotoBrowser](https://github.com/chennyhuang/HZPhotoBrowser)** – 一个类似于新浪微博图片浏览器的框架（支持显示和隐藏动画；支持双击缩放，手势放大缩小；支持图片存储；支持网络加载gif图片，长图滚动浏览；支持横竖屏显示）。
**20. [PhotoStackView-Swift](https://github.com/YiZhuoChen/PhotoStackView-Swift)** – PhotoStackView——照片叠放视图，[使用说明](http://blog.csdn.net/u013604612/article/details/46336657)。
**21. [MarkingMenu](https://github.com/FlexMonkey/MarkingMenu)**– 基于手势、类似 Autodesk Maya 风格标记菜单及图片渲染。
**22. [SXPhotoShow](https://github.com/dsxNiubility/SXPhotoShow)**– UICollectionViewFlowLayout流水布局 是当下collectionView中常用且普通的布局方式。本代码也写了三种好看的布局，其中LineLayout和流水布局有很大的相同点就直接继承UICollectionViewFlowLayout，然后StackLayout，CircleLayout这两种都是直接继承自最原始的UICollectionViewLayout 布局方案。
#### 摄像照相视频音频处理
**1. [ZFPlayer](https://github.com/renzifeng/ZFPlayer)** - 基于AVPlayer，支持竖屏、横屏（横屏可锁定屏幕方向），上下滑动调节音量、屏幕亮度，左右滑动调节播放进度[ZFPlayer剖析](http://www.jianshu.com/p/5566077bb25f)
**2. [SCRecorder](https://github.com/rFlex/SCRecorder)** – SCRecorder 短视频录制。
**3. [VideoPushDemo](https://github.com/pingguo-zangqilong/VideoPushDemo)** – 视频剪辑 [视频特效制作1](http://www.jianshu.com/p/3006502912aa)[视频特效制作2](http://www.jianshu.com/p/6313025349a9)。
**4. [LLSimpleCamera](https://github.com/omergul123/LLSimpleCamera)** – A simple, customizable camera control for iOS， 摄像头。
**5. [EZAudio](https://github.com/syedhali/EZAudio)**– EZAudio 是一个 iOS 和 OSX 上简单易用的音频框架，根据音量实时显示波形图，基于Core Audio，适合实时低延迟音频处理，非常直观。[中文介绍](http://segmentfault.com/blog/news/1190000000370957),[官网](http://www.syedharisali.com/about)。
**6. [ffmpeg](http://ffmpeg.org/)**– ffmpeg官网，[FFmpeg在iOS上完美编译](http://www.cocoachina.com/ios/20150514/11827.html)。
**7. [VCL](http://www.videolan.org/)** – VCL官网。
**8. [kxmovie](https://github.com/kolyvan/kxmovie)**– 使用ffmpeg的影片播放器，[修改说明](http://www.cocoachina.com/bbs/read.php?tid=145575)，
[修改代码](https://github.com/kinglonghuang)。
**9. [ijkplayer](https://github.com/Bilibili/ijkplayer)**– B站开源的视频播放器，支持Android和iOS。
**10. [StreamingKit](https://github.com/tumtumtum/StreamingKit)**– StreamingKit流媒体音乐播放器。
**11. [FreeStreamer](https://github.com/muhku/FreeStreamer)**– FreeStreamer流媒体音乐播放器，cpu占用非常小。
**12. [DOUAudioStreamer](https://github.com/douban/DOUAudioStreamer)** – DOUAudioStreamer豆瓣的音乐流媒体播放器。
**13. [fmpro](https://github.com/fmpro/fmpro)**– 电台播放器，支持锁屏歌词，支持基本播放流程，歌词展示，后台锁屏播放和控制以及锁屏后封面+歌词，[fmpro_R](https://github.com/jovisayhehe/fmpro_R) 。
**14. [IPDFCameraViewController](https://github.com/mmackh/IPDFCameraViewController)** – 支持相机定焦拍摄、滤镜、闪光、实时边框检测以及透视矫正功能，并有简单易用的API。
**15. [SCRecorder](https://github.com/rFlex/SCRecorder)** – 酷似 Instagram/Vine 的音频/视频摄像记录器，以 Objective-C 为基础的过滤器框架。 你可以做很多如下的操作：记录多个视频录像片段。删除任何你不想要的记录段。可以使用任何视频播放器播放片段。保存的记录可以在序列化的 NSDictionary 中使用。（在 NSUserDefaults 的中操作）添加使用 Core Image 的视频滤波器。可自由选择你需要的 parameters 合并和导出视频。
**16. [Cool-iOS-Camera](https://github.com/GabrielAlva/Cool-iOS-Camera)**– Cool-iOS-Camera。
**17. [FastttCamera](https://github.com/IFTTT/FastttCamera)**– FastttCamera 快速照相。
**18. [ICGVideoTrimmer](https://github.com/itsmeichigo/ICGVideoTrimmer)**– ICGVideoTrimmer提供提供视频剪切的视图（类似系统相册中浏览视频时顶部那个条状视图）。左右两个边界选择器还能够自定义。
**19. [IOS录音和播放功能demo](http://d.cocoachina.com/code/detail/285717)**– 比较完整的ios录音和播放功能的实现。
**20. [CameraManager](https://github.com/imaginary-cloud/CameraManager)** – 相机管理封装类库。看着极好用的样子—-swift。
**21. [MCAudioInputQueue](https://github.com/msching/MCAudioInputQueue)** – 简易录音类，基于AudioQueue的。
**22. [DraggableYoutubeFloatingVideo](https://github.com/vizllx/DraggableYoutubeFloatingVideo)**– 展示像类似Youtube移动应用的那种浏览视频的效果，当点击某视频时能够从右下方弹出一个界面，并且该界面能够通过手势，再次收缩在右下方并继续播放。这是通过AutoLayout设计实现。
**23. [amr](http://www.penguin.cz/~utx/amr)**– 做即时通讯的音频处理，录音文件是m4a，便于web端的音频播放。
**24. [FSVoiceBubble](https://github.com/f33chobits/FSVoiceBubble)**– 一个轻量级播放录音音频的气泡：1.支持短时间的音频播放（支持网络音频）；2.播放时的声波动画；3.自定义包括声波的颜色，气泡的背景等。
**25. [KRVideoPlayer](https://github.com/36Kr-Mobile/KRVideoPlayer)**– 类似Weico的播放器，支持竖屏模式下全屏播放。
#### 其他
[DateTimeKit](https://github.com/exsortis/DateTimeKit) – 一个超赞的时间处理的库，Joda-Time ！ 他能帮你轻松处理时区，处理时间加减，计算到期时间等等场景下的问题。
[SwiftDate](https://github.com/malcommac/SwiftDate) – 特别完整、强大的日期时间操作管理类库。它几乎涵盖了已知开源日期类库所有优秀特性。 他能帮你轻松处理时区，处理时间加减，计算到期时间等等场景下的问题。
[iOS私有API](https://github.com/nst/iOS-Runtime-Headers) – 私有API，绿色 == public，红色 == private，蓝色 == dylib。
[iOS源代码](http://opensource.apple.com/source/CF/) – iOS源代码。
[libfacedetection](https://github.com/ShiqiYu/libfacedetection) – C++ 人脸识别 包含正面和多视角人脸检测两个算法.优点:速度快(OpenCV haar+adaboost的2-3倍), 准确度高 (FDDB非公开类评测排名第二），能估计人脸角度。
[Slidden](https://github.com/Brimizer/Slidden) – 一个老外开源的开发自定义键盘的库，利用这个开源库，可以方便的配置键位、颜色以及键位对应的图片。
[TPKeyboardAvoiding](https://github.com/michaeltyson/TPKeyboardAvoiding) – 用户键盘弹出自动计算高度，进行屏幕滚动操作。
[CDPMonitorKeyboard](http://d.cocoachina.com/code/detail/298267) – CDPMonitorKeyboard封装,可以解决输入视图(例如textField,textView等)被键盘覆盖问题，并可设置高于键盘多少。
[自动监听键盘高度](http://code.cocoachina.com/detail/297973/%E8%87%AA%E5%8A%A8%E7%9B%91%E5%90%AC%E9%94%AE%E7%9B%98%E9%AB%98%E5%BA%A6/) – 自动监听键盘高度，初始界面，输入框在屏幕最下方，当键盘出现时，输入框随即移动到键盘上方。
[SYKeyboardTextField](https://github.com/441088327/SYKeyboardTextField) – SYKeyboardTextField 是一个轻巧,简单,非侵入式的键盘附随输入框! 采用Swift编写。
[BlocksKit](https://github.com/zwaldowski/BlocksKit) – block框架，为 OC 常用类提供了强大的 Block 语法支持，使得编写 OC 代码变得舒适、快速、优雅。
[KVOController](https://github.com/facebook/KVOController) – 在项目中有使用 KVO ，那么 KVOController 绝对是个好选择。它是 facebook 开源的一个 KVO 增强框架。
[appirater](https://github.com/arashpayan/appirater) – 用于提醒用户给你的 APP 打分的工具。
[MotionKitr](https://github.com/MHaroonBaig/MotionKitr) – 为核心运动框架（The Core Motion framework）提供友好的类库封装，以更方便使用三轴陀螺仪和加速感应器特性。
[Review Monitor](https://launchkit.io/reviews/) – 第一时间自动推送 Apple Store 的用户评论到你的邮件箱或者 Slack，第一时间跟进用户反馈，打造优秀 App 必备工具！类似的有：App annie 的类似功能。
[WBWebViewConsole](https://github.com/Naituw/WBWebViewConsole) – 类似微博iPhone客户端的 “调试选项” 吗？把其中的 “内置浏览器网页调试” 开源在 Github 上了。
[ios-good-practices](https://github.com/futurice/ios-good-practices) – ios-good-practices iOS 开发最佳实践。
[iOS开发最佳实践](http://ios.jobbole.com/81830/) – iOS 开发最佳实践 — 中文。
[TodayExtensionSharingDefaults](http://code.cocoachina.com/detail/232160) – TodayExtensionSharingDefaults是一个iOS 8 Today扩展示例，可以使用NSUserDefaults与其containing app分享数据。
[QRCodeReader.swift](https://github.com/yannickl/QRCodeReader.swift) – QRCodeReader.swift一款简单的 QR 二维码阅读组件及示例，提供前后相机切换功能。
[QRCatcher](https://github.com/100mango/QRCatcher) – 一个简洁美观的二维码扫描应用，
[iOS学习：AVFoundation 视频流处理–二维码扫描](https://github.com/100mango/zen/blob/master/iOS%E5%AD%A6%E4%B9%A0%EF%BC%9AAVFoundation%20%E8%A7%86%E9%A2%91%E6%B5%81%E5%A4%84%E7%90%86/iOS%E5%AD%A6%E4%B9%A0%EF%BC%9AAVFoundation%20%E8%A7%86%E9%A2%91%E6%B5%81%E5%A4%84%E7%90%86%20.md)。
[Parsimmon](https://github.com/ayanonagon/Parsimmon) – swift，小而美的语言学类库封装工具包。提供分词、标记词性、词形归并、朴素贝页斯分类、决策树等自然语言分析小工具。P.S. 英语分词效果好于中文，感兴趣的同学可以针对中文做一些优化开发。参考译文 NSHipster –
[NSLinguistic?Tagger](http://nshipster.cn/nslinguistictagger/)。
[Password-keyboard](https://github.com/liuchunlao/Password-keyboard) – 随机变换数字位置的密码键盘。 模仿银行类应用在付款时输入的随机密码键盘。
[MKMapView-Extension](https://github.com/SemperIdem/MKMapView-Extension) – 这是关于 MKMapView 写的一个基于swift的扩展，可以扩展 MKMapView 的相关功能，减少复用代码量。
[SemverKit](https://github.com/nomothetis/SemverKit) – 针对符合『语义化版本规范 2.0.0』版本号的解析、比较运算类库。不仅支持 Major, Minor, Patch，还支持 Alpha 和 Beta 预发布版本，以及相应地递增运算扩展。
[SwiftValidator](https://github.com/jpotts18/SwiftValidator) – 基于规则的输入验证类库。项目良好的面向对象设计思想，使规则的扩展及自定义非常方便。更专业的规则引擎（甚至是基于自然语言的规则配置）解决方案，比如：开源的 Drools，商用的 ILOG 等。
[Tesseract-OCR-iOS](https://github.com/gali8/Tesseract-OCR-iOS) – 有关OCR文字识别项目。
[iOS-Categories](https://github.com/shaojiankui/IOS-Categories) – 收集了许多有助于开发的iOS扩展,各种category。
[Olla4iOS](https://github.com/nonstriater/Olla4iOS) – 过去积累的一些方便复用的类和方法，还在整理中。
[DKNightVersion](https://github.com/Draveness/DKNightVersion) – 用最快的方式给你的应用加上夜间和白天的切换效果。
[TouchVisualizer](https://github.com/morizotter/TouchVisualizer) – 实用的多点触摸可视化组件。扩展并作用于 UIWindows，结构上提供了简单地针对触摸显示定制，比如触摸点的颜色。
[RegexKitLite](https://github.com/wezm/RegexKitLite) – 用来处理正则表达式。
[XcodeServerSDK](https://github.com/czechboy0/XcodeServerSDK) – 非官方 Xcode Server SDK 封装库。 P.S. 该 SDK 分离自之前推荐的由该作者开发的自动测试框架
[Buildasaur](https://github.com/czechboy0/Buildasaur)。
[BFKit-Swift](https://github.com/FabrizioBrancati/BFKit-Swift) – BFKit-Swift 这套工具库可以提高应用开发效率。
[CKSIncrementalStore](https://github.com/CloudKitSpace/CKSIncrementalStore) – 基于 CloudKit 服务器实现多终端数据同步。
[SwiftSequence](https://github.com/oisdk/SwiftSequence) – 简洁、灵活、多变的操作 SequenceType 的类库（基于微框架（μframework）设计思想）。
[IDNFeedParser](https://github.com/photondragon/IDNFeedParser) – 一个简单易用的Rss解析库。
[CoreUmeng](https://github.com/nsdictionary/CoreUmeng) – 简单：友盟分享封装。
[openshare](https://github.com/100apps/openshare) – 不用官方SDK，利用社交软件移动客户端(微信/QQ/微博/人人/支付宝)分享/登录/支付。
[Swifternalization](https://github.com/tomkowz/Swifternalization) – 一套实用的本地化工具库。使用教程及 API 文档完整。值得收入项目的“轮子”。
[apous](https://github.com/owensd/apous) – 一款有趣的 Swift 应用 － 让 Swift 成为脚本语言。
[Mirror](https://github.com/kostiakoval/Mirror) – 通过反射（Refection）实现镜像对象封装库。从而可以更轻松获取（或输出）对象属性名、类型及值变量。
[Proposer](https://github.com/nixzhu/Proposer) – Proposer 用单个 API 处理 iOS 上的权限请求，以便使用前确认可访问“相册”、“相机”、“麦克风”、“通讯录”或“用户位置”。
[LocationManager](https://github.com/intuit/LocationManager) – CoreLocation使用起来还是比较麻烦的，需要授权，判断系统版本等等，所以推荐使用第三方框架LocationManager，使用Block，十分简单！[iOS-CoreLocation：无论你在哪里，我都要找到你！](http://www.cocoachina.com/ios/20150721/12611.html)
 。
[pangu.objective-c](https://github.com/Cee/pangu.objective-c) – 有多种语言实现版本～ Pangu.Objective-C：格式化中英文之间的空格（OC）。
[objection](https://github.com/atomicobject/objection) – 一个轻量级的依赖注入框架Objection。
文／iOS巍（简书作者）
原文链接：http://www.jianshu.com/p/73bdbf23a605
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
