# Scala  集合 - Simple 专栏 - CSDN博客
2018年09月16日 00:49:29[Simple_Zz](https://me.csdn.net/love284969214)阅读数：31标签：[Scala](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala拥有丰富的集合库。集合是一种用来存储各种对象和数据的容器。 这些容器可以被排序，诸如列表，元组，选项，映射等的线性集合。集合可以具有任意数量的元素或被限制为零或一个元素(例如，`Option`)。
集合可以是严格的(`strict`)或懒惰的(`Lazy`)。 懒惰集合的元素在访问之前可能不会使用内存，例如`Ranges`。 此外，集合可能是可变的(引用的内容可以改变)或不可变的(引用引用的东西从不改变)。 请注意，不可变集合可能包含可变项目。
对于一些问题，可变集合的工作更好，而对于其他集合，不可变集合的工作更好。 如果有疑问，最好从不可变集合开始，如果需要可变集合，可以更改为可变集合。
本章将介绍最常用的集合类型以及对这些集合最常用的操作。
|序号|方法|描述|
|----|----|----|
|1|[Scala链表](https://blog.csdn.net/love284969214/article/details/82710399)|Scala List[T]是`T`型链表。|
|2|[Scala集合](https://blog.csdn.net/love284969214/article/details/82710402)|一组是相同类型的成对不同元素的集合。|
|3|[Scala映射](https://blog.csdn.net/love284969214/article/details/82710406)|映射是键/值对的集合，任何值都可以根据其键进行检索。|
|4|[Scala元组](https://blog.csdn.net/love284969214/article/details/82710408)|与数组或列表不同，元组可以容纳不同类型的对象。|
|5|[Scala选项](https://blog.csdn.net/love284969214/article/details/82710411)|`Option[T]`提供一个给定类型的零个或一个元素的容器。|
|6|[Scala迭代器](https://blog.csdn.net/love284969214/article/details/82710418)|迭代器不是集合，而是一种逐个访问集合元素的方法。|
