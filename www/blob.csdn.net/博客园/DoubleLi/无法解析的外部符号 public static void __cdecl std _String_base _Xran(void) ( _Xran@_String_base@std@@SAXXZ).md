# 无法解析的外部符号 "public: static void __cdecl std::_String_base::_Xran(void)" (?_Xran@_String_base@std@@SAXXZ)" - DoubleLi - 博客园






采用下面的方法。重新编译了一下依赖的库，OK了。

**  问题描述：**

      今天用VS2010编译一个在VS2008下Coding的工程的时候，VS给出了一堆链接错误信息，如下图所示：

![](https://images.cnblogs.com/cnblogs_com/royenhome/2222.jpg)

      在ErrorList里面列出了34个链接错误信息，但仔细看的话，其实很多都是重复的，只有两三个外部符号未正确解析，如下：

      "public: static void __cdecl std::_String_base::_Xran(void)" (?_Xran@_String_base@std@@SAXXZ)

      "public: static void __cdecl std::_String_base::_Xlen(void)" (?_Xran@_String_base@std@@SAXXZ)



**问题分析： **

       首先，可以确定是程序在Build(构建)过程中，链接静态链接库libprotobuf.lib的时候出现外部符号无法解析的问题，而且可以看出 libprotobuf.lib库中的很多的中间目标文件(Windows下以.obj作后缀名的文件)都对此外部符号存在着引用，但在符号重定位时候却 在全局符号表中找不到引用的符号。        

       而_Xran和_Xlen这两个符号到底是什么呢，通过google可以大致了解到时string类里面的两个符号，难道问题出在对于string类的引 用么？那么我们可以通过下术的步骤来观察一个引用string头文件的源文件经过IDE的预处理后是怎么样的   

      1、新建一个名为Test.CPP文件,内容很简单，只是包含一个string头文件，即#include<string> 

  　 2、打开VS带的Command Prompt，输入命令 CL /EP Test.cpp > D:\\1.txt

      上述命令为使用MSVC的编译器对Test.cpp进行预处理操作,并将内容重定向到1.txt中。    

      情况一：

      如果你用的VS2005或者VS2008下的命令行工具的话，在打开1.txt后其实可以搜索到如下的内容：

![](https://images.cnblogs.com/cnblogs_com/royenhome/22222.PNG)



      情况2：

      如果你使用的是VS2010的话，那么在产生的1.txt中搜索_Xlen与_Xran的话，则为如下的内容：

![](https://images.cnblogs.com/cnblogs_com/royenhome/33333.PNG)

       补充：其中第二个_Xran与_Xlen是在类basic_string中定义的，显然与VS2005与VS2008下产生的不同。



** 问题总结：**

       静态链接库libprotobuf.lib在旧版本IDE上编译的，所以string类中符号被解析成形如static void __cdecl _Xlen() 

       而在高版本的VS2010上string中的_Xlen与_Xran符号则被解析成了__declspec(noreturn) void _Xlen() const

       所以才会在链接过程中出现上述开头出现的一堆问题



**问题解决：**

       第一种方法:

 　　 　　重新在高版本的IDE下编译libprotobuf.lib，这种方法最行之有效啦，也很简单

       第二种方法：

             可以新建一个链接库，导出无法解析的几个符号，并这几个符号实现中重定向到VS2010下预处理后的那几个符号即可。具体参考这个网址：[http://social.msdn.microsoft.com/Forums/en/vcgeneral/thread/5ac28a31-3339-4db1-90f4-68edad360309](http://social.msdn.microsoft.com/Forums/en/vcgeneral/thread/5ac28a31-3339-4db1-90f4-68edad360309) 里面的JN123给出的方法 



        欢迎装载，装载请注明出处：[http://www.cnblogs.com/royenhome](http://www.cnblogs.com/royenhome)









