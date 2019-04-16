# Nginx 模块自主开发四: 模块数据结构 - youbingchen的博客 - CSDN博客





2016年06月24日 11:07:44[youbingchen](https://me.csdn.net/youbingchen)阅读数：832








> 
其中,ngx_module_s，这 个是 每个新的模块的定义


```cpp
struct ngx_module_s {
   /*ctx_index表示当前模块在这类模块中的序号。这个 成员常常是由管理模块的 一个Nginx核心模块设置的，对于所有的http模块而言，ctx_index是由核心模块ngx_http_module设置的，ctx_index非常重要，Nginx的模块化设计非常依赖各个模块的顺序*/
    ngx_uint_t            ctx_index;
    /*index表示当前模块在ngx_modules数组中 的序号。*/
    ngx_uint_t            index; 

    char                 *name;
    // sqare系列的 保留变量，暂未使用
    ngx_uint_t            spare0;
    ngx_uint_t            spare1;
    // 模块的版本
    ngx_uint_t            version;
    const char           *signature;
   /**ctx用于指向一类的模块的上下文结构体，Nginx模块有许多种类，不同模块之间的功能差别很大。例如事件模块主要处理I/O事件相关的功能，每个模块都有自己的特性，ctx将会指向特定类型模块的公共接口，例如在Http 模块 中，ctxz指向ngx_http_module_t结构体*/
    void                 *ctx;
    // commands将处理nginx.conf中 的配置项
    ngx_command_t        *commands;
    /*type 表示该模块的类型，它与ctx指针是紧密相关的，NGX_HTTP_MODULE、NGX_CORE_MODULE、NGX_CONF_MODULE、NGX_EVENT_MODULE、NGX_EVENT_MODULE、NGX_STREAM_MODULE*/
    ngx_uint_t            type;
    /*在Nginx 启动、停止过程中，以下7个函数指针表示有7个执行点分别调用这个7个方法，对于任一方法，如果不需要Nginx在某个时刻执行 它，那么 简单地把它设为NULL空指针 即可*/
    /* 虽然字面上理解应当在master进程启动时调用，但到目前为止，框架代码从来不会调用它，因此 要设为NULL*/
    ngx_int_t           (*init_master)(ngx_log_t *log);
    /*init module 回调方法在初始化所有模块被调用，在master/worker模式下，这个阶段将在worker进程前完成 */
    ngx_int_t           (*init_module)(ngx_cycle_t *cycle);
   /*init_process回调方法在正常服务前被调用，在master/worker模式下,多个worker子进程已经产生，在每个worker进程初始化会调用所有模块的init_process函数*/
    ngx_int_t           (*init_process)(ngx_cycle_t *cycle);
    // Nginx目前还支持多线程，所以这个没有被调用过
    ngx_int_t           (*init_thread)(ngx_cycle_t *cycle);
    void                (*exit_thread)(ngx_cycle_t *cycle);
   /* exit_process回调方法在服务停止前调用。在master/worker模式下，worker会在退出前调用它*/
    void                (*exit_process)(ngx_cycle_t *cycle);
    //该方法 会在master进程退出前调用它
    void                (*exit_master)(ngx_cycle_t *cycle);
   //目前还没用
    uintptr_t             spare_hook0;
    uintptr_t             spare_hook1;
    uintptr_t             spare_hook2;
    uintptr_t             spare_hook3;
    uintptr_t             spare_hook4;
    uintptr_t             spare_hook5;
    uintptr_t             spare_hook6;
    uintptr_t             spare_hook7;
};
```

> 
回调方法:init_module、init_process、exit_process、exit_master，调用它们是Nginx 的 框架代码 。


所以定义模块的 时候，最重要的就是要设置ctx和commands 这两个成员。ctx指向的 是相应的接口(结构 体 )。框架会在读取、重新配置文件定义了ctx指向的接口描述 的 几个阶段，框架会在启动过程中调接口中定义的相应方法。如果相应的某个回调函数设为NULL,那么框架不会调用这个回调。

下面 我举一个ctx指向的ngx_http_module_t结构体 （不同模块指向不一样）

```cpp
typedef struct {
     // 解析配置文件前函数调用 
    ngx_int_t   (*preconfiguration)(ngx_conf_t *cf);
    // 完成配置文件的解析后调用
    ngx_int_t   (*postconfiguration)(ngx_conf_t *cf);
    /*当需要创建数据结构用于存储main级别(直属http{...}块的配置项)的 全局配置 项时，可以通过这个回调方法 创建存储全局配置项的结构体*/
    void       *(*create_main_conf)(ngx_conf_t *cf);
    // 常用于初始化main级别配置项
    char       *(*init_main_conf)(ngx_conf_t *cf, void *conf);
    /*当需要创建数据结构用于存储srv级别(直属于虚拟机server{}块配置项)，可以通过通过实现这个方法创建srv级别配置项的结构体*/
    void       *(*create_srv_conf)(ngx_conf_t *cf);
    //合并main级别和srv级别下的同名配置
    char       *(*merge_srv_conf)(ngx_conf_t *cf, void *prev, void *conf);
    /*当需要创建数据结构用于存储loc级别的(直属于location{}块 配置项)*/
    void       *(*create_loc_conf)(ngx_conf_t *cf);
    // 合并serv级别和loc级别下的同名配置
    char       *(*merge_loc_conf)(ngx_conf_t *cf, void *prev, void *conf);
} ngx_http_module_t;
```

说完 了ctx接口 ，接下来我们 进入另一个核心的结构commands

> 
commands数组是用于定义模块配置文件的参数，每一个数组元素都是ngx_command_t类型，数组的结尾用ngx__null_command表示，Nginx在解析 配置文件中一个 配置项时，首先会遍历所有模块，对于每一个模块而言，即通过遍历commands数组进行 ，在数组中检查到 ngx_null_command,就会停止当前模块解析该 配置项。


```cpp
struct ngx_command_s {
    // 配置项的名称 比如 "http"
    ngx_str_t             name;
    /*配置项类型，type将 指定配置项可以出现的位置，例如出现在server{}或location{}中，已经他可以携带参数的个数*/
    ngx_uint_t            type;
    // 出现了name指定的配置项后，将会调用set方法处理该配置项的参数
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
    //在配置项中的偏移量 
    ngx_uint_t            conf;
    //和conf参数配合使用
    ngx_uint_t            offset;
    //配置项读取配置的处理方法 ，必须是ngx_conf_post_t结构的指针
    void                 *post;
};
```

> 
ngx_null_command只是 一个空的ngx_command_t


```cpp
#define ngx_null_command  { ngx_null_string, 0, NULL, 0, 0, NULL }
```

下面是一个command定义的例子

```cpp
static ngx_command_t ngx_http_circle_gif_commands[] = {
{ ngx_string("circle_gif"),
NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
ngx_http_circle_gif,
NGX_HTTP_LOC_CONF_OFFSET,
0,
NULL },
{ ngx_string("circle_gif_min_radius"),
NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|
NGX_CONF_TAKE1,
ngx_conf_set_num_slot,
NGX_HTTP_LOC_CONF_OFFSET,
offsetof(ngx_http_circle_gif_loc_conf_t, min_radius),
NULL },
...
ngx_null_command
};
```

type选项
|type参数|含义|
|----|----|
|NGX_HTTP_MAIN_CON|指令出现在全局配置部分是合法的|
|NGX_HTTP_SRV_CONF|指令在主机配置部分出现是合法的|
|NGX_HTTP_LOC_CONF|指令在位置配置部分出现是合法的|
|NGX_HTTP_UPS_CONF|指令在上游服务器配置部分出现是合法的|
|NGX_CONF_NOARGS|指令没有参数|
|NGX_CONF_TAKE1|指令读入一个参数|
|NGX_CONF_TAKE2|指令读入两个参数|
|…..|…..|
|NGX_CONF_TAKE7|指令读入七个参数|
|NGX_CONF_FLAG|指令读入一个布尔型数据|
|NGX_CONF_1MORE|指令至少读入 1 个参数|
|NGX_CONF_2MORE|指令至少读入 2 个参数|

# 总结

nginx 主进程在启动时，就会在代码里面找相应的ngx_module_t（ngx_XXX_module）变量，找到以后，在其中ngx_command_t(ngx_xxx_commands)指定函数ngx 

_xxx_init开始初始化模块。所有的工作都要在这里进行。

**Nginx启动时，会先启动一个master管理进程，然后根据配置启动数个worker进程。实际的module里的勾子函数（例如ngx_XXX_handle），都是被worker进程所调用的。默认情况下，nginx并不是多线程的，所以，如果你的勾子函数被调用了，那么你绝对不可以有任何阻塞操作，否则会使得nginx worker不去处理已经在链表中的其他connection，这就完全毁了nginx，如果你去同步请求硬盘IO资源，否则其他SERVER的网络IO，那么它和apach+CGI这种低性能SERVER也没啥两样了，除了epoll可以hold住大量连接。**



