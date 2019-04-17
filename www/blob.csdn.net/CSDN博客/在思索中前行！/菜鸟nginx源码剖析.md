# 菜鸟nginx源码剖析 - 在思索中前行！ - CSDN博客





2014年12月02日 15:50:12[_Tham](https://me.csdn.net/txl16211)阅读数：668标签：[Nginx配置管理																[菜鸟Nginx源码解读																[Nginx入门到精通](https://so.csdn.net/so/search/s.do?q=Nginx入门到精通&t=blog)
个人分类：[鸟哥的私房菜学习																[php开发](https://blog.csdn.net/txl16211/article/category/2231073)](https://blog.csdn.net/txl16211/article/category/2590995)





> 
# 菜鸟nginx源码剖析 配置与部署篇（一） 手把手配置nginx “I love you”


**[TCMalloc 对MYSQL 性能 优化的分析](http://blog.csdn.net/chen19870707/article/category/2628763)**

# [菜鸟nginx源码剖析系列文章解读](http://blog.csdn.net/chen19870707/article/category/2647301)





Author：Echo Chen（陈斌）


Email：chenb19870707@gmail.com


Blog：[Blog.csdn.net/chen19870707](http://blog.csdn.net/chen19870707)


Date：Nov 7th, 2014

> 

还记得在前几年的CSDN泄漏账号事件中，统计发现程序员的账号中含有love的最多，这里我也俗套下，在这篇文章中将讲解如何 一步一步实用Nginx在一台机器上搭建一个最简单的显示“I love you”的Web服务器，大家一起动手尝试吧。


> 

## 1.Nginx 程序包

> 

目前nginx最新的开发版本为1.7.7：
- Linux：[nginx-1.7.7.tar.gz](http://nginx.org/download/nginx-1.7.7.tar.gz)
- Windows：[nginx-1.7.7.zip](http://nginx.org/download/nginx-1.7.7.zip)


稳定版本为1.6.2：
- Linux：[nginx-1.6.2.tar.gz](http://nginx.org/download/nginx-1.6.2.tar.gz)
- Windows：[nginx-1.6.2.zip](http://nginx.org/download/nginx-1.6.2.zip)


> 

> 

大家可以根据自己的操作系统和喜好选择程序包来实现，这里以[nginx-1.7.7.tar.gz](http://nginx.org/download/nginx-1.7.7.tar.gz) 在**CentOs 6.4**操作系统下为例。


## 2.Nginx下载解压、安装

> 

下载和解压，这里以[nginx-1.7.7.tar.gz](http://nginx.org/download/nginx-1.7.7.tar.gz) 在**CentOs 6.4**操作系统下为例:


> 
   1: wget http://nginx.org/download/nginx-1.7.7.tar.gz   2: tar zxvf nginx-1.7.7.tar.gz 


> 

但是下载解压完不要急着安装，因为 Nginx 依赖很多软件（gcc、g++、make、`libz-dev、``libbz2-dev、``libreadline-dev`），我们假设您的 Linux 环境是“纯净”的，所以下面我们就来安装这些依赖包。请按照如下步骤安装：




   1: yum install gcc -y   2: yum install g++ -y   3: yum install make -y   4: yum install zlib-dev* -y   5: yum install bzip2-dev* -y   6: yum install readline-devel -y





这是一些基本的软件，另外还有 PCRE 要安装。PCRE 是“Perl Compatible Regular Expressions”的缩写，是一个正则表达式库。下载、解压并安装 PCRE：




   1: wget [ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.36.tar.gz](ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.36.tar.gz)

   2: tar -zxvf pcre-8.36.tar.gz   3: cd pcre-8.36.tar.gz   4: ./configure   5: make   6: make install



> 

好了，这下可以安装我们的nginx了




   1: tar -zxvf nginx-1.7.7.tar.gz   2: cd nginx-1.7.7   3: ./configure   4: sudo make   5: sudo make install





## 3.Nginx 配置文件

> 

按照上面的操作，Nginx 就已经安装完了。因为我们使用的是默认的 configure，所以会安装到`/usr/local/nginx`目录。相应的配置文件是`/usr/local/nginx/conf/nginx.conf`。我们打开配置文件，看到结构形如：




   1: ...   2: #工作模式与连接数上限   3: events   4: {   5: #参考事件模型，use [ kqueue | rtsig | epoll | /dev/poll | select | poll ]; epoll模型是Linux 2.6以上版本内核中的高性能网络I/O模型，如果跑在FreeBSD上面，就用kqueue模型。   6: use epoll;   7: #单个进程最大连接数（最大连接数=连接数*进程数）   8: worker_connections 65535;   9: }  10:  11: #设定http服务器，利用它的反向代理功能提供负载均衡支持  12: http  13: {  14:     ...  15:     #第一个虚拟服务器  16:    server {  17:     #侦听192.168.8.x的80端口  18:         listen       80;  19:         server_name  192.168.8.x;  20:        }  21: }  22:



> 其中`events模块表示工作模式与链接数上限，http模块的作用为设定http服务器，server模块表示虚拟服务器，这里只是简要列出最简单的配置，如果有需要对配置进行进一步了解，可以参考《`[Nginx配置文件nginx.conf中文详解](http://www.ha97.com/5194.html)`》。`

## 4.Nginx 一个简单配置的编写

> 

建立一个/root/test_space目录，用来存储我们的测试页面。再直接在 Nginx 的默认配置文件/usr/loca/nginx/conf/nginx.conf中修改，在http中增加一个server模块，如下：


> 

   1: server {   2:     listen      8011;   3:     server_name localhost;   4:     charset     utf-8;   5:     location / {   6:         alias   /root/test_space/;   7:     }   8: }



> 

其中`listen`表示监听的端口号，`sever_name`则是`web`服务器的名称（可以是域名、host 或 IP 地址），`charset`指定编码字符集，这里的`location`则通过`alias`指定了`web`服务的文件目录。


## 5.Nginx启动




   1: /usr/local/nginx/sbin/nginx    2: s -ef | grep nginx


> 

显示如下则启动成功：

![image](https://img-blog.csdn.net/20141108165620324)


## 6.测试

> 

在/root/test_space下建立如下html文件：




   1: <html>   2:    <head></head>   3:    <body>I love you!<body>   4: </html>




-


Echo Chen：[Blog.csdn.net/chen19870707](http://blog.csdn.net/chen19870707)](https://so.csdn.net/so/search/s.do?q=菜鸟Nginx源码解读&t=blog)](https://so.csdn.net/so/search/s.do?q=Nginx配置管理&t=blog)




