# 编程小知识之 C# indexer 和 property - tkokof1的专栏 - CSDN博客

2018年11月02日 20:36:50[tkokof1](https://me.csdn.net/tkokof1)阅读数：60


> 
本文简单介绍了混合使用 C# indexer 和 property 时可能出现的一种意外错误

C# 中的 [property](https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/properties) 想必大家都很熟悉,比起传统的 get 和 set 函数, property 的一大优势就是可以简化代码:

```
public class PropertyClass
{
	public string Item { get; set; }
}
```

不过 C# 中的 [indexer](https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/indexers/) 可能乍看上去就有些陌生了,基本的定义方法如下:

```
public class IndexerClass
{
    public object this[int index] { get { return null; } set {} }
}
```

这种定义方式对于偏于数组或者矩阵形式的数据类型特别有用,例如 Unity 中的 [Matrix4x4](https://docs.unity3d.com/ScriptReference/Matrix4x4.html) 便定义了一个二维的indexer(*Matrix4x4也定义了一维版本的indexer*),用以提供直观的数据访问方式:

```
// indexer of UnityEngine.Matrix4x4
public float this[int row, int column]
{
	get
	{
		return this[row + column * 4];
	}
	set
	{
		this[row + column * 4] = value;
	}
}
```

不过令人有些意外的是,如果我们混合使用上述的 indexer 和 property,竟然会导致编译错误:

```
// compile error ...
public class MixClass
{
	public string Item { get; set; }
	public object this[int index] { get { return null; } set {} }
}
```

原因在于 C# 使用了类似 property 的方式实现了 indexer,并且 indexer 所对应的 property 的变量名便是 “Item”, 所以上述代码会被编译器改写为以下形式(**不准确,仅作示意**):

```
public class MixClass
{
	public string Item { get; set; }
	public object Item { get { return null; } set {} }
}
```

于是同名的 property 便造成了编译错误.

解决方法大概有两种,修改 property 的名字(不要以 “Item” 命名),或者修改 indexer 的名字,其中 indexer 名字的修改需要用到[属性](https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/concepts/attributes/):

```
public class MixClass
{
	public string Item { get; set; }
	[System.Runtime.CompilerServices.IndexerName("IndexerItem")]
	public object this[int index] { get { return null; } set {} }
}
```

#### 参考
- [class with indexer and property named “Item”](https://stackoverflow.com/questions/5110403/class-with-indexer-and-property-named-item)
- [item property in c#](https://stackoverflow.com/questions/38260565/item-property-in-c-sharp)

