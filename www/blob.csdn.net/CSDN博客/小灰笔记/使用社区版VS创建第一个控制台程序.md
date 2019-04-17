# 使用社区版VS创建第一个控制台程序 - 小灰笔记 - CSDN博客





2017年07月10日 07:39:49[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：375








       先前看Linux的故事看多了，看得我有些不是很喜欢微软的技术。说起来，微软的很多产品确实是多了那么几分令人不安的因素。不过，如果实在Windows平台上混日子，又想做点什么应用程序开发，相信没有什么比C#更方便了。

       前年的时候了解了一丁点儿的C#，但是一直也没有学完整个语言体系的基础知识。我觉得障碍还是在于编程思想的阻碍，目前我还是处于C语言的世界更多一些。接下来通过Python掌握面向对象编程的技能之后，尽快把C#也掌握一下才好！

       曾经VS全都是收费的，倒是有一个VCE是免费的，可以用于C#应用程序的开发。现在倒是出现了一个社区版可以免费用，而且功能还是比较完整的。迫不及待赶紧安装了一下，或许由于服务器的位置原因，账号认证上遇到不少小问题。

       现在用VS的社区版来创建第一个控制台程序：

1，创建解决方案，选择控制台程序。

![](https://img-blog.csdn.net/20170710073857738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




2，输入熟悉的hello world

using System;

usingSystem.Collections.Generic;

usingSystem.Linq;

usingSystem.Text;

usingSystem.Threading.Tasks;



namespaceConsoleApplication1

{

class
Program

   {

static
void Main(string[] args)

       {

Console.WriteLine("Hello,World!");

Console.ReadKey();

       }

   }

}

3，编译运行后的结果如下

![](https://img-blog.csdn.net/20170710073919969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




其实，这个简单的程序并不能够说明太多，只是学习路上的一个起点。接下来，在C#上还是要继续往深处研究一下。很多时候，学习的障碍也就是是否能够找到合适的开发工具，现在有了这个社区版本VS，一切都将会顺利多了。



