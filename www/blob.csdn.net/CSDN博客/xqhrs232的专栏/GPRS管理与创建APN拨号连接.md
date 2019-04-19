# GPRS管理与创建APN拨号连接 - xqhrs232的专栏 - CSDN博客
2013年01月09日 22:24:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：757
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://blog.csdn.net/huang5556019/article/details/7794830](http://blog.csdn.net/huang5556019/article/details/7794830)
转自：[http://www.cnblogs.com/michael-zhangyu/archive/2009/07/04/1516797.html](http://www.cnblogs.com/michael-zhangyu/archive/2009/07/04/1516797.html)
本文主要介绍一些GPRS管理与创建APN拨号连接相关的知识点。新建拨号连接利用RAS实现，本文将详细介绍我在实现创建APN拨号连接时的一些经验，目前来说创建APN连接的资源非常少，尤其是c#实现的很少。GPRS管理主要是Connection Manager API，目前的资源比较丰富，我就不重复造轮子，提供一些参考资料以供参考。
一Connection Manager API
  Connection Manager(连接管理器，为一系列API的集合) 系列API的主要目的是为了集中管理基于Windows Mobile系列的设备网络连接的建立与维护。移动应用程序使用 连接管理器API去建立一个网络连接。
  参考资料：
       Windows Mobile上的GPRS连接(基于TcpClient)
[http://www.cnblogs.com/fox23/archive/2008/08/15/Establish-a-GPRS-connection-with-TcpClient.html](http://www.cnblogs.com/fox23/archive/2008/08/15/Establish-a-GPRS-connection-with-TcpClient.html)
       GPRS开发系列文章之入门篇
[http://www.cnblogs.com/jsjkandy/archive/2008/08/04/1260490.html](http://www.cnblogs.com/jsjkandy/archive/2008/08/04/1260490.html)
       GPRS开发系列文章之进阶篇
[http://www.cnblogs.com/jsjkandy/archive/2008/08/06/1262445.html](http://www.cnblogs.com/jsjkandy/archive/2008/08/06/1262445.html)
      【重点推荐】Windows Mobile 下使用C#进行GPRS、CDMA开发
[http://blog.csdn.net/zhyuanshan/archive/2009/02/26/3940125.aspx](http://blog.csdn.net/zhyuanshan/archive/2009/02/26/3940125.aspx)
 如下图：演示了Connection Manager API的基本功能(Connection Manager API DEMO与RAS DEMO下载在本文后面一起下载)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/zhyuanshan/EntryImages/20090226/7.gif)
二 RAS
      RAS（Remote Access Service），即“远程访问服务”，它允许用户从远程地点将自己的计算机连接到一个本地计算机网络。一旦建立了连接，就可以像与局域网中的计算机一样进行通讯，即使计算机实际连接的是一个远程网络。PS: RAS资料真的是相当少，在网上搜索了不少建立拨号连接的资料，基本不能用（大多是建立了连接不能拨号），关于如何输入APN指令更少。在测试新建拨号连接时，最好能准备一台真机测试，否则有些地方会测试不到（比如拨号）。演示DMEO如下图：
![RASManagerDemo](http://images.cnblogs.com/cnblogs_com/michael-zhangyu/WindowsLiveWriter/GPRS_102F5/RASManagerDemo_thumb.jpg)
   1.新建拨号连接，新建APN连接
    创建的拨号连接的操作，其实是创建一个Phone-Book Entries，并将其存入注册表中。
（1）相关API
Thisfunction changes the connection information for an entry in the phone book
or creates a new phone-book entry.
DWORD RasSetEntryProperties(
  LPWSTR[lpszPhoneBook](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPWSTR[szEntry](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPRASENTRY[lpbEntry](http://blog.csdn.net/huang5556019/article/details/7794830), 
  DWORD[dwEntrySize](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPBYTE[lpb](http://blog.csdn.net/huang5556019/article/details/7794830), 
  DWORD[dwSize](http://blog.csdn.net/huang5556019/article/details/7794830));RasSetEntryProperties新建一个拨号连接或修改已存在的拨号连接。如果想先判断是否存在已知的RAS Entry,可以调用RasValidateEntryName。lpbEntry是一个RASENTRY指针，包含：连接选项、国家码、区域码、本地电话号，可以指定本地IP地址、远程DNS服务器IP、远程WINS服务器IP等，还可以指定连接控制脚本文件路径、采用的协议、调制解调器类型及名称等；lpb:非常重要。包含一个指定设备配置的缓冲区，设置APN指令会在这里设置。
（2）注册表信息
RasSetEntryProperties建立的拨号连接将保存在注册表中，如下图：
![reg4-RASBook](http://images.cnblogs.com/cnblogs_com/michael-zhangyu/WindowsLiveWriter/GPRS_102F5/reg4-RASBook_thumb.jpg)
上图中，HKEY_CURRENT_USER\COMM\RASBOOK\ 为所有的RAS连接，（用RasEnumEntries函数枚举得到的集合就是RASBOOK下的集合);其中，Entry即lpbEntry参数(RASENTRY结构体）；DevCfg为lpb参数设置的设备设置信息；利用RasSetEntryProperties建立了拨号连接后，我们会发现在WM机器上（设置->连接->连接->管理现有连接）中找不到已建立的连接。可以通过注册表操作，使新建的拨号连接可以显示在机器的“管理现有连接”中。如下图：[](http://images.cnblogs.com/cnblogs_com/michael-zhangyu/WindowsLiveWriter/GPRS_102F5/conn_setting.jpg) 如上图：机器中的”管理现有连接“对应注册表中[Connections]下的集合；（我们可以手工在机器上添加一个拨号连接，然后参考注册表中的设置，写入注册表）。经过对比与测试，发现：[Enabled]:是否为默认连接；[RequirePW]是否需要用户名与密码；[DestId]：表示属于哪种类型的连接；[ConnectionGUID]：表示连接的标识ID，经过我搜索整个注册表，没有发现其它地方有这个值，因此，可以直接生成一个GUID，为其赋值；那么[DestId]的值到底是什么呢？我们看看下面这个图，会更清楚。我们会发现“测试连接2”\[DestId]的值与下图中"Internet 设置"\[DestId]的值相同，意义不言而喻了吧。[](http://images.cnblogs.com/cnblogs_com/michael-zhangyu/WindowsLiveWriter/GPRS_102F5/internet_1.jpg)
（3） c#代码测试代码：创建一个"cmnet"的拨号连接，如果是创建APN连接，则替换"cmnet",比如:"szxxxxxxgd.gd"
//新连接
private void button1_Click(object sender, EventArgs e)
{
    if (RASManager.CreateRasEntry("测试连接2", "cmnet"))
    {
        listBox1.Items.Add("测试连接2");
        button7.Enabled = true;
    }
}RasManager:/// <summary>
/// 创建拨号连接
/// </summary>
/// <param name="entryName">连接名称</param>
/// <param name="apn">apn,访问站点，如:cmnet</param>
public static bool CreateRasEntry(string entryName, string apn)
{
    RASENTRY rasEntry = new RASENTRY();
    //RASDIALPARAMS RasDialParams = new RASDIALPARAMS(); ;
    int dwSize = Marshal.SizeOf(rasEntry);
    byte[] lpb = new byte[436];
    int lpbSize = lpb.Length;
    int ret = -1;
    //初始化
    rasEntry.dwSize = dwSize;
    //测试
    //ret = RasGetEntryProperties(null, "我的连接 2", ref rasEntry, ref dwSize, lpb, ref lpbSize);
    //对应WM中的选择调制解调器与调制解调器类型
    rasEntry.szDeviceName = "Cellular Line";//蜂窝电话线路(GPRS)
    rasEntry.szDeviceType = "modem";
    //
    rasEntry.szLocalPhoneNumber = string.Format("~GPRS!{0}", apn);
    rasEntry.dwfOptions = 4194304;
    rasEntry.dwfNetProtocols = 4;
    rasEntry.dwFramingProtocol = 1;
    //设置lpb
    //Pointer to a buffer that contains device-specific configuration information. 
    //This is opaque TAPI device configuration information
    lpb[0] = 180;
    lpb[1] = 1;
    lpb[4] = 180;
    lpb[5] = 1;
    lpb[8] = 180;
    lpb[9] = 1;
    lpb[12] = 1;
    lpb[16] = 15;
    lpb[20] = 1;
    lpb[24] = 2;
    lpb[76] = 1;
    lpb[80] = 2;
    lpb[84] = 2;
    //从D88位开始设置APN指令
    for (int i = 0, step = 0; i < apn.Length; i++, step += 2)
    {
        lpb[88 + step] = (byte)apn[i];
    }
    lpb[344] = 1;
    lpb[348] = 1;
    ret = RasSetEntryProperties(null, entryName, ref rasEntry,
                    rasEntry.dwSize, lpb, lpbSize);
    if (ret != 0)
    {
        return false;
    }
    //设置拨号用户名与密码。暂时不需要
    //dwSize = Marshal.SizeOf(RasDialParams);
    //RasDialParams.size = dwSize;
    ////RasDialParams.dwSize = sizeof(RasDialParams);
    //RasDialParams.entryName = "";
    //RasDialParams.callbackNumber = "";
    //RasDialParams.userName = "";
    //RasDialParams.password = "";
    //ret = RasSetEntryDialParams("", ref RasDialParams, false);
    //if (ret >= 1)
    //{
    //    return false;
    //}
    RegistryKey reg = Registry.LocalMachine;
    reg = reg.CreateSubKey("Comm\\ConnMgr\\Providers\\{7C4B7A38-5FF7-4bc1-80F6-5DA7870BB1AA}");
    reg = reg.CreateSubKey("Connections");
    RegistryKey MASCA = reg.CreateSubKey(entryName);
    reg = reg.CreateSubKey(entryName);
    reg.SetValue("EntryType", 2, RegistryValueKind.DWord);
    reg.SetValue("Enabled", 1, RegistryValueKind.DWord);    //是否为默认连接
    reg.SetValue("RequirePW", 0, RegistryValueKind.DWord);  //是否需要密码
    //Internet设置{ADB0B001-10B5-3F39-27C6-9742E785FCD4}
    //WAP网络{7022E968-5A97-4051-BC1C-C578E2FBA5D9}
    //安全WAP网络{F28D1F74-72BE-4394-A4A7-4E296219390C}
    //单位设置{18AD9FBD-F716-ACB6-FD8A-1965DB95B814}
    //默认Internet设置{436EF144-B4FB-4863-A041-8F905A62C572}
    //默认单位设置{A1182988-0D73-439E-87AD-2A5B369F808B}
    //中国移动Internet设置{BAE45F01-FD2D-4E80-B025-1F3B5DADC270}
    //中国移动WAP设置{F462B82D-7E08-4AEA-B4BD-BAF97B106EA0}
    reg.SetValue("DestId", "{ADB0B001-10B5-3F39-27C6-9742E785FCD4}", RegistryValueKind.String);
    reg.SetValue("ConnectionGUID", NewGUID(), RegistryValueKind.String);
    reg.Close();
    return true;
}我的思路：先手工在机器上创建一个需要的拨号连接，然后通过如下代码，获取连接设置后，找到规律，调用RasSetEntryProperties创建新的连接;ret = RasGetEntryProperties(null, "我的连接 2", ref rasEntry, ref dwSize, lpb, ref lpbSize);
(4)遗留问题 通过以上新建拨号连接的代码新建的拨号连接（比如：测试连接2），在WM机器中“管理现有连接”->右键->“连接”，无法拨号；但是通过IE浏览页面时，又能自动拨号成功；所以这个问题并不影响使用；当我们连接WebService或HttpRequest时，会自动按默认连接拨号； 另一问题：我用C++实现的新建连接，逻辑与上面的C#代码相同（实际上，我是先用C++做的测试，然后翻译为C#)，在WM机器中“管理现有连接”->右键->“连接”，可以成功拨号；真是奇怪，希望哪个达人能解释一下：附上我的C++代码：
// 创建RAS连接
void CMFCDEMODlg::OnBnClickedButton4()
{
    CreateRasEntry(L"测试连接2",L"cmnet");
}int CMFCDEMODlg:: CreateRasEntry (LPWSTR lpszName,LPWSTR lpszAPN)
{
    DWORD dwSize,
    dwError;
    TCHAR szError[100];
    BYTE  lpb[436];
    unsigned long lpbSize = sizeof(lpb);
    RASENTRY RasEntry;
    //RASDIALPARAMS  RasDailParams;
    /*
    // Validate the format of a connection entry name.
    if (dwError = RasValidateEntryName (NULL, lpszName))
    {
        wsprintf (szError, TEXT("Unable to validate entry name.")
        TEXT(" Error %ld"), dwError);
        return FALSE;
    }
    */
    // Initialize the RASENTRY structure.
    dwSize = sizeof (RASENTRY);
    memset (&RasEntry, 0, dwSize);
    memset(lpb,0,lpbSize);
    RasEntry.dwSize = dwSize;
    
    //// Retrieve the entry properties.
    //if (dwError = RasGetEntryProperties (NULL,L"",
    //                                    &RasEntry, &dwSize, lpb, &lpbSize))
    //{
    //    wsprintf(szError, TEXT("Unable to read default entry properties.")
    //                TEXT(" Error %ld"), dwError);
    //    return FALSE;
    //}
    //对应WM中的选择调制解调器与调制解调器类型
    wcscpy(RasEntry.szDeviceName,L"Cellular Line");//蜂?窝?电?话?线?路?(GPRS)
    wcscpy(RasEntry.szDeviceType,L"modem");
    //
    TCHAR szLocalPhoneNumber[RAS_MaxPhoneNumber + 1] = {0};
    wsprintf(szLocalPhoneNumber,L"~GPRS!%s",lpszAPN);
    wcscpy(RasEntry.szLocalPhoneNumber,szLocalPhoneNumber );
    RasEntry.dwfOptions=4194304;
    RasEntry.dwfNetProtocols=4;
    RasEntry.dwFramingProtocol=1;
    //设置lpb
    //Pointer to a buffer that contains device-specific configuration information. 
    //This is opaque TAPI device configuration information
    lpb[0]=180;
    lpb[1]=1;
    lpb[4]=180;
    lpb[5]=1;
    lpb[8]=180;
    lpb[9]=1;
    lpb[12]=1;
    lpb[16]=15;
    lpb[20]=1;
    lpb[24]=2;
    lpb[76]=1;
    lpb[80]=2;
    lpb[84]=2;
    
    //从D88位开始设置APN指令
    for(int i=0,step=0;i<wcslen(lpszAPN);i++,step+=2)
    {
        *(lpb + 88 + step)=(BYTE)*(lpszAPN+i);
    }
    lpb[344]=1;
    lpb[348]=1;
    // Create a new phone-book entry.
    if (dwError = RasSetEntryProperties (NULL, lpszName,
                        &RasEntry, sizeof (RASENTRY),lpb, lpbSize))
    {
        wsprintf (szError, TEXT("Unable to create the phonebook entry.")
        TEXT(" Error %ld"), dwError);
        return FALSE;
    }
    //// 设置访问用户名、密码；对应WM中设置中的用户名与密码
    //memset (&RasDialParams, 0, sizeof (RASDIALPARAMS));
    //RasDialParams.dwSize = sizeof (RASDIALPARAMS);
    //wcscpy (RasDialParams.szEntryName, lpszName);
    //// Insert code here to fill up the RASDIALPARAMS structure.
    //wcscpy(RasDialParams.szCallbackNumber, _T(""));//callback option
    ////disabled
    //wcscpy (RasDialParams.szUserName, _T("")); //This is
    ////optional
    //wcscpy (RasDialParams.szPassword, _T("")); //This is optional
    //// Change the connection data.
    //if (dwError = RasSetEntryDialParams (NULL, &RasDialParams, FALSE))
    //{
    //    return FALSE;
    //}
    TCHAR buffer[256]={0};
    LPWSTR key=buffer;
    wcscat(key,RIL_KEY);
    wcscat(key,L"\\");
    wcscat(key,lpszName);
    unsigned long val1 = 0;
    unsigned long val2 = 1;
    unsigned long val3 = 2;
    SetRegistryDWORD(HKEY_LOCAL_MACHINE, key, RIL_REQ_PE, val1);    //是否需要密码
    SetRegistryDWORD(HKEY_LOCAL_MACHINE, key, RIL_ENABLED, val2);    //是否为默认连接
    SetRegistryDWORD(HKEY_LOCAL_MACHINE, key, RIL_ENT_TYPE, val3);
    //Internet设置{ADB0B001-10B5-3F39-27C6-9742E785FCD4}
    //WAP网络{7022E968-5A97-4051-BC1C-C578E2FBA5D9}
    //安全WAP网络{F28D1F74-72BE-4394-A4A7-4E296219390C}
    //单位设置{18AD9FBD-F716-ACB6-FD8A-1965DB95B814}
    //默认Internet设置{436EF144-B4FB-4863-A041-8F905A62C572}
    //默认单位设置{A1182988-0D73-439E-87AD-2A5B369F808B}
    //中国移动Internet设置{BAE45F01-FD2D-4E80-B025-1F3B5DADC270}
    //中国移动WAP设置{F462B82D-7E08-4AEA-B4BD-BAF97B106EA0}
    SetRegistrySZ(HKEY_LOCAL_MACHINE, key, RIL_DES_ID,
                _T("{ADB0B001-10B5-3F39-27C6-9742E785FCD4}"));    //一般是Internet设置;
    //ConnectionGUID，可以生成一个GUID;
    SetRegistrySZ(HKEY_LOCAL_MACHINE, key, RIL_CONN_ID,
                NewGUID()); 
    return TRUE;
}
LPCWSTR CMFCDEMODlg::NewGUID()
{
    static TCHAR buf[38] = {0};
    GUID guid;
    if (S_OK == ::CoCreateGuid(&guid))
    {
      _snwprintf(buf, sizeof(buf)
               , L"{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"
               , guid.Data1
               , guid.Data2
               , guid.Data3
               , guid.Data4[0], guid.Data4[1]
               , guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
               , guid.Data4[6], guid.Data4[7]);
    }
    return (LPCWSTR)buf;
}
// Registry get and set helper-function implementations
bool CMFCDEMODlg::GetRegistryDWORD(const HKEY hRoot, const TCHAR* const psKeyName,
                                const TCHAR* const psValueName, DWORD *const pdwValue)
{
    bool bSuccess = false;
    HKEY hKey;
    DWORD dwDWORD;
    DWORD dwDisposition;
    if((0 != pdwValue) && (ERROR_SUCCESS == RegCreateKeyEx(hRoot,
                                psKeyName, 0, TEXT(""), 0, KEY_READ, 0, &hKey, &dwDisposition)))
    {
        DWORD dwValueSize = sizeof(dwDWORD);
        bSuccess = ((ERROR_SUCCESS == RegQueryValueEx(hKey, psValueName, 0,
                        0, (BYTE*)(&dwDWORD), &dwValueSize)) && (sizeof(dwDWORD) ==
        dwValueSize));
        RegCloseKey(hKey);
        if(bSuccess)
        {
            *pdwValue = dwDWORD;
        }
    }
    return bSuccess;
}
bool CMFCDEMODlg::SetRegistryDWORD(const HKEY hRoot, const TCHAR* const psKeyName,
                        const TCHAR* const psValueName, const DWORD dwValue)
{
    bool bSuccess = false;
    HKEY hKey;
    DWORD dwDisposition;
    if(ERROR_SUCCESS == RegCreateKeyEx(hRoot, psKeyName, 0, TEXT(""), 0,
            KEY_WRITE, 0, &hKey, &dwDisposition))
    {
        bSuccess = (ERROR_SUCCESS == RegSetValueEx(hKey, psValueName, 0,
                    REG_DWORD, (BYTE*)(&dwValue), sizeof(dwValue)));
                    RegCloseKey(hKey);
    }
    return bSuccess;
}
bool CMFCDEMODlg::SetRegistrySZ(const HKEY hRoot, const TCHAR* const psKeyName,
        const TCHAR* const psValueName, const TCHAR *const psValue)
{
    bool bSuccess = false;
    HKEY hKey;
    DWORD dwDisposition;
    if((0 != psValue) && (ERROR_SUCCESS == RegCreateKeyEx(hRoot,
            psKeyName, 0, TEXT(""), 0, KEY_WRITE, 0, &hKey, &dwDisposition)))
    {
        bSuccess = (ERROR_SUCCESS == RegSetValueEx(hKey, psValueName, 0,
        REG_SZ, (const BYTE* const)psValue, (_tcslen(psValue)
        +1)*sizeof(TCHAR)));
        RegCloseKey(hKey);
    }
    return bSuccess;
}
2.拨号，建立连接
(1)相关API
This function establishes a RAS connection between a RAS client and a RAS server. The connection data includes callback and user authentication information.
DWORD RasDial(
  LPRASDIALEXTENSIONS[dialExtensions](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPTSTR[phoneBookPath](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPRASDIALPARAMS[rasDialParam](http://blog.csdn.net/huang5556019/article/details/7794830), 
  DWORD[NotifierType](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPVOID[notifier](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPHRASCONN[pRasConn](http://blog.csdn.net/huang5556019/article/details/7794830));
(2)实现代码
请注意：利用RasDial实现拨号时，不会在标题栏上显示"正在拨号...."等等相关状态变化和提示；但是可以通过Socket测试是否已经连接，或者通过枚举所有活动的RAS连接可以看到；//测试拨号
private void button7_Click(object sender, EventArgs e)
{
    if (listBox1.SelectedIndex >= 0)
    {
        try
        {
            RASManager.DialUp(listBox1.Text, null, null);
            listBox1.Items.Add("拨号成功.测试连接www.g.cn");
            TcpClient tcpClient = new TcpClient(AddressFamily.InterNetwork);
            tcpClient.Connect("www.g.cn", 80);
            NetworkStream ns = tcpClient.GetStream();
            byte[] buf = new byte[128];
            ns.Write(buf, 0, 128);
            tcpClient.Client.Shutdown(SocketShutdown.Both);
            ns.Close();
            tcpClient.Close();
            listBox1.Items.Add("连接成功,Wrote 128 bytes");
        }
        catch (Exception ex)
        {
            listBox1.Items.Add(ex.Message);
        }
    }
}/// <summary>
/// 拨号连接
/// </summary>
/// <param name="pEntryName"></param>
/// <param name="pUserName"></param>
/// <param name="pPassword"></param>
public static void DialUp(string pEntryName, string pUserName, string pPassword)
{
    //check the connection state first
    if (IntPtr.Zero != m_pRasSession)
    {
        RASCONNSTATE resConnState = (RASCONNSTATE)GetStatus();
        if (RASCONNSTATE.RASCS_Connected == resConnState ||
            RASCONNSTATE.RASCS_PortOpened == resConnState ||
            RASCONNSTATE.RASCS_DeviceConnected == resConnState ||
            RASCONNSTATE.RASCS_AllDevicesConnected == resConnState ||
            RASCONNSTATE.RASCS_Authenticate == resConnState ||
            RASCONNSTATE.RASCS_AuthAck == resConnState ||
            RASCONNSTATE.RASCS_Authenticated == resConnState)
        {
            return;
        }
    }
    RASDIALPARAMS rasDialParams = new RASDIALPARAMS();
    rasDialParams.size = Marshal.SizeOf(rasDialParams);
    rasDialParams.entryName = pEntryName;
    rasDialParams.password = pPassword;
    int lResult = RasDial(IntPtr.Zero, IntPtr.Zero, RASDIALPARAMS.ToBytes(rasDialParams),            0xFFFFFFFF, IntPtr.Zero, ref m_pRasSession);
    if (lResult != 0)
    {
        throw new Exception(string.Format("RasDial failed: Error {0}", lResult));
    }
}(3)疑难问题在实现DialUp方法时，碰到一个问题，RasDial函数第3个参数：LPRASDIALPARAMS[rasDialParam](http://blog.csdn.net/huang5556019/article/details/7794830)，表示一个RASDIALPARAMS的指针。
在声明RasDial函数时：一开始我声明如下:[DllImport("coredll.dll",
 CharSet = CharSet.Auto)] private static extern int RasDial(IntPtrpDialExtensions, IntPtr pPhonebook, ref RASDIALPARAMS pRasDialParam, uint pNotifierType, IntPtr pHwnd, ref IntPtr pRasConn);
但是始终拨号不成功，测试了好久，没办法，只好按结构体顺序生成byte[]，传入一个byte[]才算成功，并把ref RASDIALPARAMS pRasDialParam改为byte[] pRasDialParam；具体代码请下载源码查看；
3.终止拨号，断开连接
  (1)相关API
This function terminates a remote access connection. The connection is specified with a RAS connection handle. RasHangUpreleases all RASAPI32.DLL resources associated with the handle.
DWORD RasHangUp(
  HRASCONN[Session](http://blog.csdn.net/huang5556019/article/details/7794830));
###### Parameters
- Session
Handle to the remote access connection to terminate. This is a handle returned from a previous call to [RasDial](http://blog.csdn.net/huang5556019/article/details/7794830) or [RasEnumConnections](http://blog.csdn.net/huang5556019/article/details/7794830).
###### Return Values
Zero indicates success. A nonzero error value listed in the RAS header file or ERROR_INVALID_HANDLE indicates failure.
Include Raserror.h for definitions of the RAS error codes.
###### Remarks
The connection is terminated even if the RasDialcall has not yet been completed.
After this call, the Sessionhandle can no longer be used.
（2）实现代码
//断开连接
private void button8_Click(object sender, EventArgs e)
{
    Connection.RASManager.RASCONN[] conns = RASManager.GetRASConnections();
    if (conns !=null && conns.Length > 0)
    {
        foreach (RASManager.RASCONN conn in conns)
        {
            try
            {
                RASManager.HangUp(conn.hRasConnHandle);
                listBox1.Items.Add(conn.entryName + " 断开成功!");
            }
            catch (Exception ex)
            {
                listBox1.Items.Add(ex.Message);
            }
        }
    }
}/// <summary>
/// 断开连接
/// </summary>
/// <param name="hSession"></param>
public static void HangUp(IntPtr hSession)
{
    int lStatus = RasHangUp(hSession);
    if (lStatus != 0)
    {
        throw new Exception(string.Format("RasHangUp failed: Error {0}", lStatus));
    }
}
4.枚举所有活动的（已连接）RAS Connection
(1)相关API
This function lists all active RAS connections. It returns each connection's handle and phone book entry name.
DWORD RasEnumConnections(
  LPRASCONN[lprasconn](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPDWORD[lpcb](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPDWORD[lpcConnections](http://blog.csdn.net/huang5556019/article/details/7794830));
（2）C#实现代码
//枚举所有活动的RAS连接
private void button3_Click(object sender, EventArgs e)
{
    Connection.RASManager.RASCONN[] conns = RASManager.GetRASConnections();
    listBox1.Items.Clear();
    if (conns != null && conns.Length > 0)
    {
        listBox1.BeginUpdate();
        foreach (RASManager.RASCONN conn in conns)
        {
            listBox1.Items.Add(conn.entryName);
        }
        listBox1.EndUpdate();
    }
}/// <summary>
/// 枚举所有的活动的RAS连接
/// </summary>
/// <returns></returns>
public static RASCONN[] GetRASConnections()
{
    RASCONN[] retval = null;
    int size = Marshal.SizeOf(typeof(RASCONN));
    int cb = size;
    int iConnections = 0;
    IntPtr pConnections = IntPtr.Zero;
    bool retry = false;
    do
    {
        try
        {
            if (pConnections != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pConnections);
            }
            pConnections = Marshal.AllocHGlobal(cb);
            Marshal.Copy(BitConverter.GetBytes(size), 0, pConnections, 4); //pConnections->size = size;
            int ret = RasEnumConnections(pConnections, ref cb, ref iConnections);
            if (ret != 0)  //ERROR_BUFFER_TOO_SMALL
            {
                retry = true;
            }
            else if(ret==0)
            {
                retry = false;
                if (iConnections > 0)
                {
                    retval = CreateArrayOfType<RASCONN>(pConnections, size, iConnections);
                }
            } 
            else
            {
                throw new Exception(string.Format("RasEnumConnections failed: Error {0}", ret));
            }
        }
        finally
        {
            if (pConnections != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pConnections);
            }
        }
    } while (retry);
    return retval;
}
(3) C++实现代码
//列举所有的活动RAS连接
void CMFCDEMODlg::OnBnClickedButton3()
{
    DWORD dwCb = sizeof(RASCONN);
    DWORD dwErr = ERROR_SUCCESS;
    DWORD dwRetries = 5;
    DWORD dwConnections = 0;
    LPRASCONN lpRasConn = NULL;
    //
    // Loop through in case the information from RAS changes between calls.
    //
    while (dwRetries--)
    {
        //
        // If the memory is allocated, free it.
        //
        if (NULL != lpRasConn)
        {
            LocalFree(lpRasConn);
            lpRasConn = NULL;
        }
        //
        // Allocate the size needed for the RAS structure.
        //
        lpRasConn = (LPRASCONN)LocalAlloc(LPTR,dwCb);
        if (NULL == lpRasConn)
        {
            dwErr = ERROR_NOT_ENOUGH_MEMORY;
            break;
        }
        //
        // Set the structure size for version checking purposes.
        //
        lpRasConn->dwSize = sizeof(RASCONN);
        //
        // Call the RAS API then exit the loop if we are successful or an unknown
        // error occurs.
        //
        dwErr = RasEnumConnections(
                    lpRasConn,
                    &dwCb,
                    &dwConnections);
        if (ERROR_BUFFER_TOO_SMALL != dwErr)
        {
            break;
        }
    }
    //
    // In the success case, print the names of the connections.
    //
    if (ERROR_SUCCESS == dwErr)
    {
        DWORD i;
        printf("The following RAS connections are currently active\n\n");
        for (i = 0; i < dwConnections; i++)
        {
            AfxMessageBox(lpRasConn[i].szEntryName);//测试
            printf("%s\n", lpRasConn[i].szEntryName);
        }
    }
    else
    {
        printf("RasEnumConnections failed: Error = %d\n", dwErr);
    }
    //
    // Free the memory if necessary.
    //
    if (NULL != lpRasConn)
    {
        HeapFree(GetProcessHeap(), 0, lpRasConn);
        lpRasConn = NULL;
    }
}
5.枚举所有可用的RAS 连接
(1)相关API
This function lists all entry names in a remote access phone book.
DWORD RasEnumEntries(
  LPWSTR[Reserved](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPWSTR[lpszPhoneBookPath](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPRASENTRYNAME[lprasentryname](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPDWORD[lpcb](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPDWORD[lpcEntries](http://blog.csdn.net/huang5556019/article/details/7794830));(2)C#实现代码//枚举所有RAS连接
private void button2_Click(object sender, EventArgs e)
{
    RASManager.RASENTRYNAME[] entries = RASManager.GetRASEntries();
    listBox1.Items.Clear();
    if (entries !=null && entries.Length > 0)
    {
        listBox1.BeginUpdate();
        foreach (RASManager.RASENTRYNAME entry in entries)
        {
            listBox1.Items.Add(entry.szEntryName);
        }
        listBox1.EndUpdate();
        button7.Enabled = true;
    }
}/// <summary>
/// 枚举RAS
/// </summary>
/// <returns></returns>
public static RASENTRYNAME[] GetRASEntries()
{
    RASENTRYNAME[] result = null;
    int size = Marshal.SizeOf(typeof(RASENTRYNAME));
    int Cb = size;//new IntPtr(size);
    int cEntries = 0; //IntPtr.Zero;
    IntPtr pEntries = IntPtr.Zero;
    try
    {
        pEntries = Marshal.AllocHGlobal(Cb);
        Marshal.Copy(BitConverter.GetBytes(size), 0, pEntries, 4);//pEntries->dwSize = size;
        uint ret = RasEnumEntries(IntPtr.Zero, IntPtr.Zero, pEntries, ref Cb, ref cEntries);
        if (ret != 0) //ERROR_BUFFER_TOO_SMALL;ERROR_NOT_ENOUGH_MEMORY 
        {
            if (pEntries != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pEntries);
            }
            pEntries = Marshal.AllocHGlobal(Cb);
            Marshal.Copy(BitConverter.GetBytes(size), 0, pEntries, 4);//pEntries->dwSize = size;
        }
        ret = RasEnumEntries(IntPtr.Zero, IntPtr.Zero, pEntries, ref Cb, ref cEntries);
        if (ret == 0) //SUCCESS
        {
            if (cEntries > 0)
            {
                result = CreateArrayOfType<RASENTRYNAME>(pEntries, size, cEntries);
            }
        }
        else
        {
            throw new Exception(string.Format("RasEnumEntries failed: Error {0}", ret));
        }
    }
    finally
    {
        if (pEntries != IntPtr.Zero)
        {
            Marshal.FreeHGlobal(pEntries);
        }
    }
    return result;
}
(3)C++代码
// 列举所有的RAS
void CMFCDEMODlg::OnBnClickedButton2()
{
    LPRASENTRYNAME lprasentryname={0};
    lprasentryname = (LPRASENTRYNAME)LocalAlloc(LPTR, sizeof(RASENTRYNAME));
    lprasentryname->dwSize = sizeof(RASENTRYNAME);
    DWORD nRet=-1; 
    DWORD cb=0;
    DWORD cEntries=0;
    if ((nRet = RasEnumEntries(NULL, NULL, lprasentryname, &cb, &cEntries)) 
        == ERROR_BUFFER_TOO_SMALL) //ERROR_BUFFER_TOO_SMALL
    {
        if(lprasentryname)
        {
            ::LocalFree(lprasentryname);
            lprasentryname=NULL;
        }
        lprasentryname = (LPRASENTRYNAME)LocalAlloc(LPTR, cb);
        lprasentryname->dwSize = sizeof(RASENTRYNAME);
    }
    // Calling RasEnumEntries to enumerate the phonebook entries   
    nRet = RasEnumEntries(NULL, NULL, lprasentryname, &cb, &cEntries);
    if (nRet != ERROR_SUCCESS)
    {
        printf("RasEnumEntries failed: Error %d\n", nRet);
    }
    else
    {
        printf("Phone book entries in the default phonebook:\n\n");
        for(int i=0;i < cEntries;i++)
        {
            printf("%S\r\n",lprasentryname->szEntryName);
            lprasentryname++;
        } 
    }
    if(lprasentryname)
    {
        ::LocalFree(lprasentryname);
        lprasentryname=NULL;
    }
}
6.枚举所有的RAS设备
(1)相关API
This function returns the name and type of all available RAS-capable devices.
DWORD RasEnumDevices( 
  LPRASDEVINFOW [lpRasDevinfo](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPDWORD [lpcb](http://blog.csdn.net/huang5556019/article/details/7794830), 
  LPDWORD [lpcDevices](http://blog.csdn.net/huang5556019/article/details/7794830));(2)C#实现代码//枚举RAS设备
private void button4_Click(object sender, EventArgs e)
{
    Connection.RASManager.RASDEVINFO[] devices = RASManager.GetRASDevices();
    listBox1.Items.Clear();
    if (devices !=null && devices.Length > 0)
    {
        listBox1.BeginUpdate();
        foreach (RASManager.RASDEVINFO device in devices)
        {
            listBox1.Items.Add(device.name + "  " + device.type);
        }
        listBox1.EndUpdate();
    }
}/// <summary>
/// 枚举所有的可用的连接设备
/// </summary>
/// <returns></returns>
public static RASDEVINFO[] GetRASDevices()
{
    RASDEVINFO[] retval = null;
    int size = Marshal.SizeOf(typeof(RASDEVINFO));
    int lpCb = size;
    int lpcDevices = 0;
    bool retry = false;
    do
    {
        IntPtr pDevices = IntPtr.Zero;
        try
        {
            if (pDevices != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pDevices);
            }
            pDevices = Marshal.AllocHGlobal(lpCb);
            Marshal.Copy(BitConverter.GetBytes(size), 0, pDevices, 4);
            int ret = RasEnumDevices(pDevices, ref lpCb, ref lpcDevices);
            if (ret !=0)
            {
                retry = true;
            }
            else if (ret ==0)
            {
                retry = false;
                retval = CreateArrayOfType<RASDEVINFO>(pDevices, size, lpcDevices);
            }
            else
            {
                throw new Exception(string.Format("RasEnumDevices failed: Error {0}", ret));
            }
        }
        finally
        {
            if (pDevices != IntPtr.Zero)
            {
                Marshal.FreeHGlobal(pDevices);
            }
        }
    }
    while (retry);
    return retval;
}
(3)C++实现代码
/枚举所有RAS设备
void CMFCDEMODlg::OnBnClickedButton5()
{
    DWORD dwCb = sizeof(RASDEVINFO);
    DWORD dwErr = ERROR_SUCCESS;
    DWORD dwRetries = 5;
    DWORD dwDevices = 0;
    RASDEVINFO* lpRasDevInfo = NULL;
    //
    // Loop through in case the information from RAS changes between calls.
    //
    while (dwRetries--)
    {
        //
        // If the memory is allocated, free it.
        //
        if (NULL != lpRasDevInfo)
        {
            HeapFree(GetProcessHeap(), 0, lpRasDevInfo);
            lpRasDevInfo = NULL;
        }
        //
        // Allocate the size need for the RAS structure.
        //
        lpRasDevInfo =(RASDEVINFO*) HeapAlloc(GetProcessHeap(), 0, dwCb);
        if (NULL == lpRasDevInfo)
        {
            dwErr = ERROR_NOT_ENOUGH_MEMORY;
            break;
        }
        //
        // Set the structure size for version checking purposes.
        //
        lpRasDevInfo->dwSize = sizeof(RASDEVINFO);
        //
        // Call the RAS API, bail on the loop if we are successful or an unknown
        // error occurs.
        //
        dwErr = RasEnumDevices(
                    lpRasDevInfo,
                    &dwCb,
                    &dwDevices);
        if (ERROR_BUFFER_TOO_SMALL != dwErr)
        {
            break;
        }
    }
    //
    // In the success case print the names of the devices.
    //
    if (ERROR_SUCCESS == dwErr)
    {
        DWORD i;
        printf("The following RAS capable devices were found on this machine:\n\n");
        for (i = 0; i < dwDevices; i++)
        {
            printf("%S\n", lpRasDevInfo[i].szDeviceName);
        }
    }
    else
    {
        printf("RasEnumDevices failed: Error = %d\n", dwErr);
    }
    //
    // Free the memory if necessary.
    //
    if (NULL != lpRasDevInfo)
    {
        HeapFree(GetProcessHeap(), 0, lpRasDevInfo);
        lpRasDevInfo = NULL;
    }
}
  好了，到此RAS介绍完毕；之所以有2套代码，因为基本都是先用C++实现后，再翻译到C#去的。
C#调用P/Invoke实在很痛苦，声明常量，结构体，函数声明等等，实在太麻烦，仅仅声明相关结构体与常量就花了近一天时间，调试过程碰到好几次，因为结构体长度对应不上而出错，又到C++ 头文件中一个一个对比。
