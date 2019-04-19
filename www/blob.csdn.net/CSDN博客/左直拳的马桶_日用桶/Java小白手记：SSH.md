# Java小白手记：SSH - 左直拳的马桶_日用桶 - CSDN博客
2015年06月19日 18:53:31[左直拳](https://me.csdn.net/leftfist)阅读数：1668
以下内容只是一个小白菜鸟的理解和总结，目的仅在于梳理思路。
13年的时候，我就说要学JAVA，有个C++高手同事赞许地说：“嗯，不错，SSH。。。”我不禁肃然起敬。SSH！多么高大上的词，JAVA就是这样，这框架，那概念的，理论一套一套的，特别多，让人听了就兴奋到不行。
那么，SSH究竟是个啥？
分别是 structs、spring、hibernate。它们都是框架，结合起来用在WEB项目中。
structs： 
主要是实现MVC中的C。在JAVA WEB项目中，这个C是指Servelet、Filter等拦截、过滤的程序。现在是structs2，据说与structs1没啥关系。 
但是，Spring也有MVC的实现，有了它可以不用structs。大家之所以还用structs，其实是看中Spring的IOC、AOP，即控制反转，面向切面等机制。（哎，控制反转、面向切面这些都跟设计模式、系统架构密切相关，其中面向切面这个概念，我还是去年为了考架构师才知道的。真羡慕JAVA程序员，接触到的理论、概念非常多，从没听说.NET程序员要关心这些的！） 
参考文章：
Spring: 
说了，有MVC\控制反转\面向切面这些机制。这是一个轻量级的框架，其目的是为了帮助程序员更容易使用现有的那些笨重的框架。
据说这个开源的框架是编码典范，设计经典，任何要学习java的人都应该仔细研读，必定获益良多！
参考文章： 
[http://baike.baidu.com/link?url=hEyeK3a4meovEHS8MlfiotFUCcgS6EPiF5m7P-fFfWoulf3Imoac3B1jkrHoGJx-jX3gYiRx7Oh5V_yS46_JyLK59xGC7VTRe4j7jqs4GCW](http://baike.baidu.com/link?url=hEyeK3a4meovEHS8MlfiotFUCcgS6EPiF5m7P-fFfWoulf3Imoac3B1jkrHoGJx-jX3gYiRx7Oh5V_yS46_JyLK59xGC7VTRe4j7jqs4GCW)
Hibernate: 
不就是一个ORM框架吗？ 
除了实体类、方便程序员像使用平常面向对象方式那样编码，操作数据库外，其实它还有缓存、事务等机制。其缓存，曰Session，我猛一看，还以为是HttpSession。 
让人汗颜的是，设计出Hibernate的天才，是因为觉得用传统的JAVA方式操作数据库太繁琐了，就决定自己写一个。他对SQL一窍不通，在写Hibernate之前，先到路边的旧书摊上买了一本SQL的入门书。靠，什么叫天才。
为什么JAVA涌现出如此之多杰出思想、概念、框架？我看，主要原因是在于它的开源。因为开源，可以聚集全世界的志同道合者一起参与，大家齐心协力，不断迸发出灵感的火花；而.NET只是一个公司的产品，尽管微软也很强，其开发者也很优秀，但毕竟人数相比太少；更何况，为开源产品添砖加瓦，完全出于个人兴趣，因而参与者可以乐此不疲，不知不觉就将JAVA做大做强；而微软的雇员做.NET，多少有任务的味道，有东西产出就行。
其次，微软因为WINDOWS太成功了，认为要将所有的产品都做得非常易用，包括开发工具。但是，程序员毕竟不同于普通用户，你将一个语言、工具做得那么傻瓜，拖控件、点鼠标，三两下就可以实现个 Hello World!，似乎开发效率提高了不少，但是，智能的外表下，掩盖了那些更需要为程序员所知道通晓的运行机制和原理！所以，微软产品系列的程序员，通病就是只知道应用，而缺乏对程序本质的理解，知其然而不知其所以然，更遑论那些系统架构级的概念、思想了。跟JAVA程序员相比，.NET程序员就像个傻逼。
参考文章： 
[http://www.admin10000.com/document/150.html](http://www.admin10000.com/document/150.html)
