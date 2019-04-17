# C#语法小知识（十七）序列化与反序列化（Binary） - 凯奥斯的注释 - CSDN博客





2016年10月26日 08:01:12[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1173标签：[C#																[Serialize																[Deserialize																[Binary																[Formatter](https://so.csdn.net/so/search/s.do?q=Formatter&t=blog)
个人分类：[C#](https://blog.csdn.net/ecidevilin/article/category/6399640)

所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)](https://so.csdn.net/so/search/s.do?q=Binary&t=blog)




# 序列化和反序列化在编程中是一种非常常用的功能，[C#语法小知识（十六）序列化与反序列化（XML）](http://blog.csdn.net/ecidevilin/article/details/52925232)中我们学习了如何使用XML序列化和反序列化对象，本文介绍一下如何使用Binary（命名空间System.Runtime.Serialization.Formatters.Binary）来序列化和反序列化对象。

定义一个类型，用于序列化：



```
[System.Serializable]
public class Book
{
	public string title; 
	public int No;
	public Book(string title_, int no_)
	{
		title = title_;
		No = no_;
	}
	public Book()
	{
	}
}
```


需要为类型增加特性Serializable（关于特性参考[C#语法小知识（七）特性](http://blog.csdn.net/ecidevilin/article/details/52551302)）。
定义序列化和反序列化的方法：



```
public void WriteBinary()
	{
		Book book = new Book ("Binary", 3);
		System.IO.FileStream fs = new System.IO.FileStream ("binary.txt", System.IO.FileMode.OpenOrCreate);
		BinaryFormatter binaryFormatter = new BinaryFormatter ();
		binaryFormatter.Serialize (fs, book);
		fs.Close ();
	}

	public void ReadBinary()
	{
		System.IO.FileStream fs = new System.IO.FileStream ("binary.txt", System.IO.FileMode.Open);
		BinaryFormatter binaryFormatter = new BinaryFormatter ();
		Book book = binaryFormatter.Deserialize (fs) as Book;
		Console.WriteLine (book.title);
		Console.WriteLine(book.No);
		fs.Close ();
	}
```

使用：


```
WriteBinary ();
		ReadBinary ();
```
打印：
Binary

3




如果某些变量不需要被序列化，就需要为变量添加NonSerialized特性，例如：



```
[System.NonSerialized]
	public int No;
```

再次打印：
Binary

0




如果需要序列化多个不同类的对象，只需要逐个序列化，并逐个反序列化即可。

定义一个Album类：



```
[System.Serializable]
public class Album
{
	public string owner;
	public int count;
	public Album(string owner_, int count_)
	{
		owner = owner_;
		count = count_;
	}
	public Album()
	{
	}
}
```

序列化和反序列化方法：


```
public void WriteBinary2()
	{
		Book book = new Book ("Binary", 4);
		Album album = new Album ("Crystal", 22);
		System.IO.FileStream fs = new System.IO.FileStream ("binary.txt", System.IO.FileMode.OpenOrCreate);
		BinaryFormatter binaryFormatter = new BinaryFormatter ();
		binaryFormatter.Serialize (fs, book);
		binaryFormatter.Serialize (fs, album);
		fs.Close ();
	}

	public void ReadBinary2()
	{
		System.IO.FileStream fs = new System.IO.FileStream ("binary.txt", System.IO.FileMode.Open);
		BinaryFormatter binaryFormatter = new BinaryFormatter ();
		Book book = binaryFormatter.Deserialize (fs) as Book;
		Album album = binaryFormatter.Deserialize (fs) as Album;
		Console.WriteLine (book.title);
		Console.WriteLine(book.No);
		Console.WriteLine (album.owner);
		Console.WriteLine(album.count);
		fs.Close ();
	}
```




如果需要在序列化/反序列化过程中或者过程后，进行额外的操作，可以定义一些方法，并为它们添加指定的特性：



```
[System.Runtime.Serialization.OnSerializing]
	private void OnSerializing(System.Runtime.Serialization.StreamingContext context)
	{
		Console.WriteLine ("OnSerializing");
	}
	[System.Runtime.Serialization.OnSerialized]
	private void OnSerialized(System.Runtime.Serialization.StreamingContext context)
	{
		Console.WriteLine ("OnSerialized");
	}
	[System.Runtime.Serialization.OnDeserializing]
	private void OnDeserializing(System.Runtime.Serialization.StreamingContext context)
	{
		Console.WriteLine ("OnDeserializing");
	}
	[System.Runtime.Serialization.OnDeserialized]
	private void OnDeserialized(System.Runtime.Serialization.StreamingContext context)
	{
		Console.WriteLine ("OnDeserialized");
	}
```

方法名没有要求，只要添加指定的特性即可。
OnSerializing在对象序列化之前调用该方法。

OnSerialized在对象序列化之后调用该方法。

OnDeserializing在对象反序列化之前调用该方法。

OnDeserialized在对象反序列化之后调用该方法。

如何使用需要根据实际情况分析，这里就不做举例了。

除了这四个特性外，System.Runtime.Serialization命名空间下还有OptionalField特性，可以添加在变量上，指定序列化流中可以缺少对应字段。](https://so.csdn.net/so/search/s.do?q=Deserialize&t=blog)](https://so.csdn.net/so/search/s.do?q=Serialize&t=blog)](https://so.csdn.net/so/search/s.do?q=C#&t=blog)




