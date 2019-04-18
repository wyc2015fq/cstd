# Nginx学习之十二-负载均衡-加权轮询策略剖析 - 江南烟雨 - CSDN博客
2013年07月13日 15:52:19[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：44536
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)
本文介绍的是客户端请求在多个后端服务器之间的均衡，注意与客户端请求在多个nginx进程之间的均衡相区别（Nginx根据每个工作进程的当前压力调整它们获取监听套接口的几率，那些当前比较空闲的工作进程有更多机会获取到监听套接口，从而客户端的请求到达后也就相应地被它捕获并处理）。如果Nginx是以反向代理的形式配置运行，那么对请求的实际处理需要转发到后端服务器运行，如果后端服务器有多台，如何选择一台合适的后端服务器来处理当前请求，就是本文要说的负载均衡。这两种均衡互不冲突并且能同时生效。
nginx不单可以作为强大的web服务器，也可以作为一个反向代理服务器，而且nginx还可以按照调度规则实现动态、静态页面的分离，可以按照轮询、ip哈希、URL哈希、权重等多种方式对后端服务器做负载均衡，同时还支持后端服务器的健康检查。
如果只有一台服务器时,这个服务器挂了,那么对于网站来说是个灾难.因此，这时候的负载均衡就会大显身手了,它会自动剔除挂掉的服务器.
## 负载均衡模块简介
负载均衡模块Load-balance是辅助模块，主要为Upstream模块服务，目标明确且单一：如何从多台后端服务器中选择出一台合适的服务器来处理
当前的请求。
Load-blance模块中4个关键回调函数：
表1
|回调指针|函数功能|round_robin模块|IP_hash模块|
|----|----|----|----|
|uscf->peer.init_upstream|解析配置文件过程中调用，根据upstream里各个server配置项做初始准备工作，另外的核心工作是设置回调指针us->peer.init。配置文件解析完后不再被调用|ngx_http_upstream_init_round_robin设置：us->peer.init = ngx_http_upstream_init_round_robin_peer;|ngx_http_upstream_init_ip_hash设置：us->peer.init = ngx_http_upstream_init_ip_hash_peer;|
|us->peer.init|在每一次Nginx准备转发客户端请求到后端服务器前都会调用该函数。该函数为本次转发选择合适的后端服务器做初始准备工作，另外的核心工作是设置回调指针r->upstream->peer.get和r->upstream->peer.free等|ngx_http_upstream_init_round_robin_peer设置：r->upstream->peer.get = ngx_http_upstream_get_round_robin_peer;r->upstream->peer.free = ngx_http_upstream_free_round_robin_peer;|ngx_http_upstream_init_ip_hash_peer设置：r->upstream->peer.get = ngx_http_upstream_get_ip_hash_peer;r->upstream->peer.free为空|
|r->upstream->peer.get|在每一次Nginx准备转发客户端请求到后端服务器前都会调用该函数。该函数实现具体的位本次转发选择合适的后端服务器的算法逻辑，即完成选择获取合适后端服务器的功能|ngx_http_upstream_get_round_robin_peer加权选择当前权值最高的后端服务器|ngx_http_upstream_get_ip_hash_peer根据IP哈希值选择后端服务器|
|r->upstream->peer.free|在每一次Nginx完成与后端服务器之间的交互后都会调用该函数。|ngx_http_upstream_free_round_robin_peer更新相关数值，比如rrp->current|空|
## 负载均衡配置指令
nginx负载均衡模块ngx_http_upstream_module允许定义一组服务器，这组服务器可以被proxy_pass，fastcgi_pass和memcached_pass这些指令引用。
配置例子：
```cpp
upstream backend {
    server backend1.example.com       weight=5;
    server backend2.example.com:8080;
    server unix:/tmp/backend3;
 
    server backup1.example.com:8080   backup;
    server backup2.example.com:8080   backup;
}
 
server {
    location / {
        proxy_pass http://backend;
    }
}
```
语法：upstream name {...}
default：－
所属指令：http
定义一组用于实现nginx负载均衡的服务器，它们可以侦听在不同的端口。另外，可以混合使用侦听TCP与UNIX-domain套接字文件。
例子：
```cpp
upstream backend {
    server backend1.example.com weight=5;
    server 127.0.0.1:8080       max_fails=3 fail_timeout=30s;
    server unix:/tmp/backend3;
}
```
默认情况下，请求被分散在使用加权轮询的nginx负载均衡服务器上。在上面的例子中，每七个请求按下面分配：五个请求发送给backend1.example.com，127.0.0.1:8080和unix:/tmp/backend3各自分配一个。如果在于服务器通信时发生了一个错误，这个请求会被传递给下一个服务器，以此类推至道所有的功能服务器都尝试过。如果不能从所有的这些nginx负载均衡服务器上获得回应，客户端将会获得最后一个链接的服务器的处理结果。
语法：server 地址 [参数]；
default：－
所属指令：upstream
设置一个nginx负载均衡服务器的地址和其他参数。一个地址可以被指定为域名或IP地址，和一个可选的端口，或者被定为UNIX-domain套接字文件的路径，使用"unix:"作为前缀。如果端口没指定，则使用80端口。一个被解析到多个IP地址的域名本质上指定了多个服务器。
可以定义下面的参数：
weight=number
  设置服务器的权限，默认是1
max_fails=number
  设置在fail_timeout参数设置的时间内最大失败次数，如果在这个时间内，所有针对该服务器的请求
  都失败了，那么认为该服务器会被认为是停机了，停机时间是fail_timeout设置的时间。默认情况下，
  不成功连接数被设置为1。被设置为零则表示不进行链接数统计。那些连接被认为是不成功的可以通过
  proxy_next_upstream, fastcgi_next_upstream，和memcached_next_upstream指令配置。http_404
  状态不会被认为是不成功的尝试。
fail_time=time
  设置
    多长时间内失败次数达到最大失败次数会被认为服务器停机了
    服务器会被认为停机的时间长度
  默认情况下，超时时间被设置为10S
backup
  标记该服务器为备用服务器。当主服务器停止时，请求会被发送到它这里。
down
  标记服务器永久停机了；与指令ip_hash一起使用。
例子：
```cpp
upstream backend {
    server backend1.example.com     weight=5;
    server 127.0.0.1:8080           max_fails=3 fail_timeout=30s;
    server unix:/tmp/backend3;
 
    server backup1.example.com:8080 backup;
}
```
语法：ip_hash;
default:-
所属指令：upstream
指定nginx负载均衡器组使用基于客户端ip的负载均衡算法。IPV4的前3个八进制位和所有的IPV6地址被用作一个hash key。这个方法确保了相同客户端的请求一直发送到相同的服务器上除非这个服务器被认为是停机。这种情况下，请求会被发送到其他主机上，然后可能会一直发送到这个主机上。
  注意：在版本1.3.2中开始支持IPV6地址。
如果nginx负载均衡器组里面的一个服务器要临时移除，它应该用参数down标记，来防止之前的客户端IP还往这个服务器上发请求。
例子：
```cpp
upstream backend {
    ip_hash;
 
    server backend1.example.com;
    server backend2.example.com;
    server backend3.example.com down;
    server backend4.example.com;
}
```
 注意：在nginx版本1.3.1之前，不能在ip_hash中使用权重（weight）
语法：keepalive 连接数；
default：－
所属模块：upstream
这个指令在版本1.1.4中出现
nginx负载均衡器的活动链接数缓存。
连接数（keepalive的值）指定了每个工作进程中保留的持续连接到nginx负载均衡器缓存的最大值。如果超过这个设置值的闲置进程想链接到nginx负载均衡器组，最先连接的将被关闭。
应该注意：keepalive指令不限制nginx工作进程可以连接到nginx负载均衡器可以开启的最大公工作进程，如果
有需要的话，新进程还是会被发起。连接数应该被设置足够低来满足nginx负载均衡器处理新进的连接。
带有持续连接（keepalive connections）的memcached upstream的配置例子
```cpp
upstream memcached_backend {
    server 127.0.0.1:11211;
    server 10.0.0.2:11211;
 
    keepalive 32;
}
 
server {
    ...
 
    location /memcached/ {
        set $memcached_key $uri;
        memcached_pass memcached_backend;
    }
 
}
```
nginx负载均衡器内置变量（Embedded Variables）
nginx负载均衡模块ngx_http_upstream_module支持下列内置变量：
$upstream_addr
保存一个服务器的IP地址和端口号或者UNIX-domain套接字文件的路径。如果在处理请求过程中使用了多个服
务器，那么它们的地址将以逗号分割，例如 ：“192.168.1.1:80, 192.168.1.2:80, unix:/tmp/sock”。如果一个内置
的从一个服务器组到另一个服务器组的重定向使用X-Accel-Redirect” or error_page ，那么那些服务器组
以冒号隔开，例如“192.168.1.1:80, 192.168.1.2:80, unix:/tmp/sock : 192.168.10.1:80, 192.168.10.2:80”。
$upstream_response_time
保存nginx负载均衡服务器响应时间，以毫秒计。多个响应也以逗号和冒号隔开。
$upstream_status
保存nginx负载均衡服务器响应代码。多个响应代码也以逗号或冒号隔开。
$upstream_http_...
保存nginx负载均衡服务器响应头字段。例如，the “Server” response header field is made available through the $upstream_http_server variable.注意，只有最后一个服务器响应头字段被保存。
## 负载均衡策略
nginx的负载均衡策略可以划分为两大类：内置策略和扩展策略。内置策略包含加权轮询和ip hash，在默认情况下这两种策略会编译进nginx内核，只需在nginx配置中指明参数即可。扩展策略有很多，如fair、通用hash、consistent
 hash等，默认不编译进nginx内核，是第三方模块。
nginx 的 upstream目前支持 4 种方式的分配 ：
1)轮询（默认） 
      每个请求按时间顺序逐一分配到不同的后端服务器，如果后端服务器down掉，能自动剔除。 
2)weight 
      指定轮询几率，weight和访问比率成正比，用于后端服务器性能不均的情况。 
2)ip_hash 
      每个请求按访问ip的hash结果分配，这样每个访客固定访问一个后端服务器，可以解决session的问题。  
3)fair（第三方） 
      按后端服务器的响应时间来分配请求，响应时间短的优先分配。  
4)url_hash（第三方）
Nginx默认采用round_robin加权算法。如果要选择其他的负载均衡算法，必须在upstream的配置上下文中通过配置指令ip_hash明确指定（该配置项最好放在其他server指令等的前面，以便检查server的配置选项是否合理）。比如采用Ip_hash的upstream配置如下所示：
```cpp
upstream load_balance{
    ip_hash;
    server localhost:8001;
    server localhost:8002;
}
```
当整个http配置块被Nginx解析完毕之后，会调用各个http模块对应的初始函数。对于模块ngx_http_upstream_module而言，对应的main配置初始函数是ngx_http_upstream_init_main_conf()，在这个函数中有这样一段代码：
```cpp
for (i = 0; i < umcf->upstreams.nelts; i++) {
        init = uscfp[i]->peer.init_upstream ? uscfp[i]->peer.init_upstream:
                                            ngx_http_upstream_init_round_robin;
        if (init(cf, uscfp[i]) != NGX_OK) {
            return NGX_CONF_ERROR;
        }
}
```
默认采用加权轮询策略的原因就是在于上述代码中的init赋值一行。如果用户没有做任何策略选择，那么执行的策略初始函数为ngx_http_upstream_init_round_robin，也就是加权轮询策略。否则的话执行的是uscfp[i]->peer.init_upstream指针函数，如果有配置执行ip_hash ，那么就是ngx_http_upstream_init_ip_hash()。
## 加权轮询策略
### 全局准备工作
需要注意的是，配置文件中出现的参数只能和某些策略配合使用，所以如果发现某参数没有生效，则应该检查这一点。在配置解析的过程中，这些选项设置都被转换为Nginx内对于的变量值，对应的结构体ngx_http_upstream_server_t如下（ngx_http_upstream.h）：
```cpp
typedef struct {
    ngx_addr_t                      *addrs;//指向存储IP地址的数组的指针，host信息(对应的是 ngx_url_t->addrs )
    ngx_uint_t                       naddrs;//与第一个参数配合使用，数组元素个数(对应的是 ngx_url_t->naddrs )
    ngx_uint_t                       weight;
    ngx_uint_t                       max_fails;
    time_t                           fail_timeout;
    unsigned                         down:1;
    unsigned                         backup:1;
} ngx_http_upstream_server_t;
```
这个阶段的函数是ngx_http_upstream_init_round_robin()，其主要完成的工作详见表1.
首先是设置了一个回调指针，这个函数用来针对每个请求选择后端服务器之前做一些初始化工作：
```cpp
us->peer.init = ngx_http_upstream_init_round_robin_peer;
```
us类型是ngx_http_upstream_srv_conf_t：
```cpp
typedef struct ngx_http_upstream_srv_conf_s  ngx_http_upstream_srv_conf_t;
struct ngx_http_upstream_srv_conf_s {
    ngx_http_upstream_peer_t         peer;
    void                           **srv_conf;//在 ngx_http_upstream()函数中被设置，指向的是本层的srv_conf
    ngx_array_t                     *servers;  /*array of ngx_http_upstream_server_t */
    ngx_uint_t                       flags;//调用函数时ngx_http_upstream_add() 指定的标记
    ngx_str_t                        host;//在函数 ngx_http_upstream_add() 中设置（e.g. upstream backend中的backend）
    u_char                          *file_name;//"/usr/local/nginx/conf/nginx.conf"
    ngx_uint_t                       line;//proxy在配置文件中的行号
    in_port_t                        port;//使用的端口号（ngx_http_upstream_add()函数中添加, 指向ngx_url_t-->port，通常在函数ngx_parse_inet_url()中解析）
    in_port_t                        default_port;//默认使用的端口号（ngx_http_upstream_add()函数中添加, 指向ngx_url_t-->default_port）
    ngx_uint_t                       no_port;  /* unsigned no_port:1 */
};
```
而ngx_http_upstream_peer_t ：
```cpp
typedef struct {
    //使用负载均衡的类型，默认采用 ngx_http_upstream_init_round_robin（）
    ngx_http_upstream_init_pt        init_upstream;
    //使用的负载均衡类型的初始化函数
    ngx_http_upstream_init_peer_pt   init;
    //us->peer.data = peers; 指向的是 ngx_http_upstream_rr_peers_t（函数 ngx_http_upstream_init_round_robin()中设置）
    void                            *data;
} ngx_http_upstream_peer_t;
```
ngx_http_upstream_init_peer_pt  是函数指针类型：
```cpp
typedef ngx_int_t (*ngx_http_upstream_init_peer_pt)(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
```
服务器类型ngx_http_upstream_server_t见前面的解释。
如果upstream中服务器为空，那么默认使用proxy_pass。将利用函数ngx_inet_resolve_host依据us参数中的host和port进行解析。将结果保存在一个ngx_url_t类型的变量中：
```cpp
typedef struct {
    ngx_str_t                 url;					//保存IP地址+端口信息（e.g. 192.168.124.129:8011 或 money.163.com）
    ngx_str_t                 host;					//保存IP地址信息
    ngx_str_t                 port_text;				//保存port字符串
    ngx_str_t                 uri;					//uri部分，在函数ngx_parse_inet_url()中设置
    in_port_t                 port;					//端口，e.g. listen指令中指定的端口（listen 192.168.124.129:8011）
    in_port_t                 default_port;				//默认端口（当no_port字段为真时，将默认端口赋值给port字段， 默认端口通常是80）
    int                       family;					//address family, AF_xxx
    unsigned                  listen:1;				//是否为指监听类的设置
    unsigned                  uri_part:1;
    unsigned                  no_resolve:1;				//根据情况决定是否解析域名（将域名解析到IP地址）
    unsigned                  one_addr:1;				//等于1时，仅有一个IP地址
    unsigned                  no_port:1;				//标识url中没有显示指定端口(为1时没有指定)
    unsigned                  wildcard:1;				//标识是否使用通配符（e.g. listen *:8000;）
    socklen_t                 socklen;				//sizeof(struct sockaddr_in)
    u_char                    sockaddr[NGX_SOCKADDRLEN];		//sockaddr_in结构指向它
    ngx_addr_t               *addrs;				//数组大小是naddrs字段；每个元素对应域名的IP地址信息(struct sockaddr_in)，在函数中赋值（ngx_inet_resolve_host()）
    ngx_uint_t                naddrs;				//url对应的IP地址个数,IP格式的地址将默认为1
    char                     *err;					//错误信息字符串
} ngx_url_t;
```
此函数会创建后端服务器列表，并且将非后备服务器与后备服务器分开进行各自单独的链表。每一个后端服务器用一个结构体ngx_http_upstream_rr_peer_t与之对应（ngx_http_upstream_round_robin.h）：
```cpp
typedef struct {
    struct sockaddr                *sockaddr;//后端服务器地址
    socklen_t                       socklen;//后端服务器地址长度
    ngx_str_t                       name;//后端名称
    ngx_int_t                       current_weight;//当前权重，nginx会在运行过程中调整此权重
    ngx_int_t                       effective_weight;
    ngx_int_t                       weight;//配置的权重
    ngx_uint_t                      fails;//已尝试失败次数
    time_t                          accessed;//检测失败时间，用于计算超时
    time_t                          checked;
    ngx_uint_t                      max_fails;//最大失败次数
    time_t                          fail_timeout;//多长时间内出现max_fails次失败便认为后端down掉了
    ngx_uint_t                      down;          /* unsigned  down:1; *///指定某后端是否挂了
#if (NGX_HTTP_SSL)
    ngx_ssl_session_t              *ssl_session;   /* local to a process */
#endif
} ngx_http_upstream_rr_peer_t;
```
列表最前面需要带有一些head信息，用结构体ngx_http_upstream_rr_peers_t与之对应：
```cpp
typedef struct ngx_http_upstream_rr_peers_s  ngx_http_upstream_rr_peers_t;
struct ngx_http_upstream_rr_peers_s {
    ngx_uint_t                      number;//队列中服务器数量
 /* ngx_mutex_t                    *mutex; */
    ngx_uint_t                      total_weight;//所有服务器总权重
    unsigned                        single:1;//为1表示后端服务器总共只有一台，用于优化，此时不需要再做选择
    unsigned                        weighted:1;//为1表示总的权重值等于服务器数量
    ngx_str_t                      *name;
    ngx_http_upstream_rr_peers_t   *next;//后备服务器列表挂载在这个字段下
    ngx_http_upstream_rr_peer_t     peer[1];
};
```
函数的完整代码如下（ngx_http_upstream_round_robin.c）：
```cpp
//函数：初始化服务器负载均衡表	
//参数：
//us：ngx_http_upstream_main_conf_t结构体中upstreams数组元素
ngx_int_t
ngx_http_upstream_init_round_robin(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us)
{
    ngx_url_t                      u;
    ngx_uint_t                     i, j, n, w;
    ngx_http_upstream_server_t    *server;
    ngx_http_upstream_rr_peers_t  *peers, *backup;
    //回调指针设置
    us->peer.init = ngx_http_upstream_init_round_robin_peer;
    //服务器数组指针不为空
    if (us->servers) {
        server = us->servers->elts;
        n = 0;
        w = 0;
	//遍历所有服务器
        for (i = 0; i < us->servers->nelts; i++) {
	    //是后备服务器，跳过
            if (server[i].backup) {
                continue;
            }
	    //服务器地址数量统计
            n += server[i].naddrs;
	    //总的权重计算
            w += server[i].naddrs * server[i].weight;
        }
        if (n == 0) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                          "no servers in upstream \"%V\" in %s:%ui",
                          &us->host, us->file_name, us->line);
            return NGX_ERROR;
        }
	//为非后备服务器分配空间
        peers = ngx_pcalloc(cf->pool, sizeof(ngx_http_upstream_rr_peers_t)
                              + sizeof(ngx_http_upstream_rr_peer_t) * (n - 1));
        if (peers == NULL) {
            return NGX_ERROR;
        }
	//非后备服务器列表头中各属性设置
        peers->single = (n == 1);
        peers->number = n;
        peers->weighted = (w != n);
        peers->total_weight = w;
        peers->name = &us->host;
        n = 0;
	//后备服务器列表中各服务器项设置
        for (i = 0; i < us->servers->nelts; i++) {
            for (j = 0; j < server[i].naddrs; j++) {
                if (server[i].backup) {
                    continue;
                }
                peers->peer[n].sockaddr = server[i].addrs[j].sockaddr;
                peers->peer[n].socklen = server[i].addrs[j].socklen;
                peers->peer[n].name = server[i].addrs[j].name;
                peers->peer[n].max_fails = server[i].max_fails;
                peers->peer[n].fail_timeout = server[i].fail_timeout;
                peers->peer[n].down = server[i].down;
                peers->peer[n].weight = server[i].weight;
                peers->peer[n].effective_weight = server[i].weight;
                peers->peer[n].current_weight = 0;
                n++;
            }
        }
	//非后备服务器列表挂载的位置
        us->peer.data = peers;
        /* backup servers */
	//后备服务器
        n = 0;
        w = 0;
        for (i = 0; i < us->servers->nelts; i++) {
            if (!server[i].backup) {
                continue;
            }
	    //后备服务器地址数量统计
            n += server[i].naddrs;
	    //后备服务器总权重计算
            w += server[i].naddrs * server[i].weight;
        }
        if (n == 0) {
            return NGX_OK;
        }
	//后备服务器列表地址空间分配
        backup = ngx_pcalloc(cf->pool, sizeof(ngx_http_upstream_rr_peers_t)
                              + sizeof(ngx_http_upstream_rr_peer_t) * (n - 1));
        if (backup == NULL) {
            return NGX_ERROR;
        }
        peers->single = 0;
	//后备服务器列表头中各属性设置
        backup->single = 0;
        backup->number = n;
        backup->weighted = (w != n);
        backup->total_weight = w;
        backup->name = &us->host;
        n = 0;
	//后备服务器列表中各服务器项设置
        for (i = 0; i < us->servers->nelts; i++) {
            for (j = 0; j < server[i].naddrs; j++) {
                if (!server[i].backup) {
                    continue;
                }
                backup->peer[n].sockaddr = server[i].addrs[j].sockaddr;
                backup->peer[n].socklen = server[i].addrs[j].socklen;
                backup->peer[n].name = server[i].addrs[j].name;
                backup->peer[n].weight = server[i].weight;
                backup->peer[n].effective_weight = server[i].weight;
                backup->peer[n].current_weight = 0;
                backup->peer[n].max_fails = server[i].max_fails;
                backup->peer[n].fail_timeout = server[i].fail_timeout;
                backup->peer[n].down = server[i].down;
                n++;
            }
        }
	//后备服务器挂载
        peers->next = backup;
        return NGX_OK;
    }
    //us参数中服务器指针为空，例如用户直接在proxy_pass等指令后配置后端服务器地址
    /* an upstream implicitly defined by proxy_pass, etc. */
    if (us->port == 0) {
        ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                      "no port in upstream \"%V\" in %s:%ui",
                      &us->host, us->file_name, us->line);
        return NGX_ERROR;
    }
    ngx_memzero(&u, sizeof(ngx_url_t));
    u.host = us->host;
    u.port = us->port;
    //IP地址解析
    if (ngx_inet_resolve_host(cf->pool, &u) != NGX_OK) {
        if (u.err) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                          "%s in upstream \"%V\" in %s:%ui",
                          u.err, &us->host, us->file_name, us->line);
        }
        return NGX_ERROR;
    }
    n = u.naddrs;
    peers = ngx_pcalloc(cf->pool, sizeof(ngx_http_upstream_rr_peers_t)
                              + sizeof(ngx_http_upstream_rr_peer_t) * (n - 1));
    if (peers == NULL) {
        return NGX_ERROR;
    }
    peers->single = (n == 1);
    peers->number = n;
    peers->weighted = 0;
    peers->total_weight = n;
    peers->name = &us->host;
    for (i = 0; i < u.naddrs; i++) {
        peers->peer[i].sockaddr = u.addrs[i].sockaddr;
        peers->peer[i].socklen = u.addrs[i].socklen;
        peers->peer[i].name = u.addrs[i].name;
        peers->peer[i].weight = 1;
        peers->peer[i].effective_weight = 1;
        peers->peer[i].current_weight = 0;
        peers->peer[i].max_fails = 1;
        peers->peer[i].fail_timeout = 10;
    }
    us->peer.data = peers;
    /* implicitly defined upstream has no backup servers */
    return NGX_OK;
}
```
### 选择后端服务器
针对一个客户端请求的初始化工作
全局初始化完成之后，当一个客户端请求过来时，Nginx就要选择合适的后端服务器来处理该请求。在正式开始选择前，Nginx还要单独为本轮选择做一些初始化（针对一个客户端请求，nginx会进行多次尝试选择，尝试全部失败后才返回502错误，所以注意一轮选择与一次选择的区别）。
在前面的函数ngx_http_upstream_init_round_robin()中设置的回调函数us->peer.init，它的调用位置是函数ngx_http_upstream_init_request中（ngx_http_upstream.c）：
```cpp
static void
ngx_http_upstream_init_request(ngx_http_request_t *r)
{
...
if (uscf->peer.init(r, uscf) != NGX_OK) {
        ngx_http_upstream_finalize_request(r, u,
                                           NGX_HTTP_INTERNAL_SERVER_ERROR);
        return;
    }
    ngx_http_upstream_connect(r, u);
}
```
即在针对每个请求选择后端服务器之前被调用。
下面看看函数ngx_http_upstream_init_round_robin_peer()完成了哪些工作。
它除了完成初始化工作之外，另外的核心工作是设置回调指针。
函数ngx_http_upstream_init_round_robin_peer的完整代码（ngx_http_upstream_round_robin.c）：
```cpp
//函数：
//功能：针对每个请求选择后端服务器前做一些初始化工作
ngx_int_t
ngx_http_upstream_init_round_robin_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us)
{
    ngx_uint_t                         n;
    ngx_http_upstream_rr_peer_data_t  *rrp;
    rrp = r->upstream->peer.data;
    if (rrp == NULL) {
        rrp = ngx_palloc(r->pool, sizeof(ngx_http_upstream_rr_peer_data_t));
        if (rrp == NULL) {
            return NGX_ERROR;
        }
        r->upstream->peer.data = rrp;
    }
    rrp->peers = us->peer.data;
    rrp->current = 0;
    //n取值为：非后备服务器和后备服务器列表中个数较大的那个值
    n = rrp->peers->number;
    if (rrp->peers->next && rrp->peers->next->number > n) {
        n = rrp->peers->next->number;
    }
    //如果n小于一个指针变量所能表示的范围
    if (n <= 8 * sizeof(uintptr_t)) {
	//直接使用已有的指针类型的data变量做位图（tried是位图，用来标识在一轮选择中，各个后端服务器是否已经被选择过）
        rrp->tried = &rrp->data;
        rrp->data = 0;
    } else {
	//否则从内存池中申请空间
        n = (n + (8 * sizeof(uintptr_t) - 1)) / (8 * sizeof(uintptr_t));
        rrp->tried = ngx_pcalloc(r->pool, n * sizeof(uintptr_t));
        if (rrp->tried == NULL) {
            return NGX_ERROR;
        }
    }
    //回调函数设置
    r->upstream->peer.get = ngx_http_upstream_get_round_robin_peer;
    r->upstream->peer.free = ngx_http_upstream_free_round_robin_peer;
    r->upstream->peer.tries = rrp->peers->number;
#if (NGX_HTTP_SSL)
    r->upstream->peer.set_session =
                               ngx_http_upstream_set_round_robin_peer_session;
    r->upstream->peer.save_session =
                               ngx_http_upstream_save_round_robin_peer_session;
#endif
    return NGX_OK;
}
```
对后端服务器进行一次选择
对后端服务器做一次选择的逻辑在函数ngx_http_upstream_get_round_robin_peer内，流程图如下：
![](https://img-blog.csdn.net/20130713154517328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
代码如下：
```cpp
//函数：
//功能：对后端服务器做一次选择
ngx_int_t
ngx_http_upstream_get_round_robin_peer(ngx_peer_connection_t *pc, void *data)
{
    ngx_http_upstream_rr_peer_data_t  *rrp = data;
    ngx_int_t                      rc;
    ngx_uint_t                     i, n;
    ngx_http_upstream_rr_peer_t   *peer;
    ngx_http_upstream_rr_peers_t  *peers;
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                   "get rr peer, try: %ui", pc->tries);
    /* ngx_lock_mutex(rrp->peers->mutex); */
    pc->cached = 0;
    pc->connection = NULL;
    //如果只有一台后端服务器，Nginx直接选择并返回
    if (rrp->peers->single) {
        peer = &rrp->peers->peer[0];
        if (peer->down) {
            goto failed;
        }
    } else {
	//有多台后端服务器
        /* there are several peers */
	//按照各台服务器的当前权值进行选择
        peer = ngx_http_upstream_get_peer(rrp);
        if (peer == NULL) {
            goto failed;
        }
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                       "get rr peer, current: %ui %i",
                       rrp->current, peer->current_weight);
    }
    //设置连接的相关属性
    pc->sockaddr = peer->sockaddr;
    pc->socklen = peer->socklen;
    pc->name = &peer->name;
    /* ngx_unlock_mutex(rrp->peers->mutex); */
    if (pc->tries == 1 && rrp->peers->next) {
        pc->tries += rrp->peers->next->number;
    }
    return NGX_OK;
    //选择失败，转向后备服务器
failed:
    peers = rrp->peers;
    if (peers->next) {
        /* ngx_unlock_mutex(peers->mutex); */
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, pc->log, 0, "backup servers");
        rrp->peers = peers->next;
        pc->tries = rrp->peers->number;
        n = (rrp->peers->number + (8 * sizeof(uintptr_t) - 1))
                / (8 * sizeof(uintptr_t));
        for (i = 0; i < n; i++) {
             rrp->tried[i] = 0;
        }
        rc = ngx_http_upstream_get_round_robin_peer(pc, rrp);
        if (rc != NGX_BUSY) {
            return rc;
        }
        /* ngx_lock_mutex(peers->mutex); */
    }
    /* all peers failed, mark them as live for quick recovery */
    for (i = 0; i < peers->number; i++) {
        peers->peer[i].fails = 0;
    }
    /* ngx_unlock_mutex(peers->mutex); */
    pc->name = peers->name;
    //如果后备服务器也选择失败，则返回NGX_BUSY
    return NGX_BUSY;
}
```
后端服务器权值计算在函数ngx_http_upstream_get_peer中。
```cpp
//按照当前各服务器权值进行选择
static ngx_http_upstream_rr_peer_t *
ngx_http_upstream_get_peer(ngx_http_upstream_rr_peer_data_t *rrp)
{
    time_t                        now;
    uintptr_t                     m;
    ngx_int_t                     total;
    ngx_uint_t                    i, n;
    ngx_http_upstream_rr_peer_t  *peer, *best;
    now = ngx_time();
    best = NULL;
    total = 0;
    for (i = 0; i < rrp->peers->number; i++) {
	//计算当前服务器的标记位在位图中的位置
        n = i / (8 * sizeof(uintptr_t));
        m = (uintptr_t) 1 << i % (8 * sizeof(uintptr_t));
	//已经选择过，跳过
        if (rrp->tried[n] & m) {
            continue;
        }
	//当前服务器对象
        peer = &rrp->peers->peer[i];
	//当前服务器已宕机，排除
        if (peer->down) {
            continue;
        }
	//根据指定一段时间内最大失败次数做判断
        if (peer->max_fails
            && peer->fails >= peer->max_fails
            && now - peer->checked <= peer->fail_timeout)
        {
            continue;
        }
        peer->current_weight += peer->effective_weight;
        total += peer->effective_weight;
        if (peer->effective_weight < peer->weight) {
            peer->effective_weight++;
        }
        if (best == NULL || peer->current_weight > best->current_weight) {
            best = peer;
        }
    }
    if (best == NULL) {
        return NULL;
    }
    //所选择的服务器在服务器列表中的位置
    i = best - &rrp->peers->peer[0];
    rrp->current = i;
    n = i / (8 * sizeof(uintptr_t));
    m = (uintptr_t) 1 << i % (8 * sizeof(uintptr_t));
    //位图相应位置置位
    rrp->tried[n] |= m;
    best->current_weight -= total;
    best->checked = now;
    return best;
}
```
要理解这个函数的工作原理，先要区分下表示服务的ngx_http_upstream_rr_peer_t结构体中的一下三个成员变量：
```cpp
ngx_int_t                       current_weight;
    ngx_int_t                       effective_weight;
    ngx_int_t                       weight;
```
网上搜索到的解释：
effective_weight相当于质量(来源于配置的weight)，current_weight相当于重量。前者反应本质，一般是不变的。current_weight是运行时的动态权值，它的变化基于effective_weight。但是effective_weight在其对应的peer服务异常时，会被调低，当服务恢复正常时，effective_weight会逐渐恢复到实际值（配置的weight）。
下面我们结合具体的代码来看。
它们在函数ngx_http_upstream_init_round_robin中被初始化：
```cpp
for (i = 0; i < us->servers->nelts; i++) {
            for (j = 0; j < server[i].naddrs; j++) {
                if (server[i].backup) {
                    continue;
                }
                peers->peer[n].weight = server[i].weight;
                peers->peer[n].effective_weight = server[i].weight;
                peers->peer[n].current_weight = 0;
                n++;
            }
        }
        /* backup servers */
        for (i = 0; i < us->servers->nelts; i++) {
            for (j = 0; j < server[i].naddrs; j++) {
                if (!server[i].backup) {
                    continue;
                }
                backup->peer[n].weight = server[i].weight;
                backup->peer[n].effective_weight = server[i].weight;
                backup->peer[n].current_weight = 0;
                n++;
            }
        }
     /* an upstream implicitly defined by proxy_pass, etc. */
    for (i = 0; i < u.naddrs; i++) {
        peers->peer[i].weight = 1;
        peers->peer[i].effective_weight = 1;
        peers->peer[i].current_weight = 0;
    }
```
可以看到weight、effective_weight都是初始化为配置项中的weight值。current_weight初始化为0.
下面分析这三个变量在负载均衡过程中的变化。
weight的值在整个运行过程中不发生变化。
total变量记录了针对一个服务列表的一次轮询过程中轮询到的所有服务的effective_weight总和。在每一次针对服务列表的轮询之前会置为为0.
遍历服务列表的过程中，每遍历到一个服务，会在该服务的current_weight上加上其对应的effective_weight。这个是累加。如果对统一的服务列表进行另一次轮询，那么会在前面计算的current_weight的基础之上再加上effective_weight。
轮询策略是取current_weight最大的服务器。每次取到后端服务（用best表示）后，都会把该对象peer的current_weight减去total的值。因为该服务刚被选中过，因此要降低权值。
关于effective_weight的变化，有两处，一个是在函数ngx_http_upstream_get_peer中：
```cpp
//服务正常，effective_weight 逐渐恢复正常    
        if (peer->effective_weight < peer->weight) {
            peer->effective_weight++;
        }
```
另一处是在释放后端服务的函数ngx_http_upstream_free_round_robin_peer中：
```cpp
if (peer->max_fails) {
             //服务发生异常时，调低effective_weight
            peer->effective_weight -= peer->weight / peer->max_fails;
        }
```
权重高的会优先被选中，而且被选中的频率也更高。权重低的也会由于权重逐渐增长获得被选中的机会。
下面给出一个加权轮询的选择实例（来自[http://blog.sina.com.cn/s/blog_7303a1dc01014i0j.html](http://blog.sina.com.cn/s/blog_7303a1dc01014i0j.html)）：
|selected server|current_weight beforeselected|current_weight afterselected|
|----|----|----|
|a|{ 5, 1, 2 }|{ -3, 1, 2 }|
|c|{ 2, 2, 4 }|{ 2, 2, -4 }|
|a|{ 7, 3, -2 }|{ -1, 3, -2 }|
|a|{ 4, 4, 0 }|{ -4, 4, 0 }|
|b|{ 1, 5, 2 }|{ 1, -3, 2 }|
|a|{ 6, -2, 4 }|{ -2, -2, 4 }|
|c|{ 3, -1, 6 }|{ 3, -1, -2 }|
|a|{ 8, 0, 0 }|{ 0, 0, 0 }|
### 释放后端服务器
连接后端服务器并且正常处理当前客户端请求后需释放后端服务器。如果在某一轮选择里，某次选择的服务器因连接失败或请求处理失败而需要重新进行选择，那么这时候就需要做一些额外的处理。
```cpp
//函数：
//功能：释放后端服务器
void
ngx_http_upstream_free_round_robin_peer(ngx_peer_connection_t *pc, void *data,
    ngx_uint_t state)
{
    ngx_http_upstream_rr_peer_data_t  *rrp = data;
    time_t                       now;
    ngx_http_upstream_rr_peer_t  *peer;
    ngx_log_debug2(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                   "free rr peer %ui %ui", pc->tries, state);
    /* TODO: NGX_PEER_KEEPALIVE */
    //后端服务只有一个
    if (rrp->peers->single) {
        pc->tries = 0;
        return;
    }
    peer = &rrp->peers->peer[rrp->current];
    //在某一轮选择里，某次选择的服务器因连接失败或请求处理失败而需要重新进行选择
    if (state & NGX_PEER_FAILED) {
        now = ngx_time();
        /* ngx_lock_mutex(rrp->peers->mutex); */
	//已尝试失败次数加一
        peer->fails++;
        peer->accessed = now;
        peer->checked = now;
	//如果有最大失败次数限制
        if (peer->max_fails) {
	    //服务发生异常时，调低effective_weight
            peer->effective_weight -= peer->weight / peer->max_fails;
        }
        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                       "free rr peer failed: %ui %i",
                       rrp->current, peer->effective_weight);
	//effective_weight总大于0
        if (peer->effective_weight < 0) {
            peer->effective_weight = 0;
        }
        /* ngx_unlock_mutex(rrp->peers->mutex); */
    } else {
        /* mark peer live if check passed */
        if (peer->accessed < peer->checked) {
            peer->fails = 0;
        }
    }
    //ngx_peer_connection_t结构体中tries字段：
    //表示在连接一个远端服务器时，当前连接出现异常失败后可以重试的次数，也就是允许失败的次数
    if (pc->tries) {
        pc->tries--;
    }
    /* ngx_unlock_mutex(rrp->peers->mutex); */
}
```
### 整个加权轮询的流程
整个加权轮询的流程图如下：
![](https://img-blog.csdn.net/20130713154933343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
说明：
首先是全局初始化，由函数ngx_http_upstream_init_round_robin完成，它在函数ngx_http_upstream_init_main_conf中被调用，代码：
```cpp
static char *
ngx_http_upstream_init_main_conf(ngx_conf_t *cf, void *conf)
{    
...
    for (i = 0; i < umcf->upstreams.nelts; i++) {
        //全局初始化
        init = uscfp[i]->peer.init_upstream ? uscfp[i]->peer.init_upstream:
                                            ngx_http_upstream_init_round_robin;
        if (init(cf, uscfp[i]) != NGX_OK) {
            return NGX_CONF_ERROR;
        }
    }
...
}
```
收到客户请求之后，针对当前请求进行初始化，完成此功能的函数是ngx_http_upstream_init_round_robin_peer，它在函数ngx_http_upstream_init_request中被调用：
```cpp
static void
ngx_http_upstream_init_request(ngx_http_request_t *r)
{
...
if (uscf->peer.init(r, uscf) != NGX_OK) {
        ngx_http_upstream_finalize_request(r, u,
                                           NGX_HTTP_INTERNAL_SERVER_ERROR);
        return;
    }
    ngx_http_upstream_connect(r, u);
}
```
然后是针对每个请求选择后端服务器，实现此功能的函数是ngx_http_upstream_get_round_robin_peer。它在函数ngx_event_connect_peer中被调用：
```cpp
//函数：连接后端upstream
ngx_int_t
ngx_event_connect_peer(ngx_peer_connection_t *pc)
{
...
    //此处调用选择后端服务器功能函数ngx_http_upstream_get_round_robin_peer
    rc = pc->get(pc, pc->data);
    if (rc != NGX_OK) {
        return rc;
    }
    s = ngx_socket(pc->sockaddr->sa_family, SOCK_STREAM, 0);
...
}
```
之后是测试连接ngx_http_upstream_test_connect。它在函数ngx_http_upstream_send_request被调用：
```cpp
//函数：发送数据到后端upstream
static void
ngx_http_upstream_send_request(ngx_http_request_t *r, ngx_http_upstream_t *u)
{
...
    if (!u->request_sent && ngx_http_upstream_test_connect(c) != NGX_OK) {
        //测试连接失败
        ngx_http_upstream_next(r, u, NGX_HTTP_UPSTREAM_FT_ERROR);
        return;
    }
...
}
```
如果测试成功，继续后续处理，并释放后端服务器。
如果测试失败，调用ngx_http_upstream_next函数，这个函数可能再次调用peer.get调用别的连接。
```cpp
static void
ngx_http_upstream_next(ngx_http_request_t *r, ngx_http_upstream_t *u,
    ngx_uint_t ft_type)
{
...
    if (u->peer.sockaddr) {
        if (ft_type == NGX_HTTP_UPSTREAM_FT_HTTP_404) {
            state = NGX_PEER_NEXT;
        } else {
            state = NGX_PEER_FAILED;
        }
        //释放后端服务器
        u->peer.free(&u->peer, u->peer.data, state);
        u->peer.sockaddr = NULL;
    }
...
if (status) {
        u->state->status = status;
        if (u->peer.tries == 0 || !(u->conf->next_upstream & ft_type)) {
#if (NGX_HTTP_CACHE)
            if (u->cache_status == NGX_HTTP_CACHE_EXPIRED
                && (u->conf->cache_use_stale & ft_type))
            {
                ngx_int_t  rc;
                rc = u->reinit_request(r);
                if (rc == NGX_OK) {
                    u->cache_status = NGX_HTTP_CACHE_STALE;
                    rc = ngx_http_upstream_cache_send(r, u);
                }
                ngx_http_upstream_finalize_request(r, u, rc);
                return;
            }
#endif
            //结束请求
            ngx_http_upstream_finalize_request(r, u, status);
            return;
        }
    }
...
//再次发起连接
ngx_http_upstream_connect(r, u);
}
```
函数ngx_http_upstream_connect中会调用ngx_event_connect_peer，进而调用ngx_http_upstream_get_round_robin_peer再次选择后端服务器。
转载请注明出处：[http://blog.csdn.net/xiajun07061225/article/details/9318871](http://blog.csdn.net/xiajun07061225/article/details/9318871)
## 参考资料：
[http://www.cnblogs.com/xiaogangqq123/archive/2011/03/04/1971002.html](http://www.cnblogs.com/xiaogangqq123/archive/2011/03/04/1971002.html)
[http://tengine.taobao.org/book/chapter_05.html#id5](http://tengine.taobao.org/book/chapter_05.html#id5)
[http://blog.dccmx.com/2011/07/nginx-upsream-src-2/](http://blog.dccmx.com/2011/07/nginx-upsream-src-2/)
[http://nginx-source-analysis.googlecode.com/svn-history/r151/trunk/src/http/ngx_http_upstream.h](http://nginx-source-analysis.googlecode.com/svn-history/r151/trunk/src/http/ngx_http_upstream.h)
[http://www.inginx.org/thread-89-1-1.html](http://www.inginx.org/thread-89-1-1.html)
[http://blog.sina.com.cn/s/blog_7303a1dc01014i0j.html](http://blog.sina.com.cn/s/blog_7303a1dc01014i0j.html)
[https://github.com/phusion/nginx/commit/27e94984486058d73157038f7950a0a36ecc6e35](https://github.com/phusion/nginx/commit/27e94984486058d73157038f7950a0a36ecc6e35)
[http://www.pagefault.info/?p=251](http://www.pagefault.info/?p=251)
[http://www.pagefault.info/?p=259](http://www.pagefault.info/?p=259)

