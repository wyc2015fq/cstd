# C#语法小知识（十五）异常处理 - 凯奥斯的注释 - CSDN博客





2016年10月24日 22:19:14[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：901
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# 当程序会出现可以预知的错误时，我们就需要使用异常处理。C#里可以使用try、catch和finally关键字进行错误处理。

例如下面这段代码就是一个最简单的try catch代码块：



```
try
		{
			int b = 0;
			int a = 1 / b;
		}
		catch (System.Exception e) {
			Console.WriteLine (e.Message);
		}
```

打印：Division by zero。
我们对某些可以预知的错误，抛出自定义错误：



```
static double SafeDivision(double x, double y)
	{
		if (y == 0) {
			throw new System.Exception ("Divided By Zero!!!!");
		}
		return x / y;
	}
```

使用：


```
try
		{
			SafeDivision(1, 0);
		}
		catch (System.Exception e) {
			Console.WriteLine (e.Message);
		}
```

打印：Divided By Zero!!!!






异常具有以下特点：

各种类型的异常最终都是有System.Exception派生而来。

对于可能引发异常的语句，使用try语句。

一旦try代码块中引发异常，控制流会跳转到catch代码快中，如果没有定义catch代码块，程序会停止执行并报错。

除非你真的可以解决异常，否则请不要使用异常捕捉。

可以在catch里定义一个异常变量，可以从变量里获取更多信息，里面包含例如调用堆栈的状态和有关错误的文本说明。

使用throw可以显式的引发异常。

引发异常后可以执行finally代码块，可以在finally释放资源。






```
try {
			SafeDivision(1, 0);
		} finally {
			Console.WriteLine ("finally");
		}
```


例如上面这段代码，虽然程序依然会停止报错，但是在报错之前会先将finally打印出来。
当然我们要可以使用try-catch-finally语句：



```
try {
			SafeDivision(1, 0);
		} 
		catch (System.Exception e) {
			Console.WriteLine (e.Message);
		}
		finally {
			Console.WriteLine ("finally");
		}
		Console.WriteLine ("Testing");
```


引发异常之后会先后执行catch、finally和后面的语句。






