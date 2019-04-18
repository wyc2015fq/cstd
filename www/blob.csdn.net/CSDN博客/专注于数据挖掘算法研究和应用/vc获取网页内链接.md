# vc获取网页内链接 - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年12月22日 09:41:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4155








1.独立代码：

//-----------开始---------------------//
#include <atlbase.h>
#include <mshtml.h>
#include <winuser.h>
#include <comdef.h>
#include <string.h>
void EnumIE(void);//处理网页
CComModule _Module;  //使用CComDispatchDriver ATL的智能指针，此处必须声明
#include <atlcom.h>
void EnumAnchorElement(IHTMLDocument2 * pIHTMLDocument2);//获取内嵌网页



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
    cIEUrl_Filter="[http://127.0.0.1/iAgent](http://127.0.0.1/iAgent)"; //设置过滤的网址
    //*******************************************************************************

    CComBSTR IEUrl;
 spBrowser->get_LocationURL(&IEUrl); 
 CString cIEUrl_Get;     //从机器上取得的HTTP的完整的URL;
 cIEUrl_Get=IEUrl;
 cIEUrl_Get=cIEUrl_Get.Left(cIEUrl_Filter.GetLength()); //截取前面N位

 if (strcmp(cIEUrl_Get,cIEUrl_Filter)==0)
 {
     // 程序运行到此，已经找到了IHTMLDocument2的接口指针       
        //EnumForm(spDocument2); //枚举所有的表单   
  //EnumAllElement(spDocument2);//枚举所有字段
  EnumAnchorElement(spDocument2);//获取内嵌网页
 }    
  }   
} 


void EnumAnchorElement(IHTMLDocument2 * pIHTMLDocument2) //获取内嵌网页
{
 if (!pIHTMLDocument2) return;       
 HRESULT   hr;   

 CComQIPtr<IHTMLElementCollection> spAllElement; 
 hr=pIHTMLDocument2->get_links(&spAllElement);//获取所有网页内所有内嵌网页链接
 if (FAILED(hr))  return;  

 long nLength = 0;
 spAllElement->get_length (&nLength);
 for (int i = 0; i < nLength; i++)
 {
        CComPtr<IDispatch> pDisp;
  hr = spAllElement->item(COleVariant((long)i),COleVariant((long)0),&pDisp); //获取单个链接

  if(SUCCEEDED(hr)) 
  {
   CComQIPtr <IHTMLAnchorElement, &IID_IHTMLAnchorElement> pElement(pDisp);
   BSTR bHref;
   pElement->get_href(&bHref);
   CString strHref=bHref;
   AfxMessageBox(strHref);
  }
 }

}



//-----------结束--------------------//



2.执行代码：

void CDemoDlg::OnOK() 
{
 // TODO: Add extra validation here
 ::CoInitialize(NULL); //初始化COM
     EnumIE();             //枚举浏览器       
     ::CoUninitialize();   //释放COM
 //CDialog::OnOK();
}



