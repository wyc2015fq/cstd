# vc采集网页内指定frame框架下所有元素-再升级版 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年04月14日 08:59:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4425








再升级版说明：通过frame的get_location属性，指定frame来获取其元素，减少递归和循环，减少循环和递归，基于效能提升门户生产地址获取主叫，可从6s压缩到1s，耗时在于指定frame所有元素循环上。

1.独立代码

//--------------获取效能提升门户主叫---------------------------------------//
#include <atlbase.h>
#include <mshtml.h>
#include <winuser.h>
#include <comdef.h>
#include <string.h>
void EnumIE(void);//处理网页
void EnummiddleFrame(IHTMLDocument2 * pIHTMLDocument2);//处理框架
void EnumleftFrame(IHTMLDocument2 * pIHTMLDocument2);//处理框架
CComModule _Module;  //使用CComDispatchDriver ATL的智能指针，此处必须声明
#include <atlcom.h>
void EnumAllElement(IHTMLDocument2 * pIHTMLDocument2);//获取网页内元素
CString     glb_strCaller;//全局主叫

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

  //Modify by Fang jiansheng 2011-04-02
  //*******************************************************************************
  CString cIEUrl_Filter;  //设置的URL(必须是此URL的网站才有效);
  cIEUrl_Filter="[http://172.20.33.130:8082/csp/](http://172.20.33.130:8082/csp/)"; //效能提升门户过滤的网址    
  //*******************************************************************************

  CComBSTR IEUrl;
  spBrowser->get_LocationURL(&IEUrl); 
  CString cIEUrl_Get;     //从机器上取得的HTTP的完整的URL;
  cIEUrl_Get=IEUrl;
  cIEUrl_Get=cIEUrl_Get.Left(cIEUrl_Filter.GetLength()); //截取前面N位

  if (strcmp(cIEUrl_Get,cIEUrl_Filter)==0)
  {
   // 程序运行到此，已经找到了IHTMLDocument2的接口指针 
   EnummiddleFrame(spDocument2);
  }    
  }   
} 

//在框架内获取主叫
void EnumAllElement(IHTMLDocument2 * pIHTMLDocument2) //枚举所有字段
{
 if (!pIHTMLDocument2) return;    
 HRESULT   hr;   
 CComQIPtr<IHTMLElementCollection> spAllElement; 
 hr=pIHTMLDocument2->get_all(&spAllElement);//获取所有网页内所有元素
 if (FAILED(hr))  return;   

 long nLength = 0;
 spAllElement->get_length (&nLength);
 for (int i = 0; i < nLength; i++)
 {
        CComPtr<IDispatch> pDisp;
  hr = spAllElement->item(COleVariant((long)i),COleVariant((long)0),&pDisp); //获取单个元素
  if(SUCCEEDED(hr)) 
  {
   //CComQIPtr <IHTMLElement, &IID_IHTMLElement> pElement(pDisp);
   CComQIPtr<IHTMLElement, &IID_IHTMLElement> pElement;
   pDisp->QueryInterface(&pElement);
   BSTR bId;
   pElement->get_id(&bId);//可以获取其他特征，根据具体元素而定
   CString strId=bId;
   if(!strId.IsEmpty() && strId=="callNo")//根据id是主叫号码获取值或作其他处理
   {
    IHTMLInputTextElement* input;
    pDisp->QueryInterface(IID_IHTMLInputTextElement,(void**)&input);
    BSTR bVal;
    input->get_value(&bVal);
    if(bVal==NULL) glb_strCaller="";
    else glb_strCaller=bVal;
    break;
   }
  }
 }
}

void EnumleftFrame(IHTMLDocument2 * pIHTMLDocument2)
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
  //CComQIPtr<IHTMLFrameElement, &IID_IHTMLFrameElement> pFrmElement=vDispWin2.pdispVal;
  if (!spWin2) continue; //取得子框架的   IHTMLWindow2   接口
  CComPtr <IHTMLLocation> spLoc;
  spWin2->get_location(&spLoc);//获取frame的页面地址
  BSTR bHref;
  spLoc->get_href(&bHref);//获取链接地址
        CString strHref=bHref;
  if(!strHref.IsEmpty() && strHref=="[http://172.20.33.130:8082/csp/bsm/leftFrame.action](http://172.20.33.130:8082/csp/bsm/leftFrame.action)")
  { 
   //效能提升门户中间框架leftFrame的页面地址 
   CComPtr <IHTMLDocument2> spDoc2;   
   spWin2->get_document(&spDoc2); //取得子框架的   IHTMLDocument2   接口
   EnumAllElement(spDoc2);//获取效能提升门户主叫
   break;
  }  
 }   
}

void EnummiddleFrame(IHTMLDocument2 * pIHTMLDocument2)
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
  //CComQIPtr<IHTMLFrameElement, &IID_IHTMLFrameElement> pFrmElement=vDispWin2.pdispVal;
  if (!spWin2) continue; //取得子框架的   IHTMLWindow2   接口
  CComPtr <IHTMLLocation> spLoc;
  spWin2->get_location(&spLoc);//获取frame的页面地址
  BSTR bHref;
  spLoc->get_href(&bHref);//获取链接地址
        CString strHref=bHref;
  if(!strHref.IsEmpty() && strHref=="[http://172.20.33.130:8082/csp/mif/middleFrame.action](http://172.20.33.130:8082/csp/mif/middleFrame.action)")
  { 
   //效能提升门户中间框架middleFrame的页面地址
   CComPtr <IHTMLDocument2> spDoc2;   
   spWin2->get_document(&spDoc2); //取得子框架的   IHTMLDocument2   接口
   EnumleftFrame(spDoc2);//获取效能提升门户左边框架leftFrame
   break;
  }  
 }   
}
////////////////////////////////////////////////////////////////////////////////////////////////

//-----------结束---------------------//

2.执行代码:

void CDemoDlg::OnOK() 
{
 // TODO: Add extra validation here
 ::CoInitialize(NULL); //初始化COM
     EnumIE();             //枚举浏览器       
     ::CoUninitialize();   //释放COM
 //CDialog::OnOK();
}



