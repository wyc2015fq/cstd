# WINCE6.0下用SilverLight技术的基本控件使用 - xqhrs232的专栏 - CSDN博客
2009年12月31日 15:48:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2502标签：[silverlight																[wince																[textbox																[null																[windows																[winapi](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=textbox&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)](https://so.csdn.net/so/search/s.do?q=silverlight&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
//###############################################################################################################
//Main.cpp
//Designed by xqh
//Date:2009--12--28
//#include <pwinuser.h> 
#include <XamlRuntime.h>
#include <XRDelegate.h>
#include <XRPtr.h>
#include <Ehm.h>
//=================================================================================================================
IXRApplication* pApplication = NULL;
IXRVisualHost* pVisualHost = NULL;
class BtnEventHandler //单独响应按钮操作
{
public:
//Button使用示范
    HRESULT Btn_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args)   
 {      
#if  1
 // MessageBox(NULL,TEXT("Btn_OnClick!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
        RETAILMSG(1,(TEXT("++++BtnEventHandler( )----Btn_OnClick!/r/n")));
#else
      UINT  exitCode=0;
     // pApplication->StopProcessing(exitCode);
      pVisualHost->EndDialog(0);
#endif
     return S_OK;    
 }
//ComboBox使用示范
HRESULT ComboBox_SelChange(IXRDependencyObject* source,XRSelectionChangedEventArgs* args)   
 {      
 //MessageBox(NULL,TEXT("ComboBox_SelChange!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
      RETAILMSG(1,(TEXT("++++BtnEventHandler( )----ComboBox_SelChange!/r/n")));
     return S_OK;    
 }
//ListBox使用示范
HRESULT ListBox_SelChange(IXRDependencyObject* source,XRSelectionChangedEventArgs* args)   
 {      
 //MessageBox(NULL,TEXT("ListBox_SelChange!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
      RETAILMSG(1,(TEXT("++++BtnEventHandler( )----ListBox_SelChange!/r/n")));
     return S_OK;    
 }
//CheckBox使用示范
 HRESULT CheckBox_Check(IXRDependencyObject* source,XRRoutedEventArgs* args)   
 {      
 MessageBox(NULL,TEXT("CheckBox_Check!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
     return S_OK;    
 }
//RadioButton使用示范
HRESULT RadioBtn_Check(IXRDependencyObject* source,XRRoutedEventArgs* args)   
 {      
 MessageBox(NULL,TEXT("RadioBtn_Check!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
     return S_OK;    
 }
//ProgressBar使用示范
/*
HRESULT PrgBar_Change(IXRDependencyObject* source,XRValueChangedEventArgs* args)   
 {      
 MessageBox(NULL,TEXT("PrgBar_Change!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
     return S_OK;    
 }
*/
//TextBox使用示范
HRESULT TextBox_SelChange(IXRDependencyObject* source,XRSelectionChangedEventArgs* args)   
 {      
 //MessageBox(NULL,TEXT("TextBox_SelChange!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
      RETAILMSG(1,(TEXT("++++BtnEventHandler( )----TextBox_SelChange!/r/n")));
     return S_OK;    
 }
HRESULT TextBox_TextChange(IXRDependencyObject* source,XRTextChangedEventArgs* args)   
 {      
 //MessageBox(NULL,TEXT("TextBox_TextChange!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
      RETAILMSG(1,(TEXT("++++BtnEventHandler( )----TextBox_TextChange!/r/n")));
     return S_OK;    
 }
//Slider使用示范
/*
HRESULT Slider_Change(IXRDependencyObject* source,XRValueChangedEventArgs* args)   
 {      
 MessageBox(NULL,TEXT("Slider_Change!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
     return S_OK;    
 }
*/
//ScrollBar使用示范
/*
HRESULT ScrollBarVr_Change(IXRDependencyObject* source,XRValueChangedEventArgs* args)   
 {      
 MessageBox(NULL,TEXT("ScrollBarVr_Change!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
     return S_OK;    
 }
*/
/*
HRESULT ScrollBarHr_Change(IXRDependencyObject* source,XRValueChangedEventArgs* args)   
 {      
 MessageBox(NULL,TEXT("ScrollBarHr_Change!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
     return S_OK;    
 }
*/
//PasswordBox使用示范
HRESULT Password_Change(IXRDependencyObject* source,XRRoutedEventArgs* args)   
 {      
 //MessageBox(NULL,TEXT("Password_Change!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
      RETAILMSG(1,(TEXT("++++BtnEventHandler( )----Password_Change!/r/n")));
     return S_OK;    
 }
};
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
   HRESULT hr;
   bRet= XamlRuntimeInitialize();
   GetXRApplicationInstance(&pApplication);
   XRXamlSource SourceXaml;
   SourceXaml.SetFile(L"//Storage Card//XAMLPERF//Page_Control.xaml");
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
//找到根目录
IXRFrameworkElementPtr root;
if (FAILED(hr=pVisualHost->GetRootElement(&root)))       
return -1;
//--------------------------------------------------------------
IXRStylusInfoPtr    StylusInfo;
pApplication->CreateObject(IID_IXRStylusInfo,&StylusInfo);
bool  bStylusStatus;
StylusInfo->GetIsInverted(&bStylusStatus);
if(bStylusStatus)
   RETAILMSG(1,(TEXT("++++the bStylusStatus is true!/r/n")));
else
   RETAILMSG(1,(TEXT("++++the bStylusStatus is false!/r/n")));
StylusInfo->SetIsInverted(true);
StylusInfo->GetIsInverted(&bStylusStatus);
if(bStylusStatus)
   RETAILMSG(1,(TEXT("++++the bStylusStatus is true!/r/n")));
else
   RETAILMSG(1,(TEXT("++++the bStylusStatus is false!/r/n")));
//################################################################
//Button使用示范
IXRButtonBasePtr  btn;  
if (FAILED(hr=root->FindName(TEXT("DVD"), &btn)))        
return -1;
BtnEventHandler handlerBtn;
IXRDelegate<XRMouseButtonEventArgs>* clickdelegateBtn;
if (FAILED(hr=CreateDelegate(&handlerBtn,&BtnEventHandler::Btn_OnClick,&clickdelegateBtn)))       
  return -1;
if (FAILED(hr=btn->AddClickEventHandler(clickdelegateBtn)))  //注册交互事件      
  return -1;
//##########################################################################################################333
//ComboBox使用示范
IXRComboBoxPtr  ComBox;
//IXRComboBoxItem
IXRComboBoxItemPtr   ComBoxItem=NULL,ComBoxItemT=NULL;
XRValue           XValueComBox,XValueComBoxT;
if (FAILED(hr=root->FindName(TEXT("ComBox"), &ComBox)))        
return -1;
//XValueComBox.vType=VTYPE_BSTR;
//XValueComBox.bstrStringVal=L"熊清华";
XValueComBox.vType=VTYPE_READONLY_STRING;
XValueComBox.pReadOnlyStringVal=L"XQH";
if(ComBoxItem==NULL)
RETAILMSG(1,(TEXT("++++the ComBoxItem is NULL!/r/n")));//2009--12--29  XQH  是一个空指针！！！
pApplication->CreateObject(IID_IXRComboBoxItem,&ComBoxItem);
hr=ComBoxItem->SetContent(&XValueComBox);
DWORD   dwError;
dwError=GetLastError( );
int *pIndex=NULL;
IXRItemCollection* ppItems;
ComBox->GetItems(&ppItems);
//ppItems->Add((IXRDependencyObject*)ComBoxItem,pIndex);
ppItems->Insert(0,(IXRDependencyObject*)ComBoxItem);
//------------------------------------------------------------
pApplication->CreateObject(IID_IXRComboBoxItem,&ComBoxItemT);
//XValueComBoxT.SetNull();
XValueComBoxT.vType=VTYPE_READONLY_STRING;
XValueComBoxT.pReadOnlyStringVal=L"WXY";
hr=ComBoxItemT->SetContent(&XValueComBoxT);
//ppItems->Add((IXRDependencyObject*)ComBoxItem,pIndex);
ppItems->Insert(1,(IXRDependencyObject*)ComBoxItemT);
//=================================================================
ComBox->SetSelectedIndex(0);
//ComBox->SetSelectedItem(ComBoxItem);
BtnEventHandler handlerComboBox;
IXRDelegate<XRSelectionChangedEventArgs>* clickdelegateCombox;
if (FAILED(hr=CreateDelegate(&handlerComboBox,&BtnEventHandler::ComboBox_SelChange,&clickdelegateCombox)))       
  return -1;
//if (FAILED(hr=ComBox->AddSelectionChangedHandler(clickdelegateCombox)))  //注册交互事件     
//  return -1;
ComBox->AddSelectionChangedEventHandler(clickdelegateCombox);
//##########################################################################################################333
//ListBox使用示范
IXRListBoxPtr  ListBox;
IXRListBoxItemPtr   ListBoxItem=NULL,ListBoxItemT=NULL;
XRValue           XValueListBox,XValueListBoxT;
if (FAILED(hr=root->FindName(TEXT("ListBox"), &ListBox)))        
return -1;
XValueListBox.vType=VTYPE_READONLY_STRING;
XValueListBox.pReadOnlyStringVal=L"XQH";
if(ListBoxItem==NULL)
RETAILMSG(1,(TEXT("++++the ListBoxItem is NULL!/r/n")));//2009--12--29  XQH  是一个空指针！！！
pApplication->CreateObject(IID_IXRListBoxItem,&ListBoxItem);
hr=ListBoxItem->SetContent(&XValueListBox);
int *pIndexList=NULL;
IXRItemCollection* ppItemsList;
ListBox->GetItems(&ppItemsList);
//ppItemsList->Add((IXRDependencyObject*)ListBoxItem,pIndexList);
ppItemsList->Insert(0,(IXRDependencyObject*)ListBoxItem);
//------------------------------------------------------------
pApplication->CreateObject(IID_IXRListBoxItem,&ListBoxItemT);
//XValueListBoxT.SetNull();
XValueListBoxT.vType=VTYPE_READONLY_STRING;
XValueListBoxT.pReadOnlyStringVal=L"WXY";
hr=ListBoxItemT->SetContent(&XValueListBoxT);
//ppItemsList->Add((IXRDependencyObject*)ListBoxItemT,pIndexList);
ppItemsList->Insert(1,(IXRDependencyObject*)ListBoxItemT);
//=================================================================
ListBox->SetSelectedIndex(0);
//ListBox->SetSelectedItem(ListBoxItem);
BtnEventHandler handlerListBox;
IXRDelegate<XRSelectionChangedEventArgs>* clickdelegateListbox;
if (FAILED(hr=CreateDelegate(&handlerListBox,&BtnEventHandler::ListBox_SelChange,&clickdelegateListbox)))       
  return -1;
//if (FAILED(hr=ListBox->AddSelectionChangedHandler(clickdelegateListbox)))  //注册交互事件     
 // return -1;
ListBox->AddSelectionChangedEventHandler(clickdelegateListbox);
//##########################################################################################################333
//CheckBox使用示范
IXRCheckBoxPtr  CheckBox;
if (FAILED(hr=root->FindName(TEXT("CheckBox"), &CheckBox)))        
return -1;
XRThreeState   status;
CheckBox->GetIsChecked(&status);
BtnEventHandler handlerCheckBox;
IXRDelegate<XRRoutedEventArgs>* clickdelegateCheckBox;
if (FAILED(hr=CreateDelegate(&handlerCheckBox,&BtnEventHandler::CheckBox_Check,&clickdelegateCheckBox)))       
  return -1;
if (FAILED(hr=CheckBox->AddCheckedEventHandler(clickdelegateCheckBox)))  //注册交互事件     
  return -1;
//##########################################################################################################333
//RadioButton使用示范
IXRRadioButtonPtr  RadioBtn;
if (FAILED(hr=root->FindName(TEXT("RadioBtn1"), &RadioBtn)))        
return -1;
XRThreeState   statusRadioBtn;
RadioBtn->GetIsChecked(&statusRadioBtn);
BtnEventHandler handlerRadioBtn;
IXRDelegate<XRRoutedEventArgs>* clickdelegateRadioBtn;
if (FAILED(hr=CreateDelegate(&handlerRadioBtn,&BtnEventHandler::RadioBtn_Check,&clickdelegateRadioBtn)))       
  return -1;
if (FAILED(hr=RadioBtn->AddCheckedEventHandler(clickdelegateRadioBtn)))  //注册交互事件     
  return -1;
//##########################################################################################################333
//ProgressBar进度条使用示范
IXRProgressBarPtr  PrgBar;
if (FAILED(hr=root->FindName(TEXT("PrgBar"), &PrgBar)))        
return -1;
PrgBar->SetMinimum(0);
PrgBar->SetMaximum(100);
PrgBar->SetValue(50);
/*
BtnEventHandler handlerPrgBar;
IXRDelegate<XRValueChangedEventArgs>* clickdelegatePrgBar;
if (FAILED(hr=CreateDelegate(&handlerPrgBar,&BtnEventHandler::PrgBar_Check,&clickdelegatePrgBar)))       
  return -1;
if (FAILED(hr=PrgBar->AddMaximumChangedEventHandler(clickdelegatePrgBar)))  //注册交互事件     
  return -1;
*/
//##########################################################################################################333
//TextBox使用示范
IXRTextBoxPtr  TextBox;
if (FAILED(hr=root->FindName(TEXT("TextBox"), &TextBox)))        
return -1;
TextBox->SetText(L"XQH");
BtnEventHandler handlerTextBox;
IXRDelegate<XRTextChangedEventArgs>* clickdelegateTextBox;
if (FAILED(hr=CreateDelegate(&handlerTextBox,&BtnEventHandler::TextBox_TextChange,&clickdelegateTextBox)))       
  return -1;
if (FAILED(hr=TextBox->AddTextChangedEventHandler(clickdelegateTextBox)))  //注册交互事件     
  return -1;
//##########################################################################################################333
//Slider使用示范
IXRSliderPtr  Slider;
if (FAILED(hr=root->FindName(TEXT("Slider"), &Slider)))        
return -1;
Slider->SetMinimum(0);
Slider->SetMaximum(100);
Slider->SetValue(50);
/*
BtnEventHandler handlerSlider;
IXRDelegate<XRValueChangedEventArgs>* clickdelegateSlider;
if (FAILED(hr=CreateDelegate(&handlerSlider,&BtnEventHandler::Slider_Change,&clickdelegateSlider)))       
  return -1;
if (FAILED(hr=Slider->AddValueChangedEventHandler(clickdelegateSlider)))  //注册交互事件     
  return -1;
*/
//##########################################################################################################333
//ScrollBar使用示范
//垂直ScrollBar
IXRScrollBarPtr  ScrollBarVr;
if (FAILED(hr=root->FindName(TEXT("ScrollBar_Vr"), &ScrollBarVr)))        
return -1;
ScrollBarVr->SetMinimum(0);
ScrollBarVr->SetMaximum(100);
ScrollBarVr->SetValue(50);
/*
BtnEventHandler handlerScrollBarVr;
IXRDelegate<XRValueChangedEventArgs>* clickdelegateScrollBarVr;
if (FAILED(hr=CreateDelegate(&handlerScrollBarVr,&BtnEventHandler::ScrollBarVr_Change,&clickdelegateScrollBarVr)))       
  return -1;
if (FAILED(hr=ScrollBarVr->AddValueChangedEventHandler(clickdelegateScrollBarVr)))  //注册交互事件     
  return -1;
*/
//----------------------------------------------------------------
//水平ScrollBar
IXRScrollBarPtr  ScrollBarHr;
if (FAILED(hr=root->FindName(TEXT("ScrollBar_Hr"), &ScrollBarHr)))        
return -1;
ScrollBarHr->SetMinimum(0);
ScrollBarHr->SetMaximum(100);
ScrollBarHr->SetValue(50);
/*
BtnEventHandler handlerScrollBarHr;
IXRDelegate<XRValueChangedEventArgs>* clickdelegateScrollBarHr;
if (FAILED(hr=CreateDelegate(&handlerScrollBarHr,&BtnEventHandler::ScrollBarHr_Change,&clickdelegateScrollBarHr)))       
  return -1;
if (FAILED(hr=ScrollBarHr->AddValueChangedEventHandler(clickdelegateScrollBarHr)))  //注册交互事件     
  return -1;
*/
//##########################################################################################################333
//PasswordBox使用示范
IXRPasswordBoxPtr  PassBox;
if (FAILED(hr=root->FindName(TEXT("PassBox"), &PassBox)))        
return -1;
PassBox->SetPassword(L"xqh");
BtnEventHandler handlerPassBox;
IXRDelegate<XRRoutedEventArgs>* clickdelegatePassBox;
if (FAILED(hr=CreateDelegate(&handlerPassBox,&BtnEventHandler::Password_Change,&clickdelegatePassBox)))       
  return -1;
if (FAILED(hr=PassBox->AddPasswordChangedEventHandler(clickdelegatePassBox)))  //注册交互事件     
  return -1;
//###############################################################
pVisualHost->StartDialog(NULL);//显示对话框
UINT StartProcessCode = 0;
pApplication->StartProcessing(&StartProcessCode);
//##################################################################
clickdelegateBtn->Release();
clickdelegateCombox->Release( );
clickdelegateListbox->Release( );
clickdelegateCheckBox->Release( );
clickdelegateRadioBtn->Release( );
//clickdelegatePrgBar->Release( );
clickdelegateTextBox->Release( );
//-----------------------------------
//clickdelegateSlider->Release( );
//clickdelegateScrollBarVr->Release( );
//clickdelegateScrollBarHr->Release( );
//------------------------------------
clickdelegatePassBox->Release( );
   return exitCode;
}
//=============================================================================
//对应的Page_Control.xaml内容
<UserControl
 xmlns="[http://schemas.microsoft.com/winfx/2006/xaml/presentation](http://schemas.microsoft.com/winfx/2006/xaml/presentation)"
 xmlns:x="[http://schemas.microsoft.com/winfx/2006/xaml](http://schemas.microsoft.com/winfx/2006/xaml)"
 x:Class="SilverlightApplication1.Page"
 Width="640" Height="480">
 <Grid x:Name="LayoutRoot" Background="#FF0000FF">
  <ListBox HorizontalAlignment="Left" Margin="120,166,0,154" Width="160" x:Name="ListBox" RenderTransformOrigin="0.5,0.25"/>
  <CheckBox Height="40" HorizontalAlignment="Right" Margin="0,39,80,0" VerticalAlignment="Top" Width="160" Content="CheckBox" x:Name="CheckBox"/>
  <RadioButton Height="40" HorizontalAlignment="Right" Margin="0,81,80,0" VerticalAlignment="Top" Width="160" Content="Com1" x:Name="RadioBtn1"/>
  <ProgressBar HorizontalAlignment="Right" Margin="0,158,40,0" Width="280" x:Name="PrgBar" VerticalAlignment="Top" Height="40" Background="#FF00FF00" Foreground="#FFFF0000"/>
  <RadioButton HorizontalAlignment="Right" Margin="0,119,80,0" Width="160" Content="Com2" x:Name="RadioBtn2" VerticalAlignment="Top" Height="40"/>
  <Button Height="32" HorizontalAlignment="Left" Margin="80,8,0,0" VerticalAlignment="Top" Width="160" Content="DVD" x:Name="DVD"/>
  <ComboBox Height="40" HorizontalAlignment="Left" Margin="80,80,0,0" VerticalAlignment="Top" Width="160" x:Name="ComBox"/>
  <TextBox Height="40" HorizontalAlignment="Right" Margin="0,0,200,200" VerticalAlignment="Bottom" Width="120" Text="" TextWrapping="Wrap" x:Name="TextBox" RenderTransformOrigin="0.1,0.4"/>
  <Slider Height="40" HorizontalAlignment="Right" Margin="0,0,80,100" VerticalAlignment="Bottom" Width="240" x:Name="Slider"/>
  <ScrollBar Height="40" HorizontalAlignment="Left" Margin="80,0,0,80" VerticalAlignment="Bottom" Width="200" x:Name="ScrollBar_Hr" Orientation="Horizontal"/>
  <PasswordBox Height="40" HorizontalAlignment="Right" Margin="0,0,160,40" VerticalAlignment="Bottom" Width="160" x:Name="PassBox"/>
  <TextBlock Height="40" HorizontalAlignment="Right" Margin="0,0,40,200" VerticalAlignment="Bottom" Width="120" Text="TextBlock" TextWrapping="Wrap" x:Name="TextBlock"/>
  <ScrollBar HorizontalAlignment="Left" Margin="40,160,0,160" Width="40" x:Name="ScrollBar_Vr"/>
 </Grid>
</UserControl>
