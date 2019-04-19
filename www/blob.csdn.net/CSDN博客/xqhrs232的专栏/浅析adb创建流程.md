# 浅析adb创建流程 - xqhrs232的专栏 - CSDN博客
2011年03月17日 18:44:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：995标签：[socket																[thread																[server																[手机																[events																[shell](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=events&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.chinaunix.net/space.php?uid=20564848&do=blog&cuid=1907747](http://blog.chinaunix.net/space.php?uid=20564848&do=blog&cuid=1907747)
```
浅析adb创建流程
```
[《浅析linux开发工具adb具体实现》](http://writeblog.csdn.net/link.php?url=http://blog.chinaunix.net%2Fu1%2F38994%2Fshowart_1807376.html)
[《再次浅析adb shell,pc daemon和手机daemon三者之间的数据交互流程》](http://writeblog.csdn.net/link.php?url=http://blog.chinaunix.net%2Fu1%2F38994%2Fshowart_1818785.html)
```
```
```
output_thread==>t->read_from_remote(p,
 t)即:remote_read==>readx(t->sfd,
&p->msg,
sizeof(amessage))
等待远端数据,将远端发送过来的数据写入t->fd句柄,这样与t->fd相pair的
t->transport_socket句柄就接收到该数据了,然后t->transport_socket调用
transport_socket_events==>handle_packet处理远端数据发送过来的数据包,
函数find_local_socket(p->msg.arg1)用来查寻远端希望把数据交给本地s->id值为p->msg.arg1
的local_socket,直接调用本地local_socket的s->enqueue(s, p);即local_socket_enqueue,
local_socket_enqueue函数又将调用adb_write(s->fd, p->ptr,
 p->len);将数据发送给s->fd,
该s->fd是由ss_listener_event_func函数创建的一个fd
= adb_socket_accept(_fd,
&addr, 
&alen);网络端口连接
所以向s->fd发送数据就表示将数据发送给了与host端口#define ADB_PORT 5037建立网络
连接的adb devices等client命令,这些命令正在pending读取与host端口5037建立连接的句柄fd发送数据过来.
input_thread等待t->fd句柄有数据到来,数据由与t->fd相pair的t->transport_socket句柄调用send_packet==>
write_packet(t->transport_socket,
&p)发送过来,之后input_thread将接收到得数据通过
t->write_to_remote(p, t);即:remote_write==>writex(t->sfd,
&p->msg,
sizeof(amessage)
+ length);发送给
该transport对应的通信端口句柄t->sfd,该句柄在
local_connect==>fd
= socket_network_client(host, port,
SOCK_STREAM);// 和export ADBHOST=192.168.100.2建立连接
// 将成功连接的socket句柄fd,作为该transport的通信句柄,用来和192.168.100.2手机进行双向通信[luther.gliethttp]
==>register_socket_transport==>init_socket_transport(t,
 s, port)中设置,也就是发送给了手机上的adbd守护程序.
手机端server的output_thread线程将接收到数据,然后执行上面output_thread中同样的步骤进行数据解析[lutehr.gliethttp].
host监听#define ADB_PORT 5037端口,等待adb devices等命令发送socket连接过来,之后ss_listener_event_func回调函数
将为adb devices等命令对应的socket连接句柄s->fd在host上创建s
= create_local_socket(fd);一个socket维护对象,当该
socket句柄s->fd收到数据时[也就是adb devices发送数据到HOST了(luther.gliethttp)],执行local_socket_event_func回调函数,
读取由client发送的数据[比如adb devices或者adb shell发送的键盘输入数据],然后执行s->peer->enqueue(s->peer,
 p);将数据
的处理权交给s->peer的enqueue函数,最开始smart socket的回调函数ss_listener_event_func为client建立的peer为connect_to_smartsocket,
它对应的enqueue为smart_socket_enqueue,该函数将调用handle_host_request处理所有client发送过来的、
以"host:"或"host-local:"等开头的service命令,如果client发送了非"host:"或"host-local:"等开头,那么smart_socket_enqueue函数
将把自己从该local_socket上s->peer摘下来,解除自己与client的peer关联之后,同时负责为client和运行在
手机上的server远端程序进行open衔接操作,衔接成功之后local_socket对应的s->peer的enqueue函数就是remote_socket_enqueue了,
remote_socket_enqueue将直接调用send_packet(p, s->transport);将数据发送给手机端[luther.gliethtp]
 static int smart_socket_enqueue(asocket
*s, apacket
*p) 
{
        ...
        // s代表smart,s->peer就指向了pc daemon与client端建立的socket端口fd
        s->peer->ready
= local_socket_ready_notify;
// ready时,同时发送"OKAY"字符串给client
        s->peer->close
= local_socket_close_notify;
// close时,同时发送"closed"字符串给client
        s->peer->peer
= 0; 
// 表示client没有peer了,原来的peer smart没了[luther.gliethttp]
            /* give him our transport and upref it */
        s->peer->transport
= s->transport;
// 该client端在pc daemon上的socket端口fd,通过该transport通道与remote远端
                                           // server[我们就是手机上的adb server]进行通信[luther.gliethttp]
        connect_to_remote(s->peer,
(char*)
(p->data
+ 4));
// smart解除自己与client的peer关联之后,同时负责为client和
        // 运行在手机上的server远端程序进行open衔接操作.
        // s->peer将与remote进行p->data + 4命令连接[lutehr.gliethttp]
        s->peer
= 0; 
// ok,smart的peer清0
        s->close(s);
// 对应的该smart关闭
        return 1;
        ...
   }
void connect_to_remote(asocket
*s, 
const char 
*destination)
{
    D("Connect_to_remote call /n");
    apacket *p = get_apacket();
    int len =
strlen(destination)
+ 1;
    if(len 
> (MAX_PAYLOAD-1))
{
        fatal("destination oversized");
    }
    D("LS(%d): connect('%s')/n", s->id, destination);
    p->msg.command
= A_OPEN;
    p->msg.arg0
= s->id;
// 告诉手机上的server,向pc daemon中id值为s->id的socket发送数据
    // 手机server发送过来的数据被output_thread线程接收,执行write_packet(t->fd, &p);
    // 转发给正在监听t->transport_socket端口的回调函数transport_socket_events==>handle_packet进一步处理
    p->msg.data_length
= len;
    strcpy((char*) p->data,
 destination);
    send_packet(p, s->transport);
// 这样开始介绍的output_thread线程就开始做相应的处理了[luther.gliethttp]
}
```
```
来看看整体流程
```
```
adb_main
==>init_transport_registration 注册transport登记接口,回调函数transport_registration_func
   等待register_transport发送数据过来.
==>local_init 对于pc上的host程序,执行client_socket_thread线程==>local_connect
   优先扫描fd = socket_loopback_client(port,
SOCK_STREAM);
   本地port是否能打开,如果不能打开,那么尝试获取const
char *host 
= getenv("ADBHOST");
   环境变量,而我们恰恰定义了该环境变量export ADBHOST=192.168.100.2,
   所以尝试打开手机端server监听的相应端口[我们定义的是#define ADB_LOCAL_TRANSPORT_PORT 5555,
   这由运行在手机上的server_socket_thread线程创建],
   fd = socket_network_client(host, port,
SOCK_STREAM);打开成功后
   调用register_socket_transport==>register_transport将该transport交由
   上面的transport_registration_func函数继续处理,transport_registration_func函数将
   该transport挂接到transport_list链表上,然后创建一对socketpair[这样就可以对其中
   一个fd实行双向读写操作了,pipe只能单向操作,一个fd只读,另一个只写],
   一端给t->transport_socket
= s[0];另一端给t->fd
= s[1];之后将t->transport_socket
   添加到FDE_READ读epoll中,t->fd发过来的数据由transport_socket_events回调函数
   调用handle_packet函数做进一步处理,之后创建2个线程input_thread和output_thread,
   input_thread通过read_packet(t->fd,
&p);接收host通过t->transport_socket发送的数据,
   然后执行t->write_to_remote(p, t);将数据转发给手机上的server[192.168.100.2:5555]
   output_thread通过t->read_from_remote(p, t);接收手机server[192.168.100.2:5555]
   发送过来的数据,然后执行write_packet(t->fd,
&p);转发给正在监听t->transport_socket
   端口的回调函数transport_socket_events==>handle_packet,这样在该transport上进行数据
   收发所需要的环境都已经建立完毕了[luther.gliethttp]
==>install_listener("tcp:5037",
"*smartsocket*",
NULL);
   这是运行在pc上的host程序所创建的一个监听端口5037[#define ADB_PORT 5037],
   他将监听所有adb shell等本地命令数据[姑且把pc上执行adb shell等命令的操作为client端],
   如果该本地命令host可以执行,那么直接返回结果给client端,
   否则通过transport端口将该命令数据转发到手机上,由手机上的server进行处理[luther.gliethttp]
   具体由l->fd
= local_name_to_fd(local_name);语句
   执行socket_loopback_server(port,
SOCK_STREAM);创建[luther.gliethttp]
   5037端口监听到的数据交由ss_listener_event_func回调函数做进一步处理,
   如果是非"*smartsocket*"的listener那么执行listener_event_func回调函数[luther.gliethttp]
   ss_listener_event_func==>
   fd = adb_socket_accept(_fd,
&addr, 
&alen);
   s = create_local_socket(fd);设置该adb shell命令在host上连接处理函数local_socket_event_func
   connect_to_smartsocket(s);
    void connect_to_smartsocket(asocket
*s)
    {
        D("Connecting to smart socket /n");
        asocket *ss 
= create_smart_socket(smart_socket_action);
        //将本地client命令,比如adb shell的peer设置为smartsocket
        s->peer
= ss;
        ss->peer
= s;
        s->ready(s);
// 调用local_socket_ready==>fdevent_add(&s->fde, FDE_READ);
                     // 这样host将等待本地client命令发送数据过来,
                     // 然后执行local_socket_event_func==>s->peer->enqueue(s->peer, p);
                     // 即上面create_smart_socket创建的smartsocket==>smart_socket_enqueue
                     // 处理来自client命令的数据[比如:adb shell命令发送过来的数据]
    }
==>对于host来说fprintf(stderr,
"OK/n");// 将OK传递给执行adb start-server的parent程序,因为parent正执
                                       // 行adb_read(fd[0], temp, 3);等待管道数据[luther.gleithttp]
==>start_logging();
// 打开log文件,然后dup2到stdout和stderr,
==>fdevent_loop==>fdevent_process
// 进入event loop循环等待所有
static void fdevent_process()
{
    struct epoll_event events[256];
    fdevent *fde;
    int i, n;
    n = epoll_wait(epoll_fd, events, 256,
-1);
// 等待添加到epoll_fd中的各个fd对应event事件发生[luther.gliethttp]
    ...
    for(i 
= 0; i < n; i++)
{
        struct epoll_event 
*ev = events 
+ i;
        fde = ev->data.ptr;
        if(ev->events
& EPOLLIN) 
{
            fde->events
|= FDE_READ;
        }
        if(ev->events
& EPOLLOUT)
{
            fde->events
|= FDE_WRITE;
        }
        if(ev->events
& (EPOLLERR
| EPOLLHUP))
{
            fde->events
|= FDE_ERROR;
        }
        if(fde->events)
{
            if(fde->state
& FDE_PENDING)
continue; 
// 正在处理前一条信息
            fde->state
|= FDE_PENDING;
            fdevent_plist_enqueue(fde);
// 放入待处理的list链表上
        }
    }
}
==>比如client端执行adb shell命令
==>adb shell
==> interactive_shell
==> fd
= adb_connect("shell:");
==> fd
= _adb_connect("host:version");该
   命令将触发pc daemon上install_listener("tcp:5037",
"*smartsocket*",
NULL);创建的监听端口发生READ事件,
   执行ss_listener_event_func回调函数登记该socket连接的读回调函数local_socket_event_func,它将读取该socket数据,
   然后交由s->peer->enqueue(s->peer,
 p);[这里就是smart_socket_enqueue函数]处理所有数据,之后所有shell命令
   都通过该fd发送到pc daemon的local_socket_event_func处理函数进行处理,最终效果如下:
   | client端 | pc daemon端
|
   | | 
|
   | "host:version"
==>
| local_socket_event_func 
==> smart_socket_enqueue[最终处理数据]
|
   当发送一个非service命令,即非"host:"开头的命令时,smart_socket_enqueue将把自己从s->peer中摘掉:
   static int smart_socket_enqueue(asocket
*s, apacket
*p) 
{
        ...
        // s代表smart,s->peer就指向了pc daemon与client端建立的socket端口fd
        s->peer->ready
= local_socket_ready_notify;
// ready时,同时发送"OKAY"字符串给client
        s->peer->close
= local_socket_close_notify;
// close时,同时发送"closed"字符串给client
        s->peer->peer
= 0; 
// 表示client没有peer了,原来的peer smart没了[luther.gliethttp]
            /* give him our transport and upref it */
        s->peer->transport
= s->transport;
// 该client端在pc daemon上的socket端口fd,通过该transport通道与remote远端
                                           // server[我们就是手机上的adb server]进行通信[luther.gliethttp]
        connect_to_remote(s->peer,
(char*)
(p->data
+ 4));
// smart解除自己与client的peer关联之后,同时负责为client和
        // 运行在手机上的server远端程序进行open衔接操作.
        // s->peer将与remote进行p->data + 4命令连接[lutehr.gliethttp]
        s->peer
= 0; 
// ok,smart的peer清0
        s->close(s);
// 对应的该smart关闭
        return 1;
        ...
   }
void connect_to_remote(asocket
*s, 
const char 
*destination)
{
    D("Connect_to_remote call /n");
    apacket *p = get_apacket();
    int len =
strlen(destination)
+ 1;
    if(len 
> (MAX_PAYLOAD-1))
{
        fatal("destination oversized");
    }
    D("LS(%d): connect('%s')/n", s->id, destination);
    p->msg.command
= A_OPEN;
    p->msg.arg0
= s->id;
// 告诉手机上的server,向pc daemon中id值为s->id的socket发送数据
    // 手机server发送过来的数据被output_thread线程接收,执行write_packet(t->fd, &p);
    // 转发给正在监听t->transport_socket端口的回调函数transport_socket_events==>handle_packet进一步处理
    p->msg.data_length
= len;
    strcpy((char*) p->data,
 destination);
    send_packet(p, s->transport);
}
==>transport_socket_events
==>handle_packet
    case A_OPEN:
/* OPEN(local-id, 0, "destination") */
    //手机端接收到connect_to_remote命令A_OPEN
        if(t->connection_state
!= CS_OFFLINE)
{
            char *name
= (char*) p->data;
            name[p->msg.data_length
> 0 ? p->msg.data_length
- 1 : 0]
= 0;
            s = create_local_service_socket(name);//create_local_socket本地建立一个新的socket,用于name对应的service数据收发.
//如果service有数据发送,那么将触发local_socket_event_func函数执行,进而按上面所讲的方式发送给 s->peer->enqueue(s->peer, p);在这里就是下面create_remote_socket创建的peer,对应的enqueue函数为 remote_socket_enqueue.
//那么当对端remote来数据的话,又怎么分发给daemon所管理的那么多socket呢,即remote所期望通信的socket呢,这就需要上面的
//s->id = local_socket_next_id++;//该socket在daemon的id标识
//来进一步区分本地daemon中的socket了.
            if(s
== 0)
{
                send_close(0, p->msg.arg0, t);
            } else
{
                s->peer
= create_remote_socket(p->msg.arg0, t);//arg0就是remote端pc上daemon中socket的id标识,详细信息见上面[luther.gliethttp].
                //这样s->peer->id等于p->msg.arg0,这样就记录下在pc上远端的socket的id标识了
                s->peer->peer
= s;
                //发送本地s->id给remote端,让它执行A_OKAY处理,同样将本地id登记到remote上.
                //remote端的id标识为s->peer->id,回应给pc端的daemon
                send_ready(s->id, s->peer->id,
 t);//local_id=s->id, remote_id=s->peer->id,向pc daemon发送"A_OKAY"数据
                s->ready(s);
// 将s添加到READ事件中
            }
        }
        break;
    case A_OKAY:
/* READY(local-id, remote-id, "") */
//pc端的daemon接收到手机端send_ready(s->id, s->peer->id, t);回应信息.
//因为pc端在上面smart_socket_enqueue==>s->peer->peer = 0;//将s->peer的peer清空
//connect_to_remote之后smart将自己close了,内存也全部释放掉了[luther.gliethttp].
//所以这里s->peer等于0
        if(t->connection_state
!= CS_OFFLINE)
{
            if((s
= find_local_socket(p->msg.arg1)))
{//arg0=local_id, arg1=remote_id
                if(s->peer
== 0)
{
//生成以手机端socket id为s->id的pc daemon上的socket
                    s->peer
= create_remote_socket(p->msg.arg0, t);//创建对端socket,其在对端daemon下的id标识为msg.arg0[luther.gliethttp]
                    s->peer->peer
= s;
                }
                s->ready(s);
// 调用local_socket_ready_notify,将s添加到READ事件中,同时发送"OKAY"字符串给client
            }
        }
        break;
    case A_WRTE:
        if(t->connection_state
!= CS_OFFLINE)
{
            if((s
= find_local_socket(p->msg.arg1)))
{//根据s->id查找本地daemon[手机或pc]管理的相应socket处理函数
                unsigned rid 
= p->msg.arg0;
                p->len
= p->msg.data_length;
                if(s->enqueue(s, p)
== 0)
{//调用local的数据处理函数,处理来自remote端的数据
                    D("Enqueue the socket/n");
                    send_ready(s->id, rid, t);
                }
                return;
            }
        }
        break;
```
