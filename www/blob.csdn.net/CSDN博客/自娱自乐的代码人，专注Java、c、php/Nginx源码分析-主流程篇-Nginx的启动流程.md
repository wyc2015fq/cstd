# Nginx源码分析 - 主流程篇 - Nginx的启动流程 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年07月05日 15:46:12[initphp](https://me.csdn.net/initphp)阅读数：3940
个人分类：[Nginx源码分析](https://blog.csdn.net/initphp/article/category/6081681)

所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









前几篇主要介绍了Nginx比较常用的一些基础数据结构，例如pool,buf,array,list等。通过对Nginx基础数据结构的理解，能更好的帮助我们读懂整个Nginx的源代码。

这一章节开始主要分析Nginx的主流程。

Nginx的主流程的实现函数在./src/core/nginx.c文件中。**通过main()函数，我们可以窥探整个Nginx启动的流程。**




### Nginx的启动过程

main()函数的启动过程如下：
- 调用ngx_get_options方法，主要用于解析命令行中的参数，例如：./nginx -s stop|start|restart
- 调用ngx_time_init方法，初始化并更新时间，如全局变量ngx_cached_time
- 调用ngx_getpid方法，获取当前进程的pid。一般pid会放在/usr/local/nginx-1.4.7/nginx.pid的文件中，用于发送重启，关闭等信号命令。
- 调用ngx_log_init方法，初始化日志，并得到日志的文件句柄ngx_log_file.fd
- 初始化init_cycle Nginx的全局变量。在内存池上创建一个默认大小1024的全局变量。这里只是最简单的初始化一个变量。
- 调用ngx_save_argv方法，保存Nginx命令行中的参数和变量,放到全局变量ngx_argv
- 调用ngx_process_options方法，将ngx_get_options中获得这些参数取值赋值到ngx_cycle中。prefix, conf_prefix, conf_file, conf_param等字段。
- 调用ngx_os_init()初始化系统相关变量，如内存页面大小ngx_pagesize,ngx_cacheline_size,最大连接数ngx_max_sockets等
- 调用ngx_crc32_table_init方法，初始化一致性hash表，主要作用是加快查询
- 调用ngx_add_inherited_sockets方法，ngx_add_inherited_sockets主要是继承了socket的套接字。主要作用是热启动的时候需要平滑过渡
- 调用ngx_preinit_modules方法，主要是前置的初始化模块，对模块进行编号处理
- 调用ngx_init_cycle方法，完成全局变量cycle的初始化
- 调用ngx_signal_process方法，如果有信号，则进入ngx_signal_process方法。例如：例如./nginx -s stop,则处理Nginx的停止信号
- 调用ngx_get_conf方法，得到核心模块ngx_core_conf_t的配置文件指针
- 调用ngx_create_pidfile方法，创建pid文件。例如：/usr/local/nginx-1.4.7/nginx.pid
- 调用ngx_master_process_cycle方法，这函数里面开始真正创建多个Nginx的子进程。**这个方法包括子进程创建、事件监听、各种模块运行等都会包含进去**

![](https://img-blog.csdn.net/20160705191404703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




### 重要流程分析

#### ngx_get_options 解析外部参数

ngx_get_options方法主要用于解析命令行外部参数。例如：./nginx -s stop|start|restart



```cpp
/* 解析外部参数 */
    if (ngx_get_options(argc, argv) != NGX_OK) {
        return 1;
    }
```


```cpp
/**
 * 解析启动命令行中的参数
 * ./nginx -s stop|start|restart
 */
static ngx_int_t
ngx_get_options(int argc, char *const *argv)
{
    u_char     *p;
    ngx_int_t   i;

    for (i = 1; i < argc; i++) {

        p = (u_char *) argv[i];

        if (*p++ != '-') {
            ngx_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            return NGX_ERROR;
        }

        while (*p) {

            switch (*p++) {

            case '?':
            case 'h':
                ngx_show_version = 1;
                ngx_show_help = 1;
                break;

            case 'v':
                ngx_show_version = 1;
                break;

            case 'V':
                ngx_show_version = 1;
                ngx_show_configure = 1;
                break;

            case 't':
                ngx_test_config = 1;
                break;

            case 'T':
                ngx_test_config = 1;
                ngx_dump_config = 1;
                break;

            case 'q':
                ngx_quiet_mode = 1;
                break;

            case 'p':
                if (*p) {
                    ngx_prefix = p;
                    goto next;
                }

                if (argv[++i]) {
                    ngx_prefix = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-p\" requires directory name");
                return NGX_ERROR;

            case 'c':
                if (*p) {
                    ngx_conf_file = p;
                    goto next;
                }

                if (argv[++i]) {
                    ngx_conf_file = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-c\" requires file name");
                return NGX_ERROR;

            case 'g':
                if (*p) {
                    ngx_conf_params = p;
                    goto next;
                }

                if (argv[++i]) {
                    ngx_conf_params = (u_char *) argv[i];
                    goto next;
                }

                ngx_log_stderr(0, "option \"-g\" requires parameter");
                return NGX_ERROR;

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

            default:
                ngx_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                return NGX_ERROR;
            }
        }

    next:

        continue;
    }

    return NGX_OK;
}
```


#### init_cycle 初始化全局变量

初始化init_cycle Nginx的全局变量。在内存池上创建一个默认大小1024的全局变量。这里只是最简单的初始化一个变量。



```cpp
/*
     * init_cycle->log is required for signal handlers and
     * ngx_process_options()
     */
    /* 初始化Nginx的init_cycle */
    ngx_memzero(&init_cycle, sizeof(ngx_cycle_t));
    init_cycle.log = log;
    ngx_cycle = &init_cycle;

    /* 创建内存池 默认大小1024*/
    init_cycle.pool = ngx_create_pool(1024, log);
    if (init_cycle.pool == NULL) {
        return 1;
    }
```


真正的初始化在**ngx_init_cycle**这个函数中。ngx_init_cycle包含了Nginx的全局变量的全部初始化过程，后面会单独开一篇文章讲解。





```cpp
/* 完成cycle的初始化工作 */
    cycle = ngx_init_cycle(&init_cycle);
    if (cycle == NULL) {
        if (ngx_test_config) {
            ngx_log_stderr(0, "configuration file %s test failed",
                           init_cycle.conf_file.data);
        }

        return 1;
    }
```




#### 变量保存方法ngx_save_argv和ngx_process_options

ngx_save_argv：保存Nginx命令行中的参数和变量,放到全局变量ngx_argv

ngx_process_options：将ngx_get_options中获得这些参数取值赋值到ngx_cycle中。**prefix, conf_prefix, conf_file, conf_param等字段。**



```cpp
/* 保存Nginx命令行中的参数和变量,放到全局变量ngx_argv */
    if (ngx_save_argv(&init_cycle, argc, argv) != NGX_OK) {
        return 1;
    }

    /* 将ngx_get_options中获得这些参数取值赋值到ngx_cycle中 */
    if (ngx_process_options(&init_cycle) != NGX_OK) {
        return 1;
    }
```
头部的全局变量定义





```cpp
/**
 * 定义全局变量参数
 */
static ngx_uint_t   ngx_show_help; //是否显示帮助信息
static ngx_uint_t   ngx_show_version; //是否显示版本号
static ngx_uint_t   ngx_show_configure; //是否显示配置信息
static u_char      *ngx_prefix; //Nginx的工作目录
static u_char      *ngx_conf_file; //全局配置文件目录地址
static u_char      *ngx_conf_params; //配置参数
static char        *ngx_signal; //信号
```

```cpp
int              ngx_argc; //命令行参数个数
char           **ngx_argv; //命令行参数
char           **ngx_os_argv;
```

```cpp
/**
 * 保存Nginx命令行中的参数和变量
 * 放到全局变量ngx_argv
 */
static ngx_int_t
ngx_save_argv(ngx_cycle_t *cycle, int argc, char *const *argv)
{
#if (NGX_FREEBSD)

    ngx_os_argv = (char **) argv;
    ngx_argc = argc;
    ngx_argv = (char **) argv;

#else
    size_t     len;
    ngx_int_t  i;

    ngx_os_argv = (char **) argv;
    ngx_argc = argc;

    ngx_argv = ngx_alloc((argc + 1) * sizeof(char *), cycle->log);
    if (ngx_argv == NULL) {
        return NGX_ERROR;
    }

    for (i = 0; i < argc; i++) {
        len = ngx_strlen(argv[i]) + 1;

        ngx_argv[i] = ngx_alloc(len, cycle->log);
        if (ngx_argv[i] == NULL) {
            return NGX_ERROR;
        }

        (void) ngx_cpystrn((u_char *) ngx_argv[i], (u_char *) argv[i], len);
    }

    ngx_argv[i] = NULL;

#endif

    ngx_os_environ = environ;

    return NGX_OK;
}
```

```cpp
/**
 * 将ngx_get_options中获得这些参数取值赋值到ngx_cycle中
 */
static ngx_int_t
ngx_process_options(ngx_cycle_t *cycle)
{
    u_char  *p;
    size_t   len;

    /* Nginx工作目录 */
    if (ngx_prefix) {
        len = ngx_strlen(ngx_prefix);
        p = ngx_prefix;

        if (len && !ngx_path_separator(p[len - 1])) {
            p = ngx_pnalloc(cycle->pool, len + 1);
            if (p == NULL) {
                return NGX_ERROR;
            }

            ngx_memcpy(p, ngx_prefix, len);
            p[len++] = '/';
        }

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

    } else {

#ifndef NGX_PREFIX

        p = ngx_pnalloc(cycle->pool, NGX_MAX_PATH);
        if (p == NULL) {
            return NGX_ERROR;
        }

        if (ngx_getcwd(p, NGX_MAX_PATH) == 0) {
            ngx_log_stderr(ngx_errno, "[emerg]: " ngx_getcwd_n " failed");
            return NGX_ERROR;
        }

        len = ngx_strlen(p);

        p[len++] = '/';

        cycle->conf_prefix.len = len;
        cycle->conf_prefix.data = p;
        cycle->prefix.len = len;
        cycle->prefix.data = p;

#else

#ifdef NGX_CONF_PREFIX
        ngx_str_set(&cycle->conf_prefix, NGX_CONF_PREFIX);
#else
        ngx_str_set(&cycle->conf_prefix, NGX_PREFIX);
#endif
        ngx_str_set(&cycle->prefix, NGX_PREFIX);

#endif
    }

    /* 配置文件目录 */
    if (ngx_conf_file) {
        cycle->conf_file.len = ngx_strlen(ngx_conf_file);
        cycle->conf_file.data = ngx_conf_file;

    } else {
        ngx_str_set(&cycle->conf_file, NGX_CONF_PATH);
    }

    if (ngx_conf_full_name(cycle, &cycle->conf_file, 0) != NGX_OK) {
        return NGX_ERROR;
    }

    for (p = cycle->conf_file.data + cycle->conf_file.len - 1;
         p > cycle->conf_file.data;
         p--)
    {
        if (ngx_path_separator(*p)) {
            cycle->conf_prefix.len = p - ngx_cycle->conf_file.data + 1;
            cycle->conf_prefix.data = ngx_cycle->conf_file.data;
            break;
        }
    }

    /* 配置参数 */
    if (ngx_conf_params) {
        cycle->conf_param.len = ngx_strlen(ngx_conf_params);
        cycle->conf_param.data = ngx_conf_params;
    }

    if (ngx_test_config) {
        cycle->log->log_level = NGX_LOG_INFO;
    }

    return NGX_OK;
}
```




#### 给模块打标ngx_preinit_modules

ngx_preinit_modules方法主要初始化所有模块；并对所有模块进行编号处理；



```cpp
/* 初始化所有模块；并对所有模块进行编号处理；
     * ngx_modules数却是在自动编译的时候生成的，位于objs/ngx_modules.c文件中   */
    if (ngx_preinit_modules() != NGX_OK) {
        return 1;
    }
```


ngx_module.c





```cpp
/**
 * 初始化所有模块；并对所有模块进行编号处理；
 */
ngx_int_t
ngx_preinit_modules(void)
{
    ngx_uint_t  i;

    for (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    return NGX_OK;
}
```




#### 创建PID文件ngx_create_pidfile

ngx_create_pidfile创建PID文件



```cpp
/* 创建PID文件 */
    if (ngx_create_pidfile(&ccf->pid, cycle->log) != NGX_OK) {
        return 1;
    }
```
ngx_cycle.c





```cpp
/**
 * 创建PID的文件
 */
ngx_int_t
ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log)
{
    size_t      len;
    ngx_uint_t  create;
    ngx_file_t  file;
    u_char      pid[NGX_INT64_LEN + 2];

    if (ngx_process > NGX_PROCESS_MASTER) {
        return NGX_OK;
    }

    ngx_memzero(&file, sizeof(ngx_file_t));

    file.name = *name;
    file.log = log;

    create = ngx_test_config ? NGX_FILE_CREATE_OR_OPEN : NGX_FILE_TRUNCATE;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDWR,
                            create, NGX_FILE_DEFAULT_ACCESS);

    if (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                      ngx_open_file_n " \"%s\" failed", file.name.data);
        return NGX_ERROR;
    }

    if (!ngx_test_config) {
        len = ngx_snprintf(pid, NGX_INT64_LEN + 2, "%P%N", ngx_pid) - pid;

        if (ngx_write_file(&file, pid, len, 0) == NGX_ERROR) {
            return NGX_ERROR;
        }
    }

    if (ngx_close_file(file.fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", file.name.data);
    }

    return NGX_OK;
}
```


#### 其它重要模块

下面的模块都会**新开文章**具体详细解析：


ngx_add_inherited_sockets 继承Socket文件句柄

ngx_init_cycle 全局配置文件cycle详解

ngx_signal_process 信号的处理机制

ngx_master_process_cycle 多进程循环机制





















