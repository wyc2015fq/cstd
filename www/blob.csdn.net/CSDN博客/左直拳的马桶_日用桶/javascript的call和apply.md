# javascript的call和apply - 左直拳的马桶_日用桶 - CSDN博客
2014年12月08日 21:12:09[左直拳](https://me.csdn.net/leftfist)阅读数：1626
个人分类：[.NET																[JavaScript																[node.js](https://blog.csdn.net/leftfist/article/category/2626561)](https://blog.csdn.net/leftfist/article/category/94500)](https://blog.csdn.net/leftfist/article/category/94497)
coffeescript里，每个文件编译成JS后，都是(function(){...}).call(this);的架势
这个call，该怎么理解呢？
在javascript里面，call 或者 apply 用于使并未定义某个函数的对象也可以使用该函数。换言之，它扩展了该对象，让它忽然多了一个函数。call与apply的区别，仅仅在于参数的形式。
```java
function sayHello(sentence){
	alert(sentence);
}
function leftfist(){}
sayHello.call(leftfist,"Hello World!");
sayHello.apply(leftfist,["Hello World!"]);
```
大概，这就是所谓的语法糖吧？网上的文章都说得很复杂，什么改变运行对象，上下文之类，这当然没有错，但我认为这会越说越复杂，越说越玄虚，无助于了解本质。不及我这个例子那么直达本质。CoffeeScript里这个编译后的call(this)，倒真的是为了限定上下文。
在C#里面，也有类似的功能。它可以很方便地扩展一个现有类的功能，而不会引起已有代码的编译错误或什么变化。新加的功能，完全是额外赠送的，差不多二次开发的性质。
它就是this参数。
没代码我说个J8。
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace ConsoleApplication1
{
    using CustomExtensions;
    class Program
    {
        static void Main(string[] args)
        {
            string str = "leftfist";
            Console.WriteLine(str.HelloWorld());
            Console.Read();
        }
    }
}
namespace CustomExtensions
{
    public static class StringExtensions
    {
        public static string HelloWorld(this string str)
        {
            StringBuilder sb = new StringBuilder(str);
            sb.Append(">Hello World!");
            return sb.ToString();
        }
    }
}
```
上述代码中，string凭空获得了一个函数：HelloWorld()。

