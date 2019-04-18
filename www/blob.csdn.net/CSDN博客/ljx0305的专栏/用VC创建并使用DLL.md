# 用VC创建并使用DLL  - ljx0305的专栏 - CSDN博客
2009年09月02日 22:00:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：586标签：[dll																[api																[preprocessor																[exe																[build																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=exe&t=blog)](https://so.csdn.net/so/search/s.do?q=preprocessor&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[C++																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/380566)
**为何DLL**
先看看静态库与DLL的不同之处
可执行文件的生成（Link期）：前者很慢（因为要将库中的所有符号定义Link到EXE文件中），而后者很快（因为后者被Link的引入库文件无符号定义）
可执行文件的大小：前者很大，后者很小（加上DLL的大小就和前者差不多了）
可执行文件的运行速度：前者快（直接在EXE模块的内存中查找符号），后者慢（需要在DLL模块的内存中查找，在另一个模块的内存中查找自然较慢）
可共享性：前者不可共享，也就是说如果两个EXE使用了同一个静态库，那么实际在内存中存在此库的两份拷贝，而后者是可共享的。
可升级性：前者不可升级（因为静态库符号已经编入EXE中，要升级则EXE也需要重新编译），后者可以升级（只要接口不变，DLL即可被升级为不同的实现）
综合以上，选择静态库还是DLL
1. 静态库适于稳定的代码，而动态库则适于经常更改代码（当然接口要保持不变），当DLL更改（仅实现部分）后，用户不需要重编工程，只需要使用新的Dll即可。
2. 由于静态库很吃可执行文件的生成（Link期）时间，所以如果对可执行文件的Link时间比较敏感，那么就用DLL。
使用DLL
在介绍如何创建DLL之前，让我们先了解它是如何被使用的。
1. 显式调用（也叫动态调用）
显示调用使用API函数LoadLibrary或者MFC提供的AfxLoadLibrary将DLL加载到内存，再用GetProcAddress()在内存中获取引入函数地址，然后你就可以象使用本地函数一样来调用此引入函数了。在应用程序退出之前，应该用FreeLibrary或MFC提供的AfxLoadLibrary释放DLL。
下面是个显示调用的例子，假定你已经有一个Test.dll，并且DLL中有个函数名为Test，其声明式是void();
[](http://blog.csdn.net/hjsunj/archive/2008/01/16/2047376.aspx)
#include < iostream >
using namespace std;
typedef void(*TEST )();
int main( char argc, char* argv[] ) {         
    const char* dllName = "Test.dll";
    const char* funcName = "Test";
    HMODULE hDLL = LoadLibrary( dllName );
    if ( hDLL != NULL ) {
        TEST func = TEST( GetProcAddress( hDLL, funcName ) );
        if ( func != NULL ) {
            func();
        }
        else {
            cout << "Unable to find function /'" << funcName << "/' !" << endl;
        }
        FreeLibrary( hDLL );
    } 
    else {
        cout << "Unable to load DLL /'" << dllName << "/' !" << endl;
    }    
    return 0;
}
注意
1. 显示调用使用GetProcAddress，所以只能加载函数，无法加载变量和类。
2. 此外GetProcAddress是直接在.dll文件中寻找同名函数，如果DLL中的Test函数是个C++函数，那么由于在.dll文件中的实际文件名会被修饰（具体被修饰的规则可参考[函数调用约定详解](http://www.cppblog.com/bidepan2023/archive/2007/03/19/20129.html)或者使用VC自带的Depends.exe查看），所以直接找Test是找不到的，这时必须把函数名修改为正确的被修饰后的名称，下面是一种可能（此函数在DLL中的调用约定为__cdecl）：
const char* funcName = "[?Test@@YAXXZ](mailto:?Test@@YAXXZ)";
2. 隐式调用（也叫静态调用）
隐式调用必须提供DLL的头文件和引入库（可以看作轻量级的静态库（没有符号定义，但是说明了符号处于哪个DLL中））。
有了头文件和引入库，DLL的使用就跟普通静态库的使用没啥区别，只除了DLL要和EXE一起发布。
显示调用与隐式调用的优缺点
显示调用使用复杂，但能更加有效地使用内存，因为DLL是在EXE运行时（run time）加载，必须由用户使用LoadLibrary和FreeLibrary来控制DLL从内存加载或卸载的时机。此外还可以加载其他语言编写的DLL函数。
静态调用使用简单，但不能控制DLL加载时机，EXE加载到内存同时自动加载DLL到内存，EXE退出时DLL也被卸载。
创建DLL
下面我们着重讲解如何在VC下创建DLL
首先要建立一个Win32的DLL工程。再把普通静态库的所有文件转移到DLL工程中，然后：
为所有的类声明加上__declspec(dllexport)关键字，这有这样编译器才能自动为你产生引入库文件（否则你需要自己写.def文件来产生，因为不常用所以在此不再阐述其细节）
对于DLL的用户来讲，类声明就需要用另外一个关键字__declspec(dllimport)（此关键字对于类和函数而言并非必须，但对于变量而言则是必须的）。所以通常我们会定义一个宏来包装之，比如
#ifdef MYDLL_EXPORTS
#    define MYDLL_API __declspec(dllexport)
#else
#    define MYDLL_API __declspec(dllimport)
#endif
这样我们就能写出如下的类
class MYDLL_API MyClass {
 ...
};
当然在创建DLL的工程里需要包含preprocessor(预处理指示器)MYDLL_EXPORTS，而在用户工程里面则不应该包含MYDLL_EXPORTS。
其实上面说的VC早就帮我们做好了。如果你创建的DLL工程叫做Test，那么此工程自动包含TEST_EXPORTS。如果你在创建工程的时候选择了Exprot Symbols, 那么VC还会自动帮你创建一个示例文件Test.h，此文件定义出
#ifdef TEST_EXPORTS
#    define TEST_API __declspec(dllexport)
#else
#    define TEST_API __declspec(dllimport)
#endif
你自定义的文件都应该包含此文件以使用TEST_API。（如果没有选择Exprot Symbols，那么就得自己动手写出这段宏了）
示例文件中还包括了一个类，变量，以及全局函数的写法
class TEST_API CTest {
public:
    CTest(void);
    // TODO: add your methods here.
};
extern TEST_API int nTest;
TEST_API int fnTest(void);
通过上面的示例我们也可以看出全局(或者名字空间)变量和函数的声明方法
细节讨论
1.  DLL的入口函数DllMain并非必须，如果没有它，编译器会帮你自动生成一个不做任何事的DllMain。
2.  如果是可以定义在头文件里面的东西：包括宏，常量，内联函数（包括成员内联函数）以及模板。那么在DLL中的定义中可以不必包含TEST_API，和普通的定义没有区别。
如果一个类完全由内联函数和纯虚函数构成，那么也不需要TEST_API这样的关键字。一个不带成员函数的struct也一样。
3. 所有未经__declspec(dllexport)导出的类都只能作为DLL内部类使用。当然外部仍然可以使用其内联成员函数（前提是该成员函数不应该调用任何未经导出的函数或者类成员函数）
发布DLL
1. 程序库的作者应该将三件套：头文件，引入库文件和DLL一同发布给用户，其中头文件和引入库是专为静态调用的用户准备，也就是C/C++用户。（此外有些DLL内部使用的头文件，如果没有被接口头文件直接#include，那么该头文件就不必发布，这和静态库是一样的）。
2. DLL的用户只需将DLL和可执行程序一同发布即可。
C++程序使用C语言DLL（静态库规则一样）
C不存在class, 所以由C创建的DLL必然也不会出现class；对于全局变量的使用C和C++没有什么不同，所以我们把焦点集中在全局函数上（此外全局变量的规则一样）。 
我们知道C++程序要使用C语言的函数就必须在函数声明前加上extern "C"关键字，这对于静态库和DLL没有什么不同。 但是这个关键字不能直接出现在头文件函数声明中，否则DLL无法通过编译，  原因很简单，C语言并没有extern "C"这个关键字。
1. 一种作法是把C向C++迁移的责任交给DLL创建者。定义出一个宏，以使DLL（C工程）中不出现extern "C"或者只是extern，而在用户工程（C++工程）中保持原样。幸运的是windows早已为我们设计好一切，这个宏就是EXTERN_C（存在于 winnt.h中） :
#ifdef __cplusplus
    #define EXTERN_C    extern "C"
#else
    #define EXTERN_C    extern
#endif 
注意上面必须是extern而不是空。因为虽然C的函数声明不是必须添加extern，但是变量声明则必须添加extern。
有了EXTERN_C，在头文件中这样定义函数：
EXTERN_C TEST_API int fnTest(void);
2. 另外一种做法是把把C向C++迁移的责任交给用户，用户在包含DLL头文件的时候以extern "C"来include:
extern "C" {
    #include "mydll.h"
    #include "mydllcore.h"
    ...
}
可以把上述的extern "C"段放在新的头文件中和DLL一起发布，这样C++用户只需要包含这个头文件就可以了。比如Lua库就自带了一个etc/lua.hpp文件。通常此文件会由DLL作者提供，所以此时迁移的责任仍在DLL创建者。
注意用户不要试图以extern "C"来重新声明函数，因为重复声明是允许的，但是必须保证和头文件中的声明相同。
3. 这种做法的一个变形就是直接在C头文件中以extern "C"将所有函数和变量声明包含之，这样就无需像上面那样多提供一个额外的头文件了。通常像这样（mydll头文件）：
#include 头文件段
#ifdef __cplusplus
extern "C" {
#endif
函数和变量声明
...
#ifdef __cplusplus
}
#endif
创建标准的DLL，使其可被其他语言使用
通常我们会希望DLL能够被其他语言使用，因而我们的DLL往往不会提供类定义，而只提供函数定义。（因为许多编程语言是不支持类的）。
此时函数的调用约定必须是__stdcall（在vc下默认是__cdecl，所以你不得不手工添置），因为大部分语言不支持__cdecl，但支持__stdcall（比如VBScript，Delphi等）。
此外我们希望导出到DLL中的函数名能够更易被识别（用户使用才会更方便），也就是说DLL应该编译出无修饰的C函数名。
所以我们可能会
1. 如果你只用C语言，那么必然以C文件创建DLL（自动编出C符号名），考虑到潜在的C++用户（此类用户多以静态调用方式使用DLL，因而需要看到其函数声明），我们还需要使用EXTERN_C关键字（详见上面的讨论）。
2. 如果你使用C++，那么必然以C++文件创建DLL，这时你应该直接以extern "C"修饰。
结论
所以要创建能够为任意编程语言使用之DLL，我们应该
1. 只创建函数
2. 声明__stdcall调用约定（或者WINAPI，CALLBACK，前提是你必须包含windows头文件）
3. 以CPP文件 + extern "C" 或者 C文件 + EXTERN_C
4. 当然还需要DLL_API的声明（如果光有dllexport,那么DLL也只能被显示调用）。
更完美一点
不应该使用dllexport和extern "C"而应该使用.def文件。虽然经过上面的4个步骤，我们的DLL里面的C函数名已经相当简洁了，但仍不是最完美的：比如一个声明为function的函数，实际在DLL中的名字确可能是function@#。而使用.def文件，我们可以让DLL中的函数名和声明的函数名保持一致。
关于.def的详细用法可参考：
[微软DLL专题](http://www.microsoft.com/china/community/program/originalarticles/techdoc/dll.mspx)
[使用 DEF 文件从 DLL 导出](http://msdn2.microsoft.com/zh-cn/library/d91k01sh%28VS.80%29.aspx)
在DLL与静态库之间切换
前面我曾经提到对于使用DLL的用户__declspec(dllimport)关键字可有可无，当然前提是DLL中不包括变量定义。
所以要把库既能够做成DLL，也能够做成静态库，那么就应该作出类似下面这样的定义：
1. DLL不包括变量的定义
#ifdef TEST_EXPORTS
#    define TEST_API __declspec(dllexport)
#else
#    define TEST_API 
#endif
然后只要把工程的配置属性（Configuration Type）简单地在Static Library (.lib)或者Dynamic Library (.dll)切换即可（VC会自动地为DLL添加预处理器TEST_EXPORTS，为静态库取消TEST_EXPORTS）。
2. DLL包含变量定义，也是标准的做法
#ifdef TEST_BUILD_AS_DLL
    #ifdef TEST_EXPORTS
    #    define TEST_API __declspec(dllexport)
    #else
    #    define TEST_API __declspec(dllimport)
    #endif
#else
    #define TEST_API
#endif
如果要将库做成DLL，那么需要DLL创建者添加预处理器TEST_BUILD_AS_DLL和TEST_EXPORTS，后者通常由编译器自动添加；如果做成静态库则不需要添加任何预处理器。
用户则可以通过添加或取消TEST_BUILD_AS_DLL来使用动态库或静态库。
对于DLL的用户，TEST_BUILD_AS_DLL这个名称似乎起得不好。下面的做法或许会更合理些：
#if defined(TEST_API)
#error "macro alreday defined!"
#endif
#if defined(TEST_BUILD_DLL) && defined(TEST_USE_DLL)
#error "macro conflict!"
#endif
#if defined(TEST_BUILD_DLL) // build dll
#    define TEST_API __declspec(dllexport)
#elif defined(TEST_USE_DLL) // use dll
#    define TEST_API __declspec(dllimport)
#else // build or use library, no preprocessor needs
#    define TEST_API    
#endif
相关链接
[微软DLL专题](http://www.microsoft.com/china/community/program/originalarticles/techdoc/dll.mspx)
[Windows API编程之动态链接库（DLL）](http://blog.chinaunix.net/u/18517/showart_309975.html)[](http://www.microsoft.com/china/community/program/originalarticles/techdoc/dll.mspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/hjsunj/archive/2008/01/16/2047376.aspx](http://blog.csdn.net/hjsunj/archive/2008/01/16/2047376.aspx)
