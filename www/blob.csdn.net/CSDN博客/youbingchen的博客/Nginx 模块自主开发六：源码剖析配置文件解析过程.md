# Nginx 模块自主开发六：源码剖析配置文件解析过程 - youbingchen的博客 - CSDN博客





2016年06月29日 13:43:23[youbingchen](https://me.csdn.net/youbingchen)阅读数：7197








> 
　Nginx源码实现有一个很好的优点就是模块化，有点像面向对象语言的设计模式，实现“高内聚，松耦合”，这篇博客主要讲解模块的共有流程 

  集中在ngx_cycle.c、 ngx_process.c、 ngx_process_cycle.c 和 ngx_event.c代码中。

共有流程开始于解析 nginx 配置，这个过程集中在 ngx_init_cycle 函数中。 ngx_init_cycle 是 nginx 的一个核心函数，共有流程中与配置相关的几个过程都在这个函数中实现，其中包括解析 nginx 配置、初始化 CORE模块，接着是初始化文件句柄，初始化错误日志，初始化共享内存，然后是监听端口。可以说共有流程 80% 都是现在 ngx_init_cycle 函数中，其中流程可以参考[三：深入理解Nginx的模块化 （结合源码详解）](http://blog.csdn.net/youbingchen/article/details/51719277)


# ngx_cycle_t结构体解析

> 
Nginx框架都是围绕着ngx_cycle_t结构体控制进程运行的，ngx_cycle_t结构体初始化就是在ngx_init_cycle函数中。


```cpp
truct ngx_cycle_s {
    /*     保存着所有模块存储配置项的结构体指针，     它首先是一个数组，数组大小为ngx_max_module，正好与Nginx的module个数一样；     
    每个数组成员又是一个指针，指向另一个存储着指针的数组，因此会看到void ****    
    每个进程中都有一个唯一的ngx_cycle_t核心结构体，它有一个成员conf_ctx维护着所有模块的配置结构体，
  其类型是void ****conf_ctx。conf_ctx意义为首先指向一个成员皆为指针的数组，其中每个成员指针又指向另外一个
  成员皆为指针的数组，第2个子数组中的成员指针才会指向各模块生成的配置结构体。这正是为了事件模
  块、http模块、mail模块而设计的，这有利于不同于NGX_CORE_MODULE类型的
  特定模块解析配置项。然而，NGX_CORE_MODULE类型的核心模块解析配置项时，配置项一定是全局的，
  不会从属于任何{}配置块的，它不需要上述这种双数组设计。解析标识为NGX_DIRECT_CONF类型的配
  置项时，会把void****类型的conf_ctx强制转换为void**，也就是说，此时，在conf_ctx指向的指针数组
  中，每个成员指针不再指向其他数组，直接指向核心模块生成的配置鲒构体。因此，NGX_DIRECT_CONF
  仅由NGX_CORE_MODULE类型的核心模块使用，而且配置项只应该出现在全局配置中。
    */ //初始化见ngx_init_cycle，所有为http{} server{} location{}分配的空间都由该指针指向新开辟的空间
    //NGX_CORE_MODULE类型模块赋值在ngx_init_cycle 
    //http{}ngx_http_module相关模块赋值地方在ngx_http_block
    */

/*
    在核心结构体ngx_cycle_t的conf_ctx成员指向的指针数组中，第7个指针由ngx_http_module模块使用（ngx_http_module模块的index序号为6，
由于由0开始，所以它在ngx_modules数组中排行第7。在存放全局配置结构体的conf_ctx数组中，第7个成员指向ngx_http_module模块），这个指针
设置为指向解析http{}块时生成的ngx_http_conf_ctx_t结构体，而ngx_http_conf_ctx_t的3个成员则分别指向新分配的3个指针数组。新的指针数组中
成员的意义由每个HTTP模块的ctx_index序号指定（ctx_index在HTTP模块中表明它处于HTTP模块间的序号），例如，第6个HTTP模块的ctx_index是5
（ctx_index同样由0开始计数），那么在ngx_http_conf_ctx_t的3个数组中，第6个成员就指向第6个HTTP模块的create_main_conf、create_srv_conf、
create_loc_conf方法建立的结构体，当然，如果相应的回调方法没有实现，该指针就为NULL空指针。
*/ 
/*
 见ngx_init_cycle  conf.ctx = cycle->conf_ctx; //这样下面的ngx_conf_param解析配置的时候，里面对conf.ctx赋值操作，实际上就是对cycle->conf_ctx[i]
可如何由ngx_cycle_t核心结构体中找到main级别的配置结构体呢？Nginx提供的ngx_http_cycle_get_module_main_conf宏可以实现这个功能
*/
    void                  ****conf_ctx; //有多少个模块就会有多少个指向这些模块的指针，见ngx_init_cycle   ngx_max_module
    ngx_pool_t               *pool; // 内存池

    /*    日志模块中提供了生成基本ngx_log_t日志对象的功能，这里的log实际上是在还没有执行ngx_init_cycle方法前，    
    也就是还没有解析配置前，如果有信息需要输出到日志，就会暂时使用log对象，它会输出到屏幕。    
    在ngx_init_cycle方法执行后，将会根据nginx.conf配置文件中的配置项，构造出正确的日志文件，此时会对log重新赋值。    */
    //ngx_init_cycle中赋值cycle->log = &cycle->new_log;
    ngx_log_t                *log; //指向ngx_log_init中的ngx_log，如果配置error_log，指向这个配置后面的文件参数，见ngx_error_log。否则在ngx_log_open_default中设置

    /* 由nginx.conf配置文件读取到日志文件路径后，将开始初始化error_log日志文件，由于log对象还在用于输出日志到屏幕，    
    这时会用new_log对象暂时性地替代log日志，待初始化成功后，会用new_log的地址覆盖上面的log指针    */
    // 如果没有配置error_log则在ngx_log_open_default设置为NGX_ERROR_LOG_PATH，如果通过error_log有配置过则通过ngx_log_set_log添加到该new_log->next链表连接起来
    /* 全局中配置的error_log xxx存储在ngx_cycle_s->new_log，http{}、server{}、local{}配置的error_log保存在ngx_http_core_loc_conf_t->error_log,
    见ngx_log_set_log,如果只配置全局error_log，不配置http{}、server{}、local{}则在ngx_http_core_merge_loc_conf conf->error_log = &cf->cycle->new_log;  */
    //ngx_log_insert插入，在ngx_log_error_core找到对应级别的日志配置进行输出，因为可以配置error_log不同级别的日志存储在不同的日志文件中
    ngx_log_t                 new_log;//如果配置error_log，指向这个配置后面的文件参数，见ngx_error_log。否则在ngx_log_open_default中设置

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    /*  对于poll，rtsig这样的事件模块，会以有效文件句柄数来预先建立这些ngx_connection t结构
体，以加速事件的收集、分发。这时files就会保存所有ngx_connection_t的指针组成的数组，files_n就是指
针的总数，而文件句柄的值用来访问files数组成员 */
    ngx_connection_t        **files; //sizeof(ngx_connection_t *) * cycle->files_n  见ngx_event_process_init  ngx_get_connection

    /*
        从图9-1中可以看出，在ngx_cycle_t中的connections和free_connections达两个成员构成了一个连接池，其中connections指向整个连
    接池数组的首部，而free_connections则指向第一个ngx_connection_t空闲连接。所有的空闲连接ngx_connection_t都以data成员（见9.3.1节）作
    为next指针串联成一个单链表，如此，一旦有用户发起连接时就从free_connections指向的链表头获取一个空闲的连接，同时free_connections再指
    向下一个空闲连接。而归还连接时只需把该连接插入到free_connections链表表头即可。
     */ //见ngx_event_process_init, ngx_connection_t空间和它当中的读写ngx_event_t存储空间都在该函数一次性分配好
    ngx_connection_t         *free_connections;// 可用连接池，与free_connection_n配合使用
    ngx_uint_t                free_connection_n;// 可用连接池中连接的总数

    //ngx_connection_s中的queue添加到该链表上
    /*
    通过读操作可以判断连接是否正常，如果不正常的话，就会把该ngx_close_connection->ngx_free_connection释放出来，这样
    如果之前free_connections上没有空余ngx_connection_t，c = ngx_cycle->free_connections;就可以获取到刚才释放出来的ngx_connection_t
    见ngx_drain_connections
    */ 
    ngx_queue_t               reusable_connections_queue;/* 双向链表容器，元素类型是ngx_connection_t结构体，表示可重复使用连接队列 表示可以重用的连接 */

//ngx_http_optimize_servers->ngx_http_init_listening->ngx_http_add_listening->ngx_create_listening把解析到的listen配置项信息添加到cycle->listening中
    //通过"listen"配置创建ngx_listening_t加入到该数组中
    ngx_array_t               listening;// 动态数组，每个数组元素储存着ngx_listening_t成员，表示监听端口及相关的参数

    /*    动态数组容器，它保存着nginx所有要操作的目录。如果有目录不存在，就会试图创建，而创建目录失败就会导致nginx启动失败。    */
    //通过解析配置文件获取到的路径添加到该数组，例如nginx.conf中的client_body_temp_path proxy_temp_path，参考ngx_conf_set_path_slot
    //这些配置可能设置重复的路径，因此不需要重复创建，通过ngx_add_path检测添加的路径是否重复，不重复则添加到paths中
    ngx_array_t               paths;//数组成员 nginx_path_t ，
    ngx_array_t               config_dump;

    /*    单链表容器，元素类型是ngx_open_file_t 结构体，它表示nginx已经打开的所有文件。事实上，nginx框架不会向open_files链表中添加文件。    
    而是由对此感兴趣的模块向其中添加文件路径名，nginx框架会在ngx_init_cycle 方法中打开这些文件    */
    //该链表中所包含的文件的打开在ngx_init_cycle中打开
    ngx_list_t                open_files; //如nginx.conf配置文件中的access_log参数的文件就保存在该链表中，参考ngx_conf_open_file
    //创建ngx_shm_zone_t在ngx_init_cycle，在ngx_shared_memory_add也可能创建新的ngx_shm_zone_t，为每个ngx_shm_zone_t真正分配共享内存空间在ngx_init_cycle
    ngx_list_t                shared_memory;// 单链表容器，元素类型是ngx_shm_zone_t结构体，每个元素表示一块共享内存

    ngx_uint_t                connection_n;// 当前进程中所有链接对象的总数，与connections成员配合使用
    ngx_uint_t                files_n; //每个进程能够打开的最多文件数  赋值见ngx_event_process_init

    /*
    从图9-1中可以看出，在ngx_cycle_t中的connections和free_connections达两个成员构成了一个连接池，其中connections指向整个连接池数组的首部，
    而free_connections则指向第一个ngx_connection_t空闲连接。所有的空闲连接ngx_connection_t都以data成员（见9.3.1节）作为next指针串联成一个
    单链表，如此，一旦有用户发起连接时就从free_connections指向的链表头获取一个空闲的连接，同时free_connections再指向下一个空闲连
    接。而归还连接时只需把该连接插入到free_connections链表表头即可。

    在connections指向的连接池中，每个连接所需要的读/写事件都以相同的数组序号对应着read_events、write_events读/写事件数组，
    相同序号下这3个数组中的元素是配合使用的
     */
    ngx_connection_t         *connections;// 指向当前进程中的所有连接对象，与connection_n配合使用

    /*
    事件是不需要创建的，因为Nginx在启动时已经在ngx_cycle_t的read_events成员中预分配了所有的读事件，并在write_events成员中预分配了所有的写事件

    在connections指向的连接池中，每个连接所需要的读/写事件都以相同的数组序号对应着read_events、write_events读/写事件数组，相同序号下这
    3个数组中的元素是配合使用的。图9-1中还显示了事件池，Nginx认为每一个连接一定至少需要一个读事件和一个写事件，有多少连接就分配多少个读、
    写事件。怎样把连接池中的任一个连接与读事件、写事件对应起来呢？很简单。由于读事件、写事件、连接池是由3个大小相同的数组组成，所以根据数组
    序号就可将每一个连接、读事件、写事件对应起来，这个对应关系在ngx_event_core_module模块的初始化过程中就已经决定了（参见9.5节）。这3个数组
    的大小都是由cycle->connection_n决定。
     */
    ngx_event_t              *read_events;// 指向当前进程中的所有读事件对象，connection_n同时表示所有读事件的总数
    ngx_event_t              *write_events;// 指向当前进程中的所有写事件对象，connection_n同时表示所有写事件的总数

    /*    旧的ngx_cycle_t 对象用于引用上一个ngx_cycle_t 对象中的成员。例如ngx_init_cycle 方法，在启动初期，    
    需要建立一个临时的ngx_cycle_t对象保存一些变量， 
    再调用ngx_init_cycle 方法时就可以把旧的ngx_cycle_t 对象传进去，    而这时old_cycle对象就会保存这个前期的ngx_cycle_t对象。    */
    ngx_cycle_t              *old_cycle;

    ngx_str_t                 conf_file;// 配置文件相对于安装目录的路径名称 默认为安装路径下的NGX_CONF_PATH,见ngx_process_options
    ngx_str_t                 conf_param;// nginx 处理配置文件时需要特殊处理的在命令行携带的参数，一般是-g 选项携带的参数
    ngx_str_t                 conf_prefix;    // nginx配置文件所在目录的路径  ngx_prefix 见ngx_process_options
    ngx_str_t                 prefix; //nginx安装目录的路径 ngx_prefix 见ngx_process_options
    ngx_str_t                 lock_file;// 用于进程间同步的文件锁名称
    ngx_str_t                 hostname; // 使用gethostname系统调用得到的主机名  在ngx_init_cycle中大写字母被转换为小写字母
};
```

配置解析

# 配置解析接口 

ngx_init_cycle 提供的是配置解析接口。接口是一个切入点，通过少量代码提供一个完整功能的调用。配置解析接口分为两个阶段，一个是准备阶段，另一个就是真正开始调用配置解析。准备阶段指什么呢？主要是准备三点：
- 准备内存:nginx会根据 以往的经验（old_cycle)预测这一次的配置 需要分配多少内存 

```cpp
if (old_cycle->shared_memory.part.nelts) {
        n = old_cycle->shared_memory.part.nelts;
        for (part = old_cycle->shared_memory.part.next; part; part = part->next)
        {
            n += part->nelts;
        }

    } else {
        n = 1;
    }

    if (ngx_list_init(&cycle->shared_memory, pool, n, sizeof(ngx_shm_zone_t))
        != NGX_OK)
    {
        ngx_destroy_pool(pool);
        return NULL;
    }
```
- 准备错误日志:nginx启动可能出错，出现就要记录就要记录在日志文件中 。

```cpp
log = old_cycle->log;

    pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    if (pool == NULL) {
        return NULL;
    }
    pool->log = log;

    cycle = ngx_pcalloc(pool, sizeof(ngx_cycle_t));
    if (cycle == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }

    cycle->pool = pool;
    cycle->log = log;
    cycle->old_cycle = old_cycle;
```
- 准备数据结构，一个是ngx_cycle_t结构，一个是ngx_conf_t结构，ngx_cycle_t结构用于存放所有CORE模块的 配置，而ngx_conf_t则是用于存放解析配置的上下文的信息 

ngx_conf_t结构体

```cpp
struct ngx_conf_s {
//当前解析到的命令名
    char                 *name;
//当前命令的所有参数
    ngx_array_t          *args;

//使用的cycle
    ngx_cycle_t          *cycle;
//所使用的内存池
    ngx_pool_t           *pool;
//这个pool将会在配置解析完毕后释放。
    ngx_pool_t           *temp_pool;
//这个表示将要解析的配置文件
    ngx_conf_file_t      *conf_file;
//配置log
    ngx_log_t            *log;

//主要为了提供模块的层次化(后续会详细介绍)
    void                 *ctx;
//模块类型
    ngx_uint_t            module_type;
//命令类型
    ngx_uint_t            cmd_type;

//模块自定义的handler
    ngx_conf_handler_pt   handler;
//自定义handler的conf
    char                 *handler_conf;
};
```

```cpp
for (i = 0; ngx_modules[i]; i++) {
    if (ngx_modules[i]->type != NGX_CORE_MODULE) {
        continue;
    }

    module = ngx_modules[i]->ctx;

    if (module->create_conf) {
        rv = module->create_conf(cycle);
        if (rv == NULL) {
            ngx_destroy_pool(pool);
            return NULL;
        }
        cycle->conf_ctx[ngx_modules[i]->index] = rv;
    }
}

conf.ctx = cycle->conf_ctx;   // cycle是ngx_cycle_t结构，conf就是ngx_conf_t结构
conf.cycle = cycle;
conf.pool = pool;
conf.log = log;
conf.module_type = NGX_CORE_MODULE;  //注意，一开始命令的类型就是MAIN，并且模块类型是core
conf.cmd_type = NGX_MAIN_CONF;
```

> 
准备好了这些内容，nginx开始调用配置解析模块，其代码如下：


```cpp
if (ngx_conf_param(&conf) != NGX_CONF_OK) {
    environ = senv;
    ngx_destroy_cycle_pools(&conf);
    return NULL;
}
/*开始解析文件 然后来看ngx_conf_parse,这个函数第二个是将要解析的文件名，不过这里还有一个要注意的，那就是第二个参数可以为空的，如果为空，则说明将要解析的是block中的内容或者param。*/
if (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
    environ = senv;
    ngx_destroy_cycle_pools(&conf);
    return NULL;
}
```

第一个if解析nginx命令行参数’-g’加入的配置。第二个if解析nginx配置文件。好的设计就体现在接口极度简化，模块之间的耦合非常低。这里只使用区区10行完成了配置的解析

# 配置解析 

配置解析模块在 ngx_conf_file.c 中实现。模块提供的接口函数主要是 ngx_conf_parse，另外模块提供单独的接口ngx_conf_param,用来解析命令行传递的配置，当然这个接口也是 对ngx_conf_parse的包装。

ngx_conf_parse 函数支持三种不同的解析环境:



- parse_file：解析配置文件
- parse_block：解析块配置。块配置一定是由“ {”和“ }”包裹起来的；
- parse_param：解析命令行配置。命令行配置中不支持块指令。 




> 
  这是一个递归的过程。nginx首先解析core模块的配置。core模块提供一些块指令，这些指令引入其他类型的模块，nginx遇到这些指令，就重新迭代解析过程，解析其他模块的配置。这些模块配置中又有一些块指令引入新的模块类型或者指令类型，nginx就会再次迭代，解析这些新的配置类型。比如上图，nginx遇到“events”指令，就重新调用ngx_conf_parse()解析event模块配置，解析完以后ngx_conf_parse()返回，nginx继续解析core模块指令，直到遇到“http”指令。nginx再次调用ngx_conf_parse()解析http模块配置的http级指令，当遇到“server”指令时，nginx又一次调用ngx_conf_parse()解析http模块配置的server级指令。 
![parseconfig](https://img-blog.csdn.net/20160629134647968)
```cpp
//  ngx_conf_parse()解析配置分成两个主要阶段，一个是词法分析，一个是指令解析。
char *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
    char             *rv;
    ngx_fd_t          fd;
    ngx_int_t         rc;
    ngx_buf_t         buf;
    ngx_conf_file_t  *prev, conf_file;
    enum {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

#if (NGX_SUPPRESS_WARN)
    fd = NGX_INVALID_FILE;
    prev = NULL;
#endif

    if (filename) {

        /* open configuration file */
................................................

    } else if (cf->conf_file->file.fd != NGX_INVALID_FILE) {
  //到这里说明接下来解析的是block中的内容
        type = parse_block;

    } else {
    //参数
        type = parse_param;
    }


    for ( ;; ) {
        rc = ngx_conf_read_token(cf);  //此法分析，类似文件内容格式分析

        /*
         * ngx_conf_read_token() may return
         *
         *    NGX_ERROR             there is error
         *    NGX_OK                the token terminated by ";" was found
         *    NGX_CONF_BLOCK_START  the token terminated by "{" was found
         *    NGX_CONF_BLOCK_DONE   the "}" was found
         *    NGX_CONF_FILE_DONE    the configuration file is done
         */
.....................................................

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */
      //如果有handler，则调用handler
        if (cf->handler) {  //handler是自定义解析函数指针

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            if (rv == NGX_CONF_OK) {
                continue;
            }

            if (rv == NGX_CONF_ERROR) {
                goto failed;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, rv);

            goto failed;
        }
      //没有handler则调用默认解析函数
        rc = ngx_conf_handler(cf, rc);

        if (rc == NGX_ERROR) {
            goto failed;
        }
    }

failed:

    rc = NGX_ERROR;

done:
....................................

    return NGX_CONF_OK;
}
```

### 为什么需要cf的两个主要cf->handler和cf->handler_conf这两个属性，其中handler是自定义解析函数指针，handler_conf是conf指针。

> 
Nginx的配置文件是分块的，然后event, http都是一个大的core模块，然后core模块中包含了很多2级模块(epoll/kqeue/proxy..).也就是1级模块中必须包含一个上下文用来保存2级模块的配置。而在HTTP模块中又有一些特殊，那就是HTTP模块中每个指令都可能会有3个作用域，那就是main/server/loc,所以在HTTP的上下文中，必须同时保存这3个上下文。


同时Nginx中的命令 有

```cpp
#define NGX_CONF_ARGS_NUMBER 0x000000ff
#define NGX_CONF_BLOCK       0x00000100
#define NGX_CONF_FLAG        0x00000200
#define NGX_CONF_ANY         0x00000400
#define NGX_CONF_1MORE       0x00000800
#define NGX_CONF_2MORE       0x00001000

#define NGX_DIRECT_CONF      0x00010000

#define NGX_MAIN_CONF        0x01000000
#define NGX_ANY_CONF         0x1F000000
#define NGX_HTTP_MODULE           0x50545448   /* "HTTP" */

#define NGX_HTTP_MAIN_CONF        0x02000000
#define NGX_HTTP_SRV_CONF         0x04000000
#define NGX_HTTP_LOC_CONF         0x08000000
#define NGX_HTTP_UPS_CONF         0x10000000
#define NGX_HTTP_SIF_CONF         0x20000000
#define NGX_HTTP_LIF_CONF         0x40000000
#define NGX_HTTP_LMT_CONF         0x80000000
```

> 
DIRECT_CONF顾名思义，就是说直接存取CONF，也就是说进入命令解析函数的同时，CONF已经创建好了，只需要直接使用就行了(也就是会有create_conf回调）。而Main_conf就是说最顶层的conf，比如HTTP/EVENT/PID等等，可以看到都属属于CORE 模块。而NGX_HTTP_XXX就是所有HTTP模块的子模块.


#### ngx_conf_handler函数

```cpp
//如果设置了type
            if (!(cmd->type & NGX_CONF_ANY)) {
//首先判断参数个数是否合法
                if (cmd->type & NGX_CONF_FLAG) {

                    if (cf->args->nelts != 2) {
                        goto invalid;
                    }

                } else if (cmd->type & NGX_CONF_1MORE) {

                    if (cf->args->nelts < 2) {
                        goto invalid;
                    }
.................................................
            }

            /* set up the directive's configuration context */

            conf = NULL;
//最核心的地方，
            if (cmd->type & NGX_DIRECT_CONF) {
//我们还记得最开始ctx是包含了所有core模块的conf(create_conf回调),因此这里取出对应的模块conf.
                conf = ((void **) cf->ctx)[ngx_modules[i]->index];   // NgX_DIRECT_CONF 才有create_conf回调

            } else if (cmd->type & NGX_MAIN_CONF) {
//如果不是DIRECT_CONF并且是MAIN，则说明我们需要在配置中创建自己模块的上下文(也就是需要进入二级模块)
                conf = &(((void **) cf->ctx)[ngx_modules[i]->index]);   // MAIN_CONF一般没有create_conf回调

            } else if (cf->ctx) {
//否则进入二级模块处理(后续会详细介绍)。
                confp = *(void **) ((char *) cf->ctx + cmd->conf);

                if (confp) {
                    conf = confp[ngx_modules[i]->ctx_index];
                }
            }
//调用命令的回调函数。
            rv = cmd->set(cf, cmd, conf);

            if (rv == NGX_CONF_OK) {
                return NGX_OK;
            }

            if (rv == NGX_CONF_ERROR) {
                return NGX_ERROR;
            }

            ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                               "\"%s\" directive %s", name->data, rv);

            return NGX_ERROR;
        }
```

上面代码中二级模块解析那部分先放一下，首先来看Nginx中带二级模块的一级模块如何解析命令的，来看HTTP模块(event模块基本一样)的解析代码。

```cpp
/可以看到没有direct_conf,因为http包含有二级模块。
static ngx_command_t  ngx_http_commands[] = {

    { ngx_string("http"),
      NGX_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_NOARGS,
      ngx_http_block,
      0,
      0,
      NULL },

      ngx_null_command
};


static char *
ngx_http_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    char                        *rv;
    ngx_uint_t                   mi, m, s;
    ngx_conf_t                   pcf;
    ngx_http_module_t           *module;
    ngx_http_conf_ctx_t         *ctx;
    ngx_http_core_loc_conf_t    *clcf;
    ngx_http_core_srv_conf_t   **cscfp;
    ngx_http_core_main_conf_t   *cmcf;

    /* the main http context */

    ctx = ngx_pcalloc(cf->pool, sizeof(ngx_http_conf_ctx_t));
    if (ctx == NULL) {
        return NGX_CONF_ERROR;
    }
//最核心的地方，可以看到修改了传递进来的conf
    *(ngx_http_conf_ctx_t **) conf = ctx;

    /* count the number of the http modules and set up their indices */

    ngx_http_max_module = 0;
    for (m = 0; ngx_modules[m]; m++) {
        if (ngx_modules[m]->type != NGX_HTTP_MODULE) {
            continue;
        }
//然后保存了对应模块的索引.
        ngx_modules[m]->ctx_index = ngx_http_max_module++;
    }


    /* the http main_conf context, it is the same in the all http contexts */
//创建HTTP对应的conf，因为每个级别(main/ser/loc)都会包含模块的conf.
    ctx->main_conf = ngx_pcalloc(cf->pool,
                                 sizeof(void *) * ngx_http_max_module);
    if (ctx->main_conf == NULL) {
        return NGX_CONF_ERROR;
    }


    /*
     * the http null srv_conf context, it is used to merge
     * the server{}s' srv_conf's
     */

    ctx->srv_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
    if (ctx->srv_conf == NULL) {
        return NGX_CONF_ERROR;
    }


    /*
     * the http null loc_conf context, it is used to merge
     * the server{}s' loc_conf's
     */

    ctx->loc_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
    if (ctx->loc_conf == NULL) {
        return NGX_CONF_ERROR;
    }


    /*
     * create the main_conf's, the null srv_conf's, and the null loc_conf's
     * of the all http modules
     */
....................................
//保存当前使用的cf，因为我们只是在解析HTTP时需要改变当前的cf，
    pcf = *cf;
//保存当前模块的上下文
    cf->ctx = ctx;
..........................................

    /* parse inside the http{} block */
//设置模块类型和命令类型
    cf->module_type = NGX_HTTP_MODULE;
    cf->cmd_type = NGX_HTTP_MAIN_CONF;
//开始解析，这里注意传递进去的文件名是空
    rv = ngx_conf_parse(cf, NULL);

    if (rv != NGX_CONF_OK) {
        goto failed;
    }

    /*
     * init http{} main_conf's, merge the server{}s' srv_conf's
     * and its location{}s' loc_conf's
     */
.........................................
    /*
     * http{}'s cf->ctx was needed while the configuration merging
     * and in postconfiguration process
     */
//回复cf
    *cf = pcf;
......................................

    return NGX_CONF_OK;

failed:

    *cf = pcf;

    return rv;
```

> 
每个级别都会保存对应的ctx(main/ser/loc)，怎么说呢，就是在解析HTTP main中创建了3个ctx(main/srv/loc),而在HTTP srv block中将会创建2个ctx(srv/loc),这时发生重复了，那就需要merge了。比如一个命令(srv_offset)在HTTP main中有一个，那么Nginx将会把它放入到HTTP main的ctx的srv ctx中，然后server block也有一个，那么Nginx会继续把它放到Server ctx的 srv_conf中，最后merge他们。


所以我们解析一下server模块

```cpp
{ ngx_string("server"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_MULTI|NGX_CONF_NOARGS,
      ngx_http_core_server,
      0,
      0,
      NULL },

static char *
ngx_http_core_server(ngx_conf_t *cf, ngx_command_t *cmd, void *dummy)
{
    char                        *rv;
    void                        *mconf;
    ngx_uint_t                   i;
    ngx_conf_t                   pcf;
    ngx_http_module_t           *module;
    struct sockaddr_in          *sin;
    ngx_http_conf_ctx_t         *ctx, *http_ctx;
    ngx_http_listen_opt_t        lsopt;
    ngx_http_core_srv_conf_t    *cscf, **cscfp;
    ngx_http_core_main_conf_t   *cmcf;

    ctx = ngx_pcalloc(cf->pool, sizeof(ngx_http_conf_ctx_t));
    if (ctx == NULL) {
        return NGX_CONF_ERROR;
    }

    http_ctx = cf->ctx;
//main conf不变
    ctx->main_conf = http_ctx->main_conf;

    /* the server{}'s srv_conf */
//创建新的srv和loc conf.
    ctx->srv_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
    if (ctx->srv_conf == NULL) {
        return NGX_CONF_ERROR;
    }

    /* the server{}'s loc_conf */

    ctx->loc_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
    if (ctx->loc_conf == NULL) {
        return NGX_CONF_ERROR;
    }
............................

    /* the server configuration context */

    cscf = ctx->srv_conf[ngx_http_core_module.ctx_index];
    cscf->ctx = ctx;


    cmcf = ctx->main_conf[ngx_http_core_module.ctx_index];
//保存所有的servers，可以看到是保存在main中的。这样子最后在HTTP main中就可以取到这个srv conf.
    cscfp = ngx_array_push(&cmcf->servers);
    if (cscfp == NULL) {
        return NGX_CONF_ERROR;
    }

    *cscfp = cscf;


    /* parse inside server{} */
//解析，可以看到设置type为srv_conf.
    pcf = *cf;
    cf->ctx = ctx;
    cf->cmd_type = NGX_HTTP_SRV_CONF;

    rv = ngx_conf_parse(cf, NULL);  // 调用ngx_conf_parse函数
//恢复cf.
    *cf = pcf;

  ........................
    }

    return rv;
}
```

> 
现在来分析上述代码


```cpp
struct ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
//conf就是对应的上下文偏移.比如NGX_HTTP_LOC_CONF_OFFSET
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    void                 *post;
};
............................
            else if (cf->ctx) {
       //取得对应的1级模块的二级上下文(HTTP的 srv_offset)
                confp = *(void **) ((char *) cf->ctx + cmd->conf);

                if (confp) {
         //然后取出对应的模块conf.
                    conf = confp[ngx_modules[i]->ctx_index];
                }
            }
```

> 
接下来 一些简单的命令如何使用和配置，主要 看这几个数据结构


```cpp
typedef struct {
    void        **main_conf;
    void        **srv_conf;
    void        **loc_conf;
} ngx_http_conf_ctx_t;
// 下面这些就是放到ngx_command_t的conf域，可以看到就是对应conf的偏移
#define NGX_HTTP_MAIN_CONF_OFFSET  offsetof(ngx_http_conf_ctx_t, main_conf)
#define NGX_HTTP_SRV_CONF_OFFSET   offsetof(ngx_http_conf_ctx_t, srv_conf)
#define NGX_HTTP_LOC_CONF_OFFSET   offsetof(ngx_http_conf_ctx_t, loc_conf)
// 

//下面就是如何来取模块的配置 
#define ngx_http_get_module_main_conf(r, module)                             \
    (r)->main_conf[module.ctx_index]
#define ngx_http_get_module_srv_conf(r, module)  (r)->srv_conf[module.ctx_index]
#define ngx_http_get_module_loc_conf(r, module)  (r)->loc_conf[module.ctx_index]
#define ngx_http_conf_get_module_main_conf(cf, module)                        \
    ((ngx_http_conf_ctx_t *) cf->ctx)->main_conf[module.ctx_index]
#define ngx_http_conf_get_module_srv_conf(cf, module)                         \
    ((ngx_http_conf_ctx_t *) cf->ctx)->srv_conf[module.ctx_index]
#define ngx_http_conf_get_module_loc_conf(cf, module)                         \
    ((ngx_http_conf_ctx_t *) cf->ctx)->loc_conf[module.ctx_index]

#define ngx_http_cycle_get_module_main_conf(cycle, module)                    \
    (cycle->conf_ctx[ngx_http_module.index] ?                                 \
        ((ngx_http_conf_ctx_t *) cycle->conf_ctx[ngx_http_module.index])      \
            ->main_conf[module.ctx_index]:                                    \
        NULL)
```

# 总结

并不是所有的模块像http有三个级别（main/srv/loc）比如stream和mail模块只有两个级别(main/srv)),但是整体解析流程都是 一致的，所以学习了一个模块，就可以很清楚其他模块，只是具体的handler不一样而已。接下来的博客将要介绍整个Nginx的整个框架 的流程




