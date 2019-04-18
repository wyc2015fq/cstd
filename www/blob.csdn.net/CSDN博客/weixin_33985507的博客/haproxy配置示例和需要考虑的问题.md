# haproxy配置示例和需要考虑的问题 - weixin_33985507的博客 - CSDN博客
2018年03月10日 17:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
** HaProxy系列文章：**[http://www.cnblogs.com/f-ck-need-u/p/7576137.html](http://www.cnblogs.com/f-ck-need-u/p/7576137.html#haproxy)****
haproxy是一个非常优秀的负载均衡工具，它的特性非常丰富，功能也非常非常强大，要想好好使用它，将它的功能和性能挖掘出来，多多阅读官方手册是必不可少的。
本文提供一个简单的配置示例，后面将分别开文章详细解释它的配置文件、cookie会话保持、stick table的功能、haproxy主主模型的复制(replication)、抵御攻击等等。
# 1. 配置haproxy需要考虑的事情
尽管haproxy大多数配置选项都可以采用默认配置，但有些选项，特别是关于实际需求、连接数和超时时间相关的选项必须独立配置。
大致总结了下以下几点需要考虑的问题：
- haproxy支持5种http事务模型。一般只会选择其中两种：
- **(1).当后端为静态web或静态缓存服务器时，使用`http-keep-alive`模型，由于响应速度快，频繁建立tcp连接的代价比较大；**
- **(2).当后端为动态应用程序服务器或者静态但传输的资源对象体积较大时，使用`http-server-close`模型，因为响应速度相对较慢，占用空闲连接的资源比建立tcp连接的代价更大。**
- haproxy反向代理的调度算法优先级是低于cookie的，因此当一个连接已经保持了会话，调度算法对该连接就无效。只有新的连接请求或者长连接已经失效时，才会使用调度算法进行调度。在调度算法的选择上，如果不考虑服务器性能差距的话：
- (1).如果后端会话时间比较长(mysql)，建议使用`leastconn`，因为调度过程中，后端释放连接时动荡不大，比较稳定。
- (2).如果后端是静态web，建议使用roundrobin算法。
- (3).如果后端需要保持会话信息，但又不使用cookie时，可以使用源地址hash算法`source`，保证将同一客户端引导到同一后端服务器上。如果使用cookie，则可以使用`roundrobin`或`leastconn`算法。**源地址hash算法，一般只在没有办法的时候但又要调度到同一后端服务器时，才作为最后手段。**
- (4).如果配置了session共享，则对于haproxy来说，动态资源的请求是"无状态"的，可以使用`roundrobin`算法或`leastconn`。
- (5).如果后端是缓存服务器，为了保证命中率，建议使用`uri`算法，同时将`hash-type`设置为`consistent`方法(一致性hash)，保证后端缓存服务器down掉后对客户端的影响足够小。
- haproxy是单进程、事件驱动模型的软件，单进程下工作效率已经非常好，不建议开启的多进程/多实例。
- `maxconn`指令控制最大并发连接数，可以在多处设置，设置位置不同，代表意义不同：
- (1).设置在global段或frontend/listen/defaults段的`maxconn`代表的是和客户端(即frontend)的最大连接并发数；其中global段的值是硬限制，frontend/listen/defaults段的`maxconn`值不能超过global段的值。
- (2).设置在server指令中时，代表的是haproxy和某台后端服务器维持的最大并发连接数。
- (3).前端的最大并发数(即global段的`maxconn`)可以根据内存来估算，haproxy为每个连接维持两个缓存区，每个大致16K左右，加上一些额外数据，共约33-34K左右，因此理论上1G的空闲内存能维持2W-2.5W个纯HTTP的并发连接(只是理论上)，如果代理的是https，则允许的最大并发数量要小的多。**前端`maxconn`默认值为2000，非常有必要将其增加几倍**。一般代理纯http服务时，如果后端能处理及时，这里设置20000以上都不会有什么问题。以上只是大致估算代理能力，实际设置时必须根据后端处理能力以及haproxy自身能力设置前端`maxconn`，否则将前端接进来后端也无法立即处理。
- (4).后端所有服务器的`maxconn`值之和应接近前端的`maxconn`值，计算两者差距时，还需要考虑后端的等待队列长度`maxqueue`。其中和静态web服务器的`maxconn`可以设置大一些。
- 开启haproxy和后端的**连接重用**功能。当某客户端的请求到来后，haproxy和后端某服务器建立一个TCP连接，并将请求调度到该服务器上，该客户端后续的请求也会通过该TCP连接转发给后端(假设没有采用关闭后端连接的http事务模型)。但在响应后和该客户端的下一个请求到来前，这个连接是空闲的。和后端建立的TCP连接只是为了调度转发，免去后续再次建立tcp连接的消耗。它完全可以为其它客户端的请求调度也使用这个TCP连接，保证TCP连接资源不浪费。可以使用`http-reuse strategy_name`指令设置连接重用的策略，而默认策略禁用连接重用。
- (1).`never`：这是默认设置。表示禁用连接重用，因为老版本的haproxy认为来源不同的请求不应该共享同一个后端连接。
- (2).`safe`：这是建议使用的策略。"安全"策略下，haproxy为客户端的每个第一个请求都单独建立一个和后端的TCP连接，但是后续的请求则会重用和该后端的空闲TCP连接。这样的转发不仅提高了资源使用率，还保持了keep-alive的功能。**因此，`safe`策略配合`http-keep-alive`事务模式比`http-server-close`事务模式更高效，无论后端是静态、缓存还是动态应用服务器。**
- (3).`aggressive`：一种激进的策略，该策略的haproxy会重用空闲TCP连接来转发大多数客户端的第一次请求。之所以是大多数而不是所有，是因为haproxy会挑选那些已经被重用过至少一次的连接(即从建立开始转发过至少两次，不管源是否是同一客户端)进行重用，因为haproxy认为只有这样的连接才具有重用能力。
- (4).`always`：它将总是为第一个请求重用空闲连接。当后端是缓存服务器时，这种策略比`safe`策略的性能要高许多，因为这样的请求行为都是一样的，且可以共享同一连接来获取资源。不过不建议使用这种策略，因为大多数情况下，它和`aggressive`的性能是一样的，但是却带来了很多风险。
因此，为了性能的提升，将它设置为`safe`或`aggressive`吧，同时再将http事务模型设置为`http-keep-alive`。  
- 对于haproxy是否开启cookie以及stick table相关功能的设置必须严加考虑，它直接影响调度算法的选择和负载均衡的性能。不过如果后端应用程序服务器共享了session，haproxy可以不用设置会话粘性相关的选项。  
- haproxy的默认配置文件中关于超时时间的设置应该修改，不少项设置都很不合理。  
- 建议开启haproxy的`X-Forwarded-For`选项，使得后端服务器能够记录客户端的真实源IP地址。  
- 建议开启haproxy的状态页面，并设置访问权限。  
为了实现Haproxy完善的功能，上面几个问题是远远不够的，但可以在边使用haproxy过程中边增加功能使其不断完美。
# 2. 配置haproxy提供反向代理功能
假如要实现这样的环境：haproxy反向代理4个nginx节点，nginx1和nginx2结合php提供动态web服务，nginx3和nginx4提供静态web服务。如下图：
![](https://images2018.cnblogs.com/blog/733013/201803/733013-20180310100207113-144151624.png)
由于默认配置文件中和超时时间相关的设置比较不合理，所以建议修改这些时间。另外还有些建议开启或关闭的的项也尽量开启或关闭。
默认配置如下：
```
global
    log         127.0.0.1 local2      # 需要设置/etc/rsyslog.conf加上local2设备的日志记录级别和日志路径
    chroot      /var/lib/haproxy
    pidfile     /var/run/haproxy.pid
    maxconn     4000                  # 这是前段对外的最大连接数。代理http时，1G空闲内存承载20000以上没大问题
    user        haproxy
    group       haproxy
    daemon
    stats socket /var/lib/haproxy/stats  # 开启动态查看、管理haproxy的状态文件
                                         # 另外建议设置spread-checks全局项，且百分比建议为2-5之间
defaults
    mode                    http         # 7层http代理，另有4层tcp代理
    log                     global
    option                  httplog      # 在日志中记录http请求、session信息等
    option                  dontlognull  # 不要在日志中记录空连接
    option http-server-close             # 后端为动态应用程序建议使用http-server-close，后端为静态建议使用http-keep-alive
    option forwardfor       except 127.0.0.0/8  # haproxy将在发往后端的请求中加上"X-Forwarded-For"首部字段
    option                  redispatch   # 当某后端down掉使得haproxy无法转发携带cookie的请求到该后端时，将其转发到别的后端上
    timeout http-request    10s     # 此为等待客户端发送完整请求的最大时长，应该设置较短些防止洪水攻击，如设置为2-3秒
                                    # haproxy总是要求一次请求或响应全部发送完成后才会处理、转发，
    timeout queue           1m      # 请求在队列中的最大时长，1分钟太长了。设置为10秒都有点长，10秒请求不到资源客户端会失去耐心
    timeout connect         10s     # haproxy和服务端建立连接的最大时长，设置为1秒就足够了。局域网内建立连接一般都是瞬间的
    timeout client          1m      # 和客户端保持空闲连接的超时时长，在高并发下可稍微短一点，可设置为10秒以尽快释放连接
    timeout server          1m      # 和服务端保持空闲连接的超时时长，局域网内建立连接很快，所以尽量设置短一些，特别是并发时，如设置为1-3秒
    timeout http-keep-alive 10s     # 和客户端保持长连接的最大时长。优先级高于timeout http-request高于timeout client
    timeout check           10s     # 和后端服务器成功建立连接后到最终完成检查的时长(不包括建立连接的时间，只是读取到检查结果的时长)，
                                    # 可设置短一点，如1-2秒
    maxconn                 3000    # 默认和前段的最大连接数，但不能超过global中的maxconn硬限制数
```
所以修改后建议配置为如下：
```
global
    log         127.0.0.1 local2
    chroot      /var/lib/haproxy
    pidfile     /var/run/haproxy.pid
    maxconn     20000
    user        haproxy
    group       haproxy
    daemon
    stats socket /var/lib/haproxy/stats
    spread-checks 2
defaults
    mode                    http
    log                     global
    option                  httplog
    option                  dontlognull
    option http-server-close
    option forwardfor       except 127.0.0.0/8
    option                  redispatch
    timeout http-request    2s
    timeout queue           3s
    timeout connect         1s
    timeout client          10s
    timeout server          2s
    timeout http-keep-alive 10s
    timeout check           2s
    maxconn                 18000 
frontend http-in
    bind             *:80
    mode             http
    log              global
    capture request  header Host len 20
    capture request  header Referer len 60
    acl url_static   path_beg  -i /static /images /stylesheets
    acl url_static   path_end  -i .jpg .jpeg .gif .png .ico .bmp .css .js
    acl url_static   path_end  -i .html .htm .shtml .shtm .pdf .mp3 .mp4 .rm .rmvb .txt
    acl url_static   path_end  -i .zip .rar .gz .tgz .bz2 .tgz
    use_backend      static_group   if url_static
    default_backend  dynamic_group
backend static_group
    balance            roundrobin
    option             http-keep-alive
    http-reuse         safe
    option httpchk     GET /index.html
    http-check expect  status 200
    server staticsrv1  192.168.100.62:80 check rise 1 maxconn 5000
    server staticsrv2  192.168.100.63:80 check rise 1 maxconn 5000
backend dynamic_group
    cookie appsrv insert nocache
    balance roundrobin
    option http-server-close
    option httpchk     GET /index.php
    http-check expect  status 200
    server appsrv1 192.168.100.60:80  check rise 1 maxconn 3000 cookie appsrv1
    server appsrv2 192.168.100.61:80  check rise 1 maxconn 3000 cookie appsrv2
listen report_stats
        bind *:8081
        stats enable
        stats hide-version
        stats uri    /hastats
        stats realm  "pls enter your name"
        stats auth   admin:admin
        stats admin  if TRUE
```
上面的配置中：
- (1).静态请求将分配给static_group并进行roundrobin调度，同时通过获取index.html来做健康状况检查，此外还设置了haproxy和后端连接重用的功能。
- (2).动态请求将分配给dynamic_group并进行roundrobin调度，但是向响应报文中插入了一个cookie，保证被调度过的服务端和客户端能保持会话。此外还设置了通过获取index.php来做健康状况检查。
最后配置nginx和php+php-fpm。
```bash
yum -y install nginx php php-fpm
```
为了区分，分别为nginx1/nginx2的index.php、nginx3/nginx4的index.html文件中加入响应的主机来源提示，并在php文件中设置cookie项。其中index.php的内容参考如下：
```
<h1>response from webapp 192.168.100.60</h1>
<?php
    session_start();
    echo "Server IP: "."<font color=red>".$_SERVER['SERVER_ADDR']."</font>"."<br>";
    echo "Server Name: "."<font color=red>".$_SERVER['SERVER_NAME']."</font>"."<br>";
    echo "SESSIONNAME: "."<font color=red>".session_name()."</font>"."<br>";
    echo "SESSIONID: "."<font color=red>".session_id()."</font>"."<br>";
?>
```
测试。其中php页面返回内容大致如此：
![](https://images2018.cnblogs.com/blog/733013/201803/733013-20180310172138601-216306805.png)
