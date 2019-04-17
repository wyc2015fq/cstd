# DeepMind助力谷歌开发的安卓 9「Pie」今日上线 - 知乎
# 



**选自android-developers，作者：Dave Burke，机器之心编译。**

> 今日，谷歌开发者博客宣布，经过 1 年多的开发和数月的测试，安卓 9「Pie」将正式面世。本文中，我们将介绍安卓新版本的所有新特点。

今年 5 月份，谷歌 I/O 大会宣布推出安卓 9，而后经过数月的测试，谷歌收获了大量的反馈。此外，还有小米、Oppo 等 7 家设备制造商也将测试版本放到了他们的旗舰设备上，使得谷歌能够从全球用户那里收集反馈信息。

今日，谷歌宣布将把安卓 9 的源代码放到安卓开源项目上（AOSP），开始在所有的谷歌 Piexl 手机上用安卓 9。据介绍，安卓 9 拥有的机器学习能力能让手机变得更智能、便利、个性化。对开发者而言，安卓 9 包含很多强化 APP 的新方法。

## **Android 9 包含什么？**

**以机器学习为核心的更智能的智能手机**

Android 9 能根据你的喜好自动调整你的手机，让它更适合你。从帮助用户充分利用电池寿命，到任何时刻都能展现 app 中最佳部分，只要用户需要，Android 9 都能让这一切变得更顺畅、更持久。




自适应电池（Adaptive Battery）
![](https://pic2.zhimg.com/v2-302e44545e1375e6e322a97f5daa45f5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='396' height='512'></svg>)
谷歌与 DeepMind 合作开发了个名为 Adaptive Battery 的功能，该功能利用机器学习为用户最关心的应用优化系统资源。如果用户的 app 针对 Doze、App Standby 或 Background Limits 进行了优化，那 Adaptive Battery 可以立即开始运行。




Slice

通过在全屏应用体验之外启用互动，Slice 可以帮助用户更快地执行任务。它通过使用 UI 模板来实现这一点，这些模板从谷歌搜索应用中显示丰富、动态且交互的内容，之后还可以在 Google Assistant 等其他地方显示这些内容。
![](https://pic3.zhimg.com/v2-144c542a61916b86b5ce415ef19b7ed6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='224'></svg>)



App Actions

App Actions 是一种提高 app 可见度和参与度的新方法。Actions 利用机器学习的优势，根据 app 的语义内容和用户语境，在合适时间将特定 app 的特定功能以快捷键的形式呈现给用户。
![](https://pic4.zhimg.com/v2-80ece46ab52286ad4224e25069f95a4b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='389'></svg>)
在接下来的几周，谷歌将分享更多关于注册 app 的细节，这样更多的 app 就可以授权给 App Actions，进而能响应用户需求并在多个谷歌和安卓界面运行。




文本分类与智能链接

谷歌扩展了用于识别内容或文本输入中实体的机器学习模型，已通过 TextClassifier API 支持更多类型，如日期和航班号。Smart Linkify 可以通过让 Linkify API 利用 TextClassifer 模型，提供包括快速文本填充选项等操作。Smart Linkify 同时还可以显著提高检测精度和性能。




Neural Networks 1.1

安卓 9 中包含了神经网络 API 的新版本，以扩展安卓对设备上机器学习加速的支持。具体来说，Neural Networks 1.1 加入了对 9 个新操作的支持——Pad、BatchToSpaceND、SpaceToBatchND、Transpose、Strided Slice、Mean、Div、Sub、Squeeze。通常利用这个 API 的方法是通过 TensorFlow Lite 来进行的。




**最大限度地利用你的手机**

谷歌宣称，新版本的安卓系统可以让你的智能手机变得更加聪明。但更重要的是，随着科技的发展，技术本身将逐渐隐藏在用户界面背后。在安卓 9 中，谷歌的开发者们已将 Android 的 UI 转变为更简单、更易于使用的形式——对于开发人员而言，这些改进有助于改善用户查找、使用和管理 app 的方式。
![](https://pic2.zhimg.com/v2-15ae78963b0b7b32fa7f820537d6d3cd_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='200' height='360'></svg>)新系统导航
Android 9 引入了一个新系统导航，谷歌为此开发了一年多。新的设计使安卓的多任务处理更加简便，并且更容易发现 app。用户在手机上向上滑动，就可以查看最近使用的应用全屏预览，只需点击就可以跳转回其中一个 app。




Display cutout

现在你的 app 可以通过 Android 9 的显示切换功能充分利用最新的无边框屏幕。对于大多数应用来说，系统管理状态栏会将内容和切换功能分开，所以运行切换功能完全无障碍。如果有沉浸式内容，用户可以使用 display cutout APIs 来检查位置和切换形状并发送全屏请求。为了帮助开发和测试，谷歌添加了一个 Developer Option，可以在任何设备上模拟多个 cutout shapes。
![](https://pic1.zhimg.com/v2-635203c998fe104ea25330a9a3364af4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='256'></svg>)带有沉浸式内容的 APP 可以使用显示切换功能在设备上全屏显示
通知与智能回复

Android 9 使通知功能能有用且易操作。通知类 app 可以利用新的 MessagingStyle API 来显示对话、附上照片或标记，甚至还能提供智能回复。用户很快就可以利用 ML Kit 为 app 生成回复建议。
![](https://pic4.zhimg.com/v2-b0a5437400e72a5b3bb0c6e10115804b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='703'></svg>)MessagingStyle 通知与对话和智能回复 [左]，图像和标记 [右]
文本放大
![](https://pic4.zhimg.com/v2-b8a29f1d5decce8b0af08e03d8bf2743_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='285'></svg>)
在 Android 9 中，谷歌添加了一个放大镜部件来提高用户选择文本的体验。放大镜部件允许用户通过可拖动窗口查看放大的文本，从而精确定位光标或者文本选择句柄。用户可将它添加到任何窗口视图中，这样就可以在自定义部件或自定义文本中使用它。除了文本放大，放大镜部件还可以为任何视图提供放大功能。




**用户安全及隐私**

生物特征提示
![](https://pic4.zhimg.com/v2-cc628811a5cda1fde5d976741c88c847_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='509' height='512'></svg>)



随着一系列用于身份验证的生物传感器的使用，不同类型的传感器和应用程序之间的体验变得更加一致。Android 9 引入了一个系统管理的对话框，提示用户输入任何支持的生物认证类型。应用程序不再需要构建自己的对话框，而是使用生物计量学提示 API（BiometricPrompt API）来显示标准系统对话框。除了指纹（包括显示器内传感器），API 还支持面部和虹膜认证。

如果你的应用程序正在绘制自己的指纹验证对话框，你应该尽快切换成使用生物计量学提示 API。




受保护确认

Android 9 引入了 Android 保护确认系统（Android Protected Confirmation），它使用可信执行环境（TEE）来保证给定的提示字符串被显示给用户确认。只有在用户确认成功后，TEE 才会签署提示字符串，然后应用程序可以验证该字符串。




加强对密钥的保护

谷歌添加了 StrongBox 作为一种新的密钥存储类型，为在具有独立 CPU、RAM 和安全闪存的防篡改硬件中提供密钥存储的设备提供 API 支持。你可以设置密钥是否应该由 KeyGenParameterSpec 中的 StrongBox 安全芯片保护。




通过 TLS 的域名系统

Android 9 通过 TLS 增加了对域名系统（DNS）的内置支持，如果网络的 DNS 服务器支持，它会自动将 DNS 查询升级到 TLS。用户可以在网络和互联网设置中以新的私有 DNS 模式通过 TLS 行为管理 DNS。执行自己的 DNS 查询的应用程序可以使用新的 API——link properties . isprivated nsactive() 来检查 DNS 模式。




默认 HTTPS

作为重要工作的一部分，Android 9 将所有网络通信从 cleartext（非加密 HTTP）移动到通过 TLS 加密（HTTPS）的网站，他们将修改默认的网络安全配置以阻止所有 cleartext 通信。用户现在需要通过 TLS 连接，除非明确地选择特定域的 cleartext。




基于编译器安全的 Mitigations 

在 Android 9 中，谷歌已经扩展了编译器级 mitigations 的使用，并通过危险行为的运行时检测来强化平台。控制流完整性（Control Flow Integrity，CFI）技术有助于防止代码复用工具和代码执行。在 Android 9 中，谷歌已经极大地扩展了媒体框架和其它安全组件（如 NFC 和蓝牙）中对 CFI 的使用。在使用 LLVM 构建时，谷歌还在 Android 常用核心中引入了 CFI 核心支持。

此外，谷歌还扩展了整数溢出错误 sanitizers 的使用以缓解内存损坏和信息泄漏漏洞等问题。




用户隐私

Android 9 以多种方式保护隐私。该系统现在限制了麦克风、摄像头和空闲 app 中所有传感器的访问。当 app 的 UID 处于空闲时，麦克风和传感器都会停止报告。应用程序所用的摄像会断开连接，如果应用程序试图使用摄像头，会产生错误。大多数情况下，这些限制不应该为 app 引入新问题，但是还是建议用户从你的 app 中删除这些请求。

Android 9 还允许用户在获得 READ_PHONE_STATE 权限后访问平台的 build.serial 标识符。需要通过 Build.getSerial() 方法获得 build.serial 标识符。




**相机、音频、图像方面的新体验**

多摄像头 API 和其它拍照更新

使用 Android 9，你现在可以在支持多摄像头 API 的设备上同时打开来自两个或更多物理摄像头的流。在配备两个前置摄像头或两个后置摄像头的设备上，你可以实现单个摄像头无法实现的创新功能，例如无缝变焦、散景和立体视觉。API 还允许你调用逻辑或融合的摄像机流，该流可以在两个或多个摄像机之间自动切换。

相机的其它改进还包括新的会话参数（Session parameters）和表面共享（Surface sharing），前者有助于减少初始捕获期间的延迟，而后者允许相机客户端处理各种应用，而无需停止和启动相机流。谷歌还为基于显示的 flash support 添加了 API，并为 app 级图像稳定和特效添加了 OIS 时间戳。




HDR VP9 视频和 HEIF 图像压缩

Android 9 增加了对 HDR VP9 Profile 2 的内置支持，因此你现在可以在支持 HDR 的设备上向用户提供支持 HDR 的电影。

谷歌将 HEIF ( heic ) 图像编码添加到平台中。HEIF 是一种流行的照片格式，它改进了压缩方式以保存存储和网络数据。借助 Android 9 设备上的平台支持，从后端服务器发送和利用 HEIF 图像变得很容易。一旦确定你的应用程序与共享和显示的数据格式兼容，你可以尝试在应用程序中使用 HEIF 作为图像的存储格式。你可以使用 ImageDecoder 或 BitmapFactory 从 jpeg 获得位图来实现从 jpeg 到 heic 的转换，并且可以使用 AndroidX 库中的 HeifWriter 从 YUV 字节缓冲区、表面或位图写入 HEIF 静止图像。




通过动态处理增强音频

动态处理 API 允许你使用一种新的音频效果来隔离特定频率，降低声音或增加柔和声音，以提高 app 的音质。例如，你可以改善在嘈杂环境中说话声音不明显的人的声音。API 还允许你访问多级、多波段动态处理效果，包括预均衡器、多波段压缩器、后均衡器和链接限制器。
![](https://pic1.zhimg.com/v2-05233341993bb9da09e67bb75326d0c8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='225'></svg>)



位图和绘图的图像解码器

图像解码器（ImageDecoder）API 为你提供了一种将图像解码为位图或绘图的更简便方法。你从字节缓冲区、文件或 URI 中创建位图或绘图。与 BitmapFactory 相比，API 提供了几个优势，包括对精确缩放的支持、对物理内存的单步解码、对解码后处理的支持以及对动画图像的解码。




**连接和定位**

用于室内导航的 Wi-Fi RTT
![](https://pic1.zhimg.com/v2-856dfc58bc95c4f1f388c9f4eaa71920_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='246' height='512'></svg>)



Android 9 可以让你通过 IEEE 802.11mc Wi-Fi 协议（即 Wi-Fi Round-Trip-Time，RTT）的平台支持构建室内导航功能到你的 app 上。在具备硬件支持、位置许可和位置启用的 Android 9 设备上，你的 app 可以使用 RTT API 来测量 Wi-Fi 接入点（AP）的距离。设备不需要连接 AP 来使用 RTT，并且为了保护隐私，只有手机可以确定距离，AP 不可以。

如果知道到 3 个或 3 个以上接入点的距离，就可以将设备位置的计算精确到 1 到 2 米。在这种精确度下，设备可以支持建筑物内导航等应用；基于定位的细粒度服务，如消歧语音控制（例如「打开此灯」）；以及基于定位的信息（例如，「这种产品有特别优惠吗？」）。




JobScheduler 的数据成本敏感度

JobScheduler 是安卓的中心服务，用于帮助你管理预定的任务或运行 Doze、App Standby 和 Background Limits。在 Android 9 中，JobScheduler 可以为用户更好地处理网络相关的任务，协调运营商单独提供的网络状态。Jobs 现在可以宣布它们估计的数据大小、信号预读并规定详细的网络需求——运营商可以报告拥堵或未计量网络。JobScheduler 可根据网络状态进行管理。例如，当网络拥堵时，JobScheduler 可暂缓大型网络请求。当未计量时，它可运行预读作业以改善用户体验，例如预读标题。




为 NFC 支付和安全交易开放移动 API

Android 9 添加了 GlobalPlatform Open Mobile API 的一个实现到安卓上。在支持的设备上，app 可以使用 OMAPI API 来访问安全组件（SE），以允许智能卡支付和其它安全服务。硬件抽象层（HAL）提供潜在的 API 以列举可用的安全组件种类（eSE、UICC 和其它）。




**app 的性能**

ART 的性能改善

Android 9 能通过 ART 运行时为所有 app 带来性能和效率的提升。谷歌已经扩展了 ART 的执行文件使用来优化 app 并减少编译 app 代码的内存占用。ART 现在能使用文件信息进行 DEX 文件的设备上重写，并在多种流行的 app 中减少 11% 的内存占用。谷歌期待这些改善能带来系统 DEX 内存使用的减少，以及更快的 app 启动时间。




优化 Kotlin

Kotlin 是安卓系统的第一语言，如果你尚未尝试过，赶紧！谷歌承诺会长期在安卓中使用 Kotlin，并将继续扩展支持，包括优化 Kotlin 代码的性能。在 Android 9 中，你将能看到这项工作的首个结果。我们已经改善了多个编译器的性能，特别是那些目标指向循环语句的，以获得更优的性能。你仅需要保持 Android Studio 的 Kotlin 插件是最新的，就能获取所有最新的 Kotlin 性能提升。

此外，谷歌今天也发布了 Android 9-API 29 SDK(rev. 6) 的更新，其中在某些最频繁使用的 API 中包含空属性标记。




现代安卓

作为 Android 9 项目的一部分，谷歌的目标是现代化安卓的基础建设，以及在其上运行的 app。正如去年谷歌所宣布的，Google Play 将在 2018 年 11 月让所有 app 更新目标 Android Oreo（targetSdkVersion 26 或更高）。据此，如果你的 app 目标指向了早于 Android 4.2(API level 17) 的平台，用户在那之后安装它时将看到一个警告对话。

## **让你的 app 准备好在 Android 9 中使用**
![](https://pic1.zhimg.com/v2-e13e8cd143707eabc5668cc17c1bcd18_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='387' height='512'></svg>)
今天，Android 9 将推送给 Pixel 用户，并且也将在接下来的几个月推送给其它设备。为此，尽快测试你的 app 的兼容性是很重要的。你只需要从 Google Play 中下载 app 或使用模拟器来运行 Android 9。在你进行这个流程时，确保你的 app 可以运行，并运行良好，即确保设备能适应 Android 9 的变化。

同样请注意在 app 中使用非 SDK 界面。Android 9 限制了访问选择的非 SDK 界面，因此你应该减少对它们的依赖。

在你做了必要的更新后，建议立刻发布更新到 Google Play 上，不要改变 app 的平台目标。这能确保你在使用 Android 9 API 和目标来增强 app 时，获得 Android 9 的良好用户体验，

## **用 Android 9 的功能和 API 来增强你的 app**

当你准备好时，就安装 Android 9 并在其中使用新的功能和 API 来做开发。

最开始，你需要下载官方 API 28 SDK、最新的工具和模拟器映像到 Android Studio 3.1 中，或者使用 Android Studio 3.2 的最新版本。然后更新你的项目的 compileSdkVersion 和 targetSdkVersion 到 API 28。当你改变目标时，确保你的 app 支持所有合适的行为变化。

一旦准备好了，发布你的 API 更新到 Google Play 上。一个常用的策略是使用 Google Play 的 beta 测试功能以从一小群用户获取早期反馈，然后给产品做阶段性展示。

## **进入设备之中**

从今天开始，一项 Android 9 的无线更新将开始推送到 Pixel 手机上。并且那些安装了测试版软件的设备，包括索尼、小米、HMD Global、Oppo、Vivo、一加和 Essential，以及所有授权的 Android One 设备，将在今年秋末接收到这项更新。我们也正在与一些伙伴合作在今年将设备更新为 Android 9。

原文链接：[https://android-developers.googleblog.com/2018/08/introducing-android-9-pie.html](https://link.zhihu.com/?target=https%3A//android-developers.googleblog.com/2018/08/introducing-android-9-pie.html)


