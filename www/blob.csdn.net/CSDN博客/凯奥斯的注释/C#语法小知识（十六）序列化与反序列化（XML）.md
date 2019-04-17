# C#语法小知识（十六）序列化与反序列化（XML） - 凯奥斯的注释 - CSDN博客





2016年10月25日 23:36:46[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：953
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# C#提供了两类序列化与反序列化的手段，一种是XmlSerializer（命名空间System.Xml.Serialization），另一种我们会在后续文章中介绍（参考[C#语法小知识（十七）序列化与反序列化（Binary）](http://blog.csdn.net/ecidevilin/article/details/52930522)）。

首先我们定义一个Book类，我们要在程序中对它进行序列化和反序列化：



```
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

实现序列化和反序列化的方法：




```
public void WriteXML()
	{
		Book book = new Book("xml", 1);
		XmlSerializer writer = new XmlSerializer(typeof(Book));
		System.IO.StreamWriter file = new System.IO.StreamWriter(@"xml.xml");
		writer.Serialize(file, book);
		file.Close();
	}
	public void ReadXML()
	{
		XmlSerializer reader = new XmlSerializer(typeof(Book));
		System.IO.StreamReader file = new System.IO.StreamReader(@"xml.xml");
		Book book = reader.Deserialize(file) as Book;

		Console.WriteLine(book.title);
		Console.WriteLine(book.No);

		file.Close();
	}
```

使用：




```
WriteXML ();
		ReadXML ();
```

打印：


xml

1

如果某些变量不需要序列化，可以为这个变量增加特性XmlIgnore（关于特性参考[C#语法小知识（七）特性](http://blog.csdn.net/ecidevilin/article/details/52551302)）。

例如我们在为No变量增加特性：



```
[XmlIgnore]
	public int No;
```

那么打印出来的结果就是：


xml

0


xml序列化非常易用且好用，而且我们可以看到序列化之后的文件：



```
<?xml version="1.0" encoding="utf-8"?>
<Book xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <title>xml</title>
</Book>
```


但是如果要在一个文件中序列化多个不同类的对象，那么就会麻烦一点。


例如我们定义了一个Album类：



```
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

为了一个文件中同时序列化Book和Album的实例，我们就需要再额外定义一个包含Book和Album变量的类型：




```
public class Shelf
{
	public Book book;
	public Album album;
}
```
定义序列化和反序列化方法：




```
public void WriteShelfXML()
	{
		Shelf shelf = new Shelf ();
		shelf.book = new Book ("shelf_xml", 2);
		shelf.album = new Album ("Jessica", 27);
		XmlSerializer writer = new XmlSerializer(typeof(Shelf));
		System.IO.StreamWriter file = new System.IO.StreamWriter(@"shelf_xml.xml");
		writer.Serialize(file, shelf);
		file.Close();
	}
	public void ReadShelfXML()
	{
		XmlSerializer reader = new XmlSerializer(typeof(Shelf));
		System.IO.StreamReader file = new System.IO.StreamReader(@"shelf_xml.xml");
		Shelf shelf = reader.Deserialize(file) as Shelf;

		Console.WriteLine(shelf.book.title);
		Console.WriteLine(shelf.book.No);
		Console.WriteLine(shelf.album.owner);
		Console.WriteLine(shelf.album.count);

		file.Close();
	}
```

xml文件：




```
<?xml version="1.0" encoding="utf-8"?>
<Shelf xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <book>
    <title>shelf_xml</title>
  </book>
  <album>
    <owner>Jessica</owner>
  </album>
</Shelf>
```











