# 032_AUTOSAR学习笔记_接口 - 小灰笔记 - CSDN博客





2018年12月12日 23:34:43[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：230








         首先先总结一下AUTOSAR的接口分类：AUTOSAR接口、标准化AUTOSAR接口、标准化接口、AUTOSAR应用接口。具体的应用，参考Vector如下的资料：

![](https://img-blog.csdnimg.cn/20181212233426222.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         先从字面意思上猜测一下：标准化AUTOSAR接口是不是AUTOSAR接口的一种特殊实现？标准化接口是不是没有符合AUTOSAR具体的要求？

         从上面图上看，RTE与服务之间使用的标准AUTOSAR接口、OS跟通信与RTE之间的接口是标准化接、剩下的与RTE相关的接口全都是AUTOSAR接口。而进一步，前面的接口全都排除后，BSW用到的接口全都是标准化接口。这几种的具体划分究竟是一个什么界限，还是需要再明确了解一下。

![](https://img-blog.csdnimg.cn/20181212233426222.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         以上是标准接口的简单介绍，信息比较简洁，理解不一定正确。

         C-API的含义可能是指传统模式的简单C接口函数？

         OS与COM之间的访问必须通过RTE，这个之前已经明确。

         Ports这一条是说标准接口没有Ports？也就是说标准化接口不是用来提供或者请求数据和服务的？

         COM也算是标准化接口，由RTE管理？这样的话，为什么SW-C与RTE之间并没有一个标准化的接口描述示意图元素？

         EcuM也是标准化接口？EcuM是什么的缩写？是ECU管理的缩写吗？至少应该不是ECU的抽象层而是与RTE之间的接口，这又是图中哪一块呢？

![](https://img-blog.csdnimg.cn/20181212233426265.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         上面是AUTOSAR接口的描述，针对这一部分的信息相对来说还算是明确一点。主要是涉及到数据以及服务的请求和提供的功能。从Runnables的描述看，这个应该还有一个特征就是与任务调度有关，可能是周期性的任务调度也可能是事件触发式的任务调度。

![](https://img-blog.csdnimg.cn/20181212233426606.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         上面是标准AUTOSAR接口的描述。

         标准的AUTOSAR接口与ECU无关，服务中的存储应该也是跟ECU相关的，为什么会划分到这里来呢？ECU抽象层的功能是让人不再考虑ECU的具体实现？

         关于接口的知识点，依然还是处于模模糊糊的状态，或许还是参考某一个软件才能够有一个很好的理解。



