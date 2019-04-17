# C#语法小知识（二）struct和class的区别 - 凯奥斯的注释 - CSDN博客





2016年09月07日 01:29:09[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1967
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









我们知道C++里面struct和class唯一的区别在于：struct的缺省访问修饰符为public，而class的缺省访问修饰符为private。

那么C#里面，这二者有什么区别呢？

直接引用MSDN[结构](https://msdn.microsoft.com/zh-cn/library/saxz13w4.aspx)，随手加上序号






结构与类共享大多数相同的语法，但结构比类受到的限制更多：
- 
1、在结构声明中，除非字段被声明为 const 或 static，否则无法初始化。

- 
2、结构不能声明默认构造函数（没有参数的构造函数）或析构函数。

- 
3、结构在赋值时进行复制。将结构赋值给新变量时，将复制所有数据，并且对新副本所做的任何修改不会更改原始副本的数据。在使用值类型的集合（如 Dictionary<string, myStruct>）时，请务必记住这一点。

- 
4、结构是值类型，而类是引用类型。

- 
5、与类不同，结构的实例化可以不使用 **new** 运算符。

- 
6、结构可以声明带参数的构造函数。

- 
7、一个结构不能从另一个结构或类继承，而且不能作为一个类的基。所有结构都直接继承自 **System.ValueType**，后者继承自 **System.Object**。

- 
8、结构可以实现接口。

- 
9、结构可用作可以为 null 的类型，因而可向其赋 null 值。


这里简单讲解一下第三条



```
int a = 0;
int b = a;
b++;
```


上面一段代码把a的值赋给了b，然后b++，那么b=1，但不影响a的值，a=0。



结构作为值类型，这一点就跟int、float等内置[值类型](https://msdn.microsoft.com/zh-cn/library/s1ax56ch(v=vs.140).aspx)保持一致。

而最后一点，我用的（Unity3D）.Net的版本是 2.0 Subset，并不支持赋值为null。

[上一篇中，我们讲到了==与Equals的区别](http://blog.csdn.net/ecidevilin/article/details/52446664)，那么就借此试验一下。

首先创建一个struct



```
struct TestStruct
	{
		public int a;
		public int b;
		public TestStruct(int _a, int _b)
		{
			a = _a;
			b = _b;
		}
	}
```


声明两个变量并赋相同的值





```
TestStruct ta = new TestStruct (1, 2);
	TestStruct tb = new TestStruct (1, 2);
```


当打印ta==tb的时候会报错，因为并没有实现operator ==的方法。

当打印ta.Equals(tb)的时候，显示true。虽然没有在struct里面实现，却是继承自[System.ValueType](https://msdn.microsoft.com/zh-cn/library/system.valuetype.aspx)的方法。



