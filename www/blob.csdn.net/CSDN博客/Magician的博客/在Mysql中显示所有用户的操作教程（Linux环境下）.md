
# 在Mysql中显示所有用户的操作教程（Linux环境下） - Magician的博客 - CSDN博客


2017年10月19日 23:00:07[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：704


1.登录数据库
首先，你需要使用如下命令登录到数据库，注意，必须是root用户哦~
```python
mysql
```
```python
-u
```
```python
root
```
```python
-p
```
2.查询用户表
在Mysql中其实有一个内置且名为mysql的数据库，这个数据库中存储的是Mysql的一些数据，比如用户、权限信息、存储过程等，所以呢，我们可以通过如下简单的查询语句来显示所有的用户呢。
SELECT User, Host, Password FROM mysql.user;
你将会看到如下这样的信息：
```python
+------------------+--------------+--------------+
```
```python
| user             | host         | password     |
```
```python
+------------------+--------------+--------------+
```
```python
| root             | localhost    |
```
```python
37
```
```python
as%#
```
```python
8123
```
```python
fs |
| debian-test-user
```
```python
| localhost    |
```
```python
HmBEqPjC5Y
```
```python
|
```
```python
| johnsm           | localhost    |
```
```python
|
```
```python
| brian            | localhost    |
```
```python
|
```
```python
| root             |
```
```python
111.111
```
```python
.111
```
```python
.1
```
```python
|              |
```
```python
| guest            |
```
```python
%            |              |
```
```python
| linuxprobe       |
```
```python
10.11
```
```python
.12
```
```python
.13
```
```python
|
```
```python
RFsgY6aiVg
```
```python
|
+------------------+--------------+--------------+
```
```python
7
```
```python
rows in set (
```
```python
0.01
```
```python
sec)
```
如果你想增加或减少一些列的显示，那么你只需要编辑这条sql语句即可，比如你只需要显示用户的用户名，那么你就可以这样使用SELECT User FROM mysql.user;，就是这样了，很简单嘛，就用这种方法就可以获得所有用户了呢，快去试试吧。
3.显示所有的用户（不重复）
熟悉Mysql的朋友们都知道DISTINCT这个修饰的作用吧，对了，就是去除重复的数据，所以我们可以使用如下命令显示所有你的Mysql的用户而忽略那些仅仅是主机名不同的用户。
SELECT DISTINCT User FROM mysql.user;
这条命令的输出就像下面显示的这样：
```python
+------------------+
```
```python
| user             | 
+------------------+
```
```python
| root             | 
| debian-test-user | 
| johnsm           | 
| brian            | 
| guest            |
```
```python
| linuxprobe       | 
+------------------+
```
```python
6 rows in set (0.01 sec)
```
本文转载自：[http://www.linuxprobe.com/mysql-show-all-users.html](http://www.linuxprobe.com/mysql-show-all-users.html)

