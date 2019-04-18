# nginx利用referer指令实现防盗链配置 - z69183787的专栏 - CSDN博客
2018年07月16日 15:19:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2186
nginx模块ngx_http_referer_module通常用于阻挡来源非法的域名请求,我们应该牢记。下面这篇文章主要介绍了nginx利用referer指令实现防盗链配置的相关资料，需要的朋友可以参考借鉴，下面来一起看看吧。
实现图片防盗链：
```
location ~* \.(gif|jpg|png|webp)$ {
   valid_referers none blocked domain.com *.domain.com server_names ~\.google\. ~\.baidu\.;
   if ($invalid_referer) {
    return 403;
    #rewrite ^/ http://www.domain.com/403.jpg;
   }
   root /opt/www/image;
  }
```
以上所有来至domain.com和域名以及baidu和google的站点都可以访问到当前站点的图片,如果来源域名不在这个列表中，那么$invalid_referer等于1，在if语句中返回一个403给用户，这样用户便会看到一个403的页面,如果使用下面的rewrite，那么盗链的图片都会显示403.jpg。none规则实现了允许空referer访问，即当直接在浏览器打开图片，referer为空时，图片仍能正常显示.
```
[root@loya ~]# curl -I http://qingkang.me/1.jpg -H 'Referer:http://www.baidu.com'
HTTP/1.1 200 OK
Server: nginx/1.8.1
Date: Fri, 16 Dec 2016 14:56:51 GMT
Content-Type: image/jpeg
Content-Length: 17746
Last-Modified: Tue, 16 Aug 2016 03:20:21 GMT
Connection: keep-alive
ETag: "57b28675-4552"
Accept-Ranges: bytes
[root@loya ~]# curl -I http://qingkang.me/1.jpg -H 'Referer:http://www.qq.com'
HTTP/1.1 403 Forbidden
Server: nginx/1.8.1
Date: Fri, 16 Dec 2016 14:56:58 GMT
Content-Type: text/html; charset=utf-8
Content-Length: 168
Connection: keep-alive
```
指令
语法: valid_referers none | blocked | server_names | string …;
配置段: server, location
指定合法的来源'referer', 他决定了内置变量$invalid_referer的值，如果referer头部包含在这个合法网址里面，这个变量被设置为0，否则设置为1. 需要注意的是：这里并不区分大小写的.
参数说明：
- none “Referer” 为空
- blocked “Referer”不为空，但是里面的值被代理或者防火墙删除了，这些值都不以http://或者https://开头，而是“Referer: XXXXXXX”这种形式
- server_names “Referer”来源头部包含当前的server_names（当前域名）
- arbitrary string 任意字符串,定义服务器名或者可选的URI前缀.主机名可以使用*开头或者结尾，在检测来源头部这个过程中，来源域名中的主机端口将会被忽略掉
- regular expression 正则表达式,~表示排除https://或http://开头的字符串.
注意
通过Referer实现防盗链比较基础，仅可以简单实现方式资源被盗用。构造Referer的请求很容易实现。
