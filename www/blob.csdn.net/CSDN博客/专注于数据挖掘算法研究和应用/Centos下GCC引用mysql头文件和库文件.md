# Centos下GCC引用mysql头文件和库文件 - 专注于数据挖掘算法研究和应用 - CSDN博客





2012年04月08日 08:42:50[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2600








1.安装mysql-server：

   yum install mysql-server

2.gcc连接mysql的库安装：

   yum -y install  mysql-devel

  安装后，头文件在/usr/include/mysql，库文件在/usr/lib/mysql和/usr/lib下；

3.可以在gcc代码中用mysql的C语言API访问mysql数据库，进行数据库操作；



