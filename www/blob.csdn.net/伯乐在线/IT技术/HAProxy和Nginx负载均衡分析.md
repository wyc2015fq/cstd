# HAProxy和Nginx负载均衡分析 - 文章 - 伯乐在线
原文出处： [鸿弟](https://my.oschina.net/zhuangweihong/blog/813231)
出于对负载均衡工具的疑问，分别对HAProxy和Nginx的负载均衡进行了抓包分析，分析的过程跟大家分享下。先说下对HAProxy抓包得到的结论吧：HAProxy在负载均衡的一台后端挂掉后，如果还没达到探测的时间点时，请求还会往挂掉的这台转发，请求会丢失。
HAProxy负载均衡的实验过程如下：
1: 先看下HAProxy的配置。配置inter 20000为20s检测一次，这个是为了更明显的抓下HAProxy的负载均衡探测机制。


```
listen test9090
        bind 127.0.0.1:9090
        mode tcp
        server localhost90 127.0.0.1:90 check inter 20000
        server localhost91 127.0.0.1:91 check inter 20000
```
2: 后端我是用Nginx进行测试，看下Nginx的配置如下。可在/var/www/html/建个index.html进行测试


```
server {
            listen       90;
            listen       91;
            location /{
                    root /var/www/html;
            }
    }
```
先用curl 127.0.0.1:9090进行测试，并在机器上开两个窗口看下抓包是否均衡正常，两个窗口运行的命令分别如下：tcpdump -i lo -nn ‘port 90’      tcpdump -i lo -nn ‘port 91’看下负载均衡能否正常工作。
![](http://jbcdn2.b0.upaiyun.com/2017/01/f7c3461e3f782f3dbfdc23789ac478f2.png)
![](http://jbcdn2.b0.upaiyun.com/2017/01/a0c63cbf266a6d55a6410b2095012edf.png)
上面抓包的截图证明Nginx监听的90和91端口都有在监听。使用抓包来检测比看日志来更细点，所以还是用抓包来分析了。
3: 抓包查看HAProxy的健康检测机制
因为我们配置了inter 20000，也就是告诉HAProxy 20s检测一次，抓包查看也是20s检查一下。注意下，这个检测是在客户端无任务请求的时候进行探测的，也就是处理请求跟探测是分开的。
![](http://jbcdn2.b0.upaiyun.com/2017/01/0c864ba07227d6a8164977a7625a6d0b.png)
4: 模拟线上故障，Nginx挂掉91端口
把listen  91这个Nginx的配置去除，然后reload一下，会发现前端的请求如果分发到91端口的话，就会挂掉，经抓包发现HAProxy需要探测三次才会把故障的给切下线。我们配置的是20s探测一次，也是最长可能得探测60s才能把故障的切除掉。如果这60s内有1w请求的话，那就会丢掉5k个。如果用在线上的话，探测机制肯定不会是20s一次，一般最多3s会切换掉。
![](http://jbcdn2.b0.upaiyun.com/2017/01/f9c7ea9f15595c415902bb0332f629b7.png)
Nginx负载均衡的配置如下：
1: 先看下Nginx的反向代理负载均衡配置，如下：


```
upstream backend {
        server 127.0.0.1:90 weight=1 max_fails=3 fail_timeout=30;
        server 127.0.0.1:91 weight=5 max_fails=3 fail_timeout=30;
    }
```
还是把配置打向后端的90和91端口，然后后面再模拟故障的发生。


```
server {
            listen 9090;
            location / {
                    proxy_pass http://backend;
            }
    }
```
前端还是使用9090来监听，把请求转发到90和91端口。
2: 后端我还是是用Nginx进行测试，看下Nginx的配置如下。可在/var/www/html/建个index.html进行测试


```
server {
            listen       90;
            listen       91;
            location /{
                    root /var/www/html;
            }
    }
```
抓包同样会发生90和91的包都有过来。
3: 抓包查看Nginx反向代理负载均衡的健康检测机制
抓包你会发现Nginx在没有请求的时候，90和91端口上没有任务的请求。也就是在没有请求的时候，是不会对后端的代理服务器进行检测的。
4: 模拟线上故障，Nginx挂掉91端口
把listen  91这个Nginx的配置去除，然后reload一下，发现前端的访问没有任务影响。抓包如下，请求有打包91，但由于91没请求到数据。Nginx的均衡还会再次去90上取数据。也就是说，Nginx如果后端挂掉91端口的话，对前端的请求没有任务影响，只要并发支撑得住的话。
![](http://jbcdn2.b0.upaiyun.com/2017/01/4cad4fb76327e3f530aa47a645580ceb.png)
![](http://jbcdn2.b0.upaiyun.com/2017/01/c8c15b3d636f4384ef4addd736849229.png)
综合一下：
HAProxy对于后端的服务器就算请求没过来也有一直在健康检测，发现有故障的时候在请求没到来的时候会切掉，但如果在检测期间请求到达的话，会有异常。HAProxy只会把请求转到后端的一台服务器。
Nginx对于后端的服务器没有一直在健康检测，请求过来的时候，分发还是进行分发，只是请求不到数据的时候，会再向好的机器进行请求，直到请求正常为止。Nginx请求转到后端一台不成功的话，还会转向另外一台服务器。同时我也测试了下squid，发现squid跟Nginx的反向代理负载均衡很相似。
所以如果有用HAProxy做为前端负载均衡的话 ，如果后端服务器要维护，在高并发的情况，肯定是会影响用户的。但如果是Nginx做为前端负载均衡的话，只要并发撑得住，后端切掉几台不会影响到用户。至于两者的性能，得再继续研究。
