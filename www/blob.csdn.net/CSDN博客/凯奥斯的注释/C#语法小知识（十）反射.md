# C#语法小知识（十）反射 - 凯奥斯的注释 - CSDN博客





2016年09月28日 23:03:48[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1248
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# 反射指程序可以访问、检测和修改它本身状态或行为的一种能力。



反射（Reflection）有下列用途：
- 它允许在运行时查看特性（attribute）信息。
- 它允许审查程序集中的各种类型，以及实例化这些类型。

- 它允许调用后期绑定的方法。
- 它允许在运行时创建新类型，然后使用这些类型执行一些任务。




## 用途一：
查看特性（参考[C#语法小知识（七）特性](http://blog.csdn.net/ecidevilin/article/details/52551302)），例如下面，我们自定义了一个特性：


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
为类添加特性：

```
[Test(0)]
class Test0
{
}
```
查看特性：




```
Test0 tmp = new Test0 ();
		Type t = tmp.GetType ();
		foreach (Attribute attr in t.GetCustomAttributes(true)) {
			if (attr is TestAttribute) {
				Console.WriteLine (((TestAttribute)attr).val);
			}
		}
```




## 用途二：



查询当前程序集的各种类型：



```
Assembly assembly = Assembly.GetExecutingAssembly ();
		System.Type[] ts = assembly.GetTypes(); 
		foreach (var t in ts) {
			Console.WriteLine (t);
		}
```

实例化：



`		Test0 t0 = (Test0)assembly.CreateInstance ("Test0");`



## 用途三：



调用后期绑定的方法，例如我们定义了一个类：



```
class TestClass
{
	public void print(string str)
	{
		Debug.Log (str);
	}
	public int i {get;set;}
}
```

后期绑定调用方法：




```
System.Type testT = assembly.GetType ("TestClass");
		System.Object testO = assembly.CreateInstance ("TestClass");
		testT.InvokeMember("print", BindingFlags.InvokeMethod, System.Type.DefaultBinder, testO, new object[]{"A test"});
```




## 用途四：



运行时创建新类型和成员方法并执行：



```
AssemblyName assemblyName = new AssemblyName ("customAssembly");
		AssemblyBuilder assemblyBuilder = System.AppDomain.CurrentDomain.DefineDynamicAssembly(assemblyName, AssemblyBuilderAccess.Run); 
		ModuleBuilder moduleBuilder = assemblyBuilder.DefineDynamicModule ("Main");
		TypeBuilder typeBuilder = moduleBuilder.DefineType("Test",TypeAttributes.Public);
		MethodBuilder methodBuilder = typeBuilder.DefineMethod ("TestMethod", MethodAttributes.Public, null, null);
		ILGenerator il = methodBuilder.GetILGenerator();
		il.Emit(OpCodes.Ldstr,"TestMethod");
		il.Emit(OpCodes.Call,typeof(Console).GetMethod("WriteLine",new System.Type[]{typeof(string)}));
		il.Emit(OpCodes.Ret);
		System.Type customType = typeBuilder.CreateType ();
		System.Object customObject = customType.Assembly.CreateInstance (customType.Name);
		customType.InvokeMember (methodBuilder.Name, BindingFlags.InvokeMethod, System.Type.DefaultBinder, customObject, null);
```










## 优点：
- 1、反射提高了程序的灵活性和扩展性。 
- 2、降低耦合性，提高自适应能力。 
- 3、它允许程序创建和控制任何类的对象，无需提前硬编码目标类。

## 缺点：
- 1、性能问题：使用反射基本上是一种解释操作，用于字段和方法接入时要远慢于直接代码。因此反射机制主要应用在对灵活性和拓展性要求很高的系统框架上，普通程序不建议使用。 
- 2、使用反射会模糊程序内部逻辑；程序员希望在源代码中看到程序的逻辑，反射却绕过了源代码的技术，因而会带来维护的问题，反射代码比相应的直接代码更复杂。






