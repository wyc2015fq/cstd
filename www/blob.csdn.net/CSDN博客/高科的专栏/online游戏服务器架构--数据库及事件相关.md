
# online游戏服务器架构--数据库及事件相关 . - 高科的专栏 - CSDN博客

2012年11月24日 00:35:05[高科](https://me.csdn.net/pbymw8iwm)阅读数：5930


Online服务器的第三部分就是数据层，send_request_to_db开始了数据层的处理逻辑：
int send_request_to_db(int cmd, sprite_t* p, int body_len, const void* body_buf, uint32_t id)；
在该函数里首先以懒惰的方式连接数据库服务器，获取一个网络连接，注意参数p，如果该参数为空，那么就说明不关心数据库代理服务器返回的数据：
if (!p) pkg->seq = 0;
else pkg->seq = (sprite_fd (p) << 16) | p->waitcmd;
注意以上的代码，如果不关心返回数据，那么直接将pkg的seq字段设置为0即可，如果关心返回结果，就需要用这个seq字段保存一些信息了，比如当前处理的业务协议是什么，还有就是这个客户端实体p的对应的父进程的套结字描述符是多少，然后将这个pkg连同信息体一起发送给数据库代理服务器，等到代理服务器返回的时候会进入worker_handle_net中处理，注意handle_process函数里关于子进程的执行路线：
int worker_handle_net(int fd, void* buf, int len, sprite_t** u)
{
assert( len >= sizeof(server_proto_t) );
server_proto_t* dbpkg = buf;
…
} else if (fd == proxysvr_fd) {
return handle_db_return(fd, dbpkg, len, u);
}
return 0;
}
执行流进入handle_db_return：
static int handle_db_return(int fd, server_proto_t* dbpkg, int len, sprite_t** sp)
{
int waitcmd = dbpkg->seq & 0xFFFF;
int conn = dbpkg->seq >> 16;
if (!dbpkg->seq || waitcmd == PROTO_LOGOUT) //如果不关心返回数据，则在send_request_to_db就已经将seq设置成了0，于是直接返回，否则取出保存的fd信息
return 0;
if (!(*sp = get_sprite_by_fd(conn)) || (*sp)->waitcmd != waitcmd) {
//出错
}
…
int err = -1;
switch (dbpkg->ret) {
case 0:
break; //成功
…//处理各种错误码
在处理各种错误码的时候可以根据不同的协议进行不同的动作，协议保存在sprite_t的waitcmd字段中。在没有错误的情况下就会进入数据层的回调处理：
\#define DO_MESSAGE(n, func) /
case n: err = func(*sp, dbpkg->id, dbpkg->body, len - sizeof (server_proto_t)); break
和协议层的处理十分类似，也是回调函数的形式，只不过这里没有提前注册，只是简单的封装了一下switch-case开关。对于前面的例子就是：
DO_MESSAGE(SVR_PROTO_RACE_SIGN, race_sign_callback);
int race_sign_callback(sprite_t* p, uint32_t id, char* buf, int len)
{
uint32_t itms[2] = {12999, 12998};
CHECK_BODY_LEN(len, 4);
p->teaminfo.team = *(uint32_t*)buf;
if (p->teaminfo.team != 1 && p->teaminfo.team != 2) {
ERROR_RETURN(("race failed/t[%u %u]", p->id, p->teaminfo.team), -1);
}
db_single_item_op(0, p->id, itms[p->teaminfo.team - 1], 1, 1);
response_proto_uint32(p, p->waitcmd, p->teaminfo.team, 0); //一定要向客户端回应，否则客户端将挂起
return 0;
}
一定要返回给客户端一个数据，因为客户端和服务器是一问多答式的，服务器的应答可以分好几部分来返回给客户端，比如一共需要返回5次，那么在这5次全部返回之间，服务器是不接受同一个客户端的别的请求的，必然是一问多答，而不是多问多答。注意send_to_self的最后一个参数的意义：
int send_to_self(sprite_t *p, uint8_t *buffer, int len, int completed)
如果completed为1，那么在该函数中就会将p的waitcmd设置为0，代表当前的这个协议已经处理完毕，online可以处理下一个协议请求了，否则就意味着当前的协议还没有处理完毕，online不接收新的协议请求，这个在dispatch_protocol中体现：
if (p->waitcmd != 0) {
send_to_self_error(p, cmd, -ERR_system_busy, 0);
WARN_RETURN(("wait for cmd=%d, id=%u, new cmd=%d", p->waitcmd, p->id, cmd), 0);
}
Onlien服务器通过这种方式解决了一些同步问题，一条协议没有处理完是不接受另外的协议的。关于数据同步，其实online服务器使用了另外的方案，并没有使用传统的锁之类的，而是使用了一个全局变量，并且onlien中不存在线程的概念，因此基本不存在处理数据时的数据共享访问，因此一个子进程同时只能处理一个客户的请求，因此全局变量msg被定义出来，用来保存需要返回给客户端的消息，注意包含协议头部。最后的问题就是请求和回应时的数据组织了，对于请求包，用UNPKG_UINT32来解析包的内容，j是游标号，需要在外部定义然后在外部使用，初始值就是需要开始解析的位置距离包(也就是b)开始的以字节为单位的大小，比如一个buffer，协议头为8个字节，我们需要解析协议体，也就是有效载荷，那么我们需要如下代码：
Int j = 8, v = count;
UNPKG_UINT32(buffer, count, j);
只要看看下面的定义就一目了然了：
\#define UNPKG_UINT32(b, v, j) /
do { /
(v) = ntohl( *(uint32_t*)((b)+(j)) ); (j) += 4; /
} while (0)
对于封包同样的方式，只是将流程反过来了：
\#define PKG_UINT32(b, v, j) /
do { /
*(uint32_t*)((b)+(j)) = htonl(v); (j) += 4; /
} while (0)
在往客户端返回包的时候，封包的过程就是用的PKG_UINT32，如果连包头一起封装，那么就是下面的流程：
int j = sizeof(protocol_t); //空余了包头的空间
PKG_UINT32(msg, intbuf1, j); //从包头的下一个字节开始打包
PKG_UINT32(msg, intbuf2, j); //继续打包
…
关于事件处理器是和数据库相关的处理器并列的逻辑处理器，这个处理器主要处理系统事件的，由于事件分为好多种，如果写进一个协议处理回调函数会使得这个函数的职责太多，不明确，如果每个事件作为一个协议封装，那么又会使整个协议处理器的架构主次不分，很含糊，因此就专门为事件处理单独列一个更低级的层次进行处理，也就是和协议处理不在一个层次，而专门为所有事件单独封装一个协议处理回调函数，然后为了协议处理的清晰，在这个协议处理钩子中将事件分发到不同的事件处理器中，如此一来，事件处理就单独成了一个子层次

