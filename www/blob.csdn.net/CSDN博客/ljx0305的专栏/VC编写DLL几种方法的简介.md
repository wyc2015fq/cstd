# VC编写DLL几种方法的简介 - ljx0305的专栏 - CSDN博客
2009年06月09日 17:02:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：2754标签：[dll																[library																[class																[delphi																[mfc																[vb](https://so.csdn.net/so/search/s.do?q=vb&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=delphi&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[DLL																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/403907)
动态连接库最大的特点就是能节省磁盘空间.当多个进程共享同一个DLL的时候,内存中只有一个DLL的代码.通过映射来使各个进程得以调用.
1.用VC建立一个WIN32 DLL
我们利用VC编写DLL有几种方法.如果用VC建立一个WIN32 DLL 工程.那这个工程就应该只导出C++的类或全局变量.和全局函数.方法就是在CPP文件中编写你的代码,为每个需要导出的元素进行导出办法是增加如下语句:
_declspec(dllexport) 
你当然可以把它定义成宏
例如,如果是一个类STUDENT需要导出, 那么声明时应该是这样写 class _declspec{dllexport) student;
当然也可以定义时直接导出.
我们的客户端,也就是我们调用该函数的客户程序,就需要导入这个类或者函数..
填写如下语句:
class _declspec(dllimport) student
{
}  // 声明
之后就可以利用STUDENT来构造对象,也可以调用它的成员函数..了
记住,一定要把工程的连接设置好.要把生成的LIB文件填写好,因为客户程序要想加载DLL,能够准确的调用各个DLL中的函数,都是靠这个LIB文件哪.包括函数的地址等等.
当然也可以显示连接 
利用LOADLIBRARY
原型是 
HMODULE LoadLibrary( LPCTSTR );
返回的HMODULE就是一个DLL句柄.
所以我们在利用这个句柄来作为参数调用另一个函数GETPROCADDRESS
FARPROC GetProcAddress( HMODULE  , LPCSTR);  //如果利用序号来索引,那么要加上MAKEINTERSOURCE宏
返回一个函数指针,利用它来调用函数,
LPCSTR是函数名,但你应该利用DUMPBIN来查看一下你导出的函数名,因为C++编译器支持重载,它会以自己的方式重命名.除非你用extern "C"
用C语言的方式来命名函数.例如 一个函数 void fun();
导出格式应该是 extern "C" _declspec(dllexport) void fun();   //如果是声明导入函数,直接写原型,如果是声明类,那么一定要是类的头文件声明,包含了成员函数和数据成员的.
注意即使是采用了C语言命名方式 如果你改变了调用方式_stdcall 那么还是会改变函数命名的,除非你利用DEF文件来导出.
EXPORTS
fun
这样是可以的.
2.建立一个MFC扩展DLL
扩展DLL是为了更好的支持MFC的类.你建立这个工程后会自动生成一些代码,不要管它先,你把你要动态连接的CPP和相应的.H文件加入到工程,在.CPP文件中需要导出的类上加上AFX_EXT_Class 在.H需要导入的类上加上同样的代码,这样就可以了.
例如class AFX_EXT_CLASS CSTUDENT : public CPERSON   //.CPP
{
}
class AFX_EXT_CLASS CSTUDENT ; //.H
{
}   //声明
3.建立一个常规的DLL
如果你要建立扩展的DLL,那么其他的IDE是无法利用的,因为每个编译器的命名方式是不同的.
如果你想使其他IDE来调用VC的DLL,那么就建立一个常规的DLL.
建立工程以后,编写你要导出的类.例如
extern "C" _declspec(dllexport) void fun()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( )); 
}
在为每一个需要导出的函数的开头加上这条语句.
在客户端要加上导入语句就可以了.
具体可参考下面的例子   我已经在delphi下调试通过 
和编写一般的DLL方法相同，需要注意以下两点：
（1）调用约定
C函数有_stdcall、_cdecl、_fastcall等多种调用约定，调用约定用来说明函数参数的压栈顺序和由谁（函数自身还是调用者）来修改堆栈。关于调用约定的详细说明，请参考我转载的另一篇文章。
编写供PB调用DLL，请使用_stdcall调用约定，如下所示：
extern "C" _declspec(dllexport) int _stdcall GetInt(char* name)
{
...
}
（2）def文件
在VC++中，如果生成DLL可以不使用.def文件，只需要在VC++的函数定义前加__declspec(dllexport)修饰就可以了。生成的DLL VC++用户可以直接使用，但PB、VB等用户使用会遇到函数名转换的问题。因为VC++对于__declspec(dllexport)声明的函数会进行名称转换，如下面的函数：
__declspec(dllexport) int _stdcall GetStr()
编译后会转换为 [GetStr@0](mailto:GetStr@0)，这样在PB、VB中声明函数时应该声明[GetStr@0](mailto:GetStr@0)，
如果函数带有参数，转换后的函数名将更加复杂，这使PB、VB用户使用起来很不方便。在def文件中由EXPORT输出函数可解决这个问题。
如dll要输出如下两个函数：
extern "C" _declspec(dllexport) int _stdcall GetInt(char* name)；
extern "C" _declspec(dllexport) char* _stdcall GetStr(int id)；
则def文件书写如下（TEST为工程名）：
LIBRARY       "TEST"
DESCRIPTION   'TEST Windows Dynamic Link Library'
EXPORTS
     ; Explicit exports can go here
GetInt @1
GetStr @2   
编译生成DLL后，在PB中要调用GetStr函数，只需做如下声明即可：
function string GetStr(int a) library("TEST.dll"); 
注意，如果您建的是Win32 Dynamic-Link Library 工程，def文件需要自己创建，然后把它加入工程，def文件名需和工程名相同。如您的工程名为test，则创建test.def。
转自:http://blog.csdn.net/jiangxinyu/archive/2008/07/31/2743282.aspx
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/jiangxinyu/archive/2008/07/31/2743282.aspx](http://blog.csdn.net/jiangxinyu/archive/2008/07/31/2743282.aspx)
