# Spring中Order接口的作用 - z69183787的专栏 - CSDN博客
2016年05月04日 05:58:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3857
Spring中Ordered接口简介 目录前言 Spring中提供了一个Ordered接口。Ordered接口，顾名思义，就是用来排序的。 Spring是一个大量使用策略设计模式的框架，这意味着有很多相同接口的实现类，
Spring中Ordered接口简介
目录 前言
Spring中提供了一个Ordered接口。Ordered接口，顾名思义，就是用来排序的。
Spring是一个大量使用策略设计模式的框架，这意味着有很多相同接口的实现类，那么必定会有优先级的问题。
于是，Spring就提供了Ordered这个接口，来处理相同接口实现类的优先级问题。
Ordered接口介绍
首先，我们来看下Ordered接口的定义：
{ int HIGHEST_PRECEDENCE = Integer.MIN_VALUE; intLOWEST_PRECEDENCE = Integer.MAX_VALUE; int getOrder(); }
只有1个方法：getOrder()；  2个变量：最高级(数值最小)和最低级(数值最大)。
OrderComparator类：实现了Comparator的一个比较器。
提供了3个静态排序方法：sort(List<?> list)、sort(Object[] array)、sortIfNecessary(Object value)。根据OrderComparator对数组和集合进行排序。
sortIfNecessary方法内部会判断value参数是Object[]还是List类型，然后使用Object[]参数的sort方法和List参数的sort方法进行排序。
我们看下这个比较器的compare方法：
public int compare(Object o1, Object o2) { boolean p1 = (o1 instanceof PriorityOrdered); boolean p2
 = (o2instanceof PriorityOrdered); if (p1 && !p2) { return -1; } else if (p2 && !p1) { return 1; } int i1
 = getOrder(o1); inti2 = getOrder(o2); return (i1 < i2) ? -1 : (i1 > i2) ? 1 : 0; }
PriorityOrdered是个接口，继承自Ordered接口，未定义任何方法。
这段代码的逻辑：
　　1. 若对象o1是Ordered接口类型，o2是PriorityOrdered接口类型，那么o2的优先级高于o1
　　2. 若对象o1是PriorityOrdered接口类型，o2是Ordered接口类型，那么o1的优先级高于o2
　　3. 其他情况，若两者都是Ordered接口类型或两者都是PriorityOrdered接口类型，调用Ordered接口的getOrder方法得到order值，order值越大，优先级越小
简单概括就是：
OrderComparator比较器进行排序的时候，若2个对象中有一个对象实现了PriorityOrdered接口，那么这个对象的优先级更高。
　　若2个对象都是PriorityOrdered或Ordered接口的实现类，那么比较Ordered接口的getOrder方法得到order值，值越低，优先级越高。
Ordered接口在Spring中的使用
以SpringMVC为例，举例Ordered接口的运用。
<mvc:annotation-driven/>
这段配置在*-dispatcher.xml中定义的话，那么SpringMVC默认会注入RequestMappingHandlerAdapter和RequestMappingHandlerMapping这两个类。
关于这部分的内容，请参考楼主的另外一篇博客：
既然SpringMVC以及默认为我们注入了RequestMappingHandlerAdapter和RequestMappingHandlerMapping这两个类，我们是否可以再次配置这两个类？
答案当然是可以的。
RequestMappingHandlerMapping:
===>
RequestMappingHandlerAdapter:
====>text/plain;charset=UTF-8> =====> =====>
当我们配置了annotation-driven以及这两个bean的时候。Spring容器就有了2个RequestMappingHandlerAdapter和2个RequestMappingHandlerMapping。
DispatcherServlet内部有HandlerMapping(RequestMappingHandlerMapping是其实现类)集合和HandlerAdapter(RequestMappingHandlerAdapter是其实现类)集合。
private List<HandlerMapping> handlerMappings; private List<HandlerAdapter> handlerAdapters;
我们看下这两个集合的初始化代码：
![Spring中Ordered接口简介](http://94it.net/uploads/allimg/140612/1112492138_0.png)
![Spring中Ordered接口简介](http://94it.net/uploads/allimg/140612/111233J01_1.png)
很明显使用了我们提到的OrderComparator比较器进行了排序。
下面我们看下annotation-driven代码配置的RequestMappingHandlerMapping和RequestMappingHandlerAdapter。
![Spring中Ordered接口简介](http://94it.net/uploads/allimg/140612/111233J01_0.png)
![Spring中Ordered接口简介](http://94it.net/uploads/allimg/140612/11121D217_1.png)
RequestMappingHandlerMapping默认会设置order属性为0，RequestMappingHandlerAdapter没有设置order属性。
我们进入RequestMappingHandlerMapping和RequestMappingHandlerAdapter代码里面看看它们的order属性是如何定义的。
AbstractHandlerMethodAdapter是RequestMappingHandlerAdapter的父类。
AbstractHandlerMapping是RequestMappingHandlerMapping的父类。
