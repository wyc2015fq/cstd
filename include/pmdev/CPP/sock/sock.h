
#ifndef __MY_SOCK_H__
#define __MY_SOCK_H__

#ifdef WIN32
#ifdef SOCK_EXPORTS
#define SOCK_API 
#else
#define SOCK_API 
#endif
#define SOCK_CALL
#else
#define SOCK_CALL
#define SOCK_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define MEMRESERVER_SIZE (256*1024)
#define MEMRESERVER_UNIT (8*1024)
#define NET_SOCK_STREAM 1
#define NET_SOCK_DGRAM  2

  typedef void* HANDLE;

  typedef enum {
    NET_NONE       = 0x00,
    NET_OVERLAPPED,
    NET_SELECTED
  } NET_MODEL;

  typedef struct {
    unsigned short Port;                //端口
    char           IP_Address[16];      //点分十进制IP地址
  } SOCK_ADDRESS, *PSOCK_ADDRESS;

  typedef void (*ListenCallBack)(HANDLE hSOCK, const SOCK_ADDRESS& Addr, void* pvContext);
  typedef void (*DataRecvCallBack)(HANDLE hSOCK, const char* pbData, int nLen, void* pvContext);
  typedef void (*DataRecvFromCallBack)(HANDLE hSOCK, const char* pbData, int nLen, const SOCK_ADDRESS& addr, void* pvContext);

  /****************************************************/
  /*函数名称 CPM_InitSock
  /*功    能 初始化sock库
  /*参    数
  /*返    回
  /*说    明  此接口用于初始化客户端并设定预留内存大小，此大小用于申请的内存池
        库默认大小为192KB，在数据接受过程中将会使用此内存池
  /****************************************************/
  SOCK_API BOOL CPM_InitSock(NET_MODEL NetModel, unsigned int uMemPoolSize = MEMRESERVER_SIZE, unsigned int uMemPoolUnit = MEMRESERVER_UNIT);

  /****************************************************/
  /*函数名称 CPM_UnInitSock
  /*功    能 反初始化sock库
  /*参    数
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API void CPM_UnInitSock();

  /****************************************************/
  /*函数名称 CPM_SetListenCallBack
  /*功    能 服务器网络侦听回调设置
  /*参    数 lpfnListen 回调函数指针
         pvContext 回调函数上下文
  /*返    回
  /*说    明 当侦听到有客户端连接时回调给外部
  /****************************************************/
  SOCK_API void CPM_SetListenCallBack(ListenCallBack lpfnListen, void* pvContext);

  /****************************************************/
  /*函数名称 CPM_SetDataRecvCallBack
  /*功    能 数据接受回调
  /*参    数 lpfnRecv 回调函数指针
         pvContext 回调函数上下文
  /*返    回
  /*说    明 当接受到客户端发送过来的数据时回调给外部(TCP)
  /****************************************************/
  SOCK_API void CPM_SetDataRecvCallBack(DataRecvCallBack lpfnRecv, void* pvContext) ;

  /****************************************************/
  /*函数名称 CPM_SetDataRecvFromCallBack
  /*功    能 数据接受回调
  /*参    数 lpfnRecv 回调函数指针
         pvContext 回调函数上下文
  /*返    回
  /*说    明 当接受到网络对方发送过来的数据时回调给外部(UDP)
  /****************************************************/
  SOCK_API void CPM_SetDataRecvFromCallBack(DataRecvFromCallBack lpfnRecv, void* pvContext) ;

  /****************************************************/
  /*函数名称 CPM_StartWork
  /*功    能 启动套接字数据处理工作流
  /*参    数 bStartUp 启动与否
  /*返    回
  /*说    明 当接受到客户端发送过来的数据时回调给外部
  /****************************************************/
  SOCK_API BOOL CPM_StartWork(BOOL bStartUp);

  /****************************************************/
  /*函数名称 CPM_CreateSock
  /*功    能 创建套接字
  /*参    数 nSOCKType 套接字类型（字节流或数据报）
  /*返    回 套接字句柄
  /*说    明
  /****************************************************/
  SOCK_API HANDLE CPM_CreateSock(int nSOCKType);  //创建套接字

  /****************************************************/
  /*函数名称 CPM_ReleaseSock
  /*功    能 释放套接字
  /*参    数 hSOCK 套接字句柄
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API void CPM_ReleaseSock(HANDLE hSOCK);   //释放套接字

  /****************************************************/
  /*函数名称 CPM_Bind
  /*功    能 绑定帧听端口
  /*参    数 hSOCK 套接字句柄
         Addr  侦听地址
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API BOOL CPM_Bind(HANDLE hSOCK, const SOCK_ADDRESS& Addr);

  /****************************************************/
  /*函数名称 CPM_Connect
  /*功    能 连接某个地址
  /*参    数 hSOCK 套接字句柄
         Addr  服务地址
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API BOOL CPM_Connect(HANDLE hSOCK, const SOCK_ADDRESS& Addr);

  /****************************************************/
  /*函数名称 CPM_AddInIPGroup
  /*功    能 加入多播组
  /*参    数 hSOCK 套接字句柄
         GroupIPAddr  组播地址
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API BOOL CPM_AddInIPGroup(HANDLE hSOCK, const SOCK_ADDRESS& GroupIPAddr);

  /****************************************************/
  /*函数名称 CPM_DropOutIPGroup
  /*功    能 退出多播组
  /*参    数 hSOCK 套接字句柄
         GroupIPAddr  组播地址
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API void CPM_DropOutIPGroup(HANDLE hSOCK, const SOCK_ADDRESS& GroupIPAddr);

  /****************************************************/
  /*函数名称 CPM_Listen
  /*功    能 启动帧听
  /*参    数 hSOCK 套接字句柄
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API void CPM_Listen(HANDLE hSOCK);


  /****************************************************/
  /*函数名称 CPM_SetSendBufSize
  /*功    能 设置发送缓冲大小
  /*参    数 hSOCK 套接字句柄
         nSize 缓冲大小
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API void CPM_SetSendBufSize(HANDLE hSOCK, int nSize);

  /****************************************************/
  /*函数名称 CPM_SetRecvBufSize
  /*功    能 设置接受缓冲大小
  /*参    数 hSOCK 套接字句柄
         nSize 缓冲大小
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API void CPM_SetRecvBufSize(HANDLE hSOCK, int nSize);

  /****************************************************/
  /*函数名称 CPM_Send
  /*功    能 发送数据
  /*参    数 hSOCK 套接字句柄
         pbData 数据内存
         lpLen 数据内存大小
  /*返    回
  /*说    明 TCP协议发送数据
  /****************************************************/
  SOCK_API BOOL CPM_Send(HANDLE hSOCK, const char* pbData, int* lpLen);

  /****************************************************/
  /*函数名称 CPM_SendTo
  /*功    能 发送数据
  /*参    数 hSOCK 套接字句柄
         addr  目的地址
         pbData 数据内存
         lpLen 数据内存大小
  /*返    回
  /*说    明 UDP发送数据
  /****************************************************/
  SOCK_API BOOL CPM_SendTo(HANDLE hSOCK, const SOCK_ADDRESS& addr, const char* pbData, int* lpLen);

  /****************************************************/
  /*函数名称 CPM_Recv
  /*功    能 数据接受
  /*参    数 hSOCK 套接字句柄
  /*返    回
  /*说    明  调用此接口后，当hSOCK有数据来时会以回调方式通知给外部
  /****************************************************/
  SOCK_API void CPM_Recv(HANDLE hSOCK);  //数据接受，调用此接口后，当psock有数据来时会以回调方式通知给外部

  /****************************************************/
  /*函数名称 CPM_GetPeerAddr
  /*功    能 获取对等方地址
  /*参    数 hSOCK 套接字句柄
  /*返    回 pAddr 对等方地址
  /*说    明
  /****************************************************/
  SOCK_API BOOL CPM_GetPeerAddr(HANDLE hSOCK, PSOCK_ADDRESS pAddr);

  /****************************************************/
  /*函数名称 CPM_GetLocalIP
  /*功    能 获取本地IP
  /*参    数 hSOCK 套接字句柄
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API BOOL CPM_GetLocalIP(HANDLE hSOCK, PSOCK_ADDRESS pAddr);

  /****************************************************/
  /*函数名称 CPM_GetSockName
  /*功    能 获取本地IP
  /*参    数 hSOCK 套接字句柄
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API BOOL CPM_GetSockName(HANDLE hSOCK, PSOCK_ADDRESS pAddr);


  /****************************************************/
  /*函数名称 CPM_GetLastError
  /*功    能 获取网络错误
  /*参    数
  /*返    回
  /*说    明
  /****************************************************/
  SOCK_API int  CPM_GetLastError(); //获取网络错误

#ifdef __cplusplus
}
#endif

#endif