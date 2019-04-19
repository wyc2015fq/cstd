# wince 编写服务程序 - xqhrs232的专栏 - CSDN博客
2014年09月05日 11:54:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：631
原文地址::[http://blog.csdn.net/liguosheng/article/details/6700934](http://blog.csdn.net/liguosheng/article/details/6700934)
相关文章
1、[Wince启动自己程序与services.exe问题](http://blog.chinaunix.net/uid-112998-id-2886019.html) ----[http://blog.chinaunix.net/uid-112998-id-2886019.html](http://blog.chinaunix.net/uid-112998-id-2886019.html)
# Pocket PC 服务
发布日期： 3/9/2005 | 更新日期： 3/9/2005
Victor Sharov and Vassili Philippov
[Spb Software House](http://www.spbsoftwarehouse.com/)
适用于：
Windows Mobile 2003 Software for Pocket PC
Windows Mobile 2003 Second Edition Software for Pocket PC
Microsoft® eMbedded Visual C++®
从 Microsoft 下载中心下载 [Pocket
 PC Services.msi](http://download.microsoft.com/download/0/5/d/05d42cb9-9de3-4680-af15-d7b745f37e3b/Windows%20Mobile%20Developer%20Samples%20-%20Pocket%20PC%20Services.msi)。
**摘要：**本文说明如何创建 Windows Mobile 2003 Software for Pocket PC 服务以及如何将现有后台应用程序形成服务，这些服务作为不同的线程运行在一个进程中。
![*](http://img.microsoft.com/library/gallery/templates/MNP2.Common/images/3squares.gif)
##### 本页内容
|![简介](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_down.gif)|[简介](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#EMC)|
|----|----|
|![利用 MFC 创建服务 DLL](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_down.gif)|[利用 MFC 创建服务 DLL](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#EUD)|
|![不使用 MFC 创建服务 DLL](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_down.gif)|[不使用 MFC 创建服务 DLL](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#EJF)|
|![在注册表中注册服务](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_down.gif)|[在注册表中注册服务](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#E3G)|
|![在 Pocket PC 2000/2002 上使用服务](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_down.gif)|[在 Pocket PC 2000/2002 上使用服务](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#EEBAC)|
|![安装服务](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_down.gif)|[安装服务](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#EOBAC)|
|![调试 Pocket PC 服务](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_down.gif)|[调试 Pocket PC 服务](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#EZCAC)|
|![小结](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_down.gif)|[小结](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#EDDAC)|
|![附录](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_down.gif)|[附录](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#ELDAC)|
## 简介
许多 Pocket PC 应用程序都需要后台进程。例如：
|•|当插入 SD 卡时，显示一个窗口的程序|
|----|----|
|•|永久备份程序|
|•|控制 GPRS 流量的程序|
|•|Web 服务器|
|•|向任务栏添加特殊图标的程序|
目前，几乎所有的 Pocket PC 开发人员都是创建一个可执行文件并将该文件的快捷方式放到 **\Windows\StartUp**中，这样在重启后启动该程序并在后台运行。鉴于 Windows CE 中的进程数量限制为 32，这种方法存在严重的问题。例如，软重启后，XDA II Pocket PC 设备中运行有 27 个进程。如果您安装有 2-3 个需要后台进程的第三方程序，那么您就只有 1-2 个进程来运行程序！有些 Pocket PC 设备即使在硬重启后也有很多进程在运行，如图
 1 所示。
![](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/art/ppc_services01.gif)
**图 1. 硬重启后运行在设备上的进程。**
Microsoft 在 Windows Mobile 2003 中提出了该问题的一种解决方案。Windows Mobile 2003 支持作为不同的线程运行在一个进程中的 DLL 服务。它解决了该问题。本文说明如何为 Windows Mobile for Pocket PC 2003 创建和发布服务。
Pocket PC 服务接口类似于 Pocket PC 驱动程序接口。一项 Pocket PC 服务就是一个导出一组函数的 DLL。*services.exe* 进程加载这些 DLL 并通过调用这其中的一个函数来对它们进行初始化。
本文对于“*将服务用于后台任务*”的 Pocket PC 开发人员很具有吸引力!
![返回页首](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_up.gif)[返回页首](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#top)
## 利用 MFC 创建服务 DLL
为了实现基于 MFC 的服务，需要采取这些步骤：
在 Microsoft® eMbedded Visual C++® 中创建一个新 WCE MFC AppWizard (dll) 项目，如图 2 所示：
![](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/art/ppc_services02.gif)
**图 2. 创建新的 WCE MFC AppWizard (dll) 项目。**
|1.|添加导出函数定义（应当使用自己的前缀，而不用 *MYS*）。*Services.exe* 进程期望这些函数从各个服务 DLL 导出并调用它们来进行初始化并与服务进行通信：extern "C" DWORD PASCAL EXPORT MYS_Close(DWORD dwData){   AFX_MANAGE_STATE(AfxGetStaticModuleState());   return 0;}extern "C" DWORD PASCAL EXPORT MYS_Deinit(DWORD dwData){   AFX_MANAGE_STATE(AfxGetStaticModuleState());   return 0;}extern "C" DWORD PASCAL EXPORT MYS_Init(DWORD dwData){   AFX_MANAGE_STATE(AfxGetStaticModuleState());   return 1;}extern "C" DWORD PASCAL EXPORT MYS_IOControl(  DWORD dwData,  DWORD dwCode,  PBYTE pBufIn,  DWORD dwLenIn,  PBYTE pBufOut,  DWORD dwLenOut,  PDWORD pdwActualOut){   AFX_MANAGE_STATE(AfxGetStaticModuleState());   return 1;}extern "C" DWORD PASCAL EXPORT MYS_Open(  DWORD dwData,  DWORD dwAccess,  DWORD dwShareMode){   AFX_MANAGE_STATE(AfxGetStaticModuleState());   return 0;}extern "C" DWORD PASCAL EXPORT MYS_Read(  DWORD dwData,  LPVOID pBuf,  DWORD dwLen){   AFX_MANAGE_STATE(AfxGetStaticModuleState());   return 0;}extern "C" DWORD PASCAL EXPORT MYS_Seek(  DWORD dwData,  long pos,  DWORD type){   AFX_MANAGE_STATE(AfxGetStaticModuleState());   return 0;}extern "C" DWORD PASCAL EXPORT MYS_Write(  DWORD dwData,  LPCVOID pInBuf,  DWORD dwInLen){   AFX_MANAGE_STATE(AfxGetStaticModuleState());   return 0;}这里，*MYS* 是函数名称前缀，应用程序用它并通过 Services.exe 来调用服务（此处的 MYS 表示 MY Service）。您应当使用您自己的 3-字母前缀来调用 *_Init* 和其他的函数。**注 **由于该 DLL 将会动态地进行链接，因此从该 DLL 导出的任何调用 MFC 的函数都必须要在函数的开头添加 AFX_MANAGE_STATE 宏。在对 MFC 进行任何调用之前，该宏要出现在每个函数中，这非常重要。这意味着，它必须作为函数中的第一条语句出现，甚至要位于任何对象变量的声明之前，因为它们的构造函数可能会对 MFC DLL 进行调用。请参阅 MFC 技术说明 33 和 58，以获取其他详细信息。|
|----|----|
|2.|将这些函数名称添加到项目 .def 文件导出表中（您应当使用您自己的前缀，而不用 MYS）。“新项目向导”自动创建该文件：EXPORTS    ; Explicit exports can go here   MYS_Close   MYS_Deinit   MYS_Init   MYS_IOControl   MYS_Open   MYS_Read   MYS_Seek   MYS_Write|
|3.|向 MYS_Init 函数添加一些特殊的初始化代码。这里，您很可能需要创建一个新线程来封装所有的服务逻辑。为此要定义线程控制函数。该函数正是创建其他窗口、定时器或者任何其他应用程序特殊需要的地方。在该示例中，我们创建了一个定时器并运行线程消息循环。UINT MyControllingFunction( LPVOID pParam ){   theApp.m_nTimer = SetTimer(0, 0, 10 * 1000, MyTimerProc);    MSG msg;   while (GetMessage(&msg, 0, 0, 0))   {      TranslateMessage(&msg);      DispatchMessage(&msg);   }   return 0;}extern "C" DWORD PASCAL EXPORT MYS_Init(DWORD dwData){   AFX_MANAGE_STATE(AfxGetStaticModuleState());   theApp.m_pThread = AfxBeginThread( MyControllingFunction, 0);   return 1;}|
**注**确保 MYS_Init 函数返回非零值，因为零返回值表示服务初始化失败并会引起立即卸载服务 DLL。
根据您的应用程序需要，您可以用同样的方式在其他的 *MYS_* 方法中添加一些逻辑。
![返回页首](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_up.gif)[返回页首](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#top)
## 不使用 MFC 创建服务 DLL
为了实现基于 MFC 的服务，请采取下面的步骤：
在 Microsoft eMbedded Visual C++ 中创建一个新的 WCE 动态链接库项目，如图 3 所示：
![](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/art/ppc_services03.gif)
**图 3. 创建新的 WCE 动态链接库项目。**
|1.|添加导出函数定义（您应当使用您自己的前缀，而不用 *MYS*）。*Services.exe* 进程期望这些函数从各个服务 DLL 导出并调用它们来进行初始化并与服务进行通信：DWORD MYS_Close(DWORD dwData){   return 0;}DWORD MYS_Deinit(DWORD dwData){   return 0;}DWORD MYS_Init(DWORD dwData){   return 1;}DWORD MYS_IOControl(  DWORD dwData,  DWORD dwCode,  PBYTE pBufIn,  DWORD dwLenIn,  PBYTE pBufOut,  DWORD dwLenOut,  PDWORD pdwActualOut){      return 1;}DWORD MYS_Open(  DWORD dwData,  DWORD dwAccess,  DWORD dwShareMode){   return 0;}DWORD MYS_Read(  DWORD dwData,  LPVOID pBuf,  DWORD dwLen){      return 0;}DWORD MYS_Seek(  DWORD dwData,  long pos,  DWORD type){      return 0;}DWORD MYS_Write(  DWORD dwData,  LPCVOID pInBuf,  DWORD dwInLen){      return 0;}**注 **这里，*MYS* 是函数名称前缀，应用程序用它并通过 Services.exe 来调用服务（此处的 MYS 表示 MY Service）。您应当使用您自己的 3-字母前缀来调用 *_Init* 和其他的函数。|
|----|----|
|2.|将这些函数名称添加到项目 .def 文件导出表中（您应当使用您自己的前缀，而不用 MYS）。您必须手动创建用项目命名而扩展名为 .def 的文本文件，并将它添加到项目中：EXPORTS    ; Explicit exports can go here   MYS_Close   MYS_Deinit   MYS_Init   MYS_IOControl   MYS_Open   MYS_Read   MYS_Seek   MYS_Write|
|3.|向 MYS_Init 函数添加一些特殊的初始化代码。这里，您很可能需要创建一个新线程来封装所有的服务逻辑。为此，要定义线程控制函数。该函数正是创建其他窗口、定时器或者任何其他应用程序特殊需要的地方。在该示例中，我们创建了一个定时器并运行线程消息循环。unsigned long __cdecl SSWControllingFunction( LPVOID pParam ){   g_nTimer = SetTimer(0, 0, 10 * 1000, SSWTimerProc);    MSG msg;   while (GetMessage(&msg, 0, 0, 0))   {      TranslateMessage(&msg);      DispatchMessage(&msg);   }   return 0;}DWORD MYS_Init(DWORD dwData){   HANDLE hThread = CreateThread( 0, 0, SSWControllingFunction, 0, 0, 0);   return 1;}|
**注 **确保 MYS_Init 函数返回非零值，因为零返回值表示服务初始化失败并会引起立即卸载服务 DLL。
根据您的应用程序需要，您可以用同样的方式在其他的 *MYS_* 方法中添加一些逻辑。
![返回页首](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_up.gif)[返回页首](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#top)
## 在注册表中注册服务
为了使在系统启动时自动启动服务，您应当向注册表 HKEY_LOCAL_MACHINE\Services\Service 项添加一个唯一命名的子项，并指定下面的值：
*Order : REG_DWORD*
|•|Services.exe 加载各项服务的次序。首先加载次序最低的服务。|
*Dll : REG_SZ*
|•|加载动态链接库 (DLL) 文件。只有文件名而没有路径。该 DLL 应当位于 \Windows 文件夹中。|
*Keep : REG_DWORD*
|•|对于应当在后台运行的服务 Keep 必须为 1，如果 Keep = 0，那么在初始化后将会立即卸载该 DLL。|
*Prefix : REG_SZ*
|•|从服务 DLL 导出函数的前缀（而不是 xxx_Init 中的 xxx，等等）。必须是 3 个符号。|
*Index : REG_SZ*
|•|服务索引（设置为 0）。|
*DisplayName : REG_SZ*
|•|显示服务名称。|
*Description : REG_SZ*
|•|显示服务的说明。|
*Context : REG_DWORD*
|•|传递给初始化例程的初始值（必须为 0）。|
例如，对于基于 MFC 的示例服务，为了在启动时作为服务而启动，应当使用下面的注册表项。图 4 显示了注册表中的其他服务设置。
[HKEY_LOCAL_MACHINE\Services\MyServ]
    "Dll"="MYS.dll"
    "Order"=dword:8
    "Keep"=dword:1
    "Prefix"="MYS"
    "Index"=dword:0
    "Context"=dword:0
    "DisplayName"="Sample MFC Service"
    "Description"="Sample Service demonstratig MFC usage"
![](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/art/ppc_services04.gif)
**图 4. 注册表中的服务设置。**
很可能在安装程序中您会利用 .inf 文件将这些值写到注册表中。下面是 .inf 文件的代码：
HKLM,Services\MyServ,Dll, 0x00000000,MYS.dll
HKLM,Services\MyServ,Prefix, 0x00000000,MYS
HKLM,Services\MyServ,Order, 0x00010001, 8
HKLM,Services\MyServ,Keep, 0x00010001, 1
HKLM,Services\MyServ,Index, 0x00010001, 0
HKLM,Services\MyServ,Context, 0x00010001, 0
HKLM,Services\MyServ,DisplayName, 0x00000000,Sample MFC Service
HKLM,Services\MyServ,Description, 0x00000000,Sample Service demonstratig MFC usage
请参阅 MSDN® [Windows CE .NET
 4.2](http://msdn.microsoft.com/mobility/windowsmobile/howto/documentation/default.aspx?pull=/library/en-us/wceserv/html/cmconSystemStartup.asp)，以获取详细信息。
![返回页首](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_up.gif)[返回页首](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#top)
## 在 Pocket PC 2000/2002 上使用服务
Microsoft services.exe 在 Pocket PC 2000 和 Pocket PC 2002 平台上是不可用的。您可以利用 Pocket PC Developer Network 上的 [Pocket PC 服务](http://www.pocketpcdn.com/articles/services.html)一文中介绍的第三方解决方案来在这些平台上运行您的
 Pocket PC 服务。
![返回页首](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_up.gif)[返回页首](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#top)
## 安装服务
为了安装服务您应当将服务 DLL 复制到 **\Windows** 文件夹中，并从 *Setup.dll* 的 *Install_Exit* 函数启动该服务。切记，在卸载程序（在 *Setup.dll* 的 *Uninstall_Init* 函数中）之前要停止服务。
按步操作：
|1.|将服务 DLL 包含到 .inf 文件中，并将它安装在 *\Windows* 文件夹中|
|----|----|
|2.|在 .inf 文件中添加行，以在注册表中注册服务：（有关更多信息，请参阅“在注册表中注册服务”）[RegSettings.All]HKLM,Services\MyServ,Dll, 0x00000000,MYS.dllHKLM,Services\MyServ,Prefix, 0x00000000,MYSHKLM,Services\MyServ,Order, 0x00010001, 8HKLM,Services\MyServ,Keep, 0x00010001, 1HKLM,Services\MyServ,Index, 0x00010001, 0HKLM,Services\MyServ,Context, 0x00010001, 0HKLM,Services\MyServ,DisplayName, 0x00000000,Sample MFC ServiceHKLM,Services\MyServ,Description, 0x00000000,Sample Service demonstratig MFC usage|
|3.|安装程序后启动服务（请参阅[使用 Setup.dll 中的安装函数，获取创建自定义安装的详细信息](http://msdn.microsoft.com/mobility/windowsmobile/howto/documentation/default.aspx?pull=/library/en-us/wcesetup/html/_wcesdk_Using_Installation_Functions_in_Setupdll.asp)）：typedef HANDLE Proc_ActivateService(LPCWSTR lpszDevKey, DWORD dwClientInfo);â€¦codeINSTALL_EXITInstall_Exit(    HWND    hwndParent,    LPCTSTR pszInstallDir,    WORD    cFailedDirs,    WORD    cFailedFiles,    WORD    cFailedRegKeys,    WORD    cFailedRegVals,    WORD    cFailedShortcuts){   â€_   HANDLE hService;   HINSTANCE hLibrary = ::LoadLibrary(_T("coredll.dll"));         if (NULL == hLibrary)   {      return FALSE;   }         FARPROC pProc = ::GetProcAddress(hLibrary, _T("ActivateService"));      if (NULL == pProc)   {      ::FreeLibrary(hLibrary);      return FALSE;   }         Proc_ActivateService *pProcActivateService = (Proc_ActivateService*)pProc;      hService = pProcActivateService(lpName, 0);   â€¦}Stop your service before uninstalling your program: typedef BOOL Proc_DeregisterService(HANDLE hService);typedef HANDLE Proc_GetServiceHandle(LPWSTR szPrefix, LPWSTR szDllName, DWORD pdwDllBuf);â€¦codeUNINSTALL_INITUninstall_Init(    HWND        hwndParent,    LPCTSTR     pszInstallDir){   â€_   HINSTANCE hLibrary = LoadLibrary(_T("coredll.dll"));      if (NULL == hLibrary)   {      return FALSE;   }      FARPROC pProc = GetProcAddress(hLibrary, _T("DeregisterService"));      if (NULL == pProc)   {      ::FreeLibrary(hLibrary);      return FALSE;   }      Proc_DeregisterService *pProcDeregisterService = (Proc_DeregisterService*)pProc;      pProc = GetProcAddress(hLibrary, _T("GetServiceHandle"));      if (NULL == pProc)   {      ::FreeLibrary(hLibrary);      return FALSE;   }      Proc_GetServiceHandle *pProcGetServiceHandle = (Proc_GetServiceHandle*)pProc;   // Add "0:" to specify instance desired   TCHAR lpFullName[7];   _tcscpy(lpFullName, lpName);   _tcscat(lpFullName, _T("0:"));   DWORD dwDllName = 0;   HANDLE hService = pProcGetServiceHandle(lpFullName, NULL, dwDllName);   bRes = pProcDeregisterService(hService);   â€¦}|
|4.|在安装支持“升级”方案的程序之前，也应当停止服务|
![返回页首](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_up.gif)[返回页首](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#top)
## 调试 Pocket PC 服务
不能将调试器附加到 *services.exe*，因为您不能从 ROM 复制该文件从而获得本地副本（但是如果您拥有 Windows Mobile 2003 *services.exe* 的本地副本，您可以附加到该进程）。然而，您可以使用日志文件（例如 STLogFile 库）来跟踪您的程序。
![返回页首](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_up.gif)[返回页首](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#top)
## 小结
我们强烈推荐将服务用于后台任务，因为 Windows CE 强加了 32 进程的限制。Pocket PC 2000 和 Pocket PC 2002 没有实现 services.exe 进程，但有一种方法也可以在这些设备上使用服务。有关详细信息，请参阅 [Pocket
 PC Developer Network Web 站点](http://www.pocketpcdn.com/articles/services.html)上的 Pocket PC 服务文章。
![返回页首](http://www.microsoft.com/library/gallery/templates/MNP2.Common/images/arrow_px_up.gif)[返回页首](http://www.microsoft.com/china/MSDN/library/Mobility/pocketpc/dnppc2k3ppcservices.mspx#top)
## 附录
**MyService 示例项目**
该 eMbedded Visual C++ 项目可以用作一个编写服务的模板。服务自身不会完成太多的事情，它只是启动一个报警器，大约每 10 秒钟发出一次警告声。*请注意*£¬一般来说移动设备上通过定时器来完成定期操作不是一个好的主意，因为它使处理器处于活动状态，因此大大减少了电池的寿命。
为了运行这种服务，您需要确保具有下面的设置项。一旦您设置了这些条目，您就必须要重启您的设备。
    [HKEY_LOCAL_MACHINE\Services\MyService]
    "Dll"="MyService.dll"
    "Order"=dword:8
    "Keep"=dword:1
    "Prefix"="SRV"
    "Index"=dword:0
    "Context"=dword:0
    "DisplayName"="Sample MFC Service"
"Description"="This sample service makes your device beep about every 10 seconds."
[转到原英文页面](http://msdn.microsoft.com/mobility/windowsmobile/howto/documentation/default.aspx?pull=/library/en-us/dnppc2k3/html/ppc_services.asp)
