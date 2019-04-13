
# incompatible types: ArrayList cannot be converted to List - lin_tuer的博客 - CSDN博客


2018年01月25日 10:04:54[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：1171


这个错误出现在我试图用一个 ArrayList<ArrayList<Integer>>() new 一个 List<List<Integer>> 对象的时候


```java
List<List<Integer>> = new ArrayList<ArrayList<Integer>>();
```

也许我们会发现 把第二个 ArrayList 改成 List ，错误就没有了，那么原理是什么呢？

经查找发现，这是一个 泛型 应用的常踩坑：
# Generics, Inheritance, and Subtypes
https://docs.oracle.com/javase/tutorial/java/generics/inheritance.html

1. 首先，如果 A is a B , 我们可以把 A 赋值给 B
Object someObject = new Object();
Integer someInteger = new Integer(10);
someObject = someInteger;   // OKInteger 继承自 Object，是它的一个子类型, 所以这样赋值没有问题，这是泛型。

2. Integer 也是一种 Number， Double 也是一种 Number，所以下面这样也是可以的
public void someMethod(Number n) { /* ... */ }
someMethod(new Integer(10));   // OK
someMethod(new Double(10.1));   // OK也可以使用泛型：
Box<Number> box = new Box<Number>();
box.add(new Integer(10));   // OK
box.add(new Double(10.1));  // OK
3. 重点来了
public void boxTest(Box<Number> n) { /* ... */ }如果这样，我们可以传入 Box<Integer> 或者 Box<Double> 吗
答案是否定的。
Integer 是 Number 的子类，Double 也是 Number 的子类， 但是，Box<Integer> 和Box<Double>
 都不是 Box<Integer> 的子类，它们的关系是并列的，都是 Object 的子类。

![](https://img-blog.csdn.net/20180125100354153?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGluX3R1ZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



