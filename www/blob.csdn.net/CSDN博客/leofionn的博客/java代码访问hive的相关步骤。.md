
# java代码访问hive的相关步骤。 - leofionn的博客 - CSDN博客


2018年01月20日 15:40:38[leofionn](https://me.csdn.net/qq_36142114)阅读数：1077


## 相关说明
使用java代码访问hive的时候需要hive的守护进程——hiveserver2
也可以使用另外一个命令行客户端beeline进行对hive的访问
## 守护进程模式启动和beeline简单使用
```python
hiveserver2 &
```
```python
# 启动，并挂到后台
```
```python
netstat -tupln|grep
```
```python
10000
```
```python
# 默认的端口是10000
```
```python
beeline
!connect jdbc:hive2:
```
```python
//localhost:10000 abc
```
```python
# 交互式连接到hiveserver2
```
```python
# 默认端口是10000
```
```python
# 指定连接的OS用户是abc
```
```python
# 需要输入密码
```
```python
# 经测试，使用一个不存在的用户和密码也是可以登录的
```
```python
# 应该是个BUG
```
```python
show databases;
use test1;
show tables;
select * from emp;
```
```python
# Ctrl+d 退出
```
```python
beeline -u jdbc:hive2:
```
```python
//localhost:10000/test1
```
```python
# 直接连接到对应的库
```
```python
select * from emp;
```
```python
# Ctrl+d 退出
```
## 使用JDBC访问HiveServer2
参照《Win7 IDEA 开发环境搭建部署》和《hive UDF 测试样例开发》配置idea开发环境和pom配置文件。
pom.xml添加mysql的驱动依赖：
```python
<!-- 添加hive的jdbc驱动包 -->
```
```python
<
```
```python
dependency
```
```python
>
```
```python
<
```
```python
groupId
```
```python
>
```
```python
org.apache.hive
```
```python
</
```
```python
groupId
```
```python
>
```
```python
<
```
```python
artifactId
```
```python
>
```
```python
hive-jdbc
```
```python
</
```
```python
artifactId
```
```python
>
```
```python
<
```
```python
version
```
```python
>
```
```python
${hive.version}
```
```python
</
```
```python
version
```
```python
>
```
```python
</
```
```python
dependency
```
```python
>
```
根据官方样例代码改造测试，官方样例代码地址如下：
[https://cwiki.apache.org/confluence/display/Hive/HiveClient\#HiveClient-JDBC](https://cwiki.apache.org/confluence/display/Hive/HiveClient#HiveClient-JDBC)
创建jdbc包，创建JdbcConnectHive类，测试代码如下：
```python
package com.vincent.jdbc;
```
```python
/*
    通过JDBC的方式访问hive(HiveServer2)
 */
```
```python
import java.sql.*;
```
```python
public
```
```python
class
```
```python
JdbcConnectHive {
```
```python
private
```
```python
static
```
```python
String driverName =
```
```python
"org.apache.hive.jdbc.HiveDriver"
```
```python
;
```
```python
// 官网驱动包名字：org.apache.hadoop.hive.jdbc.HiveDriver 有问题，改掉
```
```python
public
```
```python
static
```
```python
void
```
```python
main
```
```python
(String[] args) throws SQLException {
```
```python
try
```
```python
{
            Class.forName(driverName);
        }
```
```python
catch
```
```python
(ClassNotFoundException e) {
```
```python
// TODO Auto-generated catch block
```
```python
e.printStackTrace();
            System.exit(
```
```python
1
```
```python
);
        }
```
```python
// 没有驱动就退出
```
```python
Connection con = DriverManager.getConnection(
```
```python
"jdbc:hive2://192.168.1.10:10000/default"
```
```python
,
```
```python
"hadoop"
```
```python
,
```
```python
"hadoop"
```
```python
);
```
```python
// 设置连接串对象，连接到的HiveServer2地址
```
```python
// 官网链接串写的jdbc:hive://... 应该是jdbc:hive2//...
```
```python
Statement stmt = con.createStatement();
```
```python
// 根据连接串对象，创建会话
```
```python
String sql =
```
```python
"show databases"
```
```python
;
        System.
```
```python
out
```
```python
.println(
```
```python
"Running: "
```
```python
+ sql);
        ResultSet res = stmt.executeQuery(sql);
```
```python
if
```
```python
(res.next()) {
```
```python
while
```
```python
(res.next()) {
                System.
```
```python
out
```
```python
.println(res.getString(
```
```python
1
```
```python
));
            }
        }
```
```python
// 拼sql执行字符串：show databases;
```
```python
// 使用executeQuery()函数执行DML查询
```
```python
// 将查询结果赋值给ResultSet类型的res对象
```
```python
// 判断该对象是否为空，并打印不为空时候的结果
```
```python
// default库默认不显示
```
```python
sql =
```
```python
"use default"
```
```python
;
        System.
```
```python
out
```
```python
.println(
```
```python
"\nRunning: "
```
```python
+ sql);
        stmt.execute(sql);
        sql =
```
```python
"show tables"
```
```python
;
        System.
```
```python
out
```
```python
.println(
```
```python
"Running: "
```
```python
+ sql);
        res = stmt.executeQuery(sql);
```
```python
if
```
```python
(res.next()) {
```
```python
while
```
```python
(res.next()) {
                System.
```
```python
out
```
```python
.println(res.getString(
```
```python
1
```
```python
));
            }
        }
```
```python
// 执行DDL等没有数据返回的SQL语句时使用函数execute()
```
```python
// 执行有数据返回的SQL语句时使用executeQuery()，并将返回结果赋值给ResultSet类型的一个对象
```
```python
}
}
```
执行结果如下：
![这里写图片描述](https://img-blog.csdn.net/20180114122216707?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvendqenFxYg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


来自@若泽大数据

