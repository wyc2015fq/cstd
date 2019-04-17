# vc采集网页内frame框架下所有元素（不指定具体table/form）-升级版 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年04月11日 15:56:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3007








升级版说明：增加对获取指定控件的判断，利用标志bGetCaller，减少循环和递归，基于效能提升门户生产地址获取主叫，可从60s压缩到6s

1.独立代码

#include <atlbase.h>
#include <mshtml.h>
#include <winuser.h>
#include <comdef.h>
#include <string.h>
void EnumIE(void);//处理网页
void EnumFrame(IHTMLDocument2 * pIHTMLDocument2);//处理框架
CComModule _Module;  //使用CComDispatchDriver ATL的智能指针，此处必须声明
#include <atlcom.h>
void EnumAllElement(IHTMLDocument2 * pIHTMLDocument2);//获取网页内元素
CString     glb_strCaller;//全局主叫
BOOL        glb_bGetCaller;//是否获取到主叫

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
   glb_bGetCaller=false;//恢复主叫未获取标志
   EnumAllElement(spDocument2);//枚举所有字段
  }    
  }   
} 

//在框架内获取主叫
void EnumAllElement(IHTMLDocument2 * pIHTMLDocument2) //枚举所有字段
{
 if (!pIHTMLDocument2) return;  

 EnumFrame(pIHTMLDocument2);   //递归枚举当前IHTMLDocument2上的子框架frame  

 if (glb_bGetCaller) return;//已获取主叫

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
   BSTR bTemp;
   pElement->get_id(&bTemp);//可以获取其他特征，根据具体元素而定
   CString strTemp=bTemp;
   if(!strTemp.IsEmpty() && strTemp=="callNo")//根据id是主叫号码获取值或作其他处理
   {
    IHTMLInputTextElement* input;
    pDisp->QueryInterface(IID_IHTMLInputTextElement,(void**)&input);
    input->get_value(&bTemp);
    if(bTemp==NULL) glb_strCaller="";
    else glb_strCaller=bTemp;
    glb_bGetCaller=true;//找到主叫
    break;
   }
  }
 }
}

//循环页面框架
void EnumFrame(IHTMLDocument2 * pIHTMLDocument2)
{   
 if (!pIHTMLDocument2) return;      
 if (glb_bGetCaller) return;//已获取主叫

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

  EnumAllElement(spDoc2);      //递归枚举当前子框架   IHTMLDocument2   上的所有控件  
 }   
} 



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



