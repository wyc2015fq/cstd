# nginx连接池实现 - 囧囧有神的专栏 - CSDN博客





2013年12月28日 19:37:30[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：10561








# 1. 配置

worker_connections

模块:EventsModule

语法:worker_connections number

默认:

位于main section的指令worker_connections和worker_processes可以帮助你计算你能处理的最大并发数

max clients = worker_processes * worker_connections

在一个反向代理环境中，最大并发数变成了

max clients = worker_processes * worker_connections/2



配置示例

```
worker_processes  12;

events {

        use epoll;

        worker_connections  2048000;

}
```






# 2. 工作原理



![](https://img-blog.csdn.net/20131228192516328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


同一个进程，上游下游共用一个连接池，连接池大小，进程数都可在配置中指定。

# 3. 数据结构

在nginx中connection就是对tcp连接的封装，其中包括连接的socket，读事件，写事件。利用nginx封装的connection，我们可以很方便的使用nginx来处理与连接相关的事情，比如，建立连接，发送与接受数据等。结构体如下：

Src/core/ngx_connection.h

```cpp
struct ngx_connection_s {

    void               *data;

    ngx_event_t        *read;

    ngx_event_t        *write;

ngx_socket_t        fd;

。。。

ngx_queue_t         queue;

。。。

}
```



连接池数据结构如下：

![](https://img-blog.csdn.net/20131228192628718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

连接池cycle->connections采用数组单链表实现，空闲连接链表free_connections也指向了连接池头结点，实现的非常优美。

1、使用数组实现，不用队列或栈，实现简单，可以随机访问，初始化很方便

2、所有操作只操作链表头节点，没有内存拷贝，性能复杂度O（1）



创建初始化如上数据结构在src/event/ngx_event.c:ngx_event_process_init中

```cpp
ngx_event_process_init(ngx_cycle_t *cycle)

{。。。

cycle->files_n = (ngx_uint_t) rlmt.rlim_cur;

       cycle->files = ngx_calloc(sizeof(ngx_connection_t *) * cycle->files_n, cycle->log);

。。。

    cycle->connections =

        ngx_alloc(sizeof(ngx_connection_t) * cycle->connection_n, cycle->log);

。。。

    i = cycle->connection_n;

    next = NULL;

    do {

        i--;

        c[i].data = next;

        c[i].read = &cycle->read_events[i];

        c[i].write = &cycle->write_events[i];

        c[i].fd = (ngx_socket_t) -1;

        next = &c[i];

    } while (i);

    cycle->free_connections = next;

cycle->free_connection_n = cycle->connection_n;

。。。

}
```








# 4. 基本操作

Src/core/ngx_connection.c

Ngx_get_connection

 从free_connections获取一个connection，然后初始化 

![](https://img-blog.csdn.net/20131228192710453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Free_connections指向链表下一个节点，返回头结点。

同时，cycle->files[fd]也指向返回的结点



Ngx_close_connection

 主要关闭一个connection，包括“善后”以及调用ngx_reusable_connection(c,0) ngx_free_connection来将连接放回free_connections

可以认为是ngx_get_connection的逆操作





ngx_reusable_connection

 ngx_reusable_connection(ngx_connection_t *c, ngx_uint_t reusable) 

 reusable=1 ，放进queue中 

 reusable=0 ，从queue中出来 

Ngx_free_connection

 将使用完的连接放回free_connections 





Ngx_drain_connection

当ngx_get_connection获取不到连接时（即并发比较高的时候，连接都用完了），那么使用

ngx_drain_connections来释放长连接，将长连接从queue拿出来，放回到free_connections，然后再获取






ngx_event_connect_peer

当从upstream中获得一个后端时，就会调用ngx_event_connect_peer去进行连接，ngx_event_connect_peer会调用 Ngx_get_connection得到一个connection结构体，然后去执行connect操作。



```cpp
ngx_event_connect_peer(ngx_peer_connection_t *pc)
{
。。。
s = ngx_socket(pc->sockaddr->sa_family, SOCK_STREAM, 0);
。。。
c = ngx_get_connection(s, pc->log);
。。。
rc = connect(s, pc->sockaddr, pc->socklen);
。。。
}
```









# 5. Nginx的accept_mutex锁机制

为了全面理解，我们先捋一下这个过程

1.master进程bind端口，listen后，生成了监听套接字

2.Master进程fork出work进程，work进程继承了监听套接字，执行accept获得请求

所有的work进程继承了同样的监听套接字，那么一个连接过来的时候，多个空闲的进程，会竞争这个连接，会是哪个work进程来处理这个连接呢？

如果某个进程得到accept的机会比较多，它的空闲连接很 快就用完了，如果不提前做一些控制，当accept到一个新的tcp连接后，因为无法得到空闲连接，而且无法将此连接转交给其它进程，最终会导致此tcp 连接得不到处理，就中止掉了。这是不公平的。

我们必须设计一种机制，让一个请求，有且仅有一个work进程处理，也就是说只能有一个进程accept到这个连接。同时每个进程有大致均等的机会来处理连接。





我们看nginx是怎么做的？

在master进程中已经建立了listen套接字，

Src/core/ngx_cycle.c:ngx_init_cycle

```cpp
{

...

ngx_queue_init(&cycle->reusable_connections_queue);

...

    if (ngx_open_listening_sockets(cycle) != NGX_OK) {

        goto failed;

    }

}

在src/core/ngx_connection.c:ngx_open_listening_sockets中

{

ls = cycle->listening.elts;

s = ngx_socket(ls[i].sockaddr->sa_family, ls[i].type, 0);

if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,

                           (const void *) &reuseaddr, sizeof(int))

                == -1)

            {...

  

if (bind(s, ls[i].sockaddr, ls[i].socklen) == -1) {...

if (listen(s, ls[i].backlog) == -1) {...

ls[i].listen = 1;

ls[i].fd = s;

}
```



Work进程继承了master进程的listen套接字后，循环监听网络事件

Src/os/unix/ngx_process_cycle.c:ngx_worker_process_cycle

```cpp
{

...

ngx_process_events_and_timers(cycle);

...

}

Src/event/ngx_event.c:ngx_process_events_andtimers(cycle)

{...

if(ngx_use_accept_mutex) {

if(ngx_accept_disabled > 0) {

ngx_accept_disabled--;

    

//在src/event/nginx_event_accept.c:ngx_event_accept()中计算：ngx_accept_disabled = ngx_cycle->connection_n / 8 - ngx_cycle->free_connection_n;   初始值为0，以后每来新建一个连接，都会更新一下值。

  

//当剩余连接数小于最大连接数的1/8的时候为正，表示连接有点多了，于是放弃一次争锁定机会 

  

}else{

if(ngx_trylock_accept_mutex(cycle) == NGX_ERROR) {

  

//这里ngx_trylock_accept_mutex函数就是争锁定函数，成功争得了锁则将全局变量ngx_accept_mutex_held置为1，否则置0 

return;

}

  

if(ngx_accept_mutex_held) {

  flags |= NGX_POST_EVENTS;

  

//占用了accept锁的进程在处理事件的时候是先将事件放入队列，后续慢慢处理，以便尽快走到下面释放锁。 

  

}else{

  

//没争得锁的进程不需要分两步处理事件，但是把处理事件的timer更新为ngx_accept_mutex_delay 

if(timer == NGX_TIMER_INFINITE

  || timer > ngx_accept_mutex_delay)

{

  timer = ngx_accept_mutex_delay;

}

}

}

}

  

delta = ngx_current_msec;
```



惊群

http://www.usenix.org/event/usenix2000/freenix/full_papers/molloy/molloy.pdf 

[http://simohayha.iteye.com/blog/561424](http://simohayha.iteye.com/blog/561424)

[http://blog.csdn.net/benbendy1984/article/details/5620482](http://blog.csdn.net/benbendy1984/article/details/5620482)

http://blog.csdn.net/russell_tao/article/details/7204260





































