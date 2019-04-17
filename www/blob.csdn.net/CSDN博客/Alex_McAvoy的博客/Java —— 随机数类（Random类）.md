# Java —— 随机数类（Random类） - Alex_McAvoy的博客 - CSDN博客





2018年10月15日 23:40:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









虽然 Math 类中包含了获取随机数的方法 random()，但有时不方便，因此 Java 中提供了一更灵活的可获取随机数的 Random 类，其位于 java.lang 包下。


其构造方法如下

```java
public Random()
public Random(long seed)
```

如果想获取指定范围内的随机数，可以使用 nextInt(int m) 方法，该方法返回一个 0 到 m 之间并且包括 0 不包括 m 的随机数

```java
Random rd=new Random();
rd.nextInt();//获取一个随机数
rd.nextInt(3);//获取一个0~3之间的随机数
```

如果想获取一个随机的 boolean 值，可以使用 nextBoolean() 方法。

```java
Random rd=new Random();
rd.nextBoolean();
```



