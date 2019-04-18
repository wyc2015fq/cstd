# vc遍历网页表单并自动填写提交 - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年12月16日 17:23:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：8105








1.独立代码

//-----------开始---------------------//
#include <atlbase.h>
#include <Mshtml.h>
#include <winuser.h>
#include <comdef.h>
#include <string.h>
void EnumIE(void);//处理网页
void EnumFrame(IHTMLDocument2 * pIHTMLDocument2);//处理框架
void EnumForm(IHTMLDocument2 * pIHTMLDocument2);//处理表单
CComModule _Module;  //使用CComDispatchDriver ATL的智能指针，此处必须声明
#include <atlcom.h>
void EnumField(CComDispatchDriver spInputElement,CString ComType,CString ComVal,CString ComName);//处理表单域

void EnumIE(void)   
{   
  CComPtr<IShellWindows> spShellWin;   
  HRESULT hr=spShellWin.CoCreateInstance(CLSID_ShellWindows);   
  if (FAILED(hr))   
  {   
  return;   
  }       

  long nCount=0;    //取得浏览器实例个数(Explorer和IExplorer)   
  spShellWin->get_Count(&nCount);   
  if (0==nCount)   
  {   
    return;   
  } 

  for(int i=0; i<nCount; i++)   
  {   
    CComPtr<IDispatch> spDispIE;   
    hr=spShellWin->Item(CComVariant((long)i), &spDispIE);   
    if (FAILED(hr)) continue; 

    CComQIPtr<IWebBrowser2>spBrowser=spDispIE;   
    if (!spBrowser) continue; 

    CComPtr<IDispatch> spDispDoc;   
    hr=spBrowser->get_Document(&spDispDoc);   
    if (FAILED(hr)) continue; 

    CComQIPtr<IHTMLDocument2>spDocument2 =spDispDoc;   
    if (!spDocument2) continue;       

 //Modify by jncao 2007-09-17
 //*******************************************************************************
 CString cIEUrl_Filter;  //设置的URL(必须是此URL的网站才有效);
    cIEUrl_Filter="[http://127.0.0.1/SmtCCS_manage/](http://127.0.0.1/SmtCCS_manage/)"; //设置过滤的网址
    //*******************************************************************************

    CComBSTR IEUrl;
 spBrowser->get_LocationURL(&IEUrl);    
 CString cIEUrl_Get;     //从机器上取得的HTTP的完整的URL;
 cIEUrl_Get=IEUrl;
 cIEUrl_Get=cIEUrl_Get.Left(cIEUrl_Filter.GetLength()); //截取前面N位

 if (strcmp(cIEUrl_Get,cIEUrl_Filter)==0)
 {
     // 程序运行到此，已经找到了IHTMLDocument2的接口指针       
        EnumForm(spDocument2); //枚举所有的表单         
 }    

  }   
} 

void EnumFrame(IHTMLDocument2 * pIHTMLDocument2)
{   
 if (!pIHTMLDocument2) return;       
 HRESULT   hr;   

 CComPtr<IHTMLFramesCollection2> spFramesCollection2;   
 pIHTMLDocument2->get_frames(&spFramesCollection2); //取得框架frame的集合   

 long nFrameCount=0;        //取得子框架个数   
 hr=spFramesCollection2->get_length(&nFrameCount);   
 if (FAILED(hr)|| 0==nFrameCount) return;   

 for(long i=0; i<nFrameCount; i++)   
 {   
  CComVariant vDispWin2; //取得子框架的自动化接口   
  hr = spFramesCollection2->item(&CComVariant(i), &vDispWin2);   
  if (FAILED(hr)) continue;       
  CComQIPtr<IHTMLWindow2>spWin2 = vDispWin2.pdispVal;   
  if (!spWin2) continue; //取得子框架的   IHTMLWindow2   接口       
  CComPtr <IHTMLDocument2> spDoc2;   
  spWin2->get_document(&spDoc2); //取得子框架的   IHTMLDocument2   接口

  EnumForm(spDoc2);      //递归枚举当前子框架   IHTMLDocument2   上的表单form   
 }   
} 

void EnumForm(IHTMLDocument2 * pIHTMLDocument2)   
{   
  if (!pIHTMLDocument2) return; 

  EnumFrame(pIHTMLDocument2);   //递归枚举当前IHTMLDocument2上的子框架frame   

  HRESULT hr;

  USES_CONVERSION;       

  CComQIPtr<IHTMLElementCollection> spElementCollection;   
  hr=pIHTMLDocument2->get_forms(&spElementCollection); //取得表单集合   
  if (FAILED(hr))   
  {   
    return;   
  }   

  long nFormCount=0;           //取得表单数目   
  hr=spElementCollection->get_length(&nFormCount);   
  if (FAILED(hr))   
  {   
    return;   
  }   

  for(long i=0; i<nFormCount; i++)   
  {   
    IDispatch *pDisp = NULL;   //取得第i项表单   
    hr=spElementCollection->item(CComVariant(i),CComVariant(),&pDisp);   
    if (FAILED(hr)) continue;   

    CComQIPtr<IHTMLFormElement> spFormElement= pDisp;   
    pDisp->Release();   

    long nElemCount=0;         //取得表单中域的数目   
    hr=spFormElement->get_length(&nElemCount);   
    if (FAILED(hr)) continue;   

    for(long j=0; j<nElemCount; j++)   
 {   

      CComDispatchDriver spInputElement; //取得第j项表单域   
      hr=spFormElement->item(CComVariant(j), CComVariant(), &spInputElement);   
      if (FAILED(hr)) continue;   

      CComVariant vName,vVal,vType;     //取得表单域的名称，数值，类型 
      hr=spInputElement.GetPropertyByName(L"name", &vName);   
      if (FAILED(hr)) continue;   
      hr=spInputElement.GetPropertyByName(L"value", &vVal);   
      if(FAILED(hr)) continue;   
      hr=spInputElement.GetPropertyByName(L"type", &vType);   
      if(FAILED(hr)) continue;   

      LPCTSTR lpName= vName.bstrVal ? OLE2CT(vName.bstrVal) : _T("NULL"); //未知域名   
      LPCTSTR lpVal=  vVal.bstrVal  ? OLE2CT(vVal.bstrVal)  : _T("NULL"); //空值，未输入   
      LPCTSTR lpType= vType.bstrVal ? OLE2CT(vType.bstrVal) : _T("NULL"); //未知类型  

   EnumField(spInputElement,lpType,lpVal,lpName);//传递并处理表单域的类型、值、名
 }//表单域循环结束     
  }//表单循环结束       
}   

void EnumField(CComDispatchDriver spInputElement,CString ComType,CString ComVal,CString ComName)
{//处理表单域
 if ((ComType.Find("text")>=0) && strcmp(ComVal,"NULL")==0 && strcmp(ComName,"userName")==0)
   { 
     CString Tmp="123456";
     CComVariant vSetStatus(Tmp);
  spInputElement.PutPropertyByName(L"value",&vSetStatus);
   }
   if ((ComType.Find("password")>=0) && strcmp(ComVal,"NULL")==0 && strcmp(ComName,"password")==0)
   { 
     CString Tmp="123456";
     CComVariant vSetStatus(Tmp);
  spInputElement.PutPropertyByName(L"value",&vSetStatus);
   }
   if ((ComType.Find("submit")>=0))
   {
  IHTMLElement*  pHElement;
  spInputElement->QueryInterface(IID_IHTMLElement,(void **)&pHElement);
  pHElement->click();                
   }
}
//--------------------结束--------------------------------------// 

2.执行：

void CDemoDlg::OnOK() 
{
 // TODO: Add extra validation here
  ::CoInitialize(NULL); //初始化COM
     EnumIE();             //枚举浏览器       
     ::CoUninitialize();   //释放COM
 //CDialog::OnOK();
}



