# Centos配置Nginx负载均衡详解 - fanyun的博客 - CSDN博客
2016年07月15日 10:11:21[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：5038
所属专栏：[Linux系统开发维护实战](https://blog.csdn.net/column/details/linux-01.html)
        在日常网络数据开发中，我们对服务器的处理能力要求很高，但是在服务器有限的情况下，怎么才能更好的利用服务器资源，使得我们的服务器最大限度发挥自己的作用呢？**负载均衡**是一种很好的办法。
**    哪什么是Nginx负载均衡呢？**
       Nginx是一个轻量级的、高性能的WebServer，他主要可以干下面两件事：
(1).作为http服务器（和apache的效果一样）
(2).作为反向代理服务器实现负载均衡
        现在Nginx到处都可以见到，经常会看到宕机后的网页会显示nginx的字样，这也说明Nginx由于高性能、使用配置简、开源单这些特点被越来越多的用户所接受，所使用。其中第一种作为http服务器，结合php-fpm进程，对发来的请求进行处理，nginx本身并不会解析php，他只是作为一个服务器，接受客户端发来的请求，如果是php请求，则交给php进程处理，并将php处理完成之后的结果发送给客户端。这个很简单，安装好nginx+php-fpm之后配置好各自的配置文件，启动就可以实现。运行原理可以看下面这段解释：
        Nginx不支持对外部程序的直接调用或者解析，所有的外部程序（包括PHP）必须通过FastCGI接口来调用。FastCGI接口在Linux下是socket（这个socket可以是文件socket，也可以是ip socket）。为了调用CGI程序，还需要一个FastCGI的wrapper（wrapper可以理解为用于启动另一个程序的程序），这个wrapper绑定在某个固定socket上，如端口或者文件socket。当Nginx将CGI请求发送给这个socket的时候，通过FastCGI接口，wrapper接收到请求，然后派生出一个新的线程，这个线程调用解释器或者外部程序处理脚本并读取返回数据；接着，wrapper再将返回的数据通过FastCGI接口，沿着固定的socket传递给Nginx；最后，Nginx将返回的数据发送给客户端。这就是Nginx+FastCGI的整个运作过程，如图1所示。
[](http://www.linuxdiyf.com/linux/)
![](https://img-blog.csdn.net/20160715092634931?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                         图1   Nginx+FastCGI运作图
        上面这段话解释了nginx+fastcgi的运行机制，在nginx配置文件中会对请求进行匹配，并作做出相应的处理，比如说直接返回错误文件（这里和上面说的有点区别，我估计是nginx内部对html等这些静态文件可以做类似上图的解析），使用php进程对php请求进行处理（这里的php进程可以是多个）。
        第二种是用反向代理事项负载均衡，这个其实其实很简单，说起来就是自己定义一组server，对请求进行匹配，并将请求转给server中的任意一个处理，来减轻每个server的压力，先看看网上对反向向代理的定义：
        反向代理（Reverse Proxy）方式是指以代理服务器来接受internet上的连接请求，然后将请求转发给内部网络上的服务器，并将从服务器上得到的结果返回给internet上请求连接的客户端，此时代理服务器对外就表现为一个反向代理服务器。
![](https://img-blog.csdn.net/20160715095414048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                         图2   Nginx负载均衡示意图
        反向代理是和正向代理（或者叫代理）相反的，代理大家定听过吧，为了更方便的访问B资源，通过A资源间接的访问B资源，特点就是用户知道自己最终要访问的网站是什么，但是反向代理用户是不知道代理服务器后边做了什么处理的，反向代理中服务真正的处理服务器放在内网，对外网而言只可以访问反向代理服务器，这也大大提高了安全性。
安装软件
nginx安装很简单
1、安装nginx需要的环境，pcre（作用rewrite）、zlib（作用压缩）、ssl，这个也可以自己下载编译安装
yum -y install zlib;
yum –y install pcre;
yum –y install openssl;
2、下载安装nginx-*.tar.gz。
tar –zxvf nginx-1.2.8.tar.gz –C ./;
cd nginx-1.2.8;
./congigure --prefix=/usr/local/nginx;
make && make install;
3、配置
        这里配置的时候只需要修改http{}之间的内容就行了，修改的第一个地方就是设置服务器组，在http节点之间添加
upstream myServer{
server www.abc.com:80;  #这里是你自己要做负载均衡的服务器地址1
server www.hao.com:8080; #这里是要参与负载均衡的地址2
}
        nginx中的upstream支持下面几种方式：轮询（默认，按照时间顺序对所有服务器一个一个的访问，如果有服务器宕机，会自动剔除）、weight（服务器的方位几率和weight成正比，这个可以在服务器配置不均的时候进行配置）、ip_hash（对每个请求的ip进行hash计算，并按照一定的规则分配对应的服务器）、fair（按照每台服务器的响应时间（rt）来分配请求，rt晓得优先分配）、url_hash（按照访问url的hash值来分配请求），我这里使用了默认的轮训方式。
将请求指向myServer
location / {
proxy_pass  http://myServer;
}
完整的文件（删除注释）如下：
worker_processes  1;
events {
worker_connections  1024;
}
http {
include      mime.types;
default_type  application/octet-stream;
sendfile        on;
keepalive_timeout  65;
upstream myServer{
server www.hao.com:80;
server www.abc.com:8080;
}
server {
listen      80;
server_name  my22;
location / {
proxy_pass  http://myServer;
}
}
}
设置反向代理后端作为负载均衡的两个服务器
        可以看到上一步骤有两个服务器地址，www.hao.com:80和www.abc.com:8080，上面的nginx我是安装在虚拟机上面的，这两个服务器我是安装在本机win8系统中的，使用apache的virtualhost，设置了两个域名，这两个域名下的代码是互相独立的，设置也很简单：
1、设置apache配置文件
        我使用的是xampp集成环境，要修改的地方有两个，在httpd.conf中监听端口的地方添加
Listen 8080
也就是说这个地方监听了两个端口
Listen 80
Listen 8080
看看下面这个句是否打开，没有打开的话，打开，打开如下面所示
# Virtual hosts
Include conf/extra/httpd-vhosts.conf
在httpd-vhosts.conf中添加下面的内容，
<VirtualHost *:80>
ServerName www.hao.com #对应的域名，负载均衡的服务器地址
DocumentRoot E:\soft\xampp\htdocs\www.hao.com #代码文件夹
</VirtualHost>
<VirtualHost *:8080>
ServerName www.abc.com
DocumentRoot E:\soft\xampp\htdocs\www.abc.com
</VirtualHost>
修改windows的hosts文件，追加下面的内容：
127.0.0.1       www.hao.com
127.0.0.1        www.abc.com
修改linux的/etc/hosts文件，追加下面的内容：
78.45.13.26       www.hao.com #这里前面的地址对应我win8本机的ip地址
78.45.13.26        www.abc.com
我在www.hao.com:80中放了一个文件index.php【E:\soft\xampp\htdocs\www.hao.com\index.php】
www.abc.com:8080中也放了一个文件index.php【E:\soft\xampp\htdocs\www.abc.com\index.php】
文件中的内容基本相同，只是I'm the abc这个地方有区别，一个是linux，另一个是abc。
如果你可以在win8浏览器中输入www.hao.com:80和www.abc.com:8080看到不同的效果
并且在CentOS下面看到下面的结果（自己美化了下）说明配置成功了
[root@bogon nginx]# curl www.hao.com:80
I'm the linux<br>【view】1
[root@bogon nginx]# curl www.abc.com:8080
I'm the abc<br>【view】1
<?php
session_save_path("./");
session_start();
header("Content-type:text/html;charset=utf-8");
if(isset($_SESSION['view'])){
$_SESSION['view'] = $_SESSION['view'] + 1;
}else{
$_SESSION['view'] = 1;
}
echo "I'm the abc<br>";
echo "【view】{$_SESSION['view']}";
看看效果
等所有都ok之后可硬通过浏览器访问看看效果
忘了说了，nginx代理服务器的地址为http://78.36.11.118，
浏览器输入http://78.36.11.118/index.php之后，不停的刷新，你会发现，会在
I'm the abc、I'm the linux
        这两个页面之间来回交换，view会没刷新两下增加一次，这也证明了前面所说的默认是轮训的方式，但这里又有一个比较常见的问题了，当用户访问网站时，未做处理的情况下，session会保存在不同的服务器上（我这里用两个不同的文件夹模拟两台服务器）。
