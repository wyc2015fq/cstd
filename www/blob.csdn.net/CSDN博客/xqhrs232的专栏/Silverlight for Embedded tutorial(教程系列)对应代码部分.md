# Silverlight for Embedded tutorial(教程系列)对应代码部分 - xqhrs232的专栏 - CSDN博客
2009年12月22日 14:42:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1294
//Main.cpp
//Designed by  xqh
//Date:2009--12--18
//#include <pwinuser.h> 
#include <XamlRuntime.h>
#include <XRDelegate.h>
#include <XRPtr.h>
#include <Ehm.h>
#include "resource1.h"
IXRApplication* pApplication = NULL;
IXRVisualHost* pVisualHost = NULL;
//############################################################################################################################
class BtnEventHandler //单独响应按钮操作
{
public:
    HRESULT OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args)    
 {      
#if  1
  MessageBox(NULL,TEXT("Click!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
#else
      UINT  exitCode=0;
     // pApplication->StopProcessing(exitCode);
      pVisualHost->EndDialog(0);
#endif
     return S_OK;    
 }
};
//=============================================================================================================
class BtnEventHandlerText  //响应按钮操作并且文本框旋转，按钮的名称属性对应改变---停止与旋转
{
protected:
    IXRButtonBasePtr     btn;    
 IXRStoryboardPtr     sboard;
public:
   // BtnEventHandler(IXRButtonBasePtr& button, IXRStoryboardPtr& storyboard) : btn(button),sboard(storyboard) 
   BtnEventHandlerText(IXRButtonBasePtr& button, IXRStoryboardPtr& storyboard)
 {       
    btn=button;
      sboard=storyboard;
 }
HRESULT OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args)    
{       
    HRESULT        retcode;        XRClockState ckstate;
        if (FAILED(retcode=sboard->GetCurrentState(&ckstate)))            
   return retcode;
        XRValue btnvalue;
        btnvalue.vType=VTYPE_READONLY_STRING; 
        if (ckstate==XRClockState_Stopped)        
  {     
   btnvalue.pReadOnlyStringVal=L"Stop!";   
  if (FAILED(retcode=sboard->Begin()))                
   return retcode;        
  }        
  else        
  {            
   btnvalue.pReadOnlyStringVal=L"Spin!";  
      if (FAILED(retcode=sboard->Stop()))                
   return retcode;        
  }
        if (FAILED(retcode=btn->SetContent(&btnvalue)))            
   return retcode;
        return S_OK;   
}
};
//############################################################################################
INT WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPWSTR lpCmdLine, 
    int nCmdShow
    )
{
   int    exitCode = -1;
   BOOL   bRet;
//##############################################################
   bRet= XamlRuntimeInitialize();
//##############################################################
//IXRApplication* pApplication = NULL;
GetXRApplicationInstance(&pApplication);
//##############################################################
XRXamlSource SourceXaml;
#if  1
//SourceXaml.SetFile(L"//default.xaml");  //2009--12--18  xqh  根目录表示的是错误的！！！
//SourceXaml.SetFile(L"//Page.xaml");
SourceXaml.SetFile(L"//Storage Card//XAMLPERF//Page.xaml");
// Also set the XAML resource if you called AddResourceModule
//SourceXaml.SetResource(hInstance, L"XAML", MAKEINTRESOURCE(500));  //这一句要不得！！！
#else
//2009--12--18  xqh  这样就可以了！！！
SourceXaml.SetResource(hInstance,TEXT("XAML"),MAKEINTRESOURCE(IDR_XAML1));
#endif
//###############################################################
XRWindowCreateParams WindowParameters;
ZeroMemory(&WindowParameters, sizeof(WindowParameters));
WindowParameters.Style       = WS_POPUP;
WindowParameters.pTitle      = L"Title Name";
WindowParameters.Left        = 0;
WindowParameters.Top         = 0;
//################################################################
//IXRVisualHost* pVisualHost = NULL;
pApplication->CreateHostFromXaml(&SourceXaml, &WindowParameters, &pVisualHost);
if(!pVisualHost)
 RETAILMSG(1,(TEXT("++++the pVisualHost is NULL!/r/n")));
//################################################################
HRESULT retcode;
    IXRFrameworkElementPtr root;
    if (FAILED(retcode=pVisualHost->GetRootElement(&root)))       
  return -1;
//=================================================================
#if  0
//找到按钮
IXRButtonBasePtr btn;        
if (FAILED(retcode=root->FindName(TEXT("MyButton"), &btn)))        
return -1;
    BtnEventHandler handler;
    IXRDelegate<XRMouseButtonEventArgs>* clickdelegate;
    if (FAILED(retcode=CreateDelegate(&handler,&BtnEventHandler::OnClick,&clickdelegate)))       
  return -1;
    if (FAILED(retcode=btn->AddClickEventHandler(clickdelegate)))  //注册交互事件      
  return -1;
#else
//找到按钮与文本框
IXRButtonBasePtr btn;        
if (FAILED(retcode=root->FindName(TEXT("MyButton"), &btn)))        
return -1;
IXRStoryboardPtr sboard;
    if (FAILED(retcode=root->FindName(TEXT("Storyboard1"), &sboard)))        
  return -1;
   // BtnEventHandlerText handler;
    BtnEventHandlerText  handler(btn,sboard);
    IXRDelegate<XRMouseButtonEventArgs>* clickdelegate;
    if (FAILED(retcode=CreateDelegate(&handler,&BtnEventHandlerText::OnClick,&clickdelegate)))       
  return -1;
    if (FAILED(retcode=btn->AddClickEventHandler(clickdelegate)))    //注册交互事件    
  return -1;
#endif
//===============================================================
pVisualHost->StartDialog(NULL);//显示对话框
//###############################################################
UINT StartProcessCode = 0;
pApplication->StartProcessing(&StartProcessCode);
//##################################################################
clickdelegate->Release();
    return exitCode;
}
