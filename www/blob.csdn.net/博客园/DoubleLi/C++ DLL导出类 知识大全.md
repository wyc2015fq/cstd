# C++ DLL导出类 知识大全 - DoubleLi - 博客园






在公司使用C++ 做开发，公司的大拿搭了一个C++的跨平台开发框架。在C++开发领域我还是个新手，有很多知识要学，比如Dll库的开发。

   参考了很多这方面的资料，对DLL有一个基本全面的了解。有一个问题让我有点困惑，普通的导入导出C++类的方式都是使用_declspec(dllexport) /_declspec(dllimport)来导入导出类，但是在公司的开发中我们没有导入导出，而是定义了一些只有纯虚函数的抽象类，然后定义了一个工厂类，将这个工厂类注册到框架的服务中心中，使用时从服务中心拿到这个工厂类，就可以创建Dll中的其它类。对这种使用方式我不太理解，google+百度搜索了很多这方面的内容，很多blog讲到了这种使用方式，但是也没有讲清楚这样使用的原理，后来找到了一篇老外写的blog，讲得比较清楚。

   使用只有纯虚函数的抽象类之所以不需要导出，是因为纯虚函数的虚表使然。下面是同老外的bkig中抽出来的一个示例。

   纯虚函数类的定义如下：



**[cpp]**[view plain](http://blog.csdn.net/jietoulangren/article/details/14052533#)[copy](http://blog.csdn.net/jietoulangren/article/details/14052533#)



- // The abstract interface for Xyz object.  
- // No extra specifiers required.  
- struct IXyz  
- {  
- virtual int Foo(int n) = 0;  
- virtual void Release() = 0;  
- };  
- 
- // Factory function that creates instances of the Xyz object.  
- extern "C" XYZAPI IXyz* APIENTRY GetXyz();  




   使用dll的代码如下：





**[cpp]**[view plain](http://blog.csdn.net/jietoulangren/article/details/14052533#)[copy](http://blog.csdn.net/jietoulangren/article/details/14052533#)



- #include "XyzLibrary.h"  
- 
- ...  
- IXyz* pXyz = ::GetXyz();  
- 
- if(pXyz)  
- {  
-     pXyz->Foo(42);  
- 
-     pXyz->Release();  
-     pXyz = NULL;  
- }  


该示例中导出了一个方法来创建IXyz对象，但是并没有导出IXyz对象，IXyz类中只有纯虚函数。这是如何实现的呢？我所知道的是，需要将Dll中的类导出，导出的符号将放到导出符号表中，在链接的时候根据这些符号来定位函数的地址，这个IXyz类没有声明导出，当然类中的函数就不回生成在导出符号表中，那么怎么定位到函数的地址呢？下面这张原文中的图给出了很清晰的解释：



![](http://img.blog.csdn.net/20131102110819171)

图中的伪代码部分解释了函数的调用过程，是通过虚表来定位函数的。因为定义的是只有纯虚函数的抽象类，这样的类编译之后会有一个纯粹的虚表，可以通过这张纯粹的虚表来进行函数调用，所以通过这种方式来使用dll的第一步是应以只带纯虚函数的抽象类，或者说接口。
更多内容，参见下面：





一、导出类的简单方式

这种方式是比较简单的，同时也是不建议采用的不合适方式。

只需要在导出类加上__declspec(dllexport)，就可以实现导出类。对象空间还是在使用者的模块里，dll只提供类中的函数代码。不足的地方是：使用者需要知道整个类的实现，包括基类、类中成员对象，也就是说所有跟导出类相关的东西，使用者都要知道。通过Dependency Walker可以看到，这时候的dll导出的是跟类相关的函数：如构造函数、赋值操作符、析构函数、其它函数，这些都是使用者可能会用到的函数。

这种导出类的方式，除了导出的东西太多、使用者对类的实现依赖太多之外，还有其它问题：必须保证使用同一种编译器。导出类的本质是导出类里的函数，因为语法上直接导出了类，没有对函数的调用方式、重命名进行设置，导致了产生的dll并不通用。

部分代码（DLL头文件）：


![复制代码](https://common.cnblogs.com/images/copycode.gif)
//2011.10.6//cswuyg//dll导出类，比较差劲的方法#pragma once#ifdef NAIVEAPPROACH_EXPORTS#define NAIVEAPPROACH_API __declspec(dllexport)#else#define NAIVEAPPROACH_API __declspec(dllimport)#endif//基类也必须导出，否则警告：class  NAIVEAPPROACH_API CBase{public:    void Test();private:    int m_j;};//也必须导出class NAIVEAPPROACH_API CDate{public:    void Test2();private:    int m_k;};class NAIVEAPPROACH_API CNaiveApproach : public CBase{public:    CNaiveApproach(int i = 0);    // TODO: add your methods here.    void Func();private:    int m_iwuyg;    CDate m_dobj;};
![复制代码](https://common.cnblogs.com/images/copycode.gif)


Demo代码见附件NaiveApproach部分。

二、导出类的较好方式

这种方式是比较合适的，跟com类似。

         结构是这样的：导出类是一个派生类，派生自一个抽象类——都是纯虚函数。使用者需要知道这个抽象类的结构。DLL最少只需要提供一个用于获取类对象指针的接口。使用者跟DLL提供者共用一个抽象类的头文件，使用者依赖于DLL的东西很少，只需要知道抽象类的接口，以及获取对象指针的导出函数，对象内存空间的申请是在DLL模块中做的，释放也在DLL模块中完成,最后记得要调用释放对象的函数。

         这种方式比较好，通用，产生的DLL没有特定环境限制。借助了C++类的虚函数。一般都是采用这种方式。除了对DLL导出类有好处外，采用接口跟实现分离，可以使得工程的结构更清晰，使用者只需要知道接口，而不需要知道实现。

　　部分代码：

（1）DLL头文件：


![复制代码](https://common.cnblogs.com/images/copycode.gif)
//2011.10.6//cswuyg//dll导出类//dll跟其使用者共用的头文件#pragma  once#ifdef MATUREAPPROACH_EXPORTS#define MATUREAPPROACH_API __declspec(dllexport)#else#define MATUREAPPROACH_API __declspec(dllimport)#endifclass IExport{public:    virtual void Hi() = 0;    virtual void Test() = 0;    virtual void Release() = 0;};extern "C" MATUREAPPROACH_API IExport* _stdcall CreateExportObj();extern "C" MATUREAPPROACH_API void _stdcall DestroyExportObj(IExport* pExport);
![复制代码](https://common.cnblogs.com/images/copycode.gif)


（2）导出类头文件：


![复制代码](https://common.cnblogs.com/images/copycode.gif)
//2011.10.6//cswuyg//dll导出类// 实现类#pragma once#include "MatureApproach.h"class ExportImpl : public IExport{public:    virtual void Hi();    virtual void Test();    virtual void Release();    ~ExportImpl();private:};
![复制代码](https://common.cnblogs.com/images/copycode.gif)


         Demo代码见附件MatureApproach部分。

三、总结

导出类是比较简单的，比较容易混淆的概念[上一篇总结](http://www.cnblogs.com/cswuyg/archive/2011/09/30/dll.html)已经说完了。本质上来说，跟导出函数没差别。使用VS2005自动生成的代码可以省去很多力气，比起以前做练习什么都是自己动手写方便多了。要注意一下工程的设置，熟悉它们的作用可以加快编程速度。

Demo代码附件：

[http://files.cnblogs.com/cswuyg/%E7%BC%96%E5%86%99DLL%E6%89%80%E5%AD%A6%E6%89%80%E6%80%9D(2)(%E5%AF%BC%E5%87%BA%E7%B1%BB).rar](http://files.cnblogs.com/cswuyg/%E7%BC%96%E5%86%99DLL%E6%89%80%E5%AD%A6%E6%89%80%E6%80%9D(2)(%E5%AF%BC%E5%87%BA%E7%B1%BB).rar)

参考资料：

[http://www.codeproject.com/KB/cpp/howto_export_cpp_classes.aspx](http://www.codeproject.com/KB/cpp/howto_export_cpp_classes.aspx)








# [编写DLL所学所思(1)——导出函数](http://www.cnblogs.com/cswuyg/archive/2011/09/30/dll.html)



**2010.8.31~2010.9.1总结**

**2011.9.28~30整理**

**烛秋**

# 动态链接库的使用有两种方式，一种是显式调用。一种是隐式调用。

（1）       显式调用：使用LoadLibrary载入动态链接库、使用GetProcAddress获取某函数地址。

（2）       隐式调用：可以使用#pragma comment(lib, “XX.lib”)的方式，也可以直接将XX.lib加入到工程中。



# DLL的编写

编写dll时，有个重要的问题需要解决，那就是函数重命名——Name-Mangling。解决方式有两种，一种是直接在代码里解决采用extent”c”、_declspec(dllexport)、#pragma comment(linker, "/export:[Exports Name]=[Mangling Name]")，另一种是采用def文件。

## （1）编写dll时，为什么有 extern “C”

原因：因为C和C++的重命名规则是不一样的。这种重命名称为“Name-Mangling”（名字修饰或名字改编、标识符重命名，有些人翻译为“名字粉碎法”，这翻译显得有些莫名其妙）

据说，C++标准并没有规定Name-Mangling的方案，所以不同编译器使用的是不同的，例如：Borland C++跟Mircrosoft C++就不同，而且可能不同版本的编译器他们的Name-Mangling规则也是不同的。这样的话，不同编译器编译出来的目标文件.obj 是不通用的，因为同一个函数，使用不同的Name-Mangling在obj文件中就会有不同的名字。如果DLL里的函数重命名规则跟DLL的使用者采用的重命名规则不一致，那就会找不到这个函数。

C标准规定了[C语言](http://lib.csdn.net/base/c)Name-Mangling的规范（林锐的书有这样说过）。这样就使得，任何一个支持[c语言](http://lib.csdn.net/base/c)的编译器，它编译出来的obj文件可以共享，链接成可执行文件。这是一种标准，如果DLL跟其使用者都采用这种约定，那么就可以解决函数重命名规则不一致导致的错误。

影响符号名的除了C++和C的区别、编译器的区别之外，还要考虑调用约定导致的Name Mangling。如extern “c” __stdcall的调用方式就会在原来函数名上加上写表示参数的符号，而extern “c” __cdecl则不会附加额外的符号。

dll中的函数在被调用时是以函数名或函数编号的方式被索引的。这就意味着采用某编译器的C++的Name-Mangling方式产生的dll文件可能不通用。因为它们的函数名重命名方式不同。为了使得dll可以通用些，很多时候都要使用C的Name-Mangling方式，即是对每一个导出函数声明为extern “C”，而且采用_stdcall调用约定，接着还需要对导出函数进行重命名，以便导出不加修饰的函数名。

注意到extern “C”的作用是为了解决函数符号名的问题，这对于动态链接库的制造者和动态链接库的使用者都需要遵守的规则。

动态链接库的显式装入就是通过GetProcAddress函数，依据动态链接库句柄和函数名，获取函数地址。因为GetProcAddress仅是[操作系统](http://lib.csdn.net/base/operatingsystem)相关，可能会操作各种各样的编译器产生的dll，它的参数里的函数名是原原本本的函数名，没有任何修饰，所以一般情况下需要确保dll’里的函数名是原始的函数名。分两步：一，如果导出函数使用了extern”C” _cdecl，那么就不需要再重命名了，这个时候dll里的名字就是原始名字；如果使用了extern”C” _stdcall，这时候dll中的函数名被修饰了，就需要重命名。二、重命名的方式有两种，要么使用*.def文件，在文件外修正，要么使用#pragma，在代码里给函数别名。

## （2）_declspec(dllexport)和_declspec(dllimport)的作用

       _declspec还有另外的用途，这里只讨论跟dll相关的使用。正如括号里的关键字一样，导出和导入。_declspec(dllexport)用在dll上，用于说明这是导出的函数。而_declspec(dllimport)用在调用dll的程序中，用于说明这是从dll中导入的函数。

       因为dll中必须说明函数要用于导出，所以_declspec(dllexport)很有必要。但是可以换一种方式，可以使用def文件来说明哪些函数用于导出，同时def文件里边还有函数的编号。

而使用_declspec(dllimport)却不是必须的，但是建议这么做。因为如果不用_declspec(dllimport)来说明该函数是从dll导入的，那么编译器就不知道这个函数到底在哪里，生成的exe里会有一个call XX的指令，这个XX是一个常数地址，XX地址处是一个jmp dword ptr[XXXX]的指令，跳转到该函数的函数体处，显然这样就无缘无故多了一次中间的跳转。如果使用了_declspec(dllimport)来说明，那么就直接产生call dword ptr[XXX]，这样就不会有多余的跳转了。（参考《加密与解密》第三版279页）

## （3）__stdcall带来的影响

       这是一种函数的调用方式。默认情况下VC使用的是__cdecl的函数调用方式，如果产生的dll只会给C/C++程序使用，那么就没必要定义为__stdcall调用方式，如果要给Win32汇编使用（或者其他的__stdcall调用方式的程序），那么就可以使用__stdcall。这个可能不是很重要，因为可以自己在调用函数的时候设置函数调用的规则。像VC就可以设置函数的调用方式，所以可以方便的使用win32汇编产生的dll。不过__stdcall这调用约定会Name-Mangling，所以我觉得用VC默认的调用约定简便些。但是，如果既要__stdcall调用约定，又要函数名不给修饰，那可以使用*.def文件，或者在代码里#pragma的方式给函数提供别名（这种方式需要知道修饰后的函数名是什么）。



举例：



·extern “C” __declspec(dllexport) bool  __stdcall cswuyg();

·extern “C”__declspec(dllimport) bool __stdcall cswuyg();



·#pragma comment(linker, "/export:cswuyg=_cswuyg@0")



## （4）*.def文件的用途

指定导出函数，并告知编译器不要以修饰后的函数名作为导出函数名，而以指定的函数名导出函数（比如有函数func，让编译器处理后函数名仍为func）。这样，就可以避免由于microsoft VC++编译器的独特处理方式而引起的链接错误。

也就是说，使用了def文件，那就不需要extern “C”了，也可以不需要__declspec(dllexport)了（不过，dll的制造者除了提供dll之外，还要提供头文件，需要在头文件里加上这extern”C”和调用约定，因为使用者需要跟制造者遵守同样的规则，除非使用者和制造者使用的是同样的编译器并对调用约定无特殊要求）。

举例def文件格式：

LIBRARY  XX(dll名称这个并不是必须的，但必须确保跟生成的dll名称一样)

EXPORTS

[函数名] @ [函数序号]



编写好之后加入到VC的项目中，就可以了。

       另外，要注意的是，如果要使用__stdcall，那么就必须在代码里使用上__stdcall，因为*.def文件只负责修改函数名称，不负责调用约定。

也就是说，def文件只管函数名，不管函数平衡堆栈的方式。



如果把*.def文件加入到工程之后，链接的时候并没有自动把它加进去。那么可以这样做：

手动的在link添加：

1）工程的propertiesàConfiguration PropertiesàLinkeràCommand Lineà在“Additional options”里加上：/def:[完整文件名].def

2）工程的propertiesàConfiguration PropertiesàLinkeràInputàModule Definition File里加上[完整文件名].def



注意到：即便是使用C的名称修饰方式，最终产生的函数名称也可能是会被修饰的。例如，在VC下，_stdcall的调用方式，就会对函数名称进行修饰，前面加‘_’，后面加上参数相关的其他东西。所以使用*.def文件对函数进行命名很有用，很重要。

## （5）、DllMain函数

每一个动态链接库都会有一个DllMain函数。如果在编程的时候没有定义DllMain函数，那么编译器会给你加上去。

DllMain函数格式：

BOOL APIENTRY DllMain( HANDLE hModule,

                       DWORD  ul_reason_for_call,

                       LPVOID lpReserved

                              )

{

     switch(ul_reason_for_call)

     {

     case DLL_PROCESS_ATTACH:

            printf("\nprocess attach of dll");

            break;

     case DLL_THREAD_ATTACH:

            printf("\nthread attach of dll");

            break;

     case DLL_THREAD_DETACH:

            printf("\nthread detach of dll");

            break;

     case DLL_PROCESS_DETACH:

            printf("\nprocess detach of dll");

            break;

     }

    return TRUE;

}

## （6）、很多都还没学，如：导出Class、导出变量、DLL更高级的应用。目前先了解点基础知识。以后补上。





















#  2011-8-14补充



编写dll可以使用.def文件对导出的函数名进行命名。



## 1、动态装入dll，重命名（*.def）的必要性？

因为导出的函数尽可能使用__stdcall的调用方式。而__stdcall的调用方式，无论是C的Name Mangling，还是C++的Name Mangling都会对函数名进行修饰。所以，采用__stdcall调用方式之后，必须使用*.def文件对函数名重命名，不然就不能使用GetProcAddress()通过函数名获取函数指针。



## 2、隐式调用时，头文件要注意的地方？

因为使用静态装入，需要有头文件声明这个要被使用的dll中的函数，如果声明中指定了__stdcall或者extern “C”，那么在调用这个函数的时候，编译器就通过Name Mangling之后的函数名去.lib中找这个函数，*****.**def中的内容是对*.lib里函数的名称不产生作用，*.def文件里的函数重命名只对dll有用**。这就有lib 跟dll里函数名不一致的问题了，但并不会产生影响，DLL的制造者跟使用者采用的是一致函数声明。



## 3、所以到底要不要使用__stdcall 呢？

我看到一些代码里是没有使用__stdcall的。如果不使用__stdcall，而使用默认的调用约定_cdecl，并且有extern ”C”。那么VC是不会任何修饰的。这样子生成的dll里的函数名就是原来的函数名。也就可以不使用.def文件了。

也有一些要求必须使用__stdcall，例如com相关的东西、系统的回调函数。具体看有没有需要。





## 4、导出函数别名怎么写？

可以在.def文件里对函数名写一个别名。

例如：

EXPORTS

cswuygTest（别名） = _showfun@4(要导出的函数)



或者：

#pragma comment(linker, "/export:[别名] =[NameMangling后的名称]")



这样做就可以随便修改别名了，不会出现找不到符号的错误。



## 5、用不用*.def文件？

如果采用VC默认的调用约定，可以不用*.def文件，如果要采用__stdcall调用约定，又不想函数名被修饰，那就采用*.def文件吧,另一种在代码里写的重命名的方式不够方便。

## 6、什么情况下（不）需要考虑函数重命名的问题？

1）、隐式调用（通过lib）

如果dll的制造者跟dll的使用者采用同样的语言、同样编程环境，那么就不需要考虑函数重命名。使用者在调用函数的时候，通过Name Mangling后的函数名能在lib里找到该函数。

如果dll的制造者跟dll使用不同的语言、或者不同的编译器，那就需要考虑重命名了。

2）、显示调用（通过GetProcessAddress）

       这绝对是必须考虑函数重命名的。

## 7、总结

    总的来说，在编写DLL的时候，写个头文件，头文件里声明函数的NameMingling方式、调用约定（主要是为了隐式调用）。再写个*.def文件把函数重命名了（主要是为了显式调用）。提供*.DLL\*.lib\*.h给dll的使用者，这样无论是隐式的调用，还是显式的调用，都可以方便的进行。



附：

  一个简单DLL导出函数的例子：[http://files.cnblogs.com/cswuyg/%E7%BC%96%E5%86%99DLL%E6%89%80%E5%AD%A6%E6%89%80%E6%80%9D.rar](http://files.cnblogs.com/cswuyg/%E7%BC%96%E5%86%99DLL%E6%89%80%E5%AD%A6%E6%89%80%E6%80%9D.rar)



# 学习资料：

http://www.cnblogs.com/dongzhiquan/archive/2009/08/04/1994764.html

http://topic.csdn[.net](http://lib.csdn.net/base/dotnet)/u/20081126/14/70ac75b3-6e79-4c48-b9fe-918dce147484.html


[](http://blog.csdn.net/jietoulangren/article/details/14052533)









