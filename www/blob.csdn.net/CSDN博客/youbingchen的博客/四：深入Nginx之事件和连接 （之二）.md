# 四：深入Nginx之事件和连接 （之二） - youbingchen的博客 - CSDN博客





2016年06月22日 10:40:18[youbingchen](https://me.csdn.net/youbingchen)阅读数：882








# ngx_events_module 核心事件

> 
定义新的事类型，并定义每个事件模块都需要实现的ngx_event_module_t接口，还需要管理这些事件模块生成的 配置 项结构体，并 解析事件类配置项，解析配置项时会调用其在`ngx_command_t`数组 中 定义的回调方法。 

  假设定义一个ngx_command_t(当然这个模块如何处理自己感兴趣的配置项)，ngx_events_commands数组决定了ngx_events__module模块 是如何定制其功能。


```cpp
static ngx_command_t  ngx_events_commands[] = {

    { ngx_string("events"),
      NGX_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_NOARGS,
      ngx_events_block,
      0,
      0,
      NULL },

      ngx_null_command
};
```

> 
可以看出`ngx_events_module`模块只对一个块配置项感兴趣，也就是nginx.conf中必须要有events{…}配置项。作为核心模块，ngx_events_module还需要实现核心模块的共同接口`ngx_core_module_t`如下所示:


```cpp
//  在 文件ngx_event.c
static ngx_core_module_t  ngx_events_module_ctx = {
    ngx_string("events"),
    NULL,
    ngx_event_init_conf
};
```

> 
ngx_events_module_ctx实现接口只是定义了 模块 名而已，ngx_core_module_t接口中定义的create_conf方法和init_conf方法。ngx_events_module模块并不会去解析配置项的参数，只是 在出现events配置项 后会调用各事件模块去解析events{…}块内的配置项。 

  ngx_events_module模块定义如下：


```cpp
//文件名: ngx_event.c
ngx_module_t  ngx_events_module = {
    NGX_MODULE_V1,
    &ngx_events_module_ctx,                /* module context */
    ngx_events_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};
```

> 
对events配置项的解析之外，这个模块没有做任何事情 
**每一个事件模块都必须实现`ngx_event_module_t接口`**这个接口中允许每个事件模块 建立 自己的配置项结构体，用于 存储感兴趣的配置项在nginx.conf中对应的参数。`ngx_event_module_t`中的create_conf方法就是用于创建这个结构体的方法，事件模块只需要在这个方法分配内存即可。每一个事件模块产生配置结构体指针都会 被放在`ngx_events_module`模块 创建`ngx_cycle__t`核心结构体的`conf_ctx`的 成员中，它指向一个指针数组。这个指针数组就依次存放着所有Nginx模块关于配置项方面的 指针。


# 管理事件模块

![control](https://img-blog.csdn.net/20160622154602608)
- 首先初始化所有事件模块`ctx_index`成员。这里 index是所有模块 在`ngx_modules.c`文件的`ngx_modules`数组 中的序号。它 与`ngx_modules`数组中所有模块的顺序是 一致的。
- 依次调用所有事件模块通用接口`ngx_event_module_t`中的`create_conf`方法，当然 产生的结构体的指针 保存 指针数组中。
- 针对 所有事件类型的 模块解析 配置项，这时，每个事件 模块定义的`ngx_command_t`决定 了 配置项的解析 方法，如果在nginx.conf中发现相应的配置项，就会回调各事件模块定义的方法。
- 解析完配置项后，依次调用所有事件模块通用接口 `ngx_event_module`中的`init_conf`方法。

# ngx_event_core_module事件模块

> 
ngx_event_core_module模块是 一个 事件类型的模块 ，它在所有的事件模块中的顺序是 第一位（configure执行时必须把它放在其他 事件模块 之前）。 

  ngx_event_core_module模块要完成连接池的读/写事件，同时还会决定使用哪些事件驱动机制，以及初始化将要使用的事件模块。该模块定义了`ngx_event_core_commands`数组感兴趣7个配置项


```cpp
// 文件名  nginx_event.c
static ngx_command_t  ngx_event_core_commands[] = {
/* 连接池的大小，也就是每个worker进程支持的TCP连接数*/
    { ngx_string("worker_connections"),
      NGX_EVENT_CONF|NGX_CONF_TAKE1,
      ngx_event_connections,
      0,
      0,
      NULL },
    // 选择哪种事件模块作为事件驱动机制
    { ngx_string("use"),
      NGX_EVENT_CONF|NGX_CONF_TAKE1,
      ngx_event_use,
      0,
      0,
      NULL },
    // 对于epoll事件驱动模式来说，意味着在接收有一个 新的连接事件 时，调用accept以尽可能 多接收连接
    { ngx_string("multi_accept"),
      NGX_EVENT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_event_conf_t, multi_accept),
      NULL },
     // 确定是否使用accept_mutex负载均衡锁，默认开启，同时这个参数也跟是否开启master模式或worker进程多于一个有关联
    { ngx_string("accept_mutex"),
      NGX_EVENT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_event_conf_t, accept_mutex),
      NULL },
    /*启用accept_mutex负载均衡锁之后，延迟accept_mutex_delay毫秒之后再试图处理连接事件*/
    { ngx_string("accept_mutex_delay"),
      NGX_EVENT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_event_conf_t, accept_mutex_delay),
      NULL },
     // 需要对来自指定的IP的TCP连接打印debug级别的调试日志
    { ngx_string("debug_connection"),
      NGX_EVENT_CONF|NGX_CONF_TAKE1,
      ngx_event_debug_connection,
      0,
      0,
      NULL },

      ngx_null_command
};
```

> 
用于存储上述配置项参数的结构体`ngx_event_conf_t`


```
// 文件名: nginx_event.h
typedef struct {
    // 连接池的大小
    ngx_uint_t    connections;
    //  选用的事件模块在所有事件模块中的序号，也就是ctx_index成员
    ngx_uint_t    use;
    // 标志为1，则表示接收到一个新的连接事件时，一次性建立尽可能的连接
    ngx_flag_t    multi_accept;
    // 标志位为1，启用负载均衡锁
    ngx_flag_t    accept_mutex;
    // 负载均衡 锁会使有些worker进程在拿不到锁时延迟建立新的连接
    ngx_msec_t    accept_mutex_delay;
    // 所选用 的事件 模块的名字和成员use是匹配的
    u_char       *name;

#if (NGX_DEBUG)
/*在--with-debug编译 模式 下，可以仅针对某些客户端建立的连接输出调试 级别 的日志，而debug_connection数组用于保存这些客户端的地址 信息*/
    ngx_array_t   debug_connection;
#endif
} ngx_event_conf_t;
```

> 
**对于每个事件都需要实现的ngx_event_module_t接口，ngx_event_core_module模块仅实现了create_conf和init_cconf方法 ，因为 它并 不负责TCP网络 事件的驱动**，在Nginx 启动过程还没有fork出worker进程，会首先调用ngx_event_core_module模块 的ngx_event_module_init方法，而在fork出worker子进程之后，每一个worker进程会调用ngx_event_core_module模块的ngx_event_process_init方法后 才会进入正式 的工作循环。




