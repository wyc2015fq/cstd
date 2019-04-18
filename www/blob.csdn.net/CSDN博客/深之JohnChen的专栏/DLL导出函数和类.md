# DLL导出函数和类 - 深之JohnChen的专栏 - CSDN博客

2010年06月10日 15:25:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：12841


Dll导出函数和类的方法有三种：

1.源代码中的 __declspec(dllexport)

2..def 文件中的 EXPORTS 语句

3.LINK 命令中的 /EXPORT 规范

所有这三种方法可以用在同一个程序中。LINK 在生成包含导出的程序时还创建导入库，除非生成中使用了 .exp 文件。

三者的不同之处：__declspec(dllexport)导出变量方法类很简单，也可以指定C类型约定导出（加extern “C”），但是如果你的DLL是供VB、 PB、Delphi用户使用的，那么会产生一个小麻烦。因为VC++对于__declspec(dllexport)声明的函数会进行名称转换，如下面的函数：

__declspec(dllexport) int __stdcall IsWinNT() 

会转换为IsWinNT@0，这样你在VB中必须这样声明：

Declare Function IsWinNT Lib "my.dll" Alias "IsWinNT@0" () As Long 

@的后面的数由于参数类型不同而可能不同。这显然不太方便。所以如果要想避免这种转换，就要使用.def文件方式。

.def文件是模块定义文件，使用.def文件可以DLL 导出函数，也可以通过在函数名的后面加上 @ 符和一个数字，给函数分配序号值。当指定序号值时，序号值的范围必须是从 1 到 N，其中 N 是 DLL 导出函数的个数。

LINK 命令中的 /EXPORT 规范是通过预处理指示符 “#pragma” 来指定链接选项来快捷导出DLL函数，比如：

#pragma comment(linker, "/EXPORT:MyExportFunction=_MyExportFunction@4,PRIVATE")

一、使用 __declspec(dllexport) 从 DLL 导出

Microsoft 在 Visual C++ 的 16 位编译器版本中引入了 __export，使编译器得以自动生成导出名并将它们放到一个 .lib 文件中。然后，此 .lib 文件就可以像静态 .lib 那样用于与 DLL 链接。

在更新的32位编译器版本中，可以使用 __declspec(dllexport) 关键字从 DLL 导出数据、函数、类或类成员函数。__declspec(dllexport) 会将导出指令添加到对象文件中，因此您不需要使用 .def 文件。

当试图导出 C++ 修饰函数名时，这种便利最明显。由于对名称修饰没有标准规范，因此导出函数的名称在不同的编译器版本中可能有所变化。如果使用 __declspec(dllexport)，仅当解决任何命名约定更改时才必须重新编译 DLL 和依赖 .exe 文件。

许多导出指令（如序号、NONAME 和 PRIVATE）只能在 .def 文件中创建，并且必须使用 .def 文件来指定这些属性。不过，在 .def 文件的基础上另外使用 __declspec(dllexport) 不会导致生成错误。

若要导出函数，__declspec(dllexport) 关键字必须出现在调用约定关键字的左边（如果指定了关键字）。例如：

__declspec(dllexport) void __cdecl Function1(void);若要导出类中的所有公共数据成员和成员函数，关键字必须出现在类名的左边，如下所示：

class __declspec(dllexport) CExampleExport : public CObject{ ... class definition ... };生成 DLL 时，通常创建一个包含正在导出的函数原型和/或类的头文件，并将 __declspec(dllexport) 添加到头文件中的声明中。若要提高代码的可读性，请为 __declspec(dllexport) 定义一个宏并对正在导出的每个符号使用该宏：

#define DllExport__declspec( dllexport ) 

二、使用 DEF 文件从 DLL 导出

模块定义 (.def) 文件是包含一个或多个描述 DLL 各种属性的 Module 语句的文本文件。如果不使用 **__declspec(dllexport)**关键字导出 DLL 的函数，则 DLL 需要 .def 文件。

.def 文件必须至少包含下列模块定义语句：

·文件中的第一个语句必须是 LIBRARY 语句。此语句将 .def 文件标识为属于 DLL。LIBRARY 语句的后面是 DLL 的名称。链接器将此名称放到 DLL 的导入库中。

·EXPORTS 语句列出名称，可能的话还会列出 DLL 导出函数的序号值。通过在函数名的后面加上 @ 符和一个数字，给函数分配序号值。当指定序号值时，序号值的范围必须是从 1 到 N，其中 N 是 DLL 导出函数的个数。

例如，包含实现二进制搜索树的代码的 DLL 看上去可能像下面这样：

LIBRARYBTREE

EXPORTS

Insert@1

Delete@2

Member@3

Min@4

如果使用 MFC DLL 向导创建 MFC DLL，则向导将为您创建主干 .def 文件并将其自动添加到项目中。添加要导出到此文件的函数名。对于非 MFC DLL，必须亲自创建 .def 文件并将其添加到项目中。

如果导出 C++ 文件中的函数，必须将修饰名放到 .def 文件中，或者通过使用外部“C”定义具有标准 C 链接的导出函数。如果需要将修饰名放到 .def 文件中，则可以通过使用 DUMPBIN 工具或 /MAP 链接器选项来获取修饰名。请注意，编译器产生的修饰名是编译器特定的。如果将 Visual C++ 编译器产生的修饰名放到 .def 文件中，则链接到 DLL 的应用程序必须也是用相同版本的 Visual C++ 生成的，这样调用应用程序中的修饰名才能与 DLL 的 .def 文件中的导出名相匹配。

如果生成扩展 DLL 并使用 .def 文件导出，则将下列代码放在包含导出类的头文件的开头和结尾：

#undef AFX_DATA

#define AFX_DATA AFX_EXT_DATA

// <body of your header file>

#undef AFX_DATA

#define AFX_DATA

这些代码行确保内部使用的 MFC 变量或添加到类的变量是从扩展 DLL 导出（或导入）的。例如，当使用 **DECLARE_DYNAMIC**派生类时，该宏扩展以将 **CRuntimeClass**成员变量添加到类。省去这四行代码可能会导致不能正确编译或链接 DLL，或在客户端应用程序链接到 DLL 时导致错误。

当生成 DLL 时，链接器使用 .def 文件创建导出 (.exp) 文件和导入库 (.lib) 文件。然后，链接器使用导出文件生成 DLL 文件。隐式链接到 DLL 的可执行文件在生成时链接到导入库。请注意，MFC 本身使用 .def 文件从 MFCx0.dll 导出函数和类。

注意细节问题：

1、使用MFC DLL 向导创建 MFC DLL，则向导将为您创建主干 .def 文件并将其自动添加到项目中。添加要导出到此文件的函数名。对于非 MFC DLL，必须亲自创建 .def 文件并将其添加到项目中。

2、通过.def文件导出函数和类时，最好通过添加一个新的项目名.def问题，将需要导出的函数和类写入其中。

3、DLL中.def文件不起作用

解决方法：

1）打开工程属性页。比如

![](http://hi.csdn.net/attachment/201006/10/0_1276154490z1iq.gif)

2）选择连接器目录

3）单击输入属性

4）修改模版定义文件属性。加入项目名.def文件。
![](http://hi.csdn.net/attachment/201006/10/0_1276154496Y0sc.gif)

4、将 DLL 源代码从 Win16 移植到 Win32 时，请用__declspec(dllexport) 替换__export 的每个实例。

5、DLL隐式链接与显示链接

DLL隐式链接：通过链接Lib库，然后加入需要导入的变量、函数、类并且在之前加_declspec(dllimport) ，自然直接使用DLL中的函数和类。

DLL显示链接：通过LoadLibrary加载dll，然后通过GetProcAddress得到函数，就可以使用DLL中的函数。（不需要lib文件）

举例：
 Dll程序：
__declspec(dllexport) int g_xua = 10;
__declspec(dllexport) void __cdecl myPrint()
   {
         printf("I am in dll/n");
   }
隐式链接调用：
链接Lib库
_declspec(dllimport) int g_xua;
_declspec(dllimport) void myPrint();
显示链接调用：
typedef int (__cdecl *MYPROC)();
        hinstLib = LoadLibrary(TEXT("UsedDll"));
        if(hinstLib == NULL)
          {

AfxMessageBox("GetModuleHandle fail");

}
        PVOID procAddress1 = GetProcAddress(hinstLib,(LPCSTR)MAKEINTRESOURCE(1));
        if(procAddress1 == NULL)

{

AfxMessageBox("GetProcAddress fail");

}
int* show_g_xua = (int*)procAddress1;
        printf("g_xua=%d/n",*show_g_xua);

MYPROC procAddress2 = (MYPROC)GetProcAddress(hinstLib,(LPCSTR)MAKEINTRESOURCE(2));
        if(procAddress2 == NULL)

{

AfxMessageBox("GetProcAddress fail");

}
        procAddress2();

注：GetProcAddress函数可以通过序号值来指定函数，也可以通过函数名称来指定函数（这种情况在生成DLL时，需要.def模版定义文件，把需要导出的函数名称写入.def文件中）。两种情况的比较如下图中所示：

![](http://hi.csdn.net/attachment/201006/10/0_1276154503p6GE.gif)

