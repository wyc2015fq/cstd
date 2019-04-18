# 从Windows 移植到 UNIX 环境 - ljx0305的专栏 - CSDN博客
2012年10月26日 22:58:40[ljx0305](https://me.csdn.net/ljx0305)阅读数：461
个人分类：[Linux](https://blog.csdn.net/ljx0305/article/category/364133)
从Windows 移植到 UNIX 环境
大多数基于 Microsoft® Windows® 的项目都是使用 Microsoft Visual Studio® 构建的，这是一种复杂的集成开发环境 (IDE)，它可以为开发人员实现几乎整个构建过程的自动化。此外，Windows 开发人员使用了 Windows 平台特定的应用程序程序接口 (API)、头文件和语言扩展。大多数类 UNIX® 系统，如 SunOS、OpenBSD 和 IRIX，都不支持 IDE 或者任何 Windows 特定的头文件或扩展，因此进行移植是一项非常耗费时间的活动。更麻烦的是，遗留的基于
 Windows 的代码需要运行于 16 位或者 32 位的 x86 体系结构中。基于 UNIX 的环境通常是 64 位的，并且大多数 UNIX 供应商都不支持 x86 指令集。本系列文章共由两个部分组成，本文是其中的第一部分，介绍将 Windows 操作系统中一个典型的 Visual C++ 项目移植到 SunOS 中的
`g++` 环境的过程，同时详细说明了如何解决前面提到的一些问题。 
Visual Studio 中的 C/C++ 项目类型
您可以使用 Visual C++ 项目创建三种项目变体（单线程或者多线程）中的一种：
- 动态链接库（DLL 或者 .dll）
- 静态库（LIB 或者 .lib）
- 可执行文件（.exe）
对于更复杂的变体，可以使用 Visual Studio .NET 解决方案，这种解决方案允许创建和管理多个项目。本文在下面的几个部分中将重点关注如何将动态和静态库项目变体从 Windows 移植到 UNIX。
将 DLL 移植到 UNIX 环境
对于 Windows 中的 .dll 文件，UNIX 的等价物是共享对象 (.so) 文件。然而，创建一个 .so 文件的过程与创建一个 .dll 文件的过程完全不同。请考虑[清单 1](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list1) 中的示例，在这个示例中，您尝试创建一个小的 .dll 文件，其中仅包含一个函数
`printHello`，并且在 main.cpp 文件的 main 例程中调用了这个函数。 
**清单 1. 包含 printHello 例程声明的文件 hello.h **
                
#ifdef BUILDING_DLL
  #define PRINT_API __declspec(dllexport)
#else
  #define PRINT_API __declspec(dllimport)
#endif
extern "C" PRINT_API void printHello();
[清单 2](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list2) 提供了 hello.cpp 的源代码。
**清单 2. 文件 hello.cpp**
                
#include <iostream>
#include "hello.h"
void printHello  
  {
  std::cout << "hello Windows/UNIX users\n";
  }
extern "C" PRINT_API void printHello();
如果您使用了用于 80x86 平台的 Microsoft 32 位 `C/C++` 标准编译器 (`cl`)，那么可以使用下面的命令来创建 hello.dll 文件：
cl /LD  hello.cpp /DBUILDING_DLL
`/LD` 指示 `cl` 创建一个 .dll 文件。（还可以指示它创建其他格式的文件，如 .exe 或者 .obj。）`/DBUILDING_DLL` 为这个特定的构建过程定义了
`PRINT_API` 宏，以便从这个 DLL 导出 printHello 符号。 
[清单 3](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list3) 包含了 main.cpp main 源文件，其中使用了 printHello 例程。这里所做的假设是，hello.h、hello.cpp 和 main.cpp 都位于相同的文件夹中。
**清单 3. 使用 printHello 例程的 main 的源代码**
                
#include "hello.h"
int main ( )
  {
  printHello();
  return 0;
  }
要编译并连接 main 代码，可以使用下面的命令行： 
cl main.cpp hello.lib
快速地查看源文件和生成的输出，其中说明了两个重要的问题。第一点，要从一个 DLL 中导出任何函数、变量、或者类，都需要使用 Windows 特定的语法 
```
__declspec(dllexport)
```
。同样地，要向一个 DLL 导入任何函数、变量、或者类，都需要使用 Windows 特定的语法 
```
__declspec(dllimport)
```
。第二点，这个编译过程生成了两个文件：printHello.dll 和 printHello.lib。PrintHello.lib 用于连接 main 源文件，而 UNIX 中共享对象的头文件不需要
`declspec` 语法。成功的编译过程将输出一个 .so 文件，它已经与 main 源文件进行了连接。 
要在 UNIX 平台中使用 `g++` 创建一个共享库，需要通过向 `g++` 传递 `-fPIC` 标志，将所有的源文件编译为可重定位的共享对象。PIC 表示*位置无关代码 (position independent code)*。在每次加载一个共享库时，可以将其潜在地映射为一个新的内存地址。因此，需要通过某种很容易进行计算的方式在库中生成所有变量和函数的地址（相对于加载该库的起始地址）。这个代码由
`-fPIC` 选项生成，并使得代码成为可重定位的。`-o` 选项用于指定输出文件的名称，而 `-shared` 选项用于构建一个共享库，其中允许出现未解析的引用。要创建 hello.so 文件，您必须修改头文件，如下面的[清单 4](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list4) 所示。
**清单 4. 包含 UNIX 特定更改的、经过修改的 hello.h 头文件**
                
#if defined (__GNUC__) && defined(__unix__)
  #define PRINT_API __attribute__ ((__visibility__("default")))
#elif defined (WIN32)
  #ifdef BUILDING_DLL
    #define PRINT_API __declspec(dllexport)
  #else
    #define PRINT_API __declspec(dllimport)
#endif
extern "C" PRINT_API void printHello();
下面的 `g++` 命令用于连接共享库 hello.so： 
g++ -fPIC -shared hello.cpp -o hello.so
要创建 main 可执行文件，请编译源代码： 
g++ -o main main.cpp hello.so
g++ 中的符号隐藏
有两种典型的方式可以从一个基于 Windows 的 DLL 中导出符号。第一种方法是仅对从 DLL 中导出的选择元素（例如，类、全局变量或者全局函数）使用 
```
__declspec(dllexport)
```
。第二种方法是使用一个模块-定义 (.def) 文件。.def 文件具有自己的语法，并且包含需要从 DLL 中导出的符号。
`g++` 连接器的缺省行为是从一个 .so 文件中导出所有的符号。这可能并不是所需要的，并且将使得连接多个 DLL 变成一项非常耗时的任务。为了从一个共享库中有选择地导出符号，可以使用
`g++` 属性机制。例如，可以考虑用户源代码中包含两个方法，`'void print1();'` 和 `' int print2(char*);'`，并且用户只需要导出 print2。[清单 5](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list5) 包含一种实现这个目的的方法，可用于 Windows 和 UNIX。
**清单 5. g++ 中的符号隐藏**
                
#ifdef _MSC_VER // Visual Studio specific macro
  #ifdef BUILDING_DLL
    #define DLLEXPORT __declspec(dllexport)
  #else
    #define DLLEXPORT __declspec(dllimport)
  #endif
  #define DLLLOCAL 
#else 
  #define DLLEXPORT __attribute__ ((visibility("default")))
  #define DLLLOCAL   __attribute__ ((visibility("hidden")))
#endif 
extern "C" DLLLOCAL void print1();         // print1 hidden 
extern "C" DLLEXPORT int print2(char*); // print2 exported
使用 `__attribute__ ((visibility("hidden")))` 可以防止从 DLL 中导出符号。最新版本的 
```
g++
```
（4.0.0 以及更高的版本）还提供了 `-fvisibility` 开关，您可以使用它从一个共享库中有选择地导出相关符号。在命令行中使用
`g++` 加上 `-fvisibility=hidden` 延迟从共享库中导出所有的符号，除了那些使用 
```
__attribute__ ((visibility("default")))
```
 声明的符号。这是一种非常简洁的方式，用于通知 `g++` 没有显式地标注可见属性的每项声明，其可见性都是隐藏的。使用
`dlsym` 提取一个隐藏的符号将会返回 `NULL`。 
g++ 中的属性机制概述
与 Visual Studio 环境非常相似（Visual Studio 环境在 `C/C++` 的基础上提供了许多附加的语法），`g++` 也支持该语言的许多非标准扩展。在
`g++` 中，属性机制的用途之一就是便于进行移植。前面的示例讨论了符号隐藏。属性的另一个用途是为 Visual C++ 设置函数类型，如 
```
cdecl
```
、`stdcall` 和 `fastcall`。本系列文章的第 2 部分将详细地介绍属性机制。
在 UNIX 环境中显式加载 DLL 或者共享对象
在 Windows 系统中，可以由 Windows 程序显式地加载一个 .dll 文件，这是很常见的情况。例如，可以考虑一个复杂的、提供了打印功能的、基于 Windows 的编辑器。在用户第一次提出相应请求的时候，这种编辑器将动态地加载打印机驱动程序 DLL。基于 Windows 的开发人员可以使用 Visual Studio 提供的 API，如
`LoadLibrary` 显式地加载一个 DLL，`GetProcAddress` 用于查询 DLL 中的符号，而 
```
FreeLibrary
```
 则用于卸载一个显式加载的 DLL。对于这些函数，UNIX 的等价物分别是 `dlopen`、`dlsym` 和
`dlclose` 例程。而且在 Windows 中，有一个特殊的 `DllMain` 方法，在第一次将 DLL 加载到内存时将调用这个方法。类 UNIX 系统提供了一个对应的方法，称为
`_init`。 
可以考虑前面示例的一个变体。[清单 6](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list6) 中是 loadlib.h 头文件，在调用 main 方法的源文件中使用了这个文件。
**清单 6. 头文件 loadlib.h**
                
#ifndef  __LOADLIB_H
#define  __LOADLIB_H
#ifdef UNIX
#include <dlfcn.h>
#endif 
#include <iostream>
using namespace std;
typedef void* (*funcPtr)();
#ifdef UNIX
#  define IMPORT_DIRECTIVE __attribute__((__visibility__("default")))
#  define CALL  
#else
#  define IMPORT_DIRECTIVE __declspec(dllimport) 
#  define CALL __stdcall
#endif
extern "C" {
  IMPORT_DIRECTIVE void* CALL LoadLibraryA(const char* sLibName); 
  IMPORT_DIRECTIVE funcPtr CALL GetProcAddress(
                                    void* hModule, const char* lpProcName);
  IMPORT_DIRECTIVE bool CALL  FreeLibrary(void* hLib);
}
#endif
main 方法现在显式地加载 printHello.dll 文件，并调用相同的 `print` 方法，如所示[清单 7](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list7) 中所示。
**清单 7. 主文件 Loadlib.cpp**
                
#include "loadlib.h"
int main(int argc, char* argv[])
  {
  #ifndef UNIX
    char* fileName = "hello.dll";
    void* libraryHandle = LoadLibraryA(fileName);
    if (libraryHandle == NULL)
      cout << "dll not found" << endl;
    else  // make a call to "printHello" from the hello.dll 
      (GetProcAddress(libraryHandle, "printHello"))();
    FreeLibrary(libraryHandle);
#else // unix
    void (*voidfnc)(); 
    char* fileName = "hello.so";
    void* libraryHandle = dlopen(fileName, RTLD_LAZY);
    if (libraryHandle == NULL)
      cout << "shared object not found" << endl;
    else  // make a call to "printHello" from the hello.so
      {
      voidfnc = (void (*)())dlsym(libraryHandle, "printHello"); 
      (*voidfnc)();
      }
    dlclose(libraryHandle);
  #endif
  return 0;
  }
Windows 和 UNIX 环境中的 DLL 搜索路径
在 Windows 操作系统中，按照下面的顺序搜索 DLL： 
- 可执行文件所处的目录（例如，notepad.exe 位于 Windows 目录中） 
- 当前工作目录（即，从哪个目录启动了 notepad.exe。） 
- Windows 系统目录（通常为 C:\Windows\System32） 
- Windows 目录（通常为 C:\Windows） 
- 作为 PATH 环境变量中的一部分所列举的目录 
在类 UNIX 系统中，如 Solaris，LD_LIBRARY_PATH 环境变量可以指定共享库搜索顺序。指向一个新的共享库的路径需要追加到 LD_LIBRARY_PATH 变量末尾。HP-UX 的搜索顺序包括作为 LD_LIBRARY_PATH 的一部分所列举的目录，然后是 SHLIB_PATH 中列举的目录。对于 IBM AIX® 操作系统，由 LIBPATH 变量确定共享库搜索顺序。
将静态库从 Windows 移植到 UNIX 
与动态库不同，在编译应用程序时对静态库的目标代码进行连接，并且因此成为该应用程序的一部分。在 UNIX 系统中，静态库遵循一种命名规范，使用 lib 作为前缀，而使用 .a 作为库名的后缀。例如在 UNIX 系统中，Windows 的 user.lib 文件通常被命名为 libuser.a。操作系统提供的命令
`ar` 和 `ranlib` 可用于创建静态库。[清单 8](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list8) 说明了如何从 user_sqrt1.cpp 和 user_log1.cpp 源文件创建一个静态库 libuser.a。
**清单 8. 在 UNIX 环境中创建静态库**
                
g++ -o user_sqrt1.o -c user_sqrt1.cpp 
g++ -o user_log1.o -c user_log1.cpp
ar rc libuser.a user_sqrt1.o user_log1.o 
ranlib libuser.a
`ar` 工具创建了静态库 libuser.a，并将 user_sqrt1.o 和 user_log1.o 目标文件的副本放置于其中。如果存在一个现有的库文件，那么将目标文件添加到其中。如果所使用的目标文件比库中的文件更新一些，那么则替换旧的目标文件。`r` 标志表示使用相同目标文件的更新版本替换库中旧的目标文件。如果这个库并不存在，那么
`c` 选项将创建这个库。 
在创建了一个新的存档文件，或者修改了一个现有的存档文件之后，需要创建存档文件内容的索引，并将其作为该存档文件的一部分进行存储。这个索引列出了存档文件的成员（可重定位目标文件）所定义的每个符号。该索引可以提高与静态库进行连接的速度，并允许调用库中的例程，而不考虑它们在库中的实际位置。请注意，GNU
`ranlib` 是 `ar` 工具的扩展，并且使用 `s` 参数调用 `ar`，`[ar -s]` 与调用
`ranlib` 具有相同的效果。 
预编译头文件
在 Visual C++ 中，基于 `C/C++` 的应用程序通常会使用预编译头文件。预编译头文件是某些编译器（如 Visual Studio 中的
`cl`）的一项性能特性，它可以帮助提高编译的速度。复杂的应用程序通常会使用头文件（.h 或者 .hpp）文件，它们是需要作为一部分进行包括的一个或多个源文件的代码部分。在一个项目的范围内，很少对头文件进行修改。因此，为了提高编译的速度，可以将这些文件转换为一种编译器更容易理解的中间形式，以便提高后续编译工作的速度。在 Visual Studio 环境中，这种中间形式称为预编译头文件或者 PCH。
考虑本文前面清单 [1](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list1) 和 [2](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list2) 中包括 hello.cpp 的示例。其中包含了 `iostream` 和 `EXPORT_API` 宏的定义，在该项目的范围内，这些可以被看作是该文件中不变的代码部分。因此，它们适合放在一个头文件中进行包含。[清单 9](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list9) 显示了可能会发生相关更改的代码。
**清单 9. precomp.h 的内容**
                
#ifndef __PRECOMP_H
#define __PRECOMP_H
#include <iostream>
#  if defined (__GNUC__) && defined(__unix__)
#    define EXPORT_API __attribute__((__visibility__("default")))
#  elif defined WIN32
#    define EXPORT_API __declspec(dllexport) 
#  endif
[清单 10](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html#list10) 显示了 DLL 的源代码，其中包括相关的更改。
**清单 10. 新的 hello.cpp 文件的内容**
                
#include "precomp.h"
#pragma hdrstop
extern "C" EXPORT_API void printHello()
  {
  std::cout << "hello Windows/UNIX users" << std::endl;
  }
正如其名称所表示的，预编译头文件在 *头中止 (header stop)* 点之前，以一种经过编译的形式包含目标代码。源文件中的这个点通常由一个词素进行标记，而预处理程序不会使用该词素作为一个语言符号，这表示它并不是一项预处理程序指令。或者，还可以将这个头中止点指定为
`#pragma hdrstop`，如果在源文本中，它出现在一个有效的非预处理程序语言关键字之前。 
在 Solaris 中进行编译时，当碰到 `#include` 时，将搜索预编译头文件。在搜索包含文件的过程中，编译器首先在每个目录中查找预编译头文件，然后再在这些目录中搜索包含文件。需要搜索的名称可以在带
`.gch` 的 `#include` 中进行指定。如果无法使用这个预编译头文件，那么将忽略它。 
下面的命令行可用于在 Windows 中实现预编译头文件功能： 
cl /Yc precomp.h hello.cpp /DWIN32 /LD
`/Yc` 通知 `cl` 编译器从 precomp.h 生成预编译头文件。可以使用下面的命令在 Solaris 中实现相同的功能：
g++ precomp.h
g++ -fPIC -G hello.cpp -o hello.so
第一个命令创建了预编译头文件 `precomp.h.gch`。剩下的生成共享对象的过程与本文前面所描述的相同。 
**注意：**`g++` 版本 3.4 及更高的版本提供了对预编译头文件的支持。 
结束语
在两个完全不同的系统之间（如 Windows 和 UNIX）进行移植，绝不是一项简单的任务，并且它需要大量的调整工作和耐心。本文说明了将最基本的项目类型从 Visual Studio 环境移植到基于
`g++`/Solaris 环境的基本要素。第二篇文章作为本系列文章的总结，将介绍 Visual Studio 环境及其 `g++` 等价物中各种可用的编译器选项、`g++` 属性机制、从 32 位（通常是指 Windows）环境移植到 64 位（UNIX）环境时的一些问题，以及多线程等等。
## 共享本文……
![](http://www.ibm.com/i/c.gif)
参考资料 
**学习**
- 您可以参阅本文在 developerWorks 全球站点上的 [英文原文](http://www.ibm.com/developerworks/aix/library/au-porting/?S_CMP=cn-a-aix&S_TACT=105AGX52) 。
- [Microsoft Developer Network](http://msdn2.microsoft.com/en-us/library/ms682599.aspx)：这个站点提供了有关动态链接库函数的文档说明。
- [Making C++ Loadable Modules Work](http://www.fpx.de/fp/Software/tcl-c++/)：Frank Pilhofer 详细地介绍了有关这个主题的内容。
- [Building And Using Static And Shared 'C' Libraries](http://users.actcom.co.il/~choo/lupg/tutorials/libraries/unix-c-libraries.html#creating_shared_library)：在 Little Unix Programmers Group (LUPG)'s Little 站点中查看这个教程。
- [KAI
`C++` 用户指南](http://www-d0.fnal.gov/KAI/doc/UserGuide/precompiled-headers.html)：参考这个指南，以获得有关预编译头文件的详细的描述。
- [GCC 联机文档](http://gcc.gnu.org/onlinedocs/)：这个站点为 GCC/G++ 的最新发行版提供了参考手册。
- [AIX and UNIX 专区](http://www.ibm.com/developerworks/cn/aix)：developerWorks 的“AIX and UNIX 专区”提供了大量与 AIX 系统管理的所有方面相关的信息，您可以利用它们来扩展自己的 UNIX 技能。
- [AIX and UNIX 新手入门](http://www.ibm.com/developerworks/cn/aix/newto)：访问“AIX and UNIX 新手入门”页面可了解更多关于 AIX 和 UNIX 的内容。
- [AIX Wiki](http://www.ibm.com/developerworks/apps/SendTo?aix=aix5lwiki)：AIX 相关技术信息的协作环境。
引用:[http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html](http://www.ibm.com/developerworks/cn/aix/library/au-porting/index.html)
