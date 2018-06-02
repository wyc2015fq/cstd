
// 屏蔽deprecation警告
#pragma warning(disable: 4996)
#include <winsock2.h>
#include <stdio.h>
#include <MSWSock.h>
#include "cstd.h"
//#include "auto_tchar.h"
#pragma comment(lib,"ws2_32.lib")
#define _T(x)  (x)
#define TRACE printf

// 每一个处理器上产生多少个线程
#define WORKER_THREADS_PER_PROCESSOR 2
// 同时投递的AcceptEx请求的数量
#define MAX_POST_ACCEPT 10
// 传递给Worker线程的退出信号
#define EXIT_CODE NULL
// 释放指针和句柄资源的宏
// 释放指针宏
#define RELEASE(x) {if(x != NULL ){free(x);x=NULL;}}
// 释放句柄宏
#define RELEASE_HANDLE(x) {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
// 释放Socket宏
#define RELEASE_SOCKET(x) {if(x !=INVALID_SOCKET) { closesocket(x);x=INVALID_SOCKET;}}
//缓冲区长度 (1024*8)
#define MAX_BUFFER_LEN    (8*1024)
//默认端口
#define DEFAULT_PORT 12345
//默认IP地址
#define DEFAULT_IP ("127.0.0.1")

#define DEFAULT_THREADS       100                        // 默认并发线程数
#define DEFAULT_MESSAGE       _T("Hello!")   // 默认的发送信息

class CClient;

// 用于发送数据的线程参数
typedef struct _tagThreadParams_WORKER
{
  CClient* pClient;                               // 类指针，用于调用类中的函数
  SOCKET   sock;                                  // 每个线程使用的Socket
  int      nThreadNo;                             // 线程编号
  char     szBuffer[MAX_BUFFER_LEN];
  char	 szBuffer1[MAX_BUFFER_LEN];
  
} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER;  

// 产生Socket连接的线程
typedef struct _tagThreadParams_CONNECTION
{
  CClient* pClient;                               // 类指针，用于调用类中的函数
  
} THREADPARAMS_CONNECTION,*PTHREADPARAM_CONNECTION; 


class CClient
{
public:
  CClient(void);
  ~CClient(void);
  
public:
  
  int Wait() {
    WaitForSingleObject(m_hConnectionThread, -1);
    if (m_phWorkerThreads) {
      WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, 1000000);
    }
    return 0;
  }
  // 加载Socket库
  bool LoadSocketLib();
  // 卸载Socket库，彻底完事
  void UnloadSocketLib() { WSACleanup(); }
  
  // 开始测试
  bool Start();
  //	停止测试
  void Stop();
  
  // 获得本机的IP地址
  const char* GetLocalIP();
  
  // 设置连接IP地址
  void SetIP( const char* strIP ) { strcpy(m_strServerIP, strIP); }
  // 设置监听端口
  void SetPort( const int nPort )   { m_nPort=nPort; }
  // 设置并发线程数量
  void SetThreads(const int& n)      { m_nThreads=n; }
  // 设置要按发送的信息
  void SetMessage( const char* strMessage ) { strcpy(m_strMessage, strMessage); }
  
  // 设置主界面的指针，用于调用其函数
  //void SetMainDlg( CDialog* p )      { m_pMain=p; }
  
  // 在主界面中显示信息
  void ShowMessage( const char* strInfo,...);
  
private:
  
  // 建立连接
  bool EstablishConnections();
  // 向服务器进行连接
  bool ConnetToServer( SOCKET *pSocket, const char* strServer, int nPort );
  // 用于建立连接的线程
  static DWORD WINAPI _ConnectionThread(LPVOID lpParam);
  // 用于发送信息的线程
  static DWORD WINAPI _WorkerThread(LPVOID lpParam);
  
  // 释放资源
  void CleanUp();
  
private:
  
  //CDialog*  m_pMain;                                      // 界面指针
  
  char   m_strServerIP[64];                                // 服务器端的IP地址
  char   m_strLocalIP[64];                                 // 本机IP地址
  char   m_strMessage[64];                                 // 发给服务器的信息
  int       m_nPort;                                      // 监听端口
  int       m_nThreads;                                   // 并发线程数量
  
  HANDLE    *m_phWorkerThreads;
  HANDLE    m_hConnectionThread;                          // 接受连接的线程句柄
  HANDLE    m_hShutdownEvent;                             // 用来通知线程系统退出的事件，为了能够更好的退出线程
  
  THREADPARAMS_WORKER      *m_pParamsWorker;              // 线程参数
};

CClient::CClient(void):
			m_nThreads(DEFAULT_THREADS),
			m_nPort(DEFAULT_PORT),
			m_phWorkerThreads(NULL),
      m_hConnectionThread(NULL),
      m_pParamsWorker(NULL),
			m_hShutdownEvent(NULL)
      {
        strcpy(m_strServerIP, DEFAULT_IP);
        strcpy(m_strLocalIP, DEFAULT_IP);
			strcpy(m_strMessage, DEFAULT_MESSAGE);
}

CClient::~CClient(void)
{
	this->Stop();
}
//////////////////////////////////////////////////////////////////////////////////
//	建立连接的线程
DWORD WINAPI CClient::_ConnectionThread(LPVOID lpParam)
{
	THREADPARAMS_CONNECTION* pParams = (THREADPARAMS_CONNECTION*) lpParam;
	CClient* pClient = (CClient*)pParams->pClient;

	TRACE("_AccpetThread启动，系统监听中...\n");

	pClient->EstablishConnections();

	TRACE(_T("_ConnectionThread线程结束.\n"));

	RELEASE(pParams);	

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// 用于发送信息的线程
DWORD WINAPI CClient::_WorkerThread(LPVOID lpParam)
{
	THREADPARAMS_WORKER *pParams = (THREADPARAMS_WORKER *)lpParam;
	CClient* pClient = (CClient*) pParams->pClient;

	char szTemp[MAX_BUFFER_LEN];
	memset( szTemp,0,sizeof(szTemp) );
	char szRecv[MAX_BUFFER_LEN];
	memset(szRecv,0,MAX_BUFFER_LEN);

	int nBytesSent = 0;
	int nBytesRecv = 0;

	//CopyMemory(szTemp,pParams->szBuffer,sizeof(pParams->szBuffer));

	// 向服务器发送信息
	sprintf( szTemp,("第1条信息：%s"),pParams->szBuffer );
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		TRACE("错误：发送1次信息失败，错误代码：%ld\n", WSAGetLastError());
		return 1; 
	}	
	TRACE("向服务器发送信息成功: %s\n", szTemp);
	pClient->ShowMessage("SEND1 向服务器发送信息成功: %s", szTemp);

	memset(pParams->szBuffer1, 0, 8196);
	memset( szTemp,0,sizeof(szTemp) );
	nBytesRecv = recv(pParams->sock, pParams->szBuffer1, 8196, 0);
	if (SOCKET_ERROR == nBytesRecv) 
	{
		TRACE("错误：接收1次信息失败，错误代码：%ld\n", WSAGetLastError());
		return 1; 
	}
	pParams->szBuffer[nBytesRecv] = 0;
	sprintf( szTemp,("RECV1 第%d号线程，接收第1条信息：\"%s\""), pParams->nThreadNo, pParams->szBuffer1);
	pClient->ShowMessage(szTemp);
	Sleep( 3000 );

	// 再发送一条信息
	memset( szTemp,0,sizeof(szTemp) );
	sprintf( szTemp,("第2条信息：%s"),pParams->szBuffer );
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		TRACE("错误：发送第2次信息失败，错误代码：%ld\n", WSAGetLastError());
		return 1; 
	}	
	
	TRACE("向服务器发送信息成功: %s\n", szTemp);
	pClient->ShowMessage("SEND2 向服务器发送信息成功: %s", szTemp);

	memset(pParams->szBuffer1, 0, 8196);
	memset( szTemp,0,sizeof(szTemp) );
	nBytesRecv = recv(pParams->sock, pParams->szBuffer1, 8196, 0);
	if (SOCKET_ERROR == nBytesRecv) 
	{
		TRACE("错误：接收2次信息失败，错误代码：%ld\n", WSAGetLastError());
		return 1; 
	}
	pParams->szBuffer[nBytesRecv] = 0;
	sprintf( szTemp,("RECV2 第%d号线程，接收第2条信息：\"%s\""), pParams->nThreadNo, pParams->szBuffer1);
	pClient->ShowMessage(szTemp);
	Sleep( 3000 );
	
	// 发第3条信息
	memset( szTemp,0,sizeof(szTemp) );
	sprintf( szTemp,("第3条信息：%s"),pParams->szBuffer );
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		TRACE("错误：发送第3次信息失败，错误代码：%ld\n", WSAGetLastError());
		return 1; 
	}	
	pClient->ShowMessage("SEND3 向服务器发送信息成功: %s", szTemp);
	TRACE("向服务器发送信息成功: %s\n", szTemp);

	memset(pParams->szBuffer1, 0, 8196);
	memset( szTemp,0,sizeof(szTemp) );
	nBytesRecv = recv(pParams->sock, pParams->szBuffer1, 8196, 0);
	if (SOCKET_ERROR == nBytesRecv) 
	{
		TRACE("错误：接收3次信息失败，错误代码：%ld\n", WSAGetLastError());
		return 1; 
	}
	pParams->szBuffer[nBytesRecv] = 0;
	sprintf( szTemp,("RECV3 第%d号线程，接收第3条信息：\"%s\""), pParams->nThreadNo, pParams->szBuffer1);
	pClient->ShowMessage(szTemp);

	if( pParams->nThreadNo==pClient->m_nThreads )
	{
		pClient->ShowMessage(_T("测试并发 %d 个线程完毕."),pClient->m_nThreads);
	}

	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
// 建立连接
bool  CClient::EstablishConnections()
{
	DWORD nThreadID = 0;

	MYREALLOC(m_phWorkerThreads, m_nThreads);
	MYREALLOC(m_pParamsWorker, m_nThreads);

	// 根据用户设置的线程数量，生成每一个线程连接至服务器，并生成线程发送数据
	for (int i=0; i<m_nThreads; i++)
	{
		// 监听用户的停止事件
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hShutdownEvent, 0))
		{
			TRACE(_T("接收到用户停止命令.\n"));
			return true;
		}
		
		// 向服务器进行连接
		if( !this->ConnetToServer(&m_pParamsWorker[i].sock,m_strServerIP,m_nPort) )
		{
			ShowMessage(_T("连接服务器失败！"));
			CleanUp();
			return false;
		}

		m_pParamsWorker[i].nThreadNo = i+1;
		_snprintf(m_pParamsWorker[i].szBuffer, MAX_BUFFER_LEN, "%d号线程 发送数据 %s", i+1, m_strMessage );

		Sleep(10);

		// 如果连接服务器成功，就开始建立工作者线程，向服务器发送指定数据
		m_pParamsWorker[i].pClient = this;
		m_phWorkerThreads[i] = CreateThread(0, 0, _WorkerThread, (void *)(&m_pParamsWorker[i]), 0, &nThreadID);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////
//	向服务器进行Socket连接
bool CClient::ConnetToServer( SOCKET *pSocket, const char* strServer, int nPort )
{
	struct sockaddr_in ServerAddress;
	struct hostent *Server;

	// 生成SOCKET
	*pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == *pSocket) 
	{
		TRACE("错误：初始化Socket失败，错误信息：%d\n", WSAGetLastError());
		return false;
	}

	// 生成地址信息
	Server = gethostbyname(strServer);
	if (Server == NULL) 
	{
		closesocket(*pSocket);
		TRACE("错误：无效的服务器地址.\n");
		return false; 
	}
	
	ZeroMemory((char *) &ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	CopyMemory((char *)&ServerAddress.sin_addr.s_addr, (char *)Server->h_addr, Server->h_length);

	ServerAddress.sin_port = htons(m_nPort);

	// 开始连接服务器
	if (SOCKET_ERROR == connect(*pSocket, (struct sockaddr *)(&ServerAddress),sizeof(ServerAddress))) 
	{
		closesocket(*pSocket);
		TRACE("错误：连接至服务器失败！\n");
		return false; 
	}

	return true;
}

////////////////////////////////////////////////////////////////////
// 初始化WinSock 2.2
bool CClient::LoadSocketLib()
{    
	WSADATA wsaData;
	int nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (NO_ERROR != nResult)
	{
		ShowMessage(_T("初始化WinSock 2.2失败！\n"));
		return false; // 错误
	}

	return true;
}

///////////////////////////////////////////////////////////////////
// 开始监听
bool CClient::Start()
{
	// 建立系统退出的事件通知
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// 启动连接线程
	DWORD nThreadID;
	THREADPARAMS_CONNECTION* pThreadParams = NULL;
  MYREALLOC(pThreadParams, 1);
	pThreadParams->pClient = this;
	m_hConnectionThread = ::CreateThread(0, 0, _ConnectionThread, (void *)pThreadParams, 0, &nThreadID);

	return true;
}

///////////////////////////////////////////////////////////////////////
//	停止监听
void CClient::Stop()
{
	if( m_hShutdownEvent==NULL ) return ;

	SetEvent(m_hShutdownEvent);
	// 等待Connection线程退出
	WaitForSingleObject(m_hConnectionThread, INFINITE);

	// 关闭所有的Socket
	for (int i= 0; i< m_nThreads; i++)
	{
		closesocket(m_pParamsWorker[i].sock);
	}

	// 等待所有的工作者线程退出
	WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

	// 清空资源
	CleanUp();

	TRACE("测试停止.\n");
}

//////////////////////////////////////////////////////////////////////
//	清空资源
void CClient::CleanUp()
{
	if(m_hShutdownEvent==NULL) return;

	RELEASE(m_phWorkerThreads);

	RELEASE_HANDLE(m_hConnectionThread);

	RELEASE(m_pParamsWorker);

	RELEASE_HANDLE(m_hShutdownEvent);
}

////////////////////////////////////////////////////////////////////
// 获得本机的IP地址
const char* CClient::GetLocalIP()
{
	char hostname[MAX_PATH];
	gethostname(hostname,MAX_PATH);                // 获得本机主机名

	struct hostent FAR* lpHostEnt = gethostbyname(hostname);

	if(lpHostEnt == NULL)
	{
		return DEFAULT_IP;
	}

	LPSTR lpAddr = lpHostEnt->h_addr_list[0];      // 取得IP地址列表中的第一个为返回的IP

	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);

	strcpy(m_strLocalIP, inet_ntoa(inAddr) );   // 转化成标准的IP地址形式
	return m_strLocalIP;
}

/////////////////////////////////////////////////////////////////////
// 在主界面中显示信息
void CClient::ShowMessage(const char* strInfo,...)
{
	// 根据传入的参数格式化字符串
	char   strMessage[256];
	va_list   arglist;

	va_start(arglist, strInfo);
	_snprintf(strMessage, 256, strInfo,arglist);
	va_end(arglist);

	// 在主界面中显示
}

#include "IOCPModel.inl"

int test_iocp3_server() {
  CIOCPModel m_IOCP;
  // 初始化Socket库
  if( false==m_IOCP.LoadSocketLib() )
  {
    printf(_T("加载Winsock 2.2失败，服务器端无法运行！"));
    return 0;
	}
  if( false==m_IOCP.Start() )
  {
    printf(_T("服务器启动失败！"));
    return 0;	
  }
  m_IOCP.Stop(1);
  return 0;
}
int test_iocp3_client() {
  CClient m_Client;
  int nPort = DEFAULT_PORT;
  int nThreads = 10000;
  char* strIP = "192.168.1.3";
  const char* strMessage = "1111111111111111111111111111";
  printf("strIP = %s\n",strIP);
  printf("strMessage = %s\n",strMessage);
  
  if( false==m_Client.LoadSocketLib() )
  {
    printf(_T("加载Winsock 2.2失败，服务器端无法运行！"));
    return 0;
	}

  // 给CClient设置参数
  m_Client.SetIP(strIP);
  m_Client.SetPort(nPort);
  m_Client.SetThreads(nThreads);
  m_Client.SetMessage(strMessage);
  
  // 开始
  if( !m_Client.Start() )
  {
    printf(_T("启动失败！\n"));
    return 0;
  }
  
	printf(_T("测试开始\n"));
  m_Client.Wait();
  return 0;
}
