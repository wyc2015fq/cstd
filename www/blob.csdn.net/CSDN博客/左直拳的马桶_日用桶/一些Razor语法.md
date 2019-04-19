# 一些Razor语法 - 左直拳的马桶_日用桶 - CSDN博客
2015年02月12日 23:23:10[左直拳](https://me.csdn.net/leftfist)阅读数：1456
**Layout**
asp.net mvc中的一些子视图忽然不行了，点击主视图后发现没有弹出来。
通过浏览器调试，发现打开子视图时，加载了大量的JS，CSS等。真奇怪啊，这些都是在主视图加载的啊，怎么子视图也会加载呢？
原来，没有在子视图将布局清掉。
在子视图开头加上一句：
@{ Layout = null;}，问题解决。
看来，Layout也是视图的属性。
**@Model**
类似这样的语句：@model MvcApplication1.Models.Product
意思是将本视图的Model属性的类型指向MvcApplication1.Models.Product类型，这就实现了强类型。强类型的好处之一是类型安全，如果写错了Model对象的某个成员名，编译器会报错。
Model是视图的属性。
**@:**
类似 @:纯文本 ，后面输出的是纯文本，区别于带标记的html。
如果要输出@符号，当@符号前后都有非敏感字符（如<、｛、和空格等）时，可以直接使用@符号，否则需要使用两个@符号。
参考文章：
[http://www.cnblogs.com/willick/p/3224144.html](http://www.cnblogs.com/willick/p/3224144.html)
