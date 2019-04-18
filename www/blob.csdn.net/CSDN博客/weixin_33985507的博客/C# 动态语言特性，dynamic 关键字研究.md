# C# 动态语言特性，dynamic 关键字研究 - weixin_33985507的博客 - CSDN博客
2014年11月02日 14:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
原文:[C# 动态语言特性，dynamic 关键字研究](http://www.cnblogs.com/xinghuayang/p/dynamic_keyword_search.html)
# 1       动态语言简介
支持动态特性的语言现在大行其道，并且有继续增长的趋势。比如 Ruby 和 Python, 还有天王级的巨星 --- JavaScript. 现在一个程序员说自己对 JavaScript 根本没使用过，别人一定把你当成从火星回来的吧！
很多使用过 JavaScript 的程序员，刚开始对其动态特性深恶痛绝，欲除之而后快，但是一旦熟悉这个语言以后，又会发疯般的爱上她（我的野蛮女友）。
以创建一个“人”为例， JavaScript 可以这样写：
```java
var person = {
FirstName:"Alan",
LastName:"Yang"
};
person.Age = 28;
person.show = function(){
var that = this;
return "My name is " +  that.FirstName + " " + that.LastName + ". I’m  " +  that.Age +  "years old " ;
}
person.show();
```
在Chrome 中运行的输出为：
![](https://images0.cnblogs.com/blog/507412/201410/301444418782846.png)
虽然，这是一段很简单的代码，但是已有几个点我想大家都已经注意到：
- 属性 Age 是在person 对象定义后动态添加的
- 方法 show是在person 对象定义后动态添加的
- Age 属性和 show 方法正常使用
JavaScript 这种动态特性，使其在处理web 领域大显身手，也使得JavaScript 成为端处理的当之无愧的王者，App 怎么写 – JavaScript (WinJS), Web 页面怎么写—JavaScript (JQuery, KnocoutJS, ….), Server 怎么写 --- JavaScript(Note.JS)。
One- JavaScript, Everywhere.
# 2       C# 怎么办
What about C#? What about C# developer? What about me?
作为一名C# 开发人员，作为一名光荣的OOP 的拥趸，我们被一遍又一遍的告诉，“先声明后使用”，强类型, 多态，继承，引用….
不是我不明白，只是世界变化太快。昨天程序员还高大上，今天就矮穷挫。
难道这就是命么？难道C#/.NET 就没有办法处理动态增长了么？难道没有如果了么？
等一等，C# 中貌似，好像，大概，不一定，是有动态特性的吧，就好像程序员的世界里还是有MM的：
- 君不见 var 到处飞？
- 君不见 dynamic 到处有？
- 君不见 Asp.Net MVC ViewBag 类经常被使用？
没念及此，略感宽慰，var, dynamic, ViewBag 使用方便，还没有负作用，是极好的？是在编码，修Bug 必备良药呀？
# 3       var V.S dynamic 关键字
## 3.1  var 关键字
没有 var 的日子里，我需要知道每个变量的类型；没有 var 的日子里；我需要提醒自己类型转换；没有var 的日子里，我每天问自己类型转换安全么。没有var 的日子里，写代码很痛苦。
var -- Beginning in Visual C# 3.0, variables that are declared at method scope can have an implicit type var. An implicitly typed local variable is strongly typed just as if you had declared the type yourself, but the compiler determines the type.
官方定义就是 NB, 但是说人话以后，就是 从C#3.0 引入var 关键字，只能用于方法体内, 当然包括匿名方法和Lambda表达式，编译器会解析变量类型。
但是如何理解 var 关键字?
.NET 世界里有一个非常强大的武器，我已经很多关于C#/.NET 研究文章中使用过。那就是反射，不论你披了多么美味的语法糖果( syntax sugar), 我们都可以使用照妖镜-反射使其显出原形。
先进一段广告—不，先近一段代码—
```
static void Main(string[] args)
        {
            var vari = 1;
            int inti = 1;
        }
```
看一下反编译后的IL 代码：-- 使用工具为 ildasm
```
.method private hidebysig static void  Main(string[] args) cil managed
{
  .entrypoint
  // Code size       6 (0x6)
  .maxstack  1
  .locals init ([0] int32 vari,
           [1] int32 inti)
  IL_0000:  nop
  IL_0001:  ldc.i4.1
  IL_0002:  stloc.0
  IL_0003:  ldc.i4.1
  IL_0004:  stloc.1
  IL_0005:  ret
} // end of method Program::Main
```
见证奇迹的时刻. .locals init ([0] int32 vari,[1] int32 inti) – 大致意思是有有两个局部变量，都是int类型，一个名字叫做vari, 一个名字叫做inti。
What? 明明我声名了一个 var de 变量 vari, 初始化为 1; 为什么变成了 int 类型。发生了什么事？
Compiler 分析了var 声名的值的类型，然后译为强类型的声名。这也就解释了为什么 var 类型变量必须初始化，而且声名后不能改变其类型，如下代码不能编译的。
```
static void Main(string[] args)
        {
            var vari = 1;
            vari = "test";
            int inti = 1;
        }
```
哦！编译器，编译器，你欺骗了我？这个var 不同于 JavaScript 中的var 可以在生命后改变变量的类型。
有了var， 妈妈再也不用担心我的方法中类型转换的错误。以至于，现在C# 编码规范要求程序员在方法体内所有变量的声名均使用var 关键字。如ReSharper 工具。
在这里，我也强烈推荐在方法体内变量声明均使用var 关键字。
## 3.2  dynamic 关键字
虽然我们有了var 关键字，但是还是不能和 JavaScript 程序员一起愉快的玩耍，因为他们可以在改变变量的类型。
啊!多么痛的领悟!
痛定思痛，我们来观察一下，为什么 JavaScript 可以很开心的在运行时改变变量的类型？
难道说？难道是？JavaScript 是解释型语言，边解释边执行，没有任何编译文件，你没有看到过jsdll, 或者jsjar 文件吧？C# 是编译型语言。
那么问题就来了，如何让C# 编译器不检查变量的类型那？相信这个问题很好回答，就好像皇帝如何给一个人特权，很明显发个尚方宝剑，或者其他信物，然后见宝剑（信物），如朕亲临，不听，先斩后奏。
于是乎？dynamic 关键字，应运而生 – 奉天承运，皇帝诏曰，dynamic 关键字可以不经过C# 编译器检查。接旨，吾皇万岁，万岁，万万岁—C#, Complier。
OK，看一下 dynamic 关键字的官方解释---The dynamic type enables the operations in which it occurs to bypass compile-time type checking. Instead, these operations are resolved at run time。In most cases, it functions like it has type object. At compile time, an element that is typed as dynamic is assumed to support any operation。
以我小学英语毕业证的名义，我认为它说的是， dynamic 类型及其操作可以绕过 C# 编译器的检查，这些检查将在运行时进行。在大部分情况下 dynamic 类型的功能类似于 object, object耶 – 只有想不到，没有它做不到的事情哟！在编译时，dynamic 元素被认为可以支持任何操作。（如有不严谨，请以大学英语4级的名义，拍砖）。
但是，为什么你总是要问问题？你是十万个为什么么？好吧，你要问什么？
C# 不是静态类型的语言么？这是怎么可能？为什么？
来吧，再近一段代码—
```
static void Main(string[] args)
        {
            dynamic dyni = 1;
            dyni = "test";
        }
```
你知道我要做什么了，反编译—ildasm:
```
.method private hidebysig static void  Main(string[] args) cil managed
{
  .entrypoint
  // Code size       15 (0xf)
  .maxstack  1
  .locals init ([0] object dyni)
  IL_0000:  nop
  IL_0001:  ldc.i4.1
  IL_0002:  box        [mscorlib]System.Int32
  IL_0007:  stloc.0
  IL_0008:  ldstr      "test"
  IL_000d:  stloc.0
  IL_000e:  ret
} // end of method Program::Main
```
看一下.locals init ([0] object dyni) – 说明IL 认为你声名了一个 object 类型的局部变量，然后  IL_0001:  ldc.i4.1； IL_0002:  box        [mscorlib]System.Int32 对其进行初始化，并且进行装箱操作(box) ,然后通过ldstr 把它的值初始化为 “test”。
你明白了么？你点点头，然后又摇头—你为什么要摇头，我恨你？然后说—这个解释了它最底层的运作原理，但是没有解释dynamic 到底是如何工作的？
好吧！当一切又回到原点，当一切努力都随风而去，当生活没有了方向，当…..
我们该怎么办？大喊救命啊！--somebody help me! 但是没有来帮助你，除了你自己？
嗯！我们有什么，我们要什么，然后我们怎么做？ -- 解决问题的三个很好的问题。
- 第一个问题， 我们有什么？
我们有 dynamic 关键字声名的变量，或者属性。
- 第二个问题，我们要什么？
这个变量可以正确的识别它的所有值/方法，可以正确的调用。
- 第三个问题，我们怎么办？
怎么办？凉拌。
在程序的世界里，我们经常使用且有时被滥用的一个技巧是—引入第三者，引入一个层级，专门负责某种功能。想一想，Façade，Factory, Builder, Bridge … 等等设计模式，想一想 LINQ Provider, 想一想 CLR, Windows Runtime 等待。
那么同样的，我们也应该有一个层级来支持 dynamic 关键字的运行，因为它是在运行时动态调用 dynamic 类型的方法和属性。 -- 那我们就叫它 DLR 吧（Dynamic Language Runtime）
### 3.2.1DLR
果然不出山人所料？在CLR 上添加一个 DLR 层，来隔离 dynamic 代码和实际生成的 IL 代码。
来研究一段代码(不研究代码的程序员，就像不研究打仗的军人一样)--
```
class Program
    {
        static void Main(string[] args)
        {
            dynamic dynamicClass = new DynamicClass();
            dynamicClass.Value = "Test Dynamic";
 
            StaticClass staticClass = new StaticClass();
            staticClass.Value = "Test Dynamic";
        }
    }
 
    public class DynamicClass
    {
        public dynamic Value
        {
            get;
            set;
        }
    }
 
    public class StaticClass
    {
        public object Value
        {
            get;
            set;
        }
}
```
为了对比，首先我们将有关 dynamic 的部分从 Main 方法中注释掉，然后查看他的 IL 代码—
```
.method private hidebysig static void Main(string[] args) cil managed
{
    .entrypoint
    .maxstack 2
    .locals init (
        [0] class ConsoleApplication1.StaticClass staticClass)
    L_0000: nop
    L_0001: newobj instance void ConsoleApplication1.StaticClass::.ctor()
    L_0006: stloc.0
    L_0007: ldloc.0
    L_0008: ldstr "Test Dynamic"
    L_000d: callvirt instance void ConsoleApplication1.StaticClass::set_Value(object)
    L_0012: nop
    L_0013: ret
}
```
不解释。
现在，我们把注释去掉，然后将StaticClass 的部分注释掉，看看发生了什么事情—
```
entrypoint
    .maxstack 7
    .locals init (
        [0] object dynamicClass,
        [1] class [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfo[] CS$0$0000)
    L_0000: nop
    L_0001: newobj instance void ConsoleApplication1.DynamicClass::.ctor()
    L_0006: stloc.0
    L_0007: ldsfld class [System.Core]System.Runtime.CompilerServices.CallSite`1<class [mscorlib]System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>> ConsoleApplication1.Program/<Main>o__SiteContainer0::<>p__Site1
    L_000c: brtrue.s L_004b
    L_000e: ldc.i4.0
    L_000f: ldstr "Value"
    L_0014: ldtoken ConsoleApplication1.Program
    L_0019: call class [mscorlib]System.Type [mscorlib]System.Type::GetTypeFromHandle(valuetype [mscorlib]System.RuntimeTypeHandle)
    L_001e: ldc.i4.2
    L_001f: newarr [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfo
    L_0024: stloc.1
    L_0025: ldloc.1
    L_0026: ldc.i4.0
    L_0027: ldc.i4.0
    L_0028: ldnull
    L_0029: call class [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfo [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfo::Create(valuetype [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfoFlags, string)
    L_002e: stelem.ref
    L_002f: ldloc.1
    L_0030: ldc.i4.1
    L_0031: ldc.i4.3
    L_0032: ldnull
    L_0033: call class [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfo [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfo::Create(valuetype [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfoFlags, string)
    L_0038: stelem.ref
    L_0039: ldloc.1
    L_003a: call class [System.Core]System.Runtime.CompilerServices.CallSiteBinder [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.Binder::SetMember(valuetype [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpBinderFlags, string, class [mscorlib]System.Type, class [mscorlib]System.Collections.Generic.IEnumerable`1<class [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfo>)
    L_003f: call class [System.Core]System.Runtime.CompilerServices.CallSite`1<!0> [System.Core]System.Runtime.CompilerServices.CallSite`1<class [mscorlib]System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>>::Create(class [System.Core]System.Runtime.CompilerServices.CallSiteBinder)
    L_0044: stsfld class [System.Core]System.Runtime.CompilerServices.CallSite`1<class [mscorlib]System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>> ConsoleApplication1.Program/<Main>o__SiteContainer0::<>p__Site1
    L_0049: br.s L_004b
    L_004b: ldsfld class [System.Core]System.Runtime.CompilerServices.CallSite`1<class [mscorlib]System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>> ConsoleApplication1.Program/<Main>o__SiteContainer0::<>p__Site1
    L_0050: ldfld !0 [System.Core]System.Runtime.CompilerServices.CallSite`1<class [mscorlib]System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>>::Target
    L_0055: ldsfld class [System.Core]System.Runtime.CompilerServices.CallSite`1<class [mscorlib]System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>> ConsoleApplication1.Program/<Main>o__SiteContainer0::<>p__Site1
    L_005a: ldloc.0
    L_005b: ldstr "Test Dynamic"
    L_0060: callvirt instance !3 [mscorlib]System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>::Invoke(!0, !1, !2)
    L_0065: pop
    L_0066: ret
```
在看一下 dynamic 属性的 IL 代码 – 属性类型为object哟！
```
.property instance object Value
{
    .get instance object ConsoleApplication1.DynamicClass::get_Value()
    .set instance void ConsoleApplication1.DynamicClass::set_Value(object)
    .custom instance void [System.Core]System.Runtime.CompilerServices.DynamicAttribute::.ctor()
}
```
My Lady Gaga!
我做了什么，相比静态类型，生成了这么多的代码。人哪，还是不要知道那么多呀，知道多了影响寿命。
让我大致的解释一下上面的IL 代码，你让我复杂的说，我也不会。
- 第一: RuntimeBinder – 做什么用的？我不知道，但是Binder 这个名字说明了，有源，有目标对象，不源对象绑定到目标对象上。
说着，说着，我就想到了反射，当用反射给一个属相赋值时，需要找到属性的Set 方法，把值作为参数，然后再目标对象上Invoke。
很接近哦，这里创建的CSharpArugumentInfo[] – 参数列表。
        [1] class [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfo[] CS$0$0000)
- 第二：CallSite – Call,Invoke? 难道这个是用来Invoke 方法的。
_0007: ldsfld class [System.Core]System.Runtime.CompilerServices.CallSite`1<class [mscorlib]System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>> ConsoleApplication1.Program/<Main>o__SiteContainer0::<>p__Site1
- 第三：注意 ldstr “Value” – **Value ****是什么，Value ****是我们的属性名**。为什么把属性当作字符加载进来。而不是类型与静态类型的把Value 当作方法来调用？（    L_000d: callvirt instance void ConsoleApplication1.StaticClass::set_Value(object)）。
- 第四：果然把 Value 当作参数加载， 注意idc.i4.0
    L_001f: newarr [Microsoft.CSharp]Microsoft.CSharp.RuntimeBinder.CSharpArgumentInfo
    L_0024: stloc.1
    L_0025: ldloc.1
L_0026: **ldc.i4.0**
- 第五：方法调用，
1)      加载CallSite 类的 Target 字段—
L_0050: ldfld !0 [System.Core]System.Runtime.CompilerServices.CallSite`1<class [mscorlib]System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>>::Target
2)      加载CallSite类p_Stie1, 自动生成的：
System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>> 这个委托，第一个参数CallSite, 第二个参数Target,
第三个参数属性名，第四个参数需要赋的值.
L_0055: ldsfld class [System.Core]System.Runtime.CompilerServices.CallSite`1<class [mscorlib]System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>> ConsoleApplication1.Program/<Main>o__SiteContainer0::<>p__Site1
3)      L_005a: ldloc.0
4)      加载参数值—
L_005b: ldstr "Test Dynamic"
5)      在目标对象上调用方法—
L_0060: callvirt instance !3 [mscorlib]System.Func`4<class [System.Core]System.Runtime.CompilerServices.CallSite, object, string, object>::Invoke(!0, !1, !2)
**哦！实际上是用反射+Object ****来处理运行时的动态特性！鉴定完毕。**
### 3.2.2DLR 重要概念
- Expression Tree—DLR 使用 ExpressionTree 来表示语法。所以，DLR扩展了LINQ的　ExpressionTree 用来控制流程，赋值等。
- CallSite—负责运行时的源，目标对象的查找
- CallSiteBinder – 负责绑定
- Call site Cache – 通过上面的分析我们可以得出一个结论，那就是 dynamic 使用了反射，而且进行很多检查，这回降低代码运行效率。CallSite 会把分析经过 CallSite 调用的动态方法缓存，从而提高使用效率。
- 如果在运行时，没有找到需要调用的目标方法，则抛出RuntimeBinderException 异常。
### 3.2.3dynamic 关键字的用法
我们可以把 dynamic 关键字认为成object 关键字。也就是说所以可以使用 object 关键字的地方，基本上都可以使用 dynamic 关键字。 -- 以实际情况为准，没有全部进行测试。
- 声名变量
```
dynamic dyn = 1;
Console.WriteLine(dyn);
dyn = "test";
 Console.WriteLine(dyn.GetType());
```
- 声名方法返回值
```
static  dynamic ReturnDynamic(int type)
        {
           if(type == 0)
           {
               return 1;
           }
           else
           {
 
               return "you passed a type other than " + 0;
           }
        }
```
- 声名属性
```
public dynamic Value
        {
            get;
            set;
        }
```
- 委托的泛型参数
```
Func<dynamic, int> func = (i) => int.Parse(i.ToString());
      Console.WriteLine(func("1"));
      Console.WriteLine(func(1));
```
# 4       DynamicObject V.S. ExpandoObject
现在我们有了var, 有了dynamic, 但是还是不能和 JavaScript 程序员一起愉快的玩耍，因为，我们有没有对象，没有可以动态增长的对象。
好吧，人生就是这样，永远不要满足? 有爱情，还要面包，有房子，还要到老，有车子，还要有钞票。
于是DynamicObject 和ExpandoObject 被千呼万唤后，横空出世。
DynamicObejct 是所有动态类的基类，可以从其继承以实现自己的动态对象。ExpandoObject 是一个sealed 类，已经封装好了所有方法。
这个也给我们写框架代码给了一点提醒，定义好顶层的通常是抽象的基类后，然后实现一个子类
## 4.1  DynamicObject
```
static void Main(string[] args)
        {
            dynamic dynamicObject = new MyDynamicObject();
            dynamicObject.FirstName = "Alan";
            dynamicObject.LastName = "Yang";
            dynamicObject.Age = 28;
 
            Action<dynamic> show = (item) => Console.WriteLine("My name is " + item.FirstName + " " + item.LastName
                                                               + ". I'm " + item.Age + " years old.");
            dynamicObject.Show = show;
 
            dynamicObject.Show(dynamicObject);
 
            Console.Read();
        }
 
public class MyDynamicObject : DynamicObject
    {
        Dictionary<string, object> _dynamicData = new Dictionary<string, object>();
        public override bool TryGetMember(GetMemberBinder binder, out object result)
        {
            bool success = false;
            result = null;
            if (_dynamicData.ContainsKey(binder.Name))
            {
                result = _dynamicData[binder.Name];
                success = true;
            }
            else
            {
                result = "Property Not Found!";
                success = false;
            }
            return success;
        }
        public override bool TrySetMember(SetMemberBinder binder, object value)
        {
            _dynamicData[binder.Name] = value;
            return true;
        }
        public override bool TryInvokeMember(InvokeMemberBinder binder,
        object[] args,
        out object result)
        {
            result = true;
 
            dynamic method = _dynamicData[binder.Name];
 
            method(args[0]);
 
            return true;
        }
    }
```
## 4.2  ExpandoObject
```
static void Main(string[] args)
        {
            dynamic dynamicObject = new ExpandoObject();
            dynamicObject.FirstName = "Alan";
            dynamicObject.LastName = "Yang";
            dynamicObject.Age = 28;
 
            Action<dynamic> show = (item) => Console.WriteLine("My name is " + item.FirstName + " " + item.LastName
                                                               + ". I'm " + item.Age + " years old.");
            dynamicObject.Show = show;
 
            dynamicObject.Show(dynamicObject);
 
            Console.Read();
        }
```
## 4.3  ViewBag 源代码
最后贴出来 ASP.Net MVC 中动态对象 ViewBag 的代码，让 ViewBag不再神秘。ViewResult 中的ViewBag 属性的定义为：
```
public dynamic ViewBag
        {
            get
            {
                if (_dynamicViewData == null)
                {
                    _dynamicViewData = new DynamicViewDataDictionary(() => ViewData);
                }
                return _dynamicViewData;
            }
        }
```
DynamicViewDataDictionary 类的定义为：
```
// Copyright (c) Microsoft Open Technologies, Inc. All rights reserved. See License.txt in the project root for license information.
 
using System.Collections.Generic;
using System.Diagnostics;
using System.Dynamic;
 
namespace System.Web.Mvc
{
    internal sealed class DynamicViewDataDictionary : DynamicObject
    {
        private readonly Func<ViewDataDictionary> _viewDataThunk;
 
        public DynamicViewDataDictionary(Func<ViewDataDictionary> viewDataThunk)
        {
            _viewDataThunk = viewDataThunk;
        }
 
        private ViewDataDictionary ViewData
        {
            get
            {
                ViewDataDictionary viewData = _viewDataThunk();
                Debug.Assert(viewData != null);
                return viewData;
            }
        }
 
        // Implementing this function improves the debugging experience as it provides the debugger with the list of all
        // the properties currently defined on the object
        public override IEnumerable<string> GetDynamicMemberNames()
        {
            return ViewData.Keys;
        }
 
        public override bool TryGetMember(GetMemberBinder binder, out object result)
        {
            result = ViewData[binder.Name];
            // since ViewDataDictionary always returns a result even if the key does not exist, always return true
            return true;
        }
 
        public override bool TrySetMember(SetMemberBinder binder, object value)
        {
            ViewData[binder.Name] = value;
            // you can always set a key in the dictionary so return true
            return true;
        }
    }
}
```
# 5       结论
C# 中为了支持动态语言特性，引入了 var, dynamic, anonymous object (匿名对象) 等。但是作为强类型语言，动态特性是通过引入更多的复杂度来实现的，这与脚本语言，动态增长语言有着本质的区别。
