# mysql和mysqldump出现command not found 问题解决 - sxf_123456的博客 - CSDN博客
2018年05月12日 15:04:59[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：2413
                mysql和mysqldump出现command not found 问题解决
1、查找mysql安装路径
find / -name mysql
通常mysql安装路径在:/usr/local/mysql/bin/mysql
2、mysql:command not found建立软连接
ln -s  /usr/local/mysql/bin/mysql  /usr/bin
3、mysqldump:command not found 建立软连接
ln -s  /usr/local/mysql/bin/mysqldump  /usr/bin            
