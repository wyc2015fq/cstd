# WINCE6.0下构建SilverLight程序的基本框架 - xqhrs232的专栏 - CSDN博客
2009年12月28日 15:32:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2039
//Main.cpp
//Designed by  xqh
//Date:2009--12--28
//#include <pwinuser.h> 
#include <XamlRuntime.h>
#include <XRDelegate.h>
#include <XRPtr.h>
#include <Ehm.h>
#include "MainDlg.h"
//=================================================================================================================
IXRApplication* pApplication = NULL;
//###################################################################################################################
INT WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPWSTR lpCmdLine, 
    int nCmdShow
    )
{
   int    exitCode = -1;
   BOOL   bRet;
   bRet= XamlRuntimeInitialize();
   GetXRApplicationInstance(&pApplication);
   CMainDlgBtnEventHandler  Handler;
   Handler.Init(pApplication,L"//Storage Card//XAMLPERF//Page_8.xaml");
   return exitCode;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//MainDlg.h
//Designed by xqh
//Date:2009--12--28
#ifndef  _MainDlg_H_
#define  _MainDlg_H_
#include <XamlRuntime.h>
#include <XRDelegate.h>
#include <XRPtr.h>
#include <Ehm.h>
class CMainDlgBtnEventHandler
{
public:
CMainDlgBtnEventHandler( );
~CMainDlgBtnEventHandler( );
void  Init(IXRApplication* pApplication,LPCTSTR strXAMLFilePath);
HRESULT BtnRadio_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args);
HRESULT BtnRadio_MouseEnter(IXRDependencyObject* source,XRMouseEventArgs* args);
HRESULT BtnDVD_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args);
HRESULT BtnCDC_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args);
HRESULT BtnSystem_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args);
HRESULT BtnClose_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args);
IXRApplication* pMainDlgApplication;
IXRVisualHost* pMainDlgVisualHost;
XRXamlSource  pMainDlgSourceXaml;
IXRFrameworkElementPtr pMainDlgroot;
};
#endif
//MainDlg.cpp
//Designed by xqh
//Date:2009--12--28
#include "MainDlg.h"
#include "RadioDlg.h"
CMainDlgBtnEventHandler::CMainDlgBtnEventHandler( )
{
}
CMainDlgBtnEventHandler::~CMainDlgBtnEventHandler( )
{
}
void CMainDlgBtnEventHandler::Init(IXRApplication* pApplication,LPCTSTR strXAMLFilePath)
{
HRESULT hr;
pMainDlgApplication=pApplication;
pMainDlgSourceXaml.SetFile(strXAMLFilePath);
XRWindowCreateParams WindowParameters;
ZeroMemory(&WindowParameters, sizeof(WindowParameters));
WindowParameters.Style       = WS_POPUP;
WindowParameters.pTitle      = L"MainDlg";
WindowParameters.Left        = 0;
WindowParameters.Top         = 0;
pMainDlgApplication->CreateHostFromXaml(&pMainDlgSourceXaml, &WindowParameters, &pMainDlgVisualHost);
if(!pMainDlgVisualHost)
 RETAILMSG(1,(TEXT("++++the pMainDlgVisualHost is NULL!/r/n")));
if (FAILED(hr=pMainDlgVisualHost->GetRootElement(&pMainDlgroot)))       
   return;
//----------------------------------------------------------------------------------------------------------
IXRButtonBasePtr  btn;  
IXRDelegate<XRMouseButtonEventArgs>* clickdelegate;
if (FAILED(hr=pMainDlgroot->FindName(TEXT("Radio"), &btn)))        
   return;
if (FAILED(hr=CreateDelegate(this,&CMainDlgBtnEventHandler::BtnRadio_OnClick,&clickdelegate)))       
   return;
if (FAILED(hr=btn->AddClickEventHandler(clickdelegate)))  //注册交互事件      
    return;
//---------------------------------------------------------
if (FAILED(hr=pMainDlgroot->FindName(TEXT("DVD"), &btn)))        
   return;
if (FAILED(hr=CreateDelegate(this,&CMainDlgBtnEventHandler::BtnDVD_OnClick,&clickdelegate)))       
   return;
if (FAILED(hr=btn->AddClickEventHandler(clickdelegate)))  //注册交互事件      
    return;
//---------------------------------------------------------
if (FAILED(hr=pMainDlgroot->FindName(TEXT("CDC"), &btn)))        
   return;
if (FAILED(hr=CreateDelegate(this,&CMainDlgBtnEventHandler::BtnCDC_OnClick,&clickdelegate)))       
   return;
if (FAILED(hr=btn->AddClickEventHandler(clickdelegate)))  //注册交互事件      
    return;
//---------------------------------------------------------
if (FAILED(hr=pMainDlgroot->FindName(TEXT("System"), &btn)))        
   return;
if (FAILED(hr=CreateDelegate(this,&CMainDlgBtnEventHandler::BtnSystem_OnClick,&clickdelegate)))       
   return;
if (FAILED(hr=btn->AddClickEventHandler(clickdelegate)))  //注册交互事件      
    return;
//---------------------------------------------------------------
if (FAILED(hr=pMainDlgroot->FindName(TEXT("Close"), &btn)))        
   return;
if (FAILED(hr=CreateDelegate(this,&CMainDlgBtnEventHandler::BtnClose_OnClick,&clickdelegate)))       
   return;
if (FAILED(hr=btn->AddClickEventHandler(clickdelegate)))  //注册交互事件      
    return;
pMainDlgVisualHost->StartDialog(NULL);//显示对话框
clickdelegate->Release();
}
HRESULT CMainDlgBtnEventHandler::BtnRadio_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args)   
{      
        MessageBox(NULL,TEXT("MainDlg_Radio!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
        RETAILMSG(1,(TEXT("++++MainDlgBtnEventHandler( )----the Radio Button is OnClick!/r/n")));
         CRadioDlgBtnEventHandler   Handler;
   Handler.Init(pMainDlgApplication,L"//Storage Card//XAMLPERF//Page_8_Radio.xaml");
     return S_OK;    
}
HRESULT CMainDlgBtnEventHandler::BtnRadio_MouseEnter(IXRDependencyObject* source,XRMouseEventArgs* args)   
{      
       MessageBox(NULL,TEXT("MainDlg_Radio MouseEnter!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
        RETAILMSG(1,(TEXT("++++MainDlgBtnEventHandler( )----the Radio Button is MouseEnter!/r/n")));
     return S_OK;    
}
HRESULT CMainDlgBtnEventHandler::BtnDVD_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args)   
{      
        MessageBox(NULL,TEXT("MainDlg_DVD!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
        RETAILMSG(1,(TEXT("++++MainDlgBtnEventHandler( )----the DVD Button is OnClick!/r/n")));
     return S_OK;    
}
HRESULT CMainDlgBtnEventHandler::BtnCDC_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args)   
{      
        MessageBox(NULL,TEXT("MainDlg_CDC!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
        RETAILMSG(1,(TEXT("++++MainDlgBtnEventHandler( )----the CDC Button is OnClick!/r/n")));
     return S_OK;    
}
HRESULT CMainDlgBtnEventHandler::BtnSystem_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args)   
{      
        MessageBox(NULL,TEXT("MainDlg_System!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
        RETAILMSG(1,(TEXT("++++MainDlgBtnEventHandler( )----the System Button is OnClick!/r/n")));
     return S_OK;    
}
HRESULT CMainDlgBtnEventHandler::BtnClose_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args)   
{      
        MessageBox(NULL,TEXT("MainDlg_Close!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
        RETAILMSG(1,(TEXT("++++MainDlgBtnEventHandler( )----the Close Button is OnClick!/r/n")));
    pMainDlgVisualHost->EndDialog(0); //结束对话框
     return S_OK;       
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//RadioDlg.h
//Designed by xqh
//Date:2009--12--28
#ifndef  _RadioDlg_H_
#define  _RadioDlg_H_
#include <XamlRuntime.h>
#include <XRDelegate.h>
#include <XRPtr.h>
#include <Ehm.h>
class   CRadioDlgBtnEventHandler
{
public:
CRadioDlgBtnEventHandler( );
~CRadioDlgBtnEventHandler( );
void  Init(IXRApplication* pApplication,LPCTSTR strXAMLFilePath);
HRESULT BtnSetting_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args); 
HRESULT BtnClose_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args);  
IXRApplication* pRadioDlgApplication;
IXRVisualHost* pRadioDlgVisualHost;
XRXamlSource  pRadioDlgSourceXaml;
IXRFrameworkElementPtr pRadioDlgroot;
};
#endif
//RadioDlg.cpp
//Designed by xqh
//Date:2009--12--28
#include "RadioDlg.h"
CRadioDlgBtnEventHandler::CRadioDlgBtnEventHandler( )
{
}
CRadioDlgBtnEventHandler::~CRadioDlgBtnEventHandler( )
{
}
void  CRadioDlgBtnEventHandler::Init(IXRApplication* pApplication,LPCTSTR strXAMLFilePath)
{
HRESULT hr;
pRadioDlgApplication=pApplication;
pRadioDlgSourceXaml.SetFile(strXAMLFilePath);
XRWindowCreateParams WindowParameters;
ZeroMemory(&WindowParameters, sizeof(WindowParameters));
WindowParameters.Style       = WS_POPUP;
WindowParameters.pTitle      = L"RadioDlg";
WindowParameters.Left        = 0;
WindowParameters.Top         = 0;
pRadioDlgApplication->CreateHostFromXaml(&pRadioDlgSourceXaml, &WindowParameters, &pRadioDlgVisualHost);
if(!pRadioDlgVisualHost)
 RETAILMSG(1,(TEXT("++++the pRadioDlgVisualHost is NULL!/r/n")));
if (FAILED(hr=pRadioDlgVisualHost->GetRootElement(&pRadioDlgroot)))       
   return;
//----------------------------------------------------------------------------------------------------------
IXRButtonBasePtr  btn;  
IXRDelegate<XRMouseButtonEventArgs>* clickdelegate;
if (FAILED(hr=pRadioDlgroot->FindName(TEXT("Setting"), &btn)))        
   return;
if (FAILED(hr=CreateDelegate(this,&CRadioDlgBtnEventHandler::BtnSetting_OnClick,&clickdelegate)))       
   return;
if (FAILED(hr=btn->AddClickEventHandler(clickdelegate)))  //注册交互事件      
    return;
//---------------------------------------------------------------
if (FAILED(hr=pRadioDlgroot->FindName(TEXT("Close"), &btn)))        
   return;
if (FAILED(hr=CreateDelegate(this,&CRadioDlgBtnEventHandler::BtnClose_OnClick,&clickdelegate)))       
   return;
if (FAILED(hr=btn->AddClickEventHandler(clickdelegate)))  //注册交互事件      
    return;
pRadioDlgVisualHost->StartDialog(NULL);//显示对话框
clickdelegate->Release();
}
HRESULT CRadioDlgBtnEventHandler::BtnSetting_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args)
{
        MessageBox(NULL,TEXT("RadioDlg_Setting!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
        RETAILMSG(1,(TEXT("++++RadioDlgBtnEventHandler( )----the Setting Button is OnClick!/r/n")));
   return S_OK;   
}
HRESULT CRadioDlgBtnEventHandler::BtnClose_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args)
{
       MessageBox(NULL,TEXT("RadioDlg_Close!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
        RETAILMSG(1,(TEXT("++++RadioDlgBtnEventHandler( )----the Close Button is OnClick!/r/n")));
    pRadioDlgVisualHost->EndDialog(0);//结束对话框
    return S_OK;   
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 1.编译一般要链接的库文件有
xamlruntime.lib   uuid.lib
2.编译报错的解决方法
//在XamlRuntime.h文件里面
#include <pwinuser.h> 
