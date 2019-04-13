
# Apache 工作的三种模式：Prefork、Worker、Event - 阳光岛主 - CSDN博客

2019年01月06日 13:22:26[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：297


**Apache 的三种工作模式（Prefork、Worker、Event）**
Web服务器Apache目前一共有三种稳定的MPM（Multi-Processing Module，多进程处理模块）模式。
它们分别是prefork，worker、event，它们同时也代表这Apache的演变和发展。
本文原文转自米扑博客：[Apache 工作的三种模式：Prefork、Worker、Event](https://blog.mimvp.com/article/27778.html)
如何查看我们的Apache的工作模式呢？可以使用httpd -V 命令查看，如我安装的Apache 2.4版本。
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|\# httpd -V
|Server version: Apache|/2|.4.34 (Unix)
|Server built:   Aug  2 2018 19:44:29
|Server's Module Magic Number: 20120211:79
|Server loaded:  APR 1.6.3, APR-UTIL 1.6.1
|Compiled using: APR 1.6.3, APR-UTIL 1.6.1
|Architecture:   64-bit
|Server MPM:     event
|threaded:|yes|(fixed thread count)
|forked:|yes|(variable process count)
|
或者，更直接的命令 httpd -l 或 apachectl -V | grep -i mpm
|1
|2
|3
|4
|5
|6
|7
|8
|\# httpd -l
|Compiled|in|modules:
|core.c
|mod_so.c
|http_core.c
|event.c
|\# apachectl -V | grep -i mpm
|Server MPM:     event
|
这里使用的是event模式，在apache的早期版本2.0默认prefork，2.2版本是worker，2.4版本是event，详见米扑博客：[Apache 服务器负载低访问慢的原因分析和优化方案](https://blog.mimvp.com/article/22790.html)
在configure配置编译参数的时候，可以使用`--with-mpm=prefork|worker|event`来指定编译为那一种MPM，当然也可以用编译为三种都支持：`--enable-mpms-shared=all`，这样在编译的时候会在modules目录下自动编译出三个MPM文件的so，然后通过修改httpd.conf配置文件更改MPM
**1、Prefork MPM**
Prefork MPM实现了一个非线程的、预派生的web服务器。它在Apache启动之初，就先预派生一些子进程，然后等待连接；可以减少频繁创建和销毁进程的开销，每个子进程只有一个线程，在一个时间点内，只能处理一个请求。这是一个成熟稳定，可以兼容新老模块，也不需要担心线程安全问题，但是一个进程相对占用资源，消耗大量内存，不擅长处理高并发的场景。
![](https://cdn-blog.mimvp.com/wp-content/uploads/2018/03/apache-fu-wu-qi-fu-zai-di-fang-wen-man-de-yuan-yin-fen-xi-he-you-hua-fang-an-01-700x394.jpg)
如何配置在Apache的配置文件httpd.conf的配置方式：
|1
|2
|3
|4
|5
|6
|7
|<IfModule mpm_prefork_module>
|StartServers 5
|MinSpareServers 5
|MaxSpareServers 10
|MaxRequestWorkers 250
|MaxConnectionsPerChild 1000
|<|/IfModule|>
|
`StartServers`服务器启动时建立的子进程数量，prefork默认是5，
`MinSpareServers`空闲子进程的最小数量，默认5；如果当前空闲子进程数少于`MinSpareServers`，那么Apache将以最大每秒一个的速度产生新的子进程。此参数不要设的太大。
`MaxSpareServers`空闲子进程的最大数量，默认10；如果当前有超过`MaxSpareServers`数量的空闲子进程，那么父进程会杀死多余的子进程。次参数也不需要设置太大，如果你将其设置比`MinSpareServers`小，Apache会自动将其修改为`MinSpareServers +1`的数量。
`MaxRequestWorkers`限定服务器同一时间内客户端最大接入的请求数量，默认是256；任何超过了`MaxRequestWorkers`限制的请求都要进入等待队列，一旦一个个连接被释放，队列中的请求才将得到服务，如果要增大这个数值，必须先增大`ServerLimit`。在Apache2.3.1版本之前这参数`MaxRequestWorkers`被称为`MaxClients`。
`MaxConnectionsPerChild`每个子进程在其生命周期内允许最大的请求数量，如果请求总数已经达到这个数值，子进程将会结束，如果设置为0，子进程将永远不会结束。在Apache2.3.9之前称之为`MaxRequestsPerChild`。
这里建议设置为非零，注意原因：
1）能够防止(偶然的)内存泄漏无限进行，从而耗尽内存。
2）给进程一个有限寿命，从而有助于当服务器负载减轻的时候减少活动进程的数量(重生的机会)。
**2、Worker MPM**
和prefork模式相比，worker使用了多进程和多线程的混合模式，worker模式也同样会先预派生一些子进程，然后每个子进程创建一些线程，同时包括一个监听线程，每个请求过来会被分配到一个线程来服务。线程比起进程会更轻量，因为线程是通过共享父进程的内存空间，因此，内存的占用会减少一些，在高并发的场景下会比prefork有更多可用的线程，表现会更优秀一些；另外，如果一个线程出现了问题也会导致同一进程下的线程出现问题，如果是多个线程出现问题，也只是影响Apache的一部分，而不是全部。由于用到多进程多线程，需要考虑到线程的安全了，在使用keep-alive长连接的时候，某个线程会一直被占用，即使中间没有请求，需要等待到超时才会被释放（该问题在prefork模式下也存在）。
![](https://cdn-blog.mimvp.com/wp-content/uploads/2018/03/apache-fu-wu-qi-fu-zai-di-fang-wen-man-de-yuan-yin-fen-xi-he-you-hua-fang-an-02-700x697.jpg)
如何配置在Apache的配置文件httpd.conf的配置方式：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|<IfModule mpm_worker_module>
|StartServers 3
|ServerLimit 16
|MinSpareThreads 75
|MaxSpareThreads 250
|ThreadsPerChild 25
|MaxRequestWorkers 400
|MaxConnectionsPerChild 1000
|<|/IfModule|>
|
配置参数解释：
`StartServers`服务器启动时建立的子进程数量,在workers模式下默认是3.
`ServerLimit`系统配置的最大进程数量，默认不显示，自己添加上
`MinSpareThreads`空闲子进程的最小数量，默认75
`MaxSpareThreads`空闲子进程的最大数量，默认250
`ThreadsPerChild`每个子进程产生的线程数量，默认是64
`MaxRequestWorkers / MaxClients`限定服务器同一时间内客户端最大接入的请求数量.
`MaxConnectionsPerChild`每个子进程在其生命周期内允许最大的请求数量，如果请求总数已经达到这个数值，子进程将会结束，如果设置为0，子进程将永远不会结束。在Apache2.3.9之前称之为`MaxRequestsPerChild`。
这里建议设置为非零，注意原因：
1）能够防止(偶然的)内存泄漏无限进行，从而耗尽内存；
2）给进程一个有限寿命，从而有助于当服务器负载减轻的时候减少活动进程的数量(重生的机会)。
Worker模式下所能同时处理的请求总数是由子进程总数乘以`ThreadsPerChild`值决定的，应该大于等于`MaxRequestWorkers`。
如果负载很大，现有的子进程数不能满足时，控制进程会派生新的子进程。默认`ServerLimit`最大的子进程总数是16，加大时也需要显式声明`ServerLimit`（最大值是20000）。
需要注意的是，如果显式声明了`ServerLimit`，那么它乘以`MaxRequestWorkers`必须是`hreadsPerChild`的整数倍，否则 Apache将会自动调节到一个相应值。
**3、Event MPM**
这是Apache最新的工作模式，它和worker模式很像，不同的是在于它解决了keep-alive长连接的时候占用线程资源被浪费的问题，在event工作模式中，会有一些专门的线程用来管理这些keep-alive类型的线程，当有真实请求过来的时候，将请求传递给服务器的线程，执行完毕后，又允许它释放。这增强了在高并发场景下的请求处理。
![](https://cdn-blog.mimvp.com/wp-content/uploads/2018/03/apache-fu-wu-qi-fu-zai-di-fang-wen-man-de-yuan-yin-fen-xi-he-you-hua-fang-an-03-700x424.jpg)
如何配置在Apache的配置文件httpd.conf的配置方式：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|<IfModule mpm_event_module>
|StartServers 3
|ServerLimit 16
|MinSpareThreads 75
|MaxSpareThreads 250
|ThreadsPerChild 25
|MaxRequestWorkers 400
|MaxConnectionsPerChild 1000
|<|/IfModule|>
|
event 模式与 worker 模式完全一样，参考 worker 模式参数即可，这里不再重复。
Apache httpd 能更好的为有特殊要求的站点定制。
例如，要求更高伸缩性的站点可以选择使用线程的 MPM，即 worker 或 event； 需要可靠性或者与旧软件兼容的站点可以使用 prefork。
**常见问题**
查看apache的error日志，可以发现许多系统运行中的问题。
**server reached MaxRequestWorkers setting**
`[mpm_prefork:error] [pid 1134] AH00161: server reached MaxRequestWorkers setting, consider raising the MaxRequestWorkers setting`进程或者线程数目达到了MaxRequestWorkers，可以考虑增加这个值，当然先考虑增加硬件，如内存大小、CPU、SSD硬盘等。
**scoreboard is full**
`[mpm_event:error] [pid 7555:tid 140058436118400] AH00485: scoreboard is full, not at MaxRequestWorkers`
这个问题好像是apache2自带的bug，我们无力解决。好在这个问题一般只会影响单个线程，所以暂时可以忍。
StackOverflow：[Scoreboard is full,not at MaxRequestWorkers](https://stackoverflow.com/questions/22576807/scoreboard-is-full-not-at-maxrequestworkers)
1、I had this same problem. I tried different Apache versions and MPMs. I seem to get this alot with MPM Worker. Also error does not reoccur using Apache 2.2.2，Are you using cPanel? IF so try /upcp --force and increase StartServers to a higher amount like 50 as that's all I did to get this error away.
2、Try EnableMMAP Off in 00_default_settings.conf
**apache 主要版本有：**
[Version 2.4](https://httpd.apache.org/docs/2.4/)([Current](https://httpd.apache.org/docs/current/))
[Version 2.2](https://httpd.apache.org/docs/2.2/)(Historical)
[Version 2.0](https://httpd.apache.org/docs/2.0/)(Historical)
[Version 1.3](https://httpd.apache.org/docs/1.3/)(Historical)
参考：[https://httpd.apache.org/docs/](https://httpd.apache.org/docs/)
关于 Apache 配置优化，请参见米扑博客：[Apache 服务器负载低访问慢的原因分析和优化方案](https://blog.mimvp.com/article/22790.html)

**参考推荐**：
[Apache 工作的三种模式：Prefork、Worker、Event](https://blog.mimvp.com/article/27778.html)
[Apache 服务器负载低访问慢的原因分析和优化方案](https://blog.mimvp.com/article/22790.html)
[Apache 设置禁止访问网站目录](https://blog.mimvp.com/article/12259.html)
[Apache 封禁IP及IP段访问](https://blog.mimvp.com/article/12472.html)
[Apache 日志格式详解](https://blog.mimvp.com/article/27722.html)
[Apache 实现https+Apache http访问转到https](https://blog.mimvp.com/article/12273.html)
[Nginx 配置文件禁止访问目录或文件](https://blog.mimvp.com/article/18435.html)

