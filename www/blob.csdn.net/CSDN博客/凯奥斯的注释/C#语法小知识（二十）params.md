# C#语法小知识（二十）params - 凯奥斯的注释 - CSDN博客





2016年11月25日 22:37:24[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：6755
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# C#中可以使用params来实现可变参数。

例如：



```
public void TestObjectParams(params object[] objects)
	{
		foreach (var obj in objects) {
			Console.WriteLine (obj);
		}
	}
```


或者指定某种类型：



```
public void TestIntegerParams(params int[] objects)
	{
		foreach (var obj in objects) {
			Console.WriteLine (obj);
		}
	}
```



params之前可以有任意多个参数，或者没有，params之后不可以有参数。

一个方法声明中只允许有一个params关键字。




使用：



```
TestObjectParams (1, "test", 2.0, 'b');
		TestIntegerParams (1, 2, 3, 4);
```

我们还可以直接使用数组来作为参数，例如：


```
int[] arr = new int[4]{ 1, 2, 3, 4 };
		TestIntegerParams (arr);
```

这跟之前的例子是等效的，但是在arr之前或者之后再增加参数就会报错。



如果是这样：



```
object[] objects = new object[2] { 1, "obj" };
		TestObjectParams (objects);
```
仍会当做两个参数处理，但是如果改成这样：

`		TestObjectParams (objects,99);`

就会将objects当做数组来处理。




