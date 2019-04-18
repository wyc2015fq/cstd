# cors跨域深刻理解 - weixin_33985507的博客 - CSDN博客
2017年10月12日 09:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
1.跨域问题只出现在前端和后端不在同一个主机上。前后端在同一个主机上不会出现跨域问题。
2.浏览器的一种自我保护机制，不允许出现本地浏览器ajax异步请求访问127.0.0.1以外的系统，因为浏览器不知道远程系统是不是你的，不是你的系统，浏览器对ajax的保护机制就不允许你访问喽。
3.要想解决跨域问题：就要让浏览器知道这个系统是你的系统啊，怎么让浏览器知道呢？在后台系统的response header上设置一个header，叫Access-Control-Allow-Origin一个header。
下面就是具体怎么实现cors。首先，对于支持cors的浏览器，发送请求时，如果我们观察请求头会发现带有"Origin:http://example.taobao.com" ，该标识用来说明本次请求来自的域。要实现cors，只要在服务端在响应头部加上标识“Access-Control-Allow-Origin:http://example.taobao.com”，这样浏览器发现Access-Control-Allow-Origin和请求来自的域一致，就允许跨域访问资源example.taobao.com的资源了。对应服务端代码：
rundata.getResponse().setHeader("Access-Control-Allow-Origin", "http://example.taobao.com");    
4.解决跨域的精髓是什么？
：http response返回给前端的header里包含了允许你访问这个系统的header。
controller上@cros注解的作用就是这个，向response的header里写这个header。
也可以在拦截器里设置这个response返回header，这样所有的返回都添加了这个header。就全局解决了跨域问题
