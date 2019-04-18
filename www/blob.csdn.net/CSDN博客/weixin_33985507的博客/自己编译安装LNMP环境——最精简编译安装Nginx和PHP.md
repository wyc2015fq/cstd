# 自己编译安装LNMP环境——最精简编译安装Nginx和PHP - weixin_33985507的博客 - CSDN博客
2017年07月05日 09:38:44[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
github上源码地址:[链接](https://github.com/salamander-mh/LnmpShell)
## 编译安装Nginx
编译nginx需要**pcre**, **zlib**, **openssl**库支持(需要用到它们的头)
```
#!/bin/sh
# linux上nginx，php，mysql集成环境
# Author salamander
set -e # "Exit immediately if a simple command exits with a non-zero status."
basepath=$(cd `dirname $0`; pwd)
# Check if user is root
if [ $(id -u) != "0" ]; then
    echo "Error: You must be root to run this script, please use root to install lnmp"
    exit 1
fi
    
yum install -y gcc gcc-c++
tar -zxf nginx-1.10.1.tar.gz 
tar -zxf pcre-8.38.tar.gz
tar -zxf zlib-1.2.11.tar.gz
tar -zxf openssl-1.1.0e.tar.gz
# nginx安装 注意 --with-pcre=  --with-zlib --with-openssl  指的是源码路径
cd ./nginx-1.10.1
./configure --prefix=/usr/local/nginx-1.10.1 --with-pcre=./../pcre-8.38  --with-zlib=./../zlib-1.2.11  --with-openssl=./../openssl-1.1.0e
make
make install
echo 'Nginx installed successfully!'
```
**注意** 如果你不指定路径,譬如--with-pcre这样,那说明用的是默认路径(需要yum install -y pcre-devel)
这么做只是想用比较新的库而已
安装成功后,进入/usr/local/nginx-1.10.1/sbin目录,终端输入./nginx,然后在浏览器输入localhost可以看到nginx的欢迎界面.
## 编译安装PHP
安装PHP需要注意的是**mcrypt**库的安装,因为yum源已经没有这个库了
```
#!/bin/sh
# linux上nginx，php，mysql集成环境
# Author salamander
set -e # "Exit immediately if a simple command exits with a non-zero status."
basepath=$(cd `dirname $0`; pwd)
# Check if user is root
if [ $(id -u) != "0" ]; then
    echo "Error: You must be root to run this script, please use root to install lnmp"
    exit 1
fi
yum -y install libxml2 libxml2-devel openssl openssl-devel curl-devel libjpeg-devel libpng-devel freetype-devel
cd $basepath
# 安装libmcrypt库
tar zxvf libmcrypt-2.5.8.tar.gz
cd libmcrypt-2.5.8
./configure
make
make install
cd $basepath
# 安装mhash库
tar zxvf mhash-0.9.9.9.tar.gz
cd mhash-0.9.9.9
./configure
make
make install
cd $basepath
# 安装mcrypt库
tar -zxvf mcrypt-2.6.8.tar.gz
cd mcrypt-2.6.8
export LD_LIBRARY_PATH=/usr/local/lib
./configure
make
make install
cd $basepath
tar -zxvf php-7.0.16.tar.gz
cd ./php-7.0.16
./configure \
--prefix=/usr/local/php7 \
--with-config-file-path=/usr/local/php7/etc \
--enable-fpm \
--with-fpm-user=www \
--with-fpm-group=www \
--with-mysqli \
--with-pdo-mysql \
--with-libdir=lib64 \
--with-iconv-dir \
--with-freetype-dir \
--with-jpeg-dir \
--with-png-dir \
--with-zlib \
--with-libxml-dir=/usr \
--enable-xml \
--disable-rpath  \
--enable-bcmath \
--enable-shmop \
--enable-sysvsem \
--enable-inline-optimization \
--with-curl \
--enable-mbregex \
--enable-mbstring \
--with-mcrypt \
--enable-ftp \
--with-gd \
--enable-gd-native-ttf \
--with-openssl \
--with-mhash \
--enable-pcntl \
--enable-sockets \
--with-xmlrpc \
--enable-zip \
--enable-soap \
--without-pear \
--with-gettext \
--disable-fileinfo \
--enable-maintainer-zts
make 
make install
# create a link to php
ln -s /usr/local/php7/bin/php /usr/local/bin/
echo 'PHP installed successfully!'
```
这里编译安装是PHP7,可以改成其他php版本
可以把上面的shell合在一起:[集成包](http://ongd1spyv.bkt.clouddn.com/LNMP.7z)
下一篇写编译安装**mysql**
