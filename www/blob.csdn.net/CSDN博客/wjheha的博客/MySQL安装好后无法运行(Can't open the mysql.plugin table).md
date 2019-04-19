# MySQL安装好后无法运行(Can't open the mysql.plugin table) - wjheha的博客 - CSDN博客
2018年09月13日 14:19:55[wjheha](https://me.csdn.net/wjheha)阅读数：361
原文链接：[https://blog.csdn.net/xinwenfei/article/details/72460894?utm_source=itdadao&utm_medium=referral](https://blog.csdn.net/xinwenfei/article/details/72460894?utm_source=itdadao&utm_medium=referral)
## 记录一个小问题
MySQL安装好后不知怎么就遇到了这个问题，这个问题的解决办法是使用如下命令
`mysql_install_db --user=mysql --ldata=/var/lib/mysql/`
但是问题又来了：
`FATAL ERROR: Could not find my_print_defaults`
这个问题的解决办法是根据提示再加一个参数，指定basedir：
```
mysql_install_db --user=mysql --basedir=/usr/ --ldata=/var/lib/mysql/
```
这句执行完之后，再启动mysql服务就正常了。
