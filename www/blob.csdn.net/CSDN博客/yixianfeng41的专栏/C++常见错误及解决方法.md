# C++常见错误及解决方法 - yixianfeng41的专栏 - CSDN博客
2015年08月12日 11:04:07[第2梦](https://me.csdn.net/yixianfeng41)阅读数：4095
所属专栏：[C++编程](https://blog.csdn.net/column/details/14514.html)
﻿﻿
**1、针对出现无法初始化问题的错误进行解析。 **
错误例子： 
> 
TCHAR* strLastSlash = _tcsrchr( str, TEXT(‘//’) )
error C2440: “初始化”: 无法从“const wchar_t *”转换为“TCHAR *”
修改：
> 
可以在=的右边进行强制转换。如下：
TCHAR* strLastSlash = (TCHAR*)_tcsrchr( str, TEXT(‘//’) ); 
**2、针对警告被视为错误的问题进行解析。 **
错误例子： 
> 
 c:/dxsdk/samples/c++/misc/dxdiagreport/dxdiaginfo.cpp(322) : error C2220: 警告被视为错误- 没有生成“object”文      件。
错误原因：
> 
原因是该文件的代码页为英文，而我们系统中的代码页为中文。
解决方案：
> 
启动Microsoft Visual Studio 2005，文件->打开->选择c:/dxsdk/samples/c++/misc/dxdiagreport/dxdiaginfo.cpp，然后保存。从新打开程序文件运行，此错误不再出现。如果上述不能去掉错误，还可以点击项目，右击选择属性->配置属性->c/c++->常规，将“警告视为错误”的选项改为“否”。就可以！
**3、编译Capture,无法打开libci.lib**
出现错误：
> 
LINK : fatal error LNK1104: 无法打开文件“libci.lib”
生成日志保存在“file://d:/DXSDK/Samples/C++/DirectShow/Capture/CaptureTex/Debug/BuildLog.htm”
CaptureTex - 1 个错误。 
错误原因：
> 
缺少数据库LIBCI.LIB在2003和2005中已经不存在了
解决方案：
> 
将库文件指向VC6中的库文件。
**4、编译PlayWndASF 错误：**
> 
playwndasf.obj : error LNK2001: 无法解析的外部符号“unsigned int (__stdcall* ATL::g_pfnGetThreadACP)(void)” (?g_pfnGetThreadACP@ATL@@3P6GIXZA)
urllaunch.obj : error LNK2001: 无法解析的外部符号“unsigned int (__stdcall* ATL::g_pfnGetThreadACP)(void)” (?g_pfnGetThreadACP@ATL@@3P6GIXZA)
./Debug_Unicode/PlayWndASF.exe : fatal error LNK1120: 1 个无法解析的外部命令
生成日志保存在“file://d:/DXSDK/Samples/C++/DirectShow/Players/PlayWndASF/Debug_Unicode/BuildLog.htm”
PlayWndASF - 3 个错误，个警告
========== 全部重新生成: 1 已成功, 1 已失败, 0 已跳过==========
更改： 
> 
向数据库依赖项中添加atlsd.lib
**5、编译demos（D:/DXSDK/Samples/C++/Demos） **
错误： 
> 
StdAfx.cpp 
d:/program files/microsoft directx sdk (april 2007)/include/d3d9types.h(1385) : warning C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为Unicode 格式以防止数据丢失
d:/DXSDK/Samples/C++/common/include/D3DFile.h(56) : error C2061: 语法错误: 标识符“LPDIRECTXFILEDATA”
d:/DXSDK/Samples/C++/common/include/D3DFile.h(111) : error C2061: 语法错误: 标识符“LPDIRECTXFILEDATA”
d:/DXSDK/Samples/C++/common/include/D3DFile.h(113) : error C2061: 语法错误: 标识符“LPDIRECTXFILEDATA”
生成日志保存在“file://d:/DXSDK/Samples/C++/Demos/Donuts4/Debug/BuildLog.htm”
Donuts4 - 3 个错误，个警告
========== 全部重新生成: 0 已成功, 1 已失败, 0 已跳过==========
更改:
> 
在dsdfile.h的顶部加入#include <dxfile.h>就可以 再次编译出现错误：
**6、error C1010: 在查找预编译头时遇到意外的文件结尾。是否忘记了向源中添加“#include "stdafx.h"”?  或者 fatal error C1010: unexpected end of file while looking for precompiled header directive **
解决办法：
> 
1）检查出错文件是否忘记了添加#include”stdafx.h"，如果忘记了，加上；
2）要注意下类和结构结束要加分号，要是忘了也会出错。
3）#include”stdafx.h"一定要放在最开头。
4）开始没错误，如果是添加了几个文件进当前项目，添加了#include”stdafx.h",还是出现了这个问题，退而求其次，可以选择在解决方案目录下，右键击出错的文件，属性-》c/c++-》预编译头文件-》不使用。
**7、错误17error MSB3073: 命令“regsvr32 /s /c ".........**
解决办法：
> 
在项目上点右键，选择属性”->“配置属性”->“常规”->“目标文件扩展名”，设置为.ocx 或者.dll；
在项目上在项目上点右键，选择“属性”->“配置属性”->“链接器”->“
 常规 ”->“输出文件”，设置为$(OutDir)$(ProjectName)$(TargetExt)
**8、Activex 使用DirectShow的问题解决方案**
下面的编译问题： 
> 
strmbasd.lib(dllentry.obj) : error LNK2001: unresolved external symbol "class CFactoryTemplate * g_Templates" (?g_Templates@@3PAVCFactoryTemplate@@A)
strmbasd.lib(dllentry.obj) : error LNK2001: unresolved external symbol "int g_cTemplates" (?g_cTemplates@@3HA)
解决方法： 
> 
在xxxctrl.h中加入下面的代码
/////////////////////////////////////////////////////////////////////////////
// DllGetClassObject
extern "C" 
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{ 
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      return AfxDllGetClassObject(rclsid, riid, ppv);
} 
// DllCanUnloadNow
extern "C" 
STDAPI DllCanUnloadNow(void)
{ 
      AFX_MANAGE_STATE(AfxGetStaticModuleState());
      return AfxDllCanUnloadNow();
}
**9、错误：**
![](https://img-blog.csdn.net/20150810190406136)
解决办法：
         项目属性-》配置属性-》自定义生成步骤 ，修改“输出”项为“从父级或项目默认属性继承”。
> 
**10、error:**
> vtkCommon.lib(vtkSmartPointerBase.obj) : error LNK2038: 检测到“_ITERATOR_DEBUG_LEVEL”的不匹配项:**值“0”不匹配值“2”**(cloudviewer.obj
 中)
1>vtkCommon.lib(vtkGarbageCollector.obj) : error LNK2038: 检测到“_ITERATOR_DEBUG_LEVEL”的不匹配项**: 值“0”不匹配值“2”**(cloudviewer.obj 中)
1>vtkCommon.lib(vtkDebugLeaksManager.obj) : error LNK2038: 检测到“_ITERATOR_DEBUG_LEVEL”的不匹配项:**值“0”不匹配值“2”**(cloudviewer.obj 中)
错误原因是：
> 
Debug使用了Release的库文件。
即使你连接库里面两个都添加着呢，但是release库文件放在了debug前面，也是出错的。默认按顺序使用库文件。
类似错误：如release下使用了Debug的库文件，报错类似：
error LNK2038: 检测到“_ITERATOR_DEBUG_LEVEL”的不匹配项:**值“2”不匹配值“0”.**
> 
