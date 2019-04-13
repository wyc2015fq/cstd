
# IOCP的程序 - 高科的专栏 - CSDN博客

2011年12月02日 17:07:05[高科](https://me.csdn.net/pbymw8iwm)阅读数：2043标签：[socket																](https://so.csdn.net/so/search/s.do?q=socket&t=blog)[internet																](https://so.csdn.net/so/search/s.do?q=internet&t=blog)[thread																](https://so.csdn.net/so/search/s.do?q=thread&t=blog)[tcp																](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[io																](https://so.csdn.net/so/search/s.do?q=io&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)个人分类：[linux																](https://blog.csdn.net/pbymw8iwm/article/category/863753)
[
																								](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=thread&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=thread&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=internet&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
C代码![收藏代码](http://zhangyafeikimi.iteye.com/images/icon_star.png)
\#include <winsock2.h>
\#include <mswsock.h>
\#include <windows.h>
\#include <stdio.h>
\#include <stdlib.h>
\#include <assert.h>
\#include "vld.h"
\#pragma message("automatic link to ws2_32.lib and mswsock.lib")
\#pragma comment(lib, "ws2_32.lib")
\#pragma comment(lib, "mswsock.lib")


\#define RETURN_SUCESS (0)
\#define RETURN_FAILED (-1)
\#define PORT 5150//端口
\#define IOCP_THREAD_MAX 16//最大的线程数
\#define DATA_BUFSIZE 8192

//区别是何种完成事件
enumIO_EVENT
{
IO_EVENT_ACCEPT,
IO_EVENT_WSARECV,
IO_EVENT_WSARECVFROM,
//不支持异步发送
//IO_EVENT_WSASEND,
//IO_EVENT_WSASENDTO
};


typedefstructoverlapped_wrapper
{
//OVERLAPPED要放在第一个
OVERLAPPED overlapped;
intio_type;//指示是何种IO操作
}overlapped_wrapper;

typedefstructacceptex_block
{
//OVERLAPPED要放在第一个
OVERLAPPED overlapped;
intio_type;//指示是何种IO操作
charbuffer[DATA_BUFSIZE];
SOCKET listen_socket;
SOCKET accept_socket;
}acceptex_block;

typedefstructrecv_block
{
//OVERLAPPED要放在第一个
OVERLAPPED overlapped;
intio_type;//指示是何种IO操作
charbuffer[DATA_BUFSIZE];
SOCKET socket;
WSABUF wsa_recv_buf;
DWORDbytes_recveived;
}recv_block;

typedefstructrecvfrom_block
{
//OVERLAPPED要放在第一个
OVERLAPPED overlapped;
intio_type;//指示是何种IO操作
charbuffer[DATA_BUFSIZE];
SOCKET socket;
WSABUF wsa_recv_buf;
DWORDbytes_recveived;
//UDP包的源地址
structsockaddr_in from_address;
intfrom_address_len;
}recvfrom_block;



intget_cpu_number();
intasync_AcceptEx(acceptex_block* block);
intasync_WSARecv(recv_block* block);
intasync_WSARecvFrom(recvfrom_block* block);
voidon_acceptex(acceptex_block* block);
voidon_recv(recv_block* block);
voidon_recvfrom(recvfrom_block* block);
voidon_tcp_listen_close(acceptex_block* block);
voidon_tcp_close(recv_block* block);
voidon_udp_close(recvfrom_block* block);
intinit(void);
voiduninit(void);
DWORDWINAPI worker_thread(LPVOIDCompletionPortID);
voidexit_error();


//完成端口的句柄
HANDLEg_completion_port = INVALID_HANDLE_VALUE;
//工作线程句柄
HANDLEg_threads[IOCP_THREAD_MAX];
//工作线程数量
intg_threads_number = 0;

intmain(void)
{
/************************************************************************/
/*TCP的例子*/
/************************************************************************/
SOCKADDR_IN internet_address;
SOCKET listen_socket;
acceptex_block* block;
if(RETURN_FAILED == init())
exit_error();
if((listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
exit_error();
internet_address.sin_family = AF_INET;
internet_address.sin_addr.s_addr = htonl(INADDR_ANY);
internet_address.sin_port = htons(PORT);
if(bind(listen_socket, (PSOCKADDR) &internet_address,sizeof(internet_address)) == SOCKET_ERROR)
exit_error();
if(listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
exit_error();
printf("listening socket %d\n", PORT);
//把监听的SOCKET和完成端口绑定
if(NULL == CreateIoCompletionPort((HANDLE)listen_socket, g_completion_port, (u_long)listen_socket, 0))
exit_error();
block = (acceptex_block*)malloc(sizeof(acceptex_block));
block->listen_socket = listen_socket;
async_AcceptEx(block);
getchar();
closesocket(listen_socket);
getchar();
uninit();
return0;

/************************************************************************/
/*UDP的例子*/
/************************************************************************/
//SOCKADDR_IN internet_address;
//SOCKET sock;
//recvfrom_block* block = (recvfrom_block*)malloc(sizeof(recvfrom_block));
//if(RETURN_FAILED == init())
//  exit_error();
//if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
//  exit_error();
//internet_address.sin_family = AF_INET;
//internet_address.sin_addr.s_addr = htonl(INADDR_ANY);
//internet_address.sin_port = htons(PORT);
//if (bind(sock, (PSOCKADDR) &internet_address, sizeof(internet_address)) == SOCKET_ERROR)
//  exit_error();
//if(NULL == CreateIoCompletionPort((HANDLE)sock, g_completion_port, (u_long)sock, 0))
//  exit_error();
//block->socket = sock;
//async_WSARecvFrom(block);

//getchar();
//closesocket(sock);
//getchar();
//uninit();
//return 0;
}


intinit(void)
{
WSADATA wsa_data;
inti;
\#if defined _DEBUG || defined DEBUG
//调试时用一个线程方便
intthreads = 1;
\#else
intthreads = get_cpu_number();
\#endif

if(WSAStartup(0x0202, &wsa_data) != 0)
returnRETURN_FAILED;
//建立完成端口
if((g_completion_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
returnRETURN_FAILED;
if(threads > IOCP_THREAD_MAX)
threads = IOCP_THREAD_MAX;
for(i = 0; i < threads; i++)
{
//创建工作线程
g_threads[g_threads_number++] = CreateThread(NULL, 0, worker_thread, NULL, 0, 0);
}
returnRETURN_SUCESS;
}

voiduninit(void)
{
//自定义的退出协议,三个参数全为0时退出
PostQueuedCompletionStatus(g_completion_port, 0, 0, NULL);
WaitForMultipleObjects(g_threads_number, g_threads, TRUE, INFINITE);
CloseHandle(g_completion_port);
WSACleanup();
}

intget_cpu_number()
{
SYSTEM_INFO system_info;
GetSystemInfo(&system_info);
returnsystem_info.dwNumberOfProcessors;
}

voidexit_error()
{
interror = GetLastError();
if(error == 0)
{
exit(RETURN_SUCESS);
}
else
{
fprintf(stderr,"error:%d\n", error);
exit(RETURN_FAILED);
}
}

/*
投递一次AcceptEx请求
返回TRUE，成功
返回FALSE，失败，WSAGetLastError()获取进一步信息
*/
intasync_AcceptEx(acceptex_block* block)
{
DWORDaddress_length;
DWORDbytes_received;
//准备投递一个异步接受请求
SOCKET accept_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if(accept_socket == INVALID_SOCKET)
returnRETURN_FAILED;
block->io_type = IO_EVENT_ACCEPT;
block->accept_socket = accept_socket;
memset(&block->overlapped, 0,
sizeof(block->overlapped));

address_length =sizeof(structsockaddr_in) + 16;
if(!AcceptEx(
block->listen_socket,
accept_socket,
block->buffer,//这个参数会传递给完成端口
0,//DATA_BUFSIZE - i * 2,       //传0进来，接受的连接的时候不接受数据
address_length, address_length,
&bytes_received,
&block->overlapped))
{
if(WSAGetLastError() != ERROR_IO_PENDING)
gotofail;
}
returnRETURN_SUCESS;
fail:
closesocket(accept_socket);
returnRETURN_FAILED;
}




/*
投递一次WSARecv请求
返回TRUE，表示接受请求投递成功
返回FALSE，表示连接已经断开，WSAGetLastError()获取进一步信息
*/
intasync_WSARecv(recv_block* block)
{
intret;
DWORDrecv_bytes, flags = 0;
block->io_type = IO_EVENT_WSARECV;
block->bytes_recveived = 0;
block->wsa_recv_buf.len = DATA_BUFSIZE;
block->wsa_recv_buf.buf = block->buffer;
memset(&(block->overlapped), 0,sizeof(block->overlapped));

//投递一次接受请求
ret = WSARecv(block->socket, &block->wsa_recv_buf, 1, &recv_bytes, &flags,
&(block->overlapped), NULL);
if(ret == -1 && WSAGetLastError() != ERROR_IO_PENDING)
{
printf("WSARecv() error returns %d\n", ret);
on_tcp_close(block);
returnRETURN_FAILED;
}
elseif((ret == 0) || (ret == -1 && WSAGetLastError() == ERROR_IO_PENDING))
{
//waiting... for next turn
}
else
{
//如果直接收到了数据
block->bytes_recveived = ret;
on_recv(block);
//递归
returnasync_WSARecv(block);
}
returnRETURN_SUCESS;
}


/*
投递一次WSARecvFrom请求
返回TRUE，表示接受请求投递成功
返回FALSE，表示接受请求投递失败，WSAGetLastError()获取进一步信息
*/
intasync_WSARecvFrom(recvfrom_block* block)
{
intret;
DWORDrecv_bytes = 0, flags = 0;
block->io_type = IO_EVENT_WSARECVFROM;
block->bytes_recveived = 0;
block->wsa_recv_buf.len = DATA_BUFSIZE;
block->wsa_recv_buf.buf = block->buffer;
memset(&block->from_address, 0,sizeof(block->from_address));
block->from_address_len =sizeof(block->from_address);
memset(&(block->overlapped), 0,sizeof(block->overlapped));

//投递一次接受请求
ret = WSARecvFrom(block->socket, &block->wsa_recv_buf, 1, &recv_bytes, &flags,
(structsockaddr*)&block->from_address, &block->from_address_len,
&(block->overlapped), NULL);
if(ret == -1 && WSAGetLastError() != ERROR_IO_PENDING)
{
printf("WSARecvFrom() error returns %d %d\n", ret, WSAGetLastError());
on_udp_close(block);
returnRETURN_FAILED;
}
elseif((ret == 0) || (ret == -1 && WSAGetLastError() == ERROR_IO_PENDING))
{
//waiting... for next turn
}
else
{
//如果直接收到了数据
block->bytes_recveived = ret;
on_recvfrom(block);
//递归
returnasync_WSARecvFrom(block);
}
returnRETURN_SUCESS;
}

voidon_acceptex(acceptex_block* block)
{
DWORDi;
structsockaddr *p_local_addr;
intlocal_addr_len =sizeof(structsockaddr_in);
structsockaddr *p_remote_addr;
intremote_addr_len =sizeof(structsockaddr_in);
structsockaddr_in *p_v4_addr;
recv_block* r_block;

printf("on_acceptex %d\n", block->accept_socket);
i =sizeof(structsockaddr_in) + 16;
GetAcceptExSockaddrs(
block->buffer,
0,//DATA_BUFSIZE - i * 2,
i, i,
&p_local_addr,
&local_addr_len,
&p_remote_addr,
&remote_addr_len
);
p_v4_addr = (structsockaddr_in *)p_local_addr;
printf("\t本地地址%s:%d\n",
inet_ntoa(p_v4_addr->sin_addr), ntohs(p_v4_addr->sin_port));
p_v4_addr = (structsockaddr_in *)p_remote_addr;
printf("\t远程地址%s:%d\n",
inet_ntoa(p_v4_addr->sin_addr), ntohs(p_v4_addr->sin_port));

//准备投递一次WSARecv请求
r_block = (recv_block*)malloc(sizeof(recv_block));
r_block->socket = block->accept_socket;
//绑定
CreateIoCompletionPort((HANDLE)r_block->socket,
g_completion_port, (u_long)r_block->socket, 0);
//投递一次接受请求
async_WSARecv(r_block);
//继续投递AcceptEx请求
async_AcceptEx(block);
}
voidon_recv(recv_block* block)
{
printf("on_recv %d, 收到%d bytes数据\n", block->socket, block->bytes_recveived);
async_WSARecv(block);
}

voidon_recvfrom(recvfrom_block* block)
{
printf("on_recvfrom %d, 收到%d bytes数据, 来自%s:%d\n",
block->socket,
block->bytes_recveived,
inet_ntoa(block->from_address.sin_addr),
ntohs(block->from_address.sin_port));
async_WSARecvFrom(block);
}

voidon_tcp_listen_close(acceptex_block* block)
{
printf("on_tcp_listen_close %d\n", block->accept_socket);
free(block);
closesocket(block->accept_socket);
}

voidon_tcp_close(recv_block* block)
{
printf("on_tcp_close %d\n", block->socket);
free(block);
closesocket(block->socket);
}

voidon_udp_close(recvfrom_block* block)
{
printf("on_udp_close %d\n", block->socket);
free(block);
closesocket(block->socket);
}

DWORDWINAPI worker_thread(LPVOIDnothing)
{
DWORDbytes;
overlapped_wrapper* over_type;
BOOLclose_socket = FALSE;
BOOLret;
UNREFERENCED_PARAMETER(nothing);
for(;;)
{
SOCKET socket;
//注意第三个参数，他是CreateIoCompletionPort时传入的，直接传入的是一个SOCKET
//注意第四个参数，他可能是一个recv_block或acceptex_block结构的指针
//因为OVERLAPPED是PER_IO_OPERATION_DATA的第一个成员，所以可以安全的进行转换
ret = GetQueuedCompletionStatus(g_completion_port, &bytes,
(LPDWORD)&socket, (LPOVERLAPPED *) &over_type, INFINITE);

if(ret == ERROR_SUCCESS)
{
DWORDlast_error = GetLastError();
if(ERROR_INVALID_HANDLE == last_error)
{
printf("完成端口被关闭,退出\n");
return0;
}
elseif(ERROR_NETNAME_DELETED == last_error
|| ERROR_OPERATION_ABORTED == last_error)
{
printf("socket被关闭 或者 操作被取消\n");
close_socket = TRUE;
}
else
{
printf("GetLastError %d\n", last_error);
continue;
}
}
//自定义的退出协议,三个参数全为0时退出(见uninit中的PostQueuedCompletionStatus)
elseif(bytes == 0 && socket == 0 && over_type == NULL)
{
return0;
}
assert(over_type);
switch(over_type->io_type)
{
caseIO_EVENT_ACCEPT:
{
acceptex_block* a_block = (acceptex_block*)over_type;
if(close_socket)
{
on_tcp_listen_close(a_block);
}
else
{
on_acceptex(a_block);
}
}
break;
caseIO_EVENT_WSARECV:
{
recv_block* r_block = (recv_block*)over_type;
//连接断开
if(close_socket || bytes == 0 || bytes == -1)
{
//测试一下，确定对方肯定关闭连接了
chartest_close;
intr = recv(r_block->socket, &test_close,sizeof(test_close), MSG_PEEK);
if(r == 0 || r == -1)
{
on_tcp_close(r_block);
}
}
//收到了bytes字节的数据
else
{
//处理数据
r_block->bytes_recveived = bytes;
on_recv(r_block);
}
}
break;

caseIO_EVENT_WSARECVFROM:
{
recvfrom_block* rf_block = (recvfrom_block*)over_type;
if(close_socket || bytes == -1 || bytes == 0)
{
on_udp_close(rf_block);
}
else
{
//处理数据
rf_block->bytes_recveived = bytes;
on_recvfrom(rf_block);
}
}
break;

default:
break;
}
}
return0;
}



