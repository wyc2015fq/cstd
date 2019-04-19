# Absolute C++学习笔记1 - SHINING的博客 - CSDN博客
2010年01月02日 00:08:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：1351标签：[c++																[iostream																[reference																[编程																[编译器																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=iostream&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/snsn1984/article/category/488621)
《Absolute C++》------一本写给undergraduate的C++书
      关于C++的书可以说是很多了，很多时候不知道从何下手，翻过的书也很多，有的评价很高，自己觉得读的别扭，有的评价一般，但是自己很喜欢，或许这就是适合不适合的问题吧。
      同时翻看《The C++ Programming Language》和《Absolute C++》的原版书，最后放下了前者，拿起了后者。前者被称为C++圣经，作为一个对C++有一些了解，却没有太多经验的人，总是读的不明不白，或许是我的英文水平和技术水平有限。而后者给我简单明了的感觉。或许就像作者在前言里提到的一样，讲清楚是基本的，不光要讲清楚还要让人好理解。
      作者的语言使用的很简练，没有超长的复杂的语法句型，也没有生僻的单词，很利于让人理解。并且各个方面都讲的很细致，甚至会告诉你，如果编译器通不过的话，有可能是什么原因导致的，把现实生活中遇到的一些基本情况都说的很清楚。我觉得这对一个初学者来说都是很有必要的。因为有些问题就是我初学的时候碰到过的，困惑过的，甚至花了很长时间才解决的。还有些问题，也是现在看了才明白。
      全书一共分为二十章，第一章作为开始篇，主要介绍了C++基础，从C++的历史介绍开始，主要精力放在介绍变量、表达式，还介绍了输入输出、编程风格、库和命名空间。输入输出是对COUT,CIN,CERR三个进行了简要介绍，编程风格介绍了两种注释方式：//,/*---*/;库和命名空间就是拿IOSTREAM和STD做了个例子进行介绍，后面都会对这些内容进行具体的介绍。
      第一章中介绍类型转换的时候，这点我没有搞太明白，说是后面会具体介绍，所以就摘录下来：
      This static_cast is the most common kind of type cast and the only one we will
use for some time. For completeness and reference value, we list all four kinds of type
casts. Some may not make sense until you reach the relevant topics. If some or all of the
remaining three kinds do not make sense to you at this point, do not worry. The four
kinds of type cast are as follows:
static_cast<Type>(Expression)
const_cast<Type>(Expression)
dynamic_cast<Type>(Expression)
reinterpret_cast<Type>(Expression)
