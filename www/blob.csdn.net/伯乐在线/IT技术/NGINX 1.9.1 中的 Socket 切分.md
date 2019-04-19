# NGINX 1.9.1 中的 Socket 切分 - 文章 - 伯乐在线
原文出处： [Nginx](https://www.nginx.com/blog/socket-sharding-nginx-release-1-9-1/)   译文出处：[OSChina](http://www.oschina.net/translate/socket-sharding-nginx-release-1-9-1?print)
[NGINX](http://nginx.org/en)发布的1.9.1版本引入了一个新的特性：允许使用SO_REUSEPORT套接字选项，该选项在许多操作系统的新版本中是可用的，包括DragonFly BSD和Linux（内核版本3.9及以后）。该套接字选项允许多个套接字监听同一IP和端口的组合。内核能够在这些套接字中对传入的连接进行负载均衡。
（对于[NGINX Plus](http://nginx.com/)客户，此功能将在年底发布的版本7中出现）
SO_REUSEPORT选项有许多潜在的实际应用。其他服务也可以使用它来简单实现执行中的滚动升级（Nginx已经通过[不同的办法](http://nginx.org/en/docs/control.html#upgrade)支持了滚动升级）。对于NGINX而言，启用该选项可以减少在某些场景下的锁竞争而改善性能。
如下图描述，当SO_REUSEPORT选项有效时，一个单独的监听socket通知工作进程接入的连接，并且每个工作线程都试图获得连接。
![](http://jbcdn2.b0.upaiyun.com/2016/03/798d92d9bfee98cd490e44b575b31fb4.png)
当SO_REUSEPORT选项启用是，存在对每一个IP地址和端口绑定连接的多个socket监听器，每一个工作进程都可以分配一个。系统内核决定哪一个有效的socket监听器（通过隐式的方式，给哪一个工作进程）获得连接。这可以减少工作进程之间获得新连接时的封锁竞争（译者注：工作进程请求获得互斥资源加锁之间的竞争），同时在多核系统可以提高性能。然而，这也意味着当一个工作进程陷入阻塞操作时，阻塞影响的不仅是已经接受连接的工作进程，也同时让内核发送连接请求计划分配的工作进程因此变为阻塞。
![](http://jbcdn2.b0.upaiyun.com/2016/03/06ab04b268b99d1c66ed035e19d9fb6f.png)
## 设置共享Socket
为了让SO_REUSEPORT socket选项起作用，应为HTTP或TCP（流模式）通信选项内的listen项直接引入新近的reuseport参数，就像下例这样：

C#
```
http {
     server {          listen 80 reuseport;
          server_name  localhost;
          ...
     }
}
stream {
     server {          listen 12345 reuseport;
          ...
     }
}
```
引用reuseport参数后，对引用的socket，accept_mutex参数将会无效，因为互斥量（mutex）对reuseport来说是多余的。对没有使用reuseport的端口，设置accept_mutex仍然是有价值的。
## reuseport的基准性能测试
我在一个36核的[AWS实例](https://aws.amazon.com/cn/ec2/instance-types/)运行[wrk](https://github.com/wg/wrk)基准测试工具测试4个NGINX 工作进程.为了减少网络的影响，客户端和NGINX都运行在本地，并且让NGINX返回OK字符串而不是一个文件。我比较三种NGINX配置：默认（等同于accept_mutex on ）,accept_mutex off,和reuseport。如图所示，reuseport的每秒请求是其余的两到三倍，同时延迟和延迟标准差也是减少的。
![reuseport-benchmark](http://jbcdn2.b0.upaiyun.com/2016/03/2e4f7b0e1f293cbbc137877e1e92dd2b.png)
我又运行了另一个相关的性能测试——客户端和NGINX分别在不同的机器上且NGINX返回一个HTML文件。如下表所示，用 reuseport 减少的延迟和之前的性能测试相似，延迟的标准差减少的更为显著（接近十分之一）。其他结果（没有显示在表格中）同样令人振奋。使用 reuseport ，负载被均匀分离到了worker进程。在默认条件下（等同于 accept_mutex on），一些worker分到了较高百分比的负载，而用 accept_mutex off 所有worker都受到了较高的负载。
||**Latency (ms)**|**Latency stdev (ms)**|**CPU Load**|
|----|----|----|----|
|**Default**|15.65|26.59|0.3|
|accept_mutex off|15.59|26.48|10|
|reuseport|12.35|3.15|0.3|
在这些性能测试中，连接请求的速度是很高的，但是请求不需要大量的处理。其他的基本的测试应该指出——当应用流量符合这种场景时 reuseport 也能大幅提高性能。（reuseport 参数在 mail 上下文环境下不能用在 listen 指令下，例如email，因为email流量一定不会匹配这种场景。）我们鼓励你先测试而不是直接大规模应用。关于测试NGNIX性能的一些技巧，看看[Konstantin Pavlov在nginx2014大会上的演讲。](http://www.youtube.com/watch?v=eLW_NSuwYU0)
## 致谢
感谢 Sepherosa Ziehau 和 Yingqi Lu, 他们为NGNIX项目贡献了一个使用socket选项SO_REUSEPORT 的方案。NGNIX团队整合他们的贡献和想法所创建的，我们相信是一个理想的解决方案。
