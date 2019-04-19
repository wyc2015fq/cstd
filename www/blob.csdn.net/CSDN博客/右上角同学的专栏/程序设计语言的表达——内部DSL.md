# 程序设计语言的表达——内部DSL - 右上角同学的专栏 - CSDN博客
2016年08月05日 15:55:37[右上角徐](https://me.csdn.net/u011032983)阅读数：642
## 程序设计语言的表达——内部DSL
Tag：[向上走](http://dreamhead.blogbus.com/tag/%E5%90%91%E4%B8%8A%E8%B5%B0/)[DSL](http://dreamhead.blogbus.com/tag/DSL/)
[版权声明](http://creativecommons.org/licenses/by/3.0/deed.zh)：转载时请以超链接形式标明文章原始出处和作者信息及[本声明](http://www.blogbus.com/bangzhuzhongxin-logs/11205960.html)
[http://www.blogbus.com/dreamhead-logs/17667876.html](http://www.blogbus.com/dreamhead-logs/17667876.html)
[使用Java实现内部领域特定语言](http://www.infoq.com/cn/articles/internal-dsls-java)
[One Lair and Twenty Ruby DSLs](http://media.pragprog.com/titles/twa/martin_fowler.pdf)
[Implementing an Internal DSL](http://martinfowler.com/dslwip/InternalOverview.html)
上面几个文章都是关于DSL的，不过，在这里，我并不是太关心DSL的话题，我更感兴趣的是代码的写法。按照这几篇的分类方法，直接用程序设计语言编写的DSL算是内部DSL，也就是说，所谓内部DSL，也就是一种标准的程序代码。
Kent Beck在他的《[Implementation Patterns](http://www.douban.com/subject/2042269/)》的第三章《A Theory Of Programming》中，谈到了编程的价值观（Value）：Communication（沟通）、Simplicity（简单）和Flexibility（灵活）。如果说简单和灵活很容易理解的话，那么把沟通放在价值观中，尤其排在所有价值观的第一位，则显现出Kent
 Beck对于编程的深刻。在这个软件开发越来越需要协作的年代，写代码的时候，多站在让别人理解的角度考虑一下，会极大提升代码的可读性。在ThoughtWorks的招聘流程中，有一个Code Review的环节，拜这个环节所赐，我看过很多人的代码，不在少数的应聘者其代码唯一的优点就是完成了需求。以沟通为标准进行衡量，这显然是不够的。
同样，以沟通为标准，那么内部DSL显然在这方面做得更好，因为DSL本身就是为了让人更容易理解而存在的。这几篇文章中提供了很多内部DSL的手法，比如[Method Chaining](http://martinfowler.com/dslwip/MethodChaining.html)、[Expression
 Builder](http://martinfowler.com/dslwip/ExpressionBuilder.html)等等。抛开DSL这样的BuzzWord，这些方法应该属于增强程序本身表达能力的方法。
JDK有一个很好的Method Chaining的例子：StringBuffer的append方法。
StringBuffer sb = new StringBuffer();
sb.append("log1").append("log2").append("log3");
这样的写法显然比下面的写法更为简洁，尤其是需要往StringBuffer中添加很多内容的时候。
StringBuffer sb = new StringBuffer();
sb.append("log1");
sb.append("log2");
sb.append("log3");
在我看来，这些内部DSL技术为我们打开了一扇窗，它让我们在编写代码，尤其作为API提供的代码时，有了一个新的思考方向。当然，并不是一味的应用这些内部DSL技术就会写出好代码，作为一个有经验的软件开发人员，我们需要一定的鉴别能力，分辨出究竟怎样做才会真正的提高代码的“沟通”能力。
之前写过两篇关于程序设计语言表达的blog（[1](http://www.blogbus.com/dreamhead-logs/3252068.html)、[2](http://www.blogbus.com/dreamhead-logs/10692436.html)），虽然这篇不像那两篇一样讨论语言的差异，但也算是在语言表达能力上的探讨吧！
            
