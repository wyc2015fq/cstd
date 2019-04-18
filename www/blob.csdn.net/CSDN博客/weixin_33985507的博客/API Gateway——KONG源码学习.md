# API Gateway——KONG源码学习 - weixin_33985507的博客 - CSDN博客
2016年12月13日 22:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：18
*kong源码，本文作者只阅读一少部分。kong的学习总结 需中止一段时间，暂时发布一些儿未完成的文章。以后有机会，再进一步补充完整。*
Kong基于nginx开发的，所以先看下ngnix的配置文件。作者在ubuntu安装Kong后，nginx.conf位于/usr/local/kong/目录下。由下图可知nginx.conf 包含了nginx-kong.conf(该文件跟nginx.conf位于同一目录)。
![3169646-94d932933e0df401.png](https://upload-images.jianshu.io/upload_images/3169646-94d932933e0df401.png)
nginx-kong.conf的部分配置如下
> 
lua_package_path '?/init.lua;./kong/?.lua;;';
lua_package_cpath ';;';
lua_shared_dict kong 4m;
lua_shared_dict cache 128m;
lua_shared_dict cache_locks 100k;
init_by_lua_block {
   require 'resty.core'
    kong = require 'kong'
    kong.init()
}
init_worker_by_lua_block {
      kong.init_worker()
}
由以上配置可知，lua和C文件的目录配置 （lua_package_path '?/init.lua;./kong/?.lua;;';lua_package_cpath ';;';）。Kong在启动nginx时，调用了kong.init()和kong.init_worker()。
以下是kong的对外server配置：
> 
server {
server_name kong;
listen 0.0.0.0:8000;
ssl_certificate_by_lua_block {kong.ssl_certificate()}
location / {
set $upstream_host nil; set $upstream_url nil;
access_by_lua_block {   kong.access()}
header_filter_by_lua_block {    kong.header_filter()}
body_filter_by_lua_block {    kong.body_filter()}
log_by_lua_block {   kong.log()}
}
location = /kong_error_handler {
  internal;
  content_by_lua_block {   require('kong.core.error_handlers')(ngx)  }
}
}
有以上配置可知，  “location /”拦截了所有请求，并进行了以下几个阶段的出来  access_by_lua_block（访问控制）、header_filter_by_lua_block（过滤请求头）、body_filter_by_lua_block（过滤http包体）、log_by_lua_block（打印日志），他们分别调用kong的对应方法。由“kong = require 'kong'”可知，此处变量kong 定义在kong.lua中。kong源码结构如下，可见kong.lua文件。
![3169646-6aa2168049d9295b.png](https://upload-images.jianshu.io/upload_images/3169646-6aa2168049d9295b.png)
kong.lua定义了init、init_worker、load_plugins、access、header_filter等方法。从名字可以看出各方法的含义。
后续内容待补充.....
