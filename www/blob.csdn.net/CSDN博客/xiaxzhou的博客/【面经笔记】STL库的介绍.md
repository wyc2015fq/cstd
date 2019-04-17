# 【面经笔记】STL库的介绍 - xiaxzhou的博客 - CSDN博客





2017年07月15日 22:39:03[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：231
个人分类：[阿里巴巴面试题](https://blog.csdn.net/xiaxzhou/article/category/7015658)









#### STL库的介绍

STL**六大组件**简介

1、**容器**（Containers）：各种数据结构，如Vector,List,Deque,Set,Map,用来存放数据，STL容器是一种**类模板**。

2、**算法**（Algorithms）：各种常用算法如Sort,Search,Copy,Erase,从实现的角度来看，STL算法是一种**函数模板**。

3、**迭代器**（Iterators）：扮演容器与算法之间的胶合剂，是所谓的“泛型指针”，共有五种类型，以及其它衍生变化，从实现的角度来看，迭代器是一种将：Operators*,Operator->,Operator++,Operator–等相关操作予以重载的Class Template。所有STL容器都附带有自己专属的迭代器——是的，只有容器设计者才知道如何遍历自己的元素，原生指针（Native pointer）也是一种迭代器。

4、**仿函数**（Functors）： 行为类似函数，可作为算法的某种策略（Policy）,从实现的角度来看，**仿函数是一种重载了Operator()的Class 或 Class Template**。一般函数指针可视为狭义的仿函数。

5、**适配器**（Adapters）：一种用来修饰容器（Containers）或仿函数（Functors）或迭代器（Iterators）接口的东西，例如：STL提供的Queue和Stack，虽然看似容器，其实只能算是一种容器配接器，因为 它们的底部完全借助Deque，所有操作有底层的Deque供应。改变Functor接口者，称为Function Adapter;改变Container接口者，称为Container Adapter;改变Iterator接口者，称为Iterator Adapter。配接器的实现技术很难一言蔽之，必须逐一分析。

6、**分配器**（Allocators）：负责空间配置与管理，从实现的角度来看，配置器是一个实现了动态空间配置、空间管理、空间释放的Class Template。

### STL容器

1）序列式容器（Sequence containers），每个元素都有固定位置－－取决于插入时机和地点，和元素值无关，vector、deque、list；

Vectors：将元素置于一个动态数组中加以管理，可以随机存取元素（用索引直接存取），数组尾部添加或移除元素非常快速。但是在中部或头部安插元素比较费时；

Deques：是“double-ended queue”的缩写，可以随机存取元素（用索引直接存取），数组头部和尾部添加或移除元素都非常快速。但是在中部或头部安插元素比较费时；

Lists：双向链表，不提供随机存取（按顺序走到需存取的元素，O(n)），在任何位置上执行插入或删除动作都非常迅速，内部只需调整一下指针；

2）关联式容器（Associated containers），元素位置取决于特定的排序准则，和插入顺序无关，set、multiset、map、multimap；

Sets/Multisets：内部的元素依据其值自动排序，Set内的相同数值的元素只能出现一次，Multisets内可包含多个数值相同的元素，内部由二叉树实现（实际上基于红黑树(RB-tree）实现），便于查找；

Maps/Multimaps：Map的元素是成对的键值/实值，内部的元素依据其值自动排序，Map内的相同数值的元素只能出现一次，Multimaps内可包含多个数值相同的元素，内部由二叉树实现（实际上基于红黑树(RB-tree）实现），便于查找；

另外有其他容器hash_map,hash_set,hash_multiset,hash_multimap

### STL算法

```xml
STL算法部分主要由头文件<algorithm>,<numeric>,<functional>组成。要使用 STL中的算法函数必须包含头文件<algorithm>，对于数值算法须包含<numeric>，<functional>中则定义了一些模板类，用来声明函数对象。
```

STL中算法大致分为四类： 

1）、非可变序列算法：指不直接修改其所操作的容器内容的算法。 

2）、可变序列算法：指可以修改它们所操作的容器内容的算法。 

3）、排序算法：包括对序列进行排序和合并的算法、搜索算法以及有序序列上的集合操作。 

4）、**数值算法：对容器内容进行数值计算。**
### 适配器

STL提供了三个容器适配器：queue、priority_queue、stack。这些适配器都是包装了vector、list、deque中某个顺序容器的包装器。注意：适配器没有提供迭代器，也不能同时插入或删除多个元素




