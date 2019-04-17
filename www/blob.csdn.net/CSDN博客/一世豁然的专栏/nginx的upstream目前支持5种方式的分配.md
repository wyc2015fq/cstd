# nginx的upstream目前支持5种方式的分配 - 一世豁然的专栏 - CSDN博客





2016年08月12日 16:22:48[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1529








本博客转载于：[http://blog.chinaunix.net/uid-20662363-id-3049712.html](http://blog.chinaunix.net/uid-20662363-id-3049712.html)









nginx的upstream目前支持5种方式的分配 

**1、轮询（默认）**


每个请求按时间顺序逐一分配到不同的后端服务器，如果后端服务器down掉，能自动剔除。 

**2、weight **指定轮询几率，weight和访问比率成正比，用于后端服务器性能不均的情况。 

例如： 

upstream bakend { 

server 192.168.0.14 weight=10; 

server 192.168.0.15 weight=10; 

} 

**3、ip_hash **每个请求按访问ip的hash结果分配，这样每个访客固定访问一个后端服务器，可以解决session的问题。 

例如： 

upstream bakend { 

ip_hash; 

server 192.168.0.14:88; 

server 192.168.0.15:80; 

} 

**4、fair（第三方）**

按后端服务器的响应时间来分配请求，响应时间短的优先分配。 

upstream backend { 

server server1; 

server server2; 

fair; 

} 

**5、url_hash（第三方）**


按访问url的hash结果来分配请求，使每个url定向到同一个后端服务器，后端服务器为缓存时比较有效。 


例：在upstream中加入hash语句，server语句中不能写入weight等其他的参数，hash_method是使用的hash算法 


upstream backend { 

server squid1:3128; 

server squid2:3128; 

hash $request_uri; 

hash_method crc32; 

} 


tips: 


upstream bakend{#定义负载均衡设备的Ip及设备状态 

ip_hash; 

server 127.0.0.1:9090 down; 

server 127.0.0.1:8080 weight=2; 

server 127.0.0.1:6060; 

server 127.0.0.1:7070 backup; 

} 

在需要使用负载均衡的server中增加 

proxy_pass http://bakend/; 


每个设备的状态设置为: 

1.down 表示单前的server暂时不参与负载 

2.weight 默认为1.weight越大，负载的权重就越大。 

3.max_fails ：允许请求失败的次数默认为1.当超过最大次数时，返回proxy_next_upstream 模块定义的错误 

4.fail_timeout:max_fails次失败后，暂停的时间。 

5.backup： 其它所有的非backup机器down或者忙的时候，请求backup机器。所以这台机器压力会最轻。 


nginx支持同时设置多组的负载均衡，用来给不用的server来使用。 


client_body_in_file_only 设置为On 可以讲client post过来的数据记录到文件中用来做debug 

client_body_temp_path 设置记录文件的目录 可以设置最多3层目录 


location 对URL进行匹配.可以进行重定向或者进行新的代理 负载均衡

要用nginx做负载均衡的话，首先要在配置文件里面定义一组用来负载均衡的后端服务器（backend servers），例如：
upstream backend {

  server 192.168.1.11;

  server 192.168.1.12;

  server 192.168.1.13;

}

那个server指令的语法是 server name [parameters]，这里的name是服务器名，可以是域名、ip或者unix socket，也可以指定端口，例如：
server 192.168.1.11:8080;

server指令可用的参数有：

weight —— 设置服务器的权重，默认值是1，权重值越大那么该服务器被访问到的几率就越大，例如 
server 192.168.1.11 weight=5;

max_fails和fail_timeout —— 这俩是关联的，如果某台服务器在fail_timeout时间内出现了max_fails次连接失败，那么nginx就会认为那个服务器已经挂掉，从而在 fail_timeout时间内不再去查询它，fail_timeout的默认值是10s，max_fails的默认值是1（这意味着一发生错误就认为服务器挂掉），如果把max_fails设为0则表示把这个检查取消。

举个例子：server 192.168.1.11 max_fails=3 fail_timeout=30s;
这表示，如果服务器192.168.1.11在30秒内出现了3次错误，那么就认为这个服务器工作不正常，从而在接下来的30秒内nginx不再去访问这个服务器。


down —— 表示该服务器已经停用，例如server 192.168.1.11 down;

backup —— 表示该服务器是备用服务器，只有其它后端服务器都挂了或者很忙才会访问到。

关于upstream的更多信息请参考 [http://wiki.nginx.org/NginxHttpUpstreamModule](http://blog.chinaunix.net/link.php?url=http://wiki.nginx.org%2FNginxHttpUpstreamModule)








