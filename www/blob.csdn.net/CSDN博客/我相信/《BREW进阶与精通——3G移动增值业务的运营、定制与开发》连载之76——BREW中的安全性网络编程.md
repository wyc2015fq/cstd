# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之76——BREW中的安全性网络编程 - 我相信...... - CSDN博客





2010年09月29日 13:55:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1272








安全性的网络编程主要是通过SSL实现的，首先要创建创建SSLRoot，可用的根证书通常是常用的VeriSign 根证书。它们可以节省手持设备上的空间，并且仅保留一个副本，而不是在每个使用 SSL 的应用程序中都保留副本。手持设备仅包含常用根证书的一小部分，从而防止占用过多的空间。SSL的实现有两种方法，一种是面向socket的实现，另一种是面向http的实现及HTTPS。这里主要阐明怎样使用IWeb接口实现HTTPS的功能。

// defined in pMe

IWebOpts *piRoots; …

if(ISHELL_CreateInstance(piShell, AEECLISD_SSLROOTCERTS, (void **)&piRoots) != SUCCESS){ }

然后发送root证书给服务器

// defined in App Structure

AEECallbackpcb;

IWeb*pIWeb;

IWebResp*pIWebResp;

…

// Create instance of IWeb

if(ISHELL_CreateInstance(piShell, AEECLISD_WEB, (void**)&pMe->pIWeb) != 

SUCCESS)

goto failure;

// Initialize callback

CALLBACK_Init(&pMe->pcb, (PFNNOTIFY)CheckSecureResp, pMe);

// First secure connection attempt

IWeb_GetResponse(pMe->pIWeb, (pMe->pIWeb, &pMe->pIWebResp, &pMe->pcb,

"https://brewx.qualcomm.com/developer/devexhome.jsp",

WEBOPT_HEADERHANDLER, HeaderHandlerFn,

WEBOPT_DEFAULT, pMe->piRoots, 

WEBOPT_END));

同时，需要获得HTTPS的头信息，如果需要的话，还要提供登录等功能：

void Header(void* pNotifyData, const char* pszName, GetLine* pplVal) {

HTTPSDemo* pMe = pNotifyData;

if(STRBEGINS("WWW-authenticate:", pszName)) {

if(pplVal) {

AppendIDPswdHeader(pMe, pplVal, “user”, “pass”);

}

}

if(STRBEGINS("Set-Cookie:", pszName)) {

if(pplVal) {

AppendCookieHeader(pMe, pplVal->psz);

}

}

}

现在，就可以顺利的建立https的连接了

// Create new IWeb transaction

if(pMe->pIWeb) {

IWEB_Release(pMe->pIWeb);

pMe->pIWeb = NULL;

}

if(ISHELL_CreateInstance(piShell, AEECLISD_WEB, (void**)&pMe->pIWeb) !=SUCCESS)

// handle failure

// Initialize callback

CALLBACK_Cancel(&pMe->pcb);

CALLBACK_Init(&pMe->pcb, (PFNNOTIFY)CheckSecureResp, pMe);

// Try a successful attempt

IWEB_GetResponse(pMe->pIWeb, (pMe->pIWeb, &pMe->pIWebResp, &pMe->pcb, 

"https://brewx.qualcomm.com/developer/devexhome.jsp", WEBOPT_HEADER, pMe->buffer,

WEBOPT_HEADERHANDLER, HeaderHandlerFn,WEBOPT_DEFAULTS, pMe->pIRoots,

WEBOPT_END));

为了保证应用的健壮性，需要关注SSL中的错误处理：

WebOpt wo;

SSLInfo* pSSLInfo = NULL;

// Query for SSL webopt

IWEBRESP_GetOpt(pMe->pIWebResp, WEBOPT_SSL_SEC_INFO, 0, &wo);

// pointer to ssl info

pSSLInfo = (SSLInfo*)(wo.pVal);

// error values are held in pSSLInfo->nResult

另外，还可以通过X.509对SSL进行配置：

X509TrustOVerride TO;

TO.uCertID = 0;

TO.uOverrideBits = X509CHAIN_CERT_EXIPRED;

IWeb_GetResponse(pMe->pIWeb,(pMe->pIWeb,pMe->pWebResp,&pMe->CallBack,

"https://www.securesite.com",WEBOPT_SSL_ALLOWED_VERSION, SSL_VERSION_30,

WEBOPT_X509_ROOT_CERT, pOurRoot, WEBOPT_X509_OVERRIDE, &TO,

WEBOPT_DEFAULTS, pMe->pIRoots, WEBOPT_END));

对于银行类应用，或者证券交易类应用，一般会使用自定义的证书，示例如下：

IFileMgr*pIFileMgr;

IFile*pIFile;

WebOptapwoz;

if(ISHELL_CreateInstance(piShell, AEECLSID_FILEMGR, &pIFileMgr)) !=SUCCESS）

goto Failure;

pIFile = IFILEMGR_OpenFile(pIFileMgr, “myCert.cer”, _OFM_READ);

IFILE_Read(pIFile, buffer, sizeof(buffer));

apwoz.nID = WEBOPT_X509_ROOT_CERT;

apwoz.pVal = buffer;

IWEB_AddOpt(pMe->pIWeb, apwoz);

IFILE_Release(pIFile);

IFILEMGR_Release(pIFileMgr);



