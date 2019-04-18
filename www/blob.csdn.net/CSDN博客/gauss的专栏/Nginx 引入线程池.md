# Nginx 引入线程池 - gauss的专栏 - CSDN博客
2015年10月05日 00:59:40[gauss](https://me.csdn.net/mathlmx)阅读数：274
**介绍**
众所周知，NGINX 采用异步、事件驱动的方式处理连接。意味着无需对每个请求创建专门的进程或线程，它用一个工作进程（worker process）处理多个连接和请求。为了达到这个目的，NGINX采用非阻塞模式的 socket，并利用诸如 epoll 和 kqueue 的高效方法。
全量进程（full-weight process）数很少（通常是一个 CPU 核只有一个）而且恒定、内存开销少、CPU 周期不会浪费在任务切换上。此方法的优势因为NGINX而广为人知。它能同时处理成千上万请求，而且容易扩展。
![](http://mmbiz.qpic.cn/mmbiz/KovAgJ2aWya4Ocb0bGkIg7xSZJP0wGj0ibGv0yUwusJbsO3sVJhfZMZA6YJNy0wZ8GKwiaNGqaibVKZplsgIdk5jQ/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)
每个进程消耗额外的内存，进程之间的每次切换都会消耗 CPU 周期和丢弃 CPU 缓存
不过异步、事件驱动方式依然存在一个问题，或者可以说是敌人。其名字就是：阻塞。不幸的是，许多第三方模块采用阻塞方式调用，用户（有时甚至这些模块的开发者）都没有意识到这个缺陷。阻塞操作会毁掉 NGINX 性能，必须采取一切手段避免这样的问题。
甚至在当前 NGINX 官方代码中，也无法在每个例子中避免阻塞操作，为了解决这个问题，NGINX 1.7.11 版实现了新的线程池机制。它是什么，如何使用？我会在后面说明。我们先看看我们的敌人。
**问题**
首先，为了更好的理解问题，我们先简单看看NGINX是如何工作的。
总体来说，NGINX 是一个事件处理器，一个从内核接收所有发生在连接上的事件信息的控制器，然后给操作系统发布命令。实际上，NGINX 通过编排操作系统做了全部的辛苦工作，操作系统则做了读字节和发送字节等日常工作。可见 NGINX 快速及时响应是如此重要。
![](http://mmbiz.qpic.cn/mmbiz/KovAgJ2aWya4Ocb0bGkIg7xSZJP0wGj0icT4xbPoo4PrrVqlSHBFOyG8ticAcJFuFmfbFtgicADGosq75jy5jRWZw/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)
工作进程监听、处理来自内核中的事件。
事件可能是某个超时，或者socket准备读取或者写入的通知，或者错误发生的通知。NGINX 接收一串事件，接着挨个处理，做一些必要的动作。这些处理都在线程队列的简单循环中完成。NGINX 从队列中放出一个事件，接着做出反应，例如写或者读一个 socket。在许多案例中，这非常快（也许只需要很少的CPU 周期就可以将数据复制到内存中），并且 NGINX 会立即处理队列中所有的事件。
![](http://mmbiz.qpic.cn/mmbiz/KovAgJ2aWya4Ocb0bGkIg7xSZJP0wGj0eHTpsQHySMRFDwmOG4OicBu395K1ic9yfAAPrNmibKa6cOu7buYpaM7Jw/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)
所有处理是在一个简单的循环中由某个线程完成的
但是如果遇到某些又长又重操作，又会怎样呢？整个事件处理周期可能会卡在那里等待此操作结束。
我们说的“阻塞操作”是指会让处理循环明显停止一段时间的操作。阻塞的原因多种多样。比如，NGINX忙于漫长的 CPU 密集型处理，或者不得不等待获取某个资源，比如硬件驱动、某个互斥锁、库函数以同步方式调用数据库响应。最关键的是处理诸如此类的操作，工作进程就没有办法做其他的事情，处理其他的事件，即使系统有更多可用资源可供队列中某些事件使用。
试想商店里售货员，面前排着一个很长的队列。排第一的顾客需要的货物在仓库，不是在店里，售货员去仓库搬运货物。为此整个队列需要等待几个小时，等待的人都会不高兴的。你能想象人们的反应么？队列中每个人等待时间因为这几个小时而增加，但他们想买的货物或许就在商店里。
![](http://mmbiz.qpic.cn/mmbiz/KovAgJ2aWya4Ocb0bGkIg7xSZJP0wGj0S3fdOQCY3AoBGibhP43sIhJNEJGFMWqbXo8Z3fwGScDoXzeJuuGbI3A/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)
队列中的每一个人不得不等待第一个人的订单
几乎同样的场景发生在 NGINX 中，需要读取一个文件，但它没有缓存在内存，不得不从硬盘中读取。硬盘很慢（特别是旋转的机械硬盘），然而队列中其他等待的请求即使无需读取硬盘，也被迫等待。结果增加了延迟，系统资源没有被充分利用。
![](http://mmbiz.qpic.cn/mmbiz/KovAgJ2aWya4Ocb0bGkIg7xSZJP0wGj0HUtvXRom00us2ICWXkD12NPRShK3okBgaqQ8gX9ibTia3vlJdBu8iaibvw/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)
仅仅一个阻塞操作就能长时间地延迟接下来所有的操作
某些操作系统（比如 FreeBSD）提供了一个读文件和发送文件的异步接口，NGINX可以调用这个接口（见 aio 指令）。不幸的是，Linux 并非都如此。尽管 Linux系统也提供了读取文件的异步接口，但它有两个重大缺陷。其一是文件读取和缓存时需要对齐，不过 NGINX 能处理地很好。第二个问题更糟，异步接口需要在文件描述符上作 O_DIRECT 标记，这样任何获取文件的操作越过内存级的缓存，增加了硬盘负载。在很多例子中，这真不是一个好的选择。
为解决这个问题，NGINX 1.7.11 引入了线程池。NGINX Plus 默认状态下没有线程池，如果你想给 NGINX Plus R6 构建一个线程池，请联系销售。
让我们深入探究什么是线程池、它是如何工作的。
**线程池**
让我们回到刚才那个可怜的销售助理，从很远的仓库取货物。但是他变聪明了，也或许因为愤怒地顾客鄙视变得聪明了？购买了一套配送服务。现在有人想购买远距离仓库中的货物，销售助理无需前往，只需要将订单转给配送服务，后者会处理这个订单，销售助理可以继续为其他顾客服务。由此只有货物不再商铺的顾客需要等待货物提取，其他顾客能够快速得到服务。
![](http://mmbiz.qpic.cn/mmbiz/KovAgJ2aWya4Ocb0bGkIg7xSZJP0wGj0Rd6OQibIww6RH43CbWuicSeuL8aia4U0kjxa5uoibt9sJLjDgoKcXuicevQ/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)
把订单转给配送服务，这样就不会阻塞队列了
对 NGINX 而言，线程池就是充当配送服务的角色，它由一个任务队列和一组处理队列的线程组成。一旦工作进程需要处理某个可能的长操作，不用自己操作，将其作为一个任务放出线程池的队列，接着会被某个空闲线程提取处理。
![](http://mmbiz.qpic.cn/mmbiz/KovAgJ2aWya4Ocb0bGkIg7xSZJP0wGj00mUJiaJ8qztkcWu3v6w5GjiaaVdB9CibtjibEsvaELOckkCsaYzd6VZKPA/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)
工作进程把阻塞操作转给线程池
像是拥有了一个新的队列，不过本例中的队列局限于某个特定的资源。从硬盘中读取数据的速度不会超过硬盘生成数据的速度。硬盘没有延迟处理其他事件，仅仅需要获取文件的请求在等待。
硬盘读取操作通常就是阻塞操作，不过NGINX中的线程池可以用来处理任何在主工作周期不适合处理的任务。
此刻分派给线程池的任务主要有两个：许多操作系统上 read() 方法的系统调用，以及 Linux 系统的 sendfile()方法。我们会继续测试（test）和基准测试（benchmark），未来发布的版本或许将其他的操作分派给线程池。
**基准测试**
I到了从理论到实践的时候了，为了展示利用线程池的效果，我们打算设置一个合成基准模拟最糟糕的阻塞或者非阻塞操作。
数据集不能超出内存，在一个 48GB 内存机器上，生成 256GB 随机数据，每个文件大小 4MB ，接着配置 NGINX 1.9.0 为其提供服务。
配置及其简单：
> 
worker_processes 16;
events {
accept_mutex off;
}
http {
include mime.types;
default_type application/octet-stream;
access_log off;
sendfile on;
sendfile_max_chunk 512k;
server {
listen 8000;
location / {
root /storage;
}
}
}
正如你所看到的，为了获得更好的性能，我们做了一些调优：关闭了 loggin 和 accept_mutex，同时开启了 sendfile()，设置 sendfile_max_chunk 大小为512K。最后面的指令可以减少阻塞方法 sendfile() 调用的所花费的最大时间，即 NGINX 每次无需发送整个文件，只发送 512KB 的块数据。
计算机含有两个英特尔至强 E5645 处理器(Intel Xeon E5645)，以及 10Gbps 网络接口。硬盘子系统由四个西部数据 WD1003FBYX 硬盘按放在一个 RAID10 阵列中。所有硬件由Ubuntu Server 14.04.1 LTS进行管理。
![](http://mmbiz.qpic.cn/mmbiz/KovAgJ2aWya4Ocb0bGkIg7xSZJP0wGj0en4SDibribhX6iaqGX3w9aG5AjArmkOr05JZiccpCFENcUkDW5Z0AhGzbw/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)
为基准测试，配置 NGINX 和负载生成器
客户端由两个配置相同的计算机组成，其中一台，wrk 通过用 Lua 脚本创建负载。脚本通过 200 个并行连接，随机向服务器请求文件。每一个请求可能导致缓存失效、产生一个硬盘阻塞读操作。姑且称这种负载叫随机负载。
在第二台客户端计算机上，我们运行另一个 wrk 拷贝，用 50 个并行连接多次访问同一个文件。因为文件高频访问，它会一直留在内存中。通常，NGINX可以非常快地处理这些请求，不过工作进程一旦阻塞被其他请求阻塞，性能就会下滑。姑且称这种负载为恒定负载。
利用 ifstat 命令获取第二台客户端的 wrk 结果，来监控服务器吞吐量，并以此测定服务器性能。
第一次没有线程池参与的运行，并没有带给我们什么惊喜的结果：
> 
% ifstat -bi eth2
eth2
Kbps in Kbps out
5531.24 1.03e+06
4855.23 812922.7
5994.66 1.07e+06
5476.27 981529.3
6353.62 1.12e+06
5166.17 892770.3
5522.81 978540.8
6208.10 985466.7
6370.79 1.12e+06
6123.33 1.07e+06
正如你所看到的，如此配置的服务器产生流量总计大约为 1 Gbps。从 top 的输出信息，我们可以看出所有工作进程在阻塞输入输出上花费的时间：
> 
top - 10:40:47 up 11 days, 1:32, 1 user, load average: 49.61, 45.77 62.89
Tasks: 375 total, 2 running, 373 sleeping, 0 stopped, 0 zombie
%Cpu(s): 0.0 us, 0.3 sy, 0.0 ni, 67.7 id, 31.9 wa, 0.0 hi, 0.0 si, 0.0 st
KiB Mem: 49453440 total, 49149308 used, 304132 free, 98780 buffers
KiB Swap: 10474236 total, 20124 used, 10454112 free, 46903412 cached Mem
PID USER PR NI VIRT RES SHR S %CPU %MEM TIME+ COMMAND
4639 vbart 20 0 47180 28152 496 D 0.7 0.1 0:00.17 nginx
4632 vbart 20 0 47180 28196 536 D 0.3 0.1 0:00.11 nginx
4633 vbart 20 0 47180 28324 540 D 0.3 0.1 0:00.11 nginx
4635 vbart 20 0 47180 28136 480 D 0.3 0.1 0:00.12 nginx
4636 vbart 20 0 47180 28208 536 D 0.3 0.1 0:00.14 nginx
4637 vbart 20 0 47180 28208 536 D 0.3 0.1 0:00.10 nginx
4638 vbart 20 0 47180 28204 536 D 0.3 0.1 0:00.12 nginx
4640 vbart 20 0 47180 28324 540 D 0.3 0.1 0:00.13 nginx
4641 vbart 20 0 47180 28324 540 D 0.3 0.1 0:00.13 nginx
4642 vbart 20 0 47180 28208 536 D 0.3 0.1 0:00.11 nginx
4643 vbart 20 0 47180 28276 536 D 0.3 0.1 0:00.29 nginx
4644 vbart 20 0 47180 28204 536 D 0.3 0.1 0:00.11 nginx
4645 vbart 20 0 47180 28204 536 D 0.3 0.1 0:00.17 nginx
4646 vbart 20 0 47180 28204 536 D 0.3 0.1 0:00.12 nginx
4647 vbart 20 0 47180 28208 532 D 0.3 0.1 0:00.17 nginx
4631 vbart 20 0 47180 756 252 S 0.0 0.1 0:00.00 nginx
4634 vbart 20 0 47180 28208 536 D 0.0 0.1 0:00.11 nginx
4648 vbart 20 0 25232 1956 1160 R 0.0 0.0 0:00.08 top
25921 vbart 20 0 121956 2232 1056 S 0.0 0.0 0:01.97 sshd
25923 vbart 20 0 40304 4160 2208 S 0.0 0.0 0:00.53 zsh
I本例中，吞吐量的短板为硬盘子系统，CPU大多数时间都在空闲。wrk 输出结果看吞吐量很低：
> 
Running 1m test @ http://192.0.2.1:8000/1/1/1
12 threads and 50 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 7.42s 5.31s 24.41s 74.73%
Req/Sec 0.15 0.36 1.00 84.62%
488 requests in 1.01m, 2.01GB read
Requests/sec: 8.08
Transfer/sec: 34.07MB
记住，应该从内存送达文件。巨大的延迟是因为所有的工作进程忙于从硬盘读取文件，响应第一个客户端的 200 个连接创建的随机负载，无法处理我们的请求。
是时候让线程登场了，为此我们给 location 模块添加了 aio threads 指令：
> 
location / {
root /storage;
aio threads;
}
请求NGINX重载其配置
重新测试结果：
> 
% ifstat -bi eth2
eth2
Kbps in Kbps out
60915.19 9.51e+06
59978.89 9.51e+06
60122.38 9.51e+06
61179.06 9.51e+06
61798.40 9.51e+06
57072.97 9.50e+06
56072.61 9.51e+06
61279.63 9.51e+06
61243.54 9.51e+06
59632.50 9.50e+06
此时服务器产生 9.5 Gbps 流量，而没有线程池参与时只产生大约 1 Gbps 的流量。
甚至可以产生更多流量，不过这已经达到实际网络最大容量。由此可见本测试中，制约NGINX因素为网络接口。工作进程大部分时间在休眠和等待新事件，参见top输出S state
> 
top - 10:43:17 up 11 days, 1:35, 1 user, load average: 172.71, 93.84, 77.90
Tasks: 376 total, 1 running, 375 sleeping, 0 stopped, 0 zombie
%Cpu(s): 0.2 us, 1.2 sy, 0.0 ni, 34.8 id, 61.5 wa, 0.0 hi, 2.3 si, 0.0 st
KiB Mem: 49453440 total, 49096836 used, 356604 free, 97236 buffers
KiB Swap: 10474236 total, 22860 used, 10451376 free, 46836580 cached Mem
PID USER PR NI VIRT RES SHR S %CPU %MEM TIME+ COMMAND
4654 vbart 20 0 309708 28844 596 S 9.0 0.1 0:08.65 nginx
4660 vbart 20 0 309748 28920 596 S 6.6 0.1 0:14.82 nginx
4658 vbart 20 0 309452 28424 520 S 4.3 0.1 0:01.40 nginx
4663 vbart 20 0 309452 28476 572 S 4.3 0.1 0:01.32 nginx
4667 vbart 20 0 309584 28712 588 S 3.7 0.1 0:05.19 nginx
4656 vbart 20 0 309452 28476 572 S 3.3 0.1 0:01.84 nginx
4664 vbart 20 0 309452 28428 524 S 3.3 0.1 0:01.29 nginx
4652 vbart 20 0 309452 28476 572 S 3.0 0.1 0:01.46 nginx
4662 vbart 20 0 309552 28700 596 S 2.7 0.1 0:05.92 nginx
4661 vbart 20 0 309464 28636 596 S 2.3 0.1 0:01.59 nginx
4653 vbart 20 0 309452 28476 572 S 1.7 0.1 0:01.70 nginx
4666 vbart 20 0 309452 28428 524 S 1.3 0.1 0:01.63 nginx
4657 vbart 20 0 309584 28696 592 S 1.0 0.1 0:00.64 nginx
4655 vbart 20 0 30958 28476 572 S 0.7 0.1 0:02.81 nginx
4659 vbart 20 0 309452 28468 564 S 0.3 0.1 0:01.20 nginx
4665 vbart 20 0 309452 28476 572 S 0.3 0.1 0:00.71 nginx
5180 vbart 20 0 25232 1952 1156 R 0.0 0.0 0:00.45 top
4651 vbart 20 0 20032 752 252 S 0.0 0.0 0:00.00 nginx
25921 vbart 20 0 121956 2176 1000 S 0.0 0.0 0:01.98 sshd
25923 vbart 20 0 40304 3840 2208 S 0.0 0.0 0:00.54 zsh
仍有充裕的CPU资源。
wrk执行结果：
> 
Running 1m test @ http://192.0.2.1:8000/1/1/1
12 threads and 50 connections
Thread Stats Avg Stdev Max +/- Stdev
Latency 226.32ms 392.76ms 1.72s 93.48%
Req/Sec 20.02 10.84 59.00 65.91%
15045 requests in 1.00m, 58.86GB read
Requests/sec: 250.57
Transfer/sec: 0.98GB
处理一个 4 MB文件的平均时间由 7.42 秒降到 226.32 毫秒，降低至少33倍。同时，每秒请求数提高31倍。
这是因为我们的请求无需在事件队列中等待处理，即使工作进程阻塞在读操作上，请求可以由空闲的线程来完成处理。只要硬盘子系统表现出色，NGINX很好地为来自第一个客户端的随机负载服务，它就可以利用剩余的CPU资源和网络容量，从内存读取，为第二个客户端的请求服务。
**这并非是银弹**
当我们经历了阻塞操作的带来的恐惧以及线程池带来的兴奋感之后，或许我们中的多数人已经打算在服务器中配置线程池。别急！
幸运的是，多数读写文件操作无需处理缓慢的硬盘，如果你有足够的内存，操作系统会足够聪明把那些高频次访问的文件缓存到一个称之为“页面缓存”（page cache）中。
页面缓存表现优异，使得 NGINX 几乎在通常的用例中性能表现突出。从页面缓存中读取速度非常快，没有人认为类操作是“阻塞”的。换言之，分派负载给线程池会带来一些开销。
所以，如果有合适的内存，并且数据集不大，那么无需线程池，NGINX 就可以在最佳性能下工作。
分派读操作给线程池是一种对针对特定任务的技术。频次非常高的请求内容不适合放入操作系统虚拟缓存中，这时候线程池就很有了。或许就是如此，例如，重量级基于NGINX负载流媒体服务器。我们的基准测试已模仿这个场景。
如果能将读操作分派给线程池是极好的，我们所要做的是需要的文件数据是否在内存中，如果不在内存中，那么我们就应该将读操作分派给某个线程。
回到销售的例子，当下销售员面临的情况是，不知道请求物品是否在店铺，要么将所有的订单传给提取货物服务，要么他自己处理这些订单。
要命的是，操作系统可能永远没有这个功能。第一次尝试是 2010 年 linux 中引入 fincore() 系统调用方法，没有成功。接着做了一系列尝试，例如引入新的带有 RWF_NONBLOCK 标记的 preadv2() 系统调用方法。所有的这些补丁前景依旧不明朗。比较悲剧的是，因为持续的口水战，导致这些补丁一直没有被内核接受。
另一个原因是，FreeBSD用户根本不会关心这个。因为 FreeBSD 已经有一个非常高效的异步文件读取接口，完全可以不用线程池。
**配置线程池**
如果确信你的用例采用线程池可以获利，那么是时候深入其配置了。
线程池配置非常容易而且灵活。首先你需要 NGINX 1.7.11 版，或者更新的版本，采用配置文件中的参数 –with-threads 进行编译。最简单的例子，配置看起来相当的容易，所有你需要做的的事情就是给http、server或者location上下文中添加 aio threads 指令。
> 
aio threads;
这可能是最简短的线程池配置了，实际上，下面这个配置是一个简化版的：
> 
thread_pool default threads=32 max_queue=65536;
aio threads=default;
它定义一个名为 default 的线程池，拥有 32 个工作线程，任务队列容纳的最大请求数为 65536。一旦任务队列过载，NGINX日志会报错并拒绝这一请求：
> 
thread pool "NAME" queue overflow: N tasks waiting
报错意味着线程可能处理工作的速度跟不上任务添加进队列的速度，你可以试着增加队列的到最大容量。如果还是不起作用，可能是系统服务请求的数量已达到了上线。
正如你所看到的，可以用thread_pool指令设置线程数量、队列最大容量、为某个线程池命名。为某个线程池命名意味着你可以设置多个独立的线程池，在不同的配置文件用于不同目的。
> 
http {
thread_pool one threads=128 max_queue=0;
thread_pool two threads=32;
server {
location /one {
aio threads=one;
}
location /two {
aio threads=two;
}
}
…
}
如果没有指定max_queue参数，它的默认值为65536。如上面所展示的，可以将max_queue设置为0。这意味这，如在本例，线程池只能处理分派给线程那些任务；因为队列中没有存储任何等待的任务。
试想你的服务器有三个硬盘，你希望服务器能像缓存代理一样作用，缓存所有来自后端的响应，预期缓存的数据量远远超过了现有的内存。这个缓存节点为私人内容分发网络(CDN)服务，当然本例中最重要的事情就是从硬盘那里获取最大性能。
一种选择是设置一个磁盘阵列，这种方式有其优点和缺点。NGINX采用另外一种方式：
> 
# 假定每个硬盘驱动挂载一个文件目录上
# We assume that each of the hard drives is mounted on one of the directories:
# /mnt/disk1, /mnt/disk2, or /mnt/disk3 accordingly
proxy_cache_path /mnt/disk1 levels=1:2 keys_zone=cache_1:256m max_size=1024G
use_temp_path=off;
proxy_cache_path /mnt/disk2 levels=1:2 keys_zone=cache_2:256m max_size=1024G
use_temp_path=off;
proxy_cache_path /mnt/disk3 levels=1:2 keys_zone=cache_3:256m max_size=1024G
use_temp_path=off;
thread_pool pool_1 threads=16;
thread_pool pool_2 threads=16;
thread_pool pool_3 threads=16;
split_clients $request_uri $disk {
33.3% 1;
33.3% 2;
* 3;
}
location / {
proxy_pass http://backend;
proxy_cache_key $request_uri;
proxy_cache cache_$disk;
aio threads=pool_$disk;
sendfile on;
}
在这个设置中，用到了三个独立的缓存，对应一个硬盘。同样也有三个独立的线程池对应某个硬盘。
split_clients 模块用于缓存之间的负载平衡，很好地满足这个任务。
proxy_cache_path 指令中的参数 use_temp_path=off 指示 NGINX 存储临时文件到缓存数据对应的相同目录中；在缓存更新时，避免磁盘之间拷贝响应数据。
做的所有这一切，都是为了使当前硬盘子系统性能达到最大，因为NGINX中每个线程池与磁盘的交互都是独立并行的。每个磁盘由 16 个独立的线程为其服务，即处理某个特定任务队列中文件的读取和发送。
我猜测你的客户端采用类似客户定制的方式，那么同样确保你的硬盘驱动也采用类似的方式。
本例很好的展示了NGINX如何灵活地针对特定硬盘做出调优，就像你给出指令，告诉NGINX与计算机以及数据集的最佳交互方式。通过细粒度的NGINX调优，可以确保软件、操作系统、硬件处在一种最佳的工作状态，即尽可能有效地利用系统资源。
**结论**
总之，线程池是一个非常棒的特性，它能促使NGINX性能上一个新台阶，移除了众所周知的顽疾——阻塞，特别是涉及海量数据的时候。
当然远非这些，正如前面所提到的，新的接口可能会允许我们分派任何长的阻塞操作，而且不会有性能损失。NGINX开辟了新天地，拥有一批新的模块和功能。而许多流行的库依旧没有提供某种异步非阻塞接口，这样很难和NGINX兼容。或许我们需要花费很多时间和资源，开发一些我们自己的非阻塞原生库，但这样做值得么？随着线程池特性的上线，这些库在不影响模块性能的前提下会更加相对简单易用。
