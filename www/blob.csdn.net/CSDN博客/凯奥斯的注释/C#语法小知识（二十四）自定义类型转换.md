# C#语法小知识（二十四）自定义类型转换 - 凯奥斯的注释 - CSDN博客





2016年11月30日 22:48:45[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：689
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# C#使用implicit和explicit来自定义类型转换运算符。






示例：



```
public class IntPoint
{
	int x;
	int y;
	public IntPoint(int x_, int y_)
	{
		x = x_;
		y = y_;
	}
	public static explicit operator int(IntPoint ip)
	{
		return (int)System.Math.Sqrt (ip.x * ip.x + ip.y * ip.y);
	}
	public static implicit operator double(IntPoint ip)
	{
		return System.Math.Sqrt (ip.x * ip.x + ip.y * ip.y);
	}
}
```

我们定义了一个二维整型点，并重载了int的强制类型转换运算符和double的隐式类型转换运算符。


使用：



```
IntPoint ip = new IntPoint (3, 7);
		int i = (int)ip;
		Console.WriteLine (i);
		double d = ip;
		float f = (float)ip;
		Console.WriteLine (d);
		Console.WriteLine (f);
```

这里虽然没有重载float的类型转换运算符，但是因为int可以隐式的转换为float，所以这里也可以将ip强制转换为float。


打印出来的结果是：

7

7.61577310586391


7




（关于运算符重载，参考[C#语法小知识（二十三）重载运算符](http://blog.csdn.net/ecidevilin/article/details/53401006)）




