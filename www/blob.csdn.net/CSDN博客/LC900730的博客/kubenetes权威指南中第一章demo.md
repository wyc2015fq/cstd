# kubenetes权威指南中第一章demo - LC900730的博客 - CSDN博客
2018年10月10日 17:01:17[lc900730](https://me.csdn.net/LC900730)阅读数：152
1.关于安装kubenetes，很多坑。
2.yaml文件格式问题
3.30001端口问题
先开防火墙，打开30001端口，在关闭防火墙
systemctl start firewalld
firewall-cmd --zone=public --add-port=30001/tcp --permanent
systemctl stop firewalld
systemctl disable firewalld
4.com.mysql.jdbc.exceptions.jdbc4.MySQLNonTransientConnectionException: Could not create connection to database server. 错误应该是tomcat无法获取到mysql的地址，所以无法连接到mysql。因此先注释掉！ # - name: MYSQL_SERVICE_HOST # value: ‘mysql’
书籍中mysql是latest，但是当时是15-16年左右，原来jdbc驱动可用，但是现在yaml文件中没有版本号，默认依然是latest，但是现在的jdbc驱动不能连接latest版本的mysql，故需要更改yaml文件,添加版本(我使用了5.6)
![在这里插入图片描述](https://img-blog.csdn.net/20181010165917298?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0xDOTAwNzMw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
终于出现![在这里插入图片描述](https://img-blog.csdn.net/20181010170025527?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0xDOTAwNzMw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
