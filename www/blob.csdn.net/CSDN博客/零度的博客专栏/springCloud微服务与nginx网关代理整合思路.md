# springCloud微服务与nginx网关代理整合思路 - 零度的博客专栏 - CSDN博客
2018年08月16日 16:45:24[零度anngle](https://me.csdn.net/zmx729618)阅读数：460
研究了好久的springCloud微服务架构，在这里整理总结一下，做个梳理和备忘。
    这里只记录一些个人认为比较重要，但是网上基本没有什么明确答案的问题。像什么注册中心什么的就不总结了，网上一大堆。
    先从网关开始说吧，网关也有很多东西，这里先说一下网关和nginx的整合。
    1、nginx在这里的作用：
    在这个方案中，通过nginx可以做前后端分离，静态化，对网关做高可用和负载均衡。提供统一的404，500页面。
    2、网关zuul
    zuul本身也是一个反向代理，也可以对其管理的微服务做负载均衡。同样，zuul也可以做一些统一的处理，可以做熔断，统一的错误处理，统一的拦截器，还可以做微服务的聚合处理。这些细节就先不说了。
    好了，两者的职责确立完毕，开始看看如何去做整合。
    一、思路
   假设：nginx服务器地址为192.168.2.104
             zuul的地址为192.168.160.66，端口号为5555
             oauth2认证服务器微服务地址为192.168.2.104，端口号为9999，服务名为sso
             zuul为oauth2认证服务器设置统一入口为api-auth
   我们希望把192.168.2.104:9999/sso/code/image的地址代理到192.168.2.104/api-auth/code/image上并且经过zuul
   做法为：nginx将zuul设置的代理地址(api-auth)转向到zuul所在的实际地址(192.168.160.66)和端口(9999)上。
   过程是这样的：
   1、客户端访问192.168.2.104/api-auth/code/image
   2、通过nginx转向到192.168.160.66:5555/api-auth/code/image
   3、通过zuul转向到192.168.2.104:9999/sso/code/image
  最后，出于安全考虑，获取令牌的请求192.168.2.104/api-auth/oauth/token不能通过网关，只能用如下请求192.168.2.104/sso/oauth/token
   二、配置
   1、nginx配置
[html] [view plain](https://blog.csdn.net/guduyishuai/article/details/80144501#)[copy](https://blog.csdn.net/guduyishuai/article/details/80144501#)
- upstream  zuul {  
-         server 192.168.160.66:5555;  
-     }  
[html] [view plain](https://blog.csdn.net/guduyishuai/article/details/80144501#)[copy](https://blog.csdn.net/guduyishuai/article/details/80144501#)
- location /api-auth/ {  
-             proxy_pass http://zuul;  
-         }  
  2、zuul配置   
[html] [view plain](https://blog.csdn.net/guduyishuai/article/details/80144501#)[copy](https://blog.csdn.net/guduyishuai/article/details/80144501#)
- zuul:   
-   routes:    
-      auth-server:   
-       path: /api-auth/**       
-       url: http://192.168.2.104:9999/sso/  
   auth-server这个名字可以随便取。path相当于nginx的location，url为被代理的地址。
  比如：我访问192.168.2.104:5555/api-auth/code/image，将被转向到192.168.2.104:9999/sso/code/image上。
  如果需要做高可用和负载均衡，那么可以用ribbon.listOfSevers设置多个被代理的地址。
  到此zuul+nginx的解决方案大体实现就差不多了，剩下的细节就不一一叙述了。
  为了方便开发，可以把注册中心和网关等常用的服务通过docker镜像运行成docker容器。
![](https://img-blog.csdn.net/20180429184747101)
