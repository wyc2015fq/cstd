# MVC框架中的模型-视图分离问题（一） —— “你必将业务逻辑由显示中分离” - 沧海一粟 —— 技术随手记 - CSDN博客





2011年02月13日 21:18:00[慢游](https://me.csdn.net/eengel)阅读数：2670








    说到MVC[1]（Model-View-Controller，模型-视图-控制器），应该说是这些年进行web动态网页开发最“给力”的框架了。而基于MVC的web应用框架[2]（web application framework）也多如繁星。具体到开发某个web项目，就涉及到选择什么样的应用框架实现最适合。网上有大量关于如何选择MVC框架的优秀文章，这儿就不多加讨论了。
    本文关注的是MVC中一个小而关键的问题，那就是模型-视图的分离（Model-View Separation）。之所以讨论这个问题，固然有它的现实意义，比如可以指导对具体应用框架实现的选择，本文也会在后面结合实际情况进行论述；而更重要地，希望能够从理论的角度对这个问题进行一般性的总结，抽象出一些普适性的原理和规则，无论MVC框架实现如何变化万千，其对框架的设计和运用都可以产生一定的指导意义。
    具体说来，这样的讨论涉及如下问题：
- 什么是模型-视图分离？
- 为什么要进行模型-视图的分离？这种分离会给web开发人员带来什么好处，需要他们付出多少精力？
- 指导模型-视图分离的规则和方法是什么？
- 对于某个具体的框架实现，评估其模型-视图分离的程度，以及对开发人员的影响。

    当然，每个理论层面的讨论都需要有一定的基础理论支撑。对“模型-视图分离”的探讨建立在如下原则上：你必将业务逻辑由显示中分离。（thou shalt separate business logic from display.）
    本文的主要论点及理论描述来源于文献《Enforcing Strict Model-View Separation in Template Engines》[3]；有兴趣的朋友可以直接查阅。而写此文的目的是由于近来参与了若干不同的项目，而采用的MVC框架实现也各不相同；观察到不同的框架在“视图”部分的实现有很大差异，而且一旦需要更换框架，实现“视图”动态部分的代码几乎需要全部重写。
遂萌发弄清楚“模型-视图分离”问题的想法。
    做为“模型-视图分离”问题发生的所在，下面首先介绍一下模板引擎[4]（template engine）及其历史演变。



参考文献
[1] [http://en.wikipedia.org/wiki/Model_View_Controller](http://en.wikipedia.org/wiki/Model_View_Controller)
[2] [http://en.wikipedia.org/wiki/Web_application_framework](http://en.wikipedia.org/wiki/Web_application_framework)
[3] Parr, T.J., Enforcing Strict Model-view Separation in Template Engines. WWW ’04: Proceedings of the 13th international conference onWorldWideWeb, New York, NY, USA, ACM Press (2004) 224–233, [http://www.cs.usfca.edu/~parrt/papers/mvc.templates.pdf](http://www.cs.usfca.edu/~parrt/papers/mvc.templates.pdf)
[4] [http://en.wikipedia.org/wiki/Template_engine_(web](http://en.wikipedia.org/wiki/Template_engine_(web))



