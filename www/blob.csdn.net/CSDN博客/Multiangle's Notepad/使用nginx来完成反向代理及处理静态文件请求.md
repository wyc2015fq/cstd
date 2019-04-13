
# 使用nginx来完成反向代理及处理静态文件请求 - Multiangle's Notepad - CSDN博客


2016年03月13日 10:12:51[multiangle](https://me.csdn.net/u014595019)阅读数：12532



> 这里主要分三块，分别是nginx的基础安装及使用，反向代理及处理静态文件的请求。

---1.安装及使用(windows 平台)

---首先从

---[这里](http://nginx.org/)

---下载nginx，照流程安装即可。如果觉得英语稍显吃力的话，可以看

---[教程](http://jingyan.baidu.com/article/f3e34a12a9c1c3f5eb6535d4.html)

---启动nginx

---方法1：命令行进入nginx的根目录，输入 start nginx

![这里写图片描述](https://img-blog.csdn.net/20160313094440264)
---方法2：命令行进入nginx的根目录，输入 nginx.exe

---方法3：进入nginx的根目录，双击nginx.exe图标

![这里写图片描述](https://img-blog.csdn.net/20160313094329019)
---我一开始是使用方法3的，确实当机子上只需要运行1个nginx实例时很好办，但是当需要运行多个nginx时就搞不清了，所以这里推荐方法1。方法2的问题是会使得cmd窗口一直处于执行状态，无法干其他事情

---关闭nginx

---nginx.exe

----s

---stop
nginx.exe

----s

---quit

---这两条都可以，区别在于stop是快速停止nginx，可能并不保存相关信息；quit是完整有序的停止nginx，并保存相关信息。

---重载nginx

---nginx.exe

----s

---reload

---修改conf配置文件以后，需要重启。如果关闭进程然后再打开的话，显得太过麻烦，所以需要使用重载。

---Tips

---有时候会出现启动不了的情况。这时候可以去logs/error.log查看报错信息。我之前是因为80端口被vs占用，所以在conf里把监听端口改成7000以后就好了

---2.nginx的反向代理

---nginx的一个很重要的作用是作为反向代理服务器来实现负载均衡。实现这个功能需要修改conf文件。

---打开conf/nginx.conf（推荐使用sublime等编辑器打开，尽量别用notepad）

---这里可以参考

---[配置文件详细说明](http://www.cnblogs.com/xiaogangqq123/archive/2011/03/02/1969006.html)

---如果只是实现最基本的反向代理功能，即不论域名是什么，统一把这个端口的请求发送给后台的多台服务器。那么可以这么修改配置文件

---http{

---...

---...

---...

---upstream mysvr{
        server

---127.0

---.0

---.1

---:

---8002

---;

---\# data server 数据服务器内网端口

---server

---127.0

---.0

---.1

---:

---8003

---;

---\# data server 数据服务器内网端口

---server

---127.0

---.0

---.1

---:

---8004

---;

---\# data server 数据服务器内网端口

---}
    server {
        listen

---8001

---;

---\# nginx监听的端口，也是3台数据服务器对外的端口

---server_name  localhost;

---\#charset koi8-r;

---\#access_log  logs/host.access.log  main;

---location / {

---\# root   /html;

---\# index  index.html index.htm;

---proxy_pass http://mysvr ;

---\# 对所有域名，使用proxy_pass反向代理到mysvr里面的端口

---}

---...

---...

---...

---}

---...

---...

---...

---}

---首先要加mysvr一项，然后在location / 项里面将前两行注释掉，增加proxy_pass选项即可。我这里是把8001端口的请求全部发送到8002，8003，8004这3个端口。配置好以后，先启动服务器，

---DataServer().

---listen

---(

---8002

---)
DataServer().

---listen

---(

---8003

---)
DataServer().

---listen

---(

---8004

---)

---启动nginx，打开浏览器，输入127.0.0.1:8001/auth，得如下所示

![这里写图片描述](https://img-blog.csdn.net/20160313101211595)
---[ ](https://img-blog.csdn.net/20160313101211595)

---可知8001端口的请求确实被发送到了8002等端口

---[
](https://img-blog.csdn.net/20160313101211595)

---3.处理静态文件请求

---所谓静态文件，就是指不会经常变化的文件，例如html,图片，css,js等文件。这些文件如果全部交由tornado来上传的话，太浪费tornado金贵的IO时间了，而且用tornado来处理这些请求也不见得有多方便。所以，nginx很好的承担了这个任务。

---举个栗子，我所有的图片请求格式都是 url:port/pic/pid.jpg的格式，我就可以在nginx.conf文件里面加这么一段话

---http{

---...

---...

---server{

---...

---...

---location /pic/{
            alias /multiangle/Coding!/python/milu_backstage/public/pic/ ;
        }

---...

---...

---}

---...

---...

---}

---上面这段话的意思是，对所有格式为url:port/pic/pid.jpg的请求，nginx都会在

---/multiangle/Coding!/python/milu_backstage/public/pic/

---这个文件夹下面寻找名字为pid.jpg的文件，并返回给请求者。


---[
						](https://img-blog.csdn.net/20160313101211595)

---[
	](https://img-blog.csdn.net/20160313101211595)

