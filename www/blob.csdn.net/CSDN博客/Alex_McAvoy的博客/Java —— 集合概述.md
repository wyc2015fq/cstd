# Java —— 集合概述 - Alex_McAvoy的博客 - CSDN博客





2018年10月16日 20:34:59[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

面向对象语言对事物的体现都是以对象的形式，为方便对多个对象的操作，就要对对象进行存储，但使用数组存储对象方面具有一些弊端，而 Java 集合就像一种容器，可以动态的把多个对象的引用放入容器中。

Java 集合类可以用于存储数量不等的多个对象，还可用于保存具有映射关系的关联数组。

# 【存储对象的方式】

存储对象有两种方式：数组、集合

1.创建对象数组实例

```java
Student[] stu=new Student[20];
stu[0]=new Student();
...
```

2.数组存储对象的特点

    1）一旦创建，长度不可变

    2）真实的数组存放的对象的个数不可知

    3）集合一般用来存放基本类型的数据

3.集合存储对象的特点

    1）集合的长度是可变的

    2）集合一般用来存放类对象的引用

# 【集合中的主要接口】 

在 java.util 包中提供了一些集合，常用的有：List、Set、Map 等，其 UML 图如下：

![](https://img-blog.csdn.net/20181023105054119?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



Collection 接口：Set、List 接口的父接口，通常情况下不被直接使用

List 接口：元素有序、可重复的集合

Set 接口：元素有序、不可重复的集合

Map 接口：具有映射关系键值对（key-value）的集合

Iterator 接口：提供一种方法访问一个容器对象中各个元素，而不关心该对象的内部细节

Comparable 接口：内比较器，支持自己和自己比较

Comparator 接口：外比较器，不支持自己和自己比较



