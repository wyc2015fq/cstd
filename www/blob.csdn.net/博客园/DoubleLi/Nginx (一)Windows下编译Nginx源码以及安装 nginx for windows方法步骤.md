# Nginx (一)Windows下编译Nginx源码以及安装 nginx for windows方法步骤 - DoubleLi - 博客园






转载自:

[http://apps.hi.baidu.com/share/detail/11192699#content](http://apps.hi.baidu.com/share/detail/11192699#content)



Nginx介绍：

Nginx ("engine x")是一个高性能的 HTTP 和反向代理服务器，也是一个IMAP/POP3/SMTP代理服务器。 Nginx 是由 Igor Sysoev 为俄罗斯访问量第二的 Rambler.ru 站点开发的，它已经在该站点运行超过两年半了。Igor将源代码以类BSD许可证的形式发布。
　　Nginx 超越 Apache 的高性能和稳定性，使得国内使用 Nginx作为 Web 服务器的网站也越来越多，其中包括新浪博客、新浪播客、网易新闻等门户网站频道，六间房、56.com等视频分享网站，Discuz!官方论坛、水木社区等知名论坛，豆瓣、YUPOO相册、海内SNS、迅雷在线等新兴Web 2.0网站。

Nginx 的官方中文维基：http://wiki.nginx.org/NginxChs

1.      cygwin环境安装
到http://www.cygwin.com/下载setup.exe安装程序，具体安装过程请到google找吧。这里需要注意的是：一定要安装上openssl、pcre与zlib这三个包，因为nginx部分源码需要用到这三个库。

![](http://hiphotos.baidu.com/ahong1288/pic/item/4981272d7698a719359bf7dc.jpg)

默认安装不包括上述三个库：

![](http://hiphotos.baidu.com/ahong1288/pic/item/f48c544e66ec5e24b2de05de.jpg)


上图为完全安装时会包括上述三个库。


2.nginx编译、构建与安装

1、从nginx.net下载源码 nginx-0.7.62.tar.gz存放到D:\linux\下。

2、使用cygwin把nginx安装到d:/nginx。

安装命令如下：

1.         cd /cygdrive/d/linux

2.         tar zxvf nginx-0.7.62.tar.gz

3.         cd nginx-0.7.62

4. /configure --prefix=/cygdrive/d/nginx --sbin-path=/cygdrive/d/nginx --with-cc-opt="-D FD_SETSIZE=4096" --with-rtsig_module --with-select_module --with-poll_module --with-http_ssl_module --with-http_realip_module --with-http_addition_module --with-http_xslt_module --with-http_sub_module --with-http_dav_module --with-http_flv_module --with-http_gzip_static_module --with-http_random_index_module --with-http_secure_link_module --with-http_stub_status_module --with-mail --with-cpp_test_module --with-mail_ssl_module

5. make && make install

执行完第4步：得到一个Makefile文件与objs子目录下的多个文件。

执行完第5步的make命令，这时cd进去objs子目录，然后ls一下，看到有nginx.exe文件。这就是编译构建后生成的nginx程序。make install命令是安装程序到指定的目录中。

如果不加--with-cc-opt=”-DFD_SETSIZE=4096”的话，你装好nginx后运行会出现

   2008/01/12 16:34:56 [emerg] 2496#0: the maximum number of files supported by select() is 64的错误提示，这表示FD_SETSIZE的值比nginx配置文件中worker_connections指令所指定的值，你可以把nginx.conf里的worker_connections选项改小一些，比如44,加了--with-cc-opt=”-D FD_SETSIZE=4096”后就不会碰到这问题

上面的第4、5步每次都要输入这么多命令，调试时非常麻烦。有一个办法可以减少工作量。就是在nginx-0.7.62目录中，新建一个记事本文件，里面内容为：

./configure --prefix=/cygdrive/d/nginx --sbin-path=/cygdrive/d/nginx --with-cc-opt="-D FD_SETSIZE=4096" --with-rtsig_module --with-select_module --with-poll_module --with-http_ssl_module --with-http_realip_module --with-http_addition_module --with-http_xslt_module --with-http_sub_module --with-http_dav_module --with-http_flv_module --with-http_gzip_static_module --with-http_random_index_module --with-http_secure_link_module --with-http_stub_status_module --with-mail --with-cpp_test_module--with-mail_ssl_module;make && make install

然后把这个记事本文件改名为makeexe，保存文件。注意没有后缀名。

在cygwin窗口中执行sh makeexe，相当于执行第4、5步。
- nginx运行

把nginx运行时所需要用到的DLL文件找出来，(这些文件都在cygwin安装目录的bin子目录下)，复制到d:/nginx目录下。所需文件如下图所示：


![](http://img.my.csdn.net/uploads/201210/03/1349231412_9099.jpg)最后就可以直接双击nginx.exe来运行nginx了。这时打开浏览器，输入地址：http://127.0.0.1/，如果能看到有“Welcome to nginx!”显示出来就表示nginx已经在运行，如果没有的话就打开logs子目录下的error.log文件，看看到底发生了什么错误。

start-nginx.bat批处理文件内容：

@ECHO OFF

d:\nginx\nginx.exe

ping 127.0.0.1 -n 1>NUL

echo Starting nginx

echo .

echo .

echo .

ping 127.0.0.1 >NUL

EXIT

stop-nginx.bat批处理文件内容：

@ECHO OFF

process -k nginx.exe

process -k nginx.exe

process -k nginx.exe

process -k nginx.exe

echo .

echo .

echo .

EXIT












