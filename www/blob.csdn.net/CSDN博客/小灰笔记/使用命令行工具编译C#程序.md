# 使用命令行工具编译C#程序 - 小灰笔记 - CSDN博客





2017年09月11日 22:50:14[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1092








            C#可以使用命令行的方式进行编译，其实这个我早就知道了，在我接触Cmake的时候知道的。但是，一直以来我没有系统学习过C#，相关的东西也就没有尝试更没有积累。今天学习的过程中看到了这部分，简单记录小结一下。

            C#的命令行编译程序叫做csc.exe，在使用的时候首先得把这个命令的所在目录加入到Path环境变量。这样会方便一些，免除每次都要输入完整目录的麻烦。

            为了测试演示，编写代码如下：

using static System.Console;



class Program

{

            staticvoid Main()

            {

                        WriteLine("Hello,World!");

            }

}

            代码文件命名为HelloWorld.cs。编译过程如下：

E:\01_workspace\02_programme_language\06_c#\2017\09\command_line_tools>dir

 Volume in drive E has no label.

 Volume Serial Number is 2013-5089



 Directory ofE:\01_workspace\02_programme_language\06_c#\2017\09\command_line_tools



09/11/2017  10:48 PM   <DIR>          .

09/11/2017  10:48 PM   <DIR>          ..

09/11/2017  10:42 PM               115 HelloWorld.cs

               1 File(s)            115 bytes

               2 Dir(s)  78,158,286,848 bytes free



E:\01_workspace\02_programme_language\06_c#\2017\09\command_line_tools>cscHelloWorld.cs

Microsoft (R) Visual C# Compilerversion 1.3.1.60616

Copyright (C) MicrosoftCorporation. All rights reserved.





E:\01_workspace\02_programme_language\06_c#\2017\09\command_line_tools>dir

 Volume in drive E has no label.

 Volume Serial Number is 2013-5089



 Directory ofE:\01_workspace\02_programme_language\06_c#\2017\09\command_line_tools



09/11/2017  10:48 PM   <DIR>          .

09/11/2017  10:48 PM   <DIR>          ..

09/11/2017  10:42 PM               115 HelloWorld.cs

09/11/2017  10:48 PM             3,584 HelloWorld.exe

               2 File(s)          3,699 bytes

               2 Dir(s)  78,158,282,752 bytes free

            运行结果如下：

E:\01_workspace\02_programme_language\06_c#\2017\09\command_line_tools>HelloWorld.exe

Hello, World!



