# VS中定义的DEBUG常量 - 小灰笔记 - CSDN博客





2017年10月16日 22:23:10[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：941








            学习C#的过程中按照教程的描述自己敲了一段代码，具体如下：

using System;

usingSystem.Collections.Generic;

usingSystem.Linq;

usingSystem.Text;

usingSystem.Threading.Tasks;



namespaceConsoleApplication4

{

class
Program

    {

static
void Main(string[] args)

        {

#if DEBUG

Console.WriteLine("debug");

#else

            Console.WriteLine("non-debug");

#endif

        }

    }

}

            按照我自己的理解，这个应该是执行第二个输出语句。因为代码中我并没有通过预处理指令定义DEBUG。然而编译执行结果却与我考虑的不一致：

![](https://img-blog.csdn.net/20171016222259241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            这确实是有点费解，但是联想到了之前使用的基于eclipes的C开发环境，很多宏定义都是在配置选项中的。尝试查找了一下，果真找到了相应的信息：

![](https://img-blog.csdn.net/20171016222310097?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            把项目属性中上面的信息勾掉再次测试，结果跟预期的一样。



