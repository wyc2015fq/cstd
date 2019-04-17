# Effective Java 3rd Edition（读书笔记+翻译）系列（第二章） - westbrook1998的博客 - CSDN博客





2018年03月08日 23:07:34[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：3508








## Chapter 2. Creating and Destroying Objects

这一章将介绍创建和销毁对象，包括何时以及如何创建对象，何时又将避免或者如何避免创建对象，如何确保对象被及时销毁，以及在销毁之前必须做的一些清理工作

### 第一点 考虑用静态工厂来代替构造器

例如Boolean类的valueOf方法就是一个装箱的静态方法

```java
public static Boolean valueOf(boolean b) {
return b ? Boolean.TRUE : Boolean.FALSE;
}
```

接收一个boolean基本类型返回一个Boolean对象

> 
Note that a static factory method is not the same as the Factory Method pattern 

  from Design Patterns [Gamma95]. The static factory method described in this 

  item has no direct equivalent in Design Patterns 

  这里静态工厂方法不同于设计模式中的工厂方法
同时，提供静态工厂方法代替构造器有优点也有缺点 

优点： 

 - **静态工厂方法可以有一个具有更强描述性的方法名称，更便于阅读**，如果使用构造器，通过使用不同参数列表创建不同构造器会导致api调用者难以确定需要哪一个构造器，或者弄错，而静态工厂方法有特有的方法名来区分不同的对象构造 

 - **静态工厂方法不用每次调用都创建一个新对象**，允许预构造或缓存并反复分配对象以避免重复构造，例如Boolean.valueOf方法 返回的Boolean.TRUE或者Boolean.FALSE 

 - **不同于构造器方法，静态工厂方法可以返回其返回类型的任意子类型对象，有更高的灵活性**，这种灵活性的一个应用就是比如可以返回不公开（public）的类实例，用这种方法隐藏实现类去达到一个紧凑的api接口，这种技术多用于基于接口（interface-base）的框架 。jdk8之后，允许接口有静态方法。***这段有点绕，暂时看不太懂***
```java
Boolean类源码
 /**
     * The {@code Boolean} object corresponding to the primitive
     * value {@code true}.
     */
    public static final Boolean TRUE = new Boolean(true);

    /**
     * The {@code Boolean} object corresponding to the primitive
     * value {@code false}.
     */
    public static final Boolean FALSE = new Boolean(false);
```






