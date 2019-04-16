# Nginx配置正向代理 - youbingchen的博客 - CSDN博客





2016年06月15日 12:38:08[youbingchen](https://me.csdn.net/youbingchen)阅读数：5455








前面 已经有一篇博客[Nginx 代理缓存](http://blog.csdn.net/youbingchen/article/details/51657060)阐述了Nginx的正反向代理的差别(同时那篇主要也是讲解如何配置nginx反向代理)，那么这篇博客就简单介绍一下Nginx配置正向代理（http proxy和https proxy） 

废话不多说，进入正题

# nginx前向代理

在 配置文件`nginx.conf中HTTP{}`中内容改写成以下内容

```
server {
        resolver 8.8.8.8;  # 这个字段是必须的
        resolver_timeout 5s;
        listen     192.168.56.102:8080;  # 代理服务器的地址+端口号


        #access_log  logsq/host.access.log  main;
        location /{
        proxy_pass $scheme://$http_host$request_uri;
        proxy_set_header Host $http_host;
        proxy_connect_timeout 5;
        }
        }
```

# 前向代理测试
- 1.使用浏览器测试，这里采用chrome浏览器演示，进入chrome设置–>高级设置 

第一步: 
![这里写图片描述](https://img-blog.csdn.net/20160615123847715)

第二步 : 
![这里写图片描述](https://img-blog.csdn.net/20160615123856762)

第三步 
![这里写图片描述](https://img-blog.csdn.net/20160615123910028)- 2.使用curl命令

```
curl  --proxy 192.168.56.102:8080 http://www.52os.net/ # 测试http代理 
curl  --proxy 192.168.56.102:8080 --user 用户名:密码 http://www.52os.net/ # 有认证的测试方法
```

> 
注意nginx代理不支持http CONNECT方法，如果访问https网站，会报错 
![这里写图片描述](https://img-blog.csdn.net/20160615123927143)
# Nginx 正向代理配置说明

1.配置 DNS 解析 IP 地址，比如 Google Public DNS，以及超时时间（5秒）

```
resolver 8.8.8.8;
resolver_timeout 5s;
```

2.置正向代理参数，均是由 Nginx 变量组成。其中 proxy_set_header 部分的配置，是为了解决如果 URL 中带 “.”（点）后 Nginx 503 错误。

```
proxy_pass $scheme://$host$request_uri;
proxy_set_header Host $http_host;
```

3.配置缓存大小，关闭磁盘缓存读写减少I/O，以及代理连接超时时间

```
proxy_buffers 256 4k;
proxy_max_temp_file_size 0;
proxy_connect_timeout 30;
```

4.配置代理服务器 Http 状态缓存时间

```cpp
proxy_cache_valid 200 302 10m;
proxy_cache_valid 301 1h;
proxy_cache_valid any 1m;
```

# 访问https网站

> 
默认情况下ssl模块并未被安装，如果要使用该模块则需要在编译时指定–with-http_ssl_module参数，安装模块依赖于OpenSSL库和一些引用文件，通常这些文件并不在同一个软件包中。通常这个文件名类似libssl-dev。 

  首先，进入你想创建证书和私钥的目录，例如：


```bash
cd /usr/local/nginx/conf
```

创建服务器私钥，命令会让你输入一个口令：
`openssl genrsa -des3 -out server.key 1024`
创建签名请求的证书（CSR）：
`openssl req -new -key server.key -out server.csr`
在加载SSL支持的Nginx并使用上述私钥时除去必须的口令：

```
cp server.key server.key.org
openssl rsa -in server.key.org -out server.key
```

最后标记证书使用上述私钥和CSR：
`openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt`
总结：因为 Nginx 不支持 CONNECT，所以无法正向代理 Https 网站（网上银行，Gmail）。Nginx目前无法正向代理SSL，只能使用Squid。

参考资料:[知乎](https://www.zhihu.com/question/19871146)






