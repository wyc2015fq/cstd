# java集合类概述 - 博客堂 - CSDN博客





2017年03月25日 15:08:37[最小森林](https://me.csdn.net/u012052268)阅读数：353








# 集合类概述

目录：


- [集合类概述](#集合类概述)- [集合的由来](#集合的由来)- [由对象数组引入集合的概念](#由对象数组引入集合的概念)
- [数组和集合的区别](#数组和集合的区别)

- [集合的体系结构](#集合的体系结构)
- [Collection的功能概述](#collection的功能概述)
- [代码实现](#代码实现)




## 集合的由来：

### **由对象数组引入集合的概念：**

我们学习的是面向对象语言，而面向对象语言对事物的描述是通过**对象**体现的，为了方便对多个对象进行操作，我们就必须把这**多个对象进行存储**。

而要想存储多个对象，就不能是一个基本的变量，而应该是一个容器类型的变量，在我们目前所学过的知识里面，有哪些是容器类型的呢?

**数组**和**StringBuffer**。但是呢?StringBuffer的结果是一个字符串，不一定满足我们的要求，所以我们只能选择数组，这就是**对象数组**。

而对象数组又不能适应变化的需求，因为数组的长度是固定的，这个时候，为了适应变化的需求，Java就提供了集合类供我们使用。

### 数组和集合的区别?
- 
长度区别：

数组的长度固定     集合长度可变

- 
内容不同：

数组存储的是同一种类型的元素  而集合可以存储不同类型的元素

- 
元素的数据类型问题：   

数组可以存储基本数据类型，也可以存储引用数据类型。    –  集合只能存储引用类型


## 集合的体系结构

由于需求不同，Java就提供了不同的集合类。

这多个集合类的数据结构不同，但是它们都是要提供存储和遍历功能的，我们把它们的共性不断的向上提取，最终就形成了集合的继承体系结构图。

如下图：

![集合的继承体系图解.bmp](https://ooo.0o0.ooo/2017/03/22/58d1cffaeb2c0.bmp)

## Collection的功能概述：

1：添加功能

```
boolean add(Object obj):添加一个元素
boolean addAll(Collection c):添加一个集合的元素
```

2:删除功能

```java
void clear():移除所有元素
boolean remove(Object o):移除一个元素
boolean removeAll(Collection c):移除一个集合的元素(是一个还是所有)
```

3:判断功能

```
boolean contains(Object o)：判断集合中是否包含指定的元素
boolean containsAll(Collection c)：判断集合中是否包含指定的集合元素(是一个还是所有)
boolean isEmpty()：判断集合是否为空
```

4:获取功能(**重点**)
`Iterator<E> iterator()`
5:长度功能

```
int size():元素的个数
面试题：数组有没有length()方法呢?字符串有没有length()方法呢?集合有没有length()方法呢?
```

6:交集功能

```java
boolean retainAll(Collection c):两个集合都有的元素?思考元素去哪了，返回的boolean又是什么意思呢?
//假设有两个集合A，B。
//A对B做交集，最终的结果保存在A中，B不变。
//返回值表示的是A是否发生过改变。
```

7：把集合转换为数组
`Object[] toArray()`
## 代码实现

```java
public class IteratorDemo {
    public static void main(String[] args) {
        // 创建集合对象
        Collection c = new ArrayList();

        c.add("hello");
        c.add("world");
        c.add("java");

        // Iterator iterator():迭代器，集合的专用遍历方式
        Iterator it = c.iterator(); // 实际返回的肯定是子类对象，这里是多态(注意：c是一个集合对象)

        while (it.hasNext()) {
            // System.out.println(it.next());
            String s = (String) it.next();
            System.out.println(s);
        }
    }
}
```

总结集合的使用步骤：
- 创建集合对象
- 创建元素对象
- 把元素添加到集合中
- 遍历集合 
- 通过集合对象获取迭代器对象
- 迭代器对象hasNext（）方法判断
- 迭代器对象的next（）方法获取，并移动到下一个位置




