# Nginx之4中不同的负载均衡算法 - 零度的博客专栏 - CSDN博客
2016年06月01日 14:06:16[零度anngle](https://me.csdn.net/zmx729618)阅读数：4019
         Nginx的负载均衡支持4种算法，round-robin、least-connected、ip-hash、和weightd。
### 1、round-robin
         round-robin的意思是循环轮询。Nginx最简单的负载均衡配置如下：
http {
    upstream app1 {
        server 10.10.10.1;
        server 10.10.10.2;
    }
    server {
        listen 80;
        location / {
            proxy_pass http://app1;
        }
    }
}
         upstream app1用来指定一个服务器组，该组的名字是app1，包含两台服务器。在指定服务器组里面包含的服务器时以形式“server ip/domain：port”的形式指定，其中80端口可以忽略。然后在接收到请求时通过“proxy_pass http://app1”把对应的请求转发到组app1上。**Nginx默认的负载均衡算法就是循环轮询**，如上配置我们采用的就是循环轮询，其会把接收到的请求循环的分发给其包含的（当前可用的）服务器。使用如上配置时，Nginx会把第1个请求给10.10.10.1，把第2个请求给10.10.10.2，第3个请求给10.10.10.1，以此类推。
###  2、least-connected
         least-connected算法的中文翻译是最少连接，即每次都找连接数最少的服务器来转发请求。例如Nginx负载中有两台服务器，A和B，当Nginx接收到一个请求时，A正在处理的请求数是10，B正在处理的请求数是20，则Nginx会把当前请求交给A来处理。要启用最少连接负载算法只需要在定义服务器组时加上“least_conn”，如：
    upstream app1 {
                   least_conn;
        server 10.10.10.1;
        server 10.10.10.2;
    }
### 3、ip-hash
         ip-hash算法会根据请求的客户端IP地址来决定当前请求应该交给谁。使用ip-hash算法时Nginx会确保来自同一客户端的请求都分发到同一服务器。要使用ip-hash算法时只需要在定义服务器组时加上“ip-hash   ”指令，如：
    upstream app1 {
                   ip_hash;
        server 10.10.10.1;
        server 10.10.10.2;
    }
### 4、weighted
         weighted算法也就是权重算法，会根据每个服务的权重来分发请求，权重大的请求相对会多分发一点，权重小的会少分发一点。这通常应用于多个服务器的性能不一致时。需要使用权重算法时只需要在定义服务器组时在服务器后面指定参数weight，如：
    upstream app1 {
        server 10.10.10.1 weight=3;
        server 10.10.10.2;
    }
         在如上配置时，Nginx接收到的请求中大概每4个请求中会有3个请求交给10.10.10.1服务器处理，有1个请求交给10.10.10.2处理。
         除了这些指令外，Nginx负载还可以指定其它一些指令，比如
         backup、down等，详情可参考： [http://nginx.org/en/docs/http/ngx_http_upstream_module.html](http://nginx.org/en/docs/http/ngx_http_upstream_module.html)。
         Nginx可指定的指令可参考：[http://nginx.org/en/docs/http/ngx_http_proxy_module.html](http://nginx.org/en/docs/http/ngx_http_proxy_module.html)。
