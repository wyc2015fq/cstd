# windows10 vs2015编译 带nginx-rtmp-module 模块的32位nginx - DoubleLi - 博客园






1 下载必要软件
  从 [http://xhmikosr.1f0.de/tools/msys/](http://xhmikosr.1f0.de/tools/msys/)下载msys：[http://xhmikosr.1f0.de/tools/msys/MSYS_MinGW-w64_GCC_610_x86-x64_Full.7z](http://xhmikosr.1f0.de/tools/msys/MSYS_MinGW-w64_GCC_610_x86-x64_Full.7z)。
  把MSYS_MinGW-w64_GCC_610_x86-x64_Full.7z压缩包里面的MSYS解压到C盘根目录。

  安装windwos版本git。[https://git-for-windows.github.io/](https://git-for-windows.github.io/)
[https://github.com/git-for-windows/git/releases/download/v2.9.3.windows.1/Git-2.9.3-64-bit.exe](https://github.com/git-for-windows/git/releases/download/v2.9.3.windows.1/Git-2.9.3-64-bit.exe)

  vc2015下载地址：[https://www.visualstudio.com/downloads/download-visual-studio-vs](https://www.visualstudio.com/downloads/download-visual-studio-vs)

  安装perl
[http://www.activestate.com/activeperl/downloads](http://www.activestate.com/activeperl/downloads)
[http://www.activestate.com/activeperl/downloads/thank-you?dl=http://downloads.activestate.com/ActivePerl/releases/5.24.0.2400/ActivePerl-5.24.0.2400-MSWin32-x86-64int-300560.exe](http://www.activestate.com/activeperl/downloads/thank-you?dl=http://downloads.activestate.com/ActivePerl/releases/5.24.0.2400/ActivePerl-5.24.0.2400-MSWin32-x86-64int-300560.exe)


  安装hg代码管理工具，下载nginx源码。需要设置环境变量PATH 加入 C:\Program Files\Mercurial。
[https://www.mercurial-scm.org/release/windows/Mercurial-3.9-x64.exe](https://www.mercurial-scm.org/release/windows/Mercurial-3.9-x64.exe)

2 下载编译nginx必须的源码包
   cmd.exe
   d:
   cd d:\git\
   hg clone [http://hg.nginx.org/nginx](http://hg.nginx.org/nginx)
   cd nginx
   hg tags 找到最新tag是release-1.11.3
   迁出最新稳定版本代码
   hg co release-1.11.3

![](https://img-blog.csdn.net/20160830152451806?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

   # 修改错误级别为3级，d:\git\nginx\auto\cc\msvc的83行，把 -W4 修改为 -W3
   # 否则，编译时(nmake -f build/Makefile)，会出现如下错误：
   # build/lib/nginx-rtmp-module/ngx_rtmp_core_module.c(611): error C2220: 警告被视为错误 - 没有生成“object”文件
   # build/lib/nginx-rtmp-module/ngx_rtmp_core_module.c(611): warning C4456: “sa”的声明隐藏了上一个本地声明
   # build/lib/nginx-rtmp-module/ngx_rtmp_core_module.c(611): note: 要简化迁移，请考虑暂时对用于生成且不引发警告的编译器版本 使用 /Wv:18 标记
   # build/lib/nginx-rtmp-module/ngx_rtmp_core_module.c(506): note: 参见“sa”的声明
   # NMAKE : fatal error U1077: “"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\BIN\cl.EXE"”: 返回代码“0x2”
   # Stop.
   d:\git\nginx\auto\cc\msvc
   CFLAGS="$CFLAGS -W4" ==> CFLAGS="$CFLAGS -W3"

   mkdir build
   mkdir build\lib

   # 下载 nginx-rtmp-module 代码，checkout稳定版本
   git clone [https://github.com/arut/nginx-rtmp-module.git](https://github.com/arut/nginx-rtmp-module.git)
   cd nginx-rtmp-module
   git tag
   git checkout -b b1.1.9 v1.1.9

   # 打开 msys 环境，下载必要的源码包。
   c:\msys\msys.bat

   在msys窗口输入：
   cd d:/git/nginx/build/lib/

   # 下载 opensll、pcre、zlib到lib目录，并解压
   wget [ftp://ftp.openssl.org/source/old/1.0.1/openssl-1.0.1s.tar.gz](ftp://ftp.openssl.org/source/old/1.0.1/openssl-1.0.1s.tar.gz)
   wget [http://downloads.sourceforge.net/pcre/pcre-8.39.tar.bz2](http://downloads.sourceforge.net/pcre/pcre-8.39.tar.bz2)
   wget [http://zlib.net/zlib-1.2.8.tar.gz](http://zlib.net/zlib-1.2.8.tar.gz)
   tar -xzf openssl-1.0.1s.tar.gz
   tar -jxvf pcre-8.39.tar.bz2
   tar -xzf zlib-1.2.8.tar.gz


3 编译
   cd d:/git/nginx
   # 在msys的/d/git/nginx目录下执行 auto/configure如下：
auto/configure --with-cc=cl --builddir=build --prefix= \
--conf-path=conf/nginx.conf --pid-path=logs/nginx.pid \
--http-log-path=logs/access.log --error-log-path=logs/error.log \
--sbin-path=nginx.exe --http-client-body-temp-path=temp/client_body_temp \
--http-proxy-temp-path=temp/proxy_temp \
--http-fastcgi-temp-path=temp/fastcgi_temp \
--with-cc-opt=-DFD_SETSIZE=1024 --with-pcre=build/lib/pcre-8.39 \
--with-zlib=build/lib/zlib-1.2.8 --with-openssl=build/lib/openssl-1.0.1s \
--with-select_module --with-http_ssl_module --with-ipv6 \
--with-http_sub_module \
--add-module=build/lib/nginx-rtmp-module

在cmd窗口运行：
d:
cd d:/git/nginx
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars32.bat"
nmake -f build/Makefile

运行完成后，cmd 命令行窗口显示：
已完成库搜索
        sed -e "s|%PREFIX%||"  -e "s|%PID_PATH%|/logs/nginx.pid|"  -e "s|%CONF_PATH%|/conf/nginx.conf|"  -e "s|%ERROR_LOG_PATH%|/logs/error.log|"  < docs/man/nginx.8 > build/nginx.8
'sed' 不是内部或外部命令，也不是可运行的程序
或批处理文件。
NMAKE : fatal error U1077: “sed”: 返回代码“0x1”
Stop
这个是因为我们没有正确配置msys 和 mingw32 在windows 上面的路径产生的，不用理会。

在d:\git\nginx\build\下面，已经成功生成nginx.exe

![](https://img-blog.csdn.net/20160830152557987?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



4 配置

cd d:\git\nginx\build
mkdir logs
mkdir temp
# 修改 lib\nginx-rtmp-module\test\nginx.conf 的配置内容，指定 /path/to  为 lib
# 注意系统不要占用tcp 8080端口，和 1935 端口。nginx.conf 中默认使用了8080 作为http端口，1935 作为推流端口。
      # root /path/to/nginx-rtmp-module/test;
            root lib/nginx-rtmp-module/test;

      # root /path/to/nginx-rtmp-module/test/www;
            root lib/nginx-rtmp-module/test/www;



5 测试
# 启动nginx.exe   
nginx.exe -c lib\nginx-rtmp-module\test\nginx.conf

在浏览器地址栏输入一下内容，如果pc上带有可用摄像头，就可以看到录制、观看视频流：
[http://127.0.0.1:8080/index.html](http://127.0.0.1:8080/index.html)
[http://127.0.0.1:8080/rtmp-publisher/player.html](http://127.0.0.1:8080/rtmp-publisher/player.html)
[http://127.0.0.1:8080/rtmp-publisher/publisher.html](http://127.0.0.1:8080/rtmp-publisher/publisher.html)
[http://127.0.0.1:8080/stat/](http://127.0.0.1:8080/stat/)

![](https://img-blog.csdn.net/20160830152644322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)









