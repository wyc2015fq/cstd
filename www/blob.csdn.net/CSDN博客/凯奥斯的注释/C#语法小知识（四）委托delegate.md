# C#语法小知识（四）委托delegate - 凯奥斯的注释 - CSDN博客





2016年09月09日 13:55:11[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4934
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# **delegate** 是一种可用于封装命名或匿名方法的引用类型。委托类似于 C++ 中的函数指针；但是，委托是类型安全和可靠的。有关委托的应用，请参见[委托](https://msdn.microsoft.com/zh-cn/library/ms173171.aspx)和[泛型委托](https://msdn.microsoft.com/zh-cn/library/sx2bwtw7.aspx)。

这里我们就介绍一下委托的几种用法。

一个简单的委托示例：



```
public class TestDelegate
{
	public delegate void Delegate(object sender);
	private Delegate _delegate;
	public void SetDelegate(Delegate dlg)
	{
		_delegate = dlg;
	}
	public void InvokeDelegate()
	{
		if (_delegate == null) {
			return; 
		}
		_delegate.Invoke (this);
	}
}
```




实现委托接口：



```
public class TestListenerA
{
	static public void OnDelegateInvoked(object sender)
	{
		System.Console.WriteLine ("Listener A");
	}
}
```
用法：

```
<pre name="code" class="csharp">		TestDelegate dlg = new TestDelegate();
		TestListenerA la = new TestListenerA ();
		dlg.SetDelegate (la.OnDelegateInvoked);
		dlg.InvokeDelegate ();
```


如果对[委托模式~~（或者叫代理模式）~~](http://baike.baidu.com/link?url=HQOJF4LMoTAUdv0GVPDWo7azHt_dT130KeevEYG8P_ZL7waAOR385sXDPk-aWVJN6LSsowX-lxsnYhJxrO4ouK)比较熟悉的话，应该很容易就看懂这段代码。用C++的语法来解释就是为TestDelegate设置了一个回调。


但是delegate的用法不仅限于此，我们为TestDelegate增加一段代码：



```
public void AddListener(Delegate dlg)
	{
		_delegate += dlg;
	}
	public void RemoveListener(Delegate dlg)
	{
		_delegate -= dlg;
	}
```

这样我们就可以为TestDelegate增加多个回调。


在声明一个类，实现了static的委托：



```
public class TestListenerB
{
	static public void EventHandler(object sender)
	{
		System.Console.WriteLine ("Listener B");
	}
}
```

那么我们就可以这样使用delegate：




```
TestDelegate dlg = new TestDelegate();
		TestListenerA la = new TestListenerA ();
		dlg.AddListener (la.OnDelegateInvoked);
		dlg.AddListener (TestListenerB.EventHandler);
		TestDelegate.Delegate lamdaDlg = ((object sender) => {
			System.Console.WriteLine ("Listener Lamda C");
		});
		dlg.AddListener (lamdaDlg);
		dlg.AddListener((object sender) =>
			{
				System.Console.WriteLine ("Listener Lamda D");
			});
		dlg.InvokeDelegate ();
```
我们还可以去掉部分delegate：




```
dlg.RemoveListener (TestListenerB.EventHandler);
		dlg.RemoveListener (lamdaDlg);
		dlg.InvokeDelegate ();
```
这样TestListenerB.EventHandler和lamdaDlg就不再被调用了。


我们还可以移除所有delegate：


`		dlg.SetDelegate (null);`
并重新添加：




`		dlg.AddListener (lamdaDlg);`
这里可能会有疑问，让null和lamdaDlg相加不会有问题吗？


我们就带着这个问题深入了解一下delegate。

我们知道所有的delegate都继承自[System.MulticastDelegate](https://msdn.microsoft.com/zh-cn/library/system.multicastdelegate(v=vs.110).aspx)，而后者继承自[System.Delegate](https://msdn.microsoft.com/zh-cn/library/system.delegate(v=vs.110).aspx)。

System.Delegate是一个抽象类（abstractclass），所以一些具体的实现都是在System.MulticastDelegate里完成的。

我们这里主要看两个方法Combine和Remove，这两个方法都是静态方法，可以推测，这两个方法分别调用了CombineImpl和RemoveImpl。

CombineImpl是合并委托的实现，而RemoveImpl是移除委托的实现。

而System.Delegate和System.MulticastDelegate都有一个方法是GetInvocationList。这个方法返回了一个System.Delegate数组，数组里是参与调用的delegate。

由此我们可以推测，System.MulticastDelegate里维护了一个委托表，当调用CombineImpl的时候会把新的委托加入这个表，当调用RemoveImpl的时候会把目标委托从表里删除。

那么我们就可以推测，当delegate调用operator + 的时候就会调用System.Delegate的Combine方法，而调用operator -的时候就会调用System.Delegate的Remove方法。而给一个delegate赋值的时候，便会调用System.Delegate的CreateDelegate方法。

而回到之前的问题，为什么null和lamdaDlg可以相加？我们不必探究Combine方法是如何实现的，只需要简单的转换一下便可以理解


`_delegate += dlg;`等同于：



`_delegate = (TestDelegate.Delegate)System.Delegate.Combine (_delegate, dlg);`






