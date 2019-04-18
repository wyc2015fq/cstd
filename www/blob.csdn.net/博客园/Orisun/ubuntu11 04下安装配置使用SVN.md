# ubuntu11.04下安装配置使用SVN - Orisun - 博客园







# [ubuntu11.04下安装配置使用SVN](https://www.cnblogs.com/zhangchaoyang/articles/2084740.html)





其实安装apache时./configure命令是有很多参数可以指定的，如果没有显式指定，安装守SVN后你发现在apache的modules目录下并没有mod_dav_svn.so和mod_authz_svn.so。

大多数情况是我们安装apache的时候没有考虑到后面还会安装SVN，自然没有指定 ./configure命令的参数，这时候需要自己生成

mod_dav_svn.so和mod_authz_svn.so，放到apache/modules目录下面：


下载subversion-deps-1.6.17.tar.gz 和subversion-1.6.17.tar.gz，解压合并到一个目录下面



 ./configure --with-apxs=/usr/local/apache/bin/apxs



make

然后你在subversion-1.6.17/subversion/mod_authz_svn/.lib/下发现了

mod_authz_svn.so


在subversion-1.6.17/subversion/mod_dav_svn/.lib/下发现了 mod_dav_svn.so

开启svn：svnserve -d

开启apache：/usr/local/apache/bin/apachectl start

![](https://pic002.cnblogs.com/images/2011/103496/2011070417403994.png)

向仓库中提交代码时如果出现

Can’t open file ‘XXXXXXXX\db\txn-current-lock’: 拒绝访问。

修改权限：

#chmod –R o+rw /opt/svnroot





顺便说一下locate命令是在整个磁盘上查找文件，但它不会搜索隐藏（即以.开头）的文件和文件夹；find在指定目录下查找文件，以-name选项来指定要查找的文件，如find  /home/orisun  -name 

mod_dav_svn.so。find命令可以把隐藏的命令也找出来。










1.安装OpenSSL

apt-get install openssl libssl-dev

2.安装apache2


下载httpd-2.2.8.tar.gz

./configure --enable-so --enable-proxy --enable-ssl --enable-vhost-alias --enable-modes-shared=most --enable-dav --enable-maintainer-mode --with-mpm=worker


说明：其中--enable-dav 和--enable-maintainer-mode是为了能在apache下集成subversion才加进去的编译参数。如果

--enable-ssl有问题就把这一项去掉。

make

sudo make install

3.安装BerkeleyDB

解压后进入build_unix

../dist/configure

make

sudo make install

4.安装subversion

下载subversion-deps-1.6.17.tar.gz
和subversion-1.6.17.tar.gz，解压合并到一个目录下面



 ./configure --with-apxs=/usr/local/apache2/bin/apxs

 --with-berkeley-db=/usr/local/BerkeleyDB.5.2



make

sudo make install








1.下载和apr-1.4.5和apr-util-1.3.121


2.安装apr

./configure


make

sudo make install

说明： ./configure时若没有指定--prefix默认的安装路径就是/usr/local/


3.安装apr-util


./configure --with-apr=/usr/local/apr


make

sudo make install















