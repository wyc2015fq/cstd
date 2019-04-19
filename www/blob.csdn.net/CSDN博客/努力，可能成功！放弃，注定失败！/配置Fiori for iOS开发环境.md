# 配置Fiori for iOS开发环境 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年11月22日 09:11:47[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：60
![](https://upload-images.jianshu.io/upload_images/13213291-c2b1be8f182f4e67.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/660/format/webp)
**写在前面**
本文是Fiori for iOS系列文章的**第三篇**。
SAP和苹果这两个IT大佬，在2017年联合推出了**Fiori for iOS**，让全球的开发者能在iOS平台上，方便地开发SAP企业应用。
如果你不想被潮流甩在身后，就快来看看拥有多年SAP技术经验的郭老师怎么解读Fiori for iOS吧。
**原文作者：**郭之嘉
**翻译校对：**小哈公社
**原文地址**
[https://blogs.sap.com/2017/06/22/set-up-development-environment-for-fiori-for-ios-development/](https://link.zhihu.com/?target=https%3A//blogs.sap.com/2017/06/22/set-up-development-environment-for-fiori-for-ios-development/)
本文由SAP教育培训与小哈公社共同呈现。
**Fiori for iOS系列**
我将撰写一系列的针对Fiori for iOS的文章。希望能帮助你使用SAP提供的控件来开发iOS apps，当然，用的是Swift语言。
**第一部分：概述**
Fiori for iOS介绍
Fiori for iOS控件概述
**第二部分：一步步教你使用Fiori控件**
设置Fiori for iOS开发环境（本文）
2.1 使用FUIFormCell系列控件，创建输入表单
开始使用FUIFormCell控件和它的子类
创建复杂的输入表单
让你的代码具备重用性
小结
**第三部分：使用Fiori for iOS控件，实现Fiori平面图（floor plans）**
**第四部分：定制化主题**
在上一篇《[Fiori for iOS控件概述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzODkzMjg0NA%3D%3D%26mid%3D2247483925%26idx%3D1%26sn%3Dfd5ff4880ea792b64ab90639f5910e98%26chksm%3De9308416de470d002e40cee5ffe7eb26bf156b6b04db1c82e3384be32f4d45baba3a019af996%26scene%3D21%23wechat_redirect)》中，我总结了Fiori for iOS控件的优点，从现在开始，你可以使用它们了。
开始使用这些控件之前，需要知道如何配置你的开发环境和项目。
已经有很多很棒的教程帮助你配置SDK，并连接到SAP Cloud Platform，下面是链接：
[https://www.sap.com/developer/groups/ios-sdk-setup.html](https://link.zhihu.com/?target=https%3A//www.sap.com/developer/groups/ios-sdk-setup.html)
但是以上这篇教程更偏重于如何连接到Cloud Platform，而编码部分则通过使用assistant app来实现。这种方式对于实际工作可能更有效，但是并不利于学习，所以我在本文中重写了这个教程来帮助你配置所需的开发环境。
**1. 下载SDK**
第一步是下载SDK。
你可以通过以下链接获得试用版的SDK：
[https://store.sap.com/sap/cpa/ui/resources/store/html/SolutionDetails.html?pid=0000014485](https://link.zhihu.com/?target=https%3A//store.sap.com/sap/cpa/ui/resources/store/html/SolutionDetails.html%3Fpid%3D0000014485)
在点击金色的“Trial Version”按钮之后，你需要填写一张表单来申请试用版。
几个小时后，你将收到一封带有SDK下载链接的邮件。
![](https://upload-images.jianshu.io/upload_images/13213291-953c8aca3c49e98d.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
你需要在48小时内点击链接，你将进入一个带有不同版本的SDK的下载页面。记住选择最后一行，确保你下载到的是最新版本的SDK。
**2. 安装**
解压你所下载的文件，你会得到一个 .dmg文件。
![](https://upload-images.jianshu.io/upload_images/13213291-a38aac297dd7f474.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/390/format/webp)
打开dmg文件包，把SAP CP SDK for iOS拖拽到Applications中。
![](https://upload-images.jianshu.io/upload_images/13213291-24a4e67ee4c11bf3.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
打开Finder，点击Application，你会找到SAP CP SDK for iOS对应目录，打开Frameworks->Release-fat，你会看到一些framework文件。这些文件是测试用的framework models。“fat” release包括了运行在iOS模拟器，iPhone和iPad上的所有代码，这便于测试。当你把自己的app发布出去的时候，你需要使用不同平台的不同framework文件。
![](https://upload-images.jianshu.io/upload_images/13213291-1af1fbdd531f9b88.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
**3. 创建一个新的Xcode项目**
打开你的Xcode，选择“Create a New Xcode project”。
![](https://upload-images.jianshu.io/upload_images/13213291-610fd8214bcd1463.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
选择“Single View Application”并点击“Create”。
![](https://upload-images.jianshu.io/upload_images/13213291-3595aac9d61d861a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
在Product Name字段，输入你的项目的名称。
在Team字段，选择None，如果你希望在自己的iPhone，而不是在Mac上的模拟器上来测试app，你需要有一个Team。如果你还没有加入Apple Developer Program，你可以申请一个临时的team认证。
你可以参考以下链接来绑定你的Apple ID到Xcode，并注册你的个人team。
[https://developer.apple.com/library/content/documentation/IDEs/Conceptual/AppStoreDistributionTutorial/AddingYourAccounttoXcode/AddingYourAccounttoXcode.html](https://link.zhihu.com/?target=https%3A//developer.apple.com/library/content/documentation/IDEs/Conceptual/AppStoreDistributionTutorial/AddingYourAccounttoXcode/AddingYourAccounttoXcode.html)
在Organization Name字段，填入你所在的组织。
在Organization Identifier中，你必须输入一些类似namespace的内容，例如：com.sap，com.yourcompanyname。
保持其他字段为默认，点击Next。
![](https://upload-images.jianshu.io/upload_images/13213291-8305d86bab4a2a50.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
选择你希望保存文件的目录，例如 “Documents”，然后点击“Create”。
![](https://upload-images.jianshu.io/upload_images/13213291-78fdab600fba16b7.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
恭喜，你已经创建了一个Xcode项目。下一步，就要到SAP的部分了。
**4. 导入SAP Cloud Platform SDK for iOS并测试**
忽略你项目第一页中的错误信息。向下滚动，找到“Embedded Binaries”部分。点击左下角的“+”按钮。
![](https://upload-images.jianshu.io/upload_images/13213291-87428db0dd2941a1.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
在弹出窗口中点击“Add Other…”。
![](https://upload-images.jianshu.io/upload_images/13213291-38e9237b6760a515.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
找到目录Application/SAP CP SDK for iOS/Frameworks/Release-fat，按住Shift点击SAP Fiori.framework和SAPFoundation.framework以选择全部的五个.framework文件，点击“Open”。
![](https://upload-images.jianshu.io/upload_images/13213291-4921ab3c8ac5f87f.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
在弹出窗口中，别忘记勾选“Copy items if needed”，并选择“Create groups”。
点击Finish。
![](https://upload-images.jianshu.io/upload_images/13213291-29f76f4e6593d639.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
大概10秒钟后，你会发现这些frameworks出现在Xcode窗口的左上角。
![](https://upload-images.jianshu.io/upload_images/13213291-33f36bd53ec0269a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/522/format/webp)
点击ViewController.swift，Xcode的中心区域会变为代码编辑器。
在第一行尝试输入：import SAP
如果来自SAP的一些modules会自动被系统提示出来，说明你的SAP Cloud Platform SDK for iOS安装成功了！
![](https://upload-images.jianshu.io/upload_images/13213291-473726334a04e3f6.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
**5. 小结**
通过本文，你已经知道了如何安装，创建和为Xcode项目配置Fiori for iOS。这是之后所有文章内容的前提。
在下一篇中，我将带你学习一组非常有用的Fiori控件：FUIFormCell系列。使用这组控件，你可以创建复杂的表单，来显示和输入业务内容。
[原文链接点这里](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzODkzMjg0NA%3D%3D%26mid%3D2247483957%26idx%3D1%26sn%3D5041378ae8b75dcefbfdc72313840931%26chksm%3De9308436de470d209081815ac4ac6d2ff7a97c52f2907d61dce50e7d0e28889356c7a0ffb8c4%23rd)
作者：小哈公社
链接：https://www.jianshu.com/p/5c12bb3762dc
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
