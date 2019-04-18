# VC++ MFC DLL动态链接库编写详解 - ljx0305的专栏 - CSDN博客
2010年09月03日 16:34:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：905标签：[dll																[mfc																[vc++																[扩展																[class																[delete](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[DLL																[MFC](https://blog.csdn.net/ljx0305/article/category/403913)](https://blog.csdn.net/ljx0305/article/category/403907)
VC++ MFC DLL动态链接库编写详解
虽然能用DLL实现的功能都可以用COM来替代，但DLL的优点确实不少，它更容易创建。本文将讨论如何利用VC MFC来创建不同类型的DLL，以及如何使用他们。
一、DLL的不同类型
使用VC++可以生成两种类型的DLL：MFC扩展DLL和常规DLL。常规DLL有可以分为动态连接和静态连接。Visual C++还可以生成WIN32 DLL，但不是这里讨论的主要对象。
1、MFC扩展DLL
每个DLL都有某种类型的接口：变量、指针、函数、客户程序访问的类。它们的作用是让客户程序使用DLL，MFC扩展DLL可以有C++的接口。也就是它可以导出C++类给客户端。导出的函数可以使用C++/MFC数据类型做参数或返回值，导出一个类时客户端能创建类对象或者派生这个类。同时，在DLL中也可以使用DLL和MFC。
Visual C++使用的MFC类库也是保存在一个DLL中，MFC扩展DLL动态连接到MFC代码库的DLL，客户程序也必须要动态连接到MFC代码库的DLL。（这里谈到的两个DLL，一个是我们自己编写的DLL，一个装MFC类库的DLL）现在MFC代码库的DLL也存在多个版本，客户程序和扩展DLL都必须使用相同版本的MFC代码DLL。所以为了让MFC扩展DLL能很好的工作，扩展DLL和客户程序都必须动态连接到MFC代码库DLL。而这个DLL必须在客户程序运行的计算机上。
2、常规DLL
使用MFC扩展DLL的一个问题就是DLL仅能和MFC客户程序一起工作，如果需要一个使用更广泛的DLL，最好采用常规DLL，因为它不受MFC的某些限制。常规DLL也有缺点：它不能和客户程序发送指针或MFC派生类和对象的引用。一句话就是常规DLL和客户程序的接口不能使用MFC，但在DLL和客户程序的内部还是可以使用MFC。
当在常规DLL的内部使用MFC代码库的DLL时，可以是动态连接/静态连接。如果是动态连接，也就是常规DLL需要的MFC代码没有构建到DLL中，这种情况有点和扩展DLL类似，在DLL运行的计算机上必须要MFC代码库的DLL。如果是静态连接，常规DLL里面已经包含了需要的MFC代码，这样DLL的体积将比较大，但它可以在没有MFC代码库DLL的计算机上正常运行。
二、建立DLL
利用Visual C++提供的向导功能可以很容易建立一个不完成任何实质任务的DLL，这里就不多讲了，主要的任务是如何给DLL添加功能，以及在客户程序中利用这个DLL
1、导出类
用向导建立好框架后，就可以添加需要导出类的.cpp .h文件到DLL中来，或者用向导创建C++ Herder File/C++ Source File。为了能导出这个类，在类声明的时候要加“_declspec(dllexport)”,如： 
class _declspec(dllexport) CMyClass
{
...//声明 
}
如果创建的MFC扩展DLL，可以使用宏：AFX_EXT_CLASS:
class AFX_EXT_CLASS CMyClass
{
...//声明 
}
这样导出类的方法是最简单的，也可以采用.def文件导出，这里暂不详谈。
2、导出变量、常量、对象
很多时候不需要导出一个类，可以让DLL导出一个变量、常量、对象，导出它们只需要进行简单的声明：_declspec(dllexport) int MyInt; 
_declspec(dllexport) extern const COLORREF MyColor=RGB(0,0,0); 
_declspec(dllexport) CRect rect(10,10,20,20); 
要导出一个常量时必须使用关键字extern，否则会发生连接错误。
注意：如果客户程序识别这个类而且有自己的头文件，则只能导出一个类对象。如果在DLL中创建一个类，客户程序不使用头文件就无法识别这个类。
当导出一个对象或者变量时，载入DLL的每个客户程序都有一个自己的拷贝。也就是如果两个程序使用的是同一个DLL，一个应用程序所做的修改不会影响另一个应用程序。 
我们在导出的时候只能导出DLL中的全局变量或对象，而不能导出局部的变量和对象，因为它们过了作用域也就不存在了，那样DLL就不能正常工作。如： 
MyFunction()
{
_declspec(dllexport) int MyInt; 
_declspec(dllexport) CMyClass object; 
}
3、导出函数
导出函数和导出变量/对象类似，只要把_declspec(dllexport)加到函数原型开始的位置：
_declspec(dllexport) int MyFunction(int);
如果是常规DLL，它将和C写的程序使用，声明方式如下：
extern "c" _declspec(dllexport) int MyFunction(int);
实现：
extern "c" _declspec(dllexport) int MyFunction(int x)
{
...//操作 
}
如果创建的是动态连接到MFC代码库DLL的常规DLL，则必须插入AFX_MANAGE_STATE作为导出函数的首行，因此定义如下：
extern "c" _declspec(dllexport) int MyFunction(int x)
{
AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
...//操作 
}
有时候为了安全起见，在每个常规DLL里都加上，也不会有任何问题，只是在静态连接的时候这个宏无效而已。这是导出函数的方法，记住只有MFC扩展DLL才能让参数和返回值使用MFC的数据类型。
4、导出指针
导出指针的方式如下：
_declspec(dllexport) int *pint;
_declspec(dllexport) CMyClass object = new CMyClass;
如果声明的时候同时初始化了指针，就需要找到合适的地方类释放指针。在扩展DLL中有个函数DllMain()。（注意函数名中的两个l要是小写字母），可以在这个函数中处理指针：
# include "MyClass.h"
_declspec(dllexport) CMyClass *pobject = new CMyClass;
DllMain(HINSTANCE hInstance,DWORD dwReason,LPVOID lpReserved)
{
if(dwReason == DLL_PROCESS_ATTACH) 
{ 
.....// 
} 
else if(dwReason == DLL_PROCESS_DETACH) 
{ 
delete pobject; 
} 
}
常规DLL有一个从CWinApp派生的类对象处理DLL的开和关，可以使用类向导添加InitInstance/ExitInstance函数。
int CMyDllApp::ExitInstance()
{
delete pobject; 
return CWinApp::ExitInstance(); 
}
三、在客户程序中使用DLL
编译一个DLL时将创建两个文件.dll文件和.lib文件。首先将这两个文件复制到客户程序项目的文件夹里，这里需要注意DLL和客户程序的版本问题，尽量使用相同的版本，都使用RELEASE或者都是DEBUG版本。 
接着就需要在客户程序中设置LIB文件，打开Project Settings--- >Link--->Object/library Modules中输入LIB的文件名和路径。如：Debug/SampleDll.lib。除了DLL和LIB文件外，客户程序需要针对导出类、函数、对象和变量的头文件，现在进行导入添加的关键字就是：_declspec(dllimport)，如：
_declspec(dllimport) int MyFunction(int);
_declspec(dllimport) int MyInt;
_declspec(dllimport) CMyClass object;
extern "C" _declspec(dllimport) int MyFunction(int);
在有的时候为了导入类，要把相应类的头文件添加到客户程序中，不同的是要修改类声明的标志：
class _declspec(dllimport) CMyClass，如果创建的是扩展DLL，两个位置都是:
class AFX_EXT_CLASS CMyClass。
使用DLL的一个比较严重的问题就是编译器之间的兼容性问题。不同的编译器对c++函数在二进制级别的实现方式是不同的。所以对基于C++的DLL，如果编译器不同就有很麻烦的。如果创建的是MFC扩展DLL，就不会存在问题，因为它只能被动态连接到MFC的客户应用程序。这里不是本文讨论的重点。
一、重新编译问题
我们先来看一个在实际中可能遇到的问题：
比如现在建立好了一个DLL导出了CMyClass类，客户也能正常使用这个DLL，假设CMyClass对象的大小为30字节。如果我们需要修改DLL中的CMyClass类，让它有相同的函数和成员变量，但是给增加了一个私有的成员变量int类型，现在CMyClass对象的大小就是34字节了。当直接把这个新的DLL给客户使用替换掉原来30字节大小的DLL，客户应用程序期望的是30字节大小的对象，而现在却变成了一个34字节大小的对象，糟糕，客户程序出错了。
类似的问题，如果不是导出CMyClass类，而在导出的函数中使用了CMyClass，改变对象的大小仍然会有问题的。这个时候修改这个问题的唯一办法就是替换客户程序中的CMyClass的头文件，全部重新编译整个应用程序，让客户程序使用大小为34字节的对象。
这就是一个严重的问题，有的时候如果没有客户程序的源代码，那么我们就不能使用这个新的DLL了。
二、解决方法 
为了能避免重新编译客户程序，这里介绍两个方法：（1）使用接口类。（2）使用创建和销毁类的静态函数。
1、使用接口类
接口类的也就是创建第二个类，它作为要导出类的接口，所以在导出类改变时，也不需要重新编译客户程序，因为接口类没有发生变化。
假设导出的CMyClass类有两个函数FunctionA FunctionB。现在创建一个接口类CMyInterface，下面就是在DLL中的CMyInterface类的头文件的代码：
# include "MyClass.h"
class _declspec(dllexport) CMyInterface
{
CMyClass *pmyclass;
CMyInterface();
~CMyInterface();
public:
int FunctionA(int);
int FunctionB(int);
};
而在客户程序中的头文件稍不同，不需要INCLUDE语句，因为客户程序没有它的拷贝。相反，使用一个CMyClass的向前声明，即使没有头文件也能编译：
class _declspec(dllexport) CMyInterface
{
class CMyClass;//向前声明
CMyClass *pmyclass;
CMyInterface();
~CMyInterface();
public:
int FunctionA(int);
int FunctionB(int);
};
在DLL中的CMyInterface的实现如下：
CMyInterface::CMyInterface()
{
pmyclass = new CMyClass();
}
CMyInterface::~CMyInterface()
{
delete pmyclass;
}
int CMyInterface::FunctionA()
{
return pmyclass->FunctionA();
}
int CMyInterface::FunctionB()
{
return pmyclass->FunctionB(); 
}
.....
对导出类CMyClass的每个成员函数，CMyInterface类都提供自己的对应的函数。客户程序与CMyClass没有联系，这样任意改CMyClass也不会有问题，因为CMyInterface类的大小没有发生变化。即使为了能访问CMyClass中的新增变量而给CMyInterface类加了函数也不会有问题的。
但是这种方法也存在明显的问题，对导出类的每个函数和成员变量都要对应实现，有的时候这个接口类会很庞大。同时增加了客户程序调用所需要的时间。增加了程序的开销。
2、使用静态函数
还可以使用静态函数来创建和销毁类对象。创建一个导出类的时候，增加两个静态的公有函数CreateMe()/DestroyMe()，头文件如下：
class _declspec(dllexport) CMyClass
{
CMyClass(); 
~CMyClass(); 
public: 
static CMyClass *CreateMe(); 
static void DestroyMe(CMyClass *ptr); 
};
实现函数就是：
CMyClass * CMyClass::CMyClass()
{
return new CMyClass; 
}
void CMyClass::DestroyMe(CMyClass *ptr)
{
delete ptr; 
}
然后象其他类一样导出CMyClass类，这个时候在客户程序中使用这个类的方法稍有不同了。如若想创建一个CMyClass对象，就应该是：
CMyClass x;
CMyClass *ptr = CMyClass::CreateMe();
在使用完后删除：
CMyClass::DestroyMe(ptr);
