# Java中List基本用法 - 博客堂 - CSDN博客





2017年03月25日 19:24:45[最小森林](https://me.csdn.net/u012052268)阅读数：296








# List概述


- [List概述](#list概述)- [基本用法](#基本用法)
- [List集合的特有功能](#list集合的特有功能)




## 基本用法

List是Collection的一大分支。主要特点是有索引（index），可重复。

正是因为List中的元素是有序排列的而且可重复，所以被称为是序列。

![集合的继承体系图解.bmp](https://ooo.0o0.ooo/2017/03/22/58d1cffaeb2c0.bmp)

## List集合的特有功能

```
A:添加功能
    void add(int index,Object element):在指定位置添加元素
B:获取功能
    Object get(int index):获取指定位置的元素
C:列表迭代器
    ListIterator listIterator()：List集合特有的迭代器
D:删除功能
    Object remove(int index)：根据索引删除元素,返回被删除的元素
E:修改功能
    Object set(int index,Object element):根据索引修改元素，返回被修饰的元素
```

set的用法：

```
System.out.println("set:" + list.set(1, "javaee"));

System.out.println("list:" + list);
```

因为有index说以可以改进遍历方法：

```
for (int x = 0; x < list.size(); x++) {

        String s = (String)list.get(x);

        System.out.println(s);
```



