# Nginx源码分析 - HTTP模块篇 - ngx_http_block函数和HTTP模块的初始化 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年12月17日 10:14:52[initphp](https://me.csdn.net/initphp)阅读数：2226
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









前面几章整理了Nginx的事件模块，这一章开始整理HTTP模块。

HTTP模块的初始化工作，都在**src/http/nginx_http.c 中的ngx_http_block函数中进行的。**


### http模块的配置

在看nginx_http_block之前，我们还必须看一下nginx.conf中 HTTP大模块的配置文件，只有理解了HTTP模块的配置，才能更好的理解HTTP模块如何初始化的。

配置如下：



```cpp
http 
{
  include       mime.types;
  default_type  application/octet-stream;

  #charset  gb2312;
      
  server_names_hash_bucket_size 128;
  client_header_buffer_size 32k;
  large_client_header_buffers 4 32k;
  client_max_body_size 8m;
      
  sendfile on;
  tcp_nopush     on;

  keepalive_timeout 60;

  tcp_nodelay on;

  fastcgi_connect_timeout 300;
  fastcgi_send_timeout 300;
  fastcgi_read_timeout 300;
  fastcgi_buffer_size 64k;
  fastcgi_buffers 4 64k;
  fastcgi_busy_buffers_size 128k;
  fastcgi_temp_file_write_size 128k;

  gzip on;
  gzip_min_length  1k;
  gzip_buffers     4 16k;
  gzip_http_version 1.0;
  gzip_comp_level 2;
  gzip_types       text/plain application/x-javascript text/css application/xml;
  gzip_vary on;

  #limit_zone  crawler  $binary_remote_addr  10m;

  server
  {
    listen       80;
    #server_name  blog.s135.com;
    index index.html index.htm index.php;
    root   /home/wwwroot/;

    #limit_conn   crawler  20;    
                             
    location ~ .*\.(php|php5)?$
    {      
      #fastcgi_pass  unix:/tmp/php-cgi.sock;
      fastcgi_pass  127.0.0.1:9000;
      fastcgi_index index.php;
      include fcgi.conf;
    }
    
    location ~ .*\.(gif|jpg|jpeg|png|bmp|swf)$
    {
      expires      30d;
    }

    location ~ .*\.(js|css)?$
    {
      expires      1h;
    }    

    log_format  access  '$remote_addr - $remote_user [$time_local] "$request" '
              '$status $body_bytes_sent "$http_referer" '
              '"$http_user_agent" $http_x_forwarded_for';
    access_log  /usr/local/nginx-1.4.7/logs/access.log  access;
      }

  server
  {
    listen  80;
    server_name  status.blog.s135.com;
    location / {
    stub_status on;
    access_log   off;
    }
  }
```



从上面的配置文件我们可以看到，HTTP的配置主要分为4层：

1. 最外层的http{} 模块。模块类型：NGX_CORE_MODULE （最外层的核心模块）


2. http核心模块中的main配置：http{include mine type; }。模块类型：NGX_HTTP_MODULE （全局的HTTP模块的配置信息）


3. http核心模块中的server配置：server{}。模块类型：NGX_HTTP_MODULE （主要是配置Server的信息）


4. http核心模块中的location本地信息配置：location{}。模块类型：NGX_HTTP_MODULE （主要一个server对应的本地资源信息：静态资源、反向代理端口地址、各种语言容器端口等）


**最外层的http模块，类型NGX_CORE_MODULE，属于核心模块，核心模块在最开始配置文件初始化的时候，就会调用指令的命令集。所以在核心模块启动的时候就会调用http的模块配置解析指令函数：ngx_http_block**





### HTTP核心模块的数据结构



```cpp
/**
 * HTTP模块命令集
 * HTTP模块也是一个大模块，最外层为：
 * http {
 * ....
 * }
 * ngx_http_block:该方法就是回调函数
 * HTTP核心模块
 */
static ngx_command_t  ngx_http_commands[] = {

    { ngx_string("http"),
      NGX_MAIN_CONF|NGX_CONF_BLOCK|NGX_CONF_NOARGS,
      ngx_http_block,
      0,
      0,
      NULL },

      ngx_null_command
};

/**
 *HTTP核心模块上下文
 */
static ngx_core_module_t  ngx_http_module_ctx = {
    ngx_string("http"),
    NULL,
    NULL
};

/**
 * HTTP核心模块 结构
 * 模块类型：NGX_CORE_MODULE
 * 通过调用ngx_http_block方法，解析{}中的HTTP模块配置
 */
ngx_module_t  ngx_http_module = {
    NGX_MODULE_V1,
    &ngx_http_module_ctx,                  /* module context */
    ngx_http_commands,                     /* module directives */
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

从上面的结构中可以看到http{} 是NGX_CORE_MODULE，在核心模块初始化的时候，会调用ngx_http_commands命令集中的回调函数，逐个解析核心模块的配置信息。


而HTTP模块的总入口就是这个http{}命令集的回调函数：ngx_http_block

如果对配置文件如何解析有遗忘，请回顾[《 Nginx源码分析 - 主流程篇 - 解析配置文件 》](http://blog.csdn.net/initphp/article/details/51911189)




### ngx_http_block函数详解



```cpp
/**
 *ngx_http_commands 命令集的回调函数
 *HTTP模块初始化的入口函数
 *
 */
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

    if (*(ngx_http_conf_ctx_t **) conf) {
        return "is duplicate";
    }

    /* the main http context */

    /* 分配一块内存，存放http配置上下文 */
    ctx = ngx_pcalloc(cf->pool, sizeof(ngx_http_conf_ctx_t));
    if (ctx == NULL) {
        return NGX_CONF_ERROR;
    }

    *(ngx_http_conf_ctx_t **) conf = ctx;


    /* count the number of the http modules and set up their indices */

    /* 计算http模块个数 */
    ngx_http_max_module = ngx_count_modules(cf->cycle, NGX_HTTP_MODULE);


    /* the http main_conf context, it is the same in the all http contexts */

    /**
     * 最外层的HTTP配置
     * http
	  {
	  include       mime.types;
	  default_type  application/octet-stream;
     */
    ctx->main_conf = ngx_pcalloc(cf->pool,
                                 sizeof(void *) * ngx_http_max_module);
    if (ctx->main_conf == NULL) {
        return NGX_CONF_ERROR;
    }


    /*
     * the http null srv_conf context, it is used to merge
     * the server{}s' srv_conf's
     */
    /**
     * server层的配置
     *   server
  {
    listen       80;
    #server_name  blog.s135.com;
    index index.html index.htm index.php;
    root   /home/wwwroot/;
     */
    ctx->srv_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
    if (ctx->srv_conf == NULL) {
        return NGX_CONF_ERROR;
    }


    /*
     * the http null loc_conf context, it is used to merge
     * the server{}s' loc_conf's
     */

    /**
     * location 层的配置
    location ~ .*\.(php|php5)?$
    {
      #fastcgi_pass  unix:/tmp/php-cgi.sock;
      fastcgi_pass  127.0.0.1:9000;
      fastcgi_index index.php;
      include fcgi.conf;
    }
     */
    ctx->loc_conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_http_max_module);
    if (ctx->loc_conf == NULL) {
        return NGX_CONF_ERROR;
    }


    /*
     * create the main_conf's, the null srv_conf's, and the null loc_conf's
     * of the all http modules
     */
     /**
      * 调用：create_main_conf、create_srv_conf、create_loc_conf
      * 创建配置
      */
    for (m = 0; cf->cycle->modules[m]; m++) {
        if (cf->cycle->modules[m]->type != NGX_HTTP_MODULE) {
            continue;
        }

        module = cf->cycle->modules[m]->ctx;
        mi = cf->cycle->modules[m]->ctx_index;

        if (module->create_main_conf) {
            ctx->main_conf[mi] = module->create_main_conf(cf);
            if (ctx->main_conf[mi] == NULL) {
                return NGX_CONF_ERROR;
            }
        }

        if (module->create_srv_conf) {
            ctx->srv_conf[mi] = module->create_srv_conf(cf);
            if (ctx->srv_conf[mi] == NULL) {
                return NGX_CONF_ERROR;
            }
        }

        if (module->create_loc_conf) {
            ctx->loc_conf[mi] = module->create_loc_conf(cf);
            if (ctx->loc_conf[mi] == NULL) {
                return NGX_CONF_ERROR;
            }
        }
    }

    pcf = *cf;
    cf->ctx = ctx;

    /**
     * preconfiguration 预先初始化配置信息
     */
    for (m = 0; cf->cycle->modules[m]; m++) {
        if (cf->cycle->modules[m]->type != NGX_HTTP_MODULE) {
            continue;
        }

        module = cf->cycle->modules[m]->ctx;

        if (module->preconfiguration) {
            if (module->preconfiguration(cf) != NGX_OK) {
                return NGX_CONF_ERROR;
            }
        }
    }

    /* parse inside the http{} block */

    cf->module_type = NGX_HTTP_MODULE;
    cf->cmd_type = NGX_HTTP_MAIN_CONF;
    rv = ngx_conf_parse(cf, NULL);

    if (rv != NGX_CONF_OK) {
        goto failed;
    }

    /*
     * init http{} main_conf's, merge the server{}s' srv_conf's
     * and its location{}s' loc_conf's
     */

    /**
     * 初始化main配置
     * 合并 server srv_conf
     * 合并location loc_conf
     */
    cmcf = ctx->main_conf[ngx_http_core_module.ctx_index];
    cscfp = cmcf->servers.elts;

    for (m = 0; cf->cycle->modules[m]; m++) {
        if (cf->cycle->modules[m]->type != NGX_HTTP_MODULE) {
            continue;
        }

        module = cf->cycle->modules[m]->ctx;
        mi = cf->cycle->modules[m]->ctx_index;

        /* init http{} main_conf's */

        if (module->init_main_conf) {
            rv = module->init_main_conf(cf, ctx->main_conf[mi]);
            if (rv != NGX_CONF_OK) {
                goto failed;
            }
        }

        rv = ngx_http_merge_servers(cf, cmcf, module, mi);
        if (rv != NGX_CONF_OK) {
            goto failed;
        }
    }


    /* create location trees */

    /**
     * 创建 location模块的trees
     */
    for (s = 0; s < cmcf->servers.nelts; s++) {

        clcf = cscfp[s]->ctx->loc_conf[ngx_http_core_module.ctx_index];

        if (ngx_http_init_locations(cf, cscfp[s], clcf) != NGX_OK) {
            return NGX_CONF_ERROR;
        }

        if (ngx_http_init_static_location_trees(cf, clcf) != NGX_OK) {
            return NGX_CONF_ERROR;
        }
    }


    if (ngx_http_init_phases(cf, cmcf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    if (ngx_http_init_headers_in_hash(cf, cmcf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }


    for (m = 0; cf->cycle->modules[m]; m++) {
        if (cf->cycle->modules[m]->type != NGX_HTTP_MODULE) {
            continue;
        }

        module = cf->cycle->modules[m]->ctx;

        if (module->postconfiguration) {
            if (module->postconfiguration(cf) != NGX_OK) {
                return NGX_CONF_ERROR;
            }
        }
    }

    if (ngx_http_variables_init_vars(cf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    /*
     * http{}'s cf->ctx was needed while the configuration merging
     * and in postconfiguration process
     */

    *cf = pcf;


    if (ngx_http_init_phase_handlers(cf, cmcf) != NGX_OK) {
        return NGX_CONF_ERROR;
    }


    /* optimize the lists of ports, addresses and server names */

    /* ngx_http_optimize_servers 初始化listen 端口号 ip地址 服务器等监听信息*/
    if (ngx_http_optimize_servers(cf, cmcf, cmcf->ports) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;

failed:

    *cf = pcf;

    return rv;
}
```

下一章会详细讲解：**ngx_http_optimize_servers**











