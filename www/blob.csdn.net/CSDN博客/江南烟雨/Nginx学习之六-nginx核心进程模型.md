# Nginx学习之六-nginx核心进程模型 - 江南烟雨 - CSDN博客
2013年07月04日 11:22:23[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：14756
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)
一、Nginx整体架构
正常执行中的nginx会有多个进程，最基本的有master process（监控进程，也叫做主进程）和woker process（工作进程），还可能有cache相关进程。
一个较为完整的整体框架结构如图所示：
![](https://img-blog.csdn.net/20130704111548406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
二、核心进程模型
启动nginx的主进程将充当监控进程，而由主进程fork()出来的子进程则充当工作进程。
nginx也可以单进程模型执行，在这种进程模型下，主进程就是工作进程，没有监控进程。
Nginx的核心进程模型框图如下：
![](https://img-blog.csdn.net/20130704111632187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
master进程
监控进程充当整个进程组与用户的交互接口，同时对进程进行监护。它不需要处理网络事件，不负责业务的执行，只会通过管理worker进程来实现重启服务、平滑升级、更换日志文件、配置文件实时生效等功能。
master进程全貌图（来自阿里集团数据平台博客）：
![](https://img-blog.csdn.net/20130704111755078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
master进程中for(::)无限循环内有一个关键的sigsuspend()函数调用，该函数调用是的master进程的大部分时间都处于挂起状态，直到master进程收到信号为止。
master进程通过检查一下7个标志位来决定ngx_master_process_cycle方法的运行：
sig_atomic_t ngx_reap;
sig_atomic_t ngx_terminate;
sig_atomic_t ngx_quit;
sig_atomic_t
 ngx_reconfigure;
sig_atomic_t
 ngx_reopen;
sig_atomic_t
 ngx_change_binary;
sig_atomic_t
 ngx_noaccept;
进程中接收到的信号对Nginx框架的意义：
|信号|对应进程中的全局标志位变量|意义|
|----|----|----|
|QUIT|ngx_quit|优雅地关闭整个服务|
|TERM或INT|ngx_terminate|强制关闭整个服务|
|USR1|ngx_reopen|重新打开服务中的所有文件|
|WINCH|ngx_noaccept|所有子进程不再接受处理新的连接，实际相当于对所有子进程发送QUIT信号|
|USR2|ngx_change_binary|平滑升级到新版本的Nginx程序|
|HUP|ng_reconfigure|重读配置文件|
|CHLD|ngx_reap|有子进程以外结束，需要监控所有子进程|
还有一个标志位会用到：ngx_restart，它仅仅是在master工作流程中作为标志位使用，与信号无关。
核心代码（ngx_process_cycle.c）：
```cpp
void
ngx_master_process_cycle(ngx_cycle_t *cycle)
{
    char              *title;
    u_char            *p;
    size_t             size;
    ngx_int_t          i;
    ngx_uint_t         n, sigio;
    sigset_t           set;
    struct itimerval   itv;
    ngx_uint_t         live;
    ngx_msec_t         delay;
    ngx_listening_t   *ls;
    ngx_core_conf_t   *ccf;
    //信号处理设置工作
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGIO);
    sigaddset(&set, SIGINT);
    sigaddset(&set, ngx_signal_value(NGX_RECONFIGURE_SIGNAL));
    sigaddset(&set, ngx_signal_value(NGX_REOPEN_SIGNAL));
    sigaddset(&set, ngx_signal_value(NGX_NOACCEPT_SIGNAL));
    sigaddset(&set, ngx_signal_value(NGX_TERMINATE_SIGNAL));
    sigaddset(&set, ngx_signal_value(NGX_SHUTDOWN_SIGNAL));
    sigaddset(&set, ngx_signal_value(NGX_CHANGEBIN_SIGNAL));
    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      "sigprocmask() failed");
    }
    sigemptyset(&set);
    size = sizeof(master_process);
    for (i = 0; i < ngx_argc; i++) {
        size += ngx_strlen(ngx_argv[i]) + 1;
    }
    title = ngx_pnalloc(cycle->pool, size);
    p = ngx_cpymem(title, master_process, sizeof(master_process) - 1);
    for (i = 0; i < ngx_argc; i++) {
        *p++ = ' ';
        p = ngx_cpystrn(p, (u_char *) ngx_argv[i], size);
    }
    ngx_setproctitle(title);
    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);
    //其中包含了fork产生子进程的内容
    ngx_start_worker_processes(cycle, ccf->worker_processes,
                               NGX_PROCESS_RESPAWN);
    //Cache管理进程与cache加载进程的主流程
    ngx_start_cache_manager_processes(cycle, 0);
    ngx_new_binary = 0;
    delay = 0;
    sigio = 0;
    live = 1;
    for ( ;; ) {//循环
        if (delay) {
            if (ngx_sigalrm) {
                sigio = 0;
                delay *= 2;
                ngx_sigalrm = 0;
            }
            ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                           "termination cycle: %d", delay);
            itv.it_interval.tv_sec = 0;
            itv.it_interval.tv_usec = 0;
            itv.it_value.tv_sec = delay / 1000;
            itv.it_value.tv_usec = (delay % 1000 ) * 1000;
            if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
                ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                              "setitimer() failed");
            }
        }
        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "sigsuspend");
        sigsuspend(&set);//master进程休眠，等待接受信号被激活
        ngx_time_update();
        ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                       "wake up, sigio %i", sigio);
        //标志位为1表示需要监控所有子进程
        if (ngx_reap) {
            ngx_reap = 0;
            ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "reap children");
            live = ngx_reap_children(cycle);//管理子进程
        }
        //当live标志位为0（表示所有子进程已经退出）、ngx_terminate标志位为1或者ngx_quit标志位为1表示要退出master进程
        if (!live && (ngx_terminate || ngx_quit)) {
            ngx_master_process_exit(cycle);//退出master进程
        }
        //ngx_terminate标志位为1，强制关闭服务，发送TERM信号到所有子进程
        if (ngx_terminate) {
            if (delay == 0) {
                delay = 50;
            }
            if (sigio) {
                sigio--;
                continue;
            }
            sigio = ccf->worker_processes + 2 /* cache processes */;
            if (delay > 1000) {
                ngx_signal_worker_processes(cycle, SIGKILL);
            } else {
                ngx_signal_worker_processes(cycle,
                                       ngx_signal_value(NGX_TERMINATE_SIGNAL));
            }
            continue;
        }
        //ngx_quit标志位为1，优雅的关闭服务
        if (ngx_quit) {
            ngx_signal_worker_processes(cycle,
                                        ngx_signal_value(NGX_SHUTDOWN_SIGNAL));//向所有子进程发送quit信号
            ls = cycle->listening.elts;
            for (n = 0; n < cycle->listening.nelts; n++) {//关闭监听端口
                if (ngx_close_socket(ls[n].fd) == -1) {
                    ngx_log_error(NGX_LOG_EMERG, cycle->log, ngx_socket_errno,
                                  ngx_close_socket_n " %V failed",
                                  &ls[n].addr_text);
                }
            }
            cycle->listening.nelts = 0;
            continue;
        }
        //ngx_reconfigure标志位为1，重新读取配置文件
        //nginx不会让原来的worker子进程再重新读取配置文件，其策略是重新初始化ngx_cycle_t结构体，用它来读取新的额配置文件
        //再创建新的额worker子进程，销毁旧的worker子进程
        if (ngx_reconfigure) {
            ngx_reconfigure = 0;
            //ngx_new_binary标志位为1，平滑升级Nginx
            if (ngx_new_binary) {
                ngx_start_worker_processes(cycle, ccf->worker_processes,
                                           NGX_PROCESS_RESPAWN);
                ngx_start_cache_manager_processes(cycle, 0);
                ngx_noaccepting = 0;
                continue;
            }
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "reconfiguring");
            //初始化ngx_cycle_t结构体
            cycle = ngx_init_cycle(cycle);
            if (cycle == NULL) {
                cycle = (ngx_cycle_t *) ngx_cycle;
                continue;
            }
            ngx_cycle = cycle;
            ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx,
                                                   ngx_core_module);
            //创建新的worker子进程
            ngx_start_worker_processes(cycle, ccf->worker_processes,
                                       NGX_PROCESS_JUST_RESPAWN);
            ngx_start_cache_manager_processes(cycle, 1);
            /* allow new processes to start */
            ngx_msleep(100);
            live = 1;
            //向所有子进程发送QUIT信号
            ngx_signal_worker_processes(cycle,
                                        ngx_signal_value(NGX_SHUTDOWN_SIGNAL));
        }
        //ngx_restart标志位在ngx_noaccepting（表示正在停止接受新的连接）为1的时候被设置为1.
        //重启子进程
        if (ngx_restart) {
            ngx_restart = 0;
            ngx_start_worker_processes(cycle, ccf->worker_processes,
                                       NGX_PROCESS_RESPAWN);
            ngx_start_cache_manager_processes(cycle, 0);
            live = 1;
        }
        //ngx_reopen标志位为1，重新打开所有文件
        if (ngx_reopen) {
            ngx_reopen = 0;
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "reopening logs");
            ngx_reopen_files(cycle, ccf->user);
            ngx_signal_worker_processes(cycle,
                                        ngx_signal_value(NGX_REOPEN_SIGNAL));
        }
        //平滑升级Nginx
        if (ngx_change_binary) {
            ngx_change_binary = 0;
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "changing binary");
            ngx_new_binary = ngx_exec_new_binary(cycle, ngx_argv);
        }
        //ngx_noaccept为1，表示所有子进程不再处理新的连接
        if (ngx_noaccept) {
            ngx_noaccept = 0;
            ngx_noaccepting = 1;
            ngx_signal_worker_processes(cycle,
                                        ngx_signal_value(NGX_SHUTDOWN_SIGNAL));
        }
    }
}
```
ngx_start_worker_processes函数：
```cpp
static void
ngx_start_worker_processes(ngx_cycle_t *cycle, ngx_int_t n, ngx_int_t type)
{
    ngx_int_t      i;
    ngx_channel_t  ch;
    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "start worker processes");
    ch.command = NGX_CMD_OPEN_CHANNEL;
    //循环创建n个worker子进程
    for (i = 0; i < n; i++) {
        //完成fok新进程的具体工作
        ngx_spawn_process(cycle, ngx_worker_process_cycle,
                          (void *) (intptr_t) i, "worker process", type);
        //全局数组ngx_processes就是用来存储每个子进程的相关信息，如：pid，channel，进程做具体事情的接口指针等等，这些信息就是用结构体ngx_process_t来描述的。
        ch.pid = ngx_processes[ngx_process_slot].pid;
        ch.slot = ngx_process_slot;
        ch.fd = ngx_processes[ngx_process_slot].channel[0];
        /*在ngx_spawn_process创建好一个worker进程返回后，master进程就将worker进程的pid、worker进程在ngx_processes数组中的位置及channel[0]传递给前面已经创建好的worker进程，然后继续循环开始创建下一个worker进程。刚提到一个channel[0]，这里简单说明一下：channel就是一个能够存储2个整型元素的数组而已，这个channel数组就是用于socketpair函数创建一个进程间通道之用的。master和worker进程以及worker进程之间都可以通过这样的一个通道进行通信，这个通道就是在ngx_spawn_process函数中fork之前调用socketpair创建的。*/
        ngx_pass_open_channel(cycle, &ch);
    }
}
```
ngx_spawn_process函数：
```cpp
//参数解释：
//cycle：nginx框架所围绕的核心结构体
//proc：子进程中将要执行的工作循环
//data：参数
//name：子进程名字
ngx_pid_t
ngx_spawn_process(ngx_cycle_t *cycle, ngx_spawn_proc_pt proc, void *data,
    char *name, ngx_int_t respawn)
{
    u_long     on;
    ngx_pid_t  pid;
    ngx_int_t  s;
    if (respawn >= 0) {
        s = respawn;
    } else {
        for (s = 0; s < ngx_last_process; s++) {
            if (ngx_processes[s].pid == -1) {
                break;
            }
        }
        if (s == NGX_MAX_PROCESSES) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, 0,
                          "no more than %d processes can be spawned",
                          NGX_MAX_PROCESSES);
            return NGX_INVALID_PID;
        }
    }
    if (respawn != NGX_PROCESS_DETACHED) {
        /* Solaris 9 still has no AF_LOCAL */
        //创建父子进程间通信的套接字对（基于TCP）
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, ngx_processes[s].channel) == -1)
        {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          "socketpair() failed while spawning \"%s\"", name);
            return NGX_INVALID_PID;
        }
        ngx_log_debug2(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                       "channel %d:%d",
                       ngx_processes[s].channel[0],
                       ngx_processes[s].channel[1]);
        //设置为非阻塞模式
        if (ngx_nonblocking(ngx_processes[s].channel[0]) == -1) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_nonblocking_n " failed while spawning \"%s\"",
                          name);
            ngx_close_channel(ngx_processes[s].channel, cycle->log);
            return NGX_INVALID_PID;
        }
        if (ngx_nonblocking(ngx_processes[s].channel[1]) == -1) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          ngx_nonblocking_n " failed while spawning \"%s\"",
                          name);
            ngx_close_channel(ngx_processes[s].channel, cycle->log);
            return NGX_INVALID_PID;
        }
        on = 1;
        if (ioctl(ngx_processes[s].channel[0], FIOASYNC, &on) == -1) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          "ioctl(FIOASYNC) failed while spawning \"%s\"", name);
            ngx_close_channel(ngx_processes[s].channel, cycle->log);
            return NGX_INVALID_PID;
        }
        if (fcntl(ngx_processes[s].channel[0], F_SETOWN, ngx_pid) == -1) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          "fcntl(F_SETOWN) failed while spawning \"%s\"", name);
            ngx_close_channel(ngx_processes[s].channel, cycle->log);
            return NGX_INVALID_PID;
        }
        if (fcntl(ngx_processes[s].channel[0], F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) failed while spawning \"%s\"",
                           name);
            ngx_close_channel(ngx_processes[s].channel, cycle->log);
            return NGX_INVALID_PID;
        }
        if (fcntl(ngx_processes[s].channel[1], F_SETFD, FD_CLOEXEC) == -1) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          "fcntl(FD_CLOEXEC) failed while spawning \"%s\"",
                           name);
            ngx_close_channel(ngx_processes[s].channel, cycle->log);
            return NGX_INVALID_PID;
        }
        ngx_channel = ngx_processes[s].channel[1];
    } else {
        ngx_processes[s].channel[0] = -1;
        ngx_processes[s].channel[1] = -1;
    }
    ngx_process_slot = s;
    //创建子进程
    pid = fork();
    switch (pid) {
    case -1:
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      "fork() failed while spawning \"%s\"", name);
        ngx_close_channel(ngx_processes[s].channel, cycle->log);
        return NGX_INVALID_PID;
    case 0:
        ngx_pid = ngx_getpid();
        proc(cycle, data);
        break;
    default:
        break;
    }
    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "start %s %P", name, pid);
    ngx_processes[s].pid = pid;
    ngx_processes[s].exited = 0;
    if (respawn >= 0) {
        return pid;
    }
    ngx_processes[s].proc = proc;
    ngx_processes[s].data = data;
    ngx_processes[s].name = name;
    ngx_processes[s].exiting = 0;
    switch (respawn) {
    case NGX_PROCESS_NORESPAWN:
        ngx_processes[s].respawn = 0;
        ngx_processes[s].just_spawn = 0;
        ngx_processes[s].detached = 0;
        break;
    case NGX_PROCESS_JUST_SPAWN:
        ngx_processes[s].respawn = 0;
        ngx_processes[s].just_spawn = 1;
        ngx_processes[s].detached = 0;
        break;
    case NGX_PROCESS_RESPAWN:
        ngx_processes[s].respawn = 1;
        ngx_processes[s].just_spawn = 0;
        ngx_processes[s].detached = 0;
        break;
    case NGX_PROCESS_JUST_RESPAWN:
        ngx_processes[s].respawn = 1;
        ngx_processes[s].just_spawn = 1;
        ngx_processes[s].detached = 0;
        break;
    case NGX_PROCESS_DETACHED:
        ngx_processes[s].respawn = 0;
        ngx_processes[s].just_spawn = 0;
        ngx_processes[s].detached = 1;
        break;
    }
    if (s == ngx_last_process) {
        ngx_last_process++;
    }
    return pid;
}
```
worker进程
worker进程的主要任务是完成具体的任务逻辑。其主要关注点是与客户端或后端真实服务器（此时nginx作为中间代理）之间的数据可读/可写等I/O交互事件，所以工作进程的阻塞点是在像select()、epoll_wait()等这样的I/O多路复用函数调用处，以等待发生数据可读/写事件。当然也可能被新收到的进程信号中断。
master进程如何通通知worker进程去做某些工作呢？采用的是信号。
当收到信号时，信号处理函数ngx_signal_handler()就会执行。
对于worker进程的工作方法ngx_worker_process_cycle来说，它主要关注4个全局标志位：
sig_atomic_t ngx_terminate;//强制关闭进程
sig_atomic_t ngx_quit;//优雅地关闭进程（有唯一一段代码会设置它，就是接受到QUIT信号。ngx_quit只有在首次设置为1,时，才会将ngx_exiting置为1）
ngx_uint_t ngx_exiting;//退出进程标志位
sig_atomic_t ngx_reopen;//重新打开所有文件
其中ngx_terminate、ngx_quit 、ngx_reopen都将由ngx_signal_handler根据接受到的信号来设置。ngx_exiting标志位仅由ngx_worker_cycle方法在退出时作为标志位使用。
核心代码（ngx_process_cycle.c）：
```cpp
static void
ngx_worker_process_cycle(ngx_cycle_t *cycle, void *data)
{
    ngx_int_t worker = (intptr_t) data;
    ngx_uint_t         i;
    ngx_connection_t  *c;
    ngx_process = NGX_PROCESS_WORKER;
    //子进程初始化
    ngx_worker_process_init(cycle, worker);
    ngx_setproctitle("worker process");
//这里有一段多线程条件下的代码。由于nginx并不支持多线程，因此删除掉了
    //循环
    for ( ;; ) {
        
        //ngx_exiting标志位为1，进程退出
        if (ngx_exiting) {
            c = cycle->connections;
            for (i = 0; i < cycle->connection_n; i++) {
                if (c[i].fd != -1 && c[i].idle) {
                    c[i].close = 1;
                    c[i].read->handler(c[i].read);
                }
            }
            if (ngx_event_timer_rbtree.root == ngx_event_timer_rbtree.sentinel)
            {
                ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "exiting");
                ngx_worker_process_exit(cycle);
            }
        }
        ngx_log_debug0(NGX_LOG_DEBUG_EVENT, cycle->log, 0, "worker cycle");
        ngx_process_events_and_timers(cycle);//处理事件的方法
        //强制结束进程
        if (ngx_terminate) {
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "exiting");
            ngx_worker_process_exit(cycle);
        }
        //优雅地退出进程
        if (ngx_quit) {
            ngx_quit = 0;
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                          "gracefully shutting down");
            ngx_setproctitle("worker process is shutting down");
            if (!ngx_exiting) {
                ngx_close_listening_sockets(cycle);
                //设置ngx_exiting 标志位
                ngx_exiting = 1;
            }
        }
        //重新打开所有文件
        if (ngx_reopen) {
            ngx_reopen = 0;
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "reopening logs");
            ngx_reopen_files(cycle, -1);
        }
    }
}
```
参考资料：
《深入理解Nginx》-陶辉
[阿里集团数据平台博客](http://www.alidata.org/archives/1174)
[阿里集团数据平台博客2](http://www.alidata.org/archives/1188)
[http://blog.csdn.net/lengzijian/article/details/7589998](http://blog.csdn.net/lengzijian/article/details/7589998)
