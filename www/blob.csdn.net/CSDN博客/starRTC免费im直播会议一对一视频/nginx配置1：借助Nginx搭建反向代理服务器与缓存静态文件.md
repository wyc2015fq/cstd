# nginx配置1：借助Nginx搭建反向代理服务器与缓存静态文件 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月30日 19:45:15[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：443

##  修改配置文件nginx.conf
　　（1）进程数与每个进程的最大连接数：
　　　　•nginx进程数，建议设置为等于CPU总核心数
　　　　•单个进程最大连接数，那么该服务器的最大连接数=连接数*进程数
　　（2）Nginx的基本配置：
　　　　•监听端口一般都为http端口：80;
　　　　•域名可以有多个，用空格隔开：例如 server_name www.ha97.com
 ha97.com;
　　（3）负载均衡列表基本配置：
　　　　•location / {}：对aspx后缀的进行负载均衡请求，假如我们要对所有的aspx后缀的文件进行负载均衡时，可以这样写：location ~ .*\.aspx$ {}
　　　　•proxy_pass：请求转向自定义的服务器列表，这里我们将请求都转向标识为http://cuitccol.com的负载均衡服务器列表；
　　　　•在负载均衡服务器列表的配置中，weight是权重，可以根据机器配置定义权重（如果某台服务器的硬件配置十分好，可以处理更多的请求，那么可以为其设置一个比较高的weight；而有一台的服务器的硬件配置比较差，那么可以将前一台的weight配置为weight=2，后一台差的配置为weight=1）。weigth参数表示权值，权值越高被分配到的几率越大；

## 静态文件的缓存配置
　　（1）缓存静态资源之图片文件
　　root /nginx-1.4.7/staticresources/image：对于配置中提到的jpg/png等文件均定位到/nginx-1.4.7/staticresources/image文件夹中进行寻找匹配并将文件返回；
　　expires 7d：过期时效为7天，静态文件不怎么更新，过期时效可以设大一点，如果频繁更新，则可以设置得小一点；
　　TIPS：下面的样式、脚本缓存配置同这里一样，只是定位的文件夹不一样而已，不再赘述。
　　（2）缓存静态资源之样式文件
　　（3）缓存静态资源之脚本文件
 参考：
[http://www.cnblogs.com/edisonchou/p/4126742.html](http://www.cnblogs.com/edisonchou/p/4126742.html)
##  修改配置文件nginx.conf
　　（1）进程数与每个进程的最大连接数：
　　　　•nginx进程数，建议设置为等于CPU总核心数
　　　　•单个进程最大连接数，那么该服务器的最大连接数=连接数*进程数
　　（2）Nginx的基本配置：
　　　　•监听端口一般都为http端口：80;
　　　　•域名可以有多个，用空格隔开：例如 server_name www.ha97.com
 ha97.com;
　　（3）负载均衡列表基本配置：
　　　　•location / {}：对aspx后缀的进行负载均衡请求，假如我们要对所有的aspx后缀的文件进行负载均衡时，可以这样写：location ~ .*\.aspx$ {}
　　　　•proxy_pass：请求转向自定义的服务器列表，这里我们将请求都转向标识为http://cuitccol.com的负载均衡服务器列表；
　　　　•在负载均衡服务器列表的配置中，weight是权重，可以根据机器配置定义权重（如果某台服务器的硬件配置十分好，可以处理更多的请求，那么可以为其设置一个比较高的weight；而有一台的服务器的硬件配置比较差，那么可以将前一台的weight配置为weight=2，后一台差的配置为weight=1）。weigth参数表示权值，权值越高被分配到的几率越大；

## 静态文件的缓存配置
　　（1）缓存静态资源之图片文件
　　root /nginx-1.4.7/staticresources/image：对于配置中提到的jpg/png等文件均定位到/nginx-1.4.7/staticresources/image文件夹中进行寻找匹配并将文件返回；
　　expires 7d：过期时效为7天，静态文件不怎么更新，过期时效可以设大一点，如果频繁更新，则可以设置得小一点；
　　TIPS：下面的样式、脚本缓存配置同这里一样，只是定位的文件夹不一样而已，不再赘述。
　　（2）缓存静态资源之样式文件
　　（3）缓存静态资源之脚本文件
 参考：
[http://www.cnblogs.com/edisonchou/p/4126742.html](http://www.cnblogs.com/edisonchou/p/4126742.html)
