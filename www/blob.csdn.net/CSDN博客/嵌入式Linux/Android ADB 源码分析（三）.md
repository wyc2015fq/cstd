
# Android ADB 源码分析（三） - 嵌入式Linux - CSDN博客

2018年10月26日 22:49:40[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：271



## 前言
之前分析的两篇文章
[Android Adb 源码分析(一)](https://zhuanlan.zhihu.com/p/47333062)
[嵌入式Linux：Android root破解原理（二）](https://zhuanlan.zhihu.com/p/47559722)
写完之后，都没有写到相关的实现代码，这篇文章写下ADB的通信流程的一些细节
看这篇文章之前，请先阅读
[Linux的SOCKET编程详解 - 江召伟 - 博客园](https://link.zhihu.com/?target=https%3A//www.cnblogs.com/jiangzhaowei/p/8261174.html)
## 对socket通信有简单的了解
## 1、ADB基本通信
**理解：**
（1）adb的本质，就是socket的通信，通过secket传送数据及文件
（2）adb传送是以每个固定格式的包发送的数据，包的格式如下：
`#define A_SYNC 0x434e5953
#define A_CNXN 0x4e584e43
#define A_OPEN 0x4e45504f
#define A_OKAY 0x59414b4f
#define A_CLSE 0x45534c43
#define A_WRTE 0x45545257
#define A_AUTH 0x48545541
 
 
struct amessage {
    unsigned command;       /* command identifier constant      */
    unsigned arg0;          /* first argument                   */
    unsigned arg1;          /* second argument                  */
    unsigned data_length;   /* length of payload (0 is allowed) */
    unsigned data_check;    /* checksum of data payload         */
    unsigned magic;         /* command ^ 0xffffffff             */
};
 
struct apacket
{
    apacket *next;
 
    unsigned len;
    unsigned char *ptr;
 
    amessage msg;
    unsigned char data[MAX_PAYLOAD];
};`发送的包格式为apacket格式，其中msg为消息部分，data为数据部分。msg的消息类型有很多种，包括**A_SYNC**,**A_CNXN**,**A_OPEN**,**A_OKAY**等等。
（3）adb给我们预留了调试的信息，我们只需要在adb.h中定义指定的宏，即可看到每次数据的传输过程：
\#define DEBUG_PACKETS 1
打开调试信息后，我们可以看到传输过程中的细节，在串口打印里面。
（4）我们使用adb push命令，来跟踪分析下这个apacket数据是怎样传输的：
我们以adb push profile /命令为例，在串口我们可以看见如下详细的传输信息：
**status command arg0 arg1 len data**
recv: OPEN 00141028 00000000 0006 "sync:."
send: OKAY 0000003e 00141028 0000 ""
recv: WRTE 00141028 0000003e 0009 "STAT..../"
send: OKAY 0000003e 00141028 0000 ""
send: WRTE 0000003e 00141028 0010 "STAT.A......[oHZ"
recv: OKAY 00141028 0000003e 0000 ""
recv: WRTE 00141028 0000003e 0027 "SEND..../profile,33206DATA....2D
send: OKAY 0000003e 00141028 0000 ""
send: WRTE 0000003e 00141028 0008 "OKAY...."
recv: OKAY 00141028 0000003e 0000 ""
recv: WRTE 00141028 0000003e 0008 "QUIT...."
send: OKAY 0000003e 00141028 0000 ""
send: CLSE 00000000 00141028 0000 ""
recv: CLSE 00141028 0000003e 0000 ""
以上recv表示接收的数据包，send表示回传的数据包。后面五个分别为数据包的数据字段值（command arg0 arg1 len data），这样数据我们还是不够直观，我们翻译成更加直接的数据辅以文字解释
![](https://pic2.zhimg.com/80/v2-6dfe5ba52fb74a5e1dcd17a7492630bd_hd.jpg)
这样是不是容易理解多了呢，经过这样的数据发送，我们就通过adb push命令把本地的profile文件推送到远程设备的根目录了。哇..... 原来这么简单，一个profile文件就传输了。流程理解了，我们再来看代码，现在结果你知道了，流程你也懂了，再来看源码，是不是容易理解了呢。

## 2、代码分析
我们看代码也是逆向的看，这样利于我们理解，不会被源码看到晕乎乎，上面流程懂了，知道了每次是以apacket的格式发送的，我们先来研究这个apacket的接收与发送函数。
接收函数handle_packet
`void handle_packet(apacket *p, atransport *t)
{
    asocket *s;
 
    D("handle_packet() %c%c%c%c\n", ((char*) (&(p->msg.command)))[0],
            ((char*) (&(p->msg.command)))[1],
            ((char*) (&(p->msg.command)))[2],
            ((char*) (&(p->msg.command)))[3]);
    print_packet("recv", p);
 
    switch(p->msg.command){
    case A_SYNC:
        if(p->msg.arg0){
            send_packet(p, t);
            if(HOST) send_connect(t);
        } else {
            t->connection_state = CS_OFFLINE;
            handle_offline(t);
            send_packet(p, t);
        }
        return;
 
    case A_CNXN: /* CONNECT(version, maxdata, "system-id-string") */
            /* XXX verify version, etc */
        if(t->connection_state != CS_OFFLINE) {
            t->connection_state = CS_OFFLINE;
            handle_offline(t);
        }
 
        parse_banner((char*) p->data, t);
 
        if (HOST || !auth_enabled) {
            handle_online(t);
            if(!HOST) send_connect(t);
        } else {
            send_auth_request(t);
        }
        break;
 
    case A_AUTH:
        if (p->msg.arg0 == ADB_AUTH_TOKEN) {
            t->key = adb_auth_nextkey(t->key);
            if (t->key) {
                send_auth_response(p->data, p->msg.data_length, t);
            } else {
                /* No more private keys to try, send the public key */
                send_auth_publickey(t);
            }
        } else if (p->msg.arg0 == ADB_AUTH_SIGNATURE) {
            if (adb_auth_verify(t->token, p->data, p->msg.data_length)) {
                adb_auth_verified(t);
                t->failed_auth_attempts = 0;
            } else {
                if (t->failed_auth_attempts++ > 10)
                    adb_sleep_ms(1000);
                send_auth_request(t);
            }
        } else if (p->msg.arg0 == ADB_AUTH_RSAPUBLICKEY) {
            adb_auth_confirm_key(p->data, p->msg.data_length, t);
        }
        break;
 
    case A_OPEN: /* OPEN(local-id, 0, "destination") */
        if (t->online) {
            char *name = (char*) p->data;
            name[p->msg.data_length > 0 ? p->msg.data_length - 1 : 0] = 0;
            s = create_local_service_socket(name);
            if(s == 0) {
                send_close(0, p->msg.arg0, t);
            } else {
                s->peer = create_remote_socket(p->msg.arg0, t);
                s->peer->peer = s;
                send_ready(s->id, s->peer->id, t);
                s->ready(s);
            }
        }
        break;
 
    case A_OKAY: /* READY(local-id, remote-id, "") */
        if (t->online) {
            if((s = find_local_socket(p->msg.arg1))) {
                if(s->peer == 0) {
                    s->peer = create_remote_socket(p->msg.arg0, t);
                    s->peer->peer = s;
                }
                s->ready(s);
            }
        }
        break;
 
    case A_CLSE: /* CLOSE(local-id, remote-id, "") */
        if (t->online) {
            if((s = find_local_socket(p->msg.arg1))) {
                s->close(s);
            }
        }
        break;
 
    case A_WRTE:
        if (t->online) {
            if((s = find_local_socket(p->msg.arg1))) {
                unsigned rid = p->msg.arg0;
                p->len = p->msg.data_length;
 
                if(s->enqueue(s, p) == 0) {
                    D("Enqueue the socket\n");
                    send_ready(s->id, rid, t);
                }
                return;
            }
        }
        break;
 
    default:
        printf("handle_packet: what is %08x?!\n", p->msg.command);
    }
 
    put_apacket(p);
}`哇，这个函数好像不复杂
一个函数，然后解析apacket *p数据，根据msg.command的命令值, 然后对应不同的case，有着不同的响应。事实上也就是这样，这个函数主要就是根据不同的消息类型，来处理这个apacket的数据。
下面解析下上面push命令的过程
**1、OPEN响应**
recv: OPEN 00141028 00000000 0006 "sync:."
send: OKAY 0000003e 00141028 0000 ""
接收到了OPEN的消息，然后附带了一个sync的数据，我们看看是如何响应的。
`case A_OPEN: /* OPEN(local-id, 0, "destination") */
        if (t->online) {
            char *name = (char*) p->data;
            name[p->msg.data_length > 0 ? p->msg.data_length - 1 : 0] = 0;
            s = create_local_service_socket(name);
            if(s == 0) {
                send_close(0, p->msg.arg0, t);
            } else {
                s->peer = create_remote_socket(p->msg.arg0, t);
                s->peer->peer = s;
                send_ready(s->id, s->peer->id, t);
                s->ready(s);
            }
        }
        break;`调用create_local_service_socket(“sync”);
fd = service_to_fd(name);
//创建本地socket，并为这个socket创建数据处理线程file_sync_service
ret = create_service_thread(file_sync_service, NULL);
//把这个本地socket关联到结构asocket *s
s = create_local_socket(fd);
调用create_remote_socket(p->msg.arg0, t); //把远程的socket也与这个结构体asocket 关联。
如上两个函数调用，主要是初始化本地的socket对，本地socket用来跟后台服务线程之间的通信，以及跟对应命令的后台服务线程通信。初始化adb通信的环境。其中asocket *s为本地socket与远程socket的一个关联结构体，其中s保存的是本地socket的信息，s->peer保存的是远程socket相关的信息。
send_ready(s->id, s->peer->id, t); 然后发送OKAY给PC端。
`static void send_ready(unsigned local, unsigned remote, atransport *t)
{
    D("Calling send_ready \n");
    apacket *p = get_apacket();
    p->msg.command = A_OKAY;
    p->msg.arg0 = local;
    p->msg.arg1 = remote;
    send_packet(p, t);
}`这个与我们看到的流程相符合。接收到**OPEN**的消息，初始化一些状态，然后返回一个**OKAY**的状
2、WRITE响应
recv: WRTE 00141028 0000003e 0009 "STAT..../"
send: OKAY 0000003e 00141028 0000 ""
send: WRTE 0000003e 00141028 0010 "STAT.A......[oHZ"
接收到了WRITE的消息，顺带了一个查询STAT的数据，我们看看是如何响应的：
`case A_WRTE:
        if (t->online) {
            if((s = find_local_socket(p->msg.arg1))) {
                unsigned rid = p->msg.arg0;
                p->len = p->msg.data_length;
 
                if(s->enqueue(s, p) == 0) {
                    D("Enqueue the socket\n");
                    send_ready(s->id, rid, t);
                }
                return;
            }
        }
        break;`先通过参数p->msg.arg1找到我们在OPEN的时候建立的结构体信息asocket *s， 然后处理本地socket队列中的数据（s为本地，s->peer为远程）
s->enqueue(s, p)即为之前 关联的函数local_socket_enqueue，其在create_local_socket(fd); 的时候设置。
`static int local_socket_enqueue(asocket *s, apacket *p)
{
    D("LS(%d): enqueue %d\n", s->id, p->len);
 
    p->ptr = p->data;
 
        /* if there is already data queue'd, we will receive
        ** events when it's time to write.  just add this to
        ** the tail
        */
    if(s->pkt_first) {
        goto enqueue;
    }
 
        /* write as much as we can, until we
        ** would block or there is an error/eof
        */
    while(p->len > 0) {
        int r = adb_write(s->fd, p->ptr, p->len);
        if(r > 0) {
            p->len -= r;
            p->ptr += r;
            continue;
        }
        if((r == 0) || (errno != EAGAIN)) {
            D( "LS(%d): not ready, errno=%d: %s\n", s->id, errno, strerror(errno) );
            s->close(s);
            return 1; /* not ready (error) */
        } else {
            break;
        }
    }
 
    if(p->len == 0) {
        put_apacket(p);
        return 0; /* ready for more data */
    }
 
enqueue:
    p->next = 0;
    if(s->pkt_first) {
        s->pkt_last->next = p;
    } else {
        s->pkt_first = p;
    }
    s->pkt_last = p;
 
        /* make sure we are notified when we can drain the queue */
    fdevent_add(&s->fde, FDE_WRITE);
 
    return 1; /* not ready (backlog) */
}`我们通过adb_write(s->fd, p->ptr, p->len)把要处理的数据，写入到本地socket对应的fd中，等待处理。
然后调用send_ready(s->id, rid, t);返回一个OKAY的状态
我们把待处理的数据adb_write之后，又是在哪里处理的呢，我们之前在创建本地socket的时候，就创建了一个线程，对应的处理socket数据的函数file_sync_service。
我们来看看file_sync_service函数是如何处理的
`void file_sync_service(int fd, void *cookie)
{
    syncmsg msg;
    char name[1025];
    unsigned namelen;
 
    char *buffer = malloc(SYNC_DATA_MAX);
    if(buffer == 0) goto fail;
 
    for(;;) {
        D("sync: waiting for command\n");
 
        if(readx(fd, &msg.req, sizeof(msg.req))) {
            fail_message(fd, "command read failure");
            break;
        }
        namelen = ltohl(msg.req.namelen);
        if(namelen > 1024) {
            fail_message(fd, "invalid namelen");
            break;
        }
        if(readx(fd, name, namelen)) {
            fail_message(fd, "filename read failure");
            break;
        }
        name[namelen] = 0;
 
        msg.req.namelen = 0;
        D("sync: '%s' '%s'\n", (char*) &msg.req, name);
 
        switch(msg.req.id) {
        case ID_STAT:
            if(do_stat(fd, name)) goto fail;
            break;
        case ID_LIST:
            if(do_list(fd, name)) goto fail;
            break;
        case ID_SEND:
            if(do_send(fd, name, buffer)) goto fail;
            break;
        case ID_RECV:
            if(do_recv(fd, name, buffer)) goto fail;
            break;
        case ID_QUIT:
            goto fail;
        default:
            fail_message(fd, "unknown command");
            goto fail;
        }
    }
 
fail:
    if(buffer != 0) free(buffer);
    D("sync: done\n");
    adb_close(fd);
}`原来在这里处理的数据，终于找到你， 我们收到的消息是查看路径是否存在，这里对应的就是ID_STAT，还有其他的消息处理，比如ID_SEND，ID_RECV，ID_QUIT，望文生义，我们就不具体解释了。我们还是看看ID_STAT对应的处理吧do_stat(fd, name)。
`static int do_stat(int s, const char *path)
{
    syncmsg msg;
    struct stat st;
 
    msg.stat.id = ID_STAT;
 
    if(lstat(path, &st)) {
        msg.stat.mode = 0;
        msg.stat.size = 0;
        msg.stat.time = 0;
    } else {
        msg.stat.mode = htoll(st.st_mode);
        msg.stat.size = htoll(st.st_size);
        msg.stat.time = htoll(st.st_mtime);
    }
 
    return writex(s, &msg.stat, sizeof(msg.stat));
}`这里就是判断路径是否存在的逻辑了，这个就是我们想要的，我们把判断的结果存储在msg.stat, 然后把对应的结果写回去writex。
我们把检测的状态writex之后，但是这个数据还没有发送回PC端啊，是在哪里发送回去的呢，我们继续跟踪
我们在create_local_socket创建本地socket的时候，顺便还注册了一个回调函数local_socket_event_func
`static void local_socket_event_func(int fd, unsigned ev, void *_s)
{
    asocket *s = _s;
 
    D("LS(%d): event_func(fd=%d(==%d), ev=%04x)\n", s->id, s->fd, fd, ev);
 
    /* put the FDE_WRITE processing before the FDE_READ
    ** in order to simplify the code.
    */
    if(ev & FDE_WRITE){
        apacket *p;
 
        while((p = s->pkt_first) != 0) {
            while(p->len > 0) {
                int r = adb_write(fd, p->ptr, p->len);
                if(r > 0) {
                    p->ptr += r;
                    p->len -= r;
                    continue;
                }
                if(r < 0) {
                    /* returning here is ok because FDE_READ will
                    ** be processed in the next iteration loop
                    */
                    if(errno == EAGAIN) return;
                    if(errno == EINTR) continue;
                }
                D(" closing after write because r=%d and errno is %d\n", r, errno);
                s->close(s);
                return;
            }
 
            if(p->len == 0) {
                s->pkt_first = p->next;
                if(s->pkt_first == 0) s->pkt_last = 0;
                put_apacket(p);
            }
        }
 
            /* if we sent the last packet of a closing socket,
            ** we can now destroy it.
            */
        if (s->closing) {
            D(" closing because 'closing' is set after write\n");
            s->close(s);
            return;
        }
 
            /* no more packets queued, so we can ignore
            ** writable events again and tell our peer
            ** to resume writing
            */
        fdevent_del(&s->fde, FDE_WRITE);
        s->peer->ready(s->peer);
    }
 
 
    if(ev & FDE_READ){
        apacket *p = get_apacket();
        unsigned char *x = p->data;
        size_t avail = MAX_PAYLOAD;
        int r;
        int is_eof = 0;
 
        while(avail > 0) {
            r = adb_read(fd, x, avail);
            D("LS(%d): post adb_read(fd=%d,...) r=%d (errno=%d) avail=%d\n", s->id, s->fd, r, r<0?errno:0, avail);
            if(r > 0) {
                avail -= r;
                x += r;
                continue;
            }
            if(r < 0) {
                if(errno == EAGAIN) break;
                if(errno == EINTR) continue;
            }
 
                /* r = 0 or unhandled error */
            is_eof = 1;
            break;
        }
        D("LS(%d): fd=%d post avail loop. r=%d is_eof=%d forced_eof=%d\n",
          s->id, s->fd, r, is_eof, s->fde.force_eof);
        if((avail == MAX_PAYLOAD) || (s->peer == 0)) {
            put_apacket(p);
        } else {
            p->len = MAX_PAYLOAD - avail;
 
            r = s->peer->enqueue(s->peer, p);
            D("LS(%d): fd=%d post peer->enqueue(). r=%d\n", s->id, s->fd, r);
 
            if(r < 0) {
                    /* error return means they closed us as a side-effect
                    ** and we must return immediately.
                    **
                    ** note that if we still have buffered packets, the
                    ** socket will be placed on the closing socket list.
                    ** this handler function will be called again
                    ** to process FDE_WRITE events.
                    */
                return;
            }
 
            if(r > 0) {
                    /* if the remote cannot accept further events,
                    ** we disable notification of READs.  They'll
                    ** be enabled again when we get a call to ready()
                    */
                fdevent_del(&s->fde, FDE_READ);
            }
        }
        /* Don't allow a forced eof if data is still there */
        if((s->fde.force_eof && !r) || is_eof) {
            D(" closing because is_eof=%d r=%d s->fde.force_eof=%d\n", is_eof, r, s->fde.force_eof);
            s->close(s);
        }
    }
 
    if(ev & FDE_ERROR){
            /* this should be caught be the next read or write
            ** catching it here means we may skip the last few
            ** bytes of readable data.
            */
//        s->close(s);
        D("LS(%d): FDE_ERROR (fd=%d)\n", s->id, s->fd);
 
        return;
    }
}`我们看后面if(ev & FDE_READ)部分:
adb_read(fd, x, avail);把数据读出来，然后调用r = s->peer->enqueue(s->peer, p);，即把数据发送给远程socket的队列处理。（s->speer即远程端，之前已经说明）
s->peer->enqueue函数即remote_socket_enqueue
`static int remote_socket_enqueue(asocket *s, apacket *p)
{
    D("entered remote_socket_enqueue RS(%d) WRITE fd=%d peer.fd=%d\n",
      s->id, s->fd, s->peer->fd);
    p->msg.command = A_WRTE;
    p->msg.arg0 = s->peer->id;
    p->msg.arg1 = s->id;
    p->msg.data_length = p->len;
    send_packet(p, s->transport);
    return 1;
}`这样我们就把STAT的结果，通过WRITE返回给了PC端
这个与我们看到的流程也是相符的，接收到WRITE（STAT）的消息，先返回一个OKAY的状态，在返回WRITE（STAT）的结果。
我们可以观察之前的数据接收及发送流程，可以发现每次一个WRITE消息，后面都是返回一个OKAY WRITE消息。
贴了这么多的代码，是不是有点晕了，再贴就真的看不下去了，我们下面重新来理一理思路。
1. adb其实就是个socket通信，数据发过来发过去。
2. adb每次都是发送的一个数据包，数据结构是struct apacket，其中包含msg消息部分，及data数据部分。
3. 从PC跟device通信的过程，有一条协议流程，通过不断的数据交互发送，实现数据文件传递。
4. 我们可以定义 \#define DEBUG_PACKETS 1 这样可以看到socket通信的数据发送过程。
5. socket数据建立传输过程，会创建socket，创建事件监听线程，注册回调响应函数，乱七八糟的....
如果觉得不错，请关注公众号【嵌入式Linux】,谢谢

