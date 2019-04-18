# C#语法小知识（一）Equals和==的区别 - 凯奥斯的注释 - CSDN博客





2016年09月06日 09:05:01[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：3254
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









之前因为某些原因，特意百度过这个问题，网上写这个的有很多，相互之间也会有冲突，但大部分都太浮于表面。

阐述一下我的理解，首先看一下二者的声明：


`public override bool Equals (object other);`
`public static bool operator == (object lhs, object rhs);`
*==的参数类型要换成对应的类型*。



那么区别就很明显了：

==是operator，它必须被声明为静态方法，所以不能是虚函数，无法实现多态。

Equals是非静态方法，所以可以被声明为虚函数，可以实现多态。

举个简单的例子：



```
System.Object o1 = 1;
System.Object o2 = 1;
```

当我们分别打印o1==o2和o1.Equals(o2)得到的是False和True。


通过MSDN[值类型](https://msdn.microsoft.com/zh-cn/library/s1ax56ch(v=vs.140).aspx)我们知道值类型继承自[System.ValueType](https://msdn.microsoft.com/zh-cn/library/system.valuetype.aspx)，而[System.ValueType](https://msdn.microsoft.com/zh-cn/library/system.valuetype.aspx)又继承自[System.Object](https://msdn.microsoft.com/zh-cn/library/system.object.aspx)。

因为==是静态方法，o1==o2只能调用System.Object的==方法，而这个方法比较的是引用地址是否相同。

而Equals是虚函数，所以o1.Equals(o2)实际上调用的是[int](https://msdn.microsoft.com/zh-cn/library/5kzh1b5w.aspx)（也就是[Int32](https://msdn.microsoft.com/zh-cn/library/system.int32.aspx)）的Equals方法，比较的是值是否相同。




本文作者尚为初学，如有理解不到位的地方，欢迎指正。



