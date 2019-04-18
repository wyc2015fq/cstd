# C#中error预处理命令的使用 - 小灰笔记 - CSDN博客





2017年10月16日 22:42:03[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：467








            如同warning，这个预处理的命令也似曾相识。在C语言中应该也是有相应的功能的，只是我也没有用过而已。不过，很多接触到的软件工程中都有这样的用法，这个是很肯定的。

            一个简单的Demo程序如下：

using System;

usingSystem.Collections.Generic;

usingSystem.Linq;

usingSystem.Text;

usingSystem.Threading.Tasks;



namespaceerror_demo

{

class
Program

    {

static
void Main(string[] args)

        {

#if XXX

            Console.WriteLine("XXXOK");

#else

#error"NOT OK"

#endif

        }

    }

}

            编译提示如下：

![](https://img-blog.csdn.net/20171016224158468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            从上面看，错误提示起到了作用。这种用法可以用于关键配置信息的检查，相应的配置检查能够在一定程度上保证软件的可靠性以及准确性。

            如果增加XXX的定义，上面自然也就不会出现相应的错误。测试结果也是如此。



