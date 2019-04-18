# 静态编译web server Appweb(带Matrixssl支持) For ARM9+ linux - ljx0305的专栏 - CSDN博客
2013年03月01日 14:26:37[ljx0305](https://me.csdn.net/ljx0305)阅读数：5089

在众多的web server中有许多轻量级的web server,如boa , thttp, lighttpd等,但如果要有较高的安全性和能为后期的web开发开发提供便利的嵌入式web服务器,上面提到的几个web server显得太单薄了.公司要求用web来管理开发的嵌入式产品(就像路由器那种),并且要有一定的安全性能.综合比较了大几款轻量级,嵌入式web server.最后推荐都是由Michael O'Brien架构的两个web服务器:goahead   ([http://webserver.goahead.com/webserver/webserver.htm](http://webserver.goahead.com/webserver/webserver.htm) )和appweb([http://www.appwebserver.org/downloads/appWeb/download.php](http://www.appwebserver.org/downloads/appWeb/download.php)).
      其中前者是GoAhead公司的Embedded Management Framework产品的一部分，这个软件包主要用于解决未来嵌入式系统开发的相关问题,这款WEB服务器非常小巧，它的WIN CE版本编译后的大小还不到60k，它的输出通常也是面向一些小屏幕设备。GPL & Commercial License.号称要做成未来嵌入式web server的领头羊.
     后者是Michael O'Brien创立的Mbedthis公司的第一个产品,GPL & Commercial License.其中MOB本人将其描述为mini-apache.在使用的过程中,确实也感觉到了这点:通过appweb.conf 这个配置文件能支持模块的动态加载.网上讨论说上面两款是大多数路由器等设备web server的首选,下面贴上这两款web server的主要特性:
     appweb features(懒得翻译):
AppWeb is a standards-based embedded HTTP server that has a wealth of features including:
1，Standards compliant HTTP/1.1 web server core
2，Embeddable in applications and devices (supports XIP)
3，Dynamically loadable modules
4，Embedded Server Pages (ESP) for dynamic content   (其实是为了支持像goahead上的ASP模块而换的一种称呼)
5，Session state data management
6，Server-side Embedded JavaScript
7，Secure Socket Layer (SSL) :           (OpenSSL和MatrixSsl)
8，In-process CGI as well as traditional CGI
9，ROMable web pages and configuration files
10，Digest Authentication               ********
11，HTTP Client program and library
12，Extensive logging
13，Fastest performance in its class
14，Modular source code and documentation provided
15，Apache-style configuration, logging, and single- and multi-threaded apps.
   因为公司公司的产品要考虑安全性问题,这里得指出matrixssl这个ssl 3.0实现.Matrixssl是专为嵌入式产品开发的一个SSL,比openssl远远要小,GPL & Commercial License,占用 50K RAM左右。
在性能方面，使用一颗24MH z的68040处理器，它的响应速度为20次/秒，使用266MHz的Pentium处理器可以达到50次/秒的响应速度。其具有以下特性：
1,支持ASP
2,支持嵌入式的JavaScript
3,标准的CGI执行
4,内存中的CGI处理GoFroms
5,扩展的API
6,快速响应，每秒可处理超过50个请求
7,完全和标准兼容
8, 如果不包含SSL，仅要求60K的内存；包含SSL，要求500K内存
9, web页面可以存在ROM或文件系统中
10,支持多种操作系统，包括eCos 、LINUX 、LynxOS 、QNX 、VxWorks 、WinCE、pSOS等
    由于goahead要求在使用它时,需在主页注明用的是goahead,觉得这点不是很好,就主要做了appweb的移植.可能是国人用它比较少,只看到一个帖子.参考了一下,还是出现了许多问题,另外也没有支持SSL.之后自己看英文文档,在appweb的论坛上看相关的问题,最后终于解决了一大堆疑惑,成功移植到了at91rm9200dk+linux上.为避免后来人走弯路,难得的一次原作,今天就把它晾在着了.个人觉得应该是比较详细了.
注: 环境为Cent OS 4.4 {kernel version:2.6.9-42.EL ,gcc version: 3.4.6 (Red Hat 3.4.6-3)}
      Crosstool:  arm-softfloat-linux-gnu-gcc  gcc-3.4.1-glibc-2.3.3  
1,下载appweb源代码:    [http://www.appwebserver.org/software/appweb-src-2.4.0-0.tar.gz](http://www.appwebserver.org/software/appweb-src-2.4.0-0.tar.gz)
2,解压:
#tar -xvzf appwe-src-2.4.0-0.tar.gz -C /usr/local/src
3,修改环境变量:
修改CCAR LD RANLIB STRIP为交叉编译环工具;
修改CC_FOR_BUILD为PC的gcc:
编写shell脚本export.sh如下:
#!/bin/bash
exportCC="/opt/crosstool/gcc-3.4.1-glibc-2.3.3/arm-softfloat-linux-gnu/bin/arm-softfloat-linux-gnu-gcc"
exportAR="/opt/crosstool/gcc-3.4.1-glibc-2.3.3/arm-softfloat-linux-gnu/bin/arm-softfloat-linux-gnu-ar"
exportLD="/opt/crosstool/gcc-3.4.1-glibc-2.3.3/arm-softfloat-linux-gnu/bin/arm-softfloat-linux-gnu-ld"
exportRANLIB="/opt/crosstool/gcc-3.4.1-glibc-2.3.3/arm-softfloat-linux-gnu/bin/arm-softfloat-linux-gnu-ranlib"
exportSTRIP="/opt/crosstool/gcc-3.4.1-glibc-2.3.3/arm-softfloat-linux-gnu/bin/arm-softfloat-linux-gnu-strip"
exportCC_FOR_BUILD="gcc"
echo"@ done!"
#source export.sh或 . export.sh 运行shell脚本并使它生效.
4,下载matrixssl源代码:   [http://www.matrixssl.org/download.html](http://www.matrixssl.org/download.html)
5,解压到/usr/local下:
#tar-xvzf matrixssl-1-8-3-open.tar.gz -C /usr/local
6,进入/usr/local/src/matrixssl-1-8-3-open/src下修改Makefile:
修改CC,STRIP,AR,RANLIB为我们的交叉编译器,如:
CC = /opt/crosstool/gcc-3.4.1-glibc-2.3.3/arm-softfloat-linux-gnu/bin/arm-softfloat-linux-gnu-gcc
7,make
此时会在当前目录下生成动态链接库libmatrixssl.so和静态链接库libmatrixsslstatic.a这在下面一步配置和编译Appweb时将会用到.
8,对appweb进行./configure:
#cd/usr/local/src/appweb-src-2.4.0
#./configure--host=arm-atmel-linux --build=i686-pc-linux --port=80 --type=RELEASE--disable-log --disable-multi-thread --disable-shared--disable-samples --disable-shared-libc --enable-static--disable-test --disable-access-log --enable-ejs --disable-modules--with-cgi=builtin
 --with-copy=builtin --with-auth=builtin--with-esp=builtin --with-upload=builtin --without-c-api--without-php5 --with-ssl=builtin --with-matrixssl=builtin--with-matrixssl-dir=/usr/local/matrixssl/src--with-matrixssl-libpath=/usr/local/matrixssl/src--with-matrixssl-iflags="-I
 /usr/local/matrixssl/src "--with-matrixssl-libs=libmatrixsslstatic
可以通过./configure–-help查看相关说明.关于—with-matrixssl-libs需要说明:这个选项是告诉链接器,在生成appweb可执行文件时链接哪matrixssl一个lib库.如果想动态编译的话,那么我们就选择libmatrixssl.so这个库,此时我们应该指定withmatrixssllibs=libmatrixssl; 如果想静态编译的话,我们就应该选择libmatrixsslstatic.a这个静态库文件.此时应该指定
--with-matrixssl-libs=libmatrixsslstatic.
9 make
在此过程中会出现matrixssl源代码下的src/os/linux.c中的3个函数未定义错误,此时我们将相关部分注释掉就OK了.
line 123: pthread_mutexattr_init()
line 125 pthread_mutexattr_settype();
line 135 pthread_mutexattr_destroy()
make成功后,将在当appweb目录下生成appweb和appweb.conf文件.另外,里面还有一些matrixssl运行需要的一些Key.
mkdir /usr/local/nfs 建立nfs服务工作目录,并将appweb运行需要的文件放到此目录下:
这些文件有:appweb(bin) ,appweb.conf(配置文件) ,mime.types ,目录web(DocumentRoot),server.key.pem,server.crt,server.key
10 开启NFS服务:
修改/etc/exports文件内容如下:
/usr/local/nfs *(rw,sync,root_squash)
#service protmap start
#service nfs start
#showmount -a 查看共享和访问的目录
开发上板上挂载NFS(确定开发的IP地址和主机地址在同一个网段内):
#mkdir /tmp/nfs
#mount -t nfs hostip:/usr/local/nfs
11 运行appweb,开启web服务
./appweb -f appweb.conf
如果出现错误,按出错信息修改appweb.conf配置文件.
在PC上访问appweb提供的web服务,在firefox下输入目标板的IP地址和端口号
[http://10.78.28.218:80/](http://10.78.28.218/)
[https://10.78.28.218:4443/](https://10.78.28.218:4443/)
如不能成功访问,需要修改appweb.conf配置文件.
PS:
appweb和goahead首席架构师,Mbedthis公司创始人后来在appweb的论坛上.指出了那被我注释掉的三个函数: 
> 
If you build matrixssl with multithreading, then you need to build appweb with multithreading. I would not recommend just commenting out those pthread lines. 
Regarding linking statically, you are right to use a static version of the matrix ssl library. 
Michael
所以,我们只需要在配置appweb时选上--with--multi-thread就可以了解决三个函数未定义的问题了. 
后来Michael又补充道: 
> 
Those APIs are part of pthreads the Linux multithreaded library. They are NOT part of appweb.
http://bbs.chinaunix.net/thread-2082286-1-1.html
