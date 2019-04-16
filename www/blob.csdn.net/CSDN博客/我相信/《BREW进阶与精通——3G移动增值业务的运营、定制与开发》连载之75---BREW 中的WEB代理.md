# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之75---BREW 中的WEB代理 - 我相信...... - CSDN博客





2010年09月29日 13:53:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1142








在中国电信运营BREW业务的时候，如果开发商需要访问互联网上的服务器资源，可能要应用相应的代理规范才可以，其实现可以有两种方式来设置proxy，一种方式是事先设置好WebOpt，这时需要如下代码增加web选项：

awo[i].nId= WEBOPT_PROXYSPEC;

awo[i].pVal = (void *)"http:///http://10.0.0.200:80";

第二种方式是在IWEB_GetResponse的参数中直接加入WEBOPT_PROXYSPEC项

IWEB_GetResponse(pApp->m_pIWeb,

(pApp->m_pIWeb, &pwa->piWResp, &pwa->cb, pszUrl, 

WEBOPT_PROXYSPEC, "http:///http://10.0.0.200:80", 

WEBOPT_END));：



