# 如何用odb从Oracle数据库导数据到Apache Trafodion数据库 - alw2009的博客 - CSDN博客
2018年12月31日 17:44:46[traveler_zero](https://me.csdn.net/alw2009)阅读数：109
odb是Apache Trafodion自带的一款轻量级、高性能、基于ODBC的面向命令行的ETL工具， 今天来看看如何用odb从Oracle导数据到Trafodion数据库， 包含以下步骤：
**目录**
[一、安装odb](#%E4%B8%80%E3%80%81%E5%AE%89%E8%A3%85odb)
[1.下载安装包](#1.%E4%B8%8B%E8%BD%BD%E5%AE%89%E8%A3%85%E5%8C%85%C2%A0)
[2. 安装unixODBC](#2.%20%E5%AE%89%E8%A3%85unixODBC)
[3. 安装Trafodion ODBC驱动](#3.%20%E5%AE%89%E8%A3%85Trafodion%20ODBC%E9%A9%B1%E5%8A%A8)
[4.安装odb](#4.%E5%AE%89%E8%A3%85odb)
[二、配置数据源](#%E4%BA%8C%E3%80%81%E9%85%8D%E7%BD%AE%E6%95%B0%E6%8D%AE%E6%BA%90)
[2.1 确认配置文件路径](#2.1%20%E7%A1%AE%E8%AE%A4%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E8%B7%AF%E5%BE%84)
[2.2 配置驱动信息](#2.2%20%E9%85%8D%E7%BD%AE%E9%A9%B1%E5%8A%A8%E4%BF%A1%E6%81%AF)
[2.3 配置连接信息](#2.3%20%E9%85%8D%E7%BD%AE%E8%BF%9E%E6%8E%A5%E4%BF%A1%E6%81%AF%C2%A0)
[三、基本使用方法](#%E4%B8%89%E3%80%81%E5%9F%BA%E6%9C%AC%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95)
[3.1 使用帮助](#3.1%20%E4%BD%BF%E7%94%A8%E5%B8%AE%E5%8A%A9)
[3.2 查看可用数据源](#%C2%A03.2%20%E6%9F%A5%E7%9C%8B%E5%8F%AF%E7%94%A8%E6%95%B0%E6%8D%AE%E6%BA%90)
[3.3 分别创建一张oracle表和trafodion表](#%C2%A03.3%20%E5%88%86%E5%88%AB%E5%88%9B%E5%BB%BA%E4%B8%80%E5%BC%A0oracle%E8%A1%A8%E5%92%8Ctrafodion%E8%A1%A8)
[3.4 往oracle数据库插入一些测试数据](#%C2%A03.4%20%E5%BE%80oracle%E6%95%B0%E6%8D%AE%E5%BA%93%E6%8F%92%E5%85%A5%E4%B8%80%E4%BA%9B%E6%B5%8B%E8%AF%95%E6%95%B0%E6%8D%AE)
[3.5 将数据从Oracle导到Trafodion](#%C2%A03.5%20%E5%B0%86%E6%95%B0%E6%8D%AE%E4%BB%8EOracle%E5%AF%BC%E5%88%B0Trafodion)
[3.6 验证结果](#%C2%A03.6%20%E9%AA%8C%E8%AF%81%E7%BB%93%E6%9E%9C)
[四、影响性能的参数](#%E5%9B%9B%E3%80%81%E5%BD%B1%E5%93%8D%E6%80%A7%E8%83%BD%E7%9A%84%E5%8F%82%E6%95%B0)
[4.1 parallel](#4.1%20parallel%C2%A0)
[4.2 rows](#4.2%20rows)
[4.3 loadcmd](#%C2%A04.3%20loadcmd%C2%A0)
[五、编码问题](#%E4%BA%94%E3%80%81%E7%BC%96%E7%A0%81%E9%97%AE%E9%A2%98)
## 一、安装odb
### 1.下载安装包 
有两种方式可以获取odb的安装包：
1.1 官网下载， 下载地址：[http://www.apache.org/dyn/closer.lua/trafodion/apache-trafodion-2.2.0/bin/apache-trafodion_clients-2.2.0-RH6-x86_64.tar.gz](http://www.apache.org/dyn/closer.lua/trafodion/apache-trafodion-2.2.0/bin/apache-trafodion_clients-2.2.0-RH6-x86_64.tar.gz)
1.2. 自己编译， 参考我之前的博客：[在Centos 7.6上编译Apache Trafodion](https://mp.csdn.net/postedit/85305711)
### 2. 安装unixODBC
odb依赖unixODBC, 所以还需要安装unixODBC。 unixODBC有两种安装方法：
1. yum install -y unixODBC， 这种方法安装的unixODBC版本可能比较老旧。
2. 到官网下载源码， 自己编译安装。下载地址：[http://www.unixodbc.org/](http://www.unixodbc.org/)
```
tar xf unixODBC-2.3.7.tar.gz
cd unixODBC-2.3.7
mkdir ~/local/unixODBC
./configure --prefix=$HOME/local/unixODBC --disable-gui --enable-threads --disable-drivers
make && make install
export ODBCHOME=$HOME/local/unixODBC
export ODBCSYSINI=$ODBCHOME/etc
export ODBCINI=$ODBCSYSINI/odbc.ini
export LD_LIBRARY_PATH=$ODBCHOME/lib:$LD_LIBRARY_PATH
export PATH=$ODBCHOME/bin:$PATH
odbcinst -j
```
### 3. 安装Trafodion ODBC驱动
参考了：[https://trafodion.apache.org/docs/client_install/index.html#odbc-linux-install](https://trafodion.apache.org/docs/client_install/index.html#odbc-linux-install)
这里用的是自己编译的驱动， apache-trafodion_clients-2.4.0-RH-x86_64-debug.tar.gz， 里面包含了ODBC的安装包和odb的安装包
```
tar xf apache-trafodion_clients-2.4.0-RH-x86_64-debug.tar.gz
cd clients/
tar xf TRAF_ODBC_Linux_Driver_64.tar.gz
cd PkgTmp/
mkdir -p ~/local/trafodion/odbc
./install.sh
```
根据安装脚本提示填入信息， 例如
```
Do you accept the terms of the license (YES/NO): YES
Thank You....
Proceeding with install
ENTER a directory for library files
OR hit Enter to use the default [/usr/lib64] : /home/sujinpei/local/trafodion/odbc/lib64
ENTER a directory for datasource template file
OR hit Enter to use the default [/etc/odbc] : /home/sujinpei/local/trafodion/odbc
ENTER a directory for sample
OR hit Enter to use the default [/etc/odbc] : /home/sujinpei/local/trafodion/odbc
TRAFODBC driver has successfully been installed.
  * Library libtrafodbc_l64_drvr.so is installed on /home/sujinpei/local/trafodion/odbc/lib64
  * data source template file TRAFDSN has been copied onto /home/sujinpei/local/trafodion/odbc
  * sample file has been copied onto /home/sujinpei/local/trafodion/odbc
```
设置环境变量
`export LD_LIBRARY_PATH=$HOME/local/trafodion/odbc/lib64:$LD_LIBRARY_PATH`
### 4.安装odb
回到clients目录
```
mkdir ~/local/trafodion/odb
tar xf odb64_linux.tar.gz -C ~/local/trafodion/odb
export PATH=$HOME/local/trafodion/odb/bin:$PATH
odb64luo -version
```
## 二、配置数据源
如何安装Oracle ODBC的驱动及配置数据源， 请参考我之前的博客：[如何在CentOS 7上安装配置Oracle ODBC驱动链接Oracle数据库](https://mp.csdn.net/postedit/85270066)
这里演示如何配置Trafodion的数据源
### 2.1 确认配置文件路径
```
odbcinst -j
unixODBC 2.3.7
DRIVERS............: /home/sujinpei/local/unixODBC/etc/odbcinst.ini
SYSTEM DATA SOURCES: /home/sujinpei/local/unixODBC/etc/odbc.ini
FILE DATA SOURCES..: /home/sujinpei/local/unixODBC/etc/ODBCDataSources
USER DATA SOURCES..: /home/sujinpei/local/unixODBC/etc/odbc.ini
SQLULEN Size.......: 8
SQLLEN Size........: 8
SQLSETPOSIROW Size.: 8
```
### 2.2 配置驱动信息
在odbcinst.ini文件里添加如下信息
```
[Trafodion]
Description             = Trafodion ODBC Stand Alone Driver
Driver                  = /home/sujinpei/local/trafodion/odbc/lib64/libtrafodbc_drvr64.so
FileUsage               = 1
UsageCount              = 1
Threading               = 1
```
### 2.3 配置连接信息 
在odbc.ini里添加如下信息
```
[traf]
Description = data source connect to Trafodion
Driver = Trafodion
Catalog = TRAFODION
Schema = SEABASE
DataLang = 0
FetchBufferSize = SYSTEM_DEFAULT
Server = TCP:10.10.23.20:23400
clientCharSet = utf-8
SQL_ATTR_CONNECTION_TIMEOUT = SYSTEM_DEFAULT
SQL_LOGIN_TIMEOUT = SYSTEM_DEFAULT
SQL_QUERY_TIMEOUT = NO_TIMEOUT
ServiceName = TRAFODION_DEFAULT_SERVICE
```
## 三、基本使用方法
### 3.1 使用帮助
`odb64luo -h`
###  3.2 查看可用数据源
`odb64luo -lsdsn`
###  3.3 分别创建一张oracle表和trafodion表
```
odb64luo -d orac -u odb -p odb -x "create table testcopy(id int, name char(20))"
odb64luo -d orac -u odb -p odb -x "create table testcopy(id int, name char(20))"
```
###  3.4 往oracle数据库插入一些测试数据
```
odb64luo -d orac -u odb -p odb -x "insert into testcopy values(1, 'hello')"
odb64luo -d orac -u odb -p odb -x "insert into testcopy values(2, 'world')"
```
###  3.5 将数据从Oracle导到Trafodion
`odb64luo -d orac:traf -u odb:trafodion -p odb:traf123 -cp src=testcopy:tgt=testcopy`
###  3.6 验证结果
`odb64luo -d traf -u trafodion -p traf123 -x "select * from testcopy"`
## 四、影响性能的参数
### 4.1 parallel 
parallel指定用多少组线程进行数据拷贝， parallel不是越大越好。 根据服务器配置合理的设置parallel可以达到最佳的性能。
`odb64luo -d orac:traf -u odb:trafodion -p odb:traf123 -cp src=testcopy:tgt=testcopy:parallel=8`
### 4.2 rows
rows指定odb一次提交多少数据量， 有两种设置方法：
4.2.1 rows=10000, 表示最多10000行提交一次。
`odb64luo -d orac:traf -u odb:trafodion -p odb:traf123 -cp src=testcopy:tgt=testcopy:parallel=8:rows=10000`
 4.2.2 rows=M128, 表示最大128M的数据量提交一次
`odb64luo -d orac:traf -u odb:trafodion -p odb:traf123 -cp src=testcopy:tgt=testcopy:parallel=8:rows=M128`
###  4.3 loadcmd 
loadcmd指定odb用那种加载方式， loadcmd=UL会使用UPSERT USING LOAD语句， 通常这种方式可以得到最佳的性能。
`odb64luo -d orac:traf -u odb:trafodion -p odb:traf123 -cp src=testcopy:tgt=testcopy:rows=M128:parallel=10:loadcmd=UL`
## 五、编码问题
从Oracle导数据到Trafodion数据库， 如果有中文字符，通常会涉及到编码转换。Trafodion推荐用utf-8存储中文， 源数据库的中文数据编码如果不是utf-8编码， 最终需要转换成utf8编码存储到Trafodion数据库。Oracle的ODBC驱动和Trafodion的ODBC驱动都支持编码转换， 都有相应的参数配置， 如果配置不好就会出现导入数据为乱码或是编码转换报错的情况。
控制Oracle ODBC驱动拿到的数据编码的参数是一个环境变量：NLS_LANG
告诉Trafodion ODBC驱动客户端的数据是什么编码的参数是DSN的配置项：ClientCharSet
下图是odb从Oracle导数据导Trafodion的简单数据流图
![](https://img-blog.csdnimg.cn/20181231172603816.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)
 从上图可以看出， odb本身并不会修改数据编码， 乱码问题通常会出现在Trafodion ODBC尝试将ClientCharSet指定编码的数据转换成utf8， 比如 export NLS_LANG="Simplified Chinese_china".ZHS16GBK, 但是在Trafodion的数据源却设置了ClientCharSet=utf-8, 结果就导致了编码错乱。
所以， 解决编码问题的关键就是保证NLS_LANG的编码和ClientCharSet指定的编码一致。
