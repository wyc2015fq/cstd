# C#语法小知识（八）扩展 - 凯奥斯的注释 - CSDN博客





2016年09月23日 18:47:34[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1218标签：[c#																[扩展																[Extension																[Extend																[方法](https://so.csdn.net/so/search/s.do?q=方法&t=blog)
个人分类：[C#](https://blog.csdn.net/ecidevilin/article/category/6399640)

所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)](https://so.csdn.net/so/search/s.do?q=Extend&t=blog)




# 扩展方法使你能够向现有类型“添加”方法，而无需创建新的派生类型、重新编译或以其他方式修改原始类型。扩展方法是一种特殊的静态方法，但可以像普通方法一样进行调用。对于用 C#，调用扩展方法与调用在类型中实际定义的方法之间没有明显的差异。

示例，为Int32（也就是int）添加扩展方法：



```
using System;

public static class Int32Extensions
{
	public static Int32 Digit(this Int32 num)
	{
		return (Int32)Math.Log10 (num) + 1;
	}

	public static Int32 bit(this Int32 num)
	{
		return (Int32)(Math.Log (num) / Math.Log(2)) + 1;
	}
}
```
扩展方法被定义为静态方法，但当它被调用的时候，使用的是示例方法的语法。第一个参数指定该方法作用于（被添加到）哪个类型，并且该参数以 [this](https://msdn.microsoft.com/zh-cn/library/dk1507sz.aspx)修饰符为前缀。仅当使用 **using** 指令将命名空间（本文示例没有namespace，就不需要使用**using**）显式导入到源代码中之后，扩展方法才可以使用。

使用：


```
int i = 4020;
		Console.WriteLine (i.Digit ());
		Console.WriteLine (i.bit ());
```

编译器生成的中间语言 (IL) 会将把调用代码转换为对静态方法进行调用。因此，并未真正违反封装原则。实际上，扩展方法无法访问它们所扩展的类型中的私有变量。




通常，建议只在不得已的情况下才实现扩展方法，并谨慎地实现。 必须要注意的是：如果扩展方法与该类型中定义的方法具有相同的签名，则扩展方法永远不会被调用。






## 总结：
- 

定义一个静态[类](https://msdn.microsoft.com/zh-cn/library/79b3xss3.aspx)以包含扩展方法。该类必须对客户端代码可见。有关可访问性规则的更多信息，请参见[访问修饰符（C#
 编程指南）](https://msdn.microsoft.com/zh-cn/library/ms173121.aspx)。


- 

将该扩展方法实现为静态方法，并使其至少具有与被扩展类型相同的可见性。


- 

该方法的第一个参数指定方法所操作的类型；该参数必须以 [this](https://msdn.microsoft.com/zh-cn/library/dk1507sz.aspx) 修饰符开头。


- 

在调用代码中，添加一条 **using** 指令以指定包含扩展方法类的[命名空间](https://msdn.microsoft.com/zh-cn/library/z2kcy19k.aspx)。


- 

按照与实例方法一样的语法调用扩展方法。](https://so.csdn.net/so/search/s.do?q=Extension&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)




