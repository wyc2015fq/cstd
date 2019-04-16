# Nginx源码分析 - 主流程篇 - 解析配置文件 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年07月14日 18:47:30[initphp](https://me.csdn.net/initphp)阅读数：5937
个人分类：[Nginx源码分析](https://blog.csdn.net/initphp/article/category/6081681)

所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









Nginx源码中比较重要的一块就是配置文件的解析。一般是解析/usr/local/nginx/conf/nginx.conf文件中的配置信息。

前一篇文章，我们介绍了Nginx的模块化。Nginx的功能模块都是通过**cycle->modules** 模块进行管理的。而每个模块都会有自己的配置文件。



### Nginx的配置文件nginx.conf



1. Nginx的配置文件每一行就是一条命令。

2. 最外层的为核心模块的配置参数（类型：NGX_CORE_MODULE）；内部嵌套的为各个子模块的配置。

3. events {} 为事件模块（类型：NGX_EVENT_MODULE）

4. http {} 为HTTP模块 （类型：NGX_HTTP_MODULE）

5. http模块内还会嵌套多层

6. **多层嵌套会在后续的event模块中介绍。这里只介绍最顶层的core模块的解析。**配置的层次结构如下图：



**               Main CODE**

**          /                     \**



**               Events                     Http**

**          /            \                        /            \**

**                          epoll            kqueue                 Server          Server**

**                                                                                  /             \                   \**

**                                                                                         Location     Location      Location**


```cpp
#user  nobody;
worker_processes  1;

#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;

#pid        logs/nginx.pid;


events {
    worker_connections  1024;
}


http {
    include       mime.types;
    default_type  application/octet-stream;

    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';

    #access_log  logs/access.log  main;

    sendfile        on;
    #tcp_nopush     on;

    #keepalive_timeout  0;
    keepalive_timeout  65;

    #gzip  on;

    server {
        listen       80;
        server_name  localhost;

        #charset koi8-r;

        #access_log  logs/host.access.log  main;

        location / {
            root   html;
            index  index.html index.htm;
        }

        #error_page  404              /404.html;

        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }

        # proxy the PHP scripts to Apache listening on 127.0.0.1:80
        #
        #location ~ \.php$ {
        #    proxy_pass   http://127.0.0.1;
        #}

        # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
        #
        #location ~ \.php$ {
        #    root           html;
        #    fastcgi_pass   127.0.0.1:9000;
        #    fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
        #    include        fastcgi_params;
        #}

        # deny access to .htaccess files, if Apache's document root
        # concurs with nginx's one
        #
        #location ~ /\.ht {
        #    deny  all;
        #}
    }


    # another virtual host using mix of IP-, name-, and port-based configuration
    #
    #server {
    #    listen       8000;
    #    listen       somename:8080;
    #    server_name  somename  alias  another.alias;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}


    # HTTPS server
    #
    #server {
    #    listen       443 ssl;
    #    server_name  localhost;

    #    ssl_certificate      cert.pem;
    #    ssl_certificate_key  cert.key;

    #    ssl_session_cache    shared:SSL:1m;
    #    ssl_session_timeout  5m;

    #    ssl_ciphers  HIGH:!aNULL:!MD5;
    #    ssl_prefer_server_ciphers  on;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}

}
```

### 数据结构逻辑关系图

![](https://img-blog.csdn.net/20160715163314086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 相关数据结构

#### ngx_cycle_s的conf_ctx和modules

cycle->conf_ctx ：配置文件上下文数组。每个模块的配置文件数据结构的指针地址都会按照**模块的index索引**放置在cycle->conf_ctx数组中。

cycle->modules：模块数组。模块也会按照模块的索引index放在cycle->modules数组上。具体可以参考[《Nginx源码分析 - 主流程篇 - 模块的初始化 》](http://blog.csdn.net/initphp/article/details/51898955)中ngx_cycle_modules方法。



```cpp
/**
 * Nginx全局变量cycle
 */
struct ngx_cycle_s {
    void                  ****conf_ctx; /* 配置文件 上下文的数组，每个模块的配置信息*/
    ngx_pool_t               *pool;	/* 内存池地址 */
   
    ...
    ngx_module_t            **modules;	/* 模块数组 */
    ngx_uint_t                modules_n;	/* 模块个数 */
    ngx_uint_t                modules_used;    /* unsigned  modules_used:1; */
    ...
};
```

#### ngx_module_s的index和commands

index：是模块的索引值。

commands：模块支持的命令集。主要用于将配置信息设置到每个模块的配置文件数据结构上（例如核心模块的ngx_core_conf_t）。



```cpp
/**
 * 业务模块数据结构
 */
struct ngx_module_s {
    ngx_uint_t            ctx_index;
    ngx_uint_t            index; /* 模块的唯一标识符号 */

    char                 *name;  /* 模块名称 */
    ...
    void                 *ctx;	/* 模块上下文 */
    ngx_command_t        *commands; /* 模块支持的命令集 */
    ngx_uint_t            type;	/* 模块类型 */
    ...
};
```

#### ngx_command_s 命令集的结构

set：为回调函数。最终设置值的时候，都会调用set的回调函数。




```cpp
/**
 * 模块支持的命令集结构
 */
struct ngx_command_s {
    ngx_str_t             name; /* 命令名称 */
    ngx_uint_t            type; /* 命令类别 */
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf); /* set回调函数 */
    ngx_uint_t            conf;
    ngx_uint_t            offset; /* 偏移量，命令长度 */
    void                 *post; /* 支持的回调方法；大多数情况为NULL*/
};
```




### 核心模块的定义

从上面的数据结构我们知道，每个模块都必须定义一个模块的数据结构ngx_module_s，主要用于管理每个模块的具体信息；每个模块也会定义一个ngx_command_t数组，主要用于存放需要解析的命令集的规则。

而最终的配置文件信息，由于每个模块的配置结构不同，所以在**cycle->conf_ctx**只保存**每个模块配置文件数据结构的指针地址**。

核心模块在nginx.c的文件头部


#### 核心模块



```cpp
/**
 * 定义核心配置模块命令集ngx_command_t结构
 */
static ngx_command_t  ngx_core_commands[] = {

    { ngx_string("daemon"), /* 命令名称 */
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG, /* 类型 */
      ngx_conf_set_flag_slot, /* 回调方法 */
      0,
      offsetof(ngx_core_conf_t, daemon), /* 偏移量；使用这个偏移量后，可以参考 ngx_core_module_create_conf*/
      NULL },

    { ngx_string("master_process"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      0,
      offsetof(ngx_core_conf_t, master),
      NULL },

    { ngx_string("timer_resolution"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_msec_slot,
      0,
      offsetof(ngx_core_conf_t, timer_resolution),
      NULL },

    { ngx_string("pid"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, pid),
      NULL },

    { ngx_string("lock_file"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, lock_file),
      NULL },

    { ngx_string("worker_processes"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_worker_processes,
      0,
      0,
      NULL },

    { ngx_string("debug_points"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_enum_slot,
      0,
      offsetof(ngx_core_conf_t, debug_points),
      &ngx_debug_points },

    { ngx_string("user"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE12,
      ngx_set_user,
      0,
      0,
      NULL },

    { ngx_string("worker_priority"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_priority,
      0,
      0,
      NULL },

    { ngx_string("worker_cpu_affinity"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_1MORE,
      ngx_set_cpu_affinity,
      0,
      0,
      NULL },

    { ngx_string("worker_rlimit_nofile"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_nofile),
      NULL },

    { ngx_string("worker_rlimit_core"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_off_slot,
      0,
      offsetof(ngx_core_conf_t, rlimit_core),
      NULL },

    { ngx_string("working_directory"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      0,
      offsetof(ngx_core_conf_t, working_directory),
      NULL },

    { ngx_string("env"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_set_env,
      0,
      0,
      NULL },

    { ngx_string("load_module"),
      NGX_MAIN_CONF|NGX_DIRECT_CONF|NGX_CONF_TAKE1,
      ngx_load_module,
      0,
      0,
      NULL },

      ngx_null_command
};

/**
 * 核心模块上下文
 * ngx_core_module_create_conf 核心模块创建配置文件
 * ngx_core_module_init_conf 核心模块初始化配置文件
 */
static ngx_core_module_t  ngx_core_module_ctx = {
    ngx_string("core"),
    ngx_core_module_create_conf,
    ngx_core_module_init_conf
};

/**
 * 核心模块
 */
ngx_module_t  ngx_core_module = {
    NGX_MODULE_V1,
    &ngx_core_module_ctx,                  /* module context */
    ngx_core_commands,                     /* module directives */
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

说明：


1. ngx_core_module 核心模块。主要管理核心模块的信息。

2. ngx_core_module_ctx 核心模块的上下文。每个模块的上下文都不同，所以**模块结构中只给了一个void的指针类型，可以指向不同的数据结构**。核心模块的ngx_core_module_ctx主要定义了ngx_core_module_create_conf和ngx_core_module_init_conf回调函数（创建和初始化配置文件）

3. ngx_core_commands 核心模块定义的命令集。当nginx.conf中的命令被拆简后，会通过这个命令集，逐个将核心模块的命令赋值到核心模块的配置文件数据结构上。

#### 核心模块的配置结构ngx_core_conf_t

核心模块的配置结构ngx_core_conf_t。**ngx_core_conf_t的指针地址会按照模块的index索引，放在cycle->conf_ctx数组中。**

ngx_core_conf_t结构的创建和初始化，ngx_core_module_create_conf和ngx_core_module_init_conf方法。



```cpp
/**
 * 核心配置文件信息
 * 对应nginx.conf的
 * #user  nobody;
	worker_processes  1;

	#error_log  logs/error.log;
	#error_log  logs/error.log  notice;
	#error_log  logs/error.log  info;

	#pid        logs/nginx.pid;
 */
typedef struct {
    ngx_flag_t                daemon;
    ngx_flag_t                master;

    ngx_msec_t                timer_resolution;

    ngx_int_t                 worker_processes;
    ngx_int_t                 debug_points;

    ngx_int_t                 rlimit_nofile;
    off_t                     rlimit_core;

    int                       priority;

    ngx_uint_t                cpu_affinity_auto;
    ngx_uint_t                cpu_affinity_n;
    ngx_cpuset_t             *cpu_affinity;

    char                     *username;
    ngx_uid_t                 user;
    ngx_gid_t                 group;

    ngx_str_t                 working_directory;
    ngx_str_t                 lock_file;

    ngx_str_t                 pid;
    ngx_str_t                 oldpid;

    ngx_array_t               env;
    char                    **environment;
} ngx_core_conf_t;
```

### 具体解析流程

我们这边主要讲解**核心模块配置信息的解析**。通过对核心模块的解析流程的理解，能更好的帮助你了解整个Nginx的模块管理和配置管理的流程。HTTP等模块的配置文件解析会更加复杂一些，但是基本原理是一致的。

#### 1. 创建核心模块配置文件数据结构ngx_core_conf_t

ngx_init_cycle全局变量的初始化中会初始化Nginx的核心模块的配置信息。核心模块的配置参数在nginx.conf文件中最顶层的一些参数配置。

下面这段代码我们可以看到，遍历模块数组，如果是核心模块，则**获取核心模块的上下文cycle->modules[i]->ctx**，核心模块上下文是一个自定义的数据结构**ngx_core_module_ctx**，里面包含了配置文件创建的回调函数ngx_core_module_create_conf 

**PS：这边只是针对性NGX_CORE_MODULE进行创建配置文件和初始化配置文件。**




```cpp
/*
     * 核心模块的配置文件创建
     * 配置创建调用nginx.c 中的 ngx_core_module_create_conf
     * 以及其他核心模块的init_conf，例如：ngx_event_core_module_ctx中的ngx_event_core_create_conf
     * */
    for (i = 0; cycle->modules[i]; i++) {
        if (cycle->modules[i]->type != NGX_CORE_MODULE) {
            continue;
        }

        module = cycle->modules[i]->ctx;

        if (module->create_conf) {
            rv = module->create_conf(cycle); //模块回调函数，创建模块的配置信息
            if (rv == NULL) {
                ngx_destroy_pool(pool);
                return NULL;
            }
            cycle->conf_ctx[cycle->modules[i]->index] = rv; //配置文件复制
        }
    }
```



#### 2. 创建一个临时的ngx_conf_s结构

ngx_conf_s主要定义了需要解析的配置文件，需要解析的模块类型以及命令集类型的信息。**辅助解析配置文件。**




```cpp
/* 解析配置文件 */
    ngx_memzero(&conf, sizeof(ngx_conf_t));
    /* STUB: init array ? */
    conf.args = ngx_array_create(pool, 10, sizeof(ngx_str_t));
    if (conf.args == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }

    /* 创建一个临时的内存池，后面会清空掉;conf也主要用于解析配置文件的临时变量 */
    conf.temp_pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE, log);
    if (conf.temp_pool == NULL) {
        ngx_destroy_pool(pool);
        return NULL;
    }


    conf.ctx = cycle->conf_ctx;
    conf.cycle = cycle;
    conf.pool = pool;
    conf.log = log;
    conf.module_type = NGX_CORE_MODULE; /* 配置文件模块类型 */
    conf.cmd_type = NGX_MAIN_CONF; /* 命令集类型 */
```

#### 3. 调用解析配置函数ngx_conf_param和ngx_conf_parse

1. ngx_conf_param：主要解析命令行中的核心模块配置参数，例如：nginx -t -c /usr/local/nginx/conf/nginx.conf 
2. ngx_conf_parse：主要解析配置文件/usr/local/nginx/conf/nginx.conf 信息

3. ngx_conf_param：最终也是调用ngx_conf_parse



```cpp
/* 解析命令行中的配置参数；例如：nginx -t -c /usr/local/nginx/conf/nginx.conf */
    if (ngx_conf_param(&conf) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        return NULL;
    }

    /* 解析配置文件/usr/local/nginx/conf/nginx.conf 信息 */
    if (ngx_conf_parse(&conf, &cycle->conf_file) != NGX_CONF_OK) {
        environ = senv;
        ngx_destroy_cycle_pools(&conf);
        return NULL;
    }
```



#### 4. 配置文件核心解析函数ngx_conf_parse

ngx_conf_parse解析命令中的配置信息和文件中的配置信息.

**主要工作是按行读取配置文件，并且解析成配置token数组，并将token数组进行模块commend命令集匹配和设置。**

其中比较重要的函数：

1. ngx_conf_read_token 按行读取配置文件，并将命令解析成token数组cf->args；


2. ngx_conf_handler 将命令token数组与模块命令集匹配并设置到模块配置文件数据结构上。




```cpp
/**
 * 解析配置信息核心函数
 * 包含：解析命令中的配置信息和文件中的配置信息
 */
char *
ngx_conf_parse(ngx_conf_t *cf, ngx_str_t *filename)
{
  ...

    /* 打开配置文件： /usr/local/nginx/conf/nginx.conf */
    if (filename) {

        /* open configuration file */

        fd = ngx_open_file(filename->data, NGX_FILE_RDONLY, NGX_FILE_OPEN, 0); //只读方式打开文件
        if (fd == NGX_INVALID_FILE) {
            ngx_conf_log_error(NGX_LOG_EMERG, cf, ngx_errno,
                               ngx_open_file_n " \"%s\" failed",
                               filename->data);
            return NGX_CONF_ERROR;
        }

        prev = cf->conf_file;

        cf->conf_file = &conf_file;

        if (ngx_fd_info(fd, &cf->conf_file->file.info) == NGX_FILE_ERROR) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, ngx_errno,
                          ngx_fd_info_n " \"%s\" failed", filename->data);
        }

        /* 配置文件buf，默认大小4096 */
        cf->conf_file->buffer = &buf;

        buf.start = ngx_alloc(NGX_CONF_BUFFER, cf->log);
        if (buf.start == NULL) {
            goto failed;
        }

        buf.pos = buf.start;
        buf.last = buf.start;
        buf.end = buf.last + NGX_CONF_BUFFER;
        buf.temporary = 1;

        /* 读取配置文件数据，保存到cf->conf_file中 */
        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.data = filename->data;
        cf->conf_file->file.offset = 0;
        cf->conf_file->file.log = cf->log;
        cf->conf_file->line = 1;

        type = parse_file;

        ...
    } else if (cf->conf_file->file.fd != NGX_INVALID_FILE) {

        /**
         * 解析块：events {  worker_connections  1024; }
         */
        type = parse_block;

    } else {
        type = parse_param;
    }

    for ( ;; ) {

    	/* 将配置信息解析成 token；仅仅是将配置文件的数据解析成一个个的单词，按行解析 */
        rc = ngx_conf_read_token(cf);

        /*
         * ngx_conf_read_token() may return
         *
         *    NGX_ERROR             there is error 解析失败
         *    NGX_OK                the token terminated by ";" was found  遇到结尾符号;，则表示解析成功
         *    NGX_CONF_BLOCK_START  the token terminated by "{" was found  遇到{模块配置开始标识
         *    NGX_CONF_BLOCK_DONE   the "}" was found	遇到模块}结束标识
         *    NGX_CONF_FILE_DONE    the configuration file is done 遇到文件解析完毕
         */

        if (rc == NGX_ERROR) {
            goto done;
        }

        /* 一个模块解析结束，则跳到done模块代码 */
        if (rc == NGX_CONF_BLOCK_DONE) {

            if (type != parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"}\"");
                goto failed;
            }

            goto done;
        }

        /* 一个模块解析结束，则跳到done模块代码 */
        if (rc == NGX_CONF_FILE_DONE) {

            if (type == parse_block) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "unexpected end of file, expecting \"}\"");
                goto failed;
            }

            goto done;
        }

        if (rc == NGX_CONF_BLOCK_START) {

            if (type == parse_param) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "block directives are not supported "
                                   "in -g option");
                goto failed;
            }
        }

        /* rc == NGX_OK || rc == NGX_CONF_BLOCK_START */

        /* 当遇到 NGX_CONF_BLOCK_START 和  NGX_OK*/
        if (cf->handler) {

            /*
             * the custom handler, i.e., that is used in the http's
             * "types { ... }" directive
             */

            if (rc == NGX_CONF_BLOCK_START) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "unexpected \"{\"");
                goto failed;
            }

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

        /* 配置文件处理；将配置文件设置到模块上 */
        rc = ngx_conf_handler(cf, rc);

        if (rc == NGX_ERROR) {
            goto failed;
        }
    }

failed:
...

    return NGX_CONF_OK;
}
```

#### 5. 命令token解析 ngx_conf_read_token

ngx_conf_read_token 主要是将配置文件分解成逐个的单词数组。例如配置文件中遇到“空格”则为分隔符，“;”为结束符。

每一个数组，就是一条配置命令语句。数组会放置到**cf->args**数组上。

nginx.conf配置文件如下：



```cpp
user  nfsnobody nfsnobody;
worker_processes 8;
error_log  /usr/local/nginx-1.4.7/nginx_error.log  crit;
pid        /usr/local/nginx-1.4.7/nginx.pid;
#Specifies the value for maximum file descriptors that can be opened by this process.
worker_rlimit_nofile 65535;
events
{
use epoll;
worker_connections 65535;}
```
解析成下面的格式：


```cpp
#分解成逐个单词数组：
user
nfsnobody
nfsnobody

worker_processes
8

error_log
/usr/local/nginx-1.4.7/nginx_error.log
crit

pid
/usr/local/nginx-1.4.7/nginx.pid

worker_rlimit_nofile
65535

events
```
具体逻辑实现：


```cpp
/**
 * 读取配置信息
 * 把每次分析的值放到cf->args这个数组里面	碰到{} ; 返回
 * 例如配置文件如下：
 * user  nfsnobody nfsnobody;
 * worker_processes 8;
 * error_log  /usr/local/nginx-1.4.7/nginx_error.log  crit;
 * pid        /usr/local/nginx-1.4.7/nginx.pid;
 * #Specifies the value for maximum file descriptors that can be opened by this process.
 * worker_rlimit_nofile 65535;
 * events
 * {
 * use epoll;
 * worker_connections 65535;}
 *
 * 分解成逐个单词：
 * user
 * nfsnobody
 * nfsnobody
 * worker_processes
 * 8
 * error_log
 * /usr/local/nginx-1.4.7/nginx_error.log
 * crit
 * pid
 * /usr/local/nginx-1.4.7/nginx.pid
 * worker_rlimit_nofile
 * 65535
 * events
 *
 */
static ngx_int_t
ngx_conf_read_token(ngx_conf_t *cf)
{
    u_char      *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    ngx_uint_t   found, need_space, last_space, sharp_comment, variable;
    ngx_uint_t   quoted, s_quoted, d_quoted, start_line;
    ngx_str_t   *word;
    ngx_buf_t   *b, *dump;

    found = 0; //表示找到一个 token
    need_space = 0;
    last_space = 1; //标志位,表示上一个字符为token分隔符
    sharp_comment = 0; //注释 #符号
    variable = 0; //变量符号 $
    quoted = 0;   //标志位,表示上一个字符为反斜杠
    s_quoted = 0; //标志位,表示已扫描一个双引号,期待另一个双引号
    d_quoted = 0; //标志位,表示已扫描一个单引号,期待另一个单引号

    cf->args->nelts = 0;
    b = cf->conf_file->buffer; //buffer 每次4096
    dump = cf->conf_file->dump;
    start = b->pos;
    start_line = cf->conf_file->line;

    file_size = ngx_file_size(&cf->conf_file->file.info);

    for ( ;; ) {

    	/* buf中的数据已经处理完毕，则需要判断是否文件读取完了，如果没有读取完，则继续解析配置文件 */
        if (b->pos >= b->last) {

        	/* 文件已经读取完毕，返回NGX_CONF_FILE_DONE */
            if (cf->conf_file->file.offset >= file_size) {

                if (cf->args->nelts > 0 || !last_space) {

                    if (cf->conf_file->file.fd == NGX_INVALID_FILE) {
                        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                           "unexpected end of parameter, "
                                           "expecting \";\"");
                        return NGX_ERROR;
                    }

                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                  "unexpected end of file, "
                                  "expecting \";\" or \"}\"");
                    return NGX_ERROR;
                }

                return NGX_CONF_FILE_DONE;
            }

            /* buf中已经使用的长度  */
            len = b->pos - start;

            /* 如果len=4096 则表明buf全部读取满了；如果读取了4096个字符，还是没有发现"和'的标示符号，则认为读取失败，参数太长了 */
            if (len == NGX_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                if (d_quoted) {
                    ch = '"';

                } else if (s_quoted) {
                    ch = '\'';

                } else {
                    ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                       "too long parameter \"%*s...\" started",
                                       10, start);
                    return NGX_ERROR;
                }

                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   "too long parameter, probably "
                                   "missing terminating \"%c\" character", ch);
                return NGX_ERROR;
            }

            /* 将数据移动到buf的头部 */
            if (len) {
                ngx_memmove(b->start, start, len);
            }

            /* 如果buf有空闲，则继续读取文件中的数据到buf中 */
            size = (ssize_t) (file_size - cf->conf_file->file.offset);

            if (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = ngx_read_file(&cf->conf_file->file, b->start + len, size,
                              cf->conf_file->file.offset);

            if (n == NGX_ERROR) {
                return NGX_ERROR;
            }

            if (n != size) {
                ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                                   ngx_read_file_n " returned "
                                   "only %z bytes instead of %z",
                                   n, size);
                return NGX_ERROR;
            }

            /* 设置b->pos和b->last的位置，并重新设置start的位置 */
            b->pos = b->start + len;
            b->last = b->pos + n;
            start = b->start;

            if (dump) {
                dump->last = ngx_cpymem(dump->last, b->pos, size);
            }
        }

        /* ch字符用于读取配置文件信息 */
        ch = *b->pos++;

        /* 如果遇到换行符号 \n */
        if (ch == LF) {
            cf->conf_file->line++;

            /* 判断改行是否是注释 如果遇到\n结尾，并且是注释，则设置sharp_comment = 0；当sharp_comment=1 则注释字符不处理*/
            if (sharp_comment) {
                sharp_comment = 0;
            }
        }

        /* 注释，则直接跳过 */
        if (sharp_comment) {
            continue;
        }

        /* 如果为反引号,则设置反引号标识,并且不对该字符进行解析   */
        if (quoted) {
            quoted = 0;
            continue;
        }

        /* 上一个字符为单引号或者双引号,期待一个分隔符   */
        if (need_space) {

           ...
        }
    }
}
```

#### 设值到模块配置文件数据结构上 ngx_conf_handler

ngx_conf_handler方法主要是将拿到的token数组cf->args，按照模块的命令集**cycle->modules[i]->commands** 设置值。（后面解析event和http头部核心模块的时候，会用到）

1. 通过模块的index索引值，拿到cycle->ctx 模块配置文件数据结构。

2. 通过rv = cmd->set(cf, cmd, conf)，调用命令集中定义的设值值的回调方法。

**这边会根据模块类型，将配置信息逐个解析****到相应的模块上。**




```cpp
/**
 * 配置文件处理
 * cycle->modules
 */
static ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    char           *rv;
    void           *conf, **confp;
    ngx_uint_t      i, found;
    ngx_str_t      *name;
    ngx_command_t  *cmd;

    name = cf->args->elts;

    found = 0;

    /* 循环配置模块 */
    for (i = 0; cf->cycle->modules[i]; i++) {

        cmd = cf->cycle->modules[i]->commands;
        if (cmd == NULL) {
            continue;
        }

        for ( /* void */ ; cmd->name.len; cmd++) {

            if (name->len != cmd->name.len) {
                continue;
            }

            /* 检查配置名称和token的第一个元素的名称是否一致，如果不一致，则说明命令不一样 */
            if (ngx_strcmp(name->data, cmd->name.data) != 0) {
                continue;
            }

            found = 1;

            if (cf->cycle->modules[i]->type != NGX_CONF_MODULE
                && cf->cycle->modules[i]->type != cf->module_type)
            {
                continue;
            }
            ...

            /* is the directive's argument count right ? */

            if (!(cmd->type & NGX_CONF_ANY)) {
            ...
            }

            /* set up the directive's configuration context */

            conf = NULL;

            /* 设置配置文件的值  设置配置项对外面的配置信息； */
            if (cmd->type & NGX_DIRECT_CONF) {
            	/* ngx_core_module; */
                conf = ((void **) cf->ctx)[cf->cycle->modules[i]->index];

            } else if (cmd->type & NGX_MAIN_CONF) {
                conf = &(((void **) cf->ctx)[cf->cycle->modules[i]->index]);

            } else if (cf->ctx) {
                confp = *(void **) ((char *) cf->ctx + cmd->conf);

                if (confp) {
                    conf = confp[cf->cycle->modules[i]->ctx_index];
                }
            }

            /**
             * 配置文件设置值；
             * conf为配置的指针地址;
             * cmd为命令结构；
             * conf为配置指针地址 一般情况下 conf为模块自定义的配置文件数据结构地址
             *
             */
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
    }

    ...

    return NGX_ERROR;
}
```
nginx_conf_file.c文件中定义了多种配置文件值设值的方法：




```cpp
char *ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_str_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_str_array_slot(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);
char *ngx_conf_set_keyval_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_num_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_size_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_off_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_msec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_sec_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_bufs_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_enum_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
char *ngx_conf_set_bitmask_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
```



#### 调用核心模块的配置

通过核心模块ngx_core_module的索引index值，就可以得到核心配置的数据结构 ngx_core_conf_t



```cpp
/* 获取核心配置文件的数据结构 ngx_core_conf_t */
    ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);
```












































