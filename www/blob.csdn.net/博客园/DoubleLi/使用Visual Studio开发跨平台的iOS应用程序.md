# 使用Visual Studio开发跨平台的iOS应用程序 - DoubleLi - 博客园






[原文发表地址][Developing cross-platform iOS application using Visual Studio](http://blogs.msdn.com/b/vcblog/archive/2015/06/04/developing-cross-platform-ios-applications-using-visual-studio.aspx)

[原文发表时间]2015/6/4

C ++是一种流行的高级编程语言，可运行在iOS，Android或Windows等所有的移动设备上。有许多因素推动了在这些移动的平台使用C ++，从代码重用到运行时的性能。许多平台定义的应用程序可在这些平台运行如Office，Skype， [Dropbox已](http://translate.googleusercontent.com/translate_c?depth=1&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&tl=zh-CN&u=http://oleb.net/blog/2014/05/how-dropbox-uses-cplusplus-cross-platform-development/&usg=ALkJrhhFwTd6h9KN-sord_eK6JzVSHmAHw)架构他们的移动解决方案，在某种程度上促进或利用C ++作 为 他们绝大多数的的代码库。这不仅为他们提供了代码重用的优势并且降低了开发成本，而且作为C ++代码直接在硬件上运行， 它具有高速流，快速响应和高性能的体验，这也是他们的最终用户所期望的。然而，在所有这些设备平台上使用C ++这个设想不是那么的乐观， 因为在今天，用户为了实现这一目标需要使用多个IDE，各种工具链和构建系统令人担忧。从[VS2015 RC](https://translate.googleusercontent.com/translate_c?depth=1&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&tl=zh-CN&u=https://www.visualstudio.com/en-us/downloads/visual-studio-2015-downloads-vs.aspx&usg=ALkJrhgLToikPBKKrw7BqG4obZMEdqKV_A)开始，开发人员可以使用Visual Studio作为单一的C ++ IDE满足他们所有的跨平台的需求。这个博客专门地谈论了新推出的 使用Visual Studio开发iOS应用程序的体验。让我们开始吧 。

Getting Started入门

我们当前的iOS开发体验要求在Mac计算机上，这主要是因为法律而非技术的原因。因此，在我们开始之前，不仅需要下载微软的资源（项目和构建二进制文件，模板），而且在Mac上的部件需要安装，你可以跟着[这里](https://translate.googleusercontent.com/translate_c?depth=1&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&tl=zh-CN&u=https://msdn.microsoft.com/library/dn707598(v%3Dvs.140).aspx&usg=ALkJrhgkxSNxCzOKqE_ihnFclGgWibPRFQ#ConfigureVS)的向导去设置。

在开发人员使用Visual Studio 做iOS开发之前， 第一步是配对Mac机与运行Visual Studio的实体机。配对可以通过在工具 - >选项菜单中的远程代理进行配置。确保VCRemote agent是在Mac上运行的，然后输入详细信息，如主机名和端口字段等所有的设置。成功配对的最后一件事是设置远程根文件夹，它指向与您机器配对的Mac的VCRemote的位置。配对通过点击“配对”按钮启动，一旦配对我们 就可以用Visual Studio开发我们的第一个iOS应用程序。

![](https://msdnshared.blob.core.windows.net/media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/00/65/69/6242.vcremote.png)

![](https://msdnshared.blob.core.windows.net/media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/00/65/69/8712.FileNew.jpg)

The cross-platform 'iOS' templates跨平台“的iOS'模板

打开新建工程向导，我们提供的iOS跨平台的模板在Visual C ++\Cross-platform下面。我们允许建立完整的端到端的iOS应用程序与C ++静态和共享库。各种可用的模板如下图所示。在这篇博客中，我们将演示如何使用面向iOS和 Android平台的 OpenGLES应用程序。

![](https://msdnshared.blob.core.windows.net/media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/00/65/69/7673.templates.png)

下图展示了跨平台的OpenGLES应用程序。OpenGLESApp库文件夹包含了Android native-activity项目和iOS的静态库项目。OpenGLESApp.Shared工程包含用于构建Android的动态共享库和iOS的静态库的共享的OpenGL代码。这些库会在OpenGLESApp.Android.packaging项目和OpenGLESApp.iOS.Application项目中被调用去生成Andr​​oid端到端.apk文件和iOS的.ipa文件。

![](https://msdnshared.blob.core.windows.net/media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/00/65/69/7624.start.png)

![](https://msdnshared.blob.core.windows.net/media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/00/65/69/4670.code-nav.png)

设置和编辑我的iOS代码

现在，让我们带来OpenGLESApp.shared项目组件的一部分的Cube.c跨平台源文件。作为我们即时体验的一部分，我们提供了的iOS特殊的智能感知，重构和许多的代码定位功能，如代码查看，转到定义。其中的一些功能描述如下图

![](https://msdnshared.blob.core.windows.net/media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/00/65/69/5756.intellisense.png)

现在，如果你选择了OpenGLESApp.iOS.StaticLibrary工程环境，你会看到“Cube.c”源代码饰有波浪线，所以让我们先解决这个问题。该波浪线的出现是由于在Windows机器上VS智能感知无法获取OpenGLES头文件。我们目前不集成OpenGLES1头文件在VS直接使用。为了解决这个问题可以从[这里](https://www.khronos.org/registry/gles/)下载可公开的ES1头文件，并复制它们到你的VS文件夹里。一旦这个路径被列入“包含目录”就完全设置好了。 
以我们目前的经验，一般来说如果开发者想要增加额外不属于VS项目一部分的include头文件，需要确保这些头文件都可以在Mac机上通过Xcode获取，这将会使生成成功，另外这些头文件还需要在Windows机器上可通过VS智能感知去操作。当前我们的VS体验还不能帮助对第三方库的同步，但它确实有助于您创建一个Mac文件夹和Windows文件夹之间的映射。这个映射可以在属性页中定义。下图演示了包含目录和映射目录如何设置来让OpenGLES样本工作。

![](https://msdnshared.blob.core.windows.net/media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/00/65/69/6013.Mapping.png)



生成和调试我的iOS应用程序

生成你的iOS应用程序很容易，一旦你确信VCRemote正在运行，在OpenGLESApp.iOS.Application工程上右键单击选择生成，将启动生成iOS 应用程序。 生成iOS的应用程序本质上是一个两步流程。OpenGLESApp.iOS.StaticLibrary工程被转换为一个Xcode工程并和XCode工程 一起被包含​​在 OpenGLESApp.iOS.Application工程中，他们通过网络复制，然后使用Xcode工具链在MAC上进行生成，这个过程是无缝的，错误消息/警告和其他生成的诊断信息都会在VS输出窗口显示，这与Windows和Android的生成体验是类似的。 
一旦生成完成，该应用程序就可以在iOS设备或iOS模拟器上启动。作为我们产品的一部分，我们同样[延续了强大的调试体验去调试iOS应用程序](http://blogs.msdn.com/b/vcblog/archive/2015/04/29/debugging-c-code-on-ios-with-visual-studio-2015.aspx)。我们支持在模拟器和iOS设备上调试。在Visual Studio 2015 RC的体验中包括（但不限于）[F5](http://msdn.microsoft.com/en-us/library/y740d9d3.aspx)，[输出窗口](http://msdn.microsoft.com/en-us/library/bs4c1wda.aspx)，[断点](http://msdn.microsoft.com/en-us/library/5557y8b4.aspx)，[单步/跳过/跳出](http://msdn.microsoft.com/en-us/library/ek13f001(v=vs.100).aspx)，[运行到光标处](http://msdn.microsoft.com/en-us/library/dbhfa1e0(v=vs.100).aspx)，[调用堆栈](http://msdn.microsoft.com/en-us/library/a3694ts5.aspx)，[数据和变量窗口](https://msdn.microsoft.com/en-us/library/esta7c62.aspx)，[模块窗口](https://msdn.microsoft.com/en-us/library/4c8f14c9.aspx)，地址级调试（[拆卸](https://msdn.microsoft.com/en-us/library/a3cwf295.aspx)，[内存](http://msdn.microsoft.com/en-us/library/s3aw423e.aspx)，[寄存器窗口](https://msdn.microsoft.com/en-us/library/62680774.aspx)），[线程窗口](https://msdn.microsoft.com/en-us/library/w15yf86f.aspx)，[并行堆栈](https://msdn.microsoft.com/en-us/library/dd998398.aspx)和[并行监视](https://msdn.microsoft.com/en-us/library/hh418499.aspx)窗口。下面的快照描述Visual Studio停在C++ iOS 应用程序断点处。

![](https://msdnshared.blob.core.windows.net/media/MSDNBlogsFS/prod.evol.blogs.msdn.com/CommunityServer.Blogs.Components.WeblogFiles/00/00/00/65/69/1616.Debugging.png)

有关iOS调试的更多详细信息，你可以参考这个[博客](http://blogs.msdn.com/b/vcblog/archive/2015/04/29/debugging-c-code-on-ios-with-visual-studio-2015.aspx)。

总结

此博客提供给您的信息应该让您了解了Visual Studio 团队为实现iOS开发体验所做的工作。我们的iOS肯定还没有达到目标需求，而这个功能启用分享了我们目前的想法。在接下来的几个版本中，我们将努力改善iOS的开发体验，但现在我们很乐意让大家尝试并提供反馈意见。 
分享反馈很容易！在[UserVoice](http://go.microsoft.com/fwlink/?LinkId=517102)提交功能建议，在 [Connect](https://connect.microsoft.com/visualstudio)网站提交您找到的bug，并在可视化界面中[发送一个笑脸或苦脸](http://go.microsoft.com/fwlink/?LinkId=517102)。您也可以在下面留下评论。除此之外，如果你希望和我们的产品团队有一个关于这个主题的直接的交谈，









