# C#语法小知识（十二）匿名方法与Lambda表达式 - 凯奥斯的注释 - CSDN博客





2016年10月16日 21:06:00[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1077
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# [C#语法小知识（四）委托delegate](http://blog.csdn.net/ecidevilin/article/details/52486015)里介绍了如何创建delegate，委托方法可以用匿名方法或者Lambda表达式实现。

首先是**匿名方法**。

我们先声明一个委托类型。


`delegate void TestDelegate(int x);`

将代码块传递为委托参数，创建匿名方法则是唯一的方法。

```
TestDelegate td = delegate(int x)
			{
				Console.WriteLine(x);
			};
```


如果局部变量或参数的影响范围内包含一个匿名方法，则该局部变量或参数称为这个匿名方法的外部变量。例如，下面代码段中的 n 就是一个外部变量：





```
int n = 100;
		td += delegate(int x) {
			Console.WriteLine(x + n);
		};
```


外部变量的引用n是在创建委托时被捕获的。与本地变量不同，当匿名方法被垃圾回收的时候，外部变量才会被回收。









**Lambda**表达式是一种可用于创建委托或表达式树类型的匿名函数。（表达式树参考[MSDN](https://msdn.microsoft.com/zh-cn/library/bb397951.aspx)）

Lambda表达式有两种方式，表达式
 Lambda和语句Lambda。

表达式Lambda只有一句，而这一句的结果将作为返回值返回（如果有的话）。

声明一个delegate：


`delegate bool StrLenLargeThan(string str, int i);`使用表达式Lambda：




```
StrLenLargeThan sllt = (string str, int i) => str.Length > i;
		Console.WriteLine (sllt ("Lambda", 3));
```

当然你可以在表达式Lambda里面调用其他方法，例如：



`delegate void TestPrint(string str);`
使用表达式Lambda：




```
TestPrint tp = (string str) => Console.WriteLine (str);
		tp ("Expression");
```





语句Lambda
 与表达式 Lambda 类似，只不过语句被括在大括号中，语句Lambda
 的主体可以包含任意数量的语句。像匿名方法一样，语句 lambda 不能用于创建表达式树。


例如之前的StrLenLargeThan委托就可以添加这样语句语句Lambda：


```
sllt += (string str, int i) => {
			int len = str.Length;
			return len > i;
		};
```


Lambda
 与匿名方法的一般规则如下：

参数数量必须与委托类型的参数数量相同。

每个输入参数必须都能够隐式转换为对应的委托参数。

返回值（如果有）必须能够隐式转换为委托的返回类型。




下列规则适用于 Lambda 表达式和匿名方法中的变量范围：

捕获的变量将不会被作为垃圾回收，直至引用变量的委托符合垃圾回收的条件。

在外部方法中看不到方法块内引入的变量。

无法从封闭方法中直接捕获 **ref** 或 **out** 参数。

返回语句不会导致封闭方法返回。




匿名方法与Lambda的限制：

不能访问外部范围的 [ref](https://msdn.microsoft.com/zh-cn/library/14akc2c7.aspx) 或 [out](https://msdn.microsoft.com/zh-cn/library/t3c3bfhx.aspx) 参数。

在方法块中不能访问任何不安全代码。

在 [is](https://msdn.microsoft.com/zh-cn/library/scekt9xw.aspx) 或 [as](https://msdn.microsoft.com/zh-cn/library/cscsdfbt.aspx) 运算符的左侧不允许使用匿名方法或Lambda。






匿名方法不能访问外部范围的 [ref](https://msdn.microsoft.com/zh-cn/library/14akc2c7.aspx) 或 [out](https://msdn.microsoft.com/zh-cn/library/t3c3bfhx.aspx) 参数。

在匿名方法块中不能访问任何不安全代码。

在 [is](https://msdn.microsoft.com/zh-cn/library/scekt9xw.aspx) 或 [as](https://msdn.microsoft.com/zh-cn/library/cscsdfbt.aspx) 运算符的左侧不允许使用匿名方法。




