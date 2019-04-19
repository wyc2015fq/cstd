# WinCE中的Debug Zone调试(转载) - xqhrs232的专栏 - CSDN博客
2010年01月15日 16:26:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：928标签：[wince																[initialization																[warnings																[struct																[function																[dll](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=warnings&t=blog)](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/nanjianhui/archive/2008/12/10/3488711.aspx](http://blog.csdn.net/nanjianhui/archive/2008/12/10/3488711.aspx)
相关网帖
1、[WINCE下如何生成debug版dll文件](http://blog.csdn.net/xqhrs232/article/details/8563907)----[http://blog.csdn.net/xqhrs232/article/details/8563907](http://blog.csdn.net/xqhrs232/article/details/8563907)
在WinCE的开发环境中支持Debug Zones功能，通常也被称为调试域，通过它可以控制打印信息。当某个调试域被打开以后，在这个域中的打印信息就会被打印出来，如果某个调试域被关闭了，那么这个域中的打印信息就会被关闭。调试域是基于模块的，也就是说一个模块，可能是在一个驱动或者一个应用中都可以定义一个调试域，用来调试该模块。一个调试域最多可以包括16个域，一般在每一个模块中都会有一个全局变量dpCurSettings，该变量用于描述调试域的相关信息，它由一个模块名字，16个域的名字和一个掩码组成。下面具个例子：
DBGPARAM dpCurSettings = 
{
    TEXT("PCIBUS"), {
    TEXT("Errors"),TEXT("Warnings"),TEXT("Functions"),TEXT("Initialization"),
    TEXT("Enumeration"),TEXT("Load Order"),TEXT("Resource"),TEXT("Undefined"),
    TEXT("Undefined"),TEXT("Undefined"),TEXT("Undefined"),TEXT("Undefined"),
    TEXT("Undefined"),TEXT("Undefined"),TEXT("Undefined"),TEXT("Undefined") },
    0x20
}; 
先来解释一下DBGPARAM结构，该结构在Dbgapi.h中定义，所以在定义dpCurSettings的时候还需要包含这个头文件，该结构定义如下：
typedef struct _DBGPARAM {
    WCHAR   lpszName[32];                     //模块的名字
    WCHAR   rglpszZones[16][32];            //调试域的名字
    ULONG   ulZoneMask;                        //调试域的掩码
}DBGPARAM, *LPDBGPARAM;
在上面的例子中可以看到，第一个是模块的名字，叫PCIBUS。而后定义了16个域的名字，其中只用到了7个域，剩下的都定义为Undefined了。最后一个数字为域的掩码，表示当前哪个域是被激活的，0x20表示只有第6个域是被激活的。从上面的例子还可以看出，前7个域是有意义的，而且按照顺序分别对应1到7。下面针对这些域需要定义相应Debug调试的宏定义：
#define DBGZONE_ERROR                            1
#define DBGZONE_WARNING                      2
#define DBGZONE_FUNCTION                     3
#define DBGZONE_INIT                                 4
#define DBGZONE_ENUM                             5
#define DBGZONE_LOADORDER                 6
#define DBGZONE_RESOURCE                     7
上述宏定义对应在dpCurSettings中的7个域，然后就可以在打印信息的时候，通过这些宏定义来对应相应的调试域了。例如：
while(1) 
{ 
    if (dwFlag) 
    { 
         DEBUGMSG(DBGZONE_ERROR, (L"Error found: %d/r/n", NumDevKeys)); 
         break; 
     } 
     else 
     { 
         DEBUGMSG(DBGZONE_WARNING, (L"Warning found/r/n")); 
     } 
     DEBUGMSG(DBGZONE_LOADORDER, (L"load in a while loop/r/n")); 
     Sleep(100); 
} 
从这段代码可以看出，如果dpCurSettings中的掩码定义为0x20，那么在DEBUGMSG的打印中，只有条件为DBGZONE_LOADORDER才会被打印，循环中的前两个打印信息是不会被打印的。如果想让上面的代码中的所有DEBUGMSG都能打印必须设置掩码如下：
dpCurSettings.ulZoneMask = DBGZONE_ERROR | DBGZONE_WARNING | DBGZONE_LOADORDER;
在一个模块中定义了调试域，如果想在系统中去使用还必须注册该调试域，需要用到的函数叫DEBUGREGISTER(..)，其中要把该调试模块的句柄作为参数传给它。例如：
DllMain(..)
{
    switch(op)
    {
        case DLL_PROCESS_ATTACH:
            DEBUGREGISTER(hPCIBUS);
            break;
        ….
    }
}
完成了上述工作以后，就可以重新编译调试的模块，然后运行系统来调试了。调试域的一个好处就是在Debug的过程中，不需要终止系统可以动态的改变调试域，方便我们分析问题。首先，我们可以基于Platform Builder中的CE Debug Zones来调试，在VS2005的菜单中选择Target，然后选择CE Debug Zones，如图：
然后会出现一个Debug Zones的窗口，在窗口弹出以后，它可能会花一点时间来收集当前支持Debug Zone的模块，如下图：
该图只是一个例子，左边显示了可调试的模块，选择serial_SMDK2410.dll这个模块，就是S3C2410的串口驱动模块。在右侧可以看到各个调试域及名字，用户可以根据需要来选择打开和关闭相应的调试域，最后点击Apply和OK就可以了。
当然，还有其他的方法来修改调试域，一种方法是使用Target Control中的zo命令来修改，Target Control将在以后介绍。还有一种方法就是通过SetDbgZone(..)函数来修改。定义如下：
BOOL SetDbgZone(DWORD dwProcid,  LPVOID lpvMod,  LPVOID baseptr,  DWORD zone, LPDBGPARAM lpdbgTgt)
    dwProcid：     进程的句柄
    lpvMod：        调试模块的句柄
    baseptr：        设置为NULL
    zone：            新的调试域掩码
    lpdbgTgt：      返回新的DBGPARAM结构
上面对Debug Zone的定义，使用以及调试作了大致的介绍，按照上面的步骤可以给一个模块添加调试域，注册调试域并在系统运行以后随时更改调试域，其根本目的无非是帮助我们来调试模块和分析问题。一般情况下，调试域只在Debug模式下使用，但是也可以在Release模式下使用。但是有些地方需要修改，首先前面已经介绍过Debug模式下的打印用DEBUGMSG，而Release模式下的打印应该使用RETAILMSG函数。所以在Release模式下，打印函数应该改为RETAILMSG函数。还有在注册调试域的时候，不能再使用DEBUGREGISTER(..)函数，而是应该改用RETAILREGISTERZONES(..)函数。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/nanjianhui/archive/2008/12/10/3488711.aspx](http://blog.csdn.net/nanjianhui/archive/2008/12/10/3488711.aspx)
