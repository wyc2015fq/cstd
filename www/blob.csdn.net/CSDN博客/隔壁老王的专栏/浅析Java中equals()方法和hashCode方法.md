# 浅析Java中equals()方法和hashCode方法 - 隔壁老王的专栏 - CSDN博客





2017年10月16日 22:59:49[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：739








今天去面试，总觉得自己什么都懂了，应该问题不大，当被问到“未重写的equals()方法和hashCode()方法里面的内容是什么？为什么在重写equals()方法的同时需要重写hashCode()方法?”时，说实话，一脸懵逼啊！！！

### 一、equals()方法

#### 1、什么时候应该覆盖Object.equals()方法？


原生的equals方法：

public boolean equals(Object obj) {
    return (this == obj);
}

从代码中可以看出，原生的equals方法使用的是“==”来比较的。学过Java的人都应该知道，“==”比较的是内存地址，所以原生的equals方法只有在自己与自己比较的时候才会返回true，是严格的判断一个对象是否相等的方法。所以，如果类具有自己特有的“逻辑相等”概念（不同于对象相等的概念），而且超类没有覆盖equals()方法以实现期望的行为，这时我们就需要覆盖equals()方法了（通俗来说，就是在业务系统中，有时候需要的并不是一种严格意义上的相等，而是业务上的对象相等。比如：如果两个对象中的id相等，那么就认为这两个对象是相等的）。

#### 2、覆盖equals方法时，需要遵守的通用约定


- 自反性


> 对于任意非null的引用值x，x.equals(x)必须返回true。

- 对称性

> 
对于任意非null的引用值x、y，当且仅当y.equals(x)返回true时，x.equals(y)必须返回true。

- 一致性

> 
对于任意非null的引用值x、y，只要equals方法的比较操作在对象中所用的信息没有发生改变，那么多次调用x.equals(y)应该一致的返回true或false。

- 传递性

> 
对于任意非null的引用值x、y、z，如果x.equals(y)返回true，并且y.equals(z)返回true，那么x.equals(z)必须返回true。

- 对于任意非null的引用值x，x.equals(null)必须返回false。

二、hashCode()方法

原生的hashCode方法：

public native int hashCode();

在回答为什么重写了equals()方法时需要同时重写hashCode()方法之前，我们先来看一下hashCode()方法在Object规范中的通用约定：

- 在应用运行期间，只要对象的equals方法的比较操作所用到的信息没有被修改，那么多次调用该对象的equals方法应该始终如一的返回同一个整数。在同一个应用程序的多次执行过程中，每次执行equals方法所返回的整数可以不一致。
- 如果两个对象使用equals(Object)方法比较是相等的，那么调用这两个对象中的任意一个对象的hashCode方法都必须产生相同的一个整数结果。
- 如果两个对象使用equals(Object)方法比较是不相等的，那么调用这两个对象中的任意一个对象的hashCode方法，则不一定要产生不同的整数结果。如果给不同的对象产生不同的hash码，有可能提高散列表性能（比如往HashMap中添加数据时，具体添加到哪个桶中，就是根据(table.length - 1) & hash来计算的）。


所以，根据以上的hashCode通用约定我们就可以知道：如果重写了equals方法而没有重新hashCode方法的话，将会违反hashCode通用约定的第二个约束条件：**相等的对象必须具有相等的散列码**。

根据类的equals方法，两个截然不同的实例在逻辑上有可能是相等的，但是根据Object类的hashCode方法，他们仅仅是两个没有任何共同之处的对象。因此，对象的hashCode方法会返回两个看起来是随机的整数，而不是根据第二个约定所要求的那样，返回两个相等的整数值。

































