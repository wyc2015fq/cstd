# 孙鑫-MFC笔记十一--动态链接库 - 工作笔记 - CSDN博客





2012年05月15日 09:02:34[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6000标签：[dll																[mfc																[reference																[api																[microsoft																[output](https://so.csdn.net/so/search/s.do?q=output&t=blog)
个人分类：[基础编程](https://blog.csdn.net/App_12062011/article/category/952065)





Windows三个重要的动态链接库

Kernel32.dll,它包含用于管理内存、进程和线程的各个函数；

User32.dll，它包含用于执行用户界面任务（如窗口的创建和消息的传送）的各个函数；

GDI32.dll，它包含用于画图和显示文本的各个函数。


静态库和动态库区别：
静态库：函数和数据被编译进一个二进制文件（通常扩展名为.LIB）。在使用静态库的情况下，在编译链接可执行文件时，链接器从库中复制这些函数和数据并把它们和应用程序的其他模块组合起来创建最终的可执行文件（.EXE文件）；
在使用动态库的时候，往往提供两个文件：一个引入库和一个DLL。引入库包含被DLL导出的函数和变量的符号名，DLL包含实际的函数和数据。在编译链接可执行文件时，只需要链接引入库，DLL中的函数代码和数据并不复制到可执行文件中，在运行时候，再去加载DLL，访问DLL中导出的函数。这个引入库也是lib格式，但其与静态库不同。


使用动态链接库的好处
可以采用多种编程语言来编写。

增强产品的功能。

提供二次开发的平台。

简化项目管理

可以节省磁盘空间和内存。

有助于资源的共享。

有助于实现应用程序的本地化

动态链接库加载的两种方式：

隐式连接

显示加载



### 隐式连接：

建立DLL文件代码如下:

_declspec(dllexport) int Add(int x,int y)

{

 return x+y;

}

_declspec(dllexport) int Subtract(int x,int y)

{

 return x-y;

}

//必须带_declspec(dllexport)文件,以生成*.lib文件

必须被到处的函数，可以用dumpbin工具查看那些函数是导出的。要到处的函数，前面加上标识符：_declspec(dllexport)



如果要查找*dll中包含信息，可在命令行下进入Debug所在目录，输入以下命令

dumpbin -exports dll.dll  

有些时候由于某种安装原因，dumpbin被认为是无效命令，接下来在

C:\Program Files\Microsoft Visual Studio\VC98\Bin\下找到VCVARS32.bat并在命令行运行，之后就能执行dumpbin命令了。

C++名字改编 或者叫名字粉碎



隐式连接方式：

下面是测试文件：

新建MFC程序，新建两个按钮，代码如下：

void CDllTestDlg::OnBtnAdd()

{

 // TODO: Add your control notification handler code here

 CString str;

 str.Format("3+5=&d",Add(3,5));

 MessageBox(str);

}

void CDllTestDlg::OnBtnSubtract()

{

 // TODO: Add your control notification handler code here

 CString str;

 str.Format("5-3=%d",Subtract(5,3));

 MessageBox(str);

}

为使编译器认识Add，Subtract，必须在之前使用两个声明：

extern int Add(int x,int y);

extern int Subtract(int x,int y);
可以使用标示符表示这两个函数是从动态链接库的.lib文件引用的，以生成效率更高的代码
_declspec(dllimport) int Add(int x,int y);

_declspec(dllimport) int Subtract(int x,int y);

这两段代码我们也可以在DLL中新建一个头文件放进去，并在MFC程序中添加头文件

如#include "..\Dll\Dll.h"

从原先Dll文件下Debug目录中复制*.lib到MFC程序文件夹下，并添加库函数

在project--->setting--->link--->Object/Library Modules写下所复制的文件名

如果要查看DllTest.exe文件信息，使用命令行dumpbin -imports dlltest.exe



修改动态链接库Dll.h

#ifdef DLL_API

#else

#define DLL_API _declspec(dllimport)

#endif

DLL_API int Add(int x,int y);

DLL_API int Subtract(int x,int y);

修改Dll.cpp文件

#define DLL_API _declspec(dllexport)

#include "Dll.h"

int Add(int x,int y)

{

 return x+y;

}

int Subtract(int x,int y)

{

 return x-y;

}

这样做是为了方便外部程序调用同时方便内部程序使用，因为动态链接库中只有导出的函数才可以被使用，没有导出的函数在外部是看不到的，是不能被访问的





接下来导出整个类，代码：

class DLL_API point

{

public:

 void output(int x,int y);  //如果只想导出一个函数，可把上边的DLL_API剪切        //然后放到void output(int x,int y);前边，虽然累没有被        //导出，但访问仍没有区别

};仍然受制于访问权限

实现：

void Point::output(int x,int y)

{

 HWND hwnd=GetForegroundWindow();

 HDC hdc=GetDC(hwnd);

 char buf[20];

 memset(buf,0,20);

 sprintf(buf,"x=%d,y=%d",x,y);

 TextOut(hdc,x,y,buf,strlen(buf));

 ReleaseDC(hwnd,hdc);

}

接下来在MFC程序新建一个按钮，调用动态链接库函数，代码如下：

 Point pt;

 pt.output(100,200); 

因为C++导出或导入动态链接库会发生名字的改编，如果不想发生名字改编，我们可以使用如下代码：

#define DLL_API  extern "c" _declspec(dllexport)

这样编译器就不会进行名字改编，一个用C语言编写的客户端程序就可以调用这个用C++编写的动态链接库。其缺点是，不能导入类中的成员函数

如果导出函数的调用约定，发生变化，即时使用了限定符，函数的名字也会发生变化。

如果函数使用标准调用约定_stdcall,即使使用了extern "c"，此函数仍会发生改编

在这种情况下，通过DEF,模块定义文件的方式来解决问题。



接下来新建一个动态链接库文件，文件名为Dll2，cpp文件代码为：

int Add(int x,int y)

{

 return x+y;

}

int Subtract(int x,int y)

{

 return x-y;

}

为了最终解决问题，我们可以新建一个模块文件Dll.def，以使得其他语言编制的程序也能使用我们的动态链接库。

添加代码

LIBRARY Dll2 //内部名称，与生成的动态链接库名称要匹配。非必须代码

EXPORTS   //即使调用_stdcall约定，也不会发生改编，而只会调用这里显示的Add     //字符串

Subtract

EXPORTS 语句引入了一个由一个或多个 definitions（导出的函数或数据）组成的节。每个定义必须在单独一行上。EXPORTS 关键字可以在第一个定义所在的同一行上或在前一行上。.def 文件可以包含一个或多个 EXPORTS 语句。

导出 definitions 的语法为：

entryname[=internalname] [@ordinal [NONAME]] [PRIVATE] [DATA]

entryname 是要导出的函数名或变量名。这是必选项。如果导出的名称与 DLL 中的名称不同，则通过 internalname 指定 DLL 中导出的名称。例如，如果 DLL 导出函数 func1()，要将它用作 func2()，则应指定：

EXPORTS

func2=func1

@ordinal 允许指定是序号而不是函数名将进入 DLL 的导出表。这有助于最小化 DLL 的大小。.LIB 文件将包含序号与函数之间的映射，这使您得以像通常在使用 DLL 的项目中那样使用函数名。

可选的 NONAME 关键字允许只按序号导出，并减小结果 DLL 中导出表的大小。但是，如果要在 DLL 上使用 GetProcAddress，则必须知道序号，因为名称将无效。

可选的 PRIVATE 关键字禁止将 entryname 放到由 LINK 生成的导入库中。它对同样是由 LINK 生成的图像中的导出无效。

可选的 DATA 关键字指定导出的是数据，而不是代码。例如，可以导出数据变量，如下所示：

EXPORTS

i DATA



#### 显示加载：

接下来在MFC文件中改写按钮void CDllTestDlg::OnBtnAdd() 代码：

 HINSTANCE hInst;

 hInst=LoadLibrary("Dll2.dll");  //使用动态加载

/*

HMODULE LoadLibrary(

  LPCTSTR lpFileName //*.exe *.dl文件名

);

LoadLibrary不仅可以加载动态连接库，还可以加载exe文件。

 typedef int (*ADDPROC)(int a,int b);

//如果在DLL的函数中调用_stdcall，相应的应该把代码改为

//typedef int (_stdcall *ADDPROC)(int a,int b);  //注意导出函数的调用约定

 ADDPROC Add=(ADDPROC)GetProcAddress(hInst,"Add");//构造一个函数指针


/*

得到导出函数的地址

FARPROC GetProcAddress(

  HMODULE hModule,

  LPCSTR lpProcName

);



 if(!Add)

 {

  MessageBox("获取函数地址失败！");

  return ;

 }

 CString str;

 str.Format("3+5=%d",Add(3,5));

 MessageBox(str);

因为调用LoadLibrary时动态加载动态链接库，所以不需要头文件和.lib文件



如果我们在动态链接库中使用标准调用约定_stdcall，而在可执行程序中使用动态加载DLL，会发生名字重编，如果知道DLL中函数的序号，这时可以使用宏MAKEINTRESOURCE把序号转变成名字，如：

ADDPROC Add=(ADDPROC)GetProcAddress(hInst,MAKEINTRESOURCE（1）)；
/*

The MAKEINTRESOURCE macro converts an integer value to a resource type compatible with the resource-management functions. This macro is used in place of a string containing the name of the resource.

Syntax

LPTSTR MAKEINTRESOURCE(

    WORD wInteger

);

*/

DllMain 是个可选的入口函数。

BOOL WINAPI DllMain(

  HINSTANCE hinstDLL, //动态链接库模块的句柄。首次被调用时，它的句柄通过这个参数传递进来。

  DWORD fdwReason, //标记值。用来指示调用DLL入口函数的原因。

  LPVOID lpvReserved // 检测指针，DLL被动态加载，NULL。 静态加载 非NULL

);

DllMain函数，不要做太复杂的调用，因为，在加载动态链接库的同时，可能还需要一些核心的动态链接库没有被加载。而程序会依次加载调用的Dll。我们自己的Dll可能会被考前加载，调用重要函数失败，导致程序终止。





当我们的动态链接库不再使用时可以调用FreeLibrary使动态链接库使用计数减1，当使用计数为零时，系统会收回模块资源

FreeLibrary

The FreeLibrary function decrements the reference count of the loaded dynamic-link library (DLL). When the reference count reaches zero, the module is unmapped from the address space of the calling process and the handle is no longer valid.

BOOL FreeLibrary(

  HMODULE hModule

);](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)




