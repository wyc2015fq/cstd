# Nginx源码分析 - 主流程篇 - 平滑重启和信号控制 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年07月06日 18:56:36[initphp](https://me.csdn.net/initphp)阅读数：3306
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









### 平滑重启

上一篇文章我们分析了Nginx的启动流程。其中**ngx_add_inherited_sockets**主要用于继承Socket文件句柄。

Nginx有平滑重启的功能，通过平滑重启，可以让用户无感知并且不中断。



```cpp
#平滑重启  
sudo kill -HUP `cat /usr/local/nginx-1.4.7/nginx.pid`
```


Nginx支持热切换，为了保证切换之后的套接字不丢失，就需要Nginx继承原先的socket（用户正在连接Nginx的时候的文件句柄）。

**ngx_add_inherited_sockets**正是做了这个工作



**套接字会放在“NGINX”的全局环境变量中**。

继承后的socket文件句柄放到**ngx_cycle.listening数组**中。




```cpp
/* 初始化socket端口监听，例如打开80端口监听；
     * Nginx支持热切换，为了保证切换之后的套接字不丢失，
     * 所以需要采用这一步添加继承的Socket套接字，套接字会放在NGINX的全局环境变量中*/
    if (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        return 1;
    }
```

NGINX宏变量的值，值为客户端的socket fd句柄：




```cpp
NGINX="16000:16500:16600;"
```

具体函数实现：




```cpp
/**
 * Nginx支持热切换，为了保证切换之后的套接字不丢失，所以需要采用这一步添加继承的Socket套接字，套接字会放在NGINX的全局环境变量中
 * 初始化继承的sockets
 * 函数通过环境变量NGINX完成socket的继承，继承来的socket将会放到init_cycle的listening数组中。
 * 在NGINX环境变量中，每个socket中间用冒号或分号隔开。完成继承同时设置全局变量ngx_inherited为1。
 */
static ngx_int_t
ngx_add_inherited_sockets(ngx_cycle_t *cycle)
{
    u_char           *p, *v, *inherited;
    ngx_int_t         s;
    ngx_listening_t  *ls;

    /* 获取宏环境变量NGINX的值    例如：# export NGINX="16000:16500:16600;" */
    inherited = (u_char *) getenv(NGINX_VAR);

    if (inherited == NULL) {
        return NGX_OK;
    }

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0,
                  "using inherited sockets from \"%s\"", inherited);

    /* 初始化ngx_cycle.listening数组，并且数组中包含10个元素 */
    if (ngx_array_init(&cycle->listening, cycle->pool, 10,
                       sizeof(ngx_listening_t))
        != NGX_OK)
    {
        return NGX_ERROR;
    }

    for (p = inherited, v = p; *p; p++) {
        if (*p == ':' || *p == ';') {
            s = ngx_atoi(v, p - v);
            if (s == NGX_ERROR) {
                ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                              "invalid socket number \"%s\" in " NGINX_VAR
                              " environment variable, ignoring the rest"
                              " of the variable", v);
                break;
            }

            v = p + 1;

            ls = ngx_array_push(&cycle->listening);
            if (ls == NULL) {
                return NGX_ERROR;
            }

            ngx_memzero(ls, sizeof(ngx_listening_t));

            /* 将fd保存到ngx_listening_t结构数组上 */
            ls->fd = (ngx_socket_t) s;
        }
    }

    if (v != p) {
        ngx_log_error(NGX_LOG_EMERG, cycle->log, 0,
                      "invalid socket number \"%s\" in " NGINX_VAR
                      " environment variable, ignoring", v);
    }

    /* 已经初始化要继承的socket */
    ngx_inherited = 1;

    return ngx_set_inherited_sockets(cycle);
}
```


### 信号处理

当我们使用./nginx -s stop|reload|quit  类似这样的命令的时候，就是给Nginx发送了一个信号。

信号可以控制Nginx进程的重启、停止、退出等操作。**通过获取已经启动的Nginx的进程PID，对进程发送信号操作。**

Nginx的进程ID pid一般会放置到/usr/local/nginx-1.4.7/nginx.pid。



```cpp
/* 处理信号；例如./nginx -s stop,则处理Nginx的停止信号 */
    if (ngx_signal) {
        return ngx_signal_process(cycle, ngx_signal);
    }
```

**ngx_signal**变量是在**ngx_get_options**方法中。ngx_signal主要用于标识是否是属于信号操作。例如：./nginx -s stop|reload|quit




```cpp
/* ngx_signal 是否有信号  ./nginx -s stop|reload|quit */
            case 's':
                if (*p) {
                    ngx_signal = (char *) p;

                } else if (argv[++i]) {
                    ngx_signal = argv[i];

                } else {
                    ngx_log_stderr(0, "option \"-s\" requires parameter");
                    return NGX_ERROR;
                }

                if (ngx_strcmp(ngx_signal, "stop") == 0
                    || ngx_strcmp(ngx_signal, "quit") == 0
                    || ngx_strcmp(ngx_signal, "reopen") == 0
                    || ngx_strcmp(ngx_signal, "reload") == 0)
                {
                    ngx_process = NGX_PROCESS_SIGNALLER;
                    goto next;
                }

                ngx_log_stderr(0, "invalid option: \"-s %s\"", ngx_signal);
                return NGX_ERROR;
```

然后再看下ngx_signal_process的具体操作：




```cpp
/**
 * 处理信号；
 * 例如./nginx -s stop,则处理Nginx的停止信号
 */
ngx_int_t
ngx_signal_process(ngx_cycle_t *cycle, char *sig)
{
    ssize_t           n;
    ngx_pid_t         pid;
    ngx_file_t        file;
    ngx_core_conf_t  *ccf;
    u_char            buf[NGX_INT64_LEN + 2];

    ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "signal process started");

    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);

    ngx_memzero(&file, sizeof(ngx_file_t));

    file.name = ccf->pid;
    file.log = cycle->log;

    /* 通过/usr/local/nginx/logs/nginx.pid 获取进行ID号 */
    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, NGX_FILE_DEFAULT_ACCESS);

    if (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        return 1;
    }

    n = ngx_read_file(&file, buf, NGX_INT64_LEN + 2, 0);

    if (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    if (n == NGX_ERROR) {
        return 1;
    }

    while (n-- && (buf[n] == CR || buf[n] == LF)) { /* void */ }

    /* 最终得到PID */
    pid = ngx_atoi(buf, ++n);

    if (pid == (ngx_pid_t) NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, cycle->log, 0,
                      "invalid PID number \"%*s\" in \"%s\"",
                      n, buf, file.name.data);
        return 1;
    }

    /* 调用ngx_os_signal_process方法，处理真正的信号 */
    return ngx_os_signal_process(cycle, sig, pid);

}
```


```cpp
/**
 * 处理信号
 */
ngx_int_t
ngx_os_signal_process(ngx_cycle_t *cycle, char *name, ngx_pid_t pid)
{
    ngx_signal_t  *sig;

    for (sig = signals; sig->signo != 0; sig++) {
        if (ngx_strcmp(name, sig->name) == 0) {

        	/* 通过系统调用向该进程发送信号 */
            if (kill(pid, sig->signo) != -1) {
                return 0;
            }

            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          "kill(%P, %d) failed", pid, sig->signo);
        }
    }

    return 1;
}
```












