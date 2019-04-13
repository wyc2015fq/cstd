
# 执行Hive语句报错：FAILED: Error in metadata: javax.jdo.JDOFatalDataStoreException: Access denied for user ' - Magician的博客 - CSDN博客


2017年10月19日 22:56:31[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：516


安装个Hive真不省心，各种问题。最近安装好Hive后执行Hive语句时碰到这样的错误：
```python
hive> show databases;
FAILED: Error
```
```python
in
```
```python
metadata: javax.jdo.JDOFatalDataStoreException: Access denied
```
```python
for
```
```python
user
```
```python
'root'
```
```python
@
```
```python
'localhost'
```
```python
(
```
```python
using
```
```python
password: YES)
NestedThrowables:
java.sql.SQLException: Access denied
```
```python
for
```
```python
user
```
```python
'root'
```
```python
@
```
```python
'localhost'
```
```python
(
```
```python
using
```
```python
password: YES)
FAILED: Execution Error,
```
```python
return
```
```python
code
```
```python
1
```
```python
from
```
```python
org.apache.hadoop.hive.ql.exec.DDLTask
```
从字面看是root用户访问权限被拒绝。那么也就是以下两个原因造成的：
1、root用户本身的设置问题。
2、mysql权限访问问题。
首先排除原因2：看是否由于Mysql未对root用户设置访问权限造成的。
在Mysql进行授权：
打开mysql执行下面语句（第一个单引号中的root代表的是mysql的root用户，第三个单引号中是要输入的root用户的登录密码<我这里是初始登录密码为空>。）
```python
mysql
```
```python
>
```
```python
GRANT
```
```python
ALL
```
```python
PRIVILEGES
```
```python
ON
```
```python
*
```
```python
.
```
```python
*
```
```python
TO
```
```python
'root'
```
```python
@
```
```python
'localhost'
```
```python
IDENTIFIED
```
```python
BY
```
```python
''
```
```python
WITH
```
```python
GRANT OPTION;
```
之后再执行以下语句让授权生效：
```python
FLUSH PRIVILEGES;
```
但是发现问题依旧存在。
接着再排除原因1：看是够是root用户的设置问题。
经过翻墙查找，总算发现了问题的原因：因为我的hive.site文件在配置访问mysql的用户和密码时出现了问题。
![这里写图片描述](https://img-blog.csdn.net/20171019224029229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171019224029229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因为刚装的mysql，所以root的登录密码之前为空（这里有一个疑问：第一次配置的时候，我是把hive.site文件中root的密码设为空，而当时由于刚装mysql，其用户就是root，密码本身也就是空，但是为什么Hive仍然无法访问Mysql？仍会报以上的错误？）。
[
](https://img-blog.csdn.net/20171019224029229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)后来把root的密码更改为root之后，修改相关的hive.site文件后，hive总算可以正常的执行语句了。
关于如何更改mysql中用户的密码，这里也简单说一下：
1、先启动mysql
2、然后输入：mysql -u root -p 回车，以空密码登录。
![这里写图片描述](https://img-blog.csdn.net/20171019225031673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3、选择数据库：use mysql;
修改密码：UPDATE user SET password=PASSWORD(‘123456’) WHERE user=’root’;
刷新：flush privileges;
退出：exit;
这样就完成了。
![这里写图片描述](https://img-blog.csdn.net/20171019225216300?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

