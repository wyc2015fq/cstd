# nginx进程模型 - 星爷2401 - CSDN博客
2017年05月26日 14:55:11[星爷2401](https://me.csdn.net/leifukes)阅读数：195
个人分类：[nginx](https://blog.csdn.net/leifukes/article/category/6552866)
```cpp
```
## 一、介绍
> 
nginx的核心进程采用的是master-worker进程。一个master进程（监控进程）和多个worker进程（工作进程）。
master进程：
> 
> 
1、fork出子进程（工作进程）。
2、接受来自外界的信号。
3、监控worker进程的工作状态，向worker进程发送信号，控制worker的运行状态。
4、当worker因某种原因退出时，及时启动worker进程。
5、监听端口，把监听句柄交给worker进程。
woker进程：
> 
> 
1、accept新的连接。
2、接收并处理数据。
3、接收来自master的进程控制。
## 二、进程控制（热重启）
> 
      nginx通过nginx -s 命令去控制进程的运行，nginx -s 是向master主进程发送命令，然后master主进程去控制子进程。nginx执行命令的
时候，会获取到nginx的安装目录下的 nginx/logs/nginx.pid 文件，里面保存着master进程ID，然后通过kill 向 master发送进程控制命令。
具体的代码如下：
#### 1、执行main函数（src/core/nginx.c）
int ngx_cdecl
main(int argc, char *const *argv)
{
    ...
    //1、ngx_get_options获取命令参数并解析
    if (ngx_get_options(argc, argv) != NGX_OK) {
        return 1;
    }
    ...
    //2、执行信号处理函数ngx_signal_process，ngx_signal会在ngx_get_options赋值
    if (ngx_signal) {
        return ngx_signal_process(cycle, ngx_signal); //直接执行然后return
    }
    ...
}
//ngx_get_options获取并解析命令
static ngx_int_t
ngx_get_options(int argc, char *const *argv)
{   
    ...
    case 's':
        //ngx_signal 赋值
        if (*p) {
            ngx_signal = (char *) p;
        } else if (argv[++i]) {
            ngx_signal = argv[i];
        } else {
            ngx_log_stderr(0, "option \"-s\" requires parameter");
            return NGX_ERROR;
        }
        //命令字符串对比
        if (ngx_strcmp(ngx_signal, "stop") == 0
            || ngx_strcmp(ngx_signal, "quit") == 0
            || ngx_strcmp(ngx_signal, "reopen") == 0
            || ngx_strcmp(ngx_signal, "reload") == 0)
        {
            ngx_process = NGX_PROCESS_SIGNALLER; 
            //NGX_PROCESS_SIGNALLER宏定义为2，作用是为了标示nginx不是第一次启动而是nginx已经启动了，
            //有些操作没有必须要重复去做，主要在ngx_cycle_init初始化的时候使用
            goto next;
        }
        ...
}
#### 2、执行ngx_signal_process()，主要实现读取当前nginx的master进程pid。然后发送命令。
```cpp
ngx_int_t
ngx_signal_process(ngx_cycle_t *cycle, char *sig)
{
    ...
    //读取log文件中的nginx.pid
    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);
    ngx_memzero(&file, sizeof(ngx_file_t));
    file.name = ccf->pid;
    file.log = cycle->log;
    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);
    if (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        return 1;
    }
    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);//读取nginx.pid文件
    if (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }
    if (n == NGX_ERROR) {
        return 1;
    }
    while (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }
    pid = ngx_atoi(buf, ++n); //获取进程ID
    if (pid == NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.data);
        return 1;
    }
    //执行系统信号处理
    return ngx_os_signal_process(cycle, sig, pid);
}
```
3、然后执行 ngx_os_signal_process(cycle, sig, pid)，通过kill 发送命令
```cpp
ngx_int_t
ngx_os_signal_process(ngx_cycle_t *cycle, char *name, ngx_int_t pid)
{
    ngx_signal_t  *sig;
    for (sig = signals; sig->signo != 0; sig++) { //遍历nginx命令与进程控制命令关系表
        if (ngx_strcmp(name, sig->name) == 0) {
            if (kill(pid, sig->signo) != -1) {  //发给master命令。signals在文件中有定义如下
                return 0;
            }
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          "kill(%P, %d) failed", pid, sig->signo);
        }
    }
    return 1;
}
//signals
ngx_signal_t  signals[] = {
    { ngx_signal_value(NGX_RECONFIGURE_SIGNAL),
      "SIG" ngx_value(NGX_RECONFIGURE_SIGNAL),
      "reload",
      ngx_signal_handler },
    { ngx_signal_value(NGX_REOPEN_SIGNAL),
      "SIG" ngx_value(NGX_REOPEN_SIGNAL),
      "reopen",
      ngx_signal_handler },
    { ngx_signal_value(NGX_NOACCEPT_SIGNAL),
      "SIG" ngx_value(NGX_NOACCEPT_SIGNAL),
      "",
      ngx_signal_handler },
    { ngx_signal_value(NGX_TERMINATE_SIGNAL),
      "SIG" ngx_value(NGX_TERMINATE_SIGNAL),
      "stop",
      ngx_signal_handler },
    { ngx_signal_value(NGX_SHUTDOWN_SIGNAL),
      "SIG" ngx_value(NGX_SHUTDOWN_SIGNAL),
      "quit",
      ngx_signal_handler },
    { ngx_signal_value(NGX_CHANGEBIN_SIGNAL),
      "SIG" ngx_value(NGX_CHANGEBIN_SIGNAL),
      "",
      ngx_signal_handler },
    { SIGALRM, "SIGALRM", "", ngx_signal_handler },
    { SIGINT, "SIGINT", "", ngx_signal_handler },
    { SIGIO, "SIGIO", "", ngx_signal_handler },
    { SIGCHLD, "SIGCHLD", "", ngx_signal_handler },
    { SIGSYS, "SIGSYS, SIG_IGN", "", SIG_IGN },
    { SIGPIPE, "SIGPIPE, SIG_IGN", "", SIG_IGN },
    { 0, NULL, "", NULL }
};
```
#### **至此执行nginx -s 的命令通过以上逻辑就发给了master主进程。下面看一下master主进程如何处理接受到的命令的。**
```cpp
void
ngx_master_process_cycle(ngx_cycle_t *cycle)
{
    ...
    //加入信号命令集
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
    ...
    //死循环等待信号
     for ( ;; ) {
        ...
        //阻塞等待信号,接收到信号之后会执行绑定好的ngx_singal_hander函数
        sigsuspend(&set);
        if (ngx_terminate) {...} //nginx -s stop
        if (ngx_quit) {...} //nginx -s quit
        if (ngx_restart) {...} //nginx -s restart
        if (ngx_reconfigure) {...} //nginx -s reload
        ...
    }
}
```
**master如何控制woker进程**
master是通过管道单向给woker发送数据的。
### **1、先看一下channel的结构体**
```cpp
typedef struct {  
     ngx_uint_t  command;  //发给worker的具体命令
     ngx_pid_t   pid;  //woker的进程pid
     ngx_int_t   slot; //woker在ngx_process数组的下标，所有的进程都会保存到ngx_process数组中。
     ngx_fd_t    fd;  //文件描述，master有可能会把文件描述符告诉worker，让worker操作，先大致了解。
} ngx_channel_t;
```
### 2、master如何发送channel
```cpp
static void
ngx_signal_worker_processes(ngx_cycle_t *cycle, int signo)
{
    //定义channel结构体
    ngx_channel_t  ch;
    ch.command = 0;
    switch (signo) {
    case ngx_signal_value(NGX_SHUTDOWN_SIGNAL):
        ch.command = NGX_CMD_QUIT;
        break;
    case ngx_signal_value(NGX_TERMINATE_SIGNAL):
        ch.command = NGX_CMD_TERMINATE;
        break;
    case ngx_signal_value(NGX_REOPEN_SIGNAL):
        ch.command = NGX_CMD_REOPEN;
        break;
    default:
        ch.command = 0;
    }
    ...
    if (ch.command) {
        //发送命令 ngx_write_channel 会调用sendMsg发送数据
        if (ngx_write_channel(ngx_processes[i].channel[0],
                                      &ch, sizeof(ngx_channel_t), cycle->log)
            == NGX_OK)
        {
            if (signo != ngx_signal_value(NGX_REOPEN_SIGNAL)) {
                ngx_processes[i].exiting = 1;
            }
            continue;
        }
    }
    ...
}
```
### 3、worker如何接收channel
worker会把channel加入epoll中，当有channel来的时候，会触发ngx_channel_handler函数。
执行ngx_worker_process_init() 通过ngx_add_channel_event()把事件加入epoll，并绑定事件处理函数ngx_channel_handler。
ngx_channel_handler函数负责将一些变量置位，在woker的for(;;)中去读变量。
```cpp
static void
ngx_worker_process_init(ngx_cycle_t *cycle, ngx_int_t worker)
{
    ...
    //加入epoll
    if (ngx_add_channel_event(cycle, ngx_channel, NGX_READ_EVENT,
                              ngx_channel_handler)
        == NGX_ERROR)
    {
            exit(2);
    }
    ...
}
static void
ngx_channel_handler(ngx_event_t *ev)
{
    ...
    //置位相应的标志位
    switch (ch.command) {
    case NGX_CMD_QUIT:
        ngx_quit = 1;
        break;
    case NGX_CMD_TERMINATE:
        ngx_terminate = 1;
        break;
    case NGX_CMD_REOPEN:
        ngx_reopen = 1;
        break;
    ...
}
static void
ngx_worker_process_cycle(ngx_cycle_t *cycle, void *data)
{
    ...
    //worker主循环,思路同master基本一样
    for (;;)
    {
        if (ngx_terminate) {...} //nginx -s stop
        if (ngx_quit) {...} //nginx -s quit
        if (ngx_restart) {...} //nginx -s restart
        if (ngx_reconfigure) {...} //nginx -s reload
    }
}
```
**至此从发送一个nginx -s 命令后，nginx的master进程和worker进程都做了什么事情，以及master如何控制woker，大致原理就这些。后续如果有需要热重启的或者采用master-woker进程的，可以参考nginx的进程模型。**
#### 三、惊群现象（worker）
> 
> 
> 
> 
> 
> 
> 
> 
> 
> 
> 
> 
> 
> 
