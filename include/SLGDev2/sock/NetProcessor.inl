#include "NetProcessor.h"
#include "ThreadPool.h"
#include "MemPool.h"
#include "SockHandle.h"
#ifdef WIN32
#include "sftime.inl"
#endif
CNetProcessor::CNetProcessor(uint32 uMemPoolSize, uint32 uMemPoolUnit)
{
  CMemPool::GetInstance().CreateMemPool((uMemPoolSize + sizeof(SOCK_DATA_HEADER))*MEMRESERVER_BLOCKS, uMemPoolUnit);
#ifdef WIN32
  SYSTEM_INFO info = { 0 };
  ::GetSystemInfo(&info);
  //暂时根据内存池的大小，判定是多线程回调还是单线程回调
  m_pThreadPool = new CThreadPool(uMemPoolSize < 4 * 1024 * 1024 ? 1 : info.dwNumberOfProcessors + 2);
#else
  m_pThreadPool = new CThreadPool(1);
#endif
}
CNetProcessor::~CNetProcessor()
{
  delete m_pThreadPool;
}
void CNetProcessor::StopRecv(SOCKET sock, void* user)
{
  CNetManager_OnRecv(sock, NULL, 0, NULL, user);
}
void CNetProcessor::StopListen(SOCKET sock, void* user)
{
  CNetManager_OnRecv(sock, NULL, 0, NULL, user);
}
void CNetProcessor::Accept(SOCKET sock, const addr_in* Addr, void* user)
{
  CNetManager_OnListen(sock, Addr, user);
}

