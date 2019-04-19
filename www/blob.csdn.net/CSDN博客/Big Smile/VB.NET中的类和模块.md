# VB.NET中的类和模块 - Big Smile - CSDN博客
2015年12月20日 10:13:52[王啸tr1912](https://me.csdn.net/tr1912)阅读数：4302标签：[vb.net](https://so.csdn.net/so/search/s.do?q=vb.net&t=blog)
个人分类：[[VB.NET]](https://blog.csdn.net/tr1912/article/category/6004909)
        在三层的例子中，我是仿照这C#中的结构来实现的VB.NET版的三层，但是在这之中出现了一个问题，那就是好像C#中只涉及到了各种类的使用，没有用到模块（module），但是在VB中要调用一个类中的字符串的时候却是出现了不能调用的问题，于是把类改为了模块就可以直接调用了，这又是为什么呢？
```
namespace Login.DAL
{
    class DbUtil
    {
        public static string ConnString = @"Server=sam-pc;Database=Login;User ID=sa;Password=1";
    }
}
```
    上为C#中的，调用为     SqlConnection conn = new SqlConnection(DbUtil.ConnString)
    而在VB.NET中的展示和用法：
```
Module DbUtil
    Public ConnString As String = "Server=sam-pc;Database=Login;User ID=sa;Password=1"
End Module
```
    使用：Dim conn As New SqlConnection(DbUtil.ConnString)
# 一、标准模块（Module）
        标准模块是应用程序内其它模块访问的过程和声明的容器。它们可以包含变量、常数、类型、外部过程和全局过程的全局（在整个应用程序范围内有效的）声明或模块级声明。写入标准模块的代码不必绑在特定的应用程序上。这是在vb6中的定义，也适用VB.NET中声明的这个模块（毕竟在声明的代码上是一样的）。
        简单来说模块就是用来声明各种窗体，类都能调用的公共变量用的一个区域，可以有多个，但是空间是一样的，所以进行使用的时候不用声明就可以用（VB.NET中得引用这个模块才能用）。
# 二、类（class）
        在建立各个文件结构的时候我就考虑过是建模块呢还是建立类呢？这个问题，由于C#中都是类，所以我也是都用的类才会出现这个问题，那么什么又是类呢？
        类实际上是对某种类型的对象变量和方法的原型，类是从一些具有相同属性或功能的具体实例中抽象出共有的一些属性的，自定义抽象数据类型。
        简单来说就是类是抽象的，需要实例化之后才能用其中的功能和数据，所以在使用之前都要先new一下在使用或者传值，其实这个例子使用类也是可以实现的，但是在VB中还不如用模块来的简单，所以我就使用的是模块。
# 三、总结
        既然知道了类和模块的区别，那么我们接下来的使用也就更加的轻松了，如果是带有一定值的变量需要跨窗体或者类去使用，那么就要把他定义在模块中，如果是一个类的属性或者是固有的方法值，就要定义在类中，因为他们是一个有机的整体。
