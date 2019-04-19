# Nginx 代理转发之后Java后台获取IP地址处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年03月27日 14:05:56[boonya](https://me.csdn.net/boonya)阅读数：30
后台获取IP地址如果配置了Nginx，那么相应的转发请求需要配置Nginx代理参数，否则获取到的都是Nginx服务器地址。
## Nginx Location配置
![](https://img-blog.csdnimg.cn/20190327140253389.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
核心配置：
```
proxy_redirect    off;
            proxy_set_header  Host $host;
            proxy_set_header  X-real-ip $remote_addr;
            proxy_set_header  X-Forwarded-For $proxy_add_x_forwarded_for;
```
参考地址：[http://www.cnblogs.com/yum777/p/6473017.html](http://www.cnblogs.com/yum777/p/6473017.html)
## Java获取IP地址
```java
public  String getIpAddr(HttpServletRequest request)  {
        String ip  =  request.getHeader( "x-forward-for" );//负载均衡下为小写
        if (ip  ==   null   ||  ip.length()  ==   0   ||   "unknown" .equalsIgnoreCase(ip))  {
        	ip  =  request.getHeader( "X-Forwarded-For" );
        } 
         if (ip  ==   null   ||  ip.length()  ==   0   ||   "unknown" .equalsIgnoreCase(ip))  {
            ip  =  request.getHeader( "Proxy-Client-IP" );
        } 
         if (ip  ==   null   ||  ip.length()  ==   0   ||   "unknown" .equalsIgnoreCase(ip))  {
            ip  =  request.getHeader( "WL-Proxy-Client-IP" );
        } 
         if (ip  ==   null   ||  ip.length()  ==   0   ||   "unknown" .equalsIgnoreCase(ip))  {
           ip  =  request.getRemoteAddr();
       } 
        return  ip;
   }
```
