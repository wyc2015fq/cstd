# haproxy小结（一）基础概念篇 - DoubleLi - 博客园






HAProxy是法国人Willy Tarreau个人开发的一个开源软件，目标是应对客户端10000以上的同时连接，为后端应用服务器、数据库服务器提供高性能的负载均衡服务。HAproxy可以实现基于TCP（四层 例如:SSH,SMTP,MYSQL）和HTTP（七层 例如:web服务器）应用的代理软件，同时也可以作为负载均衡器使用，并且是开源完全免费的。HAproxy完全可以支持数以万计的并发链接，它的工作模式可以将其简单而安全地整合到当前的服务架构中，同时可以保护你的WEB服务器不暴露到网络上（设置成代理来实现的 通过VIP将后端的web服务器隐藏到内网中）。



**一、HAProxy优点**

1、免费开源，稳定性也非常好，其稳定性可以与硬件级别的F5 BIG-IP相媲美。

2、负载带宽非常大。根据官方文档可知，HAproxy可以跑满10Gbps的带宽，对于软件级负载均衡器来说，是相当惊人的。

3、支持链接拒绝。因为保护一个链接保持打开状态的开销是很低的，有时我们需要防止蠕虫攻击，也就是通过限制它们的连接打开来防止它们的危害，可以用于防止DDoS攻击，这也是其他负载均衡器所不具备的。

4、支持全透明代理（已具备硬件防火墙的典型特点）。可以用客户端IP地址或任何其他地址来链接后端服务器，这个特性仅在Linux 2.4/2.6 内核打了cttproxy补丁后才可以使用。

5、支持TCP层（四层）的负载均衡。HAproxy现在多用于线上的MySQL集群环境，常用它作为MySQL（读）负载均衡，不过在后端的MySQL Slaves数量超过10台时性能不如LVS，所以更推荐推荐LVS+Keepalived。一般情况下都是前端通过LVS做四层负载，HAProxy做后端web服务器的负载 这样性能会比单独用HAProxy性能高很多。

6、强大的监控服务。自带强大的监控服务器状态的页面，在生产环境中可结合Nagios来实现邮件或短信报警。

7、能支持多种的负载均衡算法，现在为止可以支持8种

8、支持虚拟主机。

－－－page from www.361way.com(运维之路，页面被扒，打个tag)

### 二、HAProxy是什么不是什么

#### 1、HAProxy是什么



HAProxy 是：
- 
TCP 代理：可从监听 socket 接受 TCP 连接，然后自己连接到 server，HAProxy 将这些 sockets attach 到一起，使通信流量可双向流动。

- 
HTTP 反向代理（在 HTTP 专用术语中，称为 gateway）：HAProxy 自身表现得就像一个 server，通过监听 socket 接受 HTTP 请求，然后与后端服务器建立连接，通过连接将请求转发给后端服务器。

- 
SSL terminator / initiator / offloader: 客户端 -> HAProxy 的连接，以及 HAProxy -> server 端的连接都可以使用 SSL/TLS

- 
TCP normalizer: 因为连接在本地操作系统处终结，client 和 server 端没有关联，所以不正常的 traffic 如 invalid packets, flag combinations, window advertisements, sequence numbers, incomplete connections(SYN floods) 不会传递给 server 端。这种机制可以保护脆弱的TCP stacks 免遭协议上的攻击，也使得我们不必修改 server 端的 TCP 协议栈设置就可以优化与 client 的连接参数。

- 
HTTP normalizer: HAProxy 配置为 HTTP 模式时，只允许有效的完整的请求转发给后端。这样可以使得后端免遭 protocol-based 攻击。一些不规范的定义也被修改，以免在 server 端造成问题（eg: multiple-line headers，会被合并为一行）

- 
HTTP 修正工具：HAProxy 可以 modify / fix / add / remove / rewrite URL 及任何 request or response header。

- 
a content-based switch: 可基于内容进行转发。可基于请求中的任何元素转发请求或连接。因此可基于一个端口处理多种协议（http,https, ssh）

- 
a server load balancer: 可对 TCP 连接 和 HTTP 请求进行负载均衡调度。工作于 TCP 模式时，可对整个连接进行负载均衡调度；工作于 HTTP 模式时，可对 HTTP 请求进行调度。

- 
a traffic regulator: 可在不同的方面对流量进行限制，保护 server ，使其不超负荷，基于内容调整 traffic 优先级，甚至可以通过 marking packets 将这些信息传递给下层以及网络组件。

- 
防御 DDos 攻击及 service abuse: HAProxy 可为每个 IP地址，URL，cookie 等维护大量的统计信息，并对其进行检测，当发生服务滥用的情况，采取一定的措施如：slow down the offenders, block them, send them to outdated contents, etc

- 
是 network 的诊断的一个观察节点：根据精确记录细节丰富的日志，对网络诊断很有帮助

- 
an HTTP compression offloader：可自行对响应进行压缩，而不是让 server 进行压缩，因此对于连接性能较差的 client，或使用高延迟移动网络的 client，可减少页面加载时间。


#### 2、HAProxy不是什么

HAProxy 不是：
- 
显式地 HTTP 代理，即不是浏览器用于访问 internet 的代理。Squid 可用于做这种事情。HAProxy 可安装在 Squid 的前端，提供负载均衡和高可用。

- 
缓存代理：对于从服务器收到的内容，HAProxy 只能将其原样返回给 client，对 caching policy 不做任何干涉。对于这个任务， Varnish 是一个很好的选择。HAProxy 可安装在 Varnish 的前端提供 SSL offloading，以及可伸缩性（通过 smart load balancing）

- 
a data scrubber: HAProxy 不能修改请求或响应的 body 部分

- 
a web server: HAProxy 在启动时，它将自己隔离在一个 chroot 环境中，并抛弃了自己的特权，所以一旦启动之后，它不会进行任何对文件系统的访问。所以 HAProxy 不能以 web server 的方式工作。可作 web server 的是 Apache 或 Nginx，HAProxy 可安装在其前端提供提供负载均衡和高可用。

- 
a packet-based load balancer：基于 packet 的负载均衡调度器。HAProxy 不会看见 IP packet 或 UDP datagram，不能进行 NAT 转换，或者 less DSR。这些是下层的工作，比如 LVS 就做得很好，LVS 对 HAProxy 是完美的补充。




### 三、HAProxy是如何工作的





HAProxy 由单线程的、事件驱动的、非阻塞的引擎， 以及一个非常快速的 I/O 层组合而成。拥有 “基于优先级” 的调度机制。

HAProxy 的设计理念是让数据尽可能快的移动。它在每一层设计了 bypass 机制，在不必要的时候，数据不会传递到上层，直接在下层进行传递。大部分的处理都在 kernel 中完成，HAProxy 尽最大努力帮助 kernel 尽快完成工作，包括给一些 hints，或是当某些操作可稍后合并完成时，避免某些操作。

当 HAProxy 工作在 TCP 或者 HTTP close mode 时，HAProxy 消耗的 CPU 负载是 15%，内核消耗的负载是 85%。

当 HAProxy 工作在 HTTP keep-alive mode 时，HAProxy 消耗的 CPU 负载是 30%，内核消耗的负载是 70%。

一个 HAProxy 进程可以运行许多 proxy 实例；一个进程可容纳 300000 万个实例而运行良好。因此一般不需要启动多个进程。

HAProxy 被用来做 SSL offloader 时，可使用多进程。

运行 HAProxy 只需要一个 haproxy 可执行程序和一份配置文件即可运行。要记录日志，强烈建议配置好 syslog daemon 及 rotation 机制。配置文件在启动前被解析，HAProxy 尝试绑定所有的 监听 sockets，如果有地方失败就不会启动。一旦成功启动，HAProxy 不会再失效，也就是说不会有 runtime failures，一旦启动，它会工作到被关闭为止。

HAProxy 一旦启动，会做三件事情：
- 处理客户端接入的连接
- 周期性检查 server 的状态（健康检查）
- 与其他 haproxy 交换信息

处理客户端接入的连接，是目前为止最为复杂的工作，因为配置有太多的可能性，但总的说来有 9 个步骤：
- 
配置实体 frontend 拥有监听 socket，HAProxy 从它的监听 socket 处接受客户端连接

- 
根据 frontend 配置的规则，对连接进行处理。可能会拒绝一些连接，修改一些 headers，或是拦截连接，执行内部的小程序，比如统计页面，或者 CLI

- 
backend 是定义后端 servers，以及负载均衡规则的配置实体，frontend 完成上面的处理后将连接转发给 backend。

- 
根据 backend 定义的规则，对连接进行处理

- 
根据负载均衡规则对连接进行调度

- 
根据 backend 定义的规则对 response data 进行处理

- 
根据 frontend 定义的规则对 response data 进行处理

- 
发起一个 log report，记录日志

- 
在 HTTP 模式，回到第二步，等待新的请求，或者关闭连接。


frontend 和 backend 有时被认为是 half-proxy，因为他们对一个 end-to-end（端对端）的连接只关心一半：frontend 只关心 client，backend 只关心 server。

HAProxy 也支持 full proxy，通过对 frontend 和 backend 的准确联合来工作。

HAProxy 工作于 HTTP 模式时，配置被分裂为 frontend 和 backend 两个部分，因为任何 frontend 可能转发连接给 任何 backend。

HAProxy 工作于 TCP 模式时，实际上就是 full proxy 模式，配置中使用 frontend 和 backend 不能提供更多的好处，在 full proxy 模式，配置文件更有可读性。



from:http://www.361way.com/haproxy-basic/5267.html









