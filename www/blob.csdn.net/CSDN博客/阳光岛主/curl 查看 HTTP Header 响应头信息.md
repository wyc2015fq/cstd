
# curl 查看 HTTP Header 响应头信息 - 阳光岛主 - CSDN博客

2018年09月06日 22:35:49[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4841


curl 查看 HTTP Header 响应头信息，可参见米扑博客：[curl 查看 HTTP Header 响应头信息](https://blog.mimvp.com/article/25783.html)
**1、curl -I 查看HTTP 响应头信息**
curl -I https://proxy.mimvp.com
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|yg-mac:~ homer$ curl -I https:|//proxy|.mimvp.com
|HTTP|/1|.1 200 OK
|Server: nginx
|Date: Thu, 31 May 2018 03:51:08 GMT
|Content-Type: text|/html|; charset=UTF-8
|Connection: keep-alive
|Vary: Accept-Encoding
|Set-Cookie: PHPSESSID=d4kf789gvurb10p7g5pcqd80o6; path=/
|Expires: Thu, 19 Nov 1981 08:52:00 GMT
|Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0
|Pragma: no-cache
|Vary: proxys-bj
|
**2、HTTP 响应头的信息**
**1) HTTP 返回码**
1xx：client的请求server已经接收，正在处理
2xx：成功  表示 client请求，server端已经接收、理解并处理
3xx：client 请求被重定向其他的server【其他的URL】
4xx：表示client请求不正确，server不能识别
5xx：server端服务不正常
### 2) Cache-Control
web 站点对缓存的设置；
no-cache：表示server 不允许client 设置缓存；client每次请求都需要请求后端的服务
max-age：server设置页面的缓存时间；单位是秒  即：1个小时内此client再访问 www.mimvp.com 用刚才的缓存；不再重复访问后台服务器
private must-revalidate
### 3) Connection
server 是否支持长连接；如果keep-alive 说明web的server 支持长连接。
但是TCP 的长连接是双向的；必须是client和server 都支持长连接；才可以建立长连接。
一般client 【浏览器】都是默认支持长连接；所以只要sever端支持长连接；就可以建立长连接。

**3、curl 发送Get/Post 请求**
**1）get 请求**
我们平时用的最多的就是curl 不加任何的参数，其实这样就是get请求；把参数都放在http://xx 后面
curl "https://proxy.mimvp.com/freesecret.php?proxy=in_hp&sort=&page=5"
**2）post 请求**
curl -d 'post_data=i_love_mimvp.com' https://proxy.mimvp.com/ip.php
或
curl -H "Content-Type:application/json" -X POST -d '{"post_data":"i_live_mimvp.com"}' 'https://proxy.mimvp.com/ip.php'
**3）上传文件**
curl -F 'csl=@/home/xxxx/xxx.csl' -F 'tag=xxx' -F 'category=full' "https://proxy.mimvp.com/demo/"
**curl HTTP Header**
对于"User-Agent", "Cookie", "Host"这类标准的HTTP头部字段，通常会有另外一种设置方法。
curl命令提供了特定的选项来对这些头部字段进行设置：
-A (or --user-agent): 设置 "User-Agent" 字段.
-b (or --cookie): 设置 "Cookie" 字段.
-e (or --referer): 设置 "Referer" 字段.
例如，以下两个命令是等效的。这两个命令同样都对HTTP头的"User-Agent"字符串进行了更改。
|1
|2
|$ curl -H|"User-Agent: my browser"|"https://proxy.mimvp.com/demo/"
|$ curl -A|"my browser"|"https://proxy.mimvp.com/demo/"
|
wget是另外一个类似于curl，可以用来获取URL的命令行工具。
wget也一样允许你使用一个自定义的HTTP头。
点击[这里](http://xmodulo.com/how-to-use-custom-http-headers-with-wget.html)查看wget命令的详细信息。
**1、GET**
**1）with JSON**
curl -i -H "Accept: application/json" -H "Content-Type: application/json" https://proxy.mimvp.com/ip.php
**2）with XML**
curl -H "Accept: application/xml" -H "Content-Type: application/xml" -X GET https://proxy.mimvp.com/ip.php
**2、POST**
**1）For posting data**
curl --data "param1=value1&param2=value2" https://proxy.mimvp.com/ip.php
**2）For file upload**
curl --form "fileupload=@filename.txt" https://proxy.mimvp.com/ip.php
**3）RESTful HTTP Post**
curl -X POST -d @filename https://proxy.mimvp.com/ip.php
**4）For logging into a site (auth)**
curl -d "username=admin&password=admin&submit=Login" --dump-header headers https://proxy.mimvp.com/ip.php curl -L -b headers https://proxy.mimvp.com/ip.php


