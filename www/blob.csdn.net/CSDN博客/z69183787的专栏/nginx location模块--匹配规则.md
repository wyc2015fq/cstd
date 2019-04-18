# nginx location模块--匹配规则 - z69183787的专栏 - CSDN博客
2017年12月11日 14:05:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：368
[https://www.cnblogs.com/linjiqin/p/5532119.html](https://www.cnblogs.com/linjiqin/p/5532119.html)
![](http://images2015.cnblogs.com/blog/270324/201606/270324-20160606205313761-1362584261.png)
Location语法语法：location [=|~|~*|^~] /uri/ { … }
= --> 开头表示精确匹配
^~ --> 开头表示uri以某个常规字符串开头，理解为匹配url路径即可。
nginx不对url做编码，因此请求为/static/20%/aa，可以被规则^~ /static/ /aa匹配到（注意是空格）。
~ --> 开头表示区分大小写的正则匹配
~* --> 开头表示不区分大小写的正则匹配
!~和!~* --> 分别为区分大小写不匹配及不区分大小写不匹配的正则
/ --> 通用匹配，任何请求都会匹配到。
多个location配置的情况下匹配顺序为：
首先匹配=，其次匹配^~, 其次是按文件中顺序的正则匹配，最后是交给 / 通用匹配。当有匹配成功时候，停止匹配，按当前匹配规则处理请求。
例子，有如下匹配规则：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
location = / {
   #http://localhost/ 访问根目录/
   #规则A
}
location = /login {
   #http://localhost/login
   #规则B
}
location ^~ /static/ {
   #http://localhost/static/a.html
   #http://localhost/static/c.png 则优先匹配到 规则C
   #规则C
}
#http://localhost/a.gif, http://localhost/b.jpg 将匹配规则D和规则E，但是规则D顺序优先，规则E不起作用，
location ~ \.(gif|jpg|png|js|css)$ {
   
   #规则D
}
location ~* \.png$ {
   #http://localhost/a.PNG 则匹配规则E，而不会匹配规则D，因为规则E不区分大小写。
   #规则E
}
http://localhost/a.xhtml 不会匹配规则F和规则G，
http://localhost/a.XHTML 不会匹配规则G，因为不区分大小写。
规则F，规则G属于排除法，符合匹配规则但是不会匹配到，所以想想看实际应用中哪里会用到。
location !~ \.xhtml$ {
   #规则F
}
location !~* \.xhtml$ {
   #规则G
}
访问http://localhost/category/id/1111 最终匹配到规则H，因为以上规则都不匹配，这个时候应该是nginx转发请求给后端应用服务器，
比如FastCGI（php），tomcat（jsp），nginx作为方向代理服务器存在。
location / {
   #http://localhost/register
   #规则H
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
所以实际使用中，个人觉得至少有三个匹配规则定义，如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
#直接匹配网站根，通过域名访问网站首页比较频繁，使用这个会加速处理，官网如是说。
#这里是直接转发给后端应用服务器了，也可以是一个静态首页
# 第一个必选规则
location = / {
    proxy_pass http://tomcat:8080/index
}
 
# 第二个必选规则是处理静态文件请求，这是nginx作为http服务器的强项
# 有两种配置模式，目录匹配或后缀匹配,任选其一或搭配使用
location ^~ /static/ {
    root /webroot/static/;
}
location ~* \.(gif|jpg|jpeg|png|css|js|ico)$ {
    root /webroot/res/;
}
 
#第三个规则就是通用规则，用来转发动态请求到后端应用服务器
#非静态文件请求就默认是动态请求，自己根据实际把握
#毕竟目前的一些框架的流行，带.php,.jsp后缀的情况很少了
location / {
    proxy_pass http://tomcat:8080/
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
对于以上基础推荐配置，有一个补充，就是关于转发有一点需要注意。例如下面配置，对一个目录转发：
```
location ^~ /outer/ {
    #case A： url最后以/结尾
    proxy_pass http://tomcat:8080/
    #case B： url最后没有/
    #proxy_pass http://tomcat:8080  
}
```
关键在于最后的/，访问localhost/outer/in.html，其中case A会转发到tomcat:8080/in.html， 而case B会转发到tomcat:8080/outer/in.html，所以务必注意了。
