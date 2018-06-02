// SockHandle.cpp: implementation of the CSockHandle class.
//
//////////////////////////////////////////////////////////////////////

//#include "StdAfx.h"
#include "SockHandle.h"
#include "NetManager.h"
#include "Socket.h"
#include "MemPool.h"
#include "ThreadPool.h"
#ifdef WIN32
#include "sftime.hpp"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CNetManager* gNetManager;

class CDataRecvJob : public CJob
{
public:
  CDataRecvJob(SOCKET m_SOCK, const int8* pData, int32 nSize, const SOCK_ADDRESS* pAddr = NULL);
  virtual ~CDataRecvJob();
public:
  virtual void Run();
private:
  SOCKET m_hSOCK;
  const int8*  m_pData;
  int    m_nSize;
  SOCK_ADDRESS m_Addr;
  BOOL   m_bAddrValide;
};

CDataRecvJob::CDataRecvJob(SOCKET m_SOCK, const int8* pData, int32 nSize, const SOCK_ADDRESS* pAddr) :
  m_hSOCK(m_SOCK),
  m_pData(pData),
  m_nSize(nSize),
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
  gNetManager->OnRecv(m_hSOCK, m_pData + sizeof(SOCK_DATA_HEADER) + sizeof(uint32), m_nSize, m_bAddrValide ? &m_Addr : NULL);
}


CDataRecvProcess::CDataRecvProcess(SOCKET sock, CThreadPool* pThreadPool) :
  m_SOCK(sock),
  m_pThreadPool(pThreadPool),
  m_lIdentify(0),
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
}

long CDataRecvProcess::AddRef(void)
{
#ifndef WIN32
  AoutLock lock(&m_SynRef);
  return ++m_RefCount;
#else
  return InterlockedIncrement(&m_RefCount);
#endif
}

long CDataRecvProcess::Release(void)
{
#ifndef WIN32
  long rs = 0;
  m_SynRef.Lock();
  rs = --m_RefCount;
  m_SynRef.Unlock();

  if (0 == rs) {
    delete this;
  }

  return rs;
#else
  long rs = InterlockedDecrement(&m_RefCount);

  if (0 == rs) {
    delete this;
  }

  return rs;
#endif
}

int32 CDataRecvProcess::Send(const int8* pbData, int32 nLen, const SOCK_ADDRESS* pAddr)
{
  //同一个连接，采用同步发送机制
#define MAX_SEQ 0x7fffffff
  AoutLock lock(&m_synIdentfiy);
  int32 nSeq = ((m_lIdentify++) % MAX_SEQ);
  return ::Send(m_SOCK, pbData, nLen, nSeq, pAddr);
}

void CDataRecvProcess::Accept()
{
  SOCK_ADDRESS Addr = {0};
  SOCKET accepted = sock_accept(m_SOCK, Addr.IP_Address, Addr.Port);

  if (INVALID_SOCKET != accepted) {
    gNetManager->OnListen(accepted, Addr);
  }
}

BOOL CDataRecvProcess::Recv()
{
  int32 nLen = 0 , nError = 0;
  int32 nRecvBytes = sizeof(SOCK_DATA_HEADER) - m_PackHdr[0];
  char* pRecvHdr = (char*)(m_PackHdr + 1);
  SOCK_DATA_HEADER& hdr = *(SOCK_DATA_HEADER*)pRecvHdr;

  if (nRecvBytes) {
    nLen = sock_recv(m_SOCK, pRecvHdr + m_PackHdr[0], nRecvBytes);
    
    printf("recv %d\n", nLen);
    if (nLen < 0) {
      nError = sock_error();
      DEBUG_INFO3("1---nError = %d , nLen = %d  nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
      return WSAEINTR == nError || WSAEWOULDBLOCK == nError;
    }
    else if (nLen == 0) {
      DEBUG_INFO3("2---nError = %d , nLen = %d  nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
      return false;
    }

    m_PackHdr[0] += nLen;

    if (m_PackHdr[0] < sizeof(SOCK_DATA_HEADER)) {
      return true;
    }

    if (0 != sock_checksum((uint16*)&hdr, sizeof(SOCK_DATA_HEADER))) {
      DEBUG_INFO1("%s", "sock_checksum err \r\n");
      return false;
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

  assert((long)pRetMem % 4  == 0);

  SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*)pRetMem;
  uint32* pnInitTime = (uint32*)(pHdr + 1);
  *pnInitTime = m_Timer.counter();

  int8* pMem = pRetMem + sizeof(SOCK_DATA_HEADER) + sizeof(uint32);
  nLen = sock_recv(m_SOCK, pMem + hdr.msg_offset + pHdr->msg_len, ALIGN(hdr.msg_len, 4) - pHdr->msg_len);

  if (nLen < 0) {
    nError = sock_error();
    printf("3---nError = %d , nLen = %d  nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
    return WSAEINTR == nError || WSAEWOULDBLOCK == nError;
  }
  else if (nLen == 0) {
    printf("4---nError = %d , nLen = %d  nRecvBytes = %d\r\n" , nError , nLen , nRecvBytes);
    return false;
  }

  pHdr->msg_len += nLen;

  if (pHdr->msg_len < ALIGN(hdr.msg_len, 4)) {
    return true;//小包未接收完成
  }

  pHdr->msg_len = 0;
  pHdr->msg_total += hdr.msg_len;

  if (pHdr->msg_total == hdr.msg_total) {
    m_mapDataEx.erase(it);  //完成
    CJob* pJob = CPMNew(CMemPool::GetInstance()) CDataRecvJob(m_SOCK, pRetMem, pHdr->msg_total, NULL);
    m_pThreadPool->SetJob(pJob);
  }

  m_PackHdr[0] = 0;
  return true;
}

void CDataRecvProcess::RecvFrom()
{
  SOCK_ADDRESS Addr = {0};
  int32 tmp[(sizeof(SOCK_DATA_HEADER) + PACK_SIZE) >> 2 ] = {0};
  int32 nLen = sock_recvfrom(m_SOCK, (char*)tmp, sizeof(SOCK_DATA_HEADER) + PACK_SIZE, Addr.IP_Address, &Addr.Port , 0);
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

  assert((long)pRetMem % 4  == 0);

  SOCK_DATA_HEADER* pHdr = (SOCK_DATA_HEADER*)pRetMem;
  pHdr->msg_total += hdr.msg_len;
  int8* pMem = pRetMem + sizeof(SOCK_DATA_HEADER) + sizeof(uint32);
  memcpy(pMem + hdr.msg_offset, (char*)tmp + sizeof(SOCK_DATA_HEADER), hdr.msg_len);

  uint32* pnInitTime = (uint32*)(pHdr + 1);
  *pnInitTime = m_Timer.counter();

  if (pHdr->msg_total == hdr.msg_total) {
    m_mapDataEx.erase(it);  //完成
    CJob* pJob = CPMNew(CMemPool::GetInstance()) CDataRecvJob(m_SOCK, pRetMem, pHdr->msg_total, &Addr);
    m_pThreadPool->SetJob(pJob);
    DEBUG_INFO2("RecvFrom:: %d %d" , pHdr->msg_identify,  pHdr->msg_total);
  }
}

/********************************************************************
/*函数名称: CheckTimeOut
/*创建时间: 2011/07/08 17:23
/*参  数:
/*文件作者: lcy
/*功能说明: 检测数据是否有超时未到达的
*********************************************************************/

void CDataRecvProcess::CheckTimeOut(int32 nTimeOut)
{
  DATA_MAP_EX::iterator it = m_mapDataEx.begin();
  uint32 uCurent = m_Timer.counter();

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

BOOL  Send(SOCKET sock, int8* data, int32 len, const SOCK_ADDRESS* pAddr);
int32 Send(SOCKET sock, const int8* data, int32 len, uint32 nSeq, const SOCK_ADDRESS* pAddr)
{
#define OFFSET_SIZE 20

  if (NULL == data || len <= 0) {
    return 0;
  }

  uint32 pieces = len / PACK_SIZE;
  uint32 arith = len % PACK_SIZE;

  int32 nTotalSend = 0;
  BOOL bOK  = true;
  char* msg = (char*)data;
  uint32 i  = 0;
  
  for (i=0; i < pieces; ++i) {
    SOCK_DATA_HEADER* hdr = (SOCK_DATA_HEADER*)(msg - OFFSET_SIZE);
    hdr->msg_identify = nSeq;   //标识
    hdr->msg_offset   = i * PACK_SIZE ; //偏移
    hdr->msg_len    = PACK_SIZE;      //数据长度
    hdr->msg_total    = len;        //数据总长度
    hdr->msg_checksum = 0;        //检验和
    hdr->msg_checksum = sock_checksum((uint16*)hdr, sizeof(SOCK_DATA_HEADER));
    int32 nSend = sizeof(SOCK_DATA_HEADER) + hdr->msg_len;
    
    if (!Send(sock, (char*)hdr, nSend, pAddr)) {
      bOK = false;
      break;
    }
    else {
      nTotalSend += PACK_SIZE;
      msg += PACK_SIZE;
    }

    SLEEP(pAddr ? MILLISEC * 8 : MILLISEC * 5);
  }

  if (!bOK) {
    return nTotalSend;
  }

  if (arith) {
    SOCK_DATA_HEADER* hdr = (SOCK_DATA_HEADER*)(msg - OFFSET_SIZE);
    hdr->msg_identify = nSeq;   //标识
    hdr->msg_offset   = i * PACK_SIZE ; //偏移
    hdr->msg_len    = arith;      //数据长度
    hdr->msg_total    = len;        //数据总长度
    hdr->msg_checksum = 0;        //检验和
    hdr->msg_checksum = sock_checksum((uint16*)hdr, sizeof(SOCK_DATA_HEADER));
    int32 nSend = sizeof(SOCK_DATA_HEADER) + ALIGN(hdr->msg_len, 4) ;
    
    if (!Send(sock, (char*)hdr, nSend, pAddr)) {
      bOK = false;
    }
    else {
      nTotalSend += arith;
    }
  }

  return nTotalSend;
}

static int logprintf( const char* fmt, ... ) {
  FILE* pf;
  pf = fopen("log.txt", "a");
  if (pf) {
    va_list ARGLIST;
    va_start( ARGLIST, fmt );
    vfprintf( pf, fmt, ARGLIST );
    fflush( pf );
    va_end( ARGLIST );
    fclose(pf);
  }
  return 0;
}
BOOL Send(SOCKET sock, int8* data, int32 len, const SOCK_ADDRESS* pAddr)
{
  int32 nTotalSends = len;
  BOOL  bOK = false;
  int32 send_counts = 0;
  int i;
#if 0
  logprintf("send %d:", len);
  for (i=0; i<len; ++i) {
    logprintf("%02x ", ((unsigned char*)data)[i]);
  }
  logprintf("\n");
#endif


  while (send_counts++ < MAX_SEND_COUNTS) {
    WRITE_TIME_OUT(sock , 300);

    if (bTimeOut) {
      continue;
    }

    int32 nHasSend = sock_send(sock, data, nTotalSends, pAddr ? pAddr->IP_Address : NULL, pAddr ? pAddr->Port : 0);

    if (nHasSend <= 0) {
      int32 nError = sock_error();
      DEBUG_INFO3("3---nError = %d , len = %d  nHasSend = %d\r\n" , nError , len , nHasSend);

      if (WSAEINTR == nError || WSAEWOULDBLOCK == nError) {
        continue;
      }
      else {
        break;
      }
    }

    data += nHasSend;
    nTotalSends -= nHasSend;

    if (nTotalSends <= 0) {
      bOK = true;
      break;
    }
  }

  return bOK;
}

