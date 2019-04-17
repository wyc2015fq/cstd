# C#语法小知识（九）事件 - 凯奥斯的注释 - CSDN博客





2016年09月24日 06:03:38[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1278标签：[c#																[委托																[事件																[delegate																[event](https://so.csdn.net/so/search/s.do?q=event&t=blog)
个人分类：[C#](https://blog.csdn.net/ecidevilin/article/category/6399640)

所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)](https://so.csdn.net/so/search/s.do?q=delegate&t=blog)




# 事件是委托的一种特殊形式，通过event修饰符来标识。[类](https://msdn.microsoft.com/zh-cn/library/0b0thckt.aspx)或对象可以通过事件向其他类或对象通知发生的相关事情。 发送（或*引发*）事件的类称为发行者，接收（或*处理*）事件的类称为订阅者。

那么究竟事件和委托有什么区别？（关于委托，参考[C#语法小知识（四）委托delegate](http://blog.csdn.net/ecidevilin/article/details/52486015)）

我们通过下面这段代码来探究：



```
public class Test
{
	public delegate void TestDelegateType (string str);
	public TestDelegateType TestDelegate;
	public event TestDelegateType TestEvent;
	public EventHandler<EventArgs> TestDelegateHandler;
	public event EventHandler<EventArgs> TestEventHandler;

	public void Func()
	{
		TestDelegate.Invoke ("TestDelegate");
		TestEvent.Invoke ("TestEvent");
		TestDelegateHandler.Invoke (this, EventArgs.Empty);
		TestEventHandler.Invoke (this, EventArgs.Empty);
	}
}
```




调用：



```
Test t = new Test ();
		t.TestDelegate = (string str) => {
			print (str);
		};
		t.TestEvent += (string str) => {
			print(str);
		};
		t.TestDelegateHandler = (object obj, EventArgs args) => {
			print("TestDelegateHandler");
		};
		t.TestEventHandler += (object obj, EventArgs args) => {
			print("TestEventHandler");
		};
		t.Func ();
```




C#里创建标准的事件是使用EventHandler作为基础委托类型，这样EventArgs类型或者其派生类就作为第二个参数进行传递（派生类例如MyEventArgs，则需要把事件的声明改为EventHandler<MyEventArgs>
 TestMyEventHandler）。但是似乎并不受此限制，我们声明自定义delegate类型的事件也是可以的。

而event和普通delegate最大的区别在于：

可以通过=来对普通delegate变量赋值，但是不能对event变量赋值。event变量只支持+=或-=来修改它的值（当然普通delegate也支持）。

也就是说，event避免了用户对订阅列表的覆盖。这是一种很好的保护原则，所有建议尽可能的使用event。](https://so.csdn.net/so/search/s.do?q=事件&t=blog)](https://so.csdn.net/so/search/s.do?q=委托&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)




