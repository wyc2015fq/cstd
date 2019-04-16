# Nginx 配置 - youbingchen的博客 - CSDN博客





2016年06月08日 16:44:00[youbingchen](https://me.csdn.net/youbingchen)阅读数：423标签：[nginx](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)
个人分类：[nginx](https://blog.csdn.net/youbingchen/article/category/6263213)









学了 [nginx 命令行控制](http://blog.csdn.net/youbingchen/article/details/51607581)和 [nginx进程间的 关系](http://blog.csdn.net/youbingchen/article/details/51612668),接下来就 进入到Nginx的配置环节

# Nginx服务的基本 配置

主要分为下面4类 

+　用于调试、定位 

+　正常运行的配置 

+　优化性能的配置项 

+　事件类的配置项 
![nginx.cof配置结构 ](https://img-blog.csdn.net/20160614153351291)
**有一些配置项，即使没有显示地进行配置，他也会默认值**
### 用于调试进程和定位 问题 的配置项
- 1.是否以守护进程的方式运行Nginx 

设置方法 ：daemon on | off; 

系统默认是: daemon on。
#### daemon是啥？

daemon是守护进程方式，守护进程是脱离终端并且在后台运行的进程。脱离终端的目的是为了避免进程执行过程中的信息在任何终端显示 ，这样一来，进程不会被任何终端所产生 的信息所打断。 

Nginx 提供关闭守护进程模式，是为了方便跟踪调试Nginx ，毕竟 用gdb调试进程 时，最繁琐的就是如何 继续 跟进 fork出的子进程。 

 + 2.是否以master/worker方式工作的 

设置方法：master_process  on | off 

系统默认：master_process  on
> 
[nginx进程间的 关系](http://blog.csdn.net/youbingchen/article/details/51612668)这篇博客已经介绍这种工作方式 ，Nginx默认 也是以这种工作方式。如果用 off关掉 master_process方式，就不会fork出worker子进程来 处理请求，而是用 master进程自身 来处理请求 。

- 
3.error日志的设置  

语法：error_log /path/file level; 

默认：error_log logs/error..log error
> 
error 日志是定位Nginx问题的最佳工具 。/path/file最好是一个磁盘空间足够大的位置;/path/file也可以是/de/null，这样就不会 输出任何日志文件，这也是error日志关闭的唯一 方法。 

  level是日志的输出级别，取值 范围是debug、info、notice、warn、error、crit、alert、emerg，从左到 右级别 依次增大。/path/file 只会 记录比level级别 大的 记录  
**如果日志级别设定到debug，必须 在 configure时加入 –with-debug配置选项**- 
4.是否处理几个 特殊的调试点 

语法：debug_points [stop| abort]

> 
Nginx在一些关键错误逻辑中设置 了调试点，如果设置stop，那么Nginx 的代码执行到这些调试点时就会发出SIGSTOP信号以用于调试，如果是abort，则会产生coredump文件，可以使用gdb来查看Nginx当时的各种信息。


- 5.仅对指定客户端 输出debug级别 的日志 

语法 ：debug_connection [IP|CIDR] 

> 
   这个配置项实际上 属于 事件类配置，因此，它必须放在events{}中才有效。它 的 值可以是IP地址 或是CIDR地址

```
events {
debug_connection 10.224.66.14
debug_connection  10.224.66.14/24

}
```
使用debug_connection前 ，需确保在执行configure时已经 加入 了–with-debug参数，不然会失效- 6.限制coredump核心存储文件大小 

语法：worker_rlimit_core  size;
- 7.指定coredump文件生成目录 

语法：working_directory path;

### 正常运行的配置项
- 1.定义环境变量 

语法：  env VAR|VAR=VALUE 

这个配置项 可以让 用户直接 设置操作系统上的 环境变量 例如：`env TESTPATH=/tmp/;`- 2.嵌入其他配置文件  

语法 ：include /path/file 

include 配置项可以将 其他 配置文件 嵌入到 当前的Nginx.conf文件中，它的参数可以是绝对路径 也可以是 相对路径（相对Nginx 的配置目录）
```
include mime.types
include vhost/*conf
```

可以看到，参数对的 值 可以是一个明确的文件 名，也可以 是含有通配符*的文件名，还可以一次嵌入多个文件 

+ 3.pid文件路径 

语法：pid /path/file 

默认 ：pid logs/nginx.pid
> 
保存master进程ID的 pid文件存放路径 。默认与 configure执行时参数”–pid-path”所指定的路径是相同的，也可以随时修改，但确保Nginx 有权限创建pid文件，该文件直接影响Nginx是否可以运行。

- 
4.Nginx worker进程运行的用户及用户组 

语法： user username [gruopname] 

默认 ： user  nobody  nobody
> 
user 用于设置master进程 启动后 ，fork出的worker进程 运行在哪个用户和用户组下，按照“user username ”，设置时，用户组名和用户名相同

- 
5.指定Nginx worker进程中可以打开对的最大句柄描述符个数 

语法： worker_rlimit_nofile limit

- 6.限制信号 对列 

语法：worker_rlimit_sigpending limit 

> 
  设置每个用户发往Nginx的信号队列的大小，也就说，当某个用户的 信号队列 满了，这个用户在发送信号量就会 被 丢弃

### 优化性能的配置项
- 1.Nginx  worker 的进程数 

语法：worker_processes number 

默认 ：  worker_processes 1;- 2.绑定Nginx worker进程到指定的CPU内核上 

语法 : worker_cpu_affinity cpumark [] [] 

为什么 要 绑定worker进程到 指定 的CPU内核 呢 ？假定每一个 worker 进程都是繁忙的，如果多个worker进程在抢同一个CPU，那么 就会 出现同步问题，反之，如果每一个 worker进程都独享一个CPU，就在内核的调度策略上实现 了完全的并发 。
```
worker_processes 4
worker_cpu_affinity 1000 0100 0010 0001
```
- 3.SSL硬件加速 

语法：ssl_engine  device 

如果服务器上有SSL硬件加速器设备，那么 就可以进行配置以加速SSL协议的处理速度 。用户可以使用OpenSLL 提供的命令来查看 是否有SSL硬件加速设备 `openssl  engine  -t `- 
4.系统调用gettimeofday的执行频率 

语法： timer_resolution t;

> 
默认情况下，每次内核的事件调用（epoll、select、poll等）返回时，都会执行一次gettimeofday，实现用内核时钟来更新 Nginx 中的缓存时钟，在 早期的linux内核 中，一次gettimeofday的执行代价不小，因为中间有一次 内核态 到用户态的内存复制。但是在目前，gettimeofday只是 一次vsycall，仅仅 对共享内存页的数据访问，并不是通常的系统 调用，代价 不大。


- 
5.Nginx worker进程优先级 设置 

语法： worker_priority  nice 

默认： worker_priority  0;
> 
在Linux或其他 类Unix操作系统中，当许多进程都处于可执行状态 时，将按照所有进程的优先级来决定本次内核选择哪一个进程 来执行。进程所分配的CPU时间片大小 也与进程的 优先级相关，优先级越高，进程分配到的时间片也就越大（最小只有5ms，最大的时间有800ms） 

  优先级由静态优先级和内核 根据执行情况所做的动态调整,nice值是 进程的静态优先级，他的取值范围为-20~+19,-20是最高优先级。如果用户希望 Nginx占有更多系统资源，那么可以把nice值布置小一点，但是 不建议设置比内核进程nice值（通常为-5）更小


### 事件类配置项



- 
1.是否打开accept锁 

语法：accept_mutex [on|off 

默认 ： accept_mutex on;


> 
accept_mutex是Nginx的负载均衡锁，accept_mutex这把锁可以让多个worker进程轮流地、序列化地与新客户建立TCP连接。当一个worker进程建立连接的 数量达到worker_connections配置的最大连接数的7/8时，会大大减少worker进程试图建立 新TCP连接的 机会。以此实现 所有的worker进程之上处理的客户端请求数量尽量接近

- 
2.lock文件的路径 

语法： lock_file path/file 

默认:  lock_file/nginx.lock
> 
accept 锁可能需要这个lock文件，因为由于编译程序、操作系统架构等因素导致Nginx不支持原子锁，这时候采用文件锁实现accept锁

- 3.使用accept锁后到真正建立连接的延迟时间  

语法： accept_mutex_delay Nms 

默认: accept_mutex_delay 500ms 

> 
在使用accept锁之后，同一个时间只有一个worker进程能够得到accept锁。这个 accept 锁不是阻塞锁，如果取不到就立刻 返回，如果有一个 worker进程试图取accept锁而没有取到，它至少要等accept_mutex_delay定义的时间间隔才能试图取锁。

- 4.批量建立连接 

  语法：  multi_accept [on|off] 

  默认： multi_accept  off- 5.选择事件类型 

  语法：  use [kqueue|rtsig|epoll|/dev/poll|select|poll|eventport]; 

  默认： Nginx会自动使用最适合的事件模型- 6 每个worker的最大连接数 

  语法：  worker_connections number





























