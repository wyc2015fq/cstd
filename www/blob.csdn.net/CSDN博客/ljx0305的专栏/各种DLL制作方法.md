# 各种DLL制作方法 - ljx0305的专栏 - CSDN博客
2009年06月09日 17:35:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1140标签：[dll																[exe																[mfc																[float																[winapi																[class](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=exe&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[DLL																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/403907)
各种DLL制作方法
一.创建MFC 的常规DLL(设工程名为MyDLL1)(详工程F:/VcSample/DLL示例/DLL动态联接库之构共享内存)
1.     新建工程MFC AppWizard(dll),选第二项- Regular DLL using shared MFC DLL
(选第一项:Regular DLL with MFC statically linked,同第二项的区别是静态联接MFC)
2.     则系统自动建立文件:MyDLL1.h、MyDLL1.cpp、MyDLL1.def等文件.
 文件MyDLL1.h内容: (略写)  MyDLL1.cpp内容（略写） 
class CMyDLL1 App : public CWinApp
{public:
CMyDLL1App();
    DECLARE_MESSAGE_MAP()
};
//+1 //+3
BEGIN_MESSAGE_MAP(CMyDLL1App, CWinApp)
END_MESSAGE_MAP()
CMyDLL1App:: CMyDLL1App()
{    }
//+2
CMyDLL1App theApp; 
文件MyDLL1.def 内容    
LIBRARY      " MyDLL1"
DESCRIPTION 'MyDLL1 Windows
               Dynamic Link Library'
EXPORTS
    ; 外部函数出口写在这里 在.def文件中,每行前加上分号表示
注释内容 
3.     在+1的位置上加入自定义的函数
如:#define DLLEXPORT extern "C" _declspec(dllexport)
         DLLEXPORT int WINAPI GetCount();       //DLL中定义的函数GetCount()
         DLLEXPORT void WINAPI AddCount();     // DLL中定义的函数AddCount()
4.     在+2的地方加入在头文件定义的函数的函数体代码内容
int WINAPI GetCount()
{return iCount; }    //返回计数值
void WINAPI AddCount()
{ iCount++;}      //增加计数值
5.     若要求支持DLL中某些变量为全局共享变量（任何进程改动该变量都会影响其它进程调用的值）,则应在+3的位置加入代码段
#pragma data_seg("PANTO")      //字符参数”PANTO“详文件DLLDemo.def中的定义，
//表示这#... #段之间的变量为共享内存变量
     int iCount=0;            //初始化共享类成员变量为0
#pragma data_seg()            //即变量iCount不管哪个进程何时调用并更改后，其以后调//用均采用最后一次更改的值
6.     更改MyDLL1.def的内容如下:
; DLLDemo.def :在这个文件中为这个DLL声明和定义模块参数.
LIBRARY      "DLLDemo"
DESCRIPTION 'DLLDemo Windows 动态联接库'
EXPORTS
    ; 在这里写输出定义的函数及分配序号
    ;给函数指定一个顺序号以便可以用函数GetProcAddress()调用这个数字
      GetCount @1    
     AddCount @2
;声明PANTO段为共享段,详执行文件DLLDemo.cpp中的变量定义及初始化
SECTIONS 
PANTO SHARED    ;PANTO仅表示一标签不是关键字，表示在PANTO块中为共享变量
7.     在EXE工程中要调用这个DLL库中的两个函数应：
1>在要调用的函数相关文件中 #include " MyDLL1.h"
2>在调用时，不必寻找DLL中函数入口地址，如:
txt.Format("从DLL中调用共享内存变量iCount当前值=%d",GetCount()); (由于在EXE工程//中已包含了DLL工程中的主头文件,这里就不必写判断函数入口地址的代码了)
pDC->DrawText(txt,&ClientRect,DT_BOTTOM);             //往视图上写字符
AddCount();   //调用DLL中的函数,如使公用变量iCount值+1。
二.创建MFC扩展DLL工程(设工程名为MyDLL2) (详工程 F:/VcSample/DLL示例/VC动态联接库示例/DYNLINK3)
1.     MFC AppWizard(dll),选第三项-MFC Extension DLL(using shared MFC DLL)
2.     则系统自动建立文件:MyDLL2.cpp、MyDLL2.def等文件.
 文件MyDLL2.cpp内容: (略写)  
static AFX_EXTENSION_MODULE MyDLL2DLL = { NULL, NULL };
extern "C"  int  APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, 
LPVOID lpReserved)
{//删除这些若你使用lpReserved
UNREFERENCED_PARAMETER(lpReserved);
if (dwReason == DLL_PROCESS_ATTACH)
{   TRACE0("MYDLL2.DLL 正初始化!/n");
     if (!AfxInitExtensionModule(MyDLL2DLL, hInstance))
         return 0;
     new CDynLinkLibrary(MyDLL2DLL);
}
else if (dwReason == DLL_PROCESS_DETACH)
{   TRACE0("MYDLL2.DLL终止!/n");
     AfxTermExtensionModule(MyDLL2DLL);
}
return 1;   // 成功
}
文件MyDLL2.def 内容  
LIBRARY      " MyDLL2"
DESCRIPTION 'MyDLL2 Windows Dynamic Link Library'
EXPORTS
; 外部函数出口写在这里 
3.     在DLL工程中任意加入MFC类或继承类（可为对话框类等等），若增加的类要求在
EXE工程中可以象在本工程一样的引用方式，则定义的类名前应加上AFX_EXT_CLASS关键字：如class AFX_EXT_CLASS CDllDialog : public Cdialog {…};
4.     将生成的MyDLL2.dll及MyDLL2.lib文件拷贝到EXE工程目录下，并将MyDLL2.lib文件包含到EXE工程的资源中。
5.     若在EXE文件中要用到DLL中的D11Dialog对话框类，则还应在相应头文件include
#include “D11Dialog.h”    //可将此文件从DLL工程目录复制到EXE工程目录
6.     在调用时DLL中函数或引用DLL中类时，无需再查找DLL入口地址，可象本工程中的
其它函数或类一样直接使用.
如: CDllDialog dll; //CDllDialog类在动态联接库文件DYNLINK3.DLL中定义的
   dll.DoModal();
三．寻址方式调用DLL（DLL工程中的任何文件不包含在EXE工程中）
1. 新建一Win32 Dynamic-Link Library类型工程，选项二-A mpty DLL project
2．则生成的DLL工程中不含有任何代码文件，这时可自行加入以下代码，若文件名为MyDLL3.c(把文件包含工程中)
#include <windows.h>
#include <string.h>
#include <stdio.h>
#define DllExport __declspec(dllexport)
///////////////////////////////////////////////////////////////////
//定义一个可输出的返回值为long类型的函数Multiply2Longs
DllExport long Multiply2Longs(LONG lNum1, LONG lNum2) 
{    return lNum1 * lNum2; 
}
//定义一个可输出的返回值为char*的函数GetStr
DllExport char* GetStr(char* str1)
{static char mychar[81];
 char* CHARlink;
 strcpy(mychar,"static char mychar[81]=DLL中定义的的字符串");
 CHARlink=mychar;
 strcat(CHARlink,"(加EXE传入DLL中的char *str1=)");
 strcat(CHARlink,str1);
 return CHARlink;
}
3.若要使用DLL中的GetStr函数，则先应把DLL文件复制到EXE工程目录下
void CMainWnd::OnGetStr()
{ CHAR *CResult;
 HINSTANCE hModule;
typedef CHAR *(MyType)(CHAR *);
 MyType* pfunMyType =NULL; //*
VERIFY(hModule = ::LoadLibrary("MyDLL3.dll")); //导入文件MyDLL3.DLL
try{  //得到DLL中函数GetStr()的地址
   VERIFY(
      pfunMyType = 
     (MyType*)::GetProcAddress(
            (HMODULE) hModule, "GetStr")
   );
    CHAR *ch;
    ch="EXE传过去的字符串";
   CResult = (*pfunMyType)(ch);
   CString sMsg;    //显示测试结果
   sMsg.Format("调用DLL文件MyDLL3.dll中函数:/n DllExport CHAR *GetStr(CHAR * str1)/n{char *mychar;/nmychar=/'/'......./'/';/n........../n return mychar;(以下为DLL中返回字符串内容:)/n}/n :/n/n/n %s", 
                CResult);
 ::MessageBeep(MB_OK);
   MessageBox(sMsg, "GetStr()", MB_OK | MB_ICONINFORMATION);
 }
 catch(...)// 释放DLL
 {MessageBox("调用DLL中函数失败","错误",MB_OK|MB_ICONINFORMATION);}
   FreeLibrary(hModule);
 }
（注：所有的同EXE无任何关联的DLL都可以用上述方法调用，但必须保证函数名和参数准确）
四．静态调用（设工程名为MyDLL4）
1.同第一种新建工程MFC AppWizard(dll),选第二项- Regular DLL using shared MFC DLL
2.可删除MyDLL4.def文件不用，将系统生成的MyDLL4.h 及MyDLL4.cpp文件更改如下:
MyDLL4.h内容 MyDLL4.cpp内容 
define DllExport __declspec(dllexport)
//定义DLL中可导出的全局函数
extern "C" DllExport FLOAT MyDLLSub(
FLOAT f1, FLOAT f2);
class CMyDLL4App : public CWinApp
{
public:
    CMyDLL4App();
};
  #include "stdafx.h"
#include "resource.h"   // main symbols
#include "MyDLL4.h"
///////////////////////////////////////////////////////////////////
CMyDLL4App:: MyDLLSub ()
{ }
extern "C" DllExport FLOAT MyDLLSub (FLOAT f1, FLOAT f2)
{   return f1 * f2;
}
///////////////////////////////////////////////////////////////////
CMyDLL4App MyDll; 
3.将编绎的MyDLL4.lib文件复制到EXE工程目录下并将它包含含到EXE工程资源中。
 1>在EXE主头文件中加入行:
#define DllImport __declspec(dllimport)
extern "C" DllImport FLOAT MyDLLSub(FLOAT f1, FLOAT f2);
 2>在要调用DLL中的函数MyDLLSub的模块文件中
void CMainWnd::OnFileCallRegularDll()
{ FLOAT f1 = 25.3f; 
   FLOAT f2 = 13.5f;
   FLOAT fResult = MyDLLSub(f1, f2);   //不用寻找DLL入口地址
   CString sMsg;
 sMsg.Format("调用DLL文件dynlink2.dll中函数:/n extern /'C/'/' DllExport FLOAT MyDLLSub(FLOAT f1, FLOAT f2)/n {return f1 * f2;}/n %0.2f * %0.2f = %0.2f",
               f1, f2, fResult);       //显示测试结果
   ::MessageBeep(MB_OK); 
   MessageBox(sMsg, "从MyDLL4.dll中调用函数MyDLLSub（）", MB_OK | 
MB_ICONINFORMATION); 
}
五．纯资源DLL的编制
1.新建工程: 创建一个WIN32 DLL(Win32 Dynamic-Link Library)工程，不是MFC的DLL
2. 资源的DLL就是只包含资源的DLL，例如：图标，位图，字符串，声音，视频，对话框等。使用纯资源DLL可以节约可执行文件的大小，可以被所有的应用程序所共享，从而提高系统性能。纯资源DLL的编写比普通的DLL要简单的多，工程工作空间并没有资源TAB栏,需在向工程中插入资源后系统会提示产生.RC文件,或创建一个资源文件 *.RC，添加到资源DLL的工程中去。然后添加一个初始化DLL的原文件。
如系统默认提供了以下几种:
Accelerator:加速键             Dialog:      对话框
Bitmap:     位图               HTML:        超文本文件
Cursor:     光标               Icon:        图标
Menu:       菜单               String Table:字符串表
Toolbar:    工具条             Version:     版本号
还可以自行导入任何类型的资源(资源名称自己定)如:
AVI:动画                    WAV:声音文件             DATA:二进制文件 等等
注意:加入这些自定义资源后,在工作空间资源TAB上显示的资源名有双引号括起
3.示例代码
#include <windows.h>
extern "C"
BOOL WINAPI DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID )
{    return 1;}
这是纯资源DLL所必须需的代码，保存这个文件为*.CPP。编译这个资源DLL。
在应用程序显示的调用这个DLL，使用LoadLibrary函数装入资源DLL，FindResource和LoadResource来装入各种资源，或者使用下列的特定的资源装入函数： 
FormatMessage            LoadAccelerators            LoadBitmap 
LoadCursor               LoadIcon                    LoadMenu 
LoadString 
4. 在EXE工程中调用纯资源DLL的方法:
当资源使用结束，你的应用程序须调用FreeLibrary函数来释放资源。
下面就讲一下如何调用编写好的资源DLL
首先在应用程序中声明一个DLL的句柄，HINSTANCE m_hLibrary;在OnCreate( )
函数中调用LoadLirbrary( ),在OnDestory( )中调用FreeLibrary（）。
六．自定义LIB类库的DLL制作过程:
1.在StdAfx.h文件中加入定义
    #ifndef _CJX_EXT_CLASS
     #ifdef _AFXDLL
          #define _CJX_EXT_CLASS     AFX_CLASS_EXPORT //总时输出本类库文件...
     #else
          #define _CJX_EXT_CLASS     //不使用静态联编辑.
     #endif
     #endif // _CJX_EXT_CLASS
2.在DLL主头文件中加入(主头文件要自己加入)
//define中的字符串可根据实际进行更改,以符合字意
#ifndef _CJXLIB_INLINE
#define _CJXLIB_INLINE inline
#endif // _AFXCMN_INLINE
#ifndef _CJX_EXT_CLASS
#ifdef _AFXDLL
#define _CJX_EXT_CLASS     AFX_CLASS_EXPORT //总是输出本DLL文件...
#else
#define _CJX_EXT_CLASS     // 静态联编DLL文件.
#endif
#endif // _CJX_EXT_CLASS
#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif
#ifndef __AFXPRIV_H__
#include <afxpriv.h>
#endif
//#include <../src/afximpl.h> //屏蔽这一系统产生的这一行
#define _delete(p){if(p){delete p;p=NULL;}}
#define _deleteMeta(p){if(p){::DeleteEnhMetaFile(p);p=NULL;}}
#ifndef __MYCOMBOBOXEX_H__ //示例:DLL中有一自定义类CMyComboBox 中的定义#define __MYCOMBOBOXEX_H__ 
#include "MyComboBox.h"
#endif
//..............
//以下定义在EXE工程中编绎加载LIB库时出现的编绎信息
#ifdef _AFXDLL
 #ifdef _DEBUG
   #define _CJX_COMMENT     "MyLibd.lib"
   #define _CJX_MESSAGE     "MyLibd类库将自动联接使用动态联接调试版 MyLibd.dll"
 #else
   #define _CJX_COMMENT     "MyLib.lib"
   #define _CJX_MESSAGE     "MyLib类库将自动联接使用动态联接发行版MyLib.dll"
 #endif // _DEBUG
#else
 #ifdef _DEBUG
   #define _CJX_COMMENT     "MyLibdStaticd.lib"
   #define _CJX_MESSAGE     "MyLib类库将自动联接使用静态联接调试版MyLibdStaticd.dll"
 #else
   #define _CJX_COMMENT     "MyLibStatic.lib"
   #define _CJX_MESSAGE     "MyLib类库将自动联接使用静态联接发行版MyLibStaticd.dll"
 #endif // _DEBUG
#endif // _AFXDLL
#pragma comment(lib, _CJX_COMMENT) 
#pragma message(_CJX_MESSAGE)
#endif // __CJLIBRARY_H__
///////////////////////////////////////////////////////////////////////
3.在DLL主模块文件中加入
#include "DLL主头文件"
在DllMain()函数尾加入处理当前打开的是否是正确的DLL版本[可选]
     _GetComCtlVersion(); //函数为自定义函数,用于得到系统COMCTL32.DLL的版本号
     if (_ComCtlVersion < VERSION_IE4)
     {
          CString str = "你调用的COMCTL32.DLL文件版本应在 4.2或更高才可以保证程序正常运行.../n注意!/n/nCopyright ?1998-99 罗伟";
          ::MessageBox(NULL, str, "加载 COMCTL32.DLL失败", MB_ICONSTOP);
         return 0;
     }
     return 1;   // ok
//////////////////////////////////////////////////////////////////
4.向DLL工程中加入类的定义:如重载CComboBox为CMyComboBox 
1>用向导向工程中加入类CMyComboBox 生成对应的两文件MyComboBox.h    MyComboBox.CPP
2>修改头文件的define定义为(也可不修改,但向导产生的太长)
    #ifndef __MYCOMBOBOXEX_H__
    #define __MYCOMBOBOXEX_H__
3>修改类名定义为
class _CJX_EXT_CLASS CMyComboBoxEx : public CComboBox
{     DECLARE_DYNAMIC(CMyComboBoxEx)
...........................
};
//可在类体外加入内联函数的函数定义.如:
_CJXLIB_INLINE CMyComboBoxEx::CMyComboBoxEx()
     { }
_CJXLIB_INLINE DWORD CMyComboBoxEx::GetExtendedStyle() const
     { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, CBEM_GETEXTENDEDSTYLE, 0, 0); }
_CJXLIB_INLINE DWORD CMyComboBoxEx::SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles)
     { ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, CBEM_SETEXTENDEDSTYLE, (DWORD) dwExMask, (LPARAM) dwExStyles); }
4>在类中加入重载或自定义的函数体如:
int CMyComboBoxEx::InsertItem(int iItem, LPCTSTR lpszItem, int iIndent, int iImage, int iSelectedImage, UINT mask)
{
     COMBOBOXEXITEM cbItem;
     cbItem.mask                   = mask;
     cbItem.iItem             = iItem;
     cbItem.pszText           = (LPSTR)lpszItem;
     cbItem.iImage            = iImage;
     cbItem.iSelectedImage    = iSelectedImage;
     cbItem.iIndent           = iIndent;
     return InsertItem(&cbItem);
}
BOOL CMyComboBoxEx::SetItem(const COMBOBOXEXITEM* pCBItem)
{     ASSERT(::IsWindow(m_hWnd));
     ASSERT(pCBItem != NULL);
     ASSERT(AfxIsValidAddress(pCBItem, sizeof(COMBOBOXEXITEM), FALSE));
     return (int) ::SendMessage(m_hWnd, CBEM_SETITEM, 0, (LPARAM) pCBItem);
}
////////////////////////////////////////////////////////////////////////////////////////////
5.在EXE中使用LIB类库的方法
1>在EXE 工程中的StdAfx.h文件中包含DLL工程主头文件
#include "..//include//CJLibrary.h" //包含这个文件，可用DLL工程CJLibary动态联接库中的所有类
2>将.DLL 和.LIB文件复制到EXE 工程目录下或在EXE工程中设置LIB库文件所在目录即可
3>在要使用DLL 中的类定义的类中定义成员变量
   CMyComboBoxEx * cCombo;
在函数体中象一般变量一样使用DLL中的类函数
3>也可以重载DLL中的类如下例
class CCombo :public CmyComboBoxEx{…..}
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/PentiumMMX/archive/2007/05/11/1605062.aspx](http://blog.csdn.net/PentiumMMX/archive/2007/05/11/1605062.aspx)
