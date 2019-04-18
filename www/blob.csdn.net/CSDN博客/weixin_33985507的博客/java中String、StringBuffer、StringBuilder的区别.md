# java中String、StringBuffer、StringBuilder的区别 - weixin_33985507的博客 - CSDN博客
2017年10月15日 21:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
本文部分转自于:[http://www.cnblogs.com/xudong-bupt/p/3961159.html](http://www.cnblogs.com/xudong-bupt/p/3961159.html)
# **1.可变与不可变**
String类中使用字符数组保存字符串，如下就是，因为有“final”修饰符，所以可以知道string对象是不可变的。
**private final char value[];**
**附录部分String类的源码:**
```
public final class String
    implements java.io.Serializable, Comparable<String>, CharSequence {
    /** The value is used for character storage. */
    private final char value[];
    /** Cache the hash code for the string */
    private int hash; // Default to 0
    。。。。。。。。。。。。
}
```
StringBuilder与StringBuffer都继承自AbstractStringBuilder类，在AbstractStringBuilder中也是使用字符数组保存字符串，如下就是，可知这两种对象都是可变的。
**　　　　char[] value;**
附录StringBulider,StringBuffer,AbstractStringBuilder的源代码
```
public final class StringBuffer
    extends AbstractStringBuilder
    implements java.io.Serializable, CharSequence
{
。。。。。。。。。。。。。。。。。。
}
public final class StringBuilder
    extends AbstractStringBuilder
    implements java.io.Serializable, CharSequence
{
。。。。。。。。。。。。。。。。。。
}
abstract class AbstractStringBuilder implements Appendable, CharSequence {
    /**
     * The value is used for character storage.
     */
    char[] value;
    /**
     * The count is the number of characters used.
     */
    int count;
。。。。。。。。。。。。。。。。。。
}
```
# **2.是否多线程安全**
　　String中的对象是不可变的，也就可以理解为常量，**显然线程安全**。
　　AbstractStringBuilder是StringBuilder与StringBuffer的公共父类，定义了一些字符串的基本操作，如expandCapacity、append、insert、indexOf等公共方法。
　　StringBuffer对方法加了同步锁或者对调用的方法加了同步锁，所以是**线程安全的**。看如下源码
　　StringBuffer中的一段源代码:
```
/**
     * @throws NullPointerException {@inheritDoc}
     * @since      1.4
     */
    public synchronized int lastIndexOf(String str, int fromIndex) {
        return String.lastIndexOf(value, 0, count,
                              str.toCharArray(), 0, str.length(), fromIndex);
    }
    /**
     * @since   JDK1.0.2
     */
    public synchronized StringBuffer reverse() {
        super.reverse();
        return this;
    }
    public synchronized String toString() {
        return new String(value, 0, count);
    }
```
　　对应相同方法StringBuilder中的源码:
```
/**
     * @throws NullPointerException {@inheritDoc}
     */
    public int lastIndexOf(String str, int fromIndex) {
        return String.lastIndexOf(value, 0, count,
                              str.toCharArray(), 0, str.length(), fromIndex);
    }
    public StringBuilder reverse() {
        super.reverse();
        return this;
    }
    public String toString() {
        // Create a copy, don't share the array
        return new String(value, 0, count);
    }
```
 StringBuilder并没有对方法进行加同步锁，所以是**非线程安全的**。
# ** 3.StringBuilder与StringBuffer共同点**
　　StringBuilder与StringBuffer有公共父类AbstractStringBuilder(**抽象类**)。
　　抽象类与接口的其中一个区别是：抽象类中可以定义一些子类的公共方法，子类只需要增加新的功能，不需要重复写已经存在的方法；而接口中只是对方法的申明和常量的定义。
　　StringBuilder、StringBuffer的方法都会调用AbstractStringBuilder中的公共方法，如super.append(...)。只是StringBuffer会在方法上加synchronized关键字，进行同步。
**　　最后，如果程序不是多线程的，那么使用StringBuilder效率高于StringBuffer**
