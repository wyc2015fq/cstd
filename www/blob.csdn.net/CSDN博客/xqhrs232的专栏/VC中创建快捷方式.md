# VC中创建快捷方式 - xqhrs232的专栏 - CSDN博客
2017年08月09日 23:55:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：185
原文地址::[http://blog.csdn.net/thanklife/article/details/69259017](http://blog.csdn.net/thanklife/article/details/69259017)
相关文章
1、[VC
 为程序创建快捷方式的详细讲解](http://www.cnblogs.com/morewindows/archive/2011/08/14/2138412.html)----[http://www.cnblogs.com/morewindows/archive/2011/08/14/2138412.html](http://www.cnblogs.com/morewindows/archive/2011/08/14/2138412.html)
2、[VC++ 编程实现开机自启动](http://blog.csdn.net/wangwenjing90/article/details/8720529)----[http://blog.csdn.net/wangwenjing90/article/details/8720529](http://blog.csdn.net/wangwenjing90/article/details/8720529)

1、共同函数
BOOL CreateLink(LPSTR   szPath,//快捷方式的目标应用程序名   
          LPSTR   szLink)//快捷方式的数据文件名(*.lnk)   
{   
HRESULT   hres;   
IShellLink* psl;   
IPersistFile*   ppf;   
WCHAR wsz[MAX_PATH];  
////初始化COM   
CoInitialize(NULL);  
//创建一个IShellLink实例   
hres = CoCreateInstance(CLSID_ShellLink,NULL, CLSCTX_INPROC_SERVER,   IID_IShellLink, (void   **)&psl);   
if(FAILED(hres))   
{   
   CoUninitialize();   
   return FALSE;   
}  
//设置目标应用程序   
psl->SetPath(szPath);  
//设置快捷键(此处设为Shift+Ctrl+'R')   
//         psl   ->   SetHotkey(   MAKEWORD(   'R',   
// HOTKEYF_SHIFT   |HOTKEYF_CONTROL))   ;  
CString csWorkDir(szPath);
int idx = csWorkDir.ReverseFind('\\');
csWorkDir = csWorkDir.Left(idx);
psl->SetWorkingDirectory(csWorkDir);  
//从IShellLink获取其IPersistFile接口   
//用于保存快捷方式的数据文件   (*.lnk)   
hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
if(FAILED(hres))
{   
   CoUninitialize();   
   return FALSE;   
}  
//确保数据文件名为ANSI格式   
MultiByteToWideChar(CP_ACP, 0, szLink, -1, wsz, MAX_PATH);  
//调用IPersistFile::Save   
//保存快捷方式的数据文件   (*.lnk)   
hres = ppf ->Save(wsz, STGM_READWRITE);   
//释放IPersistFile和IShellLink接口   
ppf   ->   Release(   )   ;   
psl   ->   Release(   )   ;  
CoUninitialize();
return   TRUE;   
}
2、创建桌面快捷方式
void CreateDestopShortcut(LPSTR szPath, LPSTR szLinkName)   
{   
//快捷方式的数据文件名
char szLink[MAX_PATH] = {0};  
SHGetSpecialFolderPath(NULL, szLink, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE);
if(szLink[lstrlen(szLink)-1] == '\\')
{
   lstrcat(szLink, szLinkName); 
}
else
{
   lstrcat(szLink, "\\"); 
   lstrcat(szLink, szLinkName); 
}
//创建快捷方式   
if(!CreateLink(szPath, szLink))   
   return;
}
3、创建快速启动快捷方式
void CSdFinishDlg::CreateQuickShortCut(LPSTR szPath, LPSTR szLinkName)
{
//快捷方式的数据文件名
char szLink[MAX_PATH] = {0};
SHGetSpecialFolderPath(NULL, szLink,CSIDL_APPDATA, FALSE);
if(szLink[lstrlen(szLink)-1]   ==   '\\') 
{
   lstrcat(szLink,   "Microsoft\\Internet Explorer\\Quick Launch\\");   
}
else
{
   lstrcat(szLink,   "\\Microsoft\\Internet Explorer\\Quick Launch\\");
}
lstrcat(szLink, szLinkName);
if(!CreateLink(szPath, szLink))   
   return;   
}
4、创建开始菜单启动项的快捷方式
void CSdFinishDlg::CreateStartupShortCut(LPSTR szPath, LPSTR szLinkName)
{
//快捷方式的数据文件名
char szLink[MAX_PATH] = {0};
SHGetSpecialFolderPath(NULL, szLink, CSIDL_COMMON_STARTUP, FALSE);
if(szLink[lstrlen(szLink)-1] == '\\')
{
   lstrcat(szLink, szLinkName); 
}
else
{
   lstrcat(szLink, "\\"); 
   lstrcat(szLink, szLinkName); 
}
if(!CreateLink(szPath, szLink))   
   return; 
}
完整代码：
#include <stdio.h>
#include <windows.h>
#include <Shlobj.h>
#include <objbase.h>
#pragma comment(lib, "shell32")
#pragma comment(lib, "ole32.lib")
//#include "stdafx.h"
BOOL CreateLink(LPSTR   szPath,//快捷方式的目标应用程序名   
    LPSTR   szLink)//快捷方式的数据文件名(*.lnk)   
{   
 HRESULT   hres;   
 IShellLink* psl;   
 IPersistFile*   ppf;   
 WCHAR wsz[MAX_PATH];  
 ////初始化COM   
 CoInitialize(NULL);  
 //创建一个IShellLink实例   
 hres = CoCreateInstance(CLSID_ShellLink,NULL, CLSCTX_INPROC_SERVER,   IID_IShellLink, (void   **)&psl);   
 if(FAILED(hres))   
 {   
  CoUninitialize();   
  return FALSE;   
 }  
 //设置目标应用程序   
 psl->SetPath(szPath);  
 //设置快捷键(此处设为Shift+Ctrl+'R')   
 //         psl   ->   SetHotkey(   MAKEWORD(   'R',   
 // HOTKEYF_SHIFT   |HOTKEYF_CONTROL))   ;  
// CString csWorkDir(szPath);
 char csWorkDir[MAX_PATH] = {0};
 sprintf(csWorkDir, "%s", szPath);
 int i= 0;
 for(i=strlen(csWorkDir); i>=0; i--)
 {
  if(csWorkDir[i] == '\\')
  {
   csWorkDir[i+1] = 0;
   break;
  }
 }
 //int idx = csWorkDir.ReverseFind('\\');   
 //csWorkDir = csWorkDir.Left(idx);        
 psl->SetWorkingDirectory(csWorkDir);  
 //从IShellLink获取其IPersistFile接口   
 //用于保存快捷方式的数据文件   (*.lnk)   
 hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
 if(FAILED(hres))
 {   
  CoUninitialize();   
  return FALSE;   
 }  
 //确保数据文件名为ANSI格式   
 MultiByteToWideChar(CP_ACP, 0, szLink, -1, wsz, MAX_PATH);  
 //调用IPersistFile::Save   
 //保存快捷方式的数据文件   (*.lnk)   
 hres = ppf ->Save(wsz, STGM_READWRITE);   
 //释放IPersistFile和IShellLink接口   
 ppf   ->   Release(   )   ;   
 psl   ->   Release(   )   ;  
 CoUninitialize();
 return   TRUE;   
}
void CreateStartupShortCut(LPSTR szPath, LPSTR szLinkName)
{
 //快捷方式的数据文件名
 char szLink[MAX_PATH] = {0};
 SHGetSpecialFolderPath(NULL, szLink, CSIDL_COMMON_STARTUP, FALSE);
 if(szLink[lstrlen(szLink)-1] == '\\')
 {
  lstrcat(szLink, szLinkName); 
 }
 else
 {
  lstrcat(szLink, "\\"); 
  lstrcat(szLink, szLinkName); 
 }
 if(!CreateLink(szPath, szLink))   
  return; 
}
int main()
{
// CreateStartupShortCut("C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE","IEXPLORE.LNK");
 CreateStartupShortCut("C:\\Program Files\\Thunder Network\\Thunder\\Program\\Thunder.exe","Thunder.LNK");
 return 1;
}
