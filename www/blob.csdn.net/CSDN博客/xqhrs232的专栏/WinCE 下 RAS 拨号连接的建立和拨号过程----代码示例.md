# WinCE 下 RAS 拨号连接的建立和拨号过程----代码示例 - xqhrs232的专栏 - CSDN博客
2013年01月30日 05:52:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1212
原文地址::[http://blog.csdn.net/91program/article/details/8262374](http://blog.csdn.net/91program/article/details/8262374)
在 CE 下如果系统没有建立拨号网络，要实现上网功能时，就需要通过代码来实现建立拨号网络、拨号实现上网的过程。
以下代码中的参数和拨号所用数值，都是电信 EVDO 的配置。如果换 GSM 或 WCDMA 时，请相应修改。
**[cpp]**[view
 plain](http://blog.csdn.net/91program/article/details/8262374#)[copy](http://blog.csdn.net/91program/article/details/8262374#)
- 
**[cpp]**[view
 plain](http://blog.csdn.net/91program/article/details/8262374#)[copy](http://blog.csdn.net/91program/article/details/8262374#)
- HRASCONN ghRasConn = NULL;  
- /*
-  * 设置串口波特率和数据位等
- */
- BOOL CTCPClientDlg::SetDevCfg(TCHAR *ptcRasEntryName)  
- {  
- HKEY hk;  
-     unsigned char cDevCfg[684];  
- TCHAR tcRegAddr[512 + 1];  
- DWORD *pdwEapConnData = NULL;  
- 
-     ZeroMemory(tcRegAddr,sizeof(TCHAR) * (512 + 1));  
-     ZeroMemory(cDevCfg,sizeof(char) * 683);  
-     cDevCfg[0] = 0x30;  
-     cDevCfg[4] = 0x05;  
-     cDevCfg[9] = 0x01;  
-     cDevCfg[13] = 0xC2;     // 波特率
-     cDevCfg[14] = 0x01;  
-     cDevCfg[18] = 0x08;     // 数据位
- 
-     wsprintf(tcRegAddr,L"Comm\\RasBook\\%s",ptcRasEntryName);  
- 
- if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER,tcRegAddr,0,0,&hk))  
-     {  
-         RegSetValueEx(hk,L"DevCfg",0,REG_BINARY,(LPBYTE)cDevCfg,sizeof(cDevCfg));  
-         RegSetValueEx(hk,L"EapConnData",0,REG_BINARY,(LPBYTE)pdwEapConnData,0);  
-         RegFlushKey(HKEY_CURRENT_USER);  
- 
-         RegCloseKey(hk);  
- 
- return TRUE;  
-     }  
- return FALSE;  
- }  
- 
- BOOL CTCPClientDlg::CreateRASLink(TCHAR *ptcRasEntryName,TCHAR *ptcModemName)  
- {  
-     LPRASENTRY lpRasEntry = NULL;  
- DWORD dwRasEntrySize = sizeof(RASENTRY);  
- DWORD dwBufferSize = 0;  
- DWORD dwRet = 0;  
- BOOL bRet = TRUE;  
- 
- if(NULL == ptcRasEntryName || NULL == ptcModemName)  
- return FALSE;  
- 
-     RasGetEntryProperties(NULL,L"",NULL,&dwBufferSize,NULL,NULL);   
- if(0 == dwBufferSize)  
- return FALSE;  
- 
-     lpRasEntry = (LPRASENTRY)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwBufferSize);  
- if(NULL == lpRasEntry)  
- return FALSE;  
- 
-     ZeroMemory(lpRasEntry,sizeof(RASENTRY));  
-     lpRasEntry->dwSize = dwBufferSize;  
-     lpRasEntry->dwfOptions = RASEO_RemoteDefaultGateway;     // RASEO_PreviewUserPw 弹出用户名和密码输入框
- // Leo lpRasEntry->dwType = RASET_Phone;
- // lpRasEntry->dwCountryID = 86; // This member is currently ignored by Windows CE.
-     lpRasEntry->dwCountryCode = 86;  
-     lstrcpy(lpRasEntry->szAreaCode,L"10");  
-     lstrcpy(lpRasEntry->szLocalPhoneNumber,L"#777");  
- 
-     lstrcpy(lpRasEntry->szDeviceType,RASDT_Modem);  
-     lstrcpy(lpRasEntry->szDeviceName,ptcModemName);      // 选用 Modem 的名字
-     lpRasEntry->dwfNetProtocols = RASNP_Ip;  
-     lpRasEntry->dwFramingProtocol = RASFP_Ppp;  
- 
-     dwRet = RasSetEntryProperties(NULL,ptcRasEntryName,lpRasEntry,dwBufferSize,NULL,0);  //  创建连接 
- if(0 != dwRet)  
-     {  
-         bRet = FALSE;  
-     }  
-     HeapFree(GetProcessHeap(),0,(LPVOID)lpRasEntry);  
- 
- return bRet;  
- }  
- 
- BOOL CTCPClientDlg::NetworkDialAndParams(TCHAR *ptcRasEntryName)  
- {  
- DWORD dwRet = 0;  
- UINT i = 0;  
-     RASDIALPARAMS rdParams;  
- 
-     ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));    
- 
-     rdParams.dwSize = sizeof(RASDIALPARAMS);  
-     wcscpy(rdParams.szEntryName,ptcRasEntryName);  
- // wcscpy(rdParams.szPhoneNumber,L"#777");
-     wcscpy(rdParams.szUserName,L"card");  
-     wcscpy(rdParams.szPassword,L"card");  
- 
-     ghRasConn = NULL;  
-     dwRet = RasDial(NULL,NULL,&rdParams,0xFFFFFFFF,NULL,&ghRasConn);  
- if(0 != dwRet)  
-     {  
-         RETAILMSG(1,(L"RasDial Error: %d.\r\n",dwRet));  
- return FALSE;  
-     }  
- return TRUE;  
- }  
- 
//================================
备注::
1、可以参考WINCE下自带的拨号程序----Rnaapp.exe/RasDial.exe-----真正拨号的程序，被connmc.exe（网络跟连接管理）程序调起来。
2、确实不错的东西，这样就不用每次去手动建立连接并去一个个参数设置了！
