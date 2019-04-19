# WINCE手机拨号 - xqhrs232的专栏 - CSDN博客
2013年01月09日 22:27:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：539
原文地址::[http://blog.csdn.net/huang5556019/article/details/7773527](http://blog.csdn.net/huang5556019/article/details/7773527)
手机拨号的2种方式
1、Ras_系列函数
注：只在CDMA手机上测试成功
#include <ras.h>
#include <Raserror.h>
#pragma comment ( lib, \"Rasapi32.lib\" )
// 获取所有的连接的名称
RASENTRYNAME szEntryNames[MAX_ENTRYS];
memset(&szEntryNames, 0, sizeof(szEntryNames));
for (int i=0; i<MAX_ENTRYS; i++)
{
       szEntryNames[i].dwSize = sizeof(RASENTRYNAME);
}
DWORD dwCb = sizeof(szEntryNames); 
DWORD dwEntries = 0;
uiRet = RasEnumEntries( NULL, NULL, szEntryNames, &dwCb, &dwEntries);
if (uiRet != ERROR_SUCCESS)
{
       return uiRet;
}
// 获取Entry的属性
uiRet = RasGetEntryProperties(NULL, 
   szEntryNames[i].szEntryName,  
   &entryProperty, 
   &dwEntrySize, 
   szSP, 
   &dwSPLen);
// 获取拨号参数
RASDIALPARAMS rdParams = {0};
rdParams.dwSize = sizeof(RASDIALPARAMS);
lstrcpy(rdParams.szEntryName, szEntryNames[nEntryGPRS].szEntryName); 
BOOL bPassword = FALSE; 
uiRet = RasGetEntryDialParams(NULL, &rdParams, &bPassword);
if (uiRet != ERROR_SUCCESS)
{
       return uiRet;
}
lstrcpy(rdParams.szPhoneNumber, entryProperty.szLocalPhoneNumber); // CDMA的电话号码// 
// 拨号
uiRet = RasDial(NULL, NULL, &rdParams, 0, NULL, &m_hRasConn);
**2、ConnMgr_系列函数**
注：在CDMA、GPRS、EDGE拨号都成功了。。
#include <initguid.h>
#include <Connmgr.h>
#pragma comment ( lib, \"Cellcore.lib\" )
// 建立拨号链接
DWORD EstablishConnection()
{
     CONNMGR_CONNECTIONINFO connInfo;
     ZeroMemory(&connInfo, sizeof(connInfo));
     connInfo.cbSize = sizeof(connInfo);
     connInfo.dwParams = CONNMGR_PARAM_GUIDDESTNET;
     connInfo.dwFlags = CONNMGR_FLAG_PROXY_HTTP 
           | CONNMGR_FLAG_PROXY_WAP 
           | CONNMGR_FLAG_PROXY_SOCKS4 
           | CONNMGR_FLAG_PROXY_SOCKS5;
     connInfo.dwPriority = CONNMGR_PRIORITY_USERINTERACTIVE;
     connInfo.guidDestNet = IID_DestNetInternet;
     connInfo.bExclusive= FALSE;
     connInfo.bDisabled = FALSE;
     DWORD dwStatus = 0;
     if (ConnMgrEstablishConnectionSync(&connInfo, &m_hConnection, 2*60*1000, &dwStatus) != S_OK)
     // 开始拨号
     {
           DWORD dwLastError = GetLastError ();
           ReleaseConnection();
           return dwLastError;
     }
     return S_OK;
}
// 断开拨号链接
DWORD ReleaseConnection()
{
     if (m_hConnection != NULL)
     {
           LONG lCache = 0;
           ConnMgrReleaseConnection(m_hConnection, lCache);
           m_hConnection = NULL;
     }
     return 0;
}
