# C#中的局部变量冲突 - 小灰笔记 - CSDN博客





2017年09月12日 23:39:38[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：663








            一个变量在同一个作用域中不能够声明两次，如下代码错误。

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

int a =123;

int a =456;

        }

    }

}

       编译错误如下：
|严重性|代码|说明|项目|文件|行|禁止显示状态|
|----|----|----|----|----|----|----|
|错误|CS0128|已在此范围定义了名为“a”的局部变量。|ConsoleApplication1|E:\01_workspace\02_programme_language\06_c#\2017\09\varConflict\ConsoleApplication1\ConsoleApplication1\Program.cs|14|活动|
|警告|CS0219|变量“a”已被赋值，但从未使用过它的值|ConsoleApplication1|E:\01_workspace\02_programme_language\06_c#\2017\09\varConflict\ConsoleApplication1\ConsoleApplication1\Program.cs|13|活动|
|警告|CS0219|变量“a”已被赋值，但从未使用过它的值|ConsoleApplication1|E:\01_workspace\02_programme_language\06_c#\2017\09\varConflict\ConsoleApplication1\ConsoleApplication1\Program.cs|14|活动|



            但是，如下代码正常：

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

for (int a = 0;a < 10; a++)

            {

Console.WriteLine(a);

            }

for (int a = 0;a < 10; a++)

            {

Console.WriteLine(a* 2);

            }

        }

    }

}

            编译后执行结果：

![](https://img-blog.csdn.net/20170912233927544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            原因：在第一个循环结束后，a的作用域已经跳出。



