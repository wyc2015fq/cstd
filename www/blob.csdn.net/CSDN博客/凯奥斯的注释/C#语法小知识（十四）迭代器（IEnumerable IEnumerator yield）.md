# C#语法小知识（十四）迭代器（IEnumerable/IEnumerator/yield） - 凯奥斯的注释 - CSDN博客





2016年10月23日 19:27:26[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1335
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# C#里有一个很好用的foreach关键字，有些时候可以取代for循环来遍历一个数据结构，例如List、Dictionary等。而自定义的类型如果希望可以foreach来遍历的话就需要继承IEnumerable，并为它添加一个迭代器类（继承IEnumerator）。

我之前在[小话设计模式（十五）迭代器模式](http://blog.csdn.net/ecidevilin/article/details/52727049)中简单介绍过IEnumerable的写法，这里就以此为例。

继承自IEnumable（即可被枚举/迭代的类型）接口的Inventory类（接口方法GetEnumerator）：



```
public class Inventory : IEnumerable
{
	private List<object> _items = new List<object>();
	public object this[int index]
	{
		get { return _items [index];}
		set { _items.Insert (index, value);}
	}
	public int Count
	{
		get { return _items.Count;}
	}

	IEnumerator _enumerator;
	public IEnumerator GetEnumerator()
	{
		if (_enumerator == null) {
			_enumerator = new InventoryEnumerator (this);
		}
		_enumerator.Reset ();
		return _enumerator;
	}
}
```
继承自IEnumerator（即枚举/迭代器）接口的InventoryEnumerator类（接口方法MoveNext、Reset和接口属性current）：




```
public class InventoryEnumerator : IEnumerator
{
	private int _current = -1;
	private Inventory _inventory;
	public InventoryEnumerator(Inventory inventory)
	{
		_inventory = inventory;
	}
	public object Current
	{
		get {
			return _inventory [_current];
		}
	}
	public bool MoveNext()
	{
		_current++;
		return _current < _inventory.Count;
	}
	public void Reset()
	{
		_current = -1;
	}
}
```
这样就可以使用foreach来遍历Inventory：




```
Inventory inventory = new Inventory ();
		inventory[0] = "stone";
		inventory[1] = "twig";
		inventory[2] = "log";
		inventory[3] = "carrot";
		foreach (object obj in inventory) {
			Console.WriteLine (obj);
		}
```


除了显示的创建可枚举类之外，我们还可以使用yield隐式地实现一个IEnumable类型：




```
public IEnumerable CreateEnum()
	{
		for (int i = 0; i < 10; i++) {
			yield return i;
		}
	}
```

使用：




```
foreach (var i in CreateEnum()) {
			Console.WriteLine (i);
		}
```

这里yield return后面跟的值只需要是object类型。所以，我们也可以在CreateEnum的for循环下面添加：



`yield return "x";`






如果你需要限定yield return的类型，那么就需要使用泛型IEnumerable<T>：



```
public IEnumerable<string> CreateStringEnum()
	{
		yield return "pen";
		yield return "apple";
		yield return "apple pen";
		yield return "pen";
		yield return "pineapple";
		yield return "pineapple pen";
	}
```
使用：




```
foreach (var i in CreateStringEnum()) {
			Console.WriteLine (i);
		}
```





如果你需要在某种情况下终止循环，你需要使用yield break。

我们用一种很傻但是很直观的方法修改一下CreateStringEnum：



```
public IEnumerable<string> CreateStringEnum(int enumMax)
	{
		if (enumMax <= 0) {
			yield break;
		}
		yield return "pen";
		if (enumMax <= 1) {
			yield break;
		}
		yield return "apple";
		if (enumMax <= 2) {
			yield break;
		}
		yield return "apple pen";
		if (enumMax <= 3) {
			yield break;
		}
		yield return "pen";
		if (enumMax <= 4) {
			yield break;
		}
		yield return "pineapple";
		if (enumMax <= 5) {
			yield break;
		}
		yield return "pineapple pen";
		if (enumMax <= 6) {
			yield break;
		}
	}
```
使用：




```
foreach (var i in CreateStringEnum(3)) {
			Console.WriteLine (i);
		}
```

这样就只会打印到"apple pen"。




yield除了可以隐式生成IEnumable类型外，还可以隐式的生成IEnumerator类型。

例如，我们可以直接把前面的Inventory的GetEnumerator的方法修改成：



```
public IEnumerator GetEnumerator()
	{
		foreach (var item in _items) {
			yield return item;
		}
	}
```
也是一样可以使用foreach遍历的。

yield break的用法跟前面的方法类似，这里就不赘述了。



但是泛型版本的IEnumerator<T>就麻烦一点，这里也顺便讲一下继承了泛型IEnumerable<T>的类的写法：



```
public class StrInventory : IEnumerable<string>
{
	private List<string> _items = new List<string>();
	public string this[int index]
	{
		get { return _items [index];}
		set { _items.Insert (index, value);}
	}
	public int Count
	{
		get { return _items.Count;}
	}

	IEnumerator<string> _enumerator;
	public IEnumerator<string> GetEnumerator()
	{
//		if (_enumerator == null) {
//			_enumerator = new StrInventoryEnumerator (this);
//		}
//		_enumerator.Reset ();
//		return _enumerator;
		foreach (var item in _items) {
			yield return item;
		}
	}
	IEnumerator IEnumerable.GetEnumerator()
	{
		return this.GetEnumerator ();
	}
}
```

需要额外实现一个IEnumerable.GetEnumerator方法。


代码中注释掉的部分StrInventoryEnumerator是继承了IEnumerator<string>的迭代器：




```
public class StrInventoryEnumerator : IEnumerator<string>
{
	private int _current = -1;
	private StrInventory _inventory;
	public StrInventoryEnumerator(StrInventory inventory)
	{
		_inventory = inventory;
	}
	public string Current
	{
		get {
			return _inventory [_current];
		}
	}
	object IEnumerator.Current
	{
		get {
			return this.Current;
		}
	}
	public bool MoveNext()
	{
		_current++;
		return _current < _inventory.Count;
	}
	public void Reset()
	{
		_current = -1;
	}
	public void Dispose()
	{
	}
}
```

除了IEnumerator.Current属性之外，还需要额外实现Dispose()方法（IEnumerator<T>继承了IDispose，为了释放对象中的非托管资源）。





需要注意一点：yield return的类型跟IEnumerable<T>和IEnumerator<T>的T相对应（非泛型版本就是object），而方法本身的返回类型是继承了IEnumerable、IEnumerable<T>、IEnumerator或IEnumerator<T>的内部类型（由编译器额外创建的）。我们可以打印一下：



```
Console.WriteLine (inventory.GetEnumerator().GetType().Name);
		Console.WriteLine (CreateEnum().GetType().Name);
```

显示：


<GetEnumerator>c__Iterator0


<CreateEnum>c__Iterator1



注：

1、使用yield关键字的迭代器方法的返回值必须是IEnumerable、IEnumerable<T>、IEnumerator或IEnumerator<T>，并且声明的参数不能是ref或out参数。
2、yield无法在匿名方法（参考[C#语法小知识（十二）匿名方法与Lambda表达式](http://blog.csdn.net/ecidevilin/article/details/52832557)）或包含不安全的块（参考[unsafe关键字](https://msdn.microsoft.com/zh-cn/library/chfa2zb8.aspx)）的方法中使用。


3、try-catch块中不能使用yield
 return语句，但可以使用yield break语句。try-finally的try块中可以使用yield return语句或yield break语句，finally块中二者都不可以使用。如果foreach语句（迭代器方法之外）引发异常，则会执行迭代器方法中的finally块。




最后，如果你需要了解迭代器的相关理论，不妨再看一下[小话设计模式（十五）迭代器模式](http://blog.csdn.net/ecidevilin/article/details/52727049)。






