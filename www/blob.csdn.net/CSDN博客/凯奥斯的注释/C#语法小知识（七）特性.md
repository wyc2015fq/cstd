# C#语法小知识（七）特性 - 凯奥斯的注释 - CSDN博客





2016年09月15日 22:39:42[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1650
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)











# 特性（Attribute）提供功能强大的方法，用以将元数据或声明信息与代码（程序集、类型、方法、属性等）相关联。特性与程序实体关联后，即可在运行时使用名为“反射”的技术查询特性。
特性具有以下属性：
特性可向程序中添加元数据。元数据是有关在程序中定义的类型的信息。所有的
 .NET 程序集都包含指定的一组元数据，这些元数据描述在程序集中定义的类型和类型成员。
可以添加自定义特性，以指定所需的任何附加信息。
可以将一个或多个特性应用到整个程序集、模块或较小的程序元素（如类和属性）。
特性可以与方法和属性相同的方式接受参数。
程序可以使用反射检查自己的元数据或其他程序内的元数据。





本文就实现一个简单的自定义特性作为示例：



```
[AttributeUsage(AttributeTargets.Class | AttributeTargets.Method | AttributeTargets.Property)]
public class TestAttribute: Attribute
{
	public int val{ get; private set;}

	public TestAttribute(int i)
	{
		val = i;
	}
}
```
我们为TestAttribute添加了特性AttributeUsage，表明它只可以被添加到类（class）、方法（Method）和属性（Property）上。


所有的特性都继承自System.Attribute。

TestAttribute的构造函数便是它接收参数的地方（参数数量不限）。




添加自定义特性：



```
[Test(0)]
class Test0
{
	[Test(1)]
	public void Test1() {}

	[Test(2)]
	public int Test2 {get;set;}
}
```




我们为Test0这个类和它的成员方法Test1以及成员属性Test2分别添加了三个参数不一样的Test特性。如果我们想为成员变量添加一个Test特性：



```
[Test(3)]
	public int Test3;
```

就无法编译通过，需要为TestAttribute的AttributeUsage特性修改参数：




`[AttributeUsage(AttributeTargets.Class | AttributeTargets.Method | AttributeTargets.Property | AttributeTargets.Field)]`我们知道了如何创建自定义特性，知道了如何使用自定义特性，但是不知道如何访问自定义特性也并没有什么~~卵~~用。





我们需要使用System.Reflection(反射)来访问自定义特性。



```
Test0 tmp = new Test0 ();
		Type t = tmp.GetType ();
		foreach (Attribute attr in t.GetCustomAttributes(true)) {
			if (attr is TestAttribute) {
				Console.WriteLine (((TestAttribute)attr).val);
			}
		}
		foreach (var m in t.GetMethods()) {
			foreach (Attribute attr in m.GetCustomAttributes(true)) {
				if (attr is TestAttribute) {
					Console.WriteLine (((TestAttribute)attr).val);
				}
			}
		}
		foreach (var p in t.GetProperties()) {
			foreach (Attribute attr in p.GetCustomAttributes(true)) {
				if (attr is TestAttribute) {
					Console.WriteLine (((TestAttribute)attr).val);
				}
			}
		}
```


或者



```
MethodInfo mi = t.GetMethod ("Test2");

		foreach (Attribute attr in mi.GetCustomAttributes(true)) {
			if (attr is TestAttribute) {
				Console.WriteLine (((TestAttribute)attr).val);
			}
		}
		PropertyInfo pi = t.GetProperty ("Test3");

		foreach (Attribute attr in pi.GetCustomAttributes(true)) {
			if (attr is TestAttribute) {
				Console.WriteLine (((TestAttribute)attr).val);
			}
		}
```
或许你还不了解什么情况下需要使用自定义属性，Unity3D的SerializeField和ExecuteInEditMode就是很好的例子。
如果你不了解Unity3D也没关系，这里可以简单解释一下。


例如：你的代码需要将对象保存为文件，这就需要序列化成员变量，但是有些变量需要序列化，而有些并不需要，你就可以添加一个自定义特性（例如SerializeFieldAttribute）来标识这些成员变量，在序列化的时候，判断所有成员变量是否有这个特性，有的才被序列化。

再例如：你的程序有不同的两种模式Edit和Play，有些成员方法需要在Edit模式下运行，有些并不需要，你就可以添加一个自定特性（例如ExecuteInEditModeAttribute）来标识这些成员方法，在Edit模式下在调用这个方法是之前，判断这个方法是否有这个特性，有这个特性才会被调用。或者有些成员方法并不需要在Play下运行，你就可以定义一个带参数的特性，用于区分不同的模式。




附上特性的作用范围：

![](https://img-blog.csdn.net/20160929152312464)






