# 【mysql】Centos7安装mysql8.0删除MariaDB - Big Smile - CSDN博客
2018年05月25日 19:46:49[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1623
所属专栏：[mysql系列](https://blog.csdn.net/column/details/26482.html)
在安装mysql8.0并且使用最新版的centos7的时候出现了不少坑，这个是其中之一。
         由于MySQL在CentOS7中收费了，所以已经不支持MySQL了，取而代之在CentOS7内部集成了mariadb，而安装MySQL的话会和MariaDB的文件冲突，所以需要先卸载掉MariaDB。
使用rpm命令查找mariadb：
`rpm -pa | grep mariadb `
可能会出现：
**mariadb-libs-5.5.56-2.el7.x86_64 、**
然后我们进行删除rpm包：
`rpm -e mariadb-libs-5.5.56-2.el7.x86_64`
可能出现这个包已经被使用的错误，如果判断没有特别的影响的话，直接强制删除：
`rpm -e --nodeps mariadb-libs-5.5.56-2.el7.x86_64`然后我们就删除了这个mariadb数据库            
