#include "netprocessor.h"
#include "threadpool.h"
#include "sockhandle.h"
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

void CNetProcessor::StopRecv(SOCKET sock, CSockBase* user)
{
    CNetManager_OnRecv(sock, NULL, 0, NULL, user);
}

void CNetProcessor::StopListen(SOCKET sock, CSockBase* user)
{
    CNetManager_OnRecv(sock, NULL, 0, NULL, user);
}

void CNetProcessor::Accept(SOCKET sock, const addr_in* Addr, CSockBase* user)
{
    CNetManager_OnListen(sock, Addr, user);
}

