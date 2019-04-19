# 再次浅析adb shell,pc daemon和手机daemon三者之间的数据交互流程 - xqhrs232的专栏 - CSDN博客
2011年03月17日 18:35:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1063标签：[shell																[手机																[socket																[thread																[output																[events](https://so.csdn.net/so/search/s.do?q=events&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.chinaunix.net/space.php?uid=20564848&do=blog&cuid=1818785](http://blog.chinaunix.net/space.php?uid=20564848&do=blog&cuid=1818785)
`再次浅析adb shell,pc daemon和手机daemon三者之间的数据交互流程`
[《浅析linux开发工具adb具体实现》](http://writeblog.csdn.net/link.php?url=http://blog.chinaunix.net%2Fu1%2F38994%2Fshowart_1807376.html)
[《浅析adb创建流程》](http://writeblog.csdn.net/link.php?url=http://blog.chinaunix.net%2Fu1%2F38994%2Fshowart_1907747.html)
``
```
###################
 adb shell
interactive_shel
==>fd
= socket_loopback_client(ADB_PORT,
SOCK_STREAM);
向该fd发送"shell:"字符串,之后本地的daemon程序接收到来自loopback,ADB_PORT端口数据,然后交由local_socket_event_func函数处理,
int interactive_shell(void)
{
    adb_thread_t thr;
    int fdi, fd;
    int *fds;
    fd = adb_connect("shell:");//和pc daemon建立的socket连接句柄fd
    if(fd 
< 0) {
        fprintf(stderr,"error: %s/n", adb_error());
        return 1;
    }
    fdi = 0;
//dup(0);
    fds = malloc(sizeof(int)
* 2);
    fds[0] 
= fd;
    fds[1] 
= fdi;
#ifdef HAVE_TERMIO_H
    stdin_raw_init(fdi);
#endif
    //单独创建一个thread等待stdin键盘输入,
    //然后将键盘输入的数据发送给fd,也就是发送给
    //在本地与pc daemon建立的socket[luther.gliethttp]
    adb_thread_create(&thr, stdin_read_thread, fds);
    read_and_dump(fd);//回显pc daemon穿过来的数据
#ifdef HAVE_TERMIO_H
    stdin_raw_restore(fdi);
#endif
    return 0;
}
static void read_and_dump(int fd)
{
    char buf[4096];
    int len;
    while(fd
>= 0)
{
        len = adb_read(fd, buf, 4096);//读取和pc
 daemon建立的socket,然后将接收到的数据打印到stdout显示
        if(len
== 0)
{
            break;
        }
        if(len
< 0) 
{
            if(errno
== EINTR)
continue;
            break;
        }
        /* we want to output to stdout, so no adb_write here !! */
        unix_write(1, buf, len);
    }
}
###################
 pc上的daemon
ss_listener_event_func//等待连接
==>s
= create_local_socket(fd);//为新来的一个连接建立socket,并安装处理该socket的handler--local_socket_event_func
==>create_local_socket
==>install_local_socket
==>s->id
= local_socket_next_id++;//该socket在daemon的id标识
s->enqueue 
= local_socket_enqueue;
然后在output_thread线程handler中会调用到local_socket_enqueue
==>t->read_from_remote(p,
 t)
==>write_packet(t->fd,
&p)
==>transport_socket_events
==>handle_packet
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
{//调用local的数据处理函数,处理来自remote端的数据,也就是local_socket_enqueue处理函数
                    D("Enqueue the socket/n");
                    send_ready(s->id, rid, t);
                }
                return;
            }
        }
        break;
==>local_socket_enqueue
==>adb_write(s->fd,
 p->ptr, p->len)//发送给上面daemon生成的socket
对于adb shell来说由read_and_dump函数读取由daemon发送来的数据,然后将其显示到stdout上[luther.gliethttp].
==>connect_to_smartsocket
==>s->peer的enqueue处理函数为smart_socket_enqueue
==>local_socket_event_func
==>s->peer->enqueue(s->peer,
 p);//将数据交由peer方法smart_socket_enqueue处理
==>smart_socket_enqueue//接收到"shell:"字符串
==>connect_to_remote(s->peer,
(char*)
(p->data
+ 4));//s->peer就是adb shell和daemon建立的连接socket
//这里s就是smart_socket,所以s->peer就是实际的fd
//一下操作s将把自己从s->peer上摘除.
    s->peer->ready
= local_socket_ready_notify;
    s->peer->close
= local_socket_close_notify;
    s->peer->peer
= 0;//将s->peer的peer清空
        /* give him our transport and upref it */
    s->peer->transport
= s->transport;
    connect_to_remote(s->peer,
(char*)
(p->data
+ 4));
    s->peer
= 0;
    s->close(s);//smart将自己关闭,因为smart的peer经过connect_to_remote之后需要重新建立一个remote
 peer.
    return 1;
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
= s->id;//daemon里对socket的id标识,arg0=local_id,arg1=remote_id
    p->msg.data_length
= len;
    strcpy((char*) p->data,
 destination);//"shell:"字符串
    send_packet(p, s->transport);
}
==>send_packet
==>write_packet(t->transport_socket,
&p)
input_thread线程handler //等待t->fd对应的pair套接字数据的到来,也就是上面的write_packet(t->transport_socket, &p)
==>read_packet(t->fd,
&p)
==>t->write_to_remote(p,
 t);//通过本t,经过tcpip发送到手机daemon
==>remote_write
==>writex(t->sfd,
&p->msg,
sizeof(amessage)
+ length)//发送到sfd句柄,也就是local_init时,client_socket_thread或server_socket_thread尝试从网络上查询到的自己remote端的port连接.
###################
 手机上的daemon
adb_main
==>local_init
==>server_socket_thread线程handler
//等待pc上的daemon启动时连接ADB_LOCAL_TRANSPORT_PORT,这里是5555端口,
==>fd
= adb_socket_accept//接收该socket连接
==>register_socket_transport
==>register_transport//注册登记该port可以用于远端通信
==>transport_write_action(transport_registration_send,
&m)
这样由init_transport_registration创建的transport_registration_func接收处理函数将得到数据.
==>transport_registration_func
    adb_thread_create(&input_thread_ptr, input_thread, t);//接收daemon发送过来的数据,然后将数据转发给remote远端.
    adb_thread_create(&output_thread_ptr, output_thread, t);//接收remote发送过来的数据,然后转给daemon处理程序
    
output_thread线程handler //等待port对应的远端数据到来
==>t->read_from_remote(p,
 t);//等待到来port对应的远端数据
==>write_packet(t->fd,
&p) 
//将接收到的数发送给transport_registration_func中创建的adb_socketpair通信对的另一端t->transport_socket = s[0];
//这样t->transport_socket的接收处理函数transport_socket_events,进一步处理,从port到来的远端数据.
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
//如果service有数据发送,那么将触发local_socket_event_func函数执行,进而按上面所讲的方式发送给s->peer->enqueue(s->peer, p);在这里就是下面create_remote_socket创建的peer,对应的enqueue函数为remote_socket_enqueue.
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
 t);//local_id=s->id, remote_id=s->peer->id
                s->ready(s);
            }
        }
        break;
    case A_OKAY:
/* READY(local-id, remote-id, "") */
//pc端的daemon接收到手机端send_ready(s->id, s->peer->id, t);回应信息.
//因为pc端在上面smart_socket_enqueue==>s->peer->peer = 0;//将s->peer的peer清空
//connect_to_remote之后smart将自己close了,内存也全部释放掉了[luther.gliethttp].
//所以这里s->peer一定等于0
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
handle_packet
==>case A_OPEN:
==>case A_OKAY:
==>create_remote_socket
asocket *create_remote_socket(unsigned id, atransport
*t)
{
    asocket *s =
calloc(1,
sizeof(aremotesocket));
    adisconnect* dis 
= &((aremotesocket*)s)->disconnect;
    if(s 
== 0) fatal("cannot allocate socket");
    s->id 
= id;//该socket对应的id标识
    s->enqueue
= remote_socket_enqueue;
    s->ready
= remote_socket_ready;
    s->close
= remote_socket_close;
    s->transport
= t;
    dis->func
= remote_socket_disconnect;
    dis->opaque
= s;
    add_transport_disconnect( t, dis
);
    D("RS(%d): created/n", s->id);
    return s;
}
==>s->enqueue
= remote_socket_enqueue;
static int remote_socket_enqueue(asocket
*s, apacket
*p)
{
    D("Calling remote_socket_enqueue/n");
    p->msg.command
= A_WRTE;
    p->msg.arg0
= s->peer->id;//对端-手机或pc,下daemon中socket的id标识
    p->msg.arg1
= s->id;
    p->msg.data_length
= p->len;
    send_packet(p, s->transport);
    return 1;
}
handle_packet
==>case A_OPEN:
//send_ready(s->id, s->peer->id, t);//将手机端id标识s->id和pc端daemon下id标识s->peer->id发还给pc端的daemon
static void send_ready(unsigned local,
unsigned remote, atransport
*t)
{
    D("Calling send_ready /n");
    apacket *p = get_apacket();
    p->msg.command
= A_OKAY;
    p->msg.arg0
= local;
    p->msg.arg1
= remote;
    send_packet(p, t);
}
```
