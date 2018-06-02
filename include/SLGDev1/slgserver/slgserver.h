
////////////////////////////////////////////////////////////////////*/
//工程名称: SLGServer.dll
//功    能: 服务端sock通信，使用TCP传输服务跟服务端通信,用于dsp板子数据传输
//说    明: 此库已经包含了数据的拆包和组包过程，上层应用无需拆包和解包
//            服务端只跟一个客户端连接
//时    间: 2010/11/25
////////////////////////////////////////////////////////////////////*/

#ifndef __SLGSERVER_SOCK_H__
#define __SLGSERVER_SOCK_H__

#include "../slgdev.h"

#ifdef WIN32
#ifdef SLGSERVER_EXPORTS
#define SLGSERVER_API
#else
#define SLGSERVER_API
#endif
#else
#define SLGSERVER_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define MEMRESERVER_SIZE   (256*1024) //内存大小

#define MULTYGROUP_PORT  13332           //多播组端口
#define MULTYGROUP_IP    "224.0.1.100"   //多播组地址
#define UDP_PORT     13334
#define OFFSET_SIZE    20

    typedef enum {
        SERVER_OVERLAPPED = 0x01,
        SERVER_SELECTED,
    }
    SERVER_MODEL;

    /////////////////////////////////////////////////////////////////////////////////////
    //SRecvCallBack 数据接收回调指针
    //功    能 内部主动接受套接字上发送过来的数据
    //参    数
    //           buf   数据缓存
    //       len   数据大小
    //       para  应用上下文
    //说    明 当套接字上有数据发送过来时，内部将收到的数据发送给上层应用
    //           当len=0时，表明对等方已经关闭网络连接
    /////////////////////////////////////////////////////////////////////////////////////
    typedef void (*SRecvCallBack)(SOCKET hSOCK, const char* pbData, unsigned int uLen, void* pvContext);

    /////////////////////////////////////////////////////////////////////////////////////
    //SRecvCallBack 数据接收回调指针
    //功    能 内部主动接受套接字上发送过来的数据
    //参    数
    //           buf   数据缓存
    //       len   数据大小
    //       para  应用上下文
    //说    明 当套接字上有数据发送过来时，内部将收到的数据发送给上层应用
    //           当len=0时，表明对等方已经关闭网络连接
    /////////////////////////////////////////////////////////////////////////////////////
    typedef void (*SRecvFromCallBack)(SOCKET hSOCK, const char* pbData, unsigned int uLen, const addr_in* Addr, void* pvContext);

    /////////////////////////////////////////////////////////////////////////////////////
    //SCNCallBack 客户端连接状态回调
    //功    能 客户端连接提醒
    //参    数 hSOCK  跟客户端连接套接字句柄
    //                       flag  连接状态(true 建立连接，false 断开连接)
    //                       para  应用上下文
    // 返   回 套接字加密类型 <0 放弃此次连接 0不进行加密 1与客户端进行SSL加密连接
    //说    明
    /////////////////////////////////////////////////////////////////////////////////////
    typedef int (*SCNCallBack)(SOCKET hSOCK, BOOL bCN, void* pvContext);

    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_Init
    //功    能 初始化服务端
    //参    数
    //返    回
    //说    明  此接口用于初始化客户端并设定预留内存大小，此大小用于申请的内存池
    //      库默认大小为192KB，在数据接受过程中将会使用此内存池
    //      应用层根据实际设置一个预留
    //        此值必须大于或等于待接受数据的最大值，否则数据接受将不完整
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_Init(int af, unsigned int uSize = MEMRESERVER_SIZE, SERVER_MODEL eModel = SERVER_SELECTED);

    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_UnInit
    //功    能 反初始化客户端
    //参    数
    //返    回
    //说    明
    ////////////////////////////////////////////////////*/
    SLGSERVER_API void SLG_S_UnInit();

    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_SetRecvCallBack
    //功    能 设置数据接收回调函数
    //参    数 cb     回调函数
    //           param  回调上下文
    //返    回
    //说    明
    ////////////////////////////////////////////////////*/
    SLGSERVER_API void SLG_S_SetRecvCallBack(SRecvCallBack lpfnRecv, void* pvContext);

    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_SetRecvFromCallBack
    //功    能 设置数据接收回调函数
    //参    数 cb     回调函数
    //           param  回调上下文
    //返    回
    //说    明
    ////////////////////////////////////////////////////*/
    SLGSERVER_API void SLG_S_SetRecvFromCallBack(SRecvFromCallBack lpfnRecv, void* pvContext);


    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_SetCNCallBack
    //功    能 设置服务器连接状态提醒回调
    //参    数 cb     回调函数
    //           param  回调上下文
    //返    回
    //说    明
    ////////////////////////////////////////////////////*/
    SLGSERVER_API void SLG_S_SetCNCallBack(SCNCallBack lpfnCN, void* pvContext);


    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_AddInMultyGroup
    //功    能 加入多播组
    //参    数 flag 加入与否( true 加入addr所标识的组 false 退出addr所标识的组)
    //返    回 true  成功
    //           false 失败
    //说    明
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_AddInMultyGroup(BOOL bJoinIn, const addr_in* Addr, CSockBase* user);

    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_StartListen
    //功    能 启动服务器侦听与否
    //参    数 flag 启动与否( true 启动 false 停止)
    //返    回 true  成功
    //           false 失败
    //说    明
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_StartListen(BOOL bStartUp);

    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_StartWork
    //功    能 启动工作流程与否
    //参    数 flag
    //              true  启动
    //      false 停止
    //返    回 true  成功
    //     false 失败
    //说    明 启动工作流程后，内部将数据收发结果通知给外部
    //     停止工作流程后，内部不再惊醒数据收发
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_StartWork(BOOL bStartUp);

    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_CreateUDPSock
    //功    能 创建套接字
    //参    数 pAddr 套接字绑定地址
    //返    回 套接字句柄
    //说    明
    ////////////////////////////////////////////////////*/
    SLGSERVER_API SOCKET SLG_S_CreateUDPSock(const addr_in* pAddr); //创建套接字

    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_SendData
    //功    能 发送数据
    //参    数 hSock       套接字句柄
    //           buf         缓存数据
    //       lpLen       缓存数据大小
    //返    回
    //说    明
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_SendData(SOCKET hSOCK, const char* pbData, unsigned int* lpLen);

    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_SendData
    //功    能 发送数据
    //参    数 hSock       套接字句柄
    //           buf         缓存数据
    //       lpLen       缓存数据大小
    //返    回
    //说    明
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_SendDataTo(SOCKET hSOCK, const char* pbData, unsigned int* lpLen, const addr_in* Addr);


    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_DCN
    //功    能  断开连接
    //参    数
    //返    回
    //说    明
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_DCN(SOCKET hSOCK);

    ////////////////////////////////////////////////////*/
    //函数名称 SLG_S_Address
    //功    能 获取对等方IP地址
    //参    数 hSock       套接字句柄
    //           ip          对等方IP地址(形如192.168.1.1)
    //返    回
    //说    明
    ////////////////////////////////////////////////////*/
    SLGSERVER_API BOOL SLG_S_Address(SOCKET hSOCK, addr_in* addr);

#ifdef __cplusplus
}
#endif

#endif