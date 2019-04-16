# VB.NET中Module的概念 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [VB.NET中Module的概念](https://www.cnblogs.com/xdp-gacl/p/3495853.html)



今天学习VB.NET，发现VB.NET里面有一个Module的东西，如下图(图-1)所示：

![](https://images0.cnblogs.com/blog/289233/201312/28220058-3afc9d1a6fb2457381daeb9a70b052b6.png)

图-1

上网查了一下VB.NET里面的Module，才发现这是学习VB.NET遇到的第一个典型的问题就是：为什么VB.NET中会有一个Module的东西，而在C#等语言中是没有的呢？

首先，这是一个历史原因。早先的VB语言都有**模块**和**类模块**的概念。**所谓模块一般就是存放公用的一些方法****(Sub)****或者函数****(Function)**，而**类模块，相当于就是我们经常所说的类的概念**。 

**模块****(Module)****里面可以直接有方法，这些方法自动就是静态的（或者称为共享的****——VB.NET****中的关键字是****Shared)**，这些方法可以直接调用，**所以****Module****是无法实例化，也没有必要实例化的**。

模块无法继承，也无法实现接口。

模块里面可以有类，但其实这个类并不需要真的通过模块作为前缀来引用。所以模块有点虚的概念

模块可以被组织到命名空间中去，因为模块其实是个虚的，所以它的方法也可以直接外层的命名空间调用

Module代码范例：

```
1 '用命名空间封装Module
 2 Namespace VBModuleTest
 3     ''' <summary>
 4     ''' 模块无法继承，无法实现接口
 5     ''' </summary>
 6     ''' <remarks></remarks>
 7     Module ModuleTest
 8         ''' <summary>
 9         ''' 模块里面的方法都是静态的，无需实例化，也无从实例化
10         ''' </summary>
11         ''' <remarks></remarks>
12         Sub Main()
13             'new一个Class1实例
14             'Dim clazz As Class1 = New Class1这样也是对的
15             Dim clazz As Class1 = New Class1()
16             clazz.Name = "Class1类"
17             Console.WriteLine(clazz.Name)
18             Console.WriteLine(clazz.GetClassName())
19             Console.ReadKey()
20         End Sub
21         ''' <summary>
22         ''' 有返回值的方法，用Function来声明
23         ''' </summary>
24         ''' <returns></returns>
25         ''' <remarks></remarks>
26         Public Function Test1() As String
27             Console.WriteLine("这个是Module里面的Test1方法")
28             Return ""
29         End Function
30         ''' <summary>
31         ''' 无返回值的方法，用Sub来声明
32         ''' </summary>
33         ''' <remarks></remarks>
34         Public Sub Test2()
35             Console.WriteLine("这个是Module里面的Test2方法")
36         End Sub
37 
38         ''' <summary>
39         ''' 类型可以放在模块里面，但主要的现实意义在于分类存放而已，放在模块里面并不影响该类型对外的引用。
40         ''' 例如我们并不一定需要用VBModuleTest.ModuleTest.Class1，
41         ''' 而是直接通过VBModuleTest.Class1就可以对其进行引用
42         ''' </summary>
43         ''' <remarks></remarks>
44         Public Class Class1 : Implements IComparable
45             ''' <summary>
46             ''' 声明属性
47             ''' </summary>
48             ''' <remarks></remarks>
49             Private _name As String
50             Public Property Name() As String
51                 Get
52                     Return _name
53                 End Get
54                 Set(ByVal value As String)
55                     Me._name = value
56                 End Set
57             End Property
58 
59             ''' <summary>
60             ''' 声明方法
61             ''' </summary>
62             ''' <returns></returns>
63             ''' <remarks></remarks>
64             Public Function GetClassName() As String
65                 Test1()
66                 ModuleTest.Test2()
67                 Return Me.GetType().ToString()
68             End Function
69 
70             ''' <summary>
71             ''' 实现接口中的方法
72             ''' </summary>
73             ''' <param name="obj"></param>
74             ''' <returns></returns>
75             ''' <remarks></remarks>
76             Public Function CompareTo(ByVal obj As Object) As Integer Implements System.IComparable.CompareTo
77                 Return 1
78             End Function
79         End Class
80     End Module
81 End Namespace
```

这里的代码只是纯粹为演示目的而写的，没有任何具体意义 。









