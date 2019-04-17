# 微软又开源了一个机器学习框架，这次是核心产品的机器学习引擎infer.NET - 知乎
# 



> 乾明 整理编译自 Microsoft Research Blog
量子位 报道 | 公众号 QbitAI

感觉微软对开源上瘾了。

在开源了跨平台机器学习框架[http://ML.NET](https://link.zhihu.com/?target=http%3A//ML.NET)之后，微软又开源了一个非常重要的机器学习框架：**[http://infer.NET](https://link.zhihu.com/?target=http%3A//infer.NET)**，而且采用的还是MIT许可证。

这个消息一出，可以说是八方点赞，Twitter上一片好评。

那么，[http://infer.NET](https://link.zhihu.com/?target=http%3A//infer.NET)是什么？有何优点？应用范围如何？该怎么去下载使用呢？

下面就来回答这些问题~

## **关于[http://infer.NET](https://link.zhihu.com/?target=http%3A//infer.NET)**

简单来说，[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)是一个基于模型的机器学习框架。

根据官方介绍，[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)是一个在图形模型中运行贝叶斯推理框架，可以用于概率编程，也可以用来解决许多不同类型的机器学习问题，包括分类、推荐或聚类等标准问题，并可以针对特定领域问题提供定制解决方案。






![](https://pic4.zhimg.com/v2-2db2c860f744d6c10d8b9eb23587603b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='696' height='215'></svg>)






一般情况下，进行机器学习的时候，开发人员为框架提供模型后，都需要将他们的模型映射到预先存在的学习算法上去。

而[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)将这个过程反了过来，可以直接从模型中生成一个定制的机器学习算法，允许你将领域知识融入到模型之中，这样“反其道而行之”的做法，给这个框架带来了不少优势。

在微软的官方博文中，给出了[http://infer.NET](https://link.zhihu.com/?target=http%3A//infer.NET)框架的4个优势，分别是：

**1、可解释性强。**

如果你设计了一个模型，并且从模型中生成算法。那么，你就可以理解为什么系统会以特定的方式运行，或者做出某些预测。随着机器学习应用逐渐进入日常生活，理解和解释它们的行为变得越来越重要。

**2、数据适用性强。**

基于模型的机器学习框架，对于那些具有某些特征的数据有很强的适用性，可以更好的使用这些数据来完成机器学习任务。比如实时数据、异构数据、数据不足、未贴标签的数据、有缺失部分的数据和以已知偏差收集的数据等等。

**3、用概率程序来描述模型。**

[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)可以将概率化程序编译成高性能代码，用于实现被称为确定性近似贝叶斯推断的东西。这种方法提供了大量的可拓展性，例如，在一个系统中使用[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)，系统可以自动从数十亿个网页中提取知识，包括大量的数据。

**4、支持在线贝叶斯推断。**

[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)的一个关键功能是支持在线贝叶斯推断，即系统在新数据到来时进行学习的能力。这在与用户进行实时交互的商业和消费品中是必不可少的。

具有这么多的优势，在实际的应用场景中，[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)的运用范围也非常广泛。

除了广泛应用于信息检索、流行病学、生物信息学等各个学术研究领域之外，[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)已经发展成了微软 Office、 Xbox 和 Azure 等微软核心产品中的机器学习引擎。

最近还应用到了游戏评级系统之中，微软的研究团队基于TrueSkill，使用[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)框架开发了TrueSkill 2。从《Halo 5》中的应用来看，相比于TrueSkill，玩家匹配的准确率从52%提升到了68%。

## **使用指南**

[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)框架开源之后，将成为[http://ML.NET](https://link.zhihu.com/?target=http%3A//ML.NET)的一部分。这是一款面向.NET开发者的机器学习框架，微软在今年的Bulid大会上开源了这一框架。

根据博客文章的介绍，微软已经采取了大量集成[http://ML.NET](https://link.zhihu.com/?target=http%3A//ML.NET)。比如在.NET Foundation下设置存储库，并将包和命名空间移动到Microsoft.ML.Probabilistic。[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)也将提升[http://ML.NET](https://link.zhihu.com/?target=http%3A//ML.NET)在统计建模和在线学习方面的能力。

此外，[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)也是跨平台的。根据官方的Readme文档，[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)支持 .NET Framework 4.6.1、.NET Core 2.0 和 Mono 5.0。

Windows 用户需要在 Visual Studio 2017 中使用，而 macOS 和 Linux 开发者可以使用.NET Core 2.0 SDK构建和运行.NET Standard和.NET Core项目。

对[http://Infer.NET](https://link.zhihu.com/?target=http%3A//Infer.NET)感兴趣？请收好下面的一些传送门：

框架下载地址：

[https://www.nuget.org/packages?q=Microsoft.ML.Probabilistic](https://link.zhihu.com/?target=https%3A//www.nuget.org/packages%3Fq%3DMicrosoft.ML.Probabilistic)

教程和示例地址：

[https://dotnet.github.io/infer/userguide/Infer.NET%20tutorials%20and%20examples.html](https://link.zhihu.com/?target=https%3A//dotnet.github.io/infer/userguide/Infer.NET%2520tutorials%2520and%2520examples.html)

用户指南：

[https://dotnet.github.io/infer/userguide](https://link.zhihu.com/?target=https%3A//dotnet.github.io/infer/userguide)

GitHub地址：

[https://github.com/dotnet/infer](https://link.zhihu.com/?target=https%3A//github.com/dotnet/infer)

—**完**—

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




