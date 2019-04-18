# VC与JavaScript交互(四) ———— WebBrowser或CHtmlView中轻松屏蔽脚本错误(JavaScript) - L_Andy的专栏 - CSDN博客

2014年10月11日 19:01:20[卡哥](https://me.csdn.net/L_Andy)阅读数：5521


# 1.什么是javascript脚本错误

## 1.1    概述

JavaScript脚本错误包含“运行时错误”和“语法错误”。

## 1.2    JavaScript“语法错误”

JavaScript语法错误是指当 JavaScript语句违反了 JavaScript脚本语言的一条或多条语法规则时导致的错误。JavaScript语法错误发生在程序编译阶段，在开始运行该程序之前。

## 1.3    JavaScript“运行时错误”

JavaScript运行时错误是指当 JavaScript脚本试图执行一个系统不能运行的动作时导致的错误。当正在运行脚本、计算变量表达式、或者正在动态分配内存时出现 JavaScript运行时错误时。

# 2.    为什么要屏蔽javascript脚本错误？

由于开发海纳产品时，使用WebBrowser和CHtmlView来展示页面，进行填表等操作；但是由于打开的页面大多是其他用户的CMS页面，所以难免有些有脚本错误，于是决定要来屏蔽脚本错误，提升产品的易用性和友好性。

# 3.    怎么去屏蔽javascript脚本错误？

## 3.1    使用SetSilent函数

使用WebBrowser或CHtmlView的SetSilent函数可以达到屏蔽脚本错误的目的，不过这种情况，其它提示信息也都不显示了，例如使用alert进行的错误提示。

如果你觉得这样能满足你，那么推荐使用这种方法，简单啊！![](http://www.cnblogs.com/Emoticons/msn/tongue_smile.gif)

## 3.2    重载IOleCommandTarget的Exec函数

网上比较多资料都是说重载IOleCommandTarget中的Exec函数来进行屏蔽脚本错，定义如下：

HRESULT  Exec( const GUID* pguidCmdGroup, DWORD nCmdID,

      DWORD nCmdexecopt, VARIANTARG* pvaIn, VARIANTARG* pvaOut )

 然后通过判断nCmdID是否等于OLECMDID_SHOWSCRIPTERROR(即报javascript脚本错误)来进行屏蔽；由于本人对COM和OLE的知识有限，琢磨了半天也没有想到怎么实现IOleCommandTarget接口中的Exec函数，然后跟我的WebBrowser或是HtmlView挂钩起来![](http://www.cnblogs.com/Emoticons/msn/cry_smile.gif)，于是决定放弃这种方法，有兴趣的朋友可以查看参考资料的文章继续尝试一下。

## 3.3    另一种方法

不死心，继续在网上找，突然发现了一篇文章，介绍在html页面中，可以使用javascript的事件来进行javascript脚本错误的屏蔽，于是拷贝下来尝试，果然有用（即使IE浏览器设置了脚本调试，也不会进行提示![](http://www.cnblogs.com/Emoticons/msn/teeth_smile.gif)），经改造的代码如下：

![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<html>
 2![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<head>
 3![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)<script type="text/javascript">
 4![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 5![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)function fnObjNotDefine(){
 6![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    domethod();
 7![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
 8![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 9![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)function fnOnError(msg,url,lineno){
10![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    <!--
11![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    alert("window.onerror\n\n" +
12![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    "Error: " + msg + "\n" +
13![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    "URL:  " + url + "\n" +
14![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    "Line:  " + lineno);
15![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)returntrue; -->
16![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
17![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)window.onerror = fnOnError;
18![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)MethodName.badcommand();
19![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
20![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)function fnOnLoad(){
21![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    alert("on load!");
22![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
23![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)</script>
24![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)</head>
25![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<body onload="fnOnLoad();">
26![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<input type="button" value="function not defined" onclick="badcommand();">
27![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<input type="button" value="object not defined" onclick="fnObjNotDefine();">
28![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)</body>
29![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)</html>
30![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)


通过查看javascript代码，发现是“重载”了window.onerror这个事件，只要它返回true，脚本错误就不显示了，估计这个就是Microsoft自己实现的截取javascript脚本错误信息的接口，于是就想怎么把它插入到页面当中，其中有篇文章介绍说在OnDocumentComplete时来实现javascript的插入，经实践，这种方法是不行的；经过本人的不断尝试，发现在OnNavigateComplete2或OnNavigateComplete里实现javascript的注入是可行的，这两个函数只要实现一个就行，就看你用的是Navigate2还是Navigate来打开页面了。这里使用Navigate2来做例子，具体代码如下：

![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)void CMyWebBrowser::OnNavigateComplete2(LPCTSTR strURL)
 2![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 3![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
 4![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 5![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)       CComPtr<IDispatch>   spDisp   =   GetHtmlDocument(); 
 6![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 7![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(spDisp   !=   NULL) 
 8![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 9![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){ 
10![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
11![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              CComPtr<IHTMLDocument2> doc;
12![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
13![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              spDisp->QueryInterface(IID_IHTMLDocument2, reinterpret_cast<void**>(&doc));
14![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
15![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(doc != NULL)
16![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
17![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){   
18![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
19![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                     IHTMLWindow2 * pIhtmlwindow2 = NULL;
20![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
21![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                     doc->get_parentWindow(&pIhtmlwindow2);
22![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
23![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(pIhtmlwindow2 != NULL)
24![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
25![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
26![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
27![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//屏蔽javascript脚本错误的javascript脚本
28![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
29![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            CString strJavaScriptCode = "function fnOnError(msg,url,lineno){alert('script error:\\n\\nURL:'+url+'\\n\\nMSG:'+msg +'\\n\\nLine:'+lineno);return true;}window.onerror=fnOnError;";
30![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
31![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            BSTR bstrScript = strJavaScriptCode.AllocSysString();
32![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
33![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            CString strLanguage("JavaScript");
34![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
35![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            BSTR bstrLanguage = strLanguage.AllocSysString();
36![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
37![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)long lTime = 1 * 1000;
38![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
39![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)long lTimeID = 0;
40![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
41![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            VARIANT varLanguage;
42![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
43![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            varLanguage.vt = VT_BSTR;
44![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
45![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            varLanguage.bstrVal = bstrLanguage;
46![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
47![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            VARIANT pRet;
48![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
49![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//把window.onerror函数插入入当前页面中去
50![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
51![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            pIhtmlwindow2->execScript(bstrScript, bstrLanguage, &pRet);
52![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
53![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            ::SysFreeString(bstrScript);
54![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
55![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            ::SysFreeString(bstrLanguage);
56![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
57![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
58![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
59![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            pIhtmlwindow2->Release();
60![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
61![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)                     }
62![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
63![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)              }
64![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
65![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)       }
66![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
67![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}

![复制代码](http://common.cnblogs.com/images/copycode.gif)

其中，CMyWebBrowser是我自己继承了CHtmlView类的一个实现类， 这个函数可以在你的WebBrowser2或继承了CHtmlView类中实现，编写一个带有脚本错误的页面，打开进行浏览，是不是发现脚本错误被屏蔽了? 哈哈，实现起来也不麻烦。于是就把这个方法贴出来，供大家参考![](http://www.cnblogs.com/Emoticons/msn/wink_smile.gif)。

另： 经测试，发现如果存在iframe嵌套的时候，嵌套的iframe中包含脚本错误，以上方法是不能屏蔽iframe中的脚本错误的，因为window.onerror只针对当前页面有效，因此需要在OnNavigateComplete2函数里加上对当前页面进行递归所有子页面，然后重复执行execScript操作即可。

**最终代码为：**

![复制代码](http://common.cnblogs.com/images/copycode.gif)
  1![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)void CMyWebBrowser::OnNavigateComplete2(LPCTSTR strURL)
  2![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
  3![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
  4![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
  5![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)       CComPtr<IDispatch>   spDisp   =   GetHtmlDocument(); 
  6![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
  7![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(spDisp   !=   NULL) 
  8![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
  9![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){ 
 10![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 11![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              CComPtr<IHTMLDocument2> doc;
 12![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 13![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              spDisp->QueryInterface(IID_IHTMLDocument2, reinterpret_cast<void**>(&doc));
 14![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 15![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(doc != NULL)
 16![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 17![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){   
 18![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 19![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                     CScriptErrHandler scriptHandler;
 20![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 21![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                     scriptHandler.ShieldCurrPage(doc);
 22![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 23![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                     scriptHandler.ShieldAllChildPages(doc);
 24![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 25![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)              }
 26![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 27![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)       }
 28![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 29![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
 30![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 31![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)ScriptErrHandler.cpp文件：
 32![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 33![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)#include "StdAfx.h"
 34![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 35![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)#include "ScriptErrHandler.h"
 36![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 37![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)CScriptErrHandler::CScriptErrHandler(void)
 38![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 39![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
 40![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 41![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)       CString strJavaScriptCode = "function fnOnError(msg,url,lineno){alert('script error:\\n\\nURL:'+url"
 42![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 43![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)"+'\\n\\nMSG:'+msg +'\\n\\nLine:'+lineno+'\\n\\nframes:' + window.frames.length);return true;}window.onerror=fnOnError;";
 44![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 45![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 46![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 47![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//屏蔽的脚本,可以改进为从文本里读取
 48![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 49![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)       m_bstrScript = strJavaScriptCode.AllocSysString();
 50![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 51![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
 52![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 53![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 54![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 55![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)CScriptErrHandler::~CScriptErrHandler(void)
 56![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 57![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
 58![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 59![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)       SysFreeString(m_bstrScript);
 60![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 61![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
 62![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 63![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 64![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 65![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 66![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 67![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)void CScriptErrHandler::ShieldCurrPage(CComPtr<IHTMLDocument2> &doc)
 68![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
 69![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
 70![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 71![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)       CComPtr<IHTMLWindow2>  spIhtmlwindow2;
 72![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 73![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)       doc->get_parentWindow(reinterpret_cast<IHTMLWindow2**>(&spIhtmlwindow2));
 74![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 75![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(spIhtmlwindow2 != NULL)
 76![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 77![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
 78![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 79![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              CString strLanguage("JavaScript");
 80![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 81![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              BSTR bstrLanguage = strLanguage.AllocSysString();
 82![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 83![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)long lTime = 1 * 1000;
 84![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 85![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)long lTimeID = 0;
 86![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 87![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              VARIANT varLanguage;
 88![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 89![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              varLanguage.vt = VT_BSTR;
 90![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 91![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              varLanguage.bstrVal = bstrLanguage;
 92![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 93![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              VARIANT pRet;
 94![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 95![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//把window.onerror函数插入入当前页面中去
 96![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 97![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              spIhtmlwindow2->execScript(m_bstrScript, bstrLanguage, &pRet);
 98![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
 99![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              ::SysFreeString(bstrLanguage);
100![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
101![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)       }
102![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
103![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
104![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
105![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
106![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
107![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)void CScriptErrHandler::ShieldAllChildPages(CComPtr<IHTMLDocument2> &parentDoc)
108![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
109![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
110![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
111![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)       WalkAllChildPages(parentDoc);
112![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
113![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
114![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
115![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
116![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
117![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)void CScriptErrHandler::WalkAllChildPages(CComPtr<IHTMLDocument2> &parentDoc)
118![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
119![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
120![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
121![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)       CComPtr<IHTMLFramesCollection2> spFramesCol;
122![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
123![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)       HRESULT hr = parentDoc->get_frames(&spFramesCol);
124![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
125![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(SUCCEEDED(hr) && spFramesCol != NULL)
126![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
127![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
128![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
129![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)long lSize = 0;
130![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
131![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)              hr = spFramesCol->get_length(&lSize);
132![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
133![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if (SUCCEEDED(hr))
134![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
135![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
136![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
137![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)for(int i=0; i<lSize; i++)
138![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
139![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
140![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
141![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            VARIANT frameRequested;
142![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
143![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            VARIANT frameOut;
144![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
145![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            frameRequested.vt = VT_UI4;
146![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
147![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            frameRequested.lVal = i;
148![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
149![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                            hr = spFramesCol->item(&frameRequested, &frameOut);
150![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
151![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(SUCCEEDED(hr) && frameOut.pdispVal != NULL)
152![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
153![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
154![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
155![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                                   CComPtr<IHTMLWindow2> spChildWindow;
156![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
157![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
158![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
159![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                                   hr = frameOut.pdispVal->QueryInterface(IID_IHTMLWindow2,reinterpret_cast<void**>(&spChildWindow));
160![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
161![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(SUCCEEDED(hr) && spChildWindow != NULL)
162![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
163![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
164![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
165![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                                          CComPtr<IHTMLDocument2> spChildDocument;
166![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
167![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                                          hr = spChildWindow->get_document(reinterpret_cast<IHTMLDocument2**>(&spChildDocument));
168![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
169![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)if(SUCCEEDED(hr) && spChildDocument != NULL)
170![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
171![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
172![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
173![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                                                 ShieldCurrPage(spChildDocument);
174![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
175![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                                                 WalkAllChildPages(spChildDocument);
176![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
177![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)                                          }
178![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
179![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)                                   }
180![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
181![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                                   frameOut.pdispVal->Release();
182![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
183![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)                            }
184![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
185![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)                     }
186![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
187![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)              }
188![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
189![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)       }
190![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
191![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}

![复制代码](http://common.cnblogs.com/images/copycode.gif)

目前存在的一个缺陷是OnNavigateComplete2会被调用多次，那么嵌入javascript的操作也会被执行多次(不知道会产生什么副作用，目前尚未发现)；CMyWebBrowser从CHtmlView类继承，代码在VC2008和VC6.0下调试通过；若需要工程代码，[请发送邮件到zhangqingping@hylanda.com](mailto:%E8%AF%B7%E5%8F%91%E9%80%81%E9%82%AE%E4%BB%B6%E5%88%B0zhangqingping@hylanda.com) 。

# 4.    参考资料

## 4.1   How to handle script errors as a WebBrowser control host

[http://support.microsoft.com/default.aspx?scid=kb;en-us;261003](http://support.microsoft.com/default.aspx?scid=kb;en-us;261003)

## 4.2   Script error notification is not sent to Exec method of WebBrowser Host

[http://support.microsoft.com/kb/317024/en-us#top](http://support.microsoft.com/kb/317024/en-us#top)

## 4.3   How to Trap JScript Errors in Internet Explorer 4.01 and Earlier

[http://support.microsoft.com/kb/183616/en-us](http://support.microsoft.com/kb/183616/en-us)

这个是成功代码 

void CZCSoftView::OnNavigateComplete2(LPCTSTR strURL) 

{ 

// TODO: Add your specialized code here and/or call the base class 

CComPtr<IDispatch> spDisp = GetHtmlDocument(); 

if(spDisp!=NULL) 

{ 

CComPtr<IHTMLDocument2> doc; 

spDisp->QueryInterface(IID_IHTMLDocument2, reinterpret_cast<void**>(&doc)); 

if(doc != NULL) 

{ 

IHTMLWindow2 * pIhtmlwindow2 = NULL; 

doc->get_parentWindow(&pIhtmlwindow2); 

if(pIhtmlwindow2 != NULL) 

{//"function fnOnError(msg,url,lineno){alert('script error:\\n\\nURL:'+url+'\\n\\nMSG:'+msg +'\\n\\nLine:'+lineno);return true;}window.onerror=fnOnError;"; 

//屏蔽javascript脚本错误的javascript脚本 

CString strJavaScriptCode = "function fnOnError(){return true;}window.onerror=fnOnError;"; 

BSTR bstrScript = strJavaScriptCode.AllocSysString(); 

CString strLanguage("JavaScript"); 

BSTR bstrLanguage = strLanguage.AllocSysString(); 

long lTime = 1 * 1000; 

long lTimeID = 0; 

VARIANT varLanguage; 

varLanguage.vt = VT_BSTR; 

varLanguage.bstrVal = bstrLanguage; 

VARIANT pRet; 

//把window.onerror函数插入入当前页面中去 

pIhtmlwindow2->execScript(bstrScript, bstrLanguage, &pRet); 

::SysFreeString(bstrScript); 

::SysFreeString(bstrLanguage); 

pIhtmlwindow2->Release(); 

} 

} 

} 

CHtmlView::OnNavigateComplete2(strURL); 

} 



![Angela doudou](http://img3.douban.com/icon/user_normal.jpg)

禁止弹出窗口 给你的从CHtmlView派生而来的类加一个成员变量BOOL m_bPop; 

　　在构造函数里给m_bPop初始化为：m_bPop = TRUE; 

　　重载OnDownloadBegin虚函数，在其中加入：m_bPop = FALSE; 

　　重载OnDownloadComplete虚函数，在其中加入：m_bPop = TRUE; 

　　重载OnNewWindow2虚函数，在其中加入： 

　　Cancel = m_bPop; 

禁止脚本错误 在初始化的时候设置 SetSilent(TRUE);


