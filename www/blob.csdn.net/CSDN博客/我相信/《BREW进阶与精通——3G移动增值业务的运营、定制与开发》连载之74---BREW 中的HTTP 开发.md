# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之74---BREW 中的HTTP 开发 - 我相信...... - CSDN博客





2010年09月29日 13:51:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1145








IWeb接口的主要目的是为应用产生一个HTTP请求，对文件的请求使用HTTP中的“Get”方法，上载数据是通过HTTP的“Post”方法实现的。IWEB 能够管理自己的连接和sockets，使用IWEB_GetResponse 函数就可以启动一个 web 访问。

请求互联网上的一个文件示例如下：

// Create IWeb instance

ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_WEB, (void **)&pMe->m_pIWeb);

// Initialize the callback to WebReadCB

CALLBACK_Init(&pMe->m_Callback, WebReadCB, pMe);

// Request IWEB to fetch the file

IWEB_GetResponse(pMe->m_pIWeb,

(pMe->m_pIWeb, &pMe->m_pIWebResp, &pMe->m_Callback,

"http://www.qualcomm.com",WEBOPT_HANDLERDATA, pMe,

WEBOPT_STATUSHANDLER, WebStatusNotification,

WEBOPT_METHOD, "GET",WEBOPT_END));

其中WebReadCB（）为获得服务器相应内容的回调函数，参考实现如下：

static void WebReadCB(void* cxt)

{

…

// Get information about the response

WebRespInfo* **pWebRespInfo **= IWEBRESP_GetInfo(pMe->m_pIWebResp);

// the body of the response is contained in the ISOURCE within

ISource* pISource = **pWebRespInfo**->pisMessage;

}

WebOpts（Web 选项）用于操作 IWeb 配置选项。IWebOpts代表选项堆栈或列表，这些选项可以为单值或多值，且不强制单值或多值选项的语义。选项堆栈的访问按添加时的顺序进行，可以通过IWebOpt设定，也可以Web请求中直接指定。

PFNWEBHEADER是用于获取HTTP header信息的回调函数原型。它将作为 WEBOPT_HEADERHANDLER传递给IWEB_AddOpt() 或 IWEB_GetResponse()。PFNWEBSTATUS是获取请求状态的回调原型，将作为 IWEBOPT_STATUSHANDLER 传递给IWEB_AddOpt() 或 IWEB_GetResponse()。

如果URL中含有一些关键字的话，需要先对URL进行编码，否则无法正确解析，其示例如下：

// Determine what the length of the output buffer needs to be

**IWEBUTIL_UrlEncode**(pMe->m_pIWebUtil,

(const char *)(buf), // buffer containing the suffix of URL with reserved chars

&nDataLen, // number of characters to be encoded

NULL, // if NULL, method calculatessize of buffer needed

&pMe->m_dwContentLength // contains the length of the buffer required

);

// Allocate space to hold the encoded string

if(pMe->m_dwContentLength)

pMe->m_szData = (char*) MALLOC(pMe->m_dwContentLength);// Encode the string.

**IWEBUTIL_UrlEncode**(pMe->m_pIWebUtil,

(const char *)(buf), &nDataLen,pMe->m_szData,&pMe->m_dwContentLength);

IWEB 接口可以使HTTP连接一直处于激活状态，而且这是IWeb接口的默认状态，通过使用Web选项WEBREQUEST_NOKEEPALIVE 和 WEBOPT_FLAG可以禁用这一功能。其中保持激活状态依赖于Content-Length 的HTTP头，只有当“Content-Length: xxx”的HTTP头有效时，这一特性才能正常工作。对于远端，如果需要知道何时调整读取的时候或者需要知道内容长度的时候，连接都需要被关闭。



