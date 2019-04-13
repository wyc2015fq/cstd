
# linux下的RPC - 高科的专栏 - CSDN博客

2013年02月28日 17:40:56[高科](https://me.csdn.net/pbymw8iwm)阅读数：7906


一、概述
|在传统的编程概念中，过程是由程序员在本地编译完成，并只能局限在本地运行的一段代码，也即其主程序和过程之间的运行关系是本地调用关系。因此这种结构在网络日益发展的今天已无法适应实际需求。总而言之，传统过程调用模式无法充分利用网络上其他主机的资源（如CPU、 Memory等），也无法提高代码在实体间的共享程度，使得主机资源大量浪费。|
|而本文要介绍的RPC编程，正是很好地解决了传统过程所存在的一系列弊端。通过RPC我们可以充分利用非共享内存的多处理器环境（例如通过局域网连接的多台工作站）,这样可以简便地将你的应用分布在多台工作站上，应用程序就像运行在一个多处理器的计算机上一样。你可以方便的实现过程代码共享，提高系统资源的利用率，也可以将以大量数值处理的操作放在处理能力较强的系统上运行，从而减轻前端机的负担。|
|二、RPC的结构原理及其调用机制|
如前所述RPC其实也是一种C/S的编程模式，有点类似C/S Socket 编程模式，但要比它更高一层。当我们在建立RPC服务以后，客户端的调用参数通过底层的RPC传输通道，可以是UDP，也可以是TCP（也即TI-RPC —无关性传输），并根据传输前所提供的目的地址及RPC上层应用程序号转至相应的RPC Application Porgramme Server ，且此时的客户端处于等待状态，直至收到应答或Time Out超时信号。具体的流程图如图1。当服务器端获得了请求消息，则会根据注册RPC时告诉RPC系统的例程入口地址，执行相应的操作，并将结果返回至客户端。
![](http://zhoulifa.bokee.com/inc/rpc.jpg)当一次RPC调用结束后，相应线程发送相应的信号，客户端程序才会继续运行。
|当然，一台服务主机上可以有多个远程过程提供服务，那么如何来表示一个唯一存在的远程过程呢？一个远程过程是有三个要素来唯一确定的：程序号、版本号和过程号。程序号是用来区别一组相关的并且具有唯一过程号的远程过程。一个程序可以有一个或几个不同的版本，而每个版本的程序都包含一系列能被远程调用的过程，通过版本的引入，使得不同版本下的RPC能同时提供服务。每个版本都包含有许多可供远程调用的过程，每个过程则有其唯一标示的过程号。|
|三、基于RPC的应用系统开发|
|通过以上对RPC原理的简介后，我们再来继续讨论如何来开发基于RPC的应用系统。一般而言在开发RPC时，我们通常分为三个步骤：|
|a、定义说明客户/服务器的通信协议。|
|这里所说的通信协议是指定义服务过程的名称、调用参数的数据类型和返回参数的数据类型，还包括底层传输类型（可以是 UDP或TCP），当然也可以由RPC底层函数自动选择连接类型建立TI-RPC。最简单的协议生成的方法是采用协议编译工具，常用的有Rpcgen，我会在后面实例中详细描述其使用方法。|
|b、开发客户端程序。|
|c、开发服务器端程序。|
|开发客户端和服务器端的程序时，RPC提供了我们不同层次的开发例程调用接口。不同层次的接口提供了对RPC不同程度控制。一般可分为5个等级的编程接口，接下来我们分别讨论一下各层所提供的功能函数。|
|1、简单层例程|
|简单层是面向普通RPC应用，为了快速开发RPC应用服务而设计的，他提供了如下功能函数。|
|函数名
|功能描述
|
|Rpc_reg( )
|在一特定类型的传输层上注册某个过程，来作为提供服务的 RPC 程序
|
|Rpc_call( )
|远程调用在指定主机上指定的过程
|
|Rpc_Broadcast( )
|向指定类型的所有传输端口上广播一个远程过程调用请求
|
|
|2、高层例程|
|在这一层，程序需要在发出调用请求前先创建一个客户端句柄，或是在侦听请求前先建立一个服务器端句柄。程序在该层可以自由的将自己的应用绑在所有的传输端口上，它提供了如下功能函数。|
|函数名
|功能描述
|
|Clnt_create( )
|程序通过这个功能调用，告诉底层 RPC 服务器的位置及其传输类型
|
|Clnt_create_timed( )
|定义每次尝试连接的超时最大时间
|
|Svc_create( )
|在指定类型的传输端口上建立服务器句柄，告诉底层 RPC 事件过程的相应入口地址
|
|Clnt_call()
|向服务器端发出一个 RPC 调用请求
|
|
|3、中间层例程|
|中间层向程序提供更为详细的RPC控制接口,而这一层的代码变得更为复杂，但运行也更为有效，它提供了如下功能函数。|
|函数名
|功能描述
|
|Clnt_tp_create( )
|在指定的传输端口上建立客户端句柄
|
|Clnt_tp_create_timed( )
|定义最大传输时延
|
|Svc_tp_creaet( )
|在指定的传输端口上建立服务句柄
|
|Clnt_call( )
|向服务器端发出 RPC 调用请求
|
|
|4、专家层例程|
|这层提供了更多的一系列与传输相关的功能调用，它提供了如下功能函数。|
|函数名
|功能描述
|
|Clnt_tli_create( )
|在指定的传输端口上建立客户端句柄
|
|Svc_tli_create( )
|在指定的传输端口上建立服务句柄
|
|Rpcb_set( )
|通过调用 rpcbind 将 RPC 服务和网络地址做映射
|
|Rpcb_unset( )
|删除 rpcb_set( ) 所建的映射关系
|
|Rpcb_getaddr( )
|调用 rpcbind 来犯会指定 RPC 服务所对应的传输地址
|
|Svc_reg( )
|将指定的程序和版本号与相应的时间例程建起关联
|
|Svc_ureg( )
|删除有 svc_reg( ) 所建的关联
|
|Clnt_call( )
|客户端向指定的服务器端发起 RPC 请求
|
|
|5、底层例程|
|该层提供了所有对传输选项进行控制的调用接口，它提供了如下功能函数。|
|函数名
|功能描述
|
|Clnt_dg_create( )
|采用无连接方式向远程过程在客户端建立客户句柄
|
|Svc_dg_create( )
|采用无连接方式建立服务句柄
|
|Clnt_vc_create( )
|采用面向连接的方式建立客户句柄
|
|Svc_vc_create( )
|采用面向连接的方式建立 RPC 服务句柄
|
|Clnt_call( )
|客户端向服务器端发送调用请求
|
C语言进行rpc编程时可以使用rpcgen自动生成分布式的程序代码文件，自己之需要编写***.x，客户端、服务器端程序以及相应的接口就可以了。下面根据一个实现远程文件传输的rpc调用程序（客户端根据文件名请求文件，服务器端传回相应的文件），具体介绍使用rpcgen编写rpc调用的过程，最后保证调用成功必须有8个源文件，rpcgen能自动生成3个，过程比较复杂，共分为7步：
1.根据rpc调用的功能，先不考虑rpc调用，编写一个平常的实现相应功能的程序。
如一个远程的文件传输的rpc调用，平常程序便是考虑文件存储在本地，直接打开读便可，如下：
\#include <stdio.h>
\#include <stdlib.h>
\#define MAXNAME 20
\#define MAXLENGTH 1024
char * readfile(char *);
int main()
{
char name[MAXNAME];
printf("Enter File Name: ");
scanf("%s", name);
printf("%s", readfile(name));
}
char * readfile(char * name)
{
FILE *file = fopen(name, "r");
char * buf = (char *)malloc(sizeof(char)*MAXLENGTH);
if (file == NULL)
{
printf("File Cann't Be Open!");
return 0;
}
printf("The File Content is : /n");
while (fgets(buf, MAXLENGTH-1, file) != NULL)
{
return buf;
}
return NULL;
}
2.把程序拆分为两部分，main函数和readfile函数，带有main的一部分是主动发起调用的，在rpc中相当于客户端，带有readfile函数的部分是提供相应的功能的，相当于服务器端。将代码拆分后要在客户端添加相应的rpc调用函数，clnt_create(RMACHINE,
 FILETRANSPROG, FILETRANSVERS,"tcp");
FILETRANSPROG便是trans.x中的程序名，FILETRANSVERS是版本名，使用tcp进行rpc调用。
拆分后代码如下：
客户端client.c：
\#include <rpc/rpc.h>
\#include <stdio.h>
\#include <stdlib.h>
\#include <sys/socket.h>
\#include <sys/types.h>
\#include <netinet/in.h>
\#include <unistd.h>
\#include "trans.h"
\#define WSVERS MAKEWORD(0, 2)
\#define RMACHINE "localhost"
CLIENT *handle;
\#define MAXNAME 20
\#define MAXLENGTH 1024
char * readfile(char *);
int main()
{
char name[MAXNAME];
char * buf;
printf("Enter File Name: ");
scanf("%s", name);
handle = clnt_create(RMACHINE, FILETRANSPROG, FILETRANSVERS,"tcp");
if (handle == 0) {
printf("Could Not Connect To Remote Server./n");
exit(1);
}
buf = readfile(name);
printf("%s", buf);
return 0;
}
服务器端server.c：
\#include <rpc/rpc.h>
\#include <stdio.h>
\#include <stdlib.h>
\#include "trans.h"
\#define MAXNAME 20
\#define MAXLENGTH 1024
char * readfile(char * name)
{
FILE *file = fopen(name, "r");
char * buf = (char *)malloc(sizeof(char)*MAXLENGTH);
if (file == NULL)
{
printf("File Cann't Be Open!");
return 0;
}
printf("The File Content is : /n");
while (fgets(buf, MAXLENGTH-1, file) != NULL)
{
return buf;
}
return NULL;
}
3.编写***.x文件。具体步骤可以参考Douglas的那本Internetworking With TCP/IP的第三卷，客户端-服务器端编程与应用。
本程序的.x文件命名为trans.x内容如下：
const MAXLENGTH = 1024;
const MAXNAME = 20;
program FILETRANSPROG  //程序名
{
version FILETRANSVERS  //版本名
{
string READFILE(string) = 1;  //调用的方法名
} = 1;
} = 99;
4.使用rpcgen编辑.x文件，在linux下输入命令
rpcgen   trans.x
若格式正确，编译无错误则产生三个文件trans.h，trans_svc.c（服务器端），trans_clnt.c（客户端）。因为上述trans.x中无自定义数据结构，所以没有xdr文件产生。
trans.h代码：
/*
* Please do not edit this file.
* It was generated using rpcgen.
*/
\#ifndef _TRANS_H_RPCGEN
\#define _TRANS_H_RPCGEN
\#include <rpc/rpc.h>

\#ifdef __cplusplus
extern "C" {
\#endif
\#define MAXLENGTH 1024
\#define MAXNAME 20
\#define FILETRANSPROG 99
\#define FILETRANSVERS 1
\#if defined(__STDC__) || defined(__cplusplus)
\#define READFILE 1
extern  char ** readfile_1(char **, CLIENT *);
extern  char ** readfile_1_svc(char **, struct svc_req *);
extern int filetransprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);
\#else /* K&R C */
\#define READFILE 1
extern  char ** readfile_1();
extern  char ** readfile_1_svc();
extern int filetransprog_1_freeresult ();
\#endif /* K&R C */
\#ifdef __cplusplus
}
\#endif
\#endif /* !_TRANS_H_RPCGEN */
trans_svc.c代码：
/*
* Please do not edit this file.
* It was generated using rpcgen.
*/
\#include "trans.h"
\#include <stdio.h>
\#include <stdlib.h>
\#include <rpc/pmap_clnt.h>
\#include <string.h>
\#include <memory.h>
\#include <sys/socket.h>
\#include <netinet/in.h>
\#ifndef SIG_PF
\#define SIG_PF void(*)(int)
\#endif
static void
filetransprog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
union {
char *readfile_1_arg;
} argument;
char *result;
xdrproc_t _xdr_argument, _xdr_result;
char *(*local)(char *, struct svc_req *);
switch (rqstp->rq_proc) {
case NULLPROC:
(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
return;
case READFILE:
_xdr_argument = (xdrproc_t) xdr_wrapstring;
_xdr_result = (xdrproc_t) xdr_wrapstring;
local = (char *(*)(char *, struct svc_req *)) readfile_1;
break;
default:
svcerr_noproc (transp);
return;
}
memset ((char *)&argument, 0, sizeof (argument));
if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
svcerr_decode (transp);
return;
}
result = (*local)((char *)&argument, rqstp);
if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
svcerr_systemerr (transp);
}
if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
fprintf (stderr, "%s", "unable to free arguments");
exit (1);
}
return;
}
int
main (int argc, char **argv)
{
register SVCXPRT *transp;
pmap_unset (FILETRANSPROG, FILETRANSVERS);
transp = svcudp_create(RPC_ANYSOCK);
if (transp == NULL) {
fprintf (stderr, "%s", "cannot create udp service.");
exit(1);
}
if (!svc_register(transp, FILETRANSPROG, FILETRANSVERS, filetransprog_1, IPPROTO_UDP)) {
fprintf (stderr, "%s", "unable to register (FILETRANSPROG, FILETRANSVERS, udp).");
exit(1);
}
transp = svctcp_create(RPC_ANYSOCK, 0, 0);
if (transp == NULL) {
fprintf (stderr, "%s", "cannot create tcp service.");
exit(1);
}
if (!svc_register(transp, FILETRANSPROG, FILETRANSVERS, filetransprog_1, IPPROTO_TCP)) {
fprintf (stderr, "%s", "unable to register (FILETRANSPROG, FILETRANSVERS, tcp).");
exit(1);
}
svc_run ();
fprintf (stderr, "%s", "svc_run returned");
exit (1);
/* NOTREACHED */
}
trans_clnt.c代码：
/*
* Please do not edit this file.
* It was generated using rpcgen.
*/
\#include <memory.h> /* for memset */
\#include "trans.h"
/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };
char **
readfile_1(char **argp, CLIENT *clnt)
{
static char *clnt_res;
memset((char *)&clnt_res, 0, sizeof(clnt_res));
if (clnt_call (clnt, READFILE,
(xdrproc_t) xdr_wrapstring, (caddr_t) argp,
(xdrproc_t) xdr_wrapstring, (caddr_t) &clnt_res,
TIMEOUT) != RPC_SUCCESS) {
return (NULL);
}
return (&clnt_res);
}
5.编写客户端和服务器端接口。此部分可以说是最麻烦的部分，稍不注意便会出错，同样可以参考Douglas的那本书，但要注意的是他的服务器接口例程代码中的每个函数的第二个参数应该是CLIENT *clnt，而非struct svc_req * rqstp
为本程序编写的代码如下：
客户端接口文件trans_cif.c：
\#include <rpc/rpc.h>
\#include <stdio.h>
\#include "trans.h"/* Client-side stub interface routines written by programmer */
extern CLIENT * handle;
static char **ret;
char * readfile(char * name)
{
char ** arg;
arg = &name;
ret = readfile_1(arg, handle);
return ret==NULL ? NULL : *ret;
}
服务器端接口文件trans_sif.c：
\#include <rpc/rpc.h>
\#include <stdio.h>
\#include "trans.h"
char * readfile(char *);
static char * retcode;
char ** readfile_1(char ** w, CLIENT *clnt)
{
retcode = readfile(*(char**)w);
return &retcode;
}
6.编译链接客户端和服务器端程序
不管是客户端还是服务器端，都要链接三个文件，
客户端：程序文件+*** _clnt.c+客户端接口文件。
服务器端：程序文件+*** _svc.c+服务器端接口文件
同时每一段的三个文件都是互相关联的，编译出现错误时，可以根据提示查看三个文件进行debug
命令如下：
gcc -Wall -o trans_client client.c trans_clnt.c trans_cif.c
gcc -Wall -o trans_server server.c trans_svc.c trans_sif.c
7.启动服务器端和客户端，大功告成。要先运行服务器端程序，再运行客户端程序。命令如下：
./trans_server
./trans_client
client启动后，提示输入要传输的文件名，输入后，server将文件的第一行传回，大功告成！
相关：
[http://hi.baidu.com/%E2%C8%C2%F8%CD%B7%C9%E7%C7%F8/blog/item/6d50b86752957dfef63654cc.html](http://hi.baidu.com/%E2%C8%C2%F8%CD%B7%C9%E7%C7%F8/blog/item/6d50b86752957dfef63654cc.html)
[http://shake863.iteye.com/blog/181249](http://shake863.iteye.com/blog/181249)
[http://blog.csdn.net/beff2047/article/details/4028568](http://blog.csdn.net/beff2047/article/details/4028568)
[http://blog.chinaunix.net/uid-20644632-id-2220585.html](http://blog.chinaunix.net/uid-20644632-id-2220585.html)


