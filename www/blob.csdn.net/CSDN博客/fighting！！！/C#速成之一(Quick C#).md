# C#速成之一(Quick C#) - fighting！！！ - CSDN博客
2017年11月14日 20:21:41[dujiahei](https://me.csdn.net/dujiahei)阅读数：360
https://www.cnblogs.com/qqflying/archive/2007/10/21/932208.html
绪论
　　C#是这样的一种语言，具有C++的特点，象Java一样的编程风格, 并且象Basic一样的快速开发模型。如果你已经知道了C++，本文会在不到一个小时的时间内让你迅速掌握C#的语法。熟悉Java的括会更好，因为Java的程序结构、打包（Packages）和垃圾收集的概念有助于你更快的了解C#。因此在讨论C#的构造时，我会假定你了解C++。
　　本文会讨论C#语言的构造与特点，同时会采取简洁的和你能理解的方式使用些代码示例，我们会尽量让你能稍微看看这些代码就能理解这些概念。
　　注意：本文不是为C#高手(C# gurus)所写. 这是针对在C#学习上还是初学者的文章。
　　下面是将要讨论的C#问题的目录:
　　程序结构 
　　命名空间
　　数据类型
　　变量
　　运算符和表达式
　　枚举
　　语句（Statements ）
　　类（Classes）和结构（Structs）
　　修饰符（Modifiers）
　　属性(Properties)
　　接口(Interfaces)
　　方法参数(Function Parameters)
　　数组（Arrays）
　　索引器（Indexers）
　　装箱及拆箱操作
　　委托(Delegates)
　　继承和多态
　　下面的内容将不会在被讨论之列：
　　C++与C#谁更通用
　　诸如垃圾回收、线程以及文件处理等概念
　　数据的类型转换
　　异常处理
　　.NET库
　　-------------------
　　程序结构
　　-------------------
　　这一点象C++，C#是一种对大小写字母敏感的语言，分号“;”是语句间的分隔符。与C++不同的是，C#当中声明代码文件（头文件）与实现代码文件（cpp文件）不是独立存在的，所有代码（类声明和类实现）都位于一个扩展名为cs的文件内。
　　让我们瞧瞧C#当中的 Hello world 程序是怎样的。
　　using System;
　　namespace MyNameSpace
　　{
　　class HelloWorld
　　{
　　 static void Main(string[] args)
　　 {
　　 Console.WriteLine ("Hello World");
　　 }
　　}}
　　在C#当中的每样东西都被封装到一个类中，C#的类又被封装到一个命名空间当中（就象一个文件夹中的文件）。类似于 C++，main方法是你的程序的入口点。C++的main函数调用名称是"main"，而C#的main函数是以大写字母M为起点的名称是"Main"。
　　没有必要把分号分隔符放在类语句块或者结构定义语句块后。这在C++当中被要求，但在C#当中却不是。
　　-------------------
　　命名空间
　　-------------------
　　每一个类都被包装进一个命名空间。命名空间的概念与C++的完全相同，但在C#当中使用命名空间的频率较C++还高。你可以使用点限定符(dot qulifier)访问一个类。在上面的hello world程序当中MyNameSpace就是一个命名空间。
　　现在思考这样的一个问题，你想从某些别的类的命名空间当中来访问HelloWorld这个类该如何操作。
　　这有一个例子：
　　using System;
　　namespace AnotherNameSpace
　　{
　　 class AnotherClass
　　 {
　　 public void Func()
　　 {
　　 Console.WriteLine ("Hello World");
　　 }
　　 }
　　}
　　现在，从你的HelloWorld类里你能象这样去访问上面的这个AnotherNameSpace的命名空间：
　　using System;
　　using AnotherNameSpace; // you will add this using statement
　　namespace MyNameSpace
　　{
　　class HelloWorld
　　{
　　 static void Main(string[] args)
　　 {
　　 AnotherClass obj = new AnotherClass();
　　 obj.Func();
　　 }
　　}
　　}
　　在.NET库当中，System是位于顶层的命名空间，别的命名空间都存在这个命名空间之下。默认状态下，存在一个全局的命名空间，因此一个在命名空间外定义的类将直接在这个全局命名空间之下；因此，你能在没有任何点限定符的情况下访问这个类。
　　你也可以象下面这样定义嵌套的命名空间。
　　Using
　　C++当中的"#include"指示被C#的"using"关键字取代，它后面跟着一个命名空间的名字。正如上面的"using System"。"System"是别的所有被封装的命名空间和类中最底层的命名空间。所有对象的基类都是System命名空间内的Object类派生的。
　　-------------------
　　变量
　　-------------------
　　除以下并别外，C#当中的变量几乎与C++同：
　　与C++不同，C#变量被访问之前必须被初始化；否则编译时会报错。因此，访问一个未初始化变量是不可能的事。
　　C#中你不会访问到一个不确定的指针。（译者注：严格说起来C#已经把指针概念异化，限制更严格。所以有些资料上会说C#取消了指针概念）
　　一个超出数组边界的表达式是不可访问的。
　　C#中没有全局的的变量或全局函数，全局方式的操作是通过静态函数和静态变量来实现的。

