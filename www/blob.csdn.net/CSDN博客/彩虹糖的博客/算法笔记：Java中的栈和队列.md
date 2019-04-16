# 算法笔记：Java中的栈和队列 - 彩虹糖的博客 - CSDN博客





2019年04月04日 23:12:29[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：8








**栈**和**队列**是十分基本的数据结构，其基本方法和实现想必大家一定耳熟能详，因此，在这篇博文中，不会介绍栈和队列的基本知识，而是讲解其实现中我觉得比较重要和有意思的点。

## 一. 大小调整(Resizing Arrays)

关于栈和队列的实现，我们既可以使用**链表**实现，也可以使用**数组**实现。在使用数组实现的时候，一个十分重要的问题就是调整数组的大小。当数组分配的空间已经被占满，而我们要添加新的元素，这个时候我们要扩展数组。当我们不断的从数据结构中拿出元素，为了避免空间的浪费，我们也要及时缩减数组的规模。

首先看数组的**扩展**。

如果我们选择增加一个元素马上就扩展一位，这将是一个十分糟糕的解决方案，因为每扩展一位，我们就要把旧元素从旧数组中转移到新数组中。这将是一个**O(n^2)**的算法复杂度。

那么什么样的方法是比较好的呢？我们选择每次扩展数组的时候，每次把数组长度翻倍。比如原数组长度为4，在添加新元素的时候，把数组长度变为8.这保持了**O(n)**的算法复杂度，具体是多少可以算一下。

![](https://img-blog.csdnimg.cn/20190402230509386.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

再看数组的**缩小**。

同样，我们不能减少一个元素，就缩减一次，这样同样会造成极大的开销。因此我们也是选择当数组大小变为容量的四分之一的时候再把数组的长度缩减一半。

![](https://img-blog.csdnimg.cn/20190403104611190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0,size_16,color_FFFFFF,t_70)

二. 实现方法比较

使用链表或者数组实现栈和队列各有优劣。

链表：对栈和队列的操作使用的时间都是常数，但是需要多余的空间存储指针。

数组：在最差的情况下，需要O(n)的时间，但是空间浪费更少。

三. 泛型，迭代器

关于泛型可以看这篇博文：[Java 泛型详解](https://www.cnblogs.com/coprince/p/8603492.html)

关于迭代器：[深入理解Java中的迭代器](https://www.cnblogs.com/zyuze/p/7726582.html)



