# Libevent参考手册第八章：连接侦听器： 接受 TCP 连接 - u013366022的专栏 - CSDN博客
2016年11月30日 15:22:46[slitaz](https://me.csdn.net/u013366022)阅读数：343
Evconnlistener 机制为您提供了侦听和接受传入的 TCP 连接的方法。
这一节中的所有函数与类型都是在event2/listener.h中声明。他们首次出现在 Libevent 2.0.2-alpha，除非另有说明。
## 创建或释放 evconnlistener
**接口**
**struct** evconnlistener *evconnlistener_new(**struct** event_base
 *base,
    evconnlistener_cb cb, **void** *ptr, **unsigned** flags, **int** backlog,
    evutil_socket_t fd);
**struct** evconnlistener *evconnlistener_new_bind(**struct** event_base
 *base,
    evconnlistener_cb cb, **void** *ptr, **unsigned** flags, **int** backlog,
**const****struct** sockaddr
 *sa, **int** socklen);
**void** evconnlistener_free(**struct** evconnlistener
 *lev);
Evconnlistener_new*() 的两个函数都分配，并返回一个新的连接监听器对象。连接侦听器使用event_base获得通知，当在一个监听套接字上有一个新的TCP连接。当一个新的连接到达时，它将调用你所提供的回调函数。
在这两个函数中，侦听器使用一个event_base类型的base参数去侦听连接。当一个新连接被接受时，回调函数*Cb*将被调用；若回调*cb*是NULL，监听器将被视为禁用直到设置回调。*Ptr*指针将传递给回调。*Flags*参数控制侦听器的行为 —
 — 请参考下面标志部分。*backlog*参数控制着挂起连接的最大的数目，即任何时间网络堆栈应该允许等待中尚未接受的状态的数目；请参阅您的系统的 listen() 函数的更详细的文档。如果*backlog*是负的，Libevent 将尝试为backlog设置一个适当的值；如果它是零，Libevent 将假定您已经在套接字上调用listen()。
这两个函数设置监听套接字的方式不同。Evconnlistener_new() 函数假设您已有套接字已经与端口绑定，并且以fd参数传递。如果您希望自己让Libevent 分配和绑定到套接字上，调用 evconnlistener_new_bind()，传递sockaddr参数的和它的长度。
要释放连接的侦听器，请将它传递给 evconnlistener_free()。
## 认可的标志
这些都是你可以将它传递给 evconnlistener_new() 函数的*flags*参数。你可以给任意数量的这些，结合使用这些标志
**LEV_OPT_LEAVE_SOCKETS_BLOCKING**
默认情况下，当连接监听器接收新的套接字，它设置为非阻塞，使您可以使用 Libevent 的其余部分。如果您不希望此行为，请设置此标志。
**LEV_OPT_CLOSE_ON_FREE**
如果设置了此选项，则当你释放连接监听器时，也将关闭所涉及的套接字。
**LEV_OPT_CLOSE_ON_EXEC**
如果设置了此选项，则连接监听器设置所涉及的监听器套接字以close-on-exec标志。请参阅您的平台文档为 fcntl 和FD_CLOEXEC 的详细信息。
**LEV_OPT_REUSEABLE**
默认情况下，在某些平台上，监听器套接字关闭后，直到过一段时间，在同一个端口上才可以在绑定其他的套接字。设置此选项使 Libevent 标记套接字可重用，所以一旦关闭它，可以在同一端口上侦听打开另一个套接字。
**LEV_OPT_THREADSAFE**
为监听器分配锁，以便多线程访问是安全的。Libevent 2.0.8-rc 中的新功能。
## 连接监听器回调
**接口**
**typedef****void** (*evconnlistener_cb)(**struct** evconnlistener
 *listener,
    evutil_socket_t sock, **struct** sockaddr *addr, **int** len, **void** *ptr);
当收到新的连接时，调用该提供的回调函数。*listerner*参数是收到连接的连接监听器。*sock*参数是新的套接字本身。*addr*和*len*参数分别是连接地址和地址长度。
Ptr参数是用户提供的通过evconnlistener_new()函数传递的指针。
## 启用和禁用 evconnlistener
**接口**
**int** evconnlistener_disable(**struct** evconnlistener
 *lev);
**int** evconnlistener_enable(**struct** evconnlistener
 *lev);
这些功能暂时禁用或重新启用侦听新的连接。
## 调整 evconnlistener 的回调
**接口**
**void** evconnlistener_set_cb(**struct** evconnlistener
 *lev,
    evconnlistener_cb cb, **void** *arg);
此函数可以调整现有的 evconnlistener的回调和回调参数。它是在 2.0.9-rc 中引入的。
## 检查 evconnlistener
**接口**
evutil_socket_t evconnlistener_get_fd(**struct** evconnlistener *lev);
**struct** event_base *evconnlistener_get_base(**struct** evconnlistener
 *lev);
这两个函数分别返回与监听器相关联的套接字和 event_base。
evconnlistener_get_fd() 函数首次出现在 Libevent 2.0.3-alpha 。
## 检测错误
您可以设置一个错误回调函数，当你任何时候在监听器上调用accept（）函数失败时都可以获得错误信息。这可以是非常重要的当你面对进程死锁等错误状况时。
**接口**
**typedef****void** (*evconnlistener_errorcb)(**struct** evconnlistener
 *lis, **void** *ptr);
**void** evconnlistener_set_error_cb(**struct** evconnlistener
 *lev,
    evconnlistener_errorcb errorcb);
如果你使用 evconnlistener_set_error_cb() 设置侦听器上的错误回调函数，则监听器上每次出现错误都会调用错误回调函数。它将接收作为其第一个参数，此函数的第一个参数为监听器listner，第二个参数为evconnlistener_new()中的ptr参数。
此函数是 Libevent 2.0.8-rc 中引入的。
## 示例代码： 回送服务器
**示例**
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
**static****void**
echo_read_cb(**struct** bufferevent *bev, **void** *ctx)
{
*        /* This callback is invoked when there is data to read on bev. */*
**struct** evbuffer *input = bufferevent_get_input(bev);
**struct** evbuffer *output = bufferevent_get_output(bev);
*        /* Copy all the data from the input buffer to the output buffer. */*
        evbuffer_add_buffer(output, input);
}
**static****void**
echo_event_cb(**struct** bufferevent *bev, **short** events, **void** *ctx)
{
**if** (events & BEV_EVENT_ERROR)
                perror("Error from bufferevent");
**if** (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
                bufferevent_free(bev);
        }
}
**static****void**
accept_conn_cb(**struct** evconnlistener *listener,
    evutil_socket_t fd, **struct** sockaddr *address, **int** socklen,
**void** *ctx)
{
*        /* We got a new connection! Set up a bufferevent for it. */*
**struct** event_base *base = evconnlistener_get_base(listener);
**struct** bufferevent *bev = bufferevent_socket_new(
                base, fd, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(bev, echo_read_cb, NULL, echo_event_cb, NULL);
        bufferevent_enable(bev, EV_READ|EV_WRITE);
}
**static****void**
accept_error_cb(**struct** evconnlistener *listener, **void** *ctx)
{
**struct** event_base *base = evconnlistener_get_base(listener);
**int** err = EVUTIL_SOCKET_ERROR();
        fprintf(stderr, "Got an error %d (%s) on the listener. "
                "Shutting down./n", err, evutil_socket_error_to_string(err));
        event_base_loopexit(base, NULL);
}
**int**
main(**int** argc, **char** **argv)
{
**struct** event_base *base;
**struct** evconnlistener *listener;
**struct** sockaddr_in sin;
**int** port = 9876;
**if** (argc > 1) {
                port = atoi(argv[1]);
        }
**if** (port<=0 || port>65535) {
                puts("Invalid port");
**return** 1;
        }
        base = event_base_new();
**if** (!base) {
                puts("Couldn't open event base");
**return** 1;
        }
*        /* Clear the sockaddr before using it, in case there are extra*
*         * platform-specific fields that can mess us up. */*
        memset(&sin, 0, **sizeof**(sin));
*        /* This is an INET address */*
        sin.sin_family = AF_INET;
*        /* Listen on 0.0.0.0 */*
        sin.sin_addr.s_addr = htonl(0);
*        /* Listen on the given port. */*
        sin.sin_port = htons(port);
        listener = evconnlistener_new_bind(base, accept_conn_cb, NULL,
            LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
            (**struct** sockaddr*)&sin, **sizeof**(sin));
**if** (!listener) {
                perror("Couldn't create listener");
**return** 1;
        }
        evconnlistener_set_error_cb(listener, accept_error_cb);
        event_base_dispatch(base);
**return** 0;
}
