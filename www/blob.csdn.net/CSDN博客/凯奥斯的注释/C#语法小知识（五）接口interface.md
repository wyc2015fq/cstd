# C#语法小知识（五）接口interface - 凯奥斯的注释 - CSDN博客





2016年09月10日 22:39:24[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1976
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)










接口具有下列属性：


    1、接口类似于抽象基类：继承接口的任何非抽象类型都必须实现接口的所有成员。


    2、不能直接实例化接口。


    3、接口可以包含事件、索引器、方法和属性。


    4、接口不包含方法的实现。


    5、类和结构可从多个接口继承。


    6、接口自身可从多个接口继承。


一个例子简单展示一下接口的属性：



```
interface StringChar
{
	char this [int index] {
		get;
	}
}

interface StringProperty
{
	string String {
		get;
		set;
	}
}

interface StringConcat<T,U> : StringChar, StringProperty
{
	T Concat(U u);
}

class StringEx : StringConcat<StringEx, StringEx>, StringConcat<StringEx, string>
{
	protected string _string;
	public StringEx(string str)
	{
		_string = str;
	}
	public string String
	{
		get { return _string;}
		set { _string = value;}
	}
	public StringEx Concat(StringEx se)
	{
		return Concat (se._string);
	}
	public StringEx Concat(string str)
	{
		string newStr = this._string + str;
		this._string = newStr;
		return this;
	}
	public char this[int index]
	{
		get { return _string.ToCharArray ()[index];}
	}
}
```


用法示例：





```
StringEx se0 = new StringEx ("Hello");
		StringEx se1 = new StringEx ("world");
		System.Console.WriteLine (se0.Concat(" ").Concat(se1).String);
		System.Console.Write (se0[3]);
```







