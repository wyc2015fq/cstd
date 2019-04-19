# jquery源码学习笔记一：总体结构 - 左直拳的马桶_日用桶 - CSDN博客
2016年05月26日 07:27:11[左直拳](https://me.csdn.net/leftfist)阅读数：5812
练武不练功，到老一场空。计算机也一样。
计算机的功，就是原理。如果程序员只会使用各种函数，各种框架，而不知其原理，顶多熟练工人而已。知其然，更要知其所以然。
jquery我们用得很爽，但它究竟咋实现的？
1、首先，jquery就是一些javascript。 
而且完全就是一些原始的javascript，没有用其他第三方的库或什么的。它本身就存放在一个js文件里。
我们常常张嘴就说：
> 
jquery是一个javascript框架
哦，可不是随便说说而已。
2、总体结构 
javascript里，基本元素就是函数。从大的方面说，整个jquery，就是一个javascript的函数，以jquery-1.12.3为例，其总体结构如下：
```java
(function( global, factory ) {
    //调用factory(工厂)生成并返回jQuery实例
}(global实参, factory实参));
```
这个巨大的函数，声明并运行，最终目的只有一个，就是生成并返回一个jQuery实例（就是那个美元符,在这里，美元符是一个函数实例！）。
而源码的绝大部分，也就是主体部分，放在 factory实参 这里。这个 factory实参，是一个巨大无比的匿名函数。
再放大一点看看：
```java
(function( global, factory ) {
    //调用factory(工厂)生成jQuery实例
    factory( global );
}(typeof window !== "undefined" ? window : this, function( window, noGlobal ) {
    //factory实现，jquery源码主体部分
}));
```
！
