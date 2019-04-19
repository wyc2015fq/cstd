# ASP.NET MVC 原理 - 左直拳的马桶_日用桶 - CSDN博客
2015年10月31日 19:09:11[左直拳](https://me.csdn.net/leftfist)阅读数：3579
我出了份卷子做面试题，其中之一就是要求说说ASP.NET MVC的原理。感觉太空泛了，谁能说得准呢？
但是，如果站在我这个面试官立场，面试题好多时并不要求有标准答案，可能也没有什么标准答案，主要是通过问答，了解一下面试者的理解程度，相关知识程度什么的。其实，我自己水平也不行，但因为年纪大，好像不带点人大家会觉得有点怪怪的，所以过去一段时间也参与点招聘工作。
对于ASP.NET MVC，我自己的理解是酱紫的：
MVC是一种思想。ASP.NET MVC只不过是实现了这种思想的一种开发框架。利用Web Form项目，其实也可以实现MVC。以前搞WEB FORM,我就试过MVP模式。
MVC的思想要点的在于将UI和业务逻辑、数据分离。好处一是逻辑清晰，二是有利于逻辑重用和测试。因为UI很难自动化测试，但逻辑却可以。所以说，ASP.NET MVC有利于进行单元测试。并且UI与逻辑、数据分离以后，也容易重用。比如说，在ASP.NET MVC中，不同的action可以指向同一个view。
经典的MVC中，Model是个巨无霸，包含业务逻辑和数据。controller只是一个中间人，在View和Model之间传递点什么，打打酱油。有时候，Model和View甚至直接联系，绕开了controller。
而在ASP.NET MVC中，虽然有一个Model，但此Model并非彼巨无霸Model。ASP.NET MVC中的Model，是一个跟视图有关的Model，也就是传说中的那个VM，ViewModel，它只是经典巨无霸Model的一部分。ViewModel，顾名思义，主要用于View。经典巨无霸Model，被拆分成了数据（ViewModel、领域对象、数据库实体类 ）+ 业务逻辑（BLL ,各种Service）。因此，Model只不过是一群概念的集合，并不是一个实实在在的对象。
值得一提的是，ViewModel、领域对象、数据库实体类很有可能是同一个对象。比如说，我们在View里直接操作的VM，其实常常就是将数据持久层的实体类直接拿过来用的。即便如此，但在软件设计阶段，还是要将它们的概念区分开。
ASP.NET MVC的原理，其实就是使用HttpModule和HttpHandler将用户的请求拦截，按照设定的路由规则解释到相应的控制器和Action，加以执行。Module是一个比较宏观一点的概念，像Session、Cache都由相应的Module负责；而Handler就是负责处理具体请求的。先有Module，再有Handler。它们的关系，有点像进程和线程。
