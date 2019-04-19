# Nginx+Tomcat 配置负载均衡集群 - 文章 - 伯乐在线
原文出处： [努力白十一](https://my.oschina.net/bgq365/blog/870569)
# 一、Hello world
## 1、前期环境准备
- 准备两个解压版tomcat，如何同时启动两个tomcat，请看我的另一篇文章[《一台机器同时启动多个tomcat》](https://my.oschina.net/bgq365/blog/870155)。
- [nginx官网](http://nginx.org/en/download.html)下载解压版nginx。
- 创建一个简单的web项目。为了直观的区分访问的哪个tomcat，在页面写上标记8081、8082。![](http://jbcdn2.b0.upaiyun.com/2017/04/802bed3e507f48b0f9ec66517557c363.png)
- 分别部署到对应的tomcat下。如图：
![](http://jbcdn2.b0.upaiyun.com/2017/04/4e0e60ade16435ba1465e02570a84266.png)![](http://jbcdn2.b0.upaiyun.com/2017/04/2d53694f1e872a48e5f1863c0d4af703.png)
## 2、配置nginx
进入nginx-1.10.1conf路径，修改配置文件**nginx.conf**。
1、配置服务器组，在http{}节点之间添加upstream配置。（**注意不要写localhost，不然访问速度会很慢**）


```
upstream nginxDemo {
    server 127.0.0.1:8081;   #服务器地址1
    server 127.0.0.1:8082;   #服务器地址2
}
```
2、修改nginx监听的端口号80，改为8080。


```
server {
    listen       8080;
    ......
}
```
3、在location{}中，利用**proxy_pass**配置反向代理地址；此处“http://”不能少，后面的地址要和第一步**upstream**定义的名称保持一致。


```
location / {
            root   html;
            index  index.html index.htm;
            proxy_pass http://nginxDemo; #配置方向代理地址
        }
```
如下图：
![](http://jbcdn2.b0.upaiyun.com/2017/04/cf3a3c6232c798bd2438e25c8377cb77.png)
## 3、启动nginx和tomcat，访问
我是Windows系统，所以直接在nginx-1.10.1目录下双击nginx.exe即可。
可在任务管理器中查看![](http://jbcdn2.b0.upaiyun.com/2017/04/00771ab01bc5fec08e6d66dcebde9fe6.png)
最后在浏览器输入地址：http://localhost:8080/nginxDemo/index.jsp，每次访问就会轮流访问tomcat了（如果F5刷新不管用，建议试试鼠标指针放到地址栏，点击Enter键）。
![](http://jbcdn2.b0.upaiyun.com/2017/04/f369a20c6dd071c3c606bb48504caff1.png)![](http://jbcdn2.b0.upaiyun.com/2017/04/4e31d7385372dda49aac3b4560c6dccc.png)
到这里，一个非常简单的负载均衡就配置完成了，是不是很简单呢，O(∩_∩)O哈哈~
# 二、nginx负载均衡策略
## 1、轮询（默认）
每个web请求按时间顺序逐一分配到不同的后端服务器，如果后端服务器down掉，能自动剔除。


```
upstream nginxDemo {
        server 127.0.0.1:8081;
        server 127.0.0.1:8082;
    }
```
## 2、最少链接
web请求会被转发到连接数最少的服务器上。


```
upstream nginxDemo {
        least_conn;
        server 127.0.0.1:8081;
        server 127.0.0.1:8082;
    }
```
## 3、**weight 权重**
指定轮询几率，weight和访问比率成正比，用于后端服务器性能不均的情况，weight默认是1。


```
#服务器A和服务器B的访问比例为：2-1;比如有3个请求，前两个会访问A，三个访问B，其它规则和轮询一样。
    upstream nginxDemo {
        server 127.0.0.1:8081 weight=2; #服务器A
        server 127.0.0.1:8082; #服务器B
    }
```
## 4、ip_hash
每个请求按访问ip的hash值分配，这样同一客户端连续的Web请求都会被分发到同一服务器进行处理，可以解决session的问题。当后台服务器宕机时，会自动跳转到其它服务器。


```
upstream nginxDemo {
        ip_hash;
        server 127.0.0.1:8081 weight=2; #服务器A
        server 127.0.0.1:8082; #服务器B
    }
```
基于weight的负载均衡和基于ip_hash的负载均衡可以组合在一起使用。
## 5、url_hash（第三方）
url_hash是nginx的第三方模块，nginx本身不支持，需要打补丁。
nginx按访问url的hash结果来分配请求，使每个url定向到同一个后端服务器，后端服务器为缓存服务器、文件服务器、静态服务器时比较有效。缺点是当后端服务器宕机的时候，url_hash不会自动跳转的其他缓存服务器，而是返回给用户一个503错误。


```
upstream nginxDemo {
        server 127.0.0.1:8081; #服务器A
        server 127.0.0.1:8082; #服务器B
        hash $request_url;
    }
```
## 6、fair**（第三方）**
按后端服务器的响应时间来分配请求，响应时间短的优先分配。


```
upstream nginxDemo {
        server 127.0.0.1:8081; #服务器A
        server 127.0.0.1:8082; #服务器B
        fair;
    }
```
~未完待续~
