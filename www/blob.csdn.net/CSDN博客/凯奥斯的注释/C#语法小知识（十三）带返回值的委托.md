# C#语法小知识（十三）带返回值的委托 - 凯奥斯的注释 - CSDN博客





2016年10月17日 11:41:05[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4339
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# 委托的相关基础知识请参考[C#语法小知识（四）委托delegate](http://blog.csdn.net/ecidevilin/article/details/52486015)，这里讨论一下带返回值的委托如何处理。

声明一个带返回值的委托：


`public delegate string TestDel(string str);`
使用：




```
TestDel t;
		t = (string str) => str;
		t += (string str) => str + "1";
		t += (string str) => str + "2";
		t += (string str) => str + "3";
		Console.WriteLine(t ("hhhh"));
```

（这里添加监听使用了表达式Lambda，参考[C#语法小知识（十二）匿名方法与Lambda表达式](http://blog.csdn.net/ecidevilin/article/details/52832557)）


这里会打印hhhh3，也就是最后一个监听的返回值。

如果要打印所有的结果该怎么写？

我们注意到System.MultiCastDelegate里有一个方法：GetInvocationList



```
foreach (TestDel td in t.GetInvocationList()) {
			Console.WriteLine(td.Invoke ("iiii"));
		}
```


这样所有的返回值都被打印出来了。




使用这个方法我们可以做很多事情。



例如，我们可以为一个UI组件添加一个是否可以隐藏的delegate，如果有任何一个监听方返回false，那么这个UI组件便不能隐藏。


`public delegate bool CanClose();`

```
CanClose canCloseDelegate = null;
		canCloseDelegate += () => true;
		canCloseDelegate += () => false;
		bool canClose = true;
		foreach (CanClose del in canCloseDelegate.GetInvocationList()) {
			if (!del ()) {
				canClose = false;
				break;
			}
		}
		if (canClose) {
			//TODO: Hide the ui
		}
```

或者可以为一个对象添加多个数据源。



`public delegate string DataSource();`

```
DataSource dsDelegate = null;
		dsDelegate += () => "Apple";
		dsDelegate += () => "Pen";
		dsDelegate += () => "Pineapple";
		List<string> data = new List<string>();
		foreach (DataSource ds in dsDelegate.GetInvocationList()) {
			data.Add (ds ());
		}
		Console.WriteLine (data.Count);
```







