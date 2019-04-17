# 重拾C#教程：命名空间（Namespace） - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月10日 13:09:26[boonya](https://me.csdn.net/boonya)阅读数：673








原文地址：[http://www.runoob.com/csharp/csharp-namespace.html](http://www.runoob.com/csharp/csharp-namespace.html)



**命名空间**的设计目的是提供一种让一组名称与其他名称分隔开的方式。在一个命名空间中声明的类的名称与另一个命名空间中声明的相同的类的名称不冲突。

## 定义命名空间

命名空间的定义是以关键字 **namespace** 开始，后跟命名空间的名称，如下所示：
namespace namespace_name
{// 代码声明}
为了调用支持命名空间版本的函数或变量，会把命名空间的名称置于前面，如下所示：
namespace_name.item_name;
下面的程序演示了命名空间的用法：
usingSystem;namespace first_space
{class namespace_cl
   {publicvoid func(){Console.WriteLine("Inside first_space");}}}namespace second_space
{class namespace_cl
   {publicvoid func(){Console.WriteLine("Inside second_space");}}}classTestClass{staticvoidMain(string[] args){
      first_space.namespace_cl fc =new first_space.namespace_cl();
      second_space.namespace_cl sc =new second_space.namespace_cl();
      fc.func();
      sc.func();Console.ReadKey();}}
当上面的代码被编译和执行时，它会产生下列结果：
Inside first_space
Inside second_space
## *using* 关键字

**using** 关键字表明程序使用的是给定命名空间中的名称。例如，我们在程序中使用 **System** 命名空间，其中定义了类 Console。我们可以只写：
Console.WriteLine("Hello there");
我们可以写完全限定名称，如下：
System.Console.WriteLine("Hello there");
您也可以使用 **using** 命名空间指令，这样在使用的时候就不用在前面加上命名空间名称。该指令告诉编译器随后的代码使用了指定命名空间中的名称。下面的代码演示了命名空间的应用。

让我们使用 using 指定重写上面的实例：
usingSystem;using first_space;using second_space;namespace first_space
{class abc
   {publicvoid func(){Console.WriteLine("Inside first_space");}}}namespace second_space
{class efg
   {publicvoid func(){Console.WriteLine("Inside second_space");}}}classTestClass{staticvoidMain(string[] args){
      abc fc =new abc();
      efg sc =new efg();
      fc.func();
      sc.func();Console.ReadKey();}}
当上面的代码被编译和执行时，它会产生下列结果：
Inside first_space
Inside second_space
## 嵌套命名空间

命名空间可以被嵌套，即您可以在一个命名空间内定义另一个命名空间，如下所示：
namespace namespace_name1 
{// 代码声明namespace namespace_name2 
   {// 代码声明}}
您可以使用点（.）运算符访问嵌套的命名空间的成员，如下所示：
usingSystem;usingSomeNameSpace;usingSomeNameSpace.Nested;namespaceSomeNameSpace{publicclassMyClass{staticvoidMain(){Console.WriteLine("In SomeNameSpace");Nested.NestedNameSpaceClass.SayHello();}}// 内嵌命名空间namespaceNested{publicclassNestedNameSpaceClass{publicstaticvoidSayHello(){Console.WriteLine("In Nested");}}}}
当上面的代码被编译和执行时，它会产生下列结果：
InSomeNameSpaceInNested
## using的用法：

1. using指令：引入命名空间

这是最常见的用法，例如：
usingSystem;usingNamespace1.SubNameSpace;
2. using static 指令：指定无需指定类型名称即可访问其静态成员的类型
usingstaticSystem.Math;var= PI;// 直接使用System.Math.PI
3. 起别名
usingProject= PC.MyCompany.Project;
4. using语句：将实例与代码绑定
using(Font font3 =newFont("Arial",10.0f),
            font4 =newFont("Arial",10.0f)){// Use font3 and font4.}
代码段结束时，自动调用font3和font4的Dispose方法，释放实例。




