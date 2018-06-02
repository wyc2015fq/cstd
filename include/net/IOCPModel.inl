
////////////////////////////////////////////////////////////////
//BOOL WINAPI GetQueuedCompletionStatus(HANDLE CompletionPort, LPDWORD lpNumberOfBytes, PULONG_PTR lpCompletionKey, LPOVERLAPPED *lpOverlapped, DWORD dwMilliseconds);
//lpCompletionKey [out] 对应于PER_SOCKET_CONTEXT结构，调用CreateIoCompletionPort绑定套接字到完成端口时传入；
//A pointer to a variable that receives the completion key value associated with the file handle whose I/O operation has completed.
//A completion key is a per-file key that is specified in a call to CreateIoCompletionPort.
//lpOverlapped [out] 对应于PER_IO_CONTEXT结构，如：进行accept操作时，调用AcceptEx函数时传入；
//A pointer to a variable that receives the address of the OVERLAPPED structure that was specified when the completed I/O operation was started.
/////////////////////////////////////////////////////////////////
// 在完成端口上投递的I/O操作的类型
typedef enum OPERATION_TYPE {
  ACCEPT_POSTED, // 标志投递的Accept操作
  SEND_POSTED, // 标志投递的是发送操作
  RECV_POSTED, // 标志投递的是接收操作
  NULL_POSTED // 用于初始化，无意义
} OPERATION_TYPE;
//每次套接字操作(如：AcceptEx, WSARecv, WSASend等)对应的数据结构：OVERLAPPED结构(标识本次操作)，关联的套接字，缓冲区，操作类型；
typedef struct PER_IO_CONTEXT {
  OVERLAPPED m_Overlapped; // 每一个重叠网络操作的重叠结构(针对每一个Socket的每一个操作，都要有一个)
  SOCKET m_sockAccept; // 这个网络操作所使用的Socket
  WSABUF m_wsaBuf; // WSA类型的缓冲区，用于给重叠操作传参数的
  char m_szBuffer[MAX_BUFFER_LEN]; // 这个是WSABUF里具体存字符的缓冲区
  OPERATION_TYPE m_OpType; // 标识网络操作的类型(对应上面的枚举)
  DWORD m_nTotalBytes; //数据总的字节数
  DWORD m_nSendBytes; //已经发送的字节数，如未发送数据则设置为0
} PER_IO_CONTEXT;

//构造函数
void PER_IO_CONTEXT_init(PER_IO_CONTEXT* s) {
  ZeroMemory(&s->m_Overlapped, sizeof(s->m_Overlapped));
  ZeroMemory(s->m_szBuffer, MAX_BUFFER_LEN);
  s->m_sockAccept = INVALID_SOCKET;
  s->m_wsaBuf.buf = s->m_szBuffer;
  s->m_wsaBuf.len = MAX_BUFFER_LEN;
  s->m_OpType = NULL_POSTED;
  s->m_nTotalBytes = 0;
  s->m_nSendBytes = 0;
}
//析构函数
void PER_IO_CONTEXT_free(PER_IO_CONTEXT* s) {
  if (s->m_sockAccept != INVALID_SOCKET) {
    closesocket(s->m_sockAccept);
    s->m_sockAccept = INVALID_SOCKET;
  }
}
//重置缓冲区内容
void PER_IO_CONTEXT_ResetBuffer(PER_IO_CONTEXT* s) {
  ZeroMemory(s->m_szBuffer, MAX_BUFFER_LEN);
  s->m_wsaBuf.buf = s->m_szBuffer;
  s->m_wsaBuf.len = MAX_BUFFER_LEN;
}

//每个SOCKET对应的数据结构(调用GetQueuedCompletionStatus传入)：-
//SOCKET，该SOCKET对应的客户端地址，作用在该SOCKET操作集合(对应结构PER_IO_CONTEXT)；
typedef struct PER_SOCKET_CONTEXT {
  SOCKET m_Socket; //连接客户端的socket
  SOCKADDR_IN m_ClientAddr; //客户端地址
  PER_IO_CONTEXT** m_arrayIoContext; //套接字操作，本例是WSARecv和WSASend共用一个PER_IO_CONTEXT
  int m_arrayIoContext_Count;
} PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;

//构造函数
void PER_SOCKET_CONTEXT_init(PER_SOCKET_CONTEXT* s) {
  s->m_Socket = INVALID_SOCKET;
  memset(&s->m_ClientAddr, 0, sizeof(s->m_ClientAddr));
  s->m_arrayIoContext_Count = 0;
  s->m_arrayIoContext = NULL;
}
//析构函数
void PER_SOCKET_CONTEXT_free(PER_SOCKET_CONTEXT* s) {
  if (s->m_Socket != INVALID_SOCKET) {
    closesocket(s->m_Socket);
    s->m_Socket = INVALID_SOCKET;
  }
  // 释放掉所有的IO上下文数据
  for (int i = 0; i < s->m_arrayIoContext_Count; i++) {
    PER_IO_CONTEXT* pContext = s->m_arrayIoContext[i];
    PER_IO_CONTEXT_free(pContext);
    free(pContext);
  }
  free(s->m_arrayIoContext);
  s->m_arrayIoContext = NULL;
  s->m_arrayIoContext_Count = 0;
}
//进行套接字操作时，调用此函数返回PER_IO_CONTEX结构
PER_IO_CONTEXT* PER_SOCKET_CONTEXT_GetNewIoContext(PER_SOCKET_CONTEXT* s) {
  PER_IO_CONTEXT* p = NULL;
  MYREALLOC(p, 1);
  PER_IO_CONTEXT_init(p);
  ++s->m_arrayIoContext_Count;
  MYREALLOC(s->m_arrayIoContext, s->m_arrayIoContext_Count);
  s->m_arrayIoContext[s->m_arrayIoContext_Count - 1] = p;
  return p;
}
// 从数组中移除一个指定的IoContext
void PER_SOCKET_CONTEXT_RemoveContext(PER_SOCKET_CONTEXT* s, PER_IO_CONTEXT* pContext) {
  ASSERT(pContext != NULL);
  for (int i = 0; i < s->m_arrayIoContext_Count; i++) {
    if (pContext == s->m_arrayIoContext[i]) {
      PER_IO_CONTEXT_free(pContext);
      FREE(pContext);
      pContext = NULL;
      memdel(s->m_arrayIoContext, s->m_arrayIoContext_Count, i, 1, sizeof(void*));
      s->m_arrayIoContext_Count--;
      break;
    }
  }
}
// 工作者线程的线程参数
class CIOCPModel;
typedef struct IOCPTHREADPARAMS_WORKER {
  CIOCPModel* pIOCPModel; //类指针，用于调用类中的函数
  int nThreadNo; //线程编号
} IOCPTHREADPARAMS_WORKER;
// CIOCPModel类
class CIOCPModel
{
public:
  CIOCPModel(void);
  ~CIOCPModel(void);
public:
  // 加载Socket库
  bool LoadSocketLib();
  // 卸载Socket库，彻底完事
  void UnloadSocketLib() {
    WSACleanup();
  }
  // 启动服务器
  bool Start();
  // 停止服务器
  void Stop(BOOL wait);
  // 获得本机的IP地址
  const char* GetLocalIP();
  // 设置监听端口
  void SetPort(const int& nPort) {
    m_nPort = nPort;
  }
  // 设置主界面的指针，用于调用显示信息到界面中
  void SetMainDlg(void* p) {
    m_pMain = p;
  }
  //投递WSASend，用于发送数据
  bool PostWrite(PER_IO_CONTEXT* pAcceptIoContext);
  //投递WSARecv用于接收数据
  bool PostRecv(PER_IO_CONTEXT* pIoContext);
protected:
  // 初始化IOCP
  bool _InitializeIOCP();
  // 初始化Socket
  bool _InitializeListenSocket();
  // 最后释放资源
  void _DeInitialize();
  //投递AcceptEx请求
  bool _PostAccept(PER_IO_CONTEXT* pAcceptIoContext);
  //在有客户端连入的时候，进行处理
  bool _DoAccpet(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext);
  //连接成功时，根据第一次是否接收到来自客户端的数据进行调用
  bool _DoFirstRecvWithData(PER_IO_CONTEXT* pIoContext);
  bool _DoFirstRecvWithoutData(PER_IO_CONTEXT* pIoContext);
  //数据到达，数组存放在pIoContext参数中
  bool _DoRecv(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext);
  //将客户端socket的相关信息存储到数组中
  void _AddToContextList(PER_SOCKET_CONTEXT* pSocketContext);
  //将客户端socket的信息从数组中移除
  void _RemoveContext(PER_SOCKET_CONTEXT* pSocketContext);
  // 清空客户端socket信息
  void _ClearContextList();
  // 将句柄绑定到完成端口中
  bool _AssociateWithIOCP(PER_SOCKET_CONTEXT* pContext);
  // 处理完成端口上的错误
  bool HandleError(PER_SOCKET_CONTEXT* pContext, const DWORD& dwErr);
  //线程函数，为IOCP请求服务的工作者线程
  static DWORD WINAPI _WorkerThread(LPVOID lpParam);
  //获得本机的处理器数量
  int _GetNoOfProcessors();
  //判断客户端Socket是否已经断开
  bool _IsSocketAlive(SOCKET s);
  //在主界面中显示信息
  void _ShowMessage(const char* szFormat, ...) const;
private:
  HANDLE m_hShutdownEvent; // 用来通知线程系统退出的事件，为了能够更好的退出线程
  HANDLE m_hIOCompletionPort; // 完成端口的句柄
  HANDLE* m_phWorkerThreads; // 工作者线程的句柄指针
  int m_nThreads; // 生成的线程数量
  char m_strIP[64]; // 服务器端的IP地址
  int m_nPort; // 服务器端的监听端口
  void* m_pMain; // 主界面的界面指针，用于在主界面中显示消息
  CRITICAL_SECTION m_csContextList; // 用于Worker线程同步的互斥量
  PER_SOCKET_CONTEXT** m_arrayClientContext; // 客户端Socket的Context信息
  int m_arrayClientContext_Count;
  PER_SOCKET_CONTEXT* m_pListenContext; // 用于监听的Socket的Context信息
  LPFN_ACCEPTEX m_lpfnAcceptEx; // AcceptEx 和 GetAcceptExSockaddrs 的函数指针，用于调用这两个扩展函数
  LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockAddrs;
};
CIOCPModel::CIOCPModel(void):
  m_nThreads(0),
  m_hShutdownEvent(NULL),
  m_hIOCompletionPort(NULL),
  m_phWorkerThreads(NULL),
  m_nPort(DEFAULT_PORT),
  m_pMain(NULL),
  m_lpfnAcceptEx(NULL),
  m_arrayClientContext(NULL),
  m_arrayClientContext_Count(0),
  m_pListenContext(NULL)
{
  strcpy(m_strIP, DEFAULT_IP);
}
CIOCPModel::~CIOCPModel(void)
{
  // 确保资源彻底释放
  this->Stop(0);
}
//////////////////////////////////////////////////////////////////////
//函数功能：线程函数，根据GetQueuedCompletionStatus返回情况进行处理；
//函数参数：lpParam是IOCPTHREADPARAMS_WORKER类型指针；
//函数说明：GetQueuedCompletionStatus正确返回时表示某操作已经完成，
//第二个参数lpNumberOfBytes表示本次套接字传输的字节数，
//参数lpCompletionKey和lpOverlapped包含重要的信息，请查询MSDN文档；
////////////////////////////////////////////////////////////////////*/
DWORD WINAPI CIOCPModel::_WorkerThread(LPVOID lpParam)
{
  IOCPTHREADPARAMS_WORKER* pParam = (IOCPTHREADPARAMS_WORKER*)lpParam;
  CIOCPModel* pIOCPModel = (CIOCPModel*)pParam->pIOCPModel;
  int nThreadNo = (int)pParam->nThreadNo;
  pIOCPModel->_ShowMessage("工作者线程启动，ID: %d.", nThreadNo);
  OVERLAPPED* pOverlapped = NULL;
  PER_SOCKET_CONTEXT* pSocketContext = NULL;
  DWORD dwBytesTransfered = 0;
  //循环处理请求，直到接收到Shutdown信息为止
  while (WAIT_OBJECT_0 != WaitForSingleObject(pIOCPModel->m_hShutdownEvent, 0)) {
    BOOL bReturn = GetQueuedCompletionStatus(
        pIOCPModel->m_hIOCompletionPort,
        &dwBytesTransfered,
        (PULONG_PTR)&pSocketContext,
        &pOverlapped,
        INFINITE);
    //接收EXIT_CODE退出标志，则直接退出
    if (EXIT_CODE == (DWORD)pSocketContext) {
      break;
    }
    //返回值为0，表示出错
    if (!bReturn) {
      DWORD dwErr = GetLastError();
      // 显示一下提示信息
      if (!pIOCPModel->HandleError(pSocketContext, dwErr)) {
        break;
      }
      continue;
    }
    else {
      // 读取传入的参数
      PER_IO_CONTEXT* pIoContext = CONTAINING_RECORD(pOverlapped, PER_IO_CONTEXT, m_Overlapped);
      // 判断是否有客户端断开了
      if ((0 == dwBytesTransfered) && (RECV_POSTED == pIoContext->m_OpType || SEND_POSTED == pIoContext->m_OpType)) {
        pIOCPModel->_ShowMessage(_T("客户端 %s:%d 断开连接."), inet_ntoa(pSocketContext->m_ClientAddr.sin_addr), ntohs(pSocketContext->m_ClientAddr.sin_port));
        // 释放掉对应的资源
        pIOCPModel->_RemoveContext(pSocketContext);
        continue;
      }
      else {
        switch (pIoContext->m_OpType) {
          // Accept
        case ACCEPT_POSTED: {
          pIoContext->m_nTotalBytes = dwBytesTransfered;
          pIOCPModel->_DoAccpet(pSocketContext, pIoContext);
        }
        break;
        // RECV
        case RECV_POSTED: {
          pIoContext->m_nTotalBytes = dwBytesTransfered;
          pIOCPModel->_DoRecv(pSocketContext, pIoContext);
        }
        break;
        case SEND_POSTED: {
          pIoContext->m_nSendBytes += dwBytesTransfered;
          if (pIoContext->m_nSendBytes < pIoContext->m_nTotalBytes) {
            //数据未能发送完，继续发送数据
            pIoContext->m_wsaBuf.buf = pIoContext->m_szBuffer + pIoContext->m_nSendBytes;
            pIoContext->m_wsaBuf.len = pIoContext->m_nTotalBytes - pIoContext->m_nSendBytes;
            pIOCPModel->PostWrite(pIoContext);
          }
          else {
            pIOCPModel->PostRecv(pIoContext);
          }
        }
        break;
        default:
          // 不应该执行到这里
          TRACE(_T("_WorkThread中的 pIoContext->m_OpType 参数异常.\n"));
          break;
        } //switch
      }//if
    }//if
  }//while
  TRACE(_T("工作者线程 %d 号退出.\n"), nThreadNo);
  // 释放线程参数
  RELEASE(lpParam);
  return 0;
}
//函数功能：初始化套接字
bool CIOCPModel::LoadSocketLib()
{
  WSADATA wsaData;
  int nResult;
  nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  // 错误(一般都不可能出现)
  if (NO_ERROR != nResult) {
    this->_ShowMessage(_T("初始化WinSock 2.2失败！\n"));
    return false;
  }
  return true;
}
//函数功能：启动服务器
bool CIOCPModel::Start()
{
  // 初始化线程互斥量
  InitializeCriticalSection(&m_csContextList);
  // 建立系统退出的事件通知
  m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  // 初始化IOCP
  if (false == _InitializeIOCP()) {
    this->_ShowMessage(_T("初始化IOCP失败！\n"));
    return false;
  }
  else {
    this->_ShowMessage("\nIOCP初始化完毕\n.");
  }
  // 初始化Socket
  if (false == _InitializeListenSocket()) {
    this->_ShowMessage(_T("Listen Socket初始化失败！\n"));
    this->_DeInitialize();
    return false;
  }
  else {
    this->_ShowMessage("Listen Socket初始化完毕.");
  }
  this->_ShowMessage(_T("系统准备就绪，等候连接....\n"));
  return true;
}
//
// 开始发送系统退出消息，退出完成端口和线程资源
void CIOCPModel::Stop(BOOL wait)
{
  if (wait) {
    WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);
  }
  if (m_pListenContext != NULL && m_pListenContext->m_Socket != INVALID_SOCKET) {
    // 激活关闭消息通知
    SetEvent(m_hShutdownEvent);
    for (int i = 0; i < m_nThreads; i++) {
      // 通知所有的完成端口操作退出
      PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
    }
    // 等待所有的客户端资源退出
    WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);
    // 清除客户端列表信息
    this->_ClearContextList();
    // 释放其他资源
    this->_DeInitialize();
    this->_ShowMessage("停止监听\n");
  }
}
//////////////////////////////////////////////////////////////
//函数功能：投递WSARecv请求；
//函数参数：
//PER_IO_CONTEXT* pIoContext: 用于进行IO的套接字上的结构，主要为WSARecv参数和WSASend参数；
///////////////////////////////////////////////////////////////
bool CIOCPModel::PostRecv(PER_IO_CONTEXT* pIoContext)
{
  // 初始化变量
  DWORD dwFlags = 0;
  DWORD dwBytes = 0;
  WSABUF* p_wbuf = &pIoContext->m_wsaBuf;
  OVERLAPPED* p_ol = &pIoContext->m_Overlapped;
  PER_IO_CONTEXT_ResetBuffer(pIoContext);
  pIoContext->m_OpType = RECV_POSTED;
  pIoContext->m_nSendBytes = 0;
  pIoContext->m_nTotalBytes = 0;
  // 初始化完成后，，投递WSARecv请求
  int nBytesRecv = WSARecv(pIoContext->m_sockAccept, p_wbuf, 1, &dwBytes, &dwFlags, p_ol, NULL);
  // 如果返回值错误，并且错误的代码并非是Pending的话，那就说明这个重叠请求失败了
  if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError())) {
    this->_ShowMessage("投递第一个WSARecv失败！");
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////
//函数功能：投递WSASend请求
//函数参数：
//PER_IO_CONTEXT* pIoContext: 用于进行IO的套接字上的结构，主要为WSARecv参数和WSASend参数
//函数说明：调用PostWrite之前需要设置pIoContext中m_wsaBuf, m_nTotalBytes, m_nSendBytes；
///////////////////////////////////////////////////////////////
bool CIOCPModel::PostWrite(PER_IO_CONTEXT* pIoContext)
{
  // 初始化变量
  DWORD dwFlags = 0;
  DWORD dwSendNumBytes = 0;
  WSABUF* p_wbuf = &pIoContext->m_wsaBuf;
  OVERLAPPED* p_ol = &pIoContext->m_Overlapped;
  pIoContext->m_OpType = SEND_POSTED;
  //投递WSASend请求 -- 需要修改
  int nRet = WSASend(pIoContext->m_sockAccept, &pIoContext->m_wsaBuf, 1, &dwSendNumBytes, dwFlags,
      &pIoContext->m_Overlapped, NULL);
  // 如果返回值错误，并且错误的代码并非是Pending的话，那就说明这个重叠请求失败了
  if ((SOCKET_ERROR == nRet) && (WSA_IO_PENDING != WSAGetLastError())) {
    this->_ShowMessage("投递WSASend失败！");
    return false;
  }
  return true;
}
////////////////////////////////
// 初始化完成端口
bool CIOCPModel::_InitializeIOCP()
{
  // 建立第一个完成端口
  m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
  if (NULL == m_hIOCompletionPort) {
    this->_ShowMessage(_T("建立完成端口失败！错误代码: %d!\n"), WSAGetLastError());
    return false;
  }
  // 根据本机中的处理器数量，建立对应的线程数
  m_nThreads = WORKER_THREADS_PER_PROCESSOR * _GetNoOfProcessors();
  // 为工作者线程初始化句柄
  MYREALLOC(m_phWorkerThreads, m_nThreads);
  // 根据计算出来的数量建立工作者线程
  DWORD nThreadID;
  for (int i = 0; i < m_nThreads; i++) {
    IOCPTHREADPARAMS_WORKER* pThreadParams = NULL;
    MYREALLOC(pThreadParams, 1);
    pThreadParams->pIOCPModel = this;
    pThreadParams->nThreadNo = i + 1;
    m_phWorkerThreads[i] = ::CreateThread(0, 0, _WorkerThread, (void*)pThreadParams, 0, &nThreadID);
  }
  TRACE(" 建立 _WorkerThread %d 个.\n", m_nThreads);
  return true;
}
/////////////////////////////////////////////////////////////////
// 初始化Socket
bool CIOCPModel::_InitializeListenSocket()
{
  // AcceptEx 和 GetAcceptExSockaddrs 的GUID，用于导出函数指针
  GUID GuidAcceptEx = WSAID_ACCEPTEX;
  GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
  // 服务器地址?畔ⅲ糜诎蠖⊿ocket
  struct sockaddr_in ServerAddress;
  // 生成用于监听的Socket的信息
  MYREALLOC(m_pListenContext, 1);
  PER_SOCKET_CONTEXT_init(m_pListenContext);
  // 需要使用重叠IO，必须得使用WSASocket来建立Socket，才可以支持重叠IO操作
  m_pListenContext->m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
  if (INVALID_SOCKET == m_pListenContext->m_Socket) {
    this->_ShowMessage("初始化Socket失败，错误代码: %d.\n", WSAGetLastError());
    return false;
  }
  else {
    TRACE("WSASocket() 完成.\n");
  }
  // 将Listen Socket绑定至完成端口中
  if (NULL == CreateIoCompletionPort((HANDLE)m_pListenContext->m_Socket, m_hIOCompletionPort, (DWORD)m_pListenContext, 0)) {
    this->_ShowMessage("绑定 Listen Socket至完成端口失败！错误代码: %d/n", WSAGetLastError());
    RELEASE_SOCKET(m_pListenContext->m_Socket);
    return false;
  }
  else {
    TRACE("Listen Socket绑定完成端口 完成.\n");
  }
  // 填充地址信息
  ZeroMemory((char*)&ServerAddress, sizeof(ServerAddress));
  ServerAddress.sin_family = AF_INET;
  // 这里可?园蠖ㄈ魏慰捎玫腎P地址，或者绑定一个指定的IP地址
  ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  //ServerAddress.sin_addr.s_addr = inet_addr(m_strIP);
  ServerAddress.sin_port = htons(m_nPort);
  // 绑定地址和端口
  if (SOCKET_ERROR == bind(m_pListenContext->m_Socket, (struct sockaddr*) &ServerAddress, sizeof(ServerAddress))) {
    this->_ShowMessage("bind()函数执行错误.\n");
    return false;
  }
  else {
    TRACE("bind() 完成.\n");
  }
  // 开始进行监听
  if (SOCKET_ERROR == listen(m_pListenContext->m_Socket, SOMAXCONN)) {
    this->_ShowMessage("Listen()函数执行出现错误.\n");
    return false;
  }
  else {
    TRACE("Listen() 完成.\n");
  }
  // 使用AcceptEx函数，因为这个是属于WinSock2规范之外的微软另外提供的扩展函数
  // 所以需要额外获取一下函数的指针，
  // 获取AcceptEx函数指针
  DWORD dwBytes = 0;
  if (SOCKET_ERROR == WSAIoctl(m_pListenContext->m_Socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
      &GuidAcceptEx, sizeof(GuidAcceptEx), &m_lpfnAcceptEx, sizeof(m_lpfnAcceptEx), &dwBytes, NULL, NULL)) {
    this->_ShowMessage("WSAIoctl 未能获取AcceptEx函数指针。错误代码: %d\n", WSAGetLastError());
    this->_DeInitialize();
    return false;
  }
  // 获取GetAcceptExSockAddrs函数指针，也是同理
  if (SOCKET_ERROR == WSAIoctl(m_pListenContext->m_Socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidGetAcceptExSockAddrs,
      sizeof(GuidGetAcceptExSockAddrs), &m_lpfnGetAcceptExSockAddrs, sizeof(m_lpfnGetAcceptExSockAddrs), &dwBytes, NULL, NULL)) {
    this->_ShowMessage("WSAIoctl 未能获取GuidGetAcceptExSockAddrs函数指针。错误代码: %d\n", WSAGetLastError());
    this->_DeInitialize();
    return false;
  }
  // 为AcceptEx 准备参数，然后投递AcceptEx I/O请求
  //创建10个套接字，投递AcceptEx请求，即共有10个套接字进行accept操作；
  for (int i = 0; i < MAX_POST_ACCEPT; i++) {
    // 新建一个IO_CONTEXT
    PER_IO_CONTEXT* pAcceptIoContext = PER_SOCKET_CONTEXT_GetNewIoContext(m_pListenContext);
    if (false == this->_PostAccept(pAcceptIoContext)) {
      PER_SOCKET_CONTEXT_RemoveContext(m_pListenContext, pAcceptIoContext);
      return false;
    }
  }
  this->_ShowMessage(_T("投递 %d 个AcceptEx请求完毕"), MAX_POST_ACCEPT);
  return true;
}
////////////////////////////////////////////////////////////
// 最后释放掉所有资源
void CIOCPModel::_DeInitialize()
{
  // 删除客户端列表的互斥量
  DeleteCriticalSection(&m_csContextList);
  // 关闭系统退出事件句柄
  RELEASE_HANDLE(m_hShutdownEvent);
  // 释放工作者线程句柄指针
  for (int i = 0; i < m_nThreads; i++) {
    RELEASE_HANDLE(m_phWorkerThreads[i]);
  }
  RELEASE(m_phWorkerThreads);
  // 关闭IOCP句柄
  RELEASE_HANDLE(m_hIOCompletionPort);
  // 关闭监听Socket
  RELEASE(m_pListenContext);
  this->_ShowMessage("释放资源完毕.\n");
}
//
// 投递完成端口请求
//
#pragma comment( lib, "mswsock.lib")
char* GetErrorMsg(int dw)
{
  // Retrieve the system error message for the last-error code
  static char buf[256];
  LPTSTR lpMsgBuf = NULL;
  if (0 == dw) {
    dw = GetLastError();
  }
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
  _snprintf(buf, 256, "err: %d %s", dw, lpMsgBuf);
  return buf;
}
// 投递Accept请求
bool CIOCPModel::_PostAccept(PER_IO_CONTEXT* pAcceptIoContext)
{
  ASSERT(INVALID_SOCKET != m_pListenContext->m_Socket);
  // 准备参数
  DWORD dwBytes = 0;
  pAcceptIoContext->m_OpType = ACCEPT_POSTED;
  WSABUF* p_wbuf = &pAcceptIoContext->m_wsaBuf;
  OVERLAPPED* p_ol = &pAcceptIoContext->m_Overlapped;
  // 为以后新连入的客户端先准备好Socket( 这个是与传统accept最大的区别 )
  pAcceptIoContext->m_sockAccept = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
  if (INVALID_SOCKET == pAcceptIoContext->m_sockAccept) {
    _ShowMessage("创建用于Accept的Socket失败！错误代码: %d", WSAGetLastError());
    return false;
  }
#define M_ACCEPTEX m_lpfnAcceptEx
  //#define M_ACCEPTEX AcceptEx
  // 投递AcceptEx
  if (FALSE == M_ACCEPTEX(m_pListenContext->m_Socket, pAcceptIoContext->m_sockAccept, p_wbuf->buf, p_wbuf->len - ((sizeof(SOCKADDR_IN) + 16) * 2),
      sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &dwBytes, p_ol)) {
    if (WSA_IO_PENDING != WSAGetLastError()) {
      int err = WSAGetLastError();
      _ShowMessage("投递 AcceptEx 请求失败，错误代码: %d %s", err, GetErrorMsg(err));
      printf("%s", strerror(err));
      return false;
    }
  }
  return true;
}
////////////////////////////////////////////////////////////////////*
//函数功能：函数进行客户端接入处理；
//参数说明：
//PER_SOCKET_CONTEXT* pSocketContext: 本次accept操作对应的套接字，该套接字所对应的数据结构；
//PER_IO_CONTEXT* pIoContext: 本次accept操作对应的数据结构；
//DWORD dwIOSize: 本次操作数据实际传输的?纸谑?
/////////////////////////////////////////////////////////////////////
bool CIOCPModel::_DoAccpet(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext)
{
  if (pIoContext->m_nTotalBytes > 0) {
    //客户接入时，第一次接收dwIOSize字节数据
    _DoFirstRecvWithData(pIoContext);
  }
  else {
    //客户端接入时，没有发送数据，则投递WSARecv请求，接收数据
    _DoFirstRecvWithoutData(pIoContext);
  }
  // 5. 使用完毕之后，把Listen Socket的那个IoContext重置，然后准备投递新的AcceptEx
  PER_IO_CONTEXT_ResetBuffer(pIoContext);
  return this->_PostAccept(pIoContext);
}
/////////////////////////////////////////////////////////////////
//函数功能：在有接收的数据到达的时候，进行处理
bool CIOCPModel::_DoRecv(PER_SOCKET_CONTEXT* pSocketContext, PER_IO_CONTEXT* pIoContext)
{
  //输出接收的数据
  SOCKADDR_IN* ClientAddr = &pSocketContext->m_ClientAddr;
  this->_ShowMessage(_T("收到 %s:%d 信息：%s"), inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port), pIoContext->m_wsaBuf.buf);
  //发送数据
  pIoContext->m_nSendBytes = 0;
  pIoContext->m_nTotalBytes = pIoContext->m_nTotalBytes;
  pIoContext->m_wsaBuf.len = pIoContext->m_nTotalBytes;
  pIoContext->m_wsaBuf.buf = pIoContext->m_szBuffer;
  return PostWrite(pIoContext);
}
//////////////////////////////////////////////////////////////
//函数功能：AcceptEx接收客户连接成功，接收客户第一次发送的数据，故投递WSASend请求
//函数参数：
//PER_IO_CONTEXT* pIoContext: 用于监听套接字上的操作
///////////////////////////////////////////////////////////////
bool CIOCPModel::_DoFirstRecvWithData(PER_IO_CONTEXT* pIoContext)
{
  SOCKADDR_IN* ClientAddr = NULL;
  SOCKADDR_IN* LocalAddr = NULL;
  int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);
  //1. 首先取得连入客户端的地址信息
  this->m_lpfnGetAcceptExSockAddrs(pIoContext->m_wsaBuf.buf, pIoContext->m_wsaBuf.len - ((sizeof(SOCKADDR_IN) + 16) * 2),
      sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, (LPSOCKADDR*)&LocalAddr, &localLen, (LPSOCKADDR*)&ClientAddr, &remoteLen);
  //显示客户端信息
  this->_ShowMessage(_T("客户额 %s:%d 信息：%s."), inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port), pIoContext->m_wsaBuf.buf);
  //2.为新接入的套接创建PER_SOCKET_CONTEXT，并将该套接字绑定到完成端口
  PER_SOCKET_CONTEXT* pNewSocketContext = NULL;
  MYREALLOC(pNewSocketContext, 1);
  PER_SOCKET_CONTEXT_init(pNewSocketContext);
  pNewSocketContext->m_Socket = pIoContext->m_sockAccept;
  memcpy(&(pNewSocketContext->m_ClientAddr), ClientAddr, sizeof(SOCKADDR_IN));
  // 参数设置完毕，将这个Socket和完成端口绑定(这也是一个关键步骤)
  if (false == this->_AssociateWithIOCP(pNewSocketContext)) {
    PER_SOCKET_CONTEXT_free(pNewSocketContext);
    RELEASE(pNewSocketContext);
    return false;
  }
  //3. 继续，建立其下的IoContext，用于在这个Socket上投递第一个Recv数据请求
  PER_IO_CONTEXT* pNewIoContext = PER_SOCKET_CONTEXT_GetNewIoContext(pNewSocketContext);
  pNewIoContext->m_OpType = SEND_POSTED;
  pNewIoContext->m_sockAccept = pNewSocketContext->m_Socket;
  pNewIoContext->m_nTotalBytes = pIoContext->m_nTotalBytes;
  pNewIoContext->m_nSendBytes = 0;
  pNewIoContext->m_wsaBuf.len = pIoContext->m_nTotalBytes;
  memcpy(pNewIoContext->m_wsaBuf.buf, pIoContext->m_wsaBuf.buf, pIoContext->m_nTotalBytes); //复制数据到WSASend函数的参数缓冲区
  //此时是第一次接收数据成功，所以这里投递PostWrite，向客户端发送数据
  if (false == this->PostWrite(pNewIoContext)) {
    PER_SOCKET_CONTEXT_RemoveContext(pNewSocketContext, pNewIoContext);
    return false;
  }
  //4. 如果投递成功，那么就把这个有效的客户端信息，加入到ContextList中去(需要统一管理，方便释放资源)
  this->_AddToContextList(pNewSocketContext);
  return true;
}
//////////////////////////////////////////////////////////////
//函数功能：AcceptEx接收客户连接成功，此时并未接收到数据，故投递WSARecv请求
//函数参数：
//PER_IO_CONTEXT* pIoContext: 用于监听套接字上的操作
///////////////////////////////////////////////////////////////
bool CIOCPModel::_DoFirstRecvWithoutData(PER_IO_CONTEXT* pIoContext)
{
  //为新接入的套接字创建PER_SOCKET_CONTEXT结构，并绑定到完成端口
  PER_SOCKET_CONTEXT* pNewSocketContext = NULL;
  SOCKADDR_IN ClientAddr;
  int Len = sizeof(ClientAddr);
  MYREALLOC(pNewSocketContext, 1);
  PER_SOCKET_CONTEXT_init(pNewSocketContext);
  getpeername(pIoContext->m_sockAccept, (sockaddr*)&ClientAddr, &Len);
  pNewSocketContext->m_Socket = pIoContext->m_sockAccept;
  memcpy(&(pNewSocketContext->m_ClientAddr), &ClientAddr, sizeof(SOCKADDR_IN));
  //将该套接字绑定到完成端口
  if (false == this->_AssociateWithIOCP(pNewSocketContext)) {
    PER_SOCKET_CONTEXT_free(pNewSocketContext);
    RELEASE(pNewSocketContext);
    return false;
  }
  //投递WSARecv请求，接收数据
  PER_IO_CONTEXT* pNewIoContext = PER_SOCKET_CONTEXT_GetNewIoContext(pNewSocketContext);
  //此时是AcceptEx未接收到客户端第一次发送的数据，所以这里调用PostRecv，接收来自客户端的数据
  if (false == this->PostRecv(pNewIoContext)) {
    PER_SOCKET_CONTEXT_RemoveContext(pNewSocketContext, pNewIoContext);
    return false;
  }
  //如果投递成功，那么就把这个有效的客户端信息，加入到ContextList中去(需要统一管理，方便释放资源)
  this->_AddToContextList(pNewSocketContext);
  return true;
}
/////////////////////////////////////////////////////
// 将句柄(Socket)绑定到完成端口中
bool CIOCPModel::_AssociateWithIOCP(PER_SOCKET_CONTEXT* pContext)
{
  // 将用于和客户端通信的SOCKET绑定到完成端口中
  HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, m_hIOCompletionPort, (DWORD)pContext, 0);
  if (NULL == hTemp) {
    this->_ShowMessage(("执行CreateIoCompletionPort()出现错误.错误代码：%d"), GetLastError());
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////
// 将客户端的相关信息存储到数组中
void CIOCPModel::_AddToContextList(PER_SOCKET_CONTEXT* pHandleData)
{
  EnterCriticalSection(&m_csContextList);
  MEMPUSH(m_arrayClientContext, m_arrayClientContext_Count);
  m_arrayClientContext[m_arrayClientContext_Count - 1] = pHandleData;
  LeaveCriticalSection(&m_csContextList);
}
////////////////////////////////////////////////////////////////
// 移除某个特定的Context
void CIOCPModel::_RemoveContext(PER_SOCKET_CONTEXT* pSocketContext)
{
  EnterCriticalSection(&m_csContextList);
  for (int i = 0; i < m_arrayClientContext_Count; i++) {
    if (pSocketContext == m_arrayClientContext[i]) {
      PER_SOCKET_CONTEXT_free(pSocketContext);
      RELEASE(pSocketContext);
      MEMDEL(m_arrayClientContext, m_arrayClientContext_Count, i, 1);
      m_arrayClientContext_Count--;
      break;
    }
  }
  LeaveCriticalSection(&m_csContextList);
}
////////////////////////////////////////////////////////////////
// 清空客户端信息
void CIOCPModel::_ClearContextList()
{
  EnterCriticalSection(&m_csContextList);
  for (int i = 0; i < m_arrayClientContext_Count; i++) {
    PER_SOCKET_CONTEXT* pContext = m_arrayClientContext[i];
    PER_SOCKET_CONTEXT_free(pContext);
    free(pContext);
  }
  FREE(m_arrayClientContext);
  m_arrayClientContext = NULL;
  m_arrayClientContext_Count = 0;
  LeaveCriticalSection(&m_csContextList);
}
//
// 获得本机的IP地址
const char* CIOCPModel::GetLocalIP()
{
  // 获得本机主机名
  char hostname[MAX_PATH] = {0};
  gethostname(hostname, MAX_PATH);
  struct hostent FAR* lpHostEnt = gethostbyname(hostname);
  if (lpHostEnt == NULL) {
    return DEFAULT_IP;
  }
  // 取得IP地址列表中的第一个为返回的IP(因为一台主机可能会绑定多个IP)
  LPSTR lpAddr = lpHostEnt->h_addr_list[0];
  // 将IP地址转化成字符串形式
  struct in_addr inAddr;
  memmove(&inAddr, lpAddr, 4);
  strcpy(m_strIP, inet_ntoa(inAddr));
  return m_strIP;
}
// 获得本机中处理器的数量
int CIOCPModel::_GetNoOfProcessors()
{
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return si.dwNumberOfProcessors;
}
///
// 在主界面中显示提示信息
void CIOCPModel::_ShowMessage(const char* szFormat, ...) const
{
  // 根据传入的参数格式化字符串
  char strMessage[256];
  va_list arglist;
  // 处理变长参数
  va_start(arglist, szFormat);
  _vsnprintf(strMessage, 256, szFormat, arglist);
  va_end(arglist);
  // 在主界面中显示
  TRACE(strMessage);
  TRACE("\n");
}
///
// 判断客户端Socket是否已经断开，否则在一个无效的Socket上投递WSARecv操作会出现异常
// 使用的方法是尝试向这个socket发送数据，判断这个socket调用的返回值
// 因为如果客户端网络异常断开(例如客户端崩溃或者拔掉网线等)的时候，服务器端是无法收到客户端断开的通知的
bool CIOCPModel::_IsSocketAlive(SOCKET s)
{
  int nByteSent = send(s, "", 0, 0);
  if (-1 == nByteSent) {
    return false;
  }
  return true;
}
//函数功能：显示并处理完成端口上的错误
bool CIOCPModel::HandleError(PER_SOCKET_CONTEXT* pContext, const DWORD& dwErr)
{
  // 如果是超时了，就再继续等吧
  if (WAIT_TIMEOUT == dwErr) {
    // 确认客户端是否还活着...
    if (!_IsSocketAlive(pContext->m_Socket)) {
      this->_ShowMessage(_T("检测到客户端异常退出！"));
      this->_RemoveContext(pContext);
      return true;
    }
    else {
      this->_ShowMessage(_T("网络操作超时！重试中..."));
      return true;
    }
  }
  // 可能是客户端异常退出了
  else if (ERROR_NETNAME_DELETED == dwErr) {
    this->_ShowMessage(_T("检测到客户端异常退出！"));
    this->_RemoveContext(pContext);
    return true;
  }
  else {
    this->_ShowMessage(_T("完成端口操作出现错误，线程退出。错误代码：%d"), dwErr);
    return false;
  }
}

