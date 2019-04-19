# PHP编译安装 - walkabc - 博客园
# [PHP编译安装](https://www.cnblogs.com/hitfire/articles/6368664.html)
　　1.去php官方网站下载最新版本的源码包
　　http://php.net/downloads.php
　　2.下载之后执行下面的代码
```bash;gutter
./configure --prefix=/app/tool/php --with-pcre-regex --with-mysqli --with-pcre-dir --with-openssl-dir --with-openssl --with-zlib-dir --with-zlib --with-curl --with-mcrypt --with-bz2 --with-gd --with-pdo-mysql --with-freetype-dir --with-jpeg-dir --with-png-dir --with-gettext --with-iconv-dir --with-kerberos --with-libxml-dir --with-mhash --with-zlib-dir --without-pdo-sqlite --with-pear --with-xmlrpc --with-xsl --enable-opcache --enable-soap --enable-sockets --enable-sysvsem --enable-xml --enable-bcmath --enable-libxml --enable-inline-optimization --enable-mbregex --enable-zip --enable-ftp --enable-fpm --enable-mbstring --enable-gd-native-ttf --enable-calendar --enable-fpm --enable-pcntl --enable-shmop --enable-exif --enable-pcntl --enable-wddx --enable-intl --enable-libgcc
```
　　在执行这条命令时，会检查PHP所需要的依赖，根据提示安装依赖
　　3.configure之后，执行make
　　4.make install
　　5.把php.ini拷贝到/app/tool/php/etc目录中
　　6.执行php-fpm -c /app/tool/php/etc/php.ini -y /app/tool/php/etc/php-fpm.conf

