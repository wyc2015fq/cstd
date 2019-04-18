# C#中warning预处理命令的使用 - 小灰笔记 - CSDN博客





2017年10月16日 22:32:06[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：442








            说起预处理中的警告功能，在C语言中也是存在的。虽说我自己编写代码的时候没有用过，但是看别人的代码以及MATLABsimulink代码生成的代码中经常会看到这种用法。C#中的用法或许与之类似，都是我后期应该学习掌握并应用的一个简单的小技术。

            先写一段Demo代码：

using System;

usingSystem.Collections.Generic;

usingSystem.Linq;

usingSystem.Text;

usingSystem.Threading.Tasks;



namespace warning

{

class
Program

    {

static
void Main(string[] args)

        {

#if XXX

            Console.WriteLine("OK!");

#else

#warning"XXX not defined!"

#endif

        }

    }

}

            如同上面的代码，这种功能可以用于代码编译配置信息的检查。如果信息错误，在编译的时候会出现相应的警告信息。具体类似下面：

![](https://img-blog.csdn.net/20171016223151592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            或者，可以在正确的时候给我们提示一部分成功的提示信息。这都是不错的的点子，也都是值得尝试的功能。



