# 原理性地理解 Java 泛型中的 extends、super 及 Kotlin 的协变、逆变 - 我的学习笔记 - CSDN博客





2018年09月28日 23:55:31[zouzhiheng](https://me.csdn.net/u011330638)阅读数：98








相信大多学习过 Java 的都对 extends 和 super 产生过疑惑，特别是坑爹的 PECS 原则，又是英文缩写，又是消费者生产者的，如此抽象的结论，是正常人应该记的吗？

吐槽完毕，回归正题，网上有许多文章对 extends 和 super 进行过各种解析，基本都能得出如下类似的结论：
- 

> 
extends 可用于返回类型限定，不能用于参数类型限定。

super 可用于参数类型限定，不能用于返回类型限定。

- 

> 
频繁往外读取内容的，适合用上界Extends。

经常往里插入的，适合用下界Super。


当然，以上两个结论和 PECS 原则都是正确的（废话）。

那么，我为什么还要写这一篇文章呢？因为我发现事情其实很简单，上述几个推论都基于一个很简单的原理，这个原理是我们学习 Java 面向对象时被反复提到过的：**父类引用可以指向子类对象，子类引用不能指向父类对象。**

根据这个原理，我们可以很轻松地推导出以上几个结论。

比如有如下几个类：

```
public class Plate<T> {

    private class Food {}
    private class Fruit extends Food {}
    private class Apple extends Fruit {}

    T item;
    public void set(T a) {
		item = a;
	}

    public T get() { return item;}

}
```

此时，下面两个 set 方法是否正确？

```
Plate<? super Fruit> plate1 = new Plate<>();
plate1.set(new Apple());
plate1.set(new Food());
```

答：

第一个 set 正确，第二个 set 错误。

原因：

plate1 里面装的可能是 Fruit 或 Fruit 的父类，具体是哪个类不能确定（如果非要用一个类表示，只能用 Object），但不管是什么类，都是 Apple 的父类，即这个 set 方法试图传递一个 Apple 对象给 Apple 的父类引用，而父类引用指向子类对象是合法的，因此正确。

而对于第二个 set，plate1 里面装的可能是 Fruit 类的引用，而 Fruit fruit = new Food() 是不合法的，即子类引用不能指向父类对象，因此错误。

对于 extends、super 和 set、get 的更多组合，就不一一解释了，可以看下面这份代码：

```
public class Plate<T> {


    private class Food {
    }

    private class Fruit extends Food {
    }

    private class Apple extends Fruit {
    }

    T item;

    public void set(T a) {
        item = a;
    }

    public T get() {
        return item;
    }

    public void test() {
        Plate<? super Fruit> plate1 = new Plate<>();
        // 正确，plate1 里面装的可能是 Fruit 或 Fruit 的父类，不管是什么，都是 Apple 的父类， 而父类引用指向子类对象是正确的
        plate1.set(new Apple());
        // 错误，plate1 里面装的可能是 Fruit 或 Fruit 的父类
        // 而子类引用指向父类对象是错误的，比如 Fruit item = new Food()
        //  plate1.set(new Food());

        Plate<? extends Fruit> plate2 = new Plate<>();
        // 错误，T 不知道是什么类型，可能是 Fruit 的最下面的某个子类，而子类引用指向父类对象是错误的
        // plate2.set(new Fruit());
        // 错误，虽然 Apple 现在是 Fruit 最下面的子类，但以后可能会增加 Apple 的子类，那时 T 就可能是 Apple 的子类，因此也是不合法的
        // plate2.set(new Apple());

        Plate<? extends Fruit> plate3 = new Plate<>();
        // 正确，plate3 装的都是 Fruit 对象或 Fruit 子类的对象，而不管是什么对象，都可以用 Fruit 引用指向它，父类引用指向子类对象是正确的
        Fruit fruit = plate3.get();
        // 错误，因为里面装的可能是 Fruit 或处于 Fruit 和 Apple 继承链中间的对象，而子类引用指向父类对象是错误的
        // Apple apple = plate3.get();

        Plate<? super Fruit> plate4 = new Plate<>();
        // 错误，plate1 里面装的可能是 Fruit 或 Fruit 的父类，也可能是 Food 的父类，比如 Object
        // Food food = plate4.get();
        // 正确，所有类都是 Object 的子类
        Object object = plate4.get();
    }
    
}
```

一次性看完这四种组合可能会让人有些晕，但记住最核心的一点即可：**父类引用可以指向子类对象，子类引用不能指向父类对象。** 以后再见到类似的，就能自行推导出来了。

了解过 Kotlin 的可能知道协变、逆变等概念，实际上是同一个道理，为什么协变只能用于 out 位置？因为 < out T> 相当于类型 T 或 T 的子类，当它作为返回值时，无论具体类型是什么，返回的都是一个 T 类型的对象或它的子类对象，类型 T 作为父类引用指向它是很合理的；当它用在 in 位置时，情况则相反，此时相当于要传递一个 T 类型的对象给它的子类类型的引用，而子类引用指向父类对象是不合法的。具体例子就不举了，理解了原理即可。



