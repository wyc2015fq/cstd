# Java之String、StringBuffer、StringBulider辨识 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月06日 15:58:55[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1078
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









1、总引：

当对字符串进行修改的时候，需要使用 StringBuffer 和 StringBuilder 类。

和 String 类不同的是，StringBuffer 和 StringBuilder 类的对象能够被多次的修改，并且不产生新的未使用对象。

StringBuilder 类在 Java 5 中被提出，它和 StringBuffer 之间的最大不同在于 StringBuilder 的方法不是线程安全的（不能同步访问）。

由于 StringBuilder 相较于 StringBuffer 有速度优势，所以多数情况下建议使用 StringBuilder 类。然而在应用程序要求线程安全的情况下，则必须使用 StringBuffer 类。


从这段话中，可以读出两点：

1）在修改自身对象时，String是不可变的，StringBuffer和StringBuilder是可变的；

2）同是可变下，StringBuffer是线程安全的（适用多线程），而StringBuilder则有速度优势（适用单线程）。

![](https://img-blog.csdn.net/20170706155355327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2、String和StringBuffer可变和不可变



```java
public static void main(String args[]){
	       String str = "hello";
	       StringBuffer strBuffer = new StringBuffer();
	       strBuffer.append(" world");
	       System.out.println(str.getClass() + "@" + str.hashCode());
	       System.out.println(strBuffer.getClass() + "@" + strBuffer.hashCode());


	       str = "jason";
	       strBuffer.append(" and jason");
	       System.out.println(str.getClass() + "@" + str.hashCode());
	       System.out.println(strBuffer.getClass() + "@" + strBuffer.hashCode());	
	}
```


结果：String的地址已改变，对象已经改变为另一个；StringBuffer地址不变，还是同样的对象。





```
class java.lang.String@99162322
class java.lang.StringBuffer@131204848
class java.lang.String@100897019
class java.lang.StringBuffer@131204848
```


3、适用场景

1）如果要操作少量的数据用 String； 

2）多线程操作字符串缓冲区下操作大量数据 StringBuffer； 

3）单线程操作字符串缓冲区下操作大量数据 StringBuilder。






