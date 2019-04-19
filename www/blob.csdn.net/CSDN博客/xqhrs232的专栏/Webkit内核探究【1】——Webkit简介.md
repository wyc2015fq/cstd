# Webkit内核探究【1】——Webkit简介 - xqhrs232的专栏 - CSDN博客
2013年02月24日 10:14:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：556
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://www.cnblogs.com/jyli/archive/2010/01/31/1660355.html](http://www.cnblogs.com/jyli/archive/2010/01/31/1660355.html)
相关网帖::[Webkit内核探究【2】——Webkit CSS实现](Webkit%E5%86%85%E6%A0%B8%E6%8E%A2%E7%A9%B6%E3%80%902%E3%80%91%E2%80%94%E2%80%94Webkit)[Webkit内核探究【2】——Webkit CSS实现](CSS%E5%AE%9E%E7%8E%B0)
**注：**【转载请注明文章来源、保持原样】
出处：[http://www.cnblogs.com/jyli/archive/2010/01/31/1660355.html](http://www.cnblogs.com/jyli/archive/2010/01/31/1660355.html)
作者：李嘉昱
研究Webkit内核已经有一段时间了，在这期间我花了很多时间去研读webkit代码，并在网上查阅了不少的资料。在这之后，我开始尝试对Webkit内核进行剪裁，以便得到自己所需的部分。
俗语有言，“独学而无友 则孤陋而寡闻”，遂本着交流的态度拿出来与大家分享之，顺便回顾一下自己这段时间以来走过的路。到目前，本人虽不能说是精 通Webkit内核 ，但也总算是在Webkit内核中游历过了一番，并获得了不少Webkit内核代码的直接经验。
在此，我将以Webkit内核代码为基础，详细介绍Webkit内核的组成以及其各个模块的功能和实现。文章写到哪算哪，有了新的认识也会更新，补充。
**什么是Webkit**
Wekbit是一个开源的Web浏览器引擎，也就是浏览器的内核。Apple的Safari, Google的Chrome, Nokia S60平台的默认浏览器，Apple手机的默认浏览器，Android手机的默认浏览器均采用的Webkit作为器浏览器内核。Webkit的采用程度由 此可见一斑，理所当然的成为了当今主流的三大浏览器内核之一。另外两个分别是Gecko和Trident，大名鼎鼎的Firefox便是使用的Gecko 内核，而微软的IE系列则使用的是Trident内核。虽然目前市场上IE内核浏览器仍然占据大多数市场份额，但其他内核浏览器也大有后来居上之势。其
 中,Webkit作为一款优秀的浏览器内核，它众多优秀的特性引起业内的的广泛关注。尤其是近来，google的加入更是让Webkit有所升温，从 Goole Chrome浏览器, Goole Anroid手机操作系统内置浏览器均采用Webkit作为内核， 近来推出的Chrome OS更是让人期待。从实现角度来讲，据说其比Gecko代码更为整洁清晰，我目前还没研究过Gecko的代码，所以还无从比较。
**Wekbit做了什么？**
作为浏览器的内核，Webkit做了哪些工作？为了了解这些，先让我们来看下一个Web浏览器究竟做了什么。我们可以从输入输出的角度来看一个 Web浏览器为我们做了哪些工作。先看一个简单的例子，Web浏览器的输入是一个HTML文档，输出则是一个我们用眼睛所看到的一个Web页面, 就普通用户而言它的输入和输出就是这么简单，如下图所示。
![](http://images.cnblogs.com/cnblogs_com/jyli/webkit/browser_fun.jpg)
那么Webkit的输入和输出又是什么呢？如果能明白这个，那我们就能很清楚的知道Webkit到底是做什么的了。不过现在要说清楚这个还有点困 难，因为Webkit的输出就要复杂些了，因为它的输出本来就非直接面向用户，现在简单点来说，Webkit的输入是web 文档，输出是一些看不见的模型，浏览器上层借助于这些模型来绘制出我们所看到的实际页面。后面适当的时候会更具体的探讨这些模型。
**Wekbit组成**
Webkit实际上包含三大部分，至少从代码结构上来说是这样的，当然，如果细分的话还能够划分出更多的模块。如下图所示
![](http://images.cnblogs.com/cnblogs_com/jyli/webkit/webkit_overview.jpg)
其中，WebCore是Webkit的核心部分，它实现了对文档的模型化，包括了CSS, DOM, Render等的实现， JavaSript Core显然是对JavaSript支持的实现。而橘黄色标注的Webkit部分包含了很多不同平台对Webkit封装的实现，即抽象出了与浏览器所能直 接对应的一些概念的实现，如，WebView,WebPage, WebFrame等。这三部分共同构成了Webkit, 在源码中，它们分别对应这个这三个目录， 即Webkit三大部分为WebCore, JavaSript Core,Webkit。
**应用程序如何利用Webkit的**
从下图可以看出，利用Webkit的应用程序的位置处于Webkit模块之上，应用程序并不直接和WebCore以及JavaScript Core来打交道，也不需要和它们打交道，而是同我前面提到过的Webkit模块来交互，从而屏蔽了WebCore和JavaSript Core部分。Webkit模块实际上抽象出了大多数应用程序所需要的那一部分，这里所说的大多数应用程序其实就是指的浏览器， 对于实现一个浏览器而言，Webkit模块所提供的接口已经足够。不过对于某些应用，可能得对其实施一定的改造，但是话又说回来，Webkit本来就是被设计成为Browser的内核而被实现的。
![](http://images.cnblogs.com/cnblogs_com/jyli/webkit/Webkit_App.jpg)
看到这里，也就是说如果我们要想利用Webkit来实现一个功能完善Browser或其他类浏览器应用程序的话，你只需要了解Webkit模块部分 就足够了，更确切的说只需要了解它所提供的接口就够了。事实上，大多数平台，如果采用了Webkit内核作为浏览器内核组件的话，所提供的对应文档部分都 是关于Webkit模块所提供的接口的描述。很多平台都提供了Browser组件，其使用方法就像是其他可视组件一样，在应用程序中应用起来非常方便。比 如，某平台提供了叫做WebView的可视化组件，我们就可以通过如下几行代码来展示一个网页，
 使用的代码仅仅三行。
WebView *view = new WebView();
view->load(Url("http://www.google.cn"));
view->show();
以Webkit作为浏览器组件的平台有不少，包括Symbian S60, Android, Qt, GTK等等，前不久看到消息，好像Black Berry也准备在自家的手机上使用Webkit了。
