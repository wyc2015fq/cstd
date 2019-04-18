# 如何在CentOS 7上安装配置Oracle ODBC驱动链接Oracle数据库 - alw2009的博客 - CSDN博客
2018年12月27日 00:05:44[traveler_zero](https://me.csdn.net/alw2009)阅读数：142
操作步骤如下
一、安装unixODBC
二、下载Oracle客户端依赖包和Oracle ODBC驱动包
三、安装
四、配置数据源
五、测试链接
**一、安装unixODBC**
`sudo yum install -y unixODBC`
**二、下载Oracle客户端依赖包和Oracle ODBC驱动包**
需要下载[instantclient-basic-linux-12.1.0.2.0.zip](https://www.oracle.com/technetwork/topics/linuxsoft-082809.html) 和  [instantclient-odbc-linux-12.1.0.2.0.zip](https://www.oracle.com/technetwork/topics/linuxsoft-082809.html)
下载地址：[https://www.oracle.com/technetwork/topics/linuxsoft-082809.html](https://www.oracle.com/technetwork/topics/linuxsoft-082809.html)  (32位）
[https://www.oracle.com/technetwork/topics/linuxx86-64soft-092277.html](https://www.oracle.com/technetwork/topics/linuxx86-64soft-092277.html)  （64位）
**三、安装**
```
mkdir /usr/local/oracle
unzip instantclient-basic-linux-12.1.0.2.0.zip -d /usr/local/oracle/
unzip instantclient-odbc-linux-12.1.0.2.0.zip -d /usr/local/oracle/
export LD_LIBRARY_PATH=/usr/local/oracle/instantclient_12_1:$LD_LIBRARY_PATH
echo -en 'export LD_LIBRARY_PATH=/usr/local/oracle/instantclient_12_1:$LD_LIBRARY_PATH' >> ~/.bashrc
```
**四、配置数据源**
配置/etc/odbcinst.ini
```
[ORAC12c]
Description     = Oracle ODBC driver for Oracle 12c
Driver          = /usr/local/oracle/instantclient_12_1/libsqora.so.12.1
Setup           =
FileUsage       =
CPTimeout       =
CPReuse         =
```
 配置/etc/odbc.ini
```
[orac]
Description = ODBC for Oracle
Driver      = ORAC12c
ServerName = 10.10.23.19:1521/orcl11g.us.oracle.com
```
**五、测试链接**
```
$ isql orac odb odb -v
+---------------------------------------+
| Connected!                            |
|                                       |
| sql-statement                         |
| help [tablename]                      |
| quit                                  |
|                                       |
+---------------------------------------+
```
