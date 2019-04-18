# HGE系列之二    更上层楼 - tkokof1的专栏 - CSDN博客

2009年11月29日 10:32:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：2562标签：[游戏																[文档																[resources																[tools																[工具																[user](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=tools&t=blog)](https://so.csdn.net/so/search/s.do?q=resources&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)
个人分类：[游戏](https://blog.csdn.net/tkokof1/article/category/642710)


**HGE****系列之二**更上层楼

上次颇为感性的描述了一番HGE的轮廓，并给出了一个世界上最简单的HGE示例程序，不知读过的人有了怎样的认识，如果你说一点感觉都没有的话——哦，好吧，在此我再重述一番要点便是：

**1.**HGE是一款支持硬件加速的2D游戏引擎。

**2.**HGE的有以下优点：

l由于HGE构建于DirectX之上，所以其能够较好的支持硬件加速，并运行于安装有DirectX( 确切的说是DirectX 8.0 )的所有Win32平台上；

lHGE拥有较为完善的文档支持，并且HGE小组随时准备处理你的任何反馈；

lHGE是免费的，并且从1.7版本起便开始开源，而其发布在zlib/libpng许可证之下的特性，使他能够完全免费的提供给任何的使用者包括商业用户（音频部分除外）

lHGE专注于2D游戏开发，接口简单并且易于扩展，而且非常稳定！

**3.**HGE使用简单，不信看看示例：）

好了，复习时间结束，让我们转入今天讨论的主题：HGE的整体架构。也许你会问为

何不继续讲解几个示例，不过我想说只有我们首先对HGE的结构有了一些认识，才能更顺畅的挥舞起HGE的API，你说呢？当然，由于水平原因，我也不可能就此将你带入软件架构的殿堂，只是本着一起学习的态度稍稍讨论一番，所以就请原谅我的一意孤行，容我将话讲完吧：）

首先来看一张HGE官网提供的HGE架构图：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/tkokof1/EntryImages/20091203/architecture.gif)

这张图并不是专业的[UML](http://www.uml.org/)，相反的，他只是一张形象的示意图罢了，不知大家看明白了多少，在此我再细细的唠叨一番：

1.首先想讲讲的便是**HGE Core Functions**，意思很明确，就是HGE的核心接口函数，HGE文档中的HGE Functions便是指这个部分，如果你看过这部分的文档就知道，这部分函数提供了所有的底层操作支持，包括图形、声音、输入等等等等，其实就实现来说，这部分只用到了两个类：class HGE及 class HGE_Impl 。其中HGE只是一个接口类，包含了所有这些函数的纯虚定义，而HGE_Impl则公共继承HGE，并借助各类第三方库实现了所有这些接口（函数）。

2.再者便是**HGE Helper Classes**，即HGE辅助类，其实就功能来说，HGE的核心函数已经提供了全部，而这也意味着你完全可以在此基础上构建起你的游戏，但很显然，由于HGE Core Functions 提供的操作过于基本，这势必导致开发效率的降低，所以HGE的作者又在HGE Core Functions 的基础上封装了其中的很大一部分内容，于是便有了HGE Helper Classes，目前而言，他包括hgeSprite、hgeAnimation 、hgeFont等等类，提供的功能实用繁多，有兴趣的朋友可以看看文档继续了解。

3.至此，借助HGE Core Functions以及HGE Helper Classes的鼎力支持，那些HGE的用户（**User**）自然便可“逸兴遄飞”的开始编写他们的游戏代码了，而这也便成了图中的**User Code**部分。

4.HGE是否就包含以上的HGE Core Functions和HGE Helper Classes呢，毕竟现在我们已经能够**较方便**的编写游戏了，但显然，HGE的作者为了方便HGE的用户，使他们能达到**很方便**的境地，于是便有了**HGE Authoring Tools**，其实说白了，他们就是一些游戏开发中的编辑工具，HGE的最新版本中提供了很多实用的工具：如字体编辑器等共计五项，其中的一项粒子编辑器本人觉得很有味道，有兴致的朋友不妨试试：）

5.好了，HGE的构成到此已经清晰，它包括HGE Core Functions、HGE Helper Classes以及HGE Authoring Tools三个部分，而HGE的用户则可以借此编写出他们的游戏代码（**User Code**），并生成**Game ****Executable**（游戏可执行文件）以及各类的**Game Resources**（游戏资源），而这两部分便一起构成了**Game**。

6.图例里的各部分我已经讲完了，哦，不，等等，好像还有个**External Authoring Software**，这个么…我想可能是作者认为HGE提供的工具还远未完善，所以仍需要外部的一些编辑软件的帮忙，譬如PhotoShop一类，所以便有了这么一项…

呼，总算讲完了，内容虽然不多，但也挺费劲… 好了，今天就讲到这吧，其他的内容我想我得再仔细研习一下HGE，否则就免不了贻笑于大方之家，到时可就囧大了…所以么，还是那句，下次再见喽：）


