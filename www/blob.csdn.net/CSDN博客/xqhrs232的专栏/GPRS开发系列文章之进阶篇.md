# GPRS开发系列文章之进阶篇 - xqhrs232的专栏 - CSDN博客
2013年01月09日 22:38:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：509
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.cnblogs.com/jsjkandy/archive/2008/08/06/1262445.html](http://www.cnblogs.com/jsjkandy/archive/2008/08/06/1262445.html)
**一、前言**
在前篇《**GPRS开发系列文章之入门篇**》里，我主要对GPRS开发中遇到的一些常用概念和一些业务逻辑做了简单的介绍，没想到得到了很多网友的支持和关注，因昨天有事因此延迟到今天才奉上这篇进阶系列文章，还请各位见谅。希望大家支持同时欢迎拍砖，共同提高。
在最后一篇《**GPRS开发系列文章之实战篇**》我将详细叙述如何利用类库开发Client和Server端通信程序，因此本文的多数讲解将是为下文服务的。本文将向您介绍基于PPC2003的Windows
 mobile 系列的客户端和基于.Net 2005的服务器端进行开发所需掌握和了解的开发库，并着重围绕客户端进行GPRS连接所用到的一些API函数进行讲解。本文的最后是一些用于引用和学习用的链接和利用API建立GPRS连接的demo，感兴趣的同志可以去点击或下载后进行深入研究。
关于客户端API的叙述我基本上都是翻译过来的，如欠妥还请各位多多斧正！同时demo程序我也是只做了部分加工，主要都是参照了[谢红伟](mailto:chrys@163.com)的发表的文章（后面有引用链接）
**二、进阶系列篇详解**
**1.       客户端建立GPRS连接 API**
客户端开发采用 EVC4.0进行开发，主要讲解的类库为Connection Manager 系列API，客户端在进行GPRS拨号连接时将使用下文所介绍的API。
Connection Manager(本人译为连接管理器，此对象为一系列API的集合) 系列API的主要目的是为了集中管理基于Windows
 Mobile系列的设备网络连接的建立与维护。移动应用程序使用连接管理器API去建立或规划一个网络连接，而连接管理器则掌控连接过程中的所有状态信息.应用程序在要发起一个连接(比如Internet)，只要简单的告知连接管理器就OK了。
当一个应用程序发起一个网络连接的请求时，连接管理器首先从连接服务提供商（CSPS）处获取所有可能的连接信息，然后连接管理器会从这一系列连接信息中根据开销，延迟、带宽等因素来选择一个最佳的连接，最后连接管理器将被请求的连接排入队列，然后在一个适合的时间使用CSPS来建立连接。
【API函数】：
a)       ConnMgrApiReadyEvent（）函数
函数原型：HANDLE WINAPI ConnMgrApiReadyEvent();
利用此函数来我们可以返回一个连接事件的句柄，注意在的得到句柄后要记得及时释放
b)       **ConnMgrConnectionStatus（）**函数
函数原型：
HRESULT WINAPI ConnMgrConnectionStatus(HANDLE hConnection,DWORD * pdwStatus );利用此函数的返回值pdwStatus，我们可以得到很多的关于连接的信息，例如如果我们连接成功将返回CONNMGR_STATUS_CONNECTED，断开连接将返回CONNMGR_STATUS_DISCONNECTED,他的返回状态信息非常丰富，有14中之多，完全可以满足我们的应用需要；
c)**ConnMgrEnumDestinations()**函数
函数原型：
HRESULT WINAPI ConnMgrEnumDestinations(int nIndex,CONNMGR_DESTINATION_INFO * pDestInfo )；
一般我们的PDA在连接GPRS时都有好几个连接，利用此函数我们可以枚举出所有可用的连接，然后再对挑选的连接进行筛选得到一个最佳连接
接下来讲两个很重要的函数，我们将利用两个函数中的一个来发起连接，他们是：
d)       **ConnMgrEstablishConnection()和ConnMgrEstablishConnectionSync()**函数，这两个函数一个用于发起一个异步连接请求，一个用于同步请求，使用异步连接请求我们可以在发起连接后立即返回，而使用同步请求客户端将一直被阻塞知道函数返回确认连接，关于异步和同步我就不再叙述；
它们的原型依次为：
HRESULT WINAPI ConnMgrEstablishConnection( CONNMGR_CONNECTIONINFO * pConnInfo, HANDLE * phConnection);
HRESULT WINAPI ConnMgrEstablishConnectionSync(   CONNMGR_CONNECTIONINFO * pConnInfo, HANDLE * phConnection,    DWORD dwTimeout, DWORD * pdwStatus );
可以看到两个函数的第一个参数都为一个CONNMGR_CONNECTIONINFO对象，此对象为一个结构体，它保存了客户端发起连接请求的一系列信息，因此，在我们调用此函数时必须构造一个该对象，然后将其作为参数传入连接函数中。这里很有必要讲一下该结构体，该结构体的原型如下：
typedef struct _CONNMGR_CONNECTIONINFO 
{ 
DWORD cbSize;   DWORD dwParams;DWORD dwFlags;
DWORD dwPriority;BOOL bExclusive;BOOL bDisabled;GUID guidDestNet;
HWND hWnd;  UINT uMsg; LPARAM lParam;ULONG ulMaxCost;  ULONG ulMinRcvBw;   
      ULONG ulMaxConnLatency;
  } CONNMGR_CONNECTIONINFO;
其中参数dwFlags用于指定我们的接入点，比如我们常说的CMNER和CMWAP，而参数GUID则标志了我们对应于每个接入点的全球唯一标志符，关于如何得到或者查看GUID，我们可以在“\Program
 Files\Windows CE Tools\wce420\POCKET PC 2003\Include\Armv4”目录下查看connmgr.h文件，里面包含了各个接入点的GUID，例如：
**CMNET **为：(0x436ef144,
 0xb4fb, 0x4863, 0xa0, 0x41, 0x8f, 0x90, 0x5a, 0x62, 0xc5, 0x72)
**CMWAP**为：0x7022e968,
 0x5a97, 0x4051, 0xbc, 0x1c, 0xc5, 0x78, 0xe2, 0xfb, 0xa5, 0xd9
如果想更进一步了解，我们还可以通过查看注册表方式来查看PDA上连接管理器的相关连接信息，在PPC 2003中注册表路径为：
[HKEY_LOCAL_MACHINE\Comm\ConnMgr]，如下图所示：
![](http://images.cnblogs.com/cnblogs_com/jsjkandy/GPS/register.JPG)
在Destinations一项中就对应我们所有可用的网络连接，这个跟用ConnMgrEnumDestinations()方法得到的是一样的效果，在默认Internet设置中我们将看到CMNET的GUID，如下所示：
![](http://images.cnblogs.com/cnblogs_com/jsjkandy/GPRS/destNet.JPG)
在这里里面有DestId一项，就对应着我们久违的GUID
e)       ConnMgrReleaseConnection函数
我们在上一步中建立连接后我们将得到一个连接句柄，在重新开始一个新的连接或者断开连接都要调用此函数来释放掉之前创建的连接，它的原型为：
HRESULT WINAPI ConnMgrReleaseConnection( HANDLE hConnection,BOOL bCache );
【连接管理API大致使用步骤】：
首先我们利用ConnMgrApiReadyEvent()函数来确认是否有可用连接，如果有可用连接我们则利用ConnMgrEnumDestinations()函数枚举所有可用连接，然后遍历所有连接调用我们的同步或异步连接方法ConnMgrEstablishConnectionSync()与ConnMgrEstablishConnection()来发起连接，一旦连接成功后我们就可以进行我们伟大的下一步了，就是和我们的服务器进行通信。
【GPRS demo效果图】
![](http://images.cnblogs.com/cnblogs_com/jsjkandy/GPRS/demoGPRS.JPG)
【GPRSDemo介绍】
GPRSDemo主要利用了上述的几个重要的API函数来获取当前可用连接，并自动选择一个最佳的连接途径，然后启用这个连接，在连接启动成功以后再用socket 进行网络连接，与公网服务器进行通信。
首先检查是否有可用连接
[]()
BOOL CConnectManager::GetConnMgrAvailable()
{
    HANDLE hConnMgr = ConnMgrApiReadyEvent ();
    BOOL bAvailbale = FALSE;
    DWORD dwResult = ::WaitForSingleObject ( hConnMgr, 2000 );
if ( dwResult == WAIT_OBJECT_0 )
    {
        bAvailbale = TRUE;
    }
// 关闭
if ( hConnMgr ) CloseHandle ( hConnMgr );
return bAvailbale;
}
[]()
                 然后枚举所有可用连接：
[]()
void CConnectManager::EnumNetIdentifier ( OUT CStringArray &StrAry )
{
    CONNMGR_DESTINATION_INFO networkDestInfo = {0};
// 得到网络列表
for ( DWORD dwEnumIndex=0; ; dwEnumIndex++ )
    {
        memset ( &networkDestInfo, 0, sizeof(CONNMGR_DESTINATION_INFO) );
if ( ConnMgrEnumDestinations ( dwEnumIndex, &networkDestInfo ) == E_FAIL )
        {
break;
        }
        StrAry.Add ( networkDestInfo.szDescription );
    }
}
[]()
接下来找到“Internet”这个连接，可用远程URL映射的方式来完成，这样可以让系统自动选取一个最好的连接。
[]()
int CConnectManager::MapURLAndGUID ( LPCTSTR lpszURL, OUT GUID &guidNetworkObject, OUT CString *pcsDesc/*=NULL*/ )
{
if ( !lpszURL || lstrlen(lpszURL) <1 )
return FALSE;
    memset ( &guidNetworkObject, 0, sizeof(GUID) );
int nIndex =0;
    HRESULT hResult = ConnMgrMapURL ( lpszURL, &guidNetworkObject, (DWORD*)&nIndex );
if ( FAILED(hResult) )
    {
        nIndex =-1;
        DWORD dwLastError = GetLastError ();
        AfxMessageBox ( _T("Could not map a request to a network identifier") );
    }
else
    {
if ( pcsDesc )
        {
            CONNMGR_DESTINATION_INFO DestInfo = {0};
if ( SUCCEEDED(ConnMgrEnumDestinations(nIndex, &DestInfo)) )
            {
*pcsDesc = DestInfo.szDescription;
            }
        }
    }
return nIndex;
}
[]()
最后启用指定编号的连接并检查连接状态
[]()
BOOL CConnectManager::EstablishConnection ( DWORD dwIndex )
{
// 释放之前的连接
    ReleaseConnection ();
// 得到正确的连接信息
    CONNMGR_DESTINATION_INFO DestInfo = {0};
    HRESULT hResult = ConnMgrEnumDestinations(dwIndex, &DestInfo);
    BOOL bRet = FALSE;
if(SUCCEEDED(hResult))
    {
// 初始化连接结构
        CONNMGR_CONNECTIONINFO ConnInfo;
        ZeroMemory(&ConnInfo, sizeof(ConnInfo));
        ConnInfo.cbSize =sizeof(ConnInfo);
        ConnInfo.dwParams = CONNMGR_PARAM_GUIDDESTNET;
        ConnInfo.dwFlags = CONNMGR_FLAG_PROXY_HTTP | CONNMGR_FLAG_PROXY_WAP | CONNMGR_FLAG_PROXY_SOCKS4 | CONNMGR_FLAG_PROXY_SOCKS5;
        ConnInfo.dwPriority = CONNMGR_PRIORITY_USERINTERACTIVE;
        ConnInfo.guidDestNet = DestInfo.guid;
        ConnInfo.bExclusive    = FALSE; 
        ConnInfo.bDisabled = FALSE;
        DWORD dwStatus =0;
        hResult = ConnMgrEstablishConnectionSync(&ConnInfo, &m_hConnection, 10*1000, &dwStatus );
if(FAILED(hResult))
        {
            m_hConnection = NULL;
        }
else bRet = TRUE;
    }
return bRet;
}
[]()
检测连接状态
[]()
BOOL CConnectManager::WaitForConnected ( int nTimeoutSec, DWORD *pdwStatus/*=NULL*/ )
{
    DWORD dwStartTime = GetTickCount ();
    BOOL bRet = FALSE;
while ( GetTickCount ()-dwStartTime < (DWORD)nTimeoutSec * 1000 )
    {
if ( m_hConnection )
        {
            DWORD dwStatus = 0;
            HRESULT hr = ConnMgrConnectionStatus ( m_hConnection, &dwStatus );
if ( pdwStatus ) *pdwStatus = dwStatus;
if ( SUCCEEDED(hr) )
            {
if ( dwStatus == CONNMGR_STATUS_CONNECTED )
                {
                    bRet = TRUE;
break;
                }
            }
        }
        Sleep ( 100 );
    }
return bRet;
}
[]()
                   最后要记得释放连接
[]()
void CConnectManager::ReleaseConnection ()
{
if ( m_hConnection )
    {
        ConnMgrReleaseConnection(m_hConnection, FALSE);
        m_hConnection = NULL;
    }
}
[]()
2.       **客户端与服务器端进行socket通信API**socket通信相关开发API在Winsock2.h.文件中定义，因为SOCKET通信不是本文的重点但是又是必须要涉及的
a)       **WSAStartup**函数。在应用程序进行Windows Sockets通信时，必须首先调用此函数来指定应用程序要加载的Windows
 Scoket版本等信息，应用程序结束前我们应该调用WSACleanup去释放掉所用的系统资源
b)       **Connect**函数。此函数用来建立连接
c)        **Select **函数。建立连接后，用来侦听是否有数据传输
d)       **Send**函数。用于给服务器发送消息
3.       **服务器端与客户端进行socket通信**服务器端主要涉及到的库为：
**System.Net****，System.Net.Sockets，System.IO;**System.Net 命名空间为当前网络上使用的多种协议提供了简单的编程接口，System.Net.Sockets 命名空间为需要严密控制网络访问的开发人员提供了 Windows
 Sockets (Winsock) 接口的托管实现。
System.IO 命名空间包含允许读写文件和数据流的类型以及提供基本文件和目录支持的类型。
需要了解的技术有：多线程，事件与委托，SOCKET通信等
**三、           引用（参考）文章地址**
**1.[http://msdn.microsoft.com/en-us/library/ms827527.aspx](http://msdn.microsoft.com/en-us/library/ms827527.aspx)****（MSDN移动开发中心）；**
**2.[http://www.codeguru.com/cpp/w-p/ce/pocketpc/article.php/c7355/](http://www.codeguru.com/cpp/w-p/ce/pocketpc/article.php/c7355/)****（PPC应用程序Internet连接）；**
3.[http://www.vckbase.com/document/viewdoc/?id=1803](http://www.vckbase.com/document/viewdoc/?id=1803)（如何建立GPRS连接）
**4.[http://www.microsoft.com/china/msdn/library/NetFramework/netcompactframework/understandingnetcfFAQ.mspx?mfr=true](http://www.microsoft.com/china/msdn/library/NetFramework/netcompactframework/understandingnetcfFAQ.mspx?mfr=true)****（常见开发问题解答）**
**5.PPC SDK****参考（安装 ppc 2003 SDK后才有）**
**6.    [GPRS Demo下载](http://files.cnblogs.com/jsjkandy/GPRSDemo.rar)**
**作者**：[彭立云](http://www.cnblogs.com/hanchan)
**出处**：[http://www.cnblogs.com/hanchan](http://www.cnblogs.com/hanchan)
本文版权归作者和博客园共有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文连接，否则保留追究法律责任的权利。
