# EXC_BAD_ACCESS的本质详解以及僵尸模式调试原理 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年02月29日 09:33:32[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：311标签：[iOS																[iOSOS X内存管理																[EXC_BAD_ACCESS报错处理](https://so.csdn.net/so/search/s.do?q=EXC_BAD_ACCESS报错处理&t=blog)](https://so.csdn.net/so/search/s.do?q=iOSOS X内存管理&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
有时候，你会遇到由EXC_BAD_ACCESS造成的崩溃。 这篇文章会告诉你什么是EXC_BAD_ACCESS，以及它产生的原因。我还会提供一些EXC_BAD_ACCESS错误的解决方案。
**1. 什么是 EXC_BAD_ACCESS?**
一旦你理解EXC_BAD_ACCESS的本质，你就会更好地理解这个模糊的名词。这里有一个极为简单的解释，也有一个技术层面的解释。我们首先从简单的解释开始说起。
**2. 简单的解释**
不管什么时候当你遇到EXC_BAD_ACCESS这个错误，那就意味着你向一个已经释放的对象发送消息。这是最常见的情况，但也有例外，我们将在稍后讨论。
**3. EXC_BAD_ACCESS的本质**
技术层面的解释有些复杂。在C和Objective-C中，你一直在处理指针。指针无非是存储另一个变量的内存地址的变量。当您向一个对象发送消息时，指向该对象的指针将会被引用。这意味着，你获取了指针所指的内存地址，并访问该存储区域的值。
当该存储器区域不再映射到您的应用时，或者换句话说，该内存区域在你认为使用的时候却没有使用，该内存区域是无法访问的。 这时内核会抛出一个异常（ EXC ），表明你的应用程序不能访问该存储器区域（BAD ACCESS） 。
总之，当你碰到EXC_BAD_ACCESS ，这意味着你试图发送消息到的内存块，但内存块无法执行该消息。但是，在某些情况下， EXC_BAD_ACCESS是由被损坏的指针引起的。每当你的应用程序尝试引用损坏的指针，一个异常就会被内核抛出。
**4.调试EXC_BAD_ACCESS**
调试EXC_BAD_ACCESS可能会非常棘手和令人沮丧。然而，现在EXC_BAD_ACCESS不再是一个谜，它没有想象中的那么可怕。
你需要知道的第一件事是您的应用程序并不一定是在崩溃的那一刻，无法访问内存区域。这就是常使调试EXC_BAD_ACCESS变得困难的原因。
同样受损指针也是如此。当你的指针被损坏时，您的应用程序不会崩溃。同时，如果您在应用程序中来回传递一个受损的指针也不会崩溃。当应用程序试图引用受损指针的时候，就会发生奔溃。
**5.僵尸调试模式**
僵尸调试模式在过去几年中得到了普及，事实上它们已经出现在Xcode上超过十年。僵尸听起来有点戏剧性，但它实际上是为帮助我们调试EXC_BAD_ACCESS功能而取得一个伟大的名字。让我来解释它是如何工作的。
在Xcode中，您可以启用僵尸对象，这意味着被释放的对象将会以僵尸的形式被保留。换言之，保留释放的对象就是为了调试。这里没有涉及任何魔法。如果您向僵尸对象发送消息，你的应用程序将会由于EXC_BAD_ACCESS而崩溃。
这有什么好处吗？让EXC_BAD_ACCESS难以调试的原因是，你不知道你的应用程序试图访问哪个对象。僵尸对象在许多情况下解决这个问题。通过保留已释放的对象，Xcode可以告诉你你试图访问哪个对象，这使的查找问题原因容易得多。
在Xcode中启用僵尸对象是很容易的。注意，这可能会因的Xcode的版本而不同的。以下方法适用于Xcode的6和7，单击左上角的Edit Scheme，并选中Edit Scheme。
在左侧选中Run ，在上方打开 Diagnostics选项。要启用僵尸对象，勾选 Enable Zombie Objects选框。
![figure-edit-scheme.jpg](http://cc.cocimg.com/api/uploads/20160225/1456389991284163.jpg)
如果你现在遇到EXC_BAD_ACCESS ，在Xcode的控制台输出，告诉你该从哪里查找问题。看看下面的例子输出。 
```js;toolbar
2015-08-12 06:31:55.501 Debug[2371:1379247] -[ChildViewController respondsToSelector:] message sent to deallocated instance 0x17579780
```
在上面的例子中， Xcode告诉我们， respondsToSelector的消息：被发送到一个僵尸对象。然而，僵尸对象不再是ChildViewController类的一个实例。以前分配给ChildViewController实例的内存区域不再映射到您的应用程序。这为你了解问题产生的根本原因提供一个不错的建议。
不幸的是，僵尸对象将无法保存您的一天每次崩溃的EXC_BAD_ACCESS的记录。既然僵尸对象没有这些方法，那么你可以采取其他的方法进行一些适当的分析。
**6.分析**
如果僵尸对象不能解决你的问题，那么问题的根源可能就不那么简单了。在这种情况下，您需要仔细看看在应用程序崩溃时执行的代码。这可能是繁琐和耗时的。
为了帮助你发现你的代码的问题，你可以使用Xcode来分析你的代码，帮助你找到出现问题的地方。注意，Xcode分析项目，它会指出每一个潜在的可能出现的问题的地方。
使用Xcode来分析你的项目，从Xcode的 Product菜单选择 Analyze或按 Shift-Command-B.Xcode的将需要片刻的时间，但是当它完成的时候你会在左边的 Issue Navigator看到问题列表。由Analyze发现的问题用蓝色高亮显示。
![figure-analyze-1.jpg](http://cc.cocimg.com/api/uploads/20160225/1456389928973950.jpg)
当你点击一个问题，Xcode的会指向问题代码块，这些正是你要的注意的地方。注意，Xcode仅仅是建议。在某些情况下，这是可能的，问题是不相关的，不固定。
![figure-analyze-2.jpg](http://cc.cocimg.com/api/uploads/20160225/1456389913216187.jpg)
如果你找不到造成EXC_BAD_ACCESS的错误，那就需要你仔细审视Xcode项目，分析其中发现的每一个问题。
**7.结论**
EXC_BAD_ACCESS是开发者面临的一个共同的问题，它是手动内存管理固有的问题。虽然推行ARC内存管理方式 （自动引用计数）使得EXC_BAD_ACCESS没那么频繁，但他们并没有真正的消失。
