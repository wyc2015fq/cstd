# windows下编译nginx+nginx_rtmp_modue(vs2013) - DoubleLi - 博客园






阅读官方编译windows版本的方法 [http://nginx.org/en/docs/howto_build_on_win32.html](http://nginx.org/en/docs/howto_build_on_win32.html)

我的环境 Windows 7 Ultimate 64，Visual Studio 2013

nginx_rtmp_modue 在nginx 1.7是无法编译(linux就不行)，所以获取1.6的版本进行编译：

hg clone http://hg.nginx.org/nginx nginx-1.6 -u release-1.6.1 (必须在命令行下运行)



其他几个下载资源

pre [http://ncu.dl.sourceforge.net/project/pcre/pcre/8.35/pcre-8.35.zip](http://ncu.dl.sourceforge.net/project/pcre/pcre/8.35/pcre-8.35.zip)

zlib  [http://cznic.dl.sourceforge.net/project/libpng/zlib/1.2.8/zlib-1.2.8.tar.gz](http://cznic.dl.sourceforge.net/project/libpng/zlib/1.2.8/zlib-1.2.8.tar.gzhttp://)

openssl [http://www.openssl.org/source/openssl-1.0.1e.tar.gz](http://www.openssl.org/source/openssl-1.0.1e.tar.gz)

nginx_rtmp_modue [https://github.com/arut/nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module)



问题与解决

1，添加nginx_rtmp_module编译选项



auto/configure --with-cc=cl --builddir=objs --prefix= \
--conf-path=conf/nginx.conf --pid-path=logs/nginx.pid \
--http-log-path=logs/access.log --error-log-path=logs/error.log \
--sbin-path=nginx.exe --http-client-body-temp-path=temp/client_body_temp \
--http-proxy-temp-path=temp/proxy_temp \
--http-fastcgi-temp-path=temp/fastcgi_temp \
--with-cc-opt=-DFD_SETSIZE=1024 --with-pcre=objs/lib/pcre-8.35 \
--with-zlib=objs/lib/zlib-1.2.8 --with-openssl=objs/lib/openssl-1.0.1e \
--with-select_module --with-http_ssl_module --with-ipv6 \
--add-module=../nginx-module/nginx-rtmp-module



2， auto/configure 命令需要在MSYS命令行下执行；



3， nmake -f objs/Makefile需要在vs 命令行执行，在c:\Program Files (x86)\Common7\Tools\Shortcuts 可以看到：

![](https://img-blog.csdn.net/20140919171707058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2VubnkyNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果你需要编译64位的则选择x64的命令行工具；



3，当执行nmake -f objs/Makefile 时报错

![](https://img-blog.csdn.net/20140919163619196?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2VubnkyNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



从输出日志看，是编译openssl造成的，从错误信息看到： error C2220 “警告被视为错误”，这里只看到了warning C4996一个警告，所以问题必然出在这了，

那就给openssl加上编译选项忽略掉这个警告：--with-openssl-opt=-wd4996；

将configure命令改成：

auto/configure --with-cc=cl --builddir=objs --prefix= \
--conf-path=conf/nginx.conf --pid-path=logs/nginx.pid \
--http-log-path=logs/access.log --error-log-path=logs/error.log \
--sbin-path=nginx.exe --http-client-body-temp-path=temp/client_body_temp \
--http-proxy-temp-path=temp/proxy_temp \
--http-fastcgi-temp-path=temp/fastcgi_temp \
--with-cc-opt=-DFD_SETSIZE=1024 --with-pcre=objs/lib/pcre-8.35 \
--with-zlib=objs/lib/zlib-1.2.8 --with-openssl=objs/lib/openssl-1.0.1e \
--with-select_module --with-http_ssl_module --with-ipv6 \
--with-openssl-opt=-wd4996 \
--add-module=../nginx-module/nginx-rtmp-module

修改后运行；



(以下方法更简单高效)

也可以直接修改nginx目录下的的objs/Makefile文件，找到这一行：

$(MAKE) -f auto/lib/openssl/makefile.msvcOPENSSL="../lib/openssl-0.9.8r"

然后修改为：

$(MAKE) -f auto/lib/openssl/makefile.msvcOPENSSL="../lib/openssl-0.9.8r" OPENSSL_OPT="-wd4996"

修改完后执行nmake -f objs/Makefile， done！









