# C#语法小知识（六）属性与索引器 - 凯奥斯的注释 - CSDN博客





2016年09月13日 14:54:31[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4303
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# 属性是一种成员，它提供灵活的机制来读取、写入或计算私有字段的值。属性可用作公共数据成员，但它们实际上是称为“访问器”的特殊方法。这使得可以轻松访问数据，还有助于提高方法的安全性和灵活性。

一个简单的示例：


```
public class Person
{
	private string _name = "No one";
	public Person(string name)
	{
		_name = name;
	}
	public string Name
	{
		get { return _name;}
	}
};
```


用法：


```
Person p = new Person ("Arya");
		System.Console.WriteLine (p.Name);
```


这里Name是一个只读的属性，如果要给Name赋值，那么就会报错。

那么就需要在Name里添加set访问器


```
public string Name
	{
		get { return _name;}
		set { _name = value;}
	}
```
value是set访问器的隐式参数，此参数的类型是属性的类型（string）。


用法：

`p.Name = "Erya";`


# 索引器允许类或结构的实例就像数组一样进行索引。索引器类似于属性，不同之处在于它们的取值函数采用参数。

在Person类里添加一个索引器：


```
public string this[string key]
	{
		get { if (key == "Name")
				return _name;
			return null;}
		set { if (key == "Name")
				_name = value;
			return;}
	}
```

跟属性一样，索引器可以只有set访问器或者get访问器，或者二者都有，不过，索引器可输入参数。
用法：


```
p ["Name"] = "Anonymous";
		System.Console.WriteLine (p ["Name"]);
```




# 属性和索引器的区别：


|属性|索引器|
|----|----|
|允许像调用公共数据成员一样调用方法。|允许对一个对象本身使用数组表示法来访问该对象内部集合中的元素。|
|可通过简单的名称进行访问。|可通过索引器进行访问。|
|可以为静态成员或实例成员。|必须为实例成员。|
|属性的 get 访问器没有参数。|索引器的 get 访问器具有与索引器相同的形参表。|
|属性的 set 访问器包含隐式 value 参数。|除了value参数外，索引器的 set 访问器还具有与索引器相同的形参表。|
|支持对自动实现的属性使用短语法。|不支持短语法。|



所谓自动实现属性的短语法，就是一个属性可以写成下面这种方式：

`public int Age { get;set;}`而这种写法妙用就在于可以写成这样：

`public int Age { get;private set;}`即对外只读，对内可写。

或者

`protected int RealAge{ private get; set;}`

即对自己或派生类可写，对自己可读。

对于属性和索引器，get和set至少要有一个不添加访问修饰符，可访问性与自身相同，而另一个可以添加低于自身的访问修饰符。









接着，为Person添加一个默认构造函数，然后添加一个派生类：


```
public class Coder : Person
{
	public new int Age {get{ return 16;}}
	public new string this[string key]
	{
		get {if (key == "Name")
			return "I don't know.";
			return null;}
	}
}
```

这里我们使用new关键字声明了一个同名的属性Age和同名的索引器。当我们调用这个属性和索引器时：


```
Coder c = new Coder();
		System.Console.WriteLine (c.Age);
		System.Console.WriteLine (c["Name"]);
```
会显示10和I don't know.




当然我们也可以时间倒流并找回自己的名字。


```
System.Console.WriteLine (((Person)c).Age);
		System.Console.WriteLine (((Person)c)["Name"]);
```
显示0和No one。




既然有了派生类，我们就可以写虚属性和虚索引器了。

例如在Person里添加：


```
public virtual bool Male {
		get;
		set;
	}

	public virtual bool this[bool noSense]
	{
		get { return noSense;}
	}
```

在Coder里面添加


```
public override bool Male
	{
		get { return true;}
		set { }
	}	

	public override bool this[bool noSense]
	{
		get { return !noSense;}
	}
```

~~恩，这个索引器确实没什么卵用。~~




注意：在重写属性或索引器时，被重写的访问器对重写代码而言，必须是可访问的。此外，属性或索引器和访问器的可访问性级别都必须与相应的被重写属性或索引器和访问器匹配。

重写虚属性或索引器的属性还可以是 sealed 的，这表示它对派生类不再是虚拟的。并且，可以将抽象类的属性或索引器声明为 abstract。这意味着类中没有任何实现，派生类必须编写自己的实现。




说了这么多，发现属性和索引器有很多相似的地方，那么就再加点不一样的。

例如：


```
static public string Planet
	{
		get { return "The Earth";}
	}
```
（如果哪天我们人类开启星际殖民，就要修改get并添加set了。）

而索引器无法是静态成员。




再例如：


```
public int this[int a, int b]
	{
		get { return a + b;}
	}
```

索引器可以有多个参数（属性连一个参数都没有……），参数类型也不受限于int和string。




# 最后补一发访问修饰符：

# public

# 同一程序集中的任何其他代码或引用该程序集的其他程序集都可以访问该类型或成员。

# private

# 只有同一类或结构中的代码可以访问该类型或成员。

# protected

# 只有同一类或结构或者此类的派生类中的代码才可以访问的类型或成员。

# internal同一程序集中的任何代码都可以访问该类型或成员，但其他程序集中的代码不可以。

# protected internal

# 由其声明的程序集或另一个程序集派生的类中任何代码都可访问的类型或成员。从另一个程序集进行访问必须在类声明中发生，该类声明派生自其中声明受保护的内部元素的类，并且必须通过派生的类类型的实例发生。






