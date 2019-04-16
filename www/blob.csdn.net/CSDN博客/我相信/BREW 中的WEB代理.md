# BREW 中的WEB代理 - 我相信...... - CSDN博客





2010年11月04日 11:39:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1118








在中国电信运营BREW业务的时候，如果开发商需要访问互联网上的服务器资源，可能要应用相应的代理规范才可以，其实现可以有两种方式来设置proxy，一种方式是事先设置好WebOpt，这时需要如下代码增加web选项：

      awo[i].nId  = WEBOPT_PROXYSPEC;

      awo[i].pVal = (void *)"[http:///http://10.0.0.200:80](/http://10.0.0.200:80)";

第二种方式是在IWEB_GetResponse的参数中直接加入WEBOPT_PROXYSPEC项

     IWEB_GetResponse(pApp->m_pIWeb,

                      (pApp->m_pIWeb, &pwa->piWResp, &pwa->cb, pszUrl,

                      WEBOPT_PROXYSPEC, "[http:///http://10.0.0.200:80](/http://10.0.0.200:80)",

                      WEBOPT_END));：



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/09/29/5914112.aspx](http://blog.csdn.net/wireless_com/archive/2010/09/29/5914112.aspx)



