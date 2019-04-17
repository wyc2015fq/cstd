# 理解HTTP/2流量控制(一) - 囧囧有神的专栏 - CSDN博客





2017年03月06日 03:10:34[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：3122








在理解HTTP/2协议流量控制部分的过程中，容易被一些问题困惑。HTTP/2构建于tcp之上，tcp协议已经提供了流量控制机制，HTTP/2的流量控制有何不同？HTTP/2流量控制是什么原理，怎样才能发挥它的最大价值？  本文将从协议、源码来寻找问题的答案。



# 协议中怎么说？

协议中对流量控制的目的和特点有清晰的描述


```
Flow control (Section 5.2)helps to ensure that only data that can be used by a receiver is transmitted.

流量控制有助于确保只传播接受者需要使用的数据数据。
```





```
Using streams for multiplexing introduces contention over use of the TCP connection, resulting in blocked streams. A flow control scheme ensures that streams on the same connection do not destructively interfere with each other. Flow control is used for both individual streams and for the connection as a whole.

HTTP/2 provides for flow control through use of the WINDOW_UPDATE frame (Section 6.9).

HTTP/2利用流来实现多路复用，这引入了对TCP连接的使用争夺，会造成流被阻塞。流量控制方案确保在同一连接上的多个流之间不会造成破坏性的干扰。流量控制会用于各个独立的流，也会用于整个连接。

HTTP/2通过使用WINDOW_UPDATE帧来进行流量控制。
```




HTTP/2“流”的流量控制的目标是：在不改变协议的情况下允许使用多种流量控制算法。




HTTP/2的流量控制具有以下特征：
- 流量控制是特定于一个连接的。每种类型的流量控制都是在单独的一跳的两个端点之间的，并不是在整个端到端的路径上的。（这里的一跳指的是HTTP连接的一跳，而不是IP路由的一跳）
- 流量控制是基于WINDOW_UPDATE帧的。接收方公布自己打算在每个流以及整个连接上分别接收多少字节。这是一个以信用为基础的方案。
- 流量控制是有方向的，由接收者全面控制。接收方可以为每个流和整个连接设置任意的窗口大小。发送方必须尊重接收方设置的流量控制限制。客户方、服务端和中间代理作为接收方时都独立地公布各自的流量控制窗口，作为发送方时都遵守对端的流量控制设置。
- 无论是新流还是整个连接，流量控制窗口的初始值是65535字节。
- 帧的类型决定了流量控制是否适用于帧。目前，只有DATA帧服从流量控制，所有其它类型的帧并不消耗流量控制窗口的空间。这保证了重要的控制帧不会被流量控制阻塞。
- 流量控制不能被禁用。
- HTTP/2只定义了WINDOW_UPDATE帧的格式和语义，并没有规定接收方如何决定何时发送帧、发送什么样的值，也没有规定发送方如何选择发送包。具体实现可以选择任何满足需求的算法。
**重点看第7条，HTTP/2没有提出流量控制的具体算法，实现者可以有最大的自由度，去实现任何算法。**





# 怎么实现的？

## 基本概念

### 流量控制窗口 (Flow Control Window)






HTTP/2中流量控制是通过每个流上每个发送端，保持一个窗口来实现的。流量控制窗口是一个简单的整数值，指示发送端被允许传输的字节数；因此，它的大小是接收端的缓存能力的衡量。



流量控制窗口对流和连接的流量控制窗口都适用。发送端绝对不能发送超出接收端广播的流量控制窗口大小的可用空间长度的受流量控制影响的帧。在各个流量控制窗口中没有可用空间时，可以发送带有END_STREAM标记的长度为0的帧(例如，空数据帧)。

### WINDOW_UPDATE帧

HTTP/2定义的帧类型的一种，用途是通知对端增加窗口值，WINDOW_UPDATE会指定增加的大小







**算法描述：**

- 发送端保有一个流量控制窗口（window）初始值。初始值的设定请参考SETTING 帧的 SETTINGS_INITIAL_WINDOW_SIZE
- 发送端每发送一个DATA帧，就把window递减，递减量为这个帧的大小。如果当前window小于帧大小，那么这个帧就必须被拆分到不大于window，如果window等于0，就不能发送任何帧
- 接收端可以发送 WINDOW_UPDATE帧给发送端，发送端以帧内指定的Window Size Increment作为增量，加到window上





我们来看看nginx 1.10.3中相关的源码

看看在各种情况下流量控制窗口值的变化。

nginx定义了一个recv_window用来控制发送window_update帧的时机

## 基本数据结构


```
struct ngx_http_v2_connection_s {                           //HTTP/2连接数据结构
    ngx_connection_t                *connection;
    ngx_http_connection_t           *http_connection;

    ngx_uint_t                       processing;

    size_t                           send_window;           //发送窗口
    size_t                           recv_window;           //接收窗口
    size_t                           init_window;           //初始化窗口大小 用于初始化stream recv_window以及发送给客户端SETTING帧中的**SETTINGS_INITIAL_WINDOW_SIZE** 

    size_t                           frame_size;

    ngx_queue_t                      waiting;

    ngx_http_v2_state_t              state;

    ngx_http_v2_hpack_t              hpack;

    ngx_pool_t                      *pool;

    ngx_http_v2_out_frame_t         *free_frames;
    ngx_connection_t                *free_fake_connections;

    ngx_http_v2_node_t             **streams_index;

    ngx_http_v2_out_frame_t         *last_out;

    ngx_queue_t                      dependencies;
    ngx_queue_t                      closed;

    ngx_uint_t                       last_sid;

    unsigned                         closed_nodes:8;
    unsigned                         settings_ack:1;
    unsigned                         blocked:1;
    unsigned                         goaway:1;
};
```




## nginx相关配置

指令：http2_body_preread_size

默认：http2_body_preread_size 65535

上下文：http server

用来定义 Nginx 在客户端收到 SETTINGS 帧之前可以接受多大的 DATA 帧，默认为 64KB。





这个值设置为0，可能会导致一个POST bug。详情见： https://imququ.com/post/nginx-http2-post-bug.html

## 请求处理部分

### 建立连接(connection)初始阶段


NGINX 会设置整个连接的接收窗口和发送窗口的初始大小



```cpp
h2c->send_window = NGX_HTTP_V2_DEFAULT_WINDOW;   // 默认65535。 这个就是协议中说到的流量控制窗口
    h2c->recv_window = NGX_HTTP_V2_MAX_WINDOW;       // 值为2^31-1  接收窗口是nginx衡量自身接收能力定义出来的，并没有在协议中出现。

    h2c->init_window = NGX_HTTP_V2_DEFAULT_WINDOW;   // 65535
```




发送SETTING帧

在h2连接建立的初始阶段，nginx会发送一个SETTING帧，

**SETTINGS_INITIAL_WINDOW_SIZE**

值为h2scf->preread_size    默认值是65535     不能大于2%31-1

表明发送方的流级别的流量控制的初始窗口大小（以字节为单位）。初始值是2^16-1(65535)字节。这个设置影响所有流的窗口大小。超过65535的窗口大小必选被视为类型是FLOW_CONTROL_ERROR的连接错误。




发送WINDOW_UPDATE帧


`Window Size Increment的大小为` 2^31-65536










### 建立流(stream)初始阶段

建立连接之后，收到HEADERS帧时，就会创建一个流(stream)



接收到HEADER帧



```cpp
stream->send_window = h2c->init_window;        // 默认65535
    stream->recv_window = h2scf->preread_size;     // 默认65535  可配置
```

流的发送窗口和接收窗口在初始化的时候，设置的值默认比较小，只有65535.


一般情况下，流建立后，客户端会给服务端发送一个SETTING帧，重新设置发送窗口大小

### 收发数据阶段

#### 接收到DATA帧


```cpp
h2c->recv_window -= h2c->state.length
    if (h2c->recv_window < NGX_HTTP_V2_MAX_WINDOW / 4) {       //连接的接收窗口大小降到窗口最大值1/4时， 即发送window_update帧

        if (ngx_http_v2_send_window_update(h2c, 0, NGX_HTTP_V2_MAX_WINDOW
                                                   - h2c->recv_window)
            == NGX_ERROR)
        {
            return ngx_http_v2_connection_error(h2c,
                                                NGX_HTTP_V2_INTERNAL_ERROR);
        }

        h2c->recv_window = NGX_HTTP_V2_MAX_WINDOW;              //发送完毕后，连接的接收窗口恢复到最大值 2^31-1
    }
    stream->recv_window -= h2c->state.length;

    if (stream->no_flow_control        // 默认是0    
        && stream->recv_window < NGX_HTTP_V2_MAX_WINDOW / 4) //当前流的接收窗口大小降到窗口最大值1/4时， 即发送window_update帧

    {
        if (ngx_http_v2_send_window_update(h2c, node->id,
                                           NGX_HTTP_V2_MAX_WINDOW
                                           - stream->recv_window)
            == NGX_ERROR)
        {
            return ngx_http_v2_connection_error(h2c,
                                                NGX_HTTP_V2_INTERNAL_ERROR);
        }

        stream->recv_window = NGX_HTTP_V2_MAX_WINDOW;        //发送完毕后，当前流的接收窗口恢复到最大值 2^31-1

    }
```








HTTP/2没有规定接收方如何决定何时发送WINDOW_UPDATE帧、发送什么样的值，具体算法依赖服务器具体实现。


nginx选择在接收窗口小于窗口最大值1/4时发送WINDOW_UPDATE帧，并且将窗口大小增长到最大值2^31-1




并不是所有服务器都这样实现的。

比如有的实现是这样的，收到一个DATA帧，马上返回一个WINDOW_UPDATE帧，增长的值就是DATA帧的大小







#### 发送DATA帧


```
h2c->send_window -= frame_size;

        stream->send_window -= frame_size;
```




### 管理帧的处理

有一些与请求数据无关的帧，对连接和流的行为和状态能够进行控制，暂且叫它管理帧。

比如SETTING WINDOW_UPDATE帧，就能够对流量窗口进行修改

#### 接收SETTING帧





```cpp
stream->send_window += value-h2c->init_window  //所有的stream都会被设置，  但是不超过NGX_HTTP_V2_MAX_WINDOW

h2c->init_window = value;      //
```







#### 接收WINDOW_UPDATE帧


```cpp
if (h2c->state.sid) {  //  判断是不是设置了sid
        node = ngx_http_v2_get_node_by_id(h2c, h2c->state.sid, 0);
...
        stream = node->stream;
...
        stream->send_window += window;
}
h2c->send_window += window;
```




# 正确使用流控

看完HTTP/2 的send_window 在各种情形下的变化情况，可以发现从理念到实现上都非常简单，对于普通用户来说，可以将这个当成透明的。





流量控制的定义是用来保护端点在资源约束条件下的操作。例如，一个代理需要在很多连接之间共享内存，也有可能有缓慢的上游连接和快速的下游连接。流量控制解决的情况是接收端在一个流上处理数据的同时同样想继续处理同个连接上的其他流。




调度过程中不需要这种能力时可以广播一个最大值的流量控制窗口，增加接收新数据时的可用空间。发送数据时总是受接收端广播的流量控制窗口的管理(见[RFC1323])。




资源约束下(例如内存)的调度可以使用流量来限制一个对等端可以消耗的内存数量。需要注意的是如果在不知道带宽延迟乘积的时候启用流量控制可能导致无法最优的利用可用的网络资源(see RFC1323)。




即便是对当前的网络延迟乘积有充分的认识，流量控制的实现也可能很复杂。当使用流量控制时，接收端必须及时地从TCP接收缓冲区读取数据。这样做可能导致在一些例如WINDOW_UPDATE的关键帧在HTTP/2不可用时导致死锁。但是流量控制可以保证约束资源能在不需要减少连接利用的情况下得到保护。















参考

http://blog.csdn.net/jianfyun/article/details/48549939

https://tools.ietf.org/html/draft-ietf-httpbis-http2-17#section-5.3

https://imququ.com/series.html

https://segmentfault.com/a/1190000002675667










