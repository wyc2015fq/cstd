# C#语法小知识（三）枚举类型enum - 凯奥斯的注释 - CSDN博客





2016年09月08日 22:19:37[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：6454
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# 枚举类型声明一系列常数，用于表示这个类型的变量可能会在这些常数里变化。

我们在这篇文章里讲一下枚举类型的几个用法。

一个简单的枚举类型的定义：


```
enum TestEnum
{
	x,
	y,
	z,
}
```

而使用也很简单：


```
TestEnum a = TestEnum.x;
		if (a == TestEnum.x) {
			//Do someting
		}
```

也经常跟switch case搭配使用：


```
TestEnum b = TestEnum.x + 10;
		switch (b) {
		case TestEnum.x:
			//do sth
			break;
		case TestEnum.y:
			//do sth
			break;
		case TestEnum.z:
			//do sth
			break;
		default:
			//do sth
			break;
		}
```

因为枚举类都继承自[System.Enum](https://msdn.microsoft.com/zh-cn/library/system.enum(v=vs.100).aspx)，而后者实现了与int,byte,short等整数类型的相互转换，所以可以写成这样：


```
TestEnum c = (TestEnum)10;
		int i = (int)c;
```
但是[System.Enum](https://msdn.microsoft.com/zh-cn/library/system.enum(v=vs.100).aspx)，只实现了枚举转float,double等浮点类型的接口，却没有实现浮点类型转枚举的接口，所以转换的时候会先转换成整形，再转换成枚举。


```
TestEnum d = (TestEnum)1.414f;
		float j = (float)d;
```
上面这段代码，打印d和j分别会显示y和3。


上面都是基本的用法，这里要说一些进阶的用法。

枚举作为位标志：


```
[System.Flags]
enum TestFlagEnum
{
	o = 0x0,
	x = 0x1,
	y = 0x2,
	z = 0x4,
}
```

用法就是为做位运算：

`TestFlagEnum e = TestFlagEnum.x | TestFlagEnum.y;//赋值`打印e，就会显示x,y。

如果要判定的话：

`bool isX = (e & TestEnum.x) == TestFlagEnum.x;`另外一种用法就是，获取枚举值的名称：

`string name = System.Enum.GetName (typeof(TestFlagEnum), 3);`

这里会name会是null，而如果把第二个参数改为2的话，name就会是y。

还有获取所有枚举值名称的方法：

`string[] enumStrings = System.Enum.GetNames (typeof(TestFlagEnum));`那么逆操作是怎么样的呢？

通过string获取枚举值：

`TestFlagEnum val = (TestFlagEnum)System.Enum.Parse (typeof(TestFlagEnum), "x");`但是如果把第二个参数改为一个不存在对应枚举值的string，便会报错，所以我们需要使用[TryParse](https://msdn.microsoft.com/zh-cn/library/dd783499(v=vs.100).aspx)方法，但是旧版本的.NET（例如Unity3D）并不支持这个方法，因此做这项操作的时候要格外的注意。



