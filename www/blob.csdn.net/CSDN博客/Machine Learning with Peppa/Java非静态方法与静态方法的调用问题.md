# Java非静态方法与静态方法的调用问题 - Machine Learning with Peppa - CSDN博客





2018年04月30日 21:25:18[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：288标签：[Java																[编程																[面向对象](https://so.csdn.net/so/search/s.do?q=面向对象&t=blog)
个人分类：[编程之美：Java](https://blog.csdn.net/qq_39521554/article/category/7599598)

所属专栏：[JavaSE](https://blog.csdn.net/column/details/21699.html)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)




学过Java的同学都知道，非静态方法（不带static）可以访问静态方法（带static），但是反过来就不行，为什么呢？来看下面这个简单例子：

```java
public class test{
    public void static main(String args[]){
        method(); //会出错，提示你讲method方法改成静态的
        method2(); //调用方法正确
        new Test2().method(); //正确
    }
    public void method(){
        System.out.println("HelloWorld");
    }
    public static void method2(){
        System.out.println("HelloWorld");
    }
}
public class Test2{
    public void method(){
        System.out.println("HelloWorld");
    }
}
```


这个要从java的内存机制去分析，首先当你New 一个对象的时候，并不是先在堆中为对象开辟内存空间，而是先将类中的静态方法（带有static修饰的静态函数）的代码加载到一个叫做方法区的地方，然后再在堆内存中创建对象。

所以说静态方法会随着类的加载而被加载。当你new一个对象时，该对象存在于对内存中，this关键字一般指该对象，但是如果没有 new对象，而是通过类名调用该类的静态方法也可以。

程序最终都是在内存中执行，变量只有在内存中占有一席之地时才会被访问，类的静态成员（变态和方法）属于类本身，在类加载的时候就会分配内存，可以 通过类名直接去访问，非静态成员（变量和方法）属于类的对象，所以只有在类的对象禅师（创建实例）的时候才会分配内存，然后通过类的对象去访问。

在一个类的静态成员中去访问非静态成员之所以会出错是因为在类的非静态成员不存在的时候静态成员就已经存在了，访问一个内存中不存在的东西当然会出错。那类是什么时候被加载呢？在需要调用的时候被加载。

一般来说当类被实例化或者第一次调用这个类的静态方法/变量时，JVM就会根据类的初始化属性来进行加载，一般会按照如下3个顺序：

```java
static{}
{}
public class(){}
```

来进行加载，并且初始化（由上到下），当然在构造函数执行前，类本身的属性也会被初始化（比如类成员变量）](https://so.csdn.net/so/search/s.do?q=Java&t=blog)




