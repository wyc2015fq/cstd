# 【初探PHP】安装PHP环境 - CD's Coding - CSDN博客





2016年01月18日 14:26:36[糖果天王](https://me.csdn.net/okcd00)阅读数：393标签：[PHP](https://so.csdn.net/so/search/s.do?q=PHP&t=blog)
个人分类：[开发备忘](https://blog.csdn.net/okcd00/article/category/2611293)









## 0x00 前言

最近因为工作原因要学学PHP，感觉不错

一时半会儿也不知道该写点啥，就记录下怎么配置环境咯……

我才不是来水一篇的呢（骗谁呢23333

听说是世界上最好的语言？（不引战o(∩_∩)o）


## 0x01 安装PHP环境



### 1、安装libmcrypt

1）解压
`tar -zxvf libmcrypt-2.5.7.tar.gz`

2）编译
`./configure --prefix=/home/work/software/libmcrypt-2.5.7`

3）安装

```
make
make install
```


4）环境变量
`export PATH=/home/work/software/libmcrypt-2.5.7/bin:$PATH`
### 2、安装PHP

1）解压

```
tar zvxf php-5.5.30.tar.gz
cd php-5.5.30
```


2）编译
`./configure --prefix=/home/work/software/php-5.5.30 --with-config-file-path=/home/work/software/php-5.5.30/etc --enable-inline-optimization --disable-debug --disable-path --enable-shared --enable-opcache=no --enable-fpm --with-fpm-user=work --with-fpm-group=work --with-mysql=mysqlnd --with-mysqli=mysqlnd --with-pdo-mysql=mysqlnd --with-gettext --enable-mbstring --with-iconv --with-mcrypt=/home/work/software/libmcrypt-2.5.7 --with-mhash --with-openssl --enable-bcmath --enable-soap --with-libxml-dir --enable-pcntl --enable-shmop --enable-sysvmsg --enable-sysvsem --enable-sysvshm --enable-sockets --with-curl --with-zlib --enable-zip --with-bz2 --with-readline --without-sqlite3 --without-pdo-sqlite --with-pear`

3）安装

```
make
make install
```


4）查看php版本
`php -v `

## 0x02 其它



### w3school

http://www.w3school.com.cn/php/












