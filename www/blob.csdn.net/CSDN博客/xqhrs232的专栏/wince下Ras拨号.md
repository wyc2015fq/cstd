# wince下Ras拨号 - xqhrs232的专栏 - CSDN博客
2013年01月22日 21:04:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：684
原文地址::[http://blog.csdn.net/nolatestudy/article/details/6342392](http://blog.csdn.net/nolatestudy/article/details/6342392)
相关网帖
1、windows下安装sim100 modem----**SIM100 拨号上网设置----[http://www.docin.com/p-96348395.html](http://www.docin.com/p-96348395.html)**
2、SIM300拨号上网与GPRS连接----[http://www.flatws.cn/article/program/embed/2011-04-01/18948.ht](http://www.flatws.cn/article/program/embed/2011-04-01/18948.html)ml
33
利用Ras系列函数再wince系统下进行GPRS拨号，经历了几次修改，但期间太懒没有进行总结，感觉不踏实，今决定回顾代码，能总结多少算多少，然后再慢慢完善此文。
     （1）首先，在wince控制面板创建一个拨号连接，把波特率、串口号、（APN、用户名、密码）等设置后确保能够拨号，然后将注册表信息导出后合成到定制wince系统。接下来就可以在这个系统上做对应的应用程序了。
     （2）检测是否存在设备名为pszDeviceName（如我用的是SIM300W模块，SIMCOM提供驱动，名为SIM300 MUX Modem），类型为modem的设备；
     使用的API：需要调用两次RasEnumDevices来获取Devices列表，然后再比较DeviceName。 
     示例代码：
**[cpp:nogutter]**[view
 plain](http://blog.csdn.net/nolatestudy/article/details/6342392#)[copy](http://blog.csdn.net/nolatestudy/article/details/6342392#)
-  LPRASDEVINFO pRasDevInfo = NULL;  
- DWORD cb = 0;  
- DWORD dwDeviceCount = 0;  
- DWORD dwRetVal = ::RasEnumDevices(NULL, &cb, &dwDeviceCount);  
- 
- if ((0 == dwRetVal||dwRetVal==ERROR_BUFFER_TOO_SMALL) && (0 != cb))  
- {  
-     *pRasDevInfo = reinterpret_cast<LPRASDEVINFO>(newBYTE[cb]);  
-     ASSERT(*pRasDevInfo);  
- 
- if (*pRasDevInfo)  
-     {  
-         (*pRasDevInfo)->dwSize = sizeof(RASDEVINFO);  
-         dwRetVal = ::RasEnumDevices(*pRasDevInfo, &cb, &dwDeviceCount);  
-     }  
- else
-     {  
-         dwDeviceCount = 0;  
-     }  
- }  
- // pRasDevInfo保存了获取的device列表，列表size等于dwDeviceCount。
- // device name比较代码省略
    （3）设置RASENTRY属性
     使用的API：RasGetEntryProperties ；RasSetEntryProperties
     先使用RasGetEntryProperties（需2次调用）获取默认的属性，然后根据具体的情况修改属性值（如：拨号号码）。
     注：需要保存RasGetEntryProperties获取的lpbDeviceInfo**,**供后面使用（如保存为成员变量m_pDevConfig）。
     （4）设置APN（CDMA拨号无需设置APN），我是从网上找的，具体的链接不记得了。
     定义结构体
    typedef struct tagDEVMINICFG {
 WORD wVersion;
 WORD wWaitBong; // DevCfgHdr
 DWORD dwCallSetupFailTimer; // CommConfig.ModemSettings
 DWORD dwModemOptions; // CommConfig.ModemSettings
 // MDM_BLIND_DIAL MDM_FLOWCONTROL_SOFT
 // MDM_CCITT_OVERRIDE MDM_FORCED_EC
 // MDM_CELLULAR MDM_SPEED_ADJUST
 // MDM_COMPRESSION MDM_TONE_DIAL
 // MDM_ERROR_CONTROL MDM_V23_OVERRIDE
 // MDM_FLOWCONTROL_HARD
 DWORD dwBaudRate; // DCB
 WORD fwOptions; // DevCfgHdr
 // TERMINAL_PRE TERMINAL_POST
 // MANUAL_DIAL
 BYTE ByteSize; // DCB
 BYTE StopBits; // DCB
 BYTE Parity; // DCB
 BYTE Rerver;
 WCHAR szDialModifier[DIAL_MODIFIER_LEN+1]; // Unique to MiniCfg
 // Dynamic devices configuration
 WCHAR wszDriverName[MAX_NAME_LENGTH+1];
 BYTE pConfigBlob[MAX_CFG_BLOB];
 HANDLE hPort;
} DEVMINICFG, *PDEVMINICFG;
        设置代码段如下：
 TCHAR szATCmd[80]={0};
 _stprintf(szATCmd,_T("+CGDCONT=1,/"IP/",/"%s/""),pszAPN);
 PDEVMINICFG pDev=(PDEVMINICFG)m_pDevConfig;
 pDev->dwBaudRate=dwBauti;         // 波特率
 pDev->dwModemOptions&=~dwFlowControl ; // 硬件流控或软件流控
 int dwDialOff=offsetof(DEVMINICFG,szDialModifier);
#ifdef _WIN32_WCE
 memcpy(m_pDevConfig+dwDialOff,szATCmd, 80*sizeof(TCHAR));
#endif
 if (m_pszAPN)
 {
  delete []m_pszAPN;
  m_pszAPN = NULL;
 }
 m_pszAPN = new TCHAR[_tcslen(pszAPN)+1];
 _tcscpy(m_pszAPN, pszAPN);
    （5）连网
     使用的API：RasDial
     分为同步连网和异步连网，一般都是选择异步。
     先设置 RASDIALPARAMS rdParams;
     同步连网： DWORD dwRet =  RasDial(NULL, NULL, &rdParams, 0, NULL, &m_hRasConn);
     异步连网： DWORD dwRet = RasDial(NULL, NULL, &rdParams, 0xffffffff, m_hEventWnd, &m_hRasConn);   
     对RasDial返回值进行判断是否成功执行，如果不成功且m_hRasConn不为NULL，需要调用RasHangUp释放m_hRasConn。
    （6）断网
     使用的API：RasHangUp
     调用RasHangUp后，最好是能够判断下是否释放完成。
     如：
  DWORD dwRet=RasHangUp(m_hRasConn);
 DWORD dwStatusRet=0;
 RASCONNSTATUS rStatus;
 ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
 rStatus.dwSize = sizeof(RASCONNSTATUS);
 dwStatusRet = RasGetConnectStatus(m_hRasConn, &rStatus);
 long lStart=GetTickCount();
 while(dwStatusRet!=ERROR_INVALID_HANDLE)
 {
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
   TranslateMessage(&msg);
   DispatchMessage(&msg);
  }
  dwStatusRet = RasGetConnectStatus(m_hRasConn, &rStatus);
  if (GetTickCount()-lStart>3000)
  {
   break;
  }
  //Sleep(10);
  //dwStatusRet = RasGetConnectStatus(m_hRasConn, &rStatus);
  //if (GetTickCount()-lStart>5000)
  //{
  // break;
  //}
 }
 CloseHandle(m_hRasConn);
 m_hRasConn=NULL;
    （7） 获取当前活动连接的HRASCONN
     使用API：RasEnumConnections
    （8） HRASCONN的释放
    断网时必须得把此资源完全释放，否则会引起拨号不成功。这点在重连网络的时候要特别留意。
    （9）开发中发现，如果正在连网，需要取消，调用 RasHangUp, 有时出现RasHangUp函数阻塞不返回了，总结经验是：当连网过程进入到状态RASCS_DeviceConnected后，没有出现RasHangUp不返回的现象。我不知道Ras里面的机制细节，谁知道的劳烦告诉我，在此先谢谢了！
