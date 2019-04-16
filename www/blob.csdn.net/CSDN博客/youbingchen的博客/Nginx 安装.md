# Nginx 安装 - youbingchen的博客 - CSDN博客





2016年06月07日 17:09:55[youbingchen](https://me.csdn.net/youbingchen)阅读数：715标签：[nginx](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)
个人分类：[nginx](https://blog.csdn.net/youbingchen/article/category/6263213)









今天第一天来实习，带我的老师叫我先了解nginx这个东东，所以对这个了解还是比较肤浅。这里面 有错误请大家积极指正。

# nginx简介

Nginx是一个高性能的HTTP和反向代理服务器，也是一个IMAP/POP3/SMTP代理服务器，Nginx是罗斯的程序设计师lgorsysoev所开发 

将其源码以类BSD许可证的形式发布，可以运行在Unix、GUN/Linux、solaris、BSD、MicrosoftWindows等操作系统中，因其稳定性、丰富的功能模块、低系统资源消耗而文明；现在Nginx已经在众多流量很大的网站上使用了很长时间，如国外的网站Yandex、Mial.Ru、Vkontakte、Rambler等，在国内使用Nginx的网站如：搜狐、新浪、腾讯、金山网络等。

# Nginx 的优越性
- 作为Web代理 服务器，Nginx处理静态文件、索引文件以及自动索引的效率非常的高。
- 作为代理服务器，Nginx可以使用缓存加速反向代理 ，提高网站的速度
- 作为负载均衡服务器，Nginx既可以在内部直接支持Rails和PHP程序对外服务，也可以支持HTTP代理服务器对外进行服务，同时还 支持讲的容错和利用算法负载均衡。
- Nginx采用内核epoll模型，可以支持更多的并发连接，最多支持50000个并发 连接的响应，而且只用很低的内存资源
- Nginx安装简单，启动特别 容易，并且可以做到7*24不间断运行，即使运行数月也无需重新启动。

Ngnix有太多的优点，这次我主要 针对 代理缓存 和 高 并发 的两个特点进行探索。其余也会慢慢更新到

# Nginx 安装前环境配置(以Ubuntu为例)



- 第一步 安装Gcc、G++编译器 




> 
  G++编译器用于编译使用C++编写 的Nginx HTTP模块
  
  

```
apt-get install  build-essential
apt-get install  libtool
```

如果是 centos系统

```
#安装make
yum  -y install  gcc  automake  autoconf libtool make
# 安装g++ 
yum install gcc gcc-c++
```



- 第二步 PCRE（perl Compatible Regular Expression） 




> 
  用于 支持正则表达式，Nginx的HTTP模块需要用它来解析正则表达式[PCRE](ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/)

```
tar  -jxvf pcre2-10.20.tar.bz2 
cd pcre2-10.20/
./configure
make &&　make install
```



- 第三步 [Zlib库](http://zlib.net/)的 安装 




> 
用于支持HTTP包内容做gzip格式压缩
  


```
tar -zxvf  zlib-1.2.8.tar.gz
cd zlib-1.2.8
./configure
make  & make install
```



- 第四步安装OpenSSL库 




> 
  用于支持更安全的SSH协议上传输HTTP。[opensssl](https://www.openssl.org)

```
tar -zxvf openssl-1.0.1e.tar.gz
 cd openssl-1.0.1e/
./configure
make
make install
```



- 最后一步安装Nginx 




> 
  这里有个小插曲，如果用pcre2.XX的版本，nginx安装老是会出错，所以我就将pcre的环境换成pcre-8.xxx
  
  

```
tar -zxvf nginx-1.11.1.tar.gz
cd nginx-1.11.1
./configure --sbin-path=/usr/local/nginx/nginx \
--conf-path=/usr/local/nginx/nginx.conf \
--pid-path=/usr/local/nginx/nginx.pid \
--with-http_ssl_module  \
--with-pcre=/usr/local/src/pcre-8.37 \ # 这里原来是别的版本，出错了
--with-zlib=/usr/local/src/zlib-1.2.8 \
--with-openssl=/usr/local/src/openssl-1.0.1t 
make
make  install
#  --with-pcre=/usr/src/pcre-8.37 指的是pcre-8.37 的源码路径。
#--with-zlib=/usr/src/zlib-1.2.8 指的是zlib-1.2.7 的源码路径。
```

#### nginx编译选项

> 
make是用来编译的，它从Makefile中读取指令，然后编译。makeinstall是用来安装的，它也从Makefile中读取指令，安装到指定的位置。configure命令是用来检测你的安装平台的目标特征的。它定义了系统的各个方面，包括nginx的被允许使用的连接处理的方法，比如它会检测你是不是有CC或GCC，并不是需要CC或GCC，它是个shell脚本，执行结束时，它会创建一个Makefile文件。nginx的configure命令支持以下参数： 

  –prefix=path    定义一个目录，存放服务器上的文件 ，也就是nginx的安装目录。默认使用 /usr/local/nginx。 

  –sbin-path=path 设置nginx的可执行文件的路径，默认为  prefix/sbin/nginx. 

  –conf-path=path  设置在nginx.conf配置文件的路径。nginx允许使用不同的配置文件启动，通过命令行中的-c选项。默认为prefix/conf/nginx.conf. 

  –pid-path=path  设置nginx.pid文件，将存储的主进程的进程号。安装完成后，可以随时改变的文件名 ， 在nginx.conf配置文件中使用 PID指令。默认情况下，文件名 为prefix/logs/nginx.pid. 

  –error-log-path=path 设置主错误，警告，和诊断文件的名称。安装完成后，可以随时改变的文件名 ，在nginx.conf配置文件中 使用 的error_log指令。默认情况下，文件名 为prefix/logs/error.log. 

  –http-log-path=path  设置主请求的HTTP服务器的日志文件的名称。安装完成后，可以随时改变的文件名 ，在nginx.conf配置文件中 使用 的access_log指令。默认情况下，文件名 为prefix/logs/access.log. 

  –user=name  设置nginx工作进程的用户。安装完成后，可以随时更改的名称在nginx.conf配置文件中 使用的 user指令。默认的用户名是nobody。 

  –group=name  设置nginx工作进程的用户组。安装完成后，可以随时更改的名称在nginx.conf配置文件中 使用的 user指令。默认的为非特权用户。 

  –with-select_module –without-select_module 启用或禁用构建一个模块来允许服务器使用select()方法。该模块将自动建立，如果平台不支持的kqueue，epoll，rtsig或/dev/poll。 

  –with-poll_module –without-poll_module 启用或禁用构建一个模块来允许服务器使用poll()方法。该模块将自动建立，如果平台不支持的kqueue，epoll，rtsig或/dev/poll。 

  –without-http_gzip_module — 不编译压缩的HTTP服务器的响应模块。编译并运行此模块需要zlib库。 

  –without-http_rewrite_module  不编译重写模块。编译并运行此模块需要PCRE库支持。 

  –without-http_proxy_module — 不编译http_proxy模块。 

  –with-http_ssl_module — 使用https协议模块。默认情况下，该模块没有被构建。建立并运行此模块的OpenSSL库是必需的。 

  –with-pcre=path — 设置PCRE库的源码路径。PCRE库的源码（版本4.4 - 8.30）需要从PCRE网站下载并解压。其余的工作是Nginx的./ configure和make来完成。正则表达式使用在location指令和 ngx_http_rewrite_module 模块中。 

  –with-pcre-jit —编译PCRE包含“just-in-time compilation”（1.1.12中， pcre_jit指令）。 

  –with-zlib=path —设置的zlib库的源码路径。要下载从 zlib（版本1.1.3 - 1.2.5）的并解压。其余的工作是Nginx的./ configure和make完成。ngx_http_gzip_module模块需要使用zlib 。 

  –with-cc-opt=parameters — 设置额外的参数将被添加到CFLAGS变量。例如,当你在FreeBSD上使用PCRE库时需要使用:–with-cc-opt=”-I /usr/local/include。.如需要需要增加 select()支持的文件数量:–with-cc-opt=”-D FD_SETSIZE=2048”. 

  –with-ld-opt=parameters —设置附加的参数，将用于在链接期间。例如，当在FreeBSD下使用该系统的PCRE库,应指定:–with-ld-opt=”-L /usr/local/lib”. 

  + 最后 一步 启动
```
/usr/local/nginx/sbin/nginx  #每个人的地址可能不一样，同时 确保 80端口没人用
netstat  -ano  | grep 80
```





