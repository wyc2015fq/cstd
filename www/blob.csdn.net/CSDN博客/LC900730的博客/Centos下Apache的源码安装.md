# Centos下Apache的源码安装 - LC900730的博客 - CSDN博客
2016年12月01日 19:01:20[lc900730](https://me.csdn.net/LC900730)阅读数：211
版本：httpd2.4.23
依赖包：apr-1.5.2.tar.gz    apr-util-1.5.4.tar.gz  gcc pcre   
①先安装好GCC，简单 ,yum install最合适
②tar     httpd-2.4.23      apr-1.5.2.tar.gz    apr-util-1.5.4.tar.gz 解压
③ apr以及apr-util这两个文件夹移动到httpd-2.4.23的srclib目录
mv apr-1.5.2    apr         
mv apr-util-1.5.4    apr-util
mv apr /usr/local/httpd-2.4.23/srclib/
mv apr-util /usr/local/httpd-2.4.23/srclib/
④解压pcre，进入解压后的目录，
./configure --prefix=/usr/local/pcre
make
make install
⑤cd  /usr/local/httpd-2.4.23
./configure --prefix=/opt/apache2  --with-pcre=/usr/local/pcre
make
make install
在初期安装出现问题：buckets/apr_brigade.c: In function 'apr_brigade_partition':
          buckets/apr_brigade.c:140: error: 'APR_SIZE_MAX' undeclared (first use in this function)
          buckets/apr_brigade.c:140: error: (Each undeclared identifier is reported only once
          buckets/apr_brigade.c:140: error: for each function it appears in.)
          make[1]: *** [buckets/apr_brigade.lo] Error 1
          make: *** [all-recursive] Error 1
解决方案 ：apr以及apr-util版本问题，apr-util是1.5版本的，apr是1.2版本的，删除apr-1.2后换用1.5版本，一次性安装成功。
