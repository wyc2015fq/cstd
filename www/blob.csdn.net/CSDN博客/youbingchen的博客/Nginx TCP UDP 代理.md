# Nginx TCP/UDP 代理 - youbingchen的博客 - CSDN博客





2016年06月14日 14:26:33[youbingchen](https://me.csdn.net/youbingchen)阅读数：10145








这个模块可以实现基于TCP、UDP和Unix域的socket的协议的代理服务。这个 模块是在nginx-1.9 以后版本才添加的模块,如果要使用这个模块的话，要重新编译这个源代码，参考之前的的博客[nginx安装](http://blog.csdn.net/youbingchen/article/details/51605181)，添加编译选项`--with-stream`。就可以使用 这个模块

# 配置基于TCP连接和UDP的数据报的反向代理
- 1.创建最顶层的`stream {}`块

```
stream{
....
}
```
- 2.可以在`stream {}`块中创建一个或多个的`server {}`配置项
- 3.在每个`server {}`配置块中，要填写`listen`目标（IP+端口或是端口），如果是UDP协议还要包括 udp参数。在`stream`TCP是 默认 的协议。如果不进行 设置，系统默认是TCP协议 去 监听。

```
stream {
    server {
        listen 12345;
        ...
    }
    server {
        listen 53 udp;
        ...
    }
    ...
}
```
- 4.利用`proxy_pass`选项定义被代理的 服务器或是上游组

```
stream {
    server {
        listen     12345;

        #TCP traffic will be proxied to the "stream_backend" upstream group
        proxy_pass stream_backend;
    }

    server {
        listen     12346;

        #TCP traffic will be proxied a proxied server
        proxy_pass backend.example.com:12346;
    }

    server {
        listen     53 udp;

        #UDP traffic will be proxied to the "dns_servers" upstream group
        proxy_pass dns_servers;
    }
    ...
}
```
- 5.如果代理服务器有几个 网络接口，可以固定一个源IP和源端口去连接上游的服务器，可以选择`proxy_bind`字段去绑定的特定的IP和端口去连接上游的服务器（这个字段是可选的）

```
stream {
    ...
    server {
        listen     127.0.0.1:12345;
        proxy_pass backend.example.com:12345;
        proxy_bind 127.0.0.1:12345;
    }
}
```

这个 选项很可能需 要管理员用户的权限去 配置路由表截断来源被代理服务器的网络流量 。 

+  6.（这个字段是可选的），可以调整用来 传输代理服务器和上游组连接数据的双向内存缓存区的大小，根据实际传输的数据大小进行调整，如果传输数据比较少，这样做可以节省空间，如果数据量 大的可以减少`socket read/write的次数 `

```
stream {
    ...
    server {
        listen            127.0.0.1:12345;
        proxy_pass        backend.example.com:12345;
        proxy_buffer_size 16k;
    }
}
```

# 配置TCP和UDP负载均衡
- 1.在 `top-level stream{}`区块 里面可以添加一个或多个`upstream{}`的配置项，例如:`stream_backend`面向TCP servers和dns_server面向 的 是UDP_servers。

```
stream {
    upstream stream_backend {
        ...    
    }

    upstream dns_servers {
        ...    
    }
    ...
}
```

**注意：`upstream group`一定要在`之前 定义`proxy_pass**。 

+ 2.在`upstream`模块，使用`server`指令定义每一个`upstream`。包括它的域名或是IP地址加上必要的端口号。

```
stream {
    upstream stream_backend {
        server backend1.example.com:12345;
        server backend2.example.com:12345;
        server backend3.example.com:12346;
        ...
    }
    upstream dns_servers {
        server 192.168.136.130:53;
        server 192.168.136.131:53;
        ...
    }
    ...
}
```
- 
3.配置负载均衡的方法使用的是上游组，可以指定以下的一种方法：
- round-robin(默认)
- least-conn Nginx选择当前连接数最少的服务器
- least_time Nginx选择最小延时时间和连接数最少的服务器。最小延时时间是基于以下参数进行运算的 

> - connect-连接上游服务器的时间
- first_byte 收到第一字节的最短时间
- 
last_byte 收到服务器的响应



```
upstream stream_backend {
least_time first_byte;

server backend1.example.com:12345;
server backend2.example.com:12345;
server backend3.example.com:12346;
}
```

- 
hash的方法，服务器基于用户定义值，例如：源IP地址($remote_addr)



```
upstream stream_backend {
    hash $remote_addr;

    server backend1.example.com:12345;
    server backend2.example.com:12345;
    server backend3.example.com:12346;
}
```
- 4.(可选的)针对每一个上游服务器 设置固定参数，包括最大连接数，服务器的权重。

```
upstream stream_backend {
    hash   $remote_addr consistent;
    server backend1.example.com:12345 weight=5;
    server backend2.example.com:12345;
    server backend3.example.com:12346 max_conns=3;
}

    upstream dns_servers {
        least_conn;
        server 192.168.136.130:53;
        server 192.168.136.131:53;
        ...
    }
```



