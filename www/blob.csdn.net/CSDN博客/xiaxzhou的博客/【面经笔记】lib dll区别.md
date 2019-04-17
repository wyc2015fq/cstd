# 【面经笔记】lib dll区别 - xiaxzhou的博客 - CSDN博客





2017年07月20日 21:49:46[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：217
个人分类：[面试题](https://blog.csdn.net/xiaxzhou/article/category/6896019)









> 
[http://www.cppblog.com/amazon/archive/2009/09/04/95318.html](http://www.cppblog.com/amazon/archive/2009/09/04/95318.html)


### 两种库：

一种是LIB包含了函数所在的DLL文件和文件中函数位置的信息（入口），代码由运行时加载在进程空间中的DLL提供，称为**动态链接库**

一种是LIB包含函数代码本身，在编译时直接将代码加入程序当中，称为**静态链接库**

共有两种链接方式： 

动态链接使用动态链接库，允许可执行模块（.dll文件或.exe文件）仅包含在运行时定位DLL函数的可执行代码所需的信息。 

静态链接使用静态链接库，链接器从静态链接库LIB获取所有被引用函数，并将库同代码一起放到可执行文件中。
### **关于lib和dll的区别如下：**

（1）lib是编译时用到的，dll是运行时用到的。如果要完成源代码的编译，只需要lib；如果要使动态链接的程序运行起来，只需要dll。 

（2）如果有dll文件，那么lib一般是一些索引信息，记录了dll中函数的入口和位置，dll中是函数的具体内容；如果只有lib文件，那么这个lib文件是静态编译出来的，索引和实现都在其中。使用静态编译的lib文件，在运行程序时不需要再挂动态库，缺点是导致应用程序比较大，而且失去了动态库的灵活性，发布新版本时要发布新的应用程序才行。 

（3）动态链接的情况下，有两个文件：一个是LIB文件，一个是DLL文件。LIB包含被DLL导出的函数名称和位置，DLL包含实际的函数和数据，应用程序使用LIB文件链接到DLL文件。在应用程序的可执行文件中，存放的不是被调用的函数代码，而是DLL中相应函数代码的地址，从而节省了内存资源。DLL和LIB文件必须随应用程序一起发行，否则应用程序会产生错误。
#### **没有lib，可以直接用dll吗？**

如果不想用lib文件或者没有lib文件，可以用WIN32 API函数LoadLibrary、GetProcAddress装载：

需要函数指针和WIN32 API函数LoadLibrary、GetProcAddress装载，使用这种载入方法，不需要.lib文件和.h头文件，只需要.dll文件即可（将.dll文件置入工程目录中）。

```cpp
#include <iostream>
#include <windows.h>         //使用函数和某些特殊变量
typedef void (*DLLFunc)(int);
int main()
{
        DLLFunc dllFunc;
        HINSTANCE hInstLibrary = LoadLibrary("DLLSample.dll");

        if (hInstLibrary == NULL)
        {
          FreeLibrary(hInstLibrary);
        }
        dllFunc = (DLLFunc)GetProcAddress(hInstLibrary, "TestDLL");
        if (dllFunc == NULL)
        {
          FreeLibrary(hInstLibrary);
        }
        dllFunc(123);
        std::cin.get();
        FreeLibrary(hInstLibrary);
        return(1);
}
```

LoadLibrary函数利用一个名称作为参数，获得DLL的实例（HINSTANCE类型是实例的句柄），通常调用该函数后需要查看一下函数返回是否成功，如果不成功则返回NULL（句柄无效），此时调用函数FreeLibrary释放DLL获得的内存。 

GetProcAddress函数利用DLL的句柄和函数的名称作为参数，返回相应的函数指针，同时必须使用强转；判断函数指针是否为NULL，如果是则调用函数FreeLibrary释放DLL获得的内存。此后，可以使用函数指针来调用实际的函数。 

最后要记得使用FreeLibrary函数释放内存。





