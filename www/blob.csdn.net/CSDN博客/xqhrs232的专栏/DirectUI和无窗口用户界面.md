# DirectUI和无窗口用户界面 - xqhrs232的专栏 - CSDN博客
2012年08月22日 22:39:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：868标签：[silverlight																[windows																[wpf																[webbrowser																[微软																[microsoft](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=微软&t=blog)](https://so.csdn.net/so/search/s.do?q=webbrowser&t=blog)](https://so.csdn.net/so/search/s.do?q=wpf&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=silverlight&t=blog)
个人分类：[DirectUI界面技术](https://blog.csdn.net/xqhrs232/article/category/1243806)
原文地址::[http://blog.csdn.net/jiangsheng/article/details/5404320](http://blog.csdn.net/jiangsheng/article/details/5404320)
DirectUI/DirectUser是一个用户界面框架（[http://msdn.microsoft.com/en-us/library/system.windows.automation.automationelement.automationelementinformation.frameworkid.aspx](http://msdn.microsoft.com/en-us/library/system.windows.automation.automationelement.automationelementinformation.frameworkid.aspx)）。用Spy++可以在MSN
 Messenger、Windows XP、Office、IE和Windows Media Player中看到窗口类名字是DirectUIHWND的窗口。
从文件描述来看，DUI70.dll和DUser.dll看起来是这个框架的实现文件，而没有导入这些DLL的软件应该是复制了这个库的代码。举例来说，Windows Vista和IE7的测试版的IEFrame.dll导入了DUser.dll（[http://social.msdn.microsoft.com/Forums/en-US/windowsuidevelopment/thread/6b801577-1699-4093-8a58-198c64b120e0](http://social.msdn.microsoft.com/Forums/en-US/windowsuidevelopment/thread/6b801577-1699-4093-8a58-198c64b120e0)）而正式版使用IEUI.DLL。这应该是微软不愿意公开支持这个功能变化频繁的类库，而反垄断案禁止非Windows组件调用未公开Windows
 API的缘故。
再看看DUser.dll的函数导出表（[http://www.webtropy.com/articles/dll-api.aspx?dll=duser](http://www.webtropy.com/articles/dll-api.aspx?dll=duser)），可以看到Gadget这个词被广泛使用，而没有具体的控件。因为没有窗口句柄，所以控件不是用的窗口类来区分，而是可能和Windows
 Vista Sidebar Gadgets一样采用HTML做接口根据Office Communicator的资源来看，是使用XML做接口。
从这个类库的名字和行为来看，实际上应该是基于DirectX，和WPF类似的界面类库框架。在微软的招聘网站上可以看到Office Communications项目组的一个职位的介绍中描述说“Native Win32/64 UX experience via DirectUI, and Web UX experience via Silverlight”，说明这个项目组把它和Silverlight同等对待。DirectUIHWND窗口可以在需要性能和安全性的场合看到，例如IE的Tab窗口、Shell中的DefView、Windows登录界面等等。在Windows
 Vista上使用DirectUI的微软程序和WPF程序一样兼容Desktop Composition和远程桌面，应该是直接或者间接调用的Direct3D。
那么我们怎么做到类似的效果？
无窗口模式的用户界面并不是一个新的概念（[http://blogs.msdn.com/oldnewthing/archive/2005/02/11/371042.aspx](http://blogs.msdn.com/oldnewthing/archive/2005/02/11/371042.aspx)），Visual C++的应用程序向导就可以创建无窗口ActiveX。但是做过无窗口模式的RichEdit的实现的人都知道，微软的系统控件集中了各种各样的功能，比如各种快捷键、滚动条、界面风格、Accessibility、用户界面自动化等等，要像IE项目组那样几乎完全实现无窗口并不容易。Raymond
 Chen在[http://blogs.msdn.com/oldnewthing/archive/2005/02/11/371042.aspx](http://blogs.msdn.com/oldnewthing/archive/2005/02/11/371042.aspx)提到可以使用DrawThemeBackground和DrawFrameControl这两个API，不过这只对和Windows界面风格一致的程序有用。要是界面不复杂的话，可以简单的集成IE的Trident引擎，比如使用MFC的CHTMLView和CDHTMLDialog，以及Windows
 Forms的WebBrowser类。这样做的代价就是程序需要牺牲性能和可能在严格的IE安全性配置下无法工作，Visual Studio.Net开始的各种向导、Google Task、Microsoft Outlook、Outlook Express等就是使用的这种方案。
另外一个方案就是集成WPF或者Silverlight。VC项目组在用户调查中发现，需要使用WPF的Visual C++用户大都用C#编写WPF代码再用C++/CLI和非托管代码做接口([http://social.msdn.microsoft.com/forums/en-US/wpf/thread/dd1e31bb-feb4-4d77-b524-42a282f519b1/](http://social.msdn.microsoft.com/forums/en-US/wpf/thread/dd1e31bb-feb4-4d77-b524-42a282f519b1/))，所以他们决定致力于改进更多用户使用的功能，例如编辑器的智能感知，而不提供对WPF的支持。Visual
 Studio 2010就是使用这种方案。
为什么不能使用未公开的API?如果要看惊恐故事的话，可以参考[http://blogs.msdn.com/oldnewthing/archive/2003/12/23/45481.aspx](http://blogs.msdn.com/oldnewthing/archive/2003/12/23/45481.aspx)和[http://blogs.technet.com/stefan_gossner/archive/2005/07/27/undocumented_API_Part1.aspx](http://blogs.technet.com/stefan_gossner/archive/2005/07/27/undocumented_API_Part1.aspx)。
题后话：
*如果微软在产品文档中讨论未公开的API，那么API还是未公开的吗？（[http://msdn.microsoft.com/en-us/library/aa140182(office.10).aspx](http://msdn.microsoft.com/en-us/library/aa140182(office.10).aspx)）
*Visual Studio代码画廊中有个叫作DirectUI的类库，设计思想和微软的DirectUser库类似（[http://visualstudiogallery.msdn.microsoft.com/en-us/1B69C9FE-E422-4799-9EB5-6AC7034C52E1](http://visualstudiogallery.msdn.microsoft.com/en-us/1B69C9FE-E422-4799-9EB5-6AC7034C52E1)），不过也有人误认为这就是微软用来实现MSN的界面的库。
*IE9将会使用Dierct2D作为渲染引擎，不知道IE项目组在移植完代码之后是否能把控件类库的接口公开。从VC项目组征求下一个版本意见的问题([http://www.surveymonkey.com/s/HDQWGJR](http://www.surveymonkey.com/s/HDQWGJR))来看，微软也有意开发利用GPU的类库。
