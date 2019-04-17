# Nginx+FastCGI运行原理 - DoubleLi - 博客园






Nginx不支持对外部程序的直接调用或者解析，所有的外部程序（包括[PHP](http://lib.csdn.net/base/php)）必须通过FastCGI接口来调用。FastCGI接口在[Linux](http://lib.csdn.net/base/linux)下是socket（这个socket可以是文件socket，也可以是ip socket）。为了调用CGI程序，还需要一个FastCGI的wrapper（wrapper可以理解为用于启动另一个程序的程序），这个wrapper绑定在某个固定socket上，如端口或者文件socket。当Nginx将CGI请求发送给这个socket的时候，通过FastCGI接口，wrapper接收到请求，然后派生出一个新的线程，这个线程调用解释器或者外部程序处理脚本并读取返回数据；接着，wrapper再将返回的数据通过FastCGI接口，沿着固定的socket传递给Nginx；最后，Nginx将返回的数据发送给客户端。这就是Nginx+FastCGI的整个运作过程，如图1-3所示。

![](http://img.blog.csdn.net/20160411211901590?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



前面介绍过，FastCGI接口方式在脚本解析服务器上启动一个或者多个守护进程对动态脚本进行解析，这些进程就是FastCGI进程管理器，或者称为FastCGI引擎。 spawn-fcgi与[php](http://lib.csdn.net/base/php)-FPM就是支持PHP的两个FastCGI进程管理器。

下面简单介绍spawn-fcgi与PHP-FPM的异同。

spawn-fcgi是HTTP服务器lighttpd的一部分，目前已经独立成为一个项目，一般与lighttpd配合使用来支持PHP。但是ligttpd的spwan-fcgi在高并发访问的时候，会出现内存泄漏甚至自动重启FastCGI的问题。

Nginx是个轻量级的HTTP server，必须借助第三方的FastCGI处理器才可以对PHP进行解析，因此Nginx+spawn-fcgi的组合也可以实现对PHP的解析，这里不过多讲述。

PHP-FPM也是一个第三方的FastCGI进程管理器，它是作为PHP的一个补丁来开发的，在安装的时候也需要和PHP源码一起编译，也就是说PHP-FPM被编译到PHP内核中，因此在处理性能方面更加优秀。同时PHP-FPM在处理高并发方面也比spawn-fcgi引擎好很多，因此，推荐使用Nginx+PHP/PHP-FPM这个组合对PHP进行解析。

FastCGI 的主要优点是把动态语言和HTTP Server分离开来，所以Nginx与PHP/PHP-FPM经常被部署在不同的服务器上，以分担前端Nginx服务器的压力，使Nginx专一处理静态请求和转发动态请求，而PHP/PHP-FPM服务器专一解析PHP动态请求。









