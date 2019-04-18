# C#语法小知识（十一）泛型 - 凯奥斯的注释 - CSDN博客





2016年10月16日 19:37:33[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：661
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# **泛型（Generic）** 允许您延迟编写类或方法中的编程元素的数据类型的规范，直到实际在程序中使用它的时候。换句话说，泛型允许您编写一个可以与任何数据类型一起工作的类或方法。

您可以通过数据类型的替代参数编写类或方法的规范。当编译器遇到类的构造函数或方法的函数调用时，它会生成代码来处理指定的数据类型。

泛型方法：



```
public class TestGeneric
{
	public static void Swap<T> (ref T t1, ref T t2)
	{
		T tmp;
		tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
}
```
定义一个类型用于测试：


```
public class Test
{
	public string Name { get; private set;}
	public Test(string name)
	{
		Name = name;
	}
}
```
使用：


```
public class TestGeneric
{
	public static void Swap<T> (ref T t1, ref T t2)
	{
		T tmp;
		tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
}
```
我们可以为泛型添加限制，例如定义一个ISwapable接口：

```
public interface ISwapable
{
}
```
在Swap方法后面添加where关键字：

`public static void Swap<T> (ref T t1, ref T t2) where T : ISwapable`
这时候编译器就会报错，因为Test类型和int类型没有继承自ISwapable，给Test添加ISwapable的继承，就可以对Test进行Swap了，但是int就没办法了。


我们还可以创建一个泛型类：


```
public class TestGenericClass<T>
{
	T[] _arr;
	public TestGenericClass(int len)
	{
		_arr = new T[len];
	}
	public T GetItem(int idx)
	{
		return _arr [idx];
	}
	public void SetItem(int idx, T val)
	{
		_arr [idx] = val;
	}
}
```
这是一个类似于List一样的类，但是功能很简单，甚至不能改变长度，但是没关系，我们只是用来做简单的示例。
（还可以参考[小话设计模式（一）单例模式](http://blog.csdn.net/ecidevilin/article/details/52595044)和[小话设计模式（二）工厂方法模式](http://blog.csdn.net/ecidevilin/article/details/52603869)）

使用：



```
TestGenericClass<string> tgc = new TestGenericClass<string> (10);
		tgc.SetItem (0, "Test");
		tgc.SetItem (2, "Generic");
		tgc.SetItem (4, "Class");
		for (int i = 0; i < 10; i++) {
			Console.WriteLine (tgc.GetItem (i));
		}
```

泛型委托：


```
public class TestGenericDelegate
{
	public delegate void GenericDelegate<T>(T t);
	public GenericDelegate<int> numberChanged { get; set;}
	public GenericDelegate<string> stringChanged { get; set;}
	public void ChangeNumber(int num)
	{
		numberChanged (num);
	}
	public void ChangeString (string str)
	{
		stringChanged (str);
	}
}
```
使用：



```
TestGenericDelegate tgd = new TestGenericDelegate ();
		tgd.numberChanged = (int num) => {
			Console.WriteLine (num); 
		};
		tgd.stringChanged = (string str) => {
			Console.WriteLine (str);
		};
		tgd.ChangeNumber (1024);
		tgd.ChangeString ("1k");
```

最后还有泛型接口：

```
public interface ITestGenericInterface<T>
{
	T AddT (T t1, T t2);
}

public class TestGenericInterface : ITestGenericInterface<int>, ITestGenericInterface<string>
{
	public int AddT (int t1, int t2)
	{
		return t1 + t2;
	}
	public string AddT (string t1, string t2)
	{
		return t1 + t2;
	}
}
```
使用：


```
</pre><pre name="code" class="csharp" style="font-size: 14px; color: rgb(51, 51, 51);">		TestGenericInterface tgi = new TestGenericInterface ();
		Console.WriteLine (tgi.AddT (1, 10));
		Console.WriteLine (tgi.AddT ("Hello ", "world"));
```

当然，对于以上这些泛型的应用，如果有需要的话，都可以支持多个泛型，用逗号隔开，例如：


```
public interface ITestMultiplyGenericInterface<T1, T2, T3>
{
}
```


使用泛型是一种增强程序功能的技术，具体表现在以下几个方面：
它有助于您最大限度地重用代码、保护类型的安全以及提高性能。

可以创建泛型集合类。.NET 框架类库在 *System.Collections.Generic* 命名空间中包含了一些新的泛型集合类。您可以使用这些泛型集合类来替代 *System.Collections* 中的集合类。

可以创建自己的泛型接口、泛型类、泛型方法、泛型事件和泛型委托。

可以对泛型类进行约束以访问特定数据类型的方法。

关于泛型数据类型中使用的类型的信息可在运行时通过使用反射获取。


















