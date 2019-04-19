# mysql连接不上 mysqlfront连接不上的真正原因 - youfangyuan - CSDN博客
2012年01月31日 21:28:31[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：2729
Step 1
先看看root是不是可以远程连接，如果不行给予权限
grant all privileges on *.* to root[@'%'](mailto:python@' rel=) identified by 'root' with grant option;
Step 2
如果还是连不上就改下面的
vim my.cnf
注释掉
#bind-address  = 127.0.0.1
