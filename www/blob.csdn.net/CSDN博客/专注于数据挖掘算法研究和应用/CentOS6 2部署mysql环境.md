# CentOS6.2部署mysql环境 - 专注于数据挖掘算法研究和应用 - CSDN博客





2012年01月05日 11:54:17[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5392标签：[centos																[mysql																[service																[linux																[图形																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)
个人分类：[Linux](https://blog.csdn.net/fjssharpsword/article/category/6480442)





 1.命令安装：yum install mysql-server



 2.自带启动：centos6.2自带安装mysql5.1，root身份登录(mysql -u root -p)没有密码；

    系统->管理->服务，选择mysqld，设置开机自启动并启动服务；



  3.相关命令：

    1)启动服务：service mysqld start 

    2)修改密码：mysql

                        进入mysql命令后执行下面语句：

                        UPDATE mysql.user SET password=PASSWORD('123') WHERE user='root';

                        flush privileges;

   3)导入sql文件命令：source 文件路径；

  4)进入mysql命令：mysql -u 用户名 -p，然后输入密码就可以进入mysql执行环境；

  5)导出数据库或表：mysqldump -u 用户名 -p dataname>dataname.sql；



  4.mysql图形工具workbench安装：

   1）下载地址： [http://www.mysql.com/downloads/workbench/#downloads](http://www.mysql.com/downloads/workbench/#downloads)

   2）选择Oralce linux6，下载mysql-workbench-gpl-5.2.37-1el6.i686.rpm

   3）yum安装：yum install rpm包名

        yum卸载：yum -y remove rpm包名](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)](https://so.csdn.net/so/search/s.do?q=centos&t=blog)




