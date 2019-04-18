# C#语法小知识（十八）const与readonly - 凯奥斯的注释 - CSDN博客





2016年10月29日 23:33:16[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：595
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# const和readonly都可以表示不可修改的字段，而const表示常量，readonly表示只读变量。

const常量只能在声明的时候初始化，并且不允许在常量声明中使用static修饰，因为它本身就是静态值，只能是类成员。

readonly只读变量可以在声明的时候或者构造函数里初始化，可以在只读变量声明时使用static修饰，所以它可以是类成员，也可以是对象成员。

此外，const是编译时常量，而readonly可以是运行时常量，例如：


`public static readonly uint timeStamp = (uint)System.DateTime.Now.Ticks;`
示例：


```
class Test
{
	public int a;
	public const int b = 0;
	public readonly int c = -1;

	public Test()
	{
		c = 1;
	}

	public Test(int p1, int p2)
	{
		a = p1;
		c = p2;
	}
}
```
实例化：


```
Test t1 = new Test (2, 3);
		Test t2 = new Test ();
		t2.a = 5;
		Console.WriteLine(string.Format("t1: a={0}, b={1}, c={2}", t1.a, Test.b, t1.c));//t1: a=2, b=0, c=3
		Console.WriteLine(string.Format("t2: a={0}, b={1}, c={2}", t2.a, Test.b, t2.c));//t2: a=5, b=0, c=1
```

注意：
1、可以通过构造函数覆盖readonly变量声明时初始化的值。

2、const常量初始化之后，就再也不能修改。

3、构造函数之外，不能修改readonly变量。

4、通过对象访问const常量（例如t1.b）是错误的。





