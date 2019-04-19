# libevent参考手册第六章：bufferevent：概念和入门 - u013366022的专栏 - CSDN博客
2016年11月30日 15:20:58[slitaz](https://me.csdn.net/u013366022)阅读数：199
很多时候，除了响应事件之外，应用还希望做一定的数据缓冲。比如说，写入数据的时候，通常的运行模式是：
l  决定要向连接写入一些数据，把数据放入到缓冲区中。
l  等待连接可以写入。
l  写入尽量多的数据。
l  记住写入了多少数据，如果还有更多数据要写入，等待连接再次可以写入。
这种缓冲IO模式很通用，libevent为此提供了一种通用机制，即bufferevent。bufferevent由一个底层的传输端口（如套接字），一个读取缓冲区和一个写入缓冲区组成。与通常的事件在底层传输端口已经就绪，可以读取或者写入的时候执行回调不同的是，bufferevent在读取或者写入了足够量的数据之后调用用户提供的回调。
有多种共享公用接口的bufferevent类型，编写本文时已存在以下类型：
u  **基于套接字的bufferevent**：使用event_*接口作为后端，通过底层流式套接字发送或者接收数据的bufferevent
u  **异步IO bufferevent**：使用Windows IOCP接口，通过底层流式套接字发送或者接收数据的bufferevent（仅用于Windows，试验中）
u  **过滤型bufferevent**：将数据传输到底层bufferevent对象之前，处理输入或者输出数据的bufferevent：比如说，为了压缩或者转换数据。
u  **成对的bufferevent**：相互传输数据的两个bufferevent。
**注意**：截止2.0.2-alpha版，这里列出的bufferevent接口还没有完全正交于所有的bufferevent类型。也就是说，下面将要介绍的接口不是都能用于所有bufferevent类型。libevent开发者在未来版本中将修正这个问题。
**也请注意**：当前bufferevent只能用于像TCP这样的面向流的协议，将来才可能会支持像UDP这样的面向数据报的协议。
本节描述的所有函数和类型都在event2/bufferevent.h中声明。特别提及的关于evbuffer的函数声明在event2/buffer.h中，详细信息请参考下一章。
## 1 bufferevent和evbuffer
每个bufferevent都有一个输入缓冲区和一个输出缓冲区，它们的类型都是“struct evbuffer”。有数据要写入到bufferevent时，添加数据到输出缓冲区；bufferevent中有数据供读取的时候，从输入缓冲区抽取（drain）数据。
evbuffer接口支持很多种操作，后面的章节将讨论这些操作。
## 2 回调和水位
每个bufferevent有两个数据相关的回调：一个读取回调和一个写入回调。默认情况下，从底层传输端口读取了任意量的数据之后会调用读取回调；输出缓冲区中足够量的数据被清空到底层传输端口后写入回调会被调用。通过调整bufferevent的读取和写入“水位（watermarks）”可以覆盖这些函数的默认行为。
每个bufferevent有四个水位：
u  **读取低水位**：读取操作使得输入缓冲区的数据量在此级别或者更高时，读取回调将被调用。默认值为0，所以每个读取操作都会导致读取回调被调用。
u  **读取高水位**：输入缓冲区中的数据量达到此级别后，bufferevent将停止读取，直到输入缓冲区中足够量的数据被抽取，使得数据量低于此级别。默认值是无限，所以永远不会因为输入缓冲区的大小而停止读取。
u  **写入低水位**：写入操作使得输出缓冲区的数据量达到或者低于此级别时，写入回调将被调用。默认值是0，所以只有输出缓冲区空的时候才会调用写入回调。
u  **写入高水位**：bufferevent没有直接使用这个水位。它在bufferevent用作另外一个bufferevent的底层传输端口时有特殊意义。请看后面关于过滤型bufferevent的介绍。
bufferevent也有“错误”或者“事件”回调，用于向应用通知非面向数据的事件，如连接已经关闭或者发生错误。定义了下列事件标志：
l  **BEV_EVENT_READING**：读取操作时发生某事件，具体是哪种事件请看其他标志。
l  **BEV_EVENT**_**WRITING**：写入操作时发生某事件，具体是哪种事件请看其他标志。
l  **BEV_EVENT_ERROR**：操作时发生错误。关于错误的更多信息，请调用EVUTIL_SOCKET_ERROR()。
l  **BEV_EVENT_TIMEOUT**：发生超时。
l  **BEV_EVENT_EOF**：遇到文件结束指示。
l  **BEV_EVENT_CONNECTED**：请求的连接过程已经完成。
上述标志由2.0.2-alpha版新引入。
## 3 延迟回调
默认情况下，bufferevent的回调在相应的条件发生时立即被执行。（evbuffer的回调也是这样的，随后会介绍）在依赖关系复杂的情况下，这种立即调用会制造麻烦。比如说，假如某个回调在evbuffer A空的时候向其中移入数据，而另一个回调在evbuffer A满的时候从中取出数据。这些调用都是在栈上发生的，在依赖关系足够复杂的时候，有栈溢出的风险。
要解决此问题，可以请求bufferevent（或者evbuffer）延迟其回调。条件满足时，延迟回调不会立即调用，而是在event_loop（）调用中被排队，然后在定期的事件回调之后执行。
（延迟回调由libevent 2.0.1-alpha版引入）
## 4 bufferevent的选项标志
创建bufferevent时可以使用一个或者多个标志修改其行为。可识别的标志有：
l  **BEV_OPT_CLOSE_ON_FREE**：释放bufferevent时关闭底层传输端口。这将关闭底层套接字，释放底层bufferevent等。
l  **BEV_OPT_THREADSAFE**：自动为bufferevent分配锁，这样就可以安全地在多个线程中使用bufferevent。
l  **BEV_OPT_DEFER**_**CALLBACKS**：设置这个标志时，bufferevent延迟所有回调，如上所述。
l  **BEV_OPT_UNLOCK_CALLBACKS**：默认情况下，如果设置bufferevent为线程安全的，则bufferevent会在调用用户提供的回调时进行锁定。设置这个选项会让libevent在执行回调的时候不进行锁定。
（BEV_OPT_UNLOCK_CALLBACKS由2.0.5-beta版引入，其他选项由2.0.1-alpha版引入）
## 5 与基于套接字的bufferevent一起工作
基于套接字的bufferevent是最简单的，它使用libevent的底层事件机制来检测底层网络套接字是否已经就绪，可以进行读写操作，并且使用底层网络调用（如readv、writev、WSASend、WSARecv）来发送和接收数据。
## 5.1 创建基于套接字的bufferevent
可以使用bufferevent_socket_new()创建基于套接字的bufferevent。
### 接口
**struct** bufferevent *bufferevent_socket_new(
**struct** event_base *base,
    evutil_socket_t fd,
**enum** bufferevent_options options);
base是event_base，options是表示bufferevent选项（BEV_OPT_CLOSE_ON_FREE等）的位掩码，fd是一个可选的表示套接字的文件描述符。如果想以后设置文件描述符，可以设置fd为-1。
成功时函数返回一个bufferevent，失败则返回NULL。
bufferevent_socket_new()函数由2.0.1-alpha版新引入。
## 5.2 在基于套接字的bufferevent上启动连接
如果bufferevent的套接字还没有连接上，可以启动新的连接。
### 接口
**int** bufferevent_socket_connect(**struct** bufferevent *bev,**struct** sockaddr *address, **int** addrlen);
address和addrlen参数跟标准调用connect()的参数相同。如果还没有为bufferevent设置套接字，调用函数将为其分配一个新的流套接字，并且设置为非阻塞的。
如果已经为bufferevent设置套接字，调用bufferevent_socket_connect()将告知libevent套接字还未连接，直到连接成功之前不应该对其进行读取或者写入操作。
连接完成之前可以向输出缓冲区添加数据。
如果连接成功启动，函数返回0；如果发生错误则返回-1。
### 示例
#include <event2/event.h>#include <event2/bufferevent.h>#include <sys/socket.h>#include <string.h>**void** eventcb(**struct** bufferevent *bev, **short** events, **void** *ptr){**if** (events & BEV_EVENT_CONNECTED) {*         /* We're connected to 127.0.0.1:8080.   Ordinarily we'd do**            something here, like start reading or writing. */*    } **else****if** (events & BEV_EVENT_ERROR) {*         /* An error occured while connecting. */*    }}**int** main_loop(**void**){**struct** event_base *base;**struct** bufferevent *bev;**struct** sockaddr_in sin;    base = event_base_new();    memset(&sin, 0, **sizeof**(sin));    sin.sin_addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */    sin.sin_port = htons(8080); /* Port 8080 */    bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);    bufferevent_setcb(bev, NULL, NULL, eventcb, NULL);**if** (bufferevent_socket_connect(bev,        (**struct** sockaddr *)&sin, **sizeof**(sin)) < 0) {*        /* Error starting connection */*        bufferevent_free(bev);**return** -1;    }    event_base_dispatch(base);**return** 0;}
bufferevent_socket_connect()函数由2.0.2-alpha版引入。在此之前，必须自己手动在套接字上调用connect()，连接完成时，bufferevent将报告写入事件。
**注意：如果使用bufferevent_socket_connect()发起连接，将只会收到BEV_EVENT_CONNECTED事件。如果自己调用connect()，则连接上将被报告为写入事件。**
这个函数在2.0.2-alpha版引入。
## 5.3 通过主机名启动连接
常常需要将解析主机名和连接到主机合并成单个操作，libevent为此提供了：
### 接口
**int** bufferevent_socket_connect_hostname(**struct** bufferevent *bev,**struct** evdns_base *dns_base, **int** family, **const****char** *hostname,**int** port);**int** bufferevent_socket_get_dns_error(**struct** bufferevent *bev);
这个函数解析名字hostname，查找其family类型的地址（允许的地址族类型有AF_INET,AF_INET6和AF_UNSPEC）。如果名字解析失败，函数将调用事件回调，报告错误事件。如果解析成功，函数将启动连接请求，就像bufferevent_socket_connect()一样。
dns_base参数是可选的：如果为NULL，等待名字查找完成期间调用线程将被阻塞，而这通常不是期望的行为；如果提供dns_base参数，libevent将使用它来异步地查询主机名。关于DNS的更多信息，请看第九章。
跟bufferevent_socket_connect()一样，函数告知libevent，bufferevent上现存的套接字还没有连接，在名字解析和连接操作成功完成之前，不应该对套接字进行读取或者写入操作。
函数返回的错误可能是DNS主机名查询错误，可以调用bufferevent_socket_get_dns_error()来获取最近的错误。返回值0表示没有检测到DNS错误。
### 示例：简单的HTTP v0客户端
*/* Don't actually copy this code: it is a poor way to implement an**   HTTP client.  Have a look at evhttp instead.***/*#include <event2/dns.h>#include <event2/bufferevent.h>#include <event2/buffer.h>#include <event2/util.h>#include <event2/event.h>#include <stdio.h>**void** readcb(**struct** bufferevent *bev, **void** *ptr){**char** buf[1024];**int** n;**struct** evbuffer *input = bufferevent_get_input(bev);**while** ((n = evbuffer_remove(input, buf, **sizeof**(buf))) > 0) {        fwrite(buf, 1, n, stdout);    }}**void** eventcb(**struct** bufferevent *bev, **short** events, **void** *ptr){**if** (events & BEV_EVENT_CONNECTED) {         printf("Connect okay./n");    } **else****if** (events & (BEV_EVENT_ERROR|BEV_EVENT_EOF)) {**struct** event_base *base = ptr;**if** (events & BEV_EVENT_ERROR) {**int** err = bufferevent_socket_get_dns_error(bev);**if** (err)                         printf("DNS error: %s/n", evutil_gai_strerror(err));         }         printf("Closing/n");         bufferevent_free(bev);         event_base_loopexit(base, NULL);    }}**int** main(**int** argc, **char** **argv){**struct** event_base *base;**struct** evdns_base *dns_base;**struct** bufferevent *bev;**if** (argc != 3) {        printf("Trivial HTTP 0.x client/n"               "Syntax: %s [hostname] [resource]/n"               "Example: %s www.google.com //n",argv[0],argv[0]);**return** 1;    }    base = event_base_new();    dns_base = evdns_base_new(base, 1);    bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);    bufferevent_setcb(bev, readcb, NULL, eventcb, base);    bufferevent_enable(bev, EV_READ|EV_WRITE);    evbuffer_add_printf(bufferevent_get_output(bev), "GET %s/r/n", argv[2]);    bufferevent_socket_connect_hostname(        bev, dns_base, AF_UNSPEC, argv[1], 80);    event_base_dispatch(base);**return** 0;}
## 6 通用bufferevent操作
本节描述的函数可用于多种bufferevent实现。
## 6.1 释放bufferevent
### 接口
**void** bufferevent_free(**struct** bufferevent *bev);
这个函数释放bufferevent。bufferevent内部具有引用计数，所以，如果释放bufferevent时还有未决的延迟回调，则在回调完成之前bufferevent不会被删除。
如果设置了BEV_OPT_CLOSE_ON_FREE标志，并且bufferevent有一个套接字或者底层bufferevent作为其传输端口，则释放bufferevent将关闭这个传输端口。
这个函数由libevent 0.8版引入。
## 6.2 操作回调、水位和启用/禁用
### 接口
**typedef****void** (*bufferevent_data_cb)(**struct** bufferevent *bev, **void** *ctx);**typedef****void** (*bufferevent_event_cb)(**struct** bufferevent *bev,**short** events, **void** *ctx);**void** bufferevent_setcb(**struct** bufferevent *bufev,    bufferevent_data_cb readcb, bufferevent_data_cb writecb,    bufferevent_event_cb eventcb, **void** *cbarg);
bufferevent_setcb()函数修改bufferevent的一个或者多个回调。readcb、writecb和eventcb函数将分别在已经读取足够的数据、已经写入足够的数据，或者发生错误时被调用。每个回调函数的第一个参数都是发生了事件的bufferevent，最后一个参数都是调用bufferevent_setcb()时用户提供的cbarg参数：可以通过它向回调传递数据。事件回调的events参数是一个表示事件标志的位掩码：请看前面的“回调和水位”节。
要禁用回调，传递NULL而不是回调函数。**注意：bufferevent的所有回调函数共享单个cbarg，所以修改它将影响所有回调函数。**
这个函数由1.4.4版引入。类型名bufferevent_data_cb和bufferevent_event_cb由2.0.2-alpha版引入。
### 接口
**void** bufferevent_enable(**struct** bufferevent *bufev, **short** events);**void** bufferevent_disable(**struct** bufferevent *bufev, **short** events);**short** bufferevent_get_enabled(**struct** bufferevent *bufev);
可以启用或者禁用bufferevent上的EV_READ、EV_WRITE或者EV_READ | EV_WRITE事件。没有启用读取或者写入事件时，bufferevent将不会试图进行数据读取或者写入。
没有必要在输出缓冲区空时禁用写入事件：bufferevent将自动停止写入，然后在有数据等待写入时重新开始。
类似地，没有必要在输入缓冲区高于高水位时禁用读取事件：bufferevent将自动停止读取，然后在有空间用于读取时重新开始读取。
默认情况下，新创建的bufferevent的写入是启用的，但是读取没有启用。
可以调用bufferevent_get_enabled()确定bufferevent上当前启用的事件。
除了bufferevent_get_enabled()由2.0.3-alpha版引入外，这些函数都由0.8版引入。
### 接口
**void** bufferevent_setwatermark(**struct** bufferevent *bufev, **short** events,    size_t lowmark, size_t highmark);
bufferevent_setwatermark()函数调整单个bufferevent的读取水位、写入水位，或者同时调整二者。（如果events参数设置了EV_READ，调整读取水位。如果events设置了EV_WRITE标志，调整写入水位）
对于高水位，0表示“无限”。
这个函数首次出现在1.4.4版。
### 示例
#include <event2/event.h>#include <event2/bufferevent.h>#include <event2/buffer.h>#include <event2/util.h>#include <stdlib.h>#include <errno.h>#include <string.h>**struct** info {**const****char** *name;    size_t total_drained;};**void** read_callback(**struct** bufferevent *bev, **void** *ctx){**struct** info *inf = ctx;**struct** evbuffer *input = bufferevent_get_input(bev);    size_t len = evbuffer_get_length(input);**if** (len) {        inf->total_drained += len;        evbuffer_drain(input, len);        printf("Drained %lu bytes from %s/n",             (**unsigned****long**) len, inf->name);    }}**void** event_callback(**struct** bufferevent *bev, **short** events, **void** *ctx){**struct** info *inf = ctx;**struct** evbuffer *input = bufferevent_get_input(bev);**int** finished = 0;**if** (events & BEV_EVENT_EOF) {        size_t len = evbuffer_get_length(input);        printf("Got a close from %s.  We drained %lu bytes from it, "            "and have %lu left./n", inf->name,            (**unsigned****long**)inf->total_drained, (**unsigned****long**)len);        finished = 1;    }**if** (events & BEV_EVENT_ERROR) {        printf("Got an error from %s: %s/n",            inf->name, evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));        finished = 1;    }**if** (finished) {        free(ctx);        bufferevent_free(bev);    }}**struct** bufferevent *setup_bufferevent(**void**){**struct** bufferevent *b1 = NULL;**struct** info *info1;    info1 = malloc(**sizeof**(**struct** info));    info1->name = "buffer 1";    info1->total_drained = 0;*    /* ... Here we should set up the bufferevent and make sure it gets**       connected... */**    /* Trigger the read callback only whenever there is at least 128 bytes**       of data in the buffer. */*    bufferevent_setwatermark(b1, EV_READ, 128, 0);    bufferevent_setcb(b1, read_callback, NULL, event_callback, info1);    bufferevent_enable(b1, EV_READ); /* Start reading. */**return** b1;}
## 6.3 操作bufferevent中的数据
如果只是通过网络读取或者写入数据，而不能观察操作过程，是没什么好处的。bufferevent提供了下列函数用于观察要写入或者读取的数据。(Reading and writing data from the network does you no good if you can't look at it.Bufferevents give you these methods to give them data to write,and to get the data to read.)
### 接口
**struct** evbuffer *bufferevent_get_input(**struct** bufferevent *bufev);**struct** evbuffer *bufferevent_get_output(**struct** bufferevent *bufev);
这两个函数提供了非常强大的基础：它们分别返回输入和输出缓冲区。关于可以对evbuffer类型进行的所有操作的完整信息，请看下一章。
如果写入操作因为数据量太少而停止（或者读取操作因为太多数据而停止），则向输出缓冲区添加数据（或者从输入缓冲区移除数据）将自动重启操作。
这些函数由2.0.1-alpha版引入。
### 接口
**int** bufferevent_write(**struct** bufferevent *bufev,**const****void** *data, size_t size);**int** bufferevent_write_buffer(**struct** bufferevent *bufev,**struct** evbuffer *buf);
这些函数向bufferevent的输出缓冲区添加数据。bufferevent_write()将内存中从data处开始的size字节数据添加到输出缓冲区的末尾。bufferevent_write_buffer()移除buf的所有内容，将其放置到输出缓冲区的末尾。成功时这些函数都返回0，发生错误时则返回-1。
这些函数从0.8版就存在了。
### 接口
size_t bufferevent_read(**struct** bufferevent *bufev, **void** *data, size_t size);**int** bufferevent_read_buffer(**struct** bufferevent *bufev,**struct** evbuffer *buf);
这些函数从bufferevent的输入缓冲区移除数据。bufferevent_read()至多从输入缓冲区移除size字节的数据，将其存储到内存中data处。函数返回实际移除的字节数。bufferevent_read_buffer()函数抽空输入缓冲区的所有内容，将其放置到buf中，成功时返回0，失败时返回-1。
注意，对于bufferevent_read()，data处的内存块必须有足够的空间容纳size字节数据。
bufferevent_read()函数从0.8版就存在了；bufferevnet_read_buffer()由2.0.1-alpha版引入。
### 示例
#include <event2/bufferevent.h>#include <event2/buffer.h>#include <ctype.h>**void**read_callback_uppercase(**struct** bufferevent *bev, **void** *ctx){*        /* This callback removes the data from bev's input buffer 128**           bytes at a time, uppercases it, and starts sending it**           back.**           (Watch out!  In practice, you shouldn't use toupper to implement**           a network protocol, unless you know for a fact that the current**           locale is the one you want to be using.)**         */***char** tmp[128];        size_t n;**int** i;**while** (1) {                n = bufferevent_read(bev, tmp, **sizeof**(tmp));**if** (n <= 0)**break**; /* No more data. */**for** (i=0; i<n; ++i)                        tmp[i] = toupper(tmp[i]);                bufferevent_write(bev, tmp, n);        }}**struct** proxy_info {**struct** bufferevent *other_bev;};**void**read_callback_proxy(**struct** bufferevent *bev, **void** *ctx){*        /* You might use a function like this if you're implementing**           a simple proxy: it will take data from one connection (on**           bev), and write it to another, copying as little as**           possible. */***struct** proxy_info *inf = ctx;        bufferevent_read_buffer(bev,            bufferevent_get_output(inf->other_bev));}**struct** count {**unsigned****long** last_fib[2];};**void**write_callback_fibonacci(**struct** bufferevent *bev, **void** *ctx){*        /* Here's a callback that adds some Fibonacci numbers to the**           output buffer of bev.  It stops once we have added 1k of**           data; once this data is drained, we'll add more. */***struct** count *c = ctx;**struct** evbuffer *tmp = evbuffer_new();**while** (evbuffer_get_length(tmp) < 1024) {**unsigned****long** next = c->last_fib[0] + c->last_fib[1];                 c->last_fib[0] = c->last_fib[1];                 c->last_fib[1] = next;                 evbuffer_add_printf(tmp, "%lu", next);        }*        /* Now we add the whole contents of tmp to bev. */*        bufferevent_write_buffer(bev, tmp);*        /* We don't need tmp any longer. */*        evbuffer_free(tmp);}
## 6.4 读写超时
跟其他事件一样，可以要求在一定时间内，而没有成功写入或者读取数据的时候调用一个超时回调。
### 接口
**void** bufferevent_set_timeouts(**struct** bufferevent *bufev,**const****struct** timeval *timeout_read, **const****struct** timeval *timeout_write);
设置超时为NULL会移除超时回调。
试图读取数据的时候，如果至少等待了timeout_read秒，则读取超时事件将被触发。试图写入数据的时候，如果至少等待了timeout_write秒，则写入超时事件将被触发。
注意，只有在读取或者写入的时候才会计算超时。也就是说，如果bufferevent的读取被禁止，或者输入缓冲区满（达到其高水位），则读取超时被禁止。类似的，如果写入被禁止，或者没有数据待写入，则写入超时被禁止。
读取或者写入超时发生时，相应的读取或者写入操作被禁止，然后超时事件回调被调用，带有标志BEV_EVENT_TIMEOUT | BEV_EVENT_READING或者BEV_EVENT_TIMEOUT | BEV_EVENT_WRITING。
这个函数从2.0.1-alpha版就存在了，但是直到2.0.4-alpha版才对于各种bufferevent类型行为一致。
## 6.5 对bufferevent发起清空操作
### 接口
**int** bufferevent_flush(**struct** bufferevent *bufev,**short** iotype, **enum** bufferevent_flush_mode state);
清空bufferevent要求bufferevent强制从底层传输端口读取或者写入尽可能多的数据，而忽略其他可能保持数据不被写入的限制条件。函数的细节功能依赖于bufferevent的具体类型。
iotype参数应该是EV_READ、EV_WRITE或者EV_READ | EV_WRITE，用于指示应该处理读取、写入，还是二者都处理。state参数可以是BEV_NORMAL、BEV_FLUSH或者BEV_FINISHED。BEV_FINISHED指示应该告知另一端，没有更多数据需要发送了；而BEV_NORMAL和BEV_FLUSH的区别依赖于具体的bufferevent类型。
失败时bufferevent_flush()返回-1，如果没有数据被清空则返回0，有数据被清空则返回1。
当前（2.0.5-beta版）仅有一些bufferevent类型实现了bufferevent_flush()。特别是，基于套接字的bufferevent没有实现。
## 7 类型特定的bufferevent函数
这些bufferevent函数不能支持所有bufferevent类型。
### 接口
**int** bufferevent_priority_set(**struct** bufferevent *bufev, **int** pri);
这个函数调整bufev的优先级为pri。关于优先级的更多信息请看event_priority_set()。
成功时函数返回0，失败时返回-1。这个函数仅能用于基于套接字的bufferevent。
这个函数由1.0版引入。
### 接口
**int** bufferevent_setfd(**struct** bufferevent *bufev, evutil_socket_t fd);evutil_socket_t bufferevent_getfd(**struct** bufferevent *bufev);
这些函数设置或者返回基于fd的事件的文件描述符。只有基于套接字的bufferevent支持setfd()。两个函数都在失败时返回-1；setfd()成功时返回0。
bufferevent_setfd()函数由1.4.4版引入；bufferevent_getfd()函数由2.0.2-alpha版引入。
### 接口
**struct** event_base *bufferevent_get_base(**struct** bufferevent *bev);
这个函数返回bufferevent的event_base，由2.0.9-rc版引入。
### 接口
**struct** bufferevent *bufferevent_get_underlying(**struct** bufferevent *bufev);
这个函数返回作为bufferevent底层传输端口的另一个bufferevent。关于这种情况，请看关于过滤型bufferevent的介绍。
这个函数由2.0.2-alpha版引入。
## 8 手动锁定和解锁
有时候需要确保对bufferevent的一些操作是原子地执行的。为此，libevent提供了手动锁定和解锁bufferevent的函数。
### 接口
**void** bufferevent_lock(**struct** bufferevent *bufev);**void** bufferevent_unlock(**struct** bufferevent *bufev);
**注意：如果创建bufferevent时没有指定BEV_OPT_THREADSAFE标志，或者没有激活libevent的线程支持，则锁定操作是没有效果的。**
用这个函数锁定bufferevent将自动同时锁定相关联的evbuffer。这些函数是递归的：锁定已经持有锁的bufferevent是安全的。当然，对于每次锁定都必须进行一次解锁。
这些函数由2.0.6-rc版引入。
## 9 已废弃的bufferevent功能
从1.4到2.0版，bufferevent的后端代码一直在进行修订。在老的接口中，访问bufferevent结构体的内部是很平常的，并且还会使用依赖于这种访问的宏。
更复杂的是，老的代码有时候将“evbuffer”前缀用于bufferevent功能。
这里有一个在2.0版之前使用过的东西的概要：
|**Current name**|**Old name**|
|----|----|
|bufferevent_data_cb|evbuffercb|
|bufferevent_event_cb|everrorcb|
|BEV_EVENT_READING|EVBUFFER_READ|
|BEV_EVENT_WRITE|EVBUFFER_WRITE|
|BEV_EVENT_EOF|EVBUFFER_EOF|
|BEV_EVENT_ERROR|EVBUFFER_ERROR|
|BEV_EVENT_TIMEOUT|EVBUFFER_TIMEOUT|
|bufferevent_get_input(b)|EVBUFFER_INPUT(b)|
|bufferevent_get_output(b)|EVBUFFER_OUTPUT(b)|
老的函数定义在event.h中，而不是在event2/bufferevent.h中。
如果仍然需要访问bufferevent结构体内部的某些公有部分，可以包含event2/bufferevent_struct.h。但是不建议这么做：不同版本的Libevent中bufferevent结构体的内容可能会改变。本节描述的宏和名字只有在包含了event2/bufferevent_compat.h时才能使用。
较老版本中用于设置bufferevent的接口有所不同：
### 接口
**struct** bufferevent *bufferevent_new(evutil_socket_t fd,    evbuffercb readcb, evbuffercb writecb, everrorcb errorcb, **void** *cbarg);**int** bufferevent_base_set(**struct** event_base *base, **struct** bufferevent *bufev);
bufferevent_new()函数仅仅在已经废弃的“默认”event_base上创建一个套接字bufferevent。调用bufferevent_base_set()可以调整套接字bufferevent的event_base。
较老版本不使用timeval结构体设置超时，而是使用秒数：
### 接口
**void** bufferevent_settimeout(**struct** bufferevent *bufev,**int** timeout_read, **int** timeout_write);
最后要指出的是，2.0之前版本中的evbuffer实现是极其低效的，这对将bufferevent用于高性能应用是一个问题。
原文：http://www.wangafu.net/~nickm/libevent-book/Ref6_bufferevent.html
