# dbus1.0.2、libxm2交叉编译、安装 - xqhrs232的专栏 - CSDN博客
2017年03月19日 10:56:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：398
原文地址::[http://blog.csdn.net/victaminc/article/details/47311503](http://blog.csdn.net/victaminc/article/details/47311503)
相关文章
1、**glib-dbus环境的搭建和相关的操作----[http://blog.csdn.net/shanzhizi/article/details/7711269](http://blog.csdn.net/shanzhizi/article/details/7711269)**
2、[dbus-glib
 安装环境搭建](http://blog.csdn.net/shanzhizi/article/details/7711269)----[http://blog.csdn.net/shanzhizi/article/details/7711269](http://blog.csdn.net/shanzhizi/article/details/7711269)
1、环境
ubuntu12.04
arm-[Linux](http://lib.csdn.net/base/linux)-gcc  4.6.2
dbus1.0.2
2、配置
参考[yanzi1225627](http://my.csdn.net/yanzi1225627)的博客 [http://blog.csdn.net/yanzi1225627/article/details/7855124](http://blog.csdn.net/yanzi1225627/article/details/7855124)
./configure --host=arm-linux --prefix=/usr/local/dbus-1.0.2-arm CC=arm-linux-gcc --cache-file=arm-linux.cache --without-x
2.1 报错：configure: error: cannot run test program while cross compiling
输入：sudo echo ac_cv_have_abstract_sockets=yes>arm-linux.cache
2.2 报错：checking for libxml-2.0 >= 2.6.0... configure: error: No XML library found, check config.log for failed attempts
缺少libxml2的库，那就安装吧,  输入 sudo apt-get install libxml2-dev
再次配置，成功
3、编译
make
出错：
dbus-sysdeps-unix.c:996:18: error: storage size of 'cr' isn't known
打开该文件，加入ucred的定义
**[cpp]**[view
 plain](http://blog.csdn.net/victaminc/article/details/47311503#)[copy](http://blog.csdn.net/victaminc/article/details/47311503#)
- <pre name="code"class="objc">struct ucred  
- {  
- pid_t pid; /* PID of sending process. */
- uid_t uid; /* UID of sending process. */
- gid_t gid; /* GID of sending process. */
- };  
若出错：......./arm-fsl-linux-gnueabi/bin/ld: cannot find -lxml2
可能是因为刚才使用apt-get安装的是ubuntu上的libxml2，虽然配置通过来，但现在需要的是arm版本的dbus，必然是需要链接arm版本的libxml2，所以还是重新交叉编译一个xml2好了。
下载libxml2
http://download.chinaunix[.NET](http://lib.csdn.net/base/dotnet)/download.[PHP](http://lib.csdn.net/base/php)?id=35177&ResourceID=6095
解压，进入目录，同样是configure => make => make install的安装方式
参考configure -help配置安装路径，编译器，安装环境
**[plain]**[view
 plain](http://blog.csdn.net/victaminc/article/details/47311503#)[copy](http://blog.csdn.net/victaminc/article/details/47311503#)
- <span style="font-size:18px;">czw@ubuntu:/usr/local/libxml2-2.7.8-arm/lib$ ./configure --prefix /usr/local/libxml2-2.7.8-arm --host arm-linux CC=arm-linux-gcc  
- </span>  
这个安装好顺利
xml2安装完成，设置环境链接环境变量
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/libxml2-2.7.8-arm/lib
或者再配置时加入CFLAGS=”-L/usr/local/libxml2-2.7.8-arm/lib“
继续make
成功
4、安装
make install 
5、dbus1.2.4的安装
config-loader-libxml.c:26:30: fatal error: libxml/xmlreader.h: No such file or directory
compilation terminated.
./configure --host=arm-linux --prefix=/usr/local/dbus-1.2.4-arm CC=arm-linux-gcc --cache-file=arm-linux.cache --without-x --x-includes=/usr/local/libxml2-2.7.8-arm/include/libxml2
还有错误。。。待解决
