// SockHandle.cpp: implementation of the CSockHandle class.
//
class CDataRecvJob : public CJob
{
public:
  CDataRecvJob(SOCKET m_SOCK, const int8* pData, int32 nSize, const addr_in* pAddr, void* user);
  virtual ~CDataRecvJob();
public:
  virtual void Run();
private:
  SOCKET m_hSOCK;
  const int8* m_pData;
  int m_nSize;
  addr_in m_Addr;
  BOOL m_bAddrValide;;
  void* m_user;
};
CDataRecvJob::CDataRecvJob(SOCKET m_SOCK, const int8* pData, int32 nSize, const addr_in* pAddr, void* user) :
  m_hSOCK(m_SOCK),
  m_pData(pData),
  m_nSize(nSize),
  m_user(user),
  m_bAddrValide(false)
{
  memset(&m_Addr, 0x0, sizeof(m_Addr));

  if (pAddr) {
    m_Addr = *pAddr;
    m_bAddrValide = true;
  }
}
CDataRecvJob::~CDataRecvJob()
{
  CMemPool::GetInstance().FreeMem((int8*)m_pData);
}
void CDataRecvJob::Run()
{
  CNetManager_OnRecv(m_hSOCK, m_pData + sizeof(SOCK_DATA_HEADER) + sizeof(uint32), m_nSize, m_bAddrValide ? &m_Addr : NULL, m_user);
}
CDataRecvProcess::CDataRecvProcess(SOCKET sock, SSL* ssl , CThreadPool* pThreadPool, void* user) :
  m_SOCK(sock),
  m_pSsl(ssl),
  m_pThreadPool(pThreadPool),
  m_lIdentify(0),
  m_user(user),
  m_RefCount(1)
{
  memset(m_PackHdr, 0x0, sizeof(m_PackHdr));
}
CDataRecvProcess::~CDataRecvProcess()
{
  DATA_MAP_EX::iterator it = m_mapDataEx.begin();

  for (; it != m_mapDataEx.end(); ++it) {
    CMemPool::GetInstance().FreeMem(it->second);
  }

  printf("%s %d\n", __FILE__, __LINE__);
  DEBUG_INFO1("%s" , "~CDataRecvProcess");
  COpenSsl::Obj().Free(m_SOCK , m_pSsl);
}
int CDataRecvProcess::AddRef(void)
{
#ifndef WIN32
  AoutLock lock(&m_SynRef);
  return ++m_RefCount;
#else
  return InterlockedIncrement(&m_RefCount);
#endif
}
int CDataRecvProcess::Release(void)
{
#ifndef _WIN32
  int rs = 0;
  m_SynRef.Lock();
  rs = --m_RefCount;
  m_SynRef.Unlock();

  if (0 == rs) {
    delete this;
  }

  return rs;
#else
  int rs = InterlockedDecrement(&m_RefCount);

  if (0 == rs) {
    delete this;
  }

  return rs;
#endif
}
int32 CDataRecvProcess::Send(const int8* pbData, int32 nLen, const addr_in* pAddr)
{
  // 同一个连接，采用同步发送机制
#define MAX_SEQ 0x7fffffff
  AoutLock lock(&m_synIdentfiy);
  int32 nSeq = ((m_lIdentify++) % MAX_SEQ);
  return COpenSsl::Obj().Send(m_SOCK, m_pSsl, pbData, nLen, nSeq, pAddr);
}
void CDataRecvProcess::Accept()
{
  addr_in Addr[1] = {0};
  SOCKET accepted = sock_accept(m_SOCK, Addr);

  if (INVALID_SOCKET != accepted) {
    CNetManager_OnListen(accepted, Addr, m_user);
  }
}
BOOL CDataRecvProcess::Recv()
{
  int32 nLen = 0;
  int32 nError = 0;
  int32 nRecvBytes = sizeof(SOCK_DATA_HEADER) - m_PackHdr[0];
  char* pRecvHdr = (char*)(m_PackHdr + 1);
  SOCK_DATA_HEADER& hdr = *(SOCK_DATA_HEADER*)pRecvHdr;

  if (nRecvBytes) {
    nLen = COpenSsl::Obj().Recv(m_SOCK, m_pSsl, pRecvHdr + m_PackHdr[0], nRecvBytes);

    if (nLen < 0) {
      nError = sock_error();
      printf("1---nError = %d , nLen = %d nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
      return WSAEINTR == nError || WSAEWOULDBLOCK == nError;
    }
    else if (nLen == 0) {
      printf("2---nError = %d , nLen = %d nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
      return false;
    }

    m_PackHdr[0] += nLen;

    if (m_PackHdr[0] < sizeof(SOCK_DATA_HEADER)) {
      return true;
    }

    if (0 != sock_checksum((uint16*)&hdr, sizeof(SOCK_DATA_HEADER))) {
      printf("%s", "sock_checksum err \r\n");
      return false;
    }
    else {
#ifndef WIN32
      m_Timer.Restart();
#endif
    }
  }

  DATA_MAP_EX::iterator it = m_mapDataEx.find(hdr.msg_identify);
  int8* pRetMem = NULL;

  if (it == m_mapDataEx.end()) {
    pRetMem = CMemPool::GetInstance().GetMem(hdr.msg_total + sizeof(SOCK_DATA_HEADER) + sizeof(uint32) + 4, true);

    if (NULL != pRetMem) {
      SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*)pRetMem;
      memset(pHdr , 0 , sizeof(SOCK_DATA_HEADER));
      pHdr->msg_identify = hdr.msg_identify;
      std::pair<DATA_MAP_EX::iterator, BOOL> IT = m_mapDataEx.insert(std::pair<uint32, int8*>(hdr.msg_identify, pRetMem));
      it = IT.first;
    }
  }
  else {
    pRetMem = it->second;
  }

  if (pRetMem == NULL) {
    return true;
  }

  //assert((pRetMem & 3) == 0);
  SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*)pRetMem;
  uint32* pnInitTime = (uint32*)(pHdr + 1);
#ifdef WIN32
  *pnInitTime = ::GetTickCount();
#else
  *pnInitTime = m_Timer.Elapsed();
#endif
  int8* pMem = pRetMem + sizeof(SOCK_DATA_HEADER) + sizeof(uint32);
  nLen = COpenSsl::Obj().Recv(m_SOCK, m_pSsl, pMem + hdr.msg_offset + pHdr->msg_len, ALIGN(hdr.msg_len, 4) - pHdr->msg_len);

  if (nLen < 0) {
    nError = sock_error();
    printf("3---nError = %d , nLen = %d nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
    return WSAEINTR == nError || WSAEWOULDBLOCK == nError;
  }
  else if (nLen == 0) {
    printf("4---nError = %d , nLen = %d nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
    return false;
  }

  pHdr->msg_len += nLen;

  if (pHdr->msg_len < ALIGN(hdr.msg_len, 4)) {
    return true;//小包未接收完成
  }

  pHdr->msg_len = 0;
  pHdr->msg_total += hdr.msg_len;

  if (pHdr->msg_total == hdr.msg_total) {
    m_mapDataEx.erase(it); //完成
    CJob* pJob = CPMNew(CMemPool::GetInstance()) CDataRecvJob(m_SOCK, pRetMem, pHdr->msg_total , NULL, m_user);
    m_pThreadPool->SetJob(pJob);
  }

  m_PackHdr[0] = 0;
  return true;
}
void CDataRecvProcess::RecvFrom()
{
  addr_in Addr = {0};
  int32 tmp[(sizeof(SOCK_DATA_HEADER) + PACK_SIZE) >> 2 ] = {0};
  int32 nLen = sock_recvfrom(m_SOCK, (char*)tmp, sizeof(SOCK_DATA_HEADER) + PACK_SIZE, &Addr, 0);
  SOCK_DATA_HEADER& hdr = *(SOCK_DATA_HEADER*)tmp;

  if (0 != sock_checksum((uint16*)&hdr, sizeof(SOCK_DATA_HEADER))) {
    return;
  }

  DATA_MAP_EX::iterator it = m_mapDataEx.find(hdr.msg_identify);
  int8* pRetMem = NULL;

  if (it == m_mapDataEx.end()) {
    pRetMem = CMemPool::GetInstance().GetMem(hdr.msg_total + sizeof(SOCK_DATA_HEADER) + sizeof(uint32) + 4, false);

    if (NULL != pRetMem) {
      SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*)pRetMem;
      memset(pHdr , 0 , sizeof(SOCK_DATA_HEADER));
      pHdr->msg_identify = hdr.msg_identify;
      std::pair<DATA_MAP_EX::iterator, BOOL> IT = m_mapDataEx.insert(std::pair<uint32, int8*>(hdr.msg_identify, pRetMem));
      it = IT.first;
    }
  }
  else {
    pRetMem = it->second;
  }

  if (pRetMem == NULL) {
    return ;
  }

  //assert((pRetMem & 3) == 0);
  SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*)pRetMem;
  pHdr->msg_total += hdr.msg_len;
  int8* pMem = pRetMem + sizeof(SOCK_DATA_HEADER) + sizeof(uint32);
  memcpy(pMem + hdr.msg_offset, (char*)tmp + sizeof(SOCK_DATA_HEADER), hdr.msg_len);
  uint32* pnInitTime = (uint32*)(pHdr + 1);
#ifdef WIN32
  *pnInitTime =::GetTickCount();
#else
  *pnInitTime = m_Timer.Elapsed();
#endif

  if (pHdr->msg_total == hdr.msg_total) {
    m_mapDataEx.erase(it); //完成
    CJob* pJob = CPMNew(CMemPool::GetInstance()) CDataRecvJob(m_SOCK, pRetMem, pHdr->msg_total, &Addr, m_user);
    m_pThreadPool->SetJob(pJob);
    //DEBUG_INFO2( "RecvFrom:: %d %d" , pHdr->msg_identify, pHdr->msg_total );
  }
}
/////////////////////////////////////////////////////////////////////
//函数名称: CheckTimeOut
//创建时间: 2011/07/08 17:23
//参 数:
//文件作者: lcy
//功能说明: 检测数据是否有超时未到达的
//////////////////////////////////////////////////////////////////////
void CDataRecvProcess::CheckTimeOut(int32 nTimeOut)
{
  DATA_MAP_EX::iterator it = m_mapDataEx.begin();
#ifdef WIN32
  uint32 uCurent = ::GetTickCount();
#else
  uint32 uCurent = m_Timer.Elapsed();
#endif

  for (; it != m_mapDataEx.end();) {
    uint32* puElapse = (uint32*)(it->second + sizeof(SOCK_DATA_HEADER)); //+ nTimeOut;

    if ((*puElapse + nTimeOut) <= uCurent) { //数据超时，释放到其占有的内存
      CMemPool::GetInstance().FreeMem(it->second);
      m_mapDataEx.erase(it++);
    }
    else {
      ++it;
    }
  }
}

