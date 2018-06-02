//#include "StdAfx.h"
#include "NetManager.h"
#include "OverlappedProcessor.h"
#include "SelectProcessor.h"
#include "Socket.h"

CNetManager::CNetManager() :
  m_pProc(NULL),
  m_lpfnListen(NULL),
  m_lpListenContext(NULL),
  m_lpfnRecv(NULL),
  m_lpRecvContext(NULL),
  m_lpfnRecvFrom(NULL),
  m_lpRecvFromContext(NULL)
{
}

CNetManager::~CNetManager()
{
  UnInit();
}

/********************************************************************
/*函数名称: Init
/*创建时间: 2011/07/08 16:44
/*参  数: Model 套接字处理模型
      nMemPoolSize 内存池大小
/*文件作者: lcy
/*功能说明: 创建套接字模型
*********************************************************************/
BOOL CNetManager::Init(NET_MODEL Model, uint32 uMemPoolSize, uint32 uMemPoolUnit)
{
  if (m_pProc) {
    return true;
  }

  try {
    if (NET_OVERLAPPED == Model) {
#ifdef WIN32

      //if ( NULL == m_pProc ) m_pProc = new COverlappedProcessor( uMemPoolSize, uMemPoolUnit );
      if (NULL == m_pProc) {
        m_pProc = new CSelectProcessor(uMemPoolSize, uMemPoolUnit);
      }

#else

      if (NULL == m_pProc) {
        m_pProc = new CSelectProcessor(uMemPoolSize, uMemPoolUnit);
      }

#endif
    }
    else if (NET_SELECTED == Model) {
      if (NULL == m_pProc) {
        m_pProc = new CSelectProcessor(uMemPoolSize, uMemPoolUnit);
      }
    }
  }
  catch (std::bad_alloc) {
    return false;
  }

  return m_pProc != NULL;
}

/********************************************************************
/*函数名称: UnInit
/*创建时间: 2011/07/08 16:46
/*参  数:
/*文件作者: lcy
/*功能说明: 释放套接字模型
*********************************************************************/
void CNetManager::UnInit()
{
  if (m_pProc) {
    delete m_pProc;
    m_pProc = NULL;
  }

  m_lpfnListen = NULL;
  m_lpListenContext = NULL;
  m_lpfnRecv = NULL;
  m_lpRecvContext = NULL;
}

/********************************************************************
/*函数名称: CreateSocket
/*创建时间: 2011/07/08 16:46
/*参  数: type 套接字类型(字节流或数据报)
/*文件作者: lcy
/*功能说明: 创建套接字
*********************************************************************/
SOCKET CNetManager::CreateSocket(int32 type)
{
  return sock_create(type);
}

/********************************************************************
/*函数名称: ReleaseSocket
/*创建时间: 2011/07/08 16:47
/*参  数: sock 套接字
/*文件作者: lcy
/*功能说明: 释放套接字
*********************************************************************/
void CNetManager::ReleaseSocket(SOCKET sock)
{
  sock_close(sock);
  m_pProc->StopRecv(sock);
}

/********************************************************************
/*函数名称: Connect
/*创建时间: 2011/07/08 16:47
/*参  数: sock 套接字
PeerAddr 服务器地址
/*文件作者: lcy
/*功能说明: 连接服务器
*********************************************************************/
BOOL CNetManager::Connect(SOCKET sock, const SOCK_ADDRESS& PeerAddr)
{
  sock_ioctl(sock,  FIONBIO , true);
  sock_connect(sock, PeerAddr.IP_Address, PeerAddr.Port);
  WRITE_TIME_OUT(sock, (3 * 1000));
  return !bTimeOut;
}

/********************************************************************
/*函数名称: Bind
/*创建时间: 2011/07/08 16:48
/*参  数: sock 套接字
      LocalAddr 本地地址
/*文件作者: lcy
/*功能说明: 将套接字绑定到本地地址
*********************************************************************/
BOOL CNetManager::Bind(SOCKET sock, const SOCK_ADDRESS& LocalAddr)
{
  return sock_bind(sock, LocalAddr.IP_Address, LocalAddr.Port) ;
}

/********************************************************************
/*函数名称: Listen
/*创建时间: 2011/07/08 16:49
/*参  数: sock 套接字
      bStartUp 启动侦听与否
/*文件作者: lcy
/*功能说明: 启动或停止套接字侦听
*********************************************************************/
void CNetManager::Listen(SOCKET sock)
{
  sock_listen(sock, 5);
  m_pProc->StartListen(sock);
}

/********************************************************************
/*函数名称: JoinIPGroup
/*创建时间: 2011/07/08 16:50
/*参  数: sock 套接字
      GroupIPAddr 多播组地址
/*文件作者: lcy
/*功能说明: 加入多播组
*********************************************************************/
BOOL CNetManager::JoinIPGroup(SOCKET sock, const SOCK_ADDRESS& GroupIPAddr)
{
  return sock_bind(sock, NULL, GroupIPAddr.Port) && sock_join_group(sock, GroupIPAddr.IP_Address);
}

/********************************************************************
/*函数名称: DropIPGroup
/*创建时间: 2011/07/08 16:50 s
/*参  数: sock 套接字
      GroupIPAddr 多播组地址
/*文件作者: lcy
/*功能说明: 退出多播组
*********************************************************************/
void CNetManager::DropIPGroup(SOCKET sock, const SOCK_ADDRESS& GroupIPAddr)
{
  sock_drop_group(sock, GroupIPAddr.IP_Address);
}

/********************************************************************
/*函数名称: SetRecvBuf
/*创建时间: 2011/07/08 16:52
/*参  数: sock 套接字
      nSize 缓冲区大小
/*文件作者: lcy
/*功能说明: 设置接受缓冲区大小
*********************************************************************/
void CNetManager::SetRecvBuf(SOCKET sock, int32 nSize)
{
  sock_set_recvbuf_size(sock, nSize);
}

/********************************************************************
/*函数名称: SetSendBuf
/*创建时间: 2011/07/08 16:53
/*参  数: sock 套接字
      nSize 缓冲区大小
/*文件作者: lcy
/*功能说明: 设置发送缓冲区大小
*********************************************************************/
void CNetManager::SetSendBuf(SOCKET sock, int32 nSize)
{
  sock_set_sendbuf_size(sock, nSize);
}

/********************************************************************
/*函数名称: GetPeerAddr
/*创建时间: 2011/07/08 16:53
/*参  数: sock 套接字
      lpPeerAddr 返回地址
/*文件作者: lcy
/*功能说明: 获取对方地址
*********************************************************************/
BOOL CNetManager::GetPeerAddr(SOCKET sock, PSOCK_ADDRESS lpPeerAddr)
{
  return sock_addr(sock, lpPeerAddr->IP_Address, lpPeerAddr->Port);
}

BOOL CNetManager::GetLocalIP(SOCKET sock, PSOCK_ADDRESS lpLocalIP)
{
  return sock_DNS2LocalIP(lpLocalIP->IP_Address);
}

BOOL CNetManager::GetSockName(SOCKET sock, PSOCK_ADDRESS lpLocalIP)
{
  return sock_name(sock, lpLocalIP->IP_Address, lpLocalIP->Port);
}

/********************************************************************
/*函数名称: Send
/*创建时间: 2011/07/08 16:54
/*参  数: sock 套接字
      pbData 数据内存
      nLen   数据大小
/*文件作者: lcy
/*功能说明: 发送数据
*********************************************************************/
int32 CNetManager::Send(SOCKET sock, const int8* pbData, int32 nLen)
{
  return m_pProc->Send(sock, pbData, nLen);
}

/********************************************************************
/*函数名称: Send
/*创建时间: 2011/07/08 16:54
/*参  数: sock 套接字
      pbData 数据内存
      nLen   数据大小
/*文件作者: lcy
/*功能说明: 发送数据
*********************************************************************/
int32 CNetManager::SendTo(SOCKET sock, const int8* pbData, int32 nLen, PSOCK_ADDRESS lpAddr)
{
  return m_pProc->Send(sock, pbData, nLen, lpAddr);
}

/********************************************************************
/*函数名称: Recv
/*创建时间: 2011/07/08 16:54
/*参  数: sock 套接字
/*文件作者: lcy
/*功能说明: 启动数据接受
*********************************************************************/
void CNetManager::Recv(SOCKET sock)
{
  m_pProc->StartRecv(sock);
}

/********************************************************************
/*函数名称: StartWork
/*创建时间: 2011/07/08 16:55
/*参  数: bStartUp 启动或关闭数据处理工作流
/*文件作者: lcy
/*功能说明: 启动或关闭数据处理工作流
*********************************************************************/
BOOL CNetManager::StartWork(BOOL bStartUp)
{
  return m_pProc->StartUpWork(bStartUp);
}


/********************************************************************
/*函数名称: OnListen
/*创建时间: 2011/07/08 16:56
/*参  数: sock 套接字
      Addr 客户端地址
/*文件作者: lcy
/*功能说明: 侦听回调
*********************************************************************/
void CNetManager::OnListen(SOCKET sock, const SOCK_ADDRESS& Addr)
{
  if (m_lpfnListen) {
    m_lpfnListen((HANDLE)sock, Addr, m_lpListenContext);
  }
}

/********************************************************************
/*函数名称: OnRecv
/*创建时间: 2011/07/08 16:56
/*参  数: sock 套接字
      pbData 数据内存首地址
      nLen 数据内存大小
/*文件作者: lcy
/*功能说明: 数据接受回调
*********************************************************************/
void CNetManager::OnRecv(SOCKET sock, const int8* pbData, int32 nLen, PSOCK_ADDRESS lpAddr /*= NULL*/)
{
  if (lpAddr == NULL) {
    if (m_lpfnRecv) {
      m_lpfnRecv((HANDLE)sock, pbData, nLen, m_lpRecvContext);
    }
  }
  else {
    if (m_lpfnRecvFrom) {
      m_lpfnRecvFrom((HANDLE)sock, pbData, nLen, *lpAddr, m_lpRecvFromContext);
    }
  }
}


