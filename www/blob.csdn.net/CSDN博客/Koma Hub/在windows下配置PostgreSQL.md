# 在windows下配置PostgreSQL - Koma Hub - CSDN博客
2018年06月11日 15:02:45[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：762

## 安装PostgreSQL
在Windows下的安装就位无脑安装，选择好安装路径就好了，我的安装目录为`D:\PostgreSQL\10`，需要注意一下几点：
- 
安装过程中需要一个数据库的目录，我的为`D:\PostgreSQL\10\data`；
- 
安装后在安装目录总会有一个环境变量的文件`pg_env.bat`，这个文件中的内容为：
@ECHOOFFREM The script sets environment variables helpful for PostgreSQL@SETPATH="D:\PostgreSQL\10\bin";%PATH%@SETPGDATA=D:\PostgreSQL\10\data
@SETPGDATABASE=postgres
@SETPGUSER=postgres
@SETPGPORT=5432@SETPGLOCALEDIR=D:\PostgreSQL\10\share\locale
- 这里我们不用这个环境变量设置文件，新建一个名为`env.vbs`的批处理文件，文件内容如下：
onerrorresumenextsetsysenv=CreateObject("WScript.Shell").Environment("system")'system environment arrayPath=CreateObject("Scripting.FileSystemObject").GetFolder(".").Path'add variablesysenv("PGHOME")="D:\PostgreSQL\10"sysenv("PGHOST")="localhost"sysenv("Path")=sysenv("PGHOME")+"\bin;"+sysenv("Path")sysenv("PGLIB")=sysenv("PGHOME")+"\lib"sysenv("PGDATA")=sysenv("PGHOME")+"\data"wscript.echo"PostgreSQL Success"
- 注意修改里面对应的安装目录，然后双击一下，跳出`PostgreSQL Success`的窗口表明环境变量设置成功。
- ![](https://img-blog.csdn.net/20180611150220306)
## [](https://github.com/rtoax/Knowledge/blob/master/SQL/PostgreSQL/PostgreSQL-Windows10-config.md#%E5%88%9D%E5%A7%8B%E5%8C%96%E6%95%B0%E6%8D%AE%E5%BA%93)初始化数据库
打开Windows的`CMD`，进入目录`D:\PostgreSQL\10\bin`，并在`CMD`下输入：
```
D:\PostgreSQL\10\bin>initdb.exe -D D:\PostgreSQL\10\data -E UTF-8  -U postgres -W
输入新的超级用户口令:
再输入一遍:
initdb: 目录"D:/PostgreSQL/10/data"已存在，但不是空的
如果您想创建一个新的数据库系统, 请删除或清空
目录 "D:/PostgreSQL/10/data" 或者运行带参数的 initdb
而不是 "D:/PostgreSQL/10/data".
D:\PostgreSQL\10\bin>
```
- 由于data是已经创建的，所以会有如上的提示，如果我们改为`data1`，就会有如下的结果：
```
Success. You can now start the database server using:
    pg_ctl -D ^"D^:^\PostgreSQL^\10^\data1^" -l logfile start
```
## [](https://github.com/rtoax/Knowledge/blob/master/SQL/PostgreSQL/PostgreSQL-Windows10-config.md#%E5%90%AF%E5%8A%A8%E5%88%9A%E6%89%8D%E5%88%9B%E5%BB%BA%E7%9A%84data1%E6%95%B0%E6%8D%AE%E5%BA%93)启动刚才创建的`data1`数据库
```
D:\PostgreSQL\10\bin>pg_ctl -D ^"D^:^\PostgreSQL^\10^\data1^" -l logfile start
等待服务器进程启动 .... 完成
服务器进程已经启动
```
## [](https://github.com/rtoax/Knowledge/blob/master/SQL/PostgreSQL/PostgreSQL-Windows10-config.md#%E8%BF%9B%E5%85%A5%E6%95%B0%E6%8D%AE%E5%BA%93)进入数据库
```
D:\PostgreSQL\10\bin>psql -U postgres
psql (10.4)
输入 "help" 来获取帮助信息.
postgres=#
```
## [](https://github.com/rtoax/Knowledge/blob/master/SQL/PostgreSQL/PostgreSQL-Windows10-config.md#%E5%88%9B%E5%BB%BA%E4%B8%80%E4%B8%AAtable)创建一个`TABLE`
postgres=# create table temp(
postgres(# name text,
postgres(# age integer);
CREATE TABLE
postgres=#
## [](https://github.com/rtoax/Knowledge/blob/master/SQL/PostgreSQL/PostgreSQL-Windows10-config.md#%E4%BB%8E%E6%96%87%E4%BB%B6%E4%B8%AD%E8%AF%BB%E5%8F%96sql%E7%A8%8B%E5%BA%8F%E5%B8%B8%E8%A7%81%E4%B8%80%E4%B8%AAtable)从文件中读取SQL程序常见一个`TABLE`
文件`create-table.sql`内容如下：
CREATETABLEitem ( 
    item_id serial , 
    description varchar(64) NOT NULL, 
    cost_price numeric(7,2) , 
    sell_price numeric(7,2) , 
    CONSTRAINT item_pk PRIMARY KEY(item_id) 
);
创建`TABLE`
```
D:\PostgreSQL\10\bin>psql -U postgres -d postgres -f create-table.sql
CREATE TABLE
```
显示刚才创建的表
```
postgres=# table item;
 item_id | description | cost_price | sell_price
---------+-------------+------------+------------
(0 行记录)
```
## [](https://github.com/rtoax/Knowledge/blob/master/SQL/PostgreSQL/PostgreSQL-Windows10-config.md#%E5%88%A0%E9%99%A4%E4%B8%80%E4%B8%AAtable)删除一个`TABLE`
```
postgres=# drop table item;
DROP TABLE
```
