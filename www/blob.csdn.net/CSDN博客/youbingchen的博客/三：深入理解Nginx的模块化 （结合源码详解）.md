# 三：深入理解Nginx的模块化 （结合源码详解） - youbingchen的博客 - CSDN博客





2016年06月20日 10:56:16[youbingchen](https://me.csdn.net/youbingchen)阅读数：1287








盗用前面用到的流程图 
![nginx_process](https://img-blog.csdn.net/20160620105436180)
> 
第二步实际上是调用 `ngx_add_inherited_sockets()`


```cpp
//文件名: Nginx.c

int ngx_cdecl
main(int argc, char *const *argv)
{
 ...

    if (ngx_add_inherited_sockets(&init_cycle) != NGX_OK) {
        return 1;
    }

 ...
}
```

> 
Nginx在不重启服务升级，旧版本的`master`进程会通过`execve`系统调用来启动新版本的`master`进程（先 fork()出子进程再调用exec来运行新程序），这时旧版本 的`master`进程通过 环境变量通知新版本的`master`这是在升级，新版本的`master`进程通过 `ngx_add_inherited_sockets()`方法 由环境变量里读取平滑升级信息，并对旧版本Nginx服务监听 的句柄做继承处理 。




> 
3~8步都在 `ngx_init_cycle()`方法中执行的。在初始化`ngx_cycle_t`中所有的容器后，会为读取、解析文件 做准备工作。因为每个 模块都必须有相应的数据结构来 存储配置文件中的各配置项，创建这些数据结构的工作都需要在这一步进行。Nginx框架只关心`NGX_CORE_MODULE`核心模块,从而降低框架的复杂度，这里将会调用所有核心模块的`create_conf`的方法(也只有核心模块才有这个方法 )，这意味着所有核心模块开始构造用于存储配置项的结构体。非核心模块将由每个模块进行管理，如 HTTP模块都由`ngx_http_module`管理。这样`ngx_http_module`在解析自己感兴趣的”http”配置项时，将会调用所有HTTP模块 约定的方法来 创建存储配置的结构体


```cpp
//文件名 ngx_cycle.h
typedef struct ngx_cycle_s       ngx_cycle_t;
struct ngx_cycle_s {
    void                  ****conf_ctx;                       // 配置上下文数组(含所有模块)
    ngx_pool_t               *pool;                           // 内存池

    ngx_log_t                *log;                            // 日志
    ngx_log_t                 new_log;

    ngx_connection_t        **files;                         // 连接文件
    ngx_connection_t         *free_connections;              // 空闲连接
    ngx_uint_t                free_connection_n;             // 空闲连接个数
    ngx_module_t            **modules;
    ngx_uint_t                modules_n;
    ngx_uint_t                modules_used;    /* unsigned  modules_used:1; */
    ngx_queue_t               reusable_connections_queue;    // 再利用连接队列

    ngx_array_t               listening;                     // 监听套接字数组
    ngx_array_t               pathes;                        // 路径数组
    ngx_list_t                open_files;                    // 打开文件链表
    ngx_list_t                shared_memory;                 // 共享内存链表

    ngx_uint_t                connection_n;                  // 连接个数
    ngx_uint_t                files_n;                       // 打开文件个数

    ngx_connection_t         *connections;                   // 连接
    ngx_event_t              *read_events;                   // 读事件
    ngx_event_t              *write_events;                  // 写事件

    ngx_cycle_t              *old_cycle;                     // old cycle指针

    ngx_str_t                 conf_file;                     // 配置文件
    ngx_str_t                 conf_param;                    // 配置参数
    ngx_str_t                 conf_prefix;                   // 配置文件目录
    ngx_str_t                 prefix;                        // 程序工作目录
    ngx_str_t                 lock_file;                     // 锁文件，用在不支持accept_mutex的系统中
    ngx_str_t                 hostname;                      // 主机名
};
```

> 
调用配置模块提供的解析配置项方法 。遍历`nginx.conf`的所有配置项，对于任一配置项，将会检查所有核心模块以找出对它所感兴趣的模块。并调用该模块在 `ngx_command_t` 结构体中的定义的 配置项处理办法 。 

  调用所有的`NGX_CORE_MODULE`核心模块的`init_conf`的 方法。这一步骤的 目的在于让所有核心模块在解析完配置项可以做综合性处理。 

  之前第四步在 解析配置项时，所有的模块都已经解析出自己需要监听的端口 ，如HTTP模块 已经 在 解析http{…}配置项时得到要监听的端口 ，并添加到listening数组中。这一步骤就是按照listening数组中的每一个` ngx_listening_t`元素设置socket句柄并监听端口，实际上就是调用`ngx_open_listening_sockets()`。 

  在这个阶段会调用所有模块的 `init_module`方法。接下来 就是根据 配置Nginx运行模式决定如何工作。 

  接下来流程可以参考之前的博客[nginx代码分析](http://blog.csdn.net/youbingchen/article/details/51626351)
# worker进程工作流程

`master`采用的是信号的方式通知worker进程停止服务或更换日志。在函数`ngx_worker_process_cycle()`通过检查`ngx_exiting、ngx_terminate、ngx_quit、ngx_reopen`这4个标志位来决定后续动作。 
![worker_process](https://img-blog.csdn.net/20160620105518016)
# master进程工作流程 

`master`进程不需要处理网络事件，不负责业务的执行，只会通过该管理`worker`等子进程来实现重启服务、平滑升级、更换日志文件、配置文件实时生效等 功能 。 
`master`进程中所有子进程相关的状态信息都保存在ngx_processes数组中，下面 是数组元素的类型`ngx_process_t`的 结构的定义，代码如下 ：

```cpp
typedef struct {
    //进程 ID
    ngx_pid_t           pid;
    // 由waitpid系统调用获取到进程状态
    int                 status;
    // 这是由socketpair系统调用产生出用于进程 间通信 的socket句柄
    ngx_socket_t        channel[2];
    // 子进程的循环执行的办法，当父进程调用ngx_spawn_process  生成子进程时使用
    ngx_spawn_proc_pt   proc;
    void               *data;
    // 进程名称。操作系统中显示的进程名称与name相同
    char               *name;
    // 标志位，为1时表示在重新生成子进程
    unsigned            respawn:1;
    // 标志位, 为1时表示正在 生成子进程
    unsigned            just_spawn:1;
    // 标志位，为1时表示在父进程、子进程分离
    unsigned            detached:1;
    // 标志位，为1时表示进程正在退出
    unsigned            exiting:1;
    // 标志位，为1时表示进程已经退出
    unsigned            exited:1;
} ngx_process_t;
```

> 
`ngx_spawn_process`方法封装了`fork`系统调用，并且会从`ngx_processes`数组中选择一个还未使用的`ngx_process_t`元素存储这个子进程的相关信息。 
![master_process](https://img-blog.csdn.net/20160620105545806)








