# 服务器端判断request是Ajax请求(异步)还是传统请求(同步) - 零度的博客专栏 - CSDN博客
2016年07月15日 16:39:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：2502
                
在一次做非常复杂的ajax应用时，如果一个会话已经超时，但是此时再通过ajax请求，那么ajax返回的则是一个登陆页面的html，那这下就惨了，页面上而已就乱了，那么，能否在java端，如拦截器里判断客户的的请求是否是ajax请求呢，经过查询，能. 
   普通请求与ajax请求的报文头不一样，通过如下 
Java代码  ![收藏代码](http://zwllxs.iteye.com/images/icon_star.png)
- String requestType = request.getHeader("X-Requested-With");  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
如果requestType能拿到值，并且值为XMLHttpRequest,表示客户端的请求为异步请求，那自然是ajax请求了，反之如果为null,则是普通的请求 
在服务器端判断request来自Ajax请求(异步)还是传统请求(同步)：
　　两种请求在请求的Header不同，Ajax 异步请求比传统的同步请求多了一个头参数
　　1、传统同步请求参数
　　　 accept  text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
　　　 accept-charset  gb2312,utf-8;q=0.7,*;q=0.7
　　　 accept-encoding  gzip,deflate
　　　 accept-language  zh-cn,zh;q=0.5
　　　 cache-control  max-age=0
　　　 connection  keep-alive
　　　 cookie  JSESSIONID=1A3BED3F593EA9747C9FDA16D309AF6B
　　　 host  192.168.101.72:8080
　　　 keep-alive  300
　　　 referer  [http://192.168.101.72:8080/htfsweb/spring_security_login](http://192.168.101.72:8080/htfsweb/spring_security_login)
　　　 user-agent  Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.0.15) Gecko/2009101601 Firefox/3.0.15 (.NET CLR 3.5.30729)
　　2、Ajax 异步请求方式
　　　 accept  */*
　　　 accept-language  zh-cn
　　　 referer  [http://192.168.101.72:8080/scm/?token=3b194bff23bf6acecea5661ac8e14c51](http://192.168.101.72:8080/scm/?token=3b194bff23bf6acecea5661ac8e14c51)
x-requested-with  XMLHttpRequest
　　　 content-type  application/x-www-form-urlencoded,text/javascript
　　　 accept-encoding  gzip, deflate
　　　 user-agent  Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; QQDownload 598; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; CIBA; .NET CLR 1.1.4322; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; InfoPath.1)
　　　 host  192.168.101.72:8080
　　　 content-length  233
　　　 connection  Keep-Alive
　　　 cache-control  no-cache
　　　 cookie  CSS=undefined; JSESSIONID=1B9AC25036290F7FB6823CCE1A24E541
　　可以看到 Ajax 请求多了个 x-requested-with ，可以利用它，request.getHeader("x-requested-with"); 为 null，则为传统同步请求，为 XMLHttpRequest，则为 Ajax 异步请求。
