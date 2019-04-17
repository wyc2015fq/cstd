# C#语法小知识（二十一）ref与out - 凯奥斯的注释 - CSDN博客





2016年11月26日 22:00:31[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：839
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# C#只能有一个返回值，当我们需要一个方法返回多个返回值的时候，可以为方法添加一个ref或者out修饰的参数。

例如：



```
public bool TestRef(ref int i)
	{
		i = i + 44;
		return true;
	}
	public bool TestOut(out int i)
	{
		i = 55;
		return false;
	}
```

使用：



```
int i = 0;
		TestRef (ref i);
		Console.WriteLine (i);
		TestOut (out i);
		Console.WriteLine (i);
```

ref与out区别在于，ref的实参必须先初始化才能传递，而out的实参不需要初始化。而out的形参必须当做未初始化的值来处理（例如上面的例子不能写成i+=55），并且在返回之前必须为该参数赋值。



ref和out并不作为编译时方法签名的一部分，所以下面这样是编译不过的：



```
class TestRefOut
{
	public void SampleMethod(out int i) { i = 1; }
	public void SampleMethod(ref int i) { i = 2; }
}
```

但是如果其中一个方法去掉out或者ref，那么便是可编译通过的。



注意：不能将ref和out用于异步方法（通过async修饰符定义）和迭代器方法（包含yield return和yield break语句）。






