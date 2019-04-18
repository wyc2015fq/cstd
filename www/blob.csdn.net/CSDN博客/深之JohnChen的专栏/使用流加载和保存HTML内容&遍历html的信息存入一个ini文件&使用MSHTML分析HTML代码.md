# 使用流加载和保存HTML内容&遍历html的信息存入一个ini文件&使用MSHTML分析HTML代码 - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:15:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2704


使用流加载和保存HTML内容&遍历html的信息存入一个ini文件&使用MSHTML分析HTML代码

使用流加载和保存HTML内容 

本文的部分内容翻译自MSDN文章Loading HTML content from a Stream

IPersist* 接口，以及它的附属方法，可以被用于Microsoft® Visual C++® 和 WebBrowser 控件使用流载入和保存HTML内容

本文讨论载入HTML内容需要的步骤，分为以下几部分：

定位到 about:blank 
DHTML 对象模型的有效性 
使用 QueryInterface 获得IPersist*接口 
使用IPersist*接口载入和保存HTML内容 
载入和保存HTML元素数据 
已知问题 
参考 
相关主题
定位到 about:blank
IWebBrowser2 接口的IWebBrowser2::Navigate2 方法 使得你可以让浏览器定位(Navigate)到一个URL。在下面的示例代码中, IWebBrowser2::Navigate2 方法 被用于定位到 about:blank 页面. 定位到这个空的页面确保了MSHTML 被加载，并且动态 HTML (DHTML) 对象模型中的 HTML 元素有效.

本示例演示了如何让浏览器定位到一个空的页面。m_pBrowser 变量包含从WebBrowser 控件获得的 IWebBrowser2 接口指针。

m_pBrowser->Navigate2( _T("about:blank"), NULL, NULL, NULL, NULL );
DHTML 对象模型的有效性
DHTML 对象模型 用于访问和操作HTML页面的内容，并且在页面装载之前不可用。你的应用程序通过处理WebBrowser 控件的DWebBrowserEvents2::DocumentComplete事件来判断一个页面是否被装载了。 这个事件可能被页面中的每个框架触发，并且在顶层文档载入完成时再触发一次。你可以通过比较事件传递的IDispatch 接口指针和WebBrowser 控件来判断DWebBrowserEvents2::DocumentComplete 事件是否是顶层框架的。

这个WebBrowser DWebBrowserEvents2::DocumentComplete 事件的示例处理代码演示如何判断事件是否是顶层框架的, （如果是，）这指明HTML页面载入完成. 本示例也演示如何从一个内存块——在这个场合是一个包含需要显示的HTML内容的字符串——创建流。

void myObject::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
    HRESULT hr;
    IUnknown* pUnkBrowser = NULL;
    IUnknown* pUnkDisp = NULL;
    IStream* pStream = NULL;
    HGLOBAL hHTMLText;
    static TCHAR szHTMLText[] = "<html><h1>流测试</h1><p>本HTML内容已经从流中加载。</html>";
    // 这个 DocumentComplete 事件是否是顶层框架窗口的?
    // 检查 COM 标识: 比较IUnknown 接口指针.
    hr = m_pBrowser->QueryInterface( IID_IUnknown,  (void**)&pUnkBrowser );
    if ( SUCCEEDED(hr) )
    {
        hr = pDisp->QueryInterface( IID_IUnknown,  (void**)&pUnkDisp );
        if ( SUCCEEDED(hr) )
        {
            if ( pUnkBrowser == pUnkDisp )
            {   // 这是顶层框架窗口的DocumentComplete 事件 —— 页面 载入完成!
                // 建立一个包含HTML内容的流
                // 另外, 这个流可以是被传递过来的（而不是被创建的）

                size_t = cchLength;
                //  TODO: 安全地判断 szHTMLText的长度，单位是TCHAR.
                hHTMLText = GlobalAlloc( GPTR, cchLength+1 );

                if ( hHTMLText )
                {
                    size_t cchMax = 256;
                    StringCchCopy((TCHAR*)hHTMLText, cchMax + 1, szHTMLText);
                    //  TODO: 在这里加入错误处理代码。
                    hr = CreateStreamOnHGlobal( hHTMLText, TRUE, &pStream );
                    if ( SUCCEEDED(hr) )
                    {
                       // 调用辅助函数让网络浏览器加载流。
                       LoadWebBrowserFromStream( m_pBrowser, pStream  );
                       pStream->Release();
                    }
                    GlobalFree( hHTMLText );
                }
            }
            pUnkDisp->Release();
        }
        pUnkBrowser->Release();
    }
}
使用 QueryInterface 获得IPersis*等接口
WebBrowser 控件的IWebBrowser2::get_Document 属性返回表示顶层框架的DHTML 对象模型的文档对象。MSHTML 通过文档对象和其他HTML元素对象，例如Frame, IFrame等等实现的IPersistStreamInit，IPersistFile等接口提供使用流载入和保存HTML的功能。对象的IDispatch 接口可用于通过使用QueryInterface和IID_IPersistStreamInit 等接口标识查询相应接口指针，如下列代码示例所述.

HRESULT LoadWebBrowserFromStream(IWebBrowser2* pWebBrowser, IStream* pStream)
{
HRESULT hr;
IDispatch* pHtmlDoc = NULL;
IPersistStreamInit* pPersistStreamInit = NULL;
    // 返回文档对象.
    hr = pWebBrowser->get_Document( &pHtmlDoc );
    if ( SUCCEEDED(hr) )
    {
        // >查询 IPersistStreamInit接口
        hr = pHtmlDoc->QueryInterface( IID_IPersistStreamInit,  (void**)&pPersistStreamInit );
        if ( SUCCEEDED(hr) )
        {
            // 初始化文档.
            hr = pPersistStreamInit->InitNew();
            if ( SUCCEEDED(hr) )
            {
                // 载入流内容
                hr = pPersistStreamInit->Load( pStream );
            }
            pPersistStreamInit->Release();
        }
    }
}
使用IPersist*接口载入和保存HTML内容
IPersistStreamInit 接口具有用于从流初始化和载入HTML内容的InitNew 和Load 方法以及用于保存的Save方法。InitNew 方法初始化流到一个已知状态，Load 方法从流载入HTML内容，Save方法将HTML内容保存到流。

IPersistFile 接口具有用于从磁盘文件载入和保存HTML内容的Load 和Save方法。

在前面的示例代码中, HTML文档被初始化，并且HTML内容被从流中载入。

注意  从Microsoft Internet Explorer 5开始，多于一次调用 IPersist* 接口的Load 方法 是可行的。在更早的版本中，每个MSHTML的实例只支持一次Load 调用。
载入和保存HTML元素数据
如果HTML元素支持使用IPersistStorage, IPersistStreamInit, 或者 IPersistMemory，那么也可以通过类似的代码载入和保存信息。

对于网页中的ActiveX控件的信息的载入和保存，可以参考我的文章 如何: 通过HTML文档对象模型访问文档中的ActiveX控件的属性 （CSDN文档中心）来获得控件接口，然后查询ActiveX控件是否支持IPersist*接口。

注意：使用VB6.0编写的控件可能不支持这些接口，这时候需要使用IPersistPropertyBag或者属性集载入和保存信息。参见微软知识库文章Q272490 BUG: Visual Basic 组件的错误 0x800A02E0 "无法保存未初始化的类" 

已知问题
微软知识库文章

Q271868  在Microsoft Internet Explorer (Programming) 5.5中，框架对象不支持IPersistStream, IPersistFile，IPersistMemory接口。

Q323569 BUG: PersistStreamInit::Load() 显示HTML内容为文本

Q264868 BUG:Internet Explorer 没有检测到内容类型从text/html到text/xml的改变。 

参考
下列文章提供了关于组件对象模型(COM)的信息.

Inside OLE, 2nd Edition, by Kraig Brockschmidt (Microsoft Press) 
Understanding ActiveX and OLE, by David Chappell (Microsoft Press) 
Inside COM, by Dale Rogerson (Microsoft Press) 
微软知识库文章

Q223337 信息: 使用Internet Explorer XML 解析器载入/保存XML数据 
Q196340 如何: 获得HTML框架的WebBrowser 对象模型
相关主题

Microsoft Visual Studio  
The Component Object Model Specification 

===============================
遍历html的信息存入一个ini文件 

BOOL SaveFormData(CHtmlView* pView,CString DataFileName,CString SectionName)
{
 IHTMLDocument2* pDoc = NULL;
 IHTMLElementCollection* pAllElem = NULL;
 IHTMLElement* pElem = NULL;
 IHTMLTextAreaElement* pTextArea=NULL;
 IHTMLSelectElement* pSelect=NULL;
 IHTMLInputElement* pInput=NULL;
 BOOL Result=TRUE;
 pDoc = (IHTMLDocument2*)(pView->GetHtmlDocument());
 if(pDoc!=NULL)
 {
  pDoc->get_all(&pAllElem);
  if(pAllElem!=NULL)
  {
   long EleCount;
   pAllElem->get_length(&EleCount);
   VARIANT vEleName;
   BSTR bValue;
   for(int i=0; i    {
    vEleName.vt = VT_I4;
    vEleName.lVal=i;
    if(pAllElem->item(vEleName,vEleName,(LPDISPATCH*)&pElem)==S_OK)
    {
     if(pElem!=NULL)
     {
      pElem->QueryInterface(&pInput);
      if(pInput!=NULL)
      {
       BSTR bType;
       pInput->get_type(&bType);
       CString Type(bType);
       SysFreeString(bType);
       if(Type=="text" || Type=="hidden")
       {
        pInput->get_name(&bValue);
        CString name(bValue);
        SysFreeString(bValue);
        pInput->get_value(&bValue);
        CString value(bValue);
        SysFreeString(bValue);
        WritePrivateProfileString(SectionName,name,value,DataFileName);
       }//if(Type=="text"
       else if(Type=="checkbox")
       {
        pInput->get_name(&bValue);
        CString name(bValue);
        SysFreeString(bValue);
        VARIANT_BOOL Check;
        pInput->get_checked(&Check);
        if(Check)
        {
         pInput->get_value(&bValue);
         CString value(bValue);
         SysFreeString(bValue);
         WritePrivateProfileString(SectionName,name,value,DataFileName);
        }
        else
         WritePrivateProfileString(SectionName,name,"0",DataFileName);
       }
       pInput->Release();
      }//if(pInput!=NULL)
      pElem->QueryInterface(&pTextArea);
      if(pTextArea!=NULL)
      {
       pTextArea->get_name(&bValue);
       CString name(bValue);
       SysFreeString(bValue);
       pTextArea->get_value(&bValue);
       CString value(bValue);
       SysFreeString(bValue);
       WritePrivateProfileString(SectionName,name,value,DataFileName);
       pTextArea->Release();
      }//if(pTextArea!=NULL)
      pElem->QueryInterface(&pSelect);
      if(pSelect!=NULL)
      {
       pSelect->get_name(&bValue);
       CString name(bValue);
       SysFreeString(bValue);
       pSelect->get_value(&bValue);
       CString value(bValue);
       SysFreeString(bValue);
       WritePrivateProfileString(SectionName,name,value,DataFileName);
       pSelect->Release();
      }//if(pSelect!=NULL)
      pElem->Release();
     }//if(pElem!=NULL)
    }//pAllElem->item(
   }//for
   pAllElem->Release();
  }//if(pAllElem!=NULL)
  pDoc->Release();
 }//if(pDoc!=NULL)
 return Result;
}

=========================
使用MSHTML分析HTML代码 

原作者：Asher Kobin

环境：Windows 2000 / Windows ME / IE 5.0+ 

我有很多在程序中使用MSHTML的经验，并经常涉及到怎样使用MSHTML来分析HTML代码并通过DOM来访问的问题。

这里给出一个例子，我使用了MSHTML提供的IMarkupServices接口。不需要IOleClientSite或其他任何的嵌入式操作。它可以尽可能简单的获得你所需要的东西。

以后的文章，我将集中的介绍在程序中使用另外的MSHTML使用方法，比如使用MSHTML作为一个编辑器。

这段代码只使用了简单的COM调用。它可以很容易的使用在ATL、MFC、VB和其他的一些语言中。请不要向我要其他语言的例子。如果你需要做的话，你需要IE SDK。 

/******************************************************************
 * ParseHTML.cpp
 *
 *  ParseHTML: Lightweight UI-less HTML parser using MSHTML
 *
 *  Note: This is for accessing the DOM only. No image download, 
 *        script execution, etc...
 *
 *  8 June 2001 - Asher Kobin (asherk@pobox.com)
 *  
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY 
 *  OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT 
 *  LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR 
 *  FITNESS FOR A PARTICULAR PURPOSE.
 *
 *******************************************************************/

#include 
#include 

OLECHAR szHTML[] = OLESTR("Hello World!");

int __stdcall WinMain(HINSTANCE hInst, 
                      HINSTANCE hPrev, 
                      LPSTR lpCmdLine, 
                      int nShowCmd)
{
  IHTMLDocument2 *pDoc = NULL;

  CoInitialize(NULL);

  CoCreateInstance(CLSID_HTMLDocument, 
                   NULL, 
                   CLSCTX_INPROC_SERVER, 
                   IID_IHTMLDocument2, 
                   (LPVOID *) &pDoc);

  if (pDoc)
  {
    IPersistStreamInit *pPersist = NULL;

    pDoc->QueryInterface(IID_IPersistStreamInit, 
                       (LPVOID *) &pPersist);

    if (pPersist)
    {
      IMarkupServices *pMS = NULL;

      pPersist->InitNew();
      pPersist->Release();

      pDoc->QueryInterface(IID_IMarkupServices, 
                              (LPVOID *) &pMS);

      if (pMS)
      {
        IMarkupContainer *pMC = NULL;
        IMarkupPointer *pMkStart = NULL;
        IMarkupPointer *pMkFinish = NULL;

        pMS->CreateMarkupPointer(&pMkStart);
        pMS->CreateMarkupPointer(&pMkFinish);

        pMS->ParseString(szHTML, 
                         0, 
                         &pMC, 
                         pMkStart, 
                         pMkFinish);

        if (pMC)
        {
          IHTMLDocument2 *pNewDoc = NULL;

          pMC->QueryInterface(IID_IHTMLDocument, 
                              (LPVOID *) &pNewDoc);

          if (pNewDoc)
          {
            // do anything with pNewDoc, in this case 
            // get the body innerText.

            IHTMLElement *pBody;
            pNewDoc-gt;get_body(&pBody);

            if (pBody)
            {
              BSTR strText;

              pBody->get_innerText(&strText);
              pBody->Release();

              SysFreeString(strText);
            }

            pNewDoc->Release();
          }

          pMC->Release();
        }

        if (pMkStart)
            pMkStart->Release();

        if (pMkFinish)
          pMkFinish->Release();

        pMS->Release();
      }
    }

    pDoc->Release();
  }

  CoUninitialize();

  return TRUE;
}

