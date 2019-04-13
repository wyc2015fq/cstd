
# Linux环境下Mysql的安装教程及安装过程常见问题的解决方法 - Magician的博客 - CSDN博客


2017年10月16日 22:45:36[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：230


最近安装mysql时看到一篇不错的文章
1、下载
下载地址：[http://dev.mysql.com/downloads/mysql/5.6.html\#downloads](http://dev.mysql.com/downloads/mysql/5.6.html#downloads)
下载版本：我这里选择的5.6.33，通用版，linux下64位
也可以直接复制64位的下载地址，通过命令下载：
```python
wget http://dev
```
```python
.mysql
```
```python
.com
```
```python
/get/Downloads/MySQL-
```
```python
5.6
```
```python
/mysql-
```
```python
5.6
```
```python
.33
```
```python
-linux-glibc2
```
```python
.5
```
```python
-x86_64
```
```python
.tar
```
```python
.gz
```
2、解压，复制解压后的mysql目录到系统的本地软件目录
```python
#解压
```
```python
tar -zxvf mysql-
```
```python
5.6
```
```python
.33
```
```python
-linux-glibc2
```
```python
.5
```
```python
-x86_64
```
```python
.tar
```
```python
.gz
```
```python
#复制解压后的mysql目录
```
```python
cp
```
```python
-r mysql-
```
```python
5.6
```
```python
.33
```
```python
-linux-glibc2
```
```python
.5
```
```python
-x86_64 /usr/local/mysql
```
3、添加系统mysql组和mysql用户
```python
groupadd mysql
useradd
```
```python
-r
```
```python
-g
```
```python
mysql
```
```python
-s
```
```python
/bin/
```
```python
false
```
```python
mysql
```
```python
注意：Because
```
```python
the
```
```python
user
```
```python
is
```
```python
required only
```
```python
for
```
```python
ownership purposes,
```
```python
not
```
```python
login purposes,
```
```python
the
```
```python
useradd command uses
```
```python
the
```
```python
-r
```
```python
and
```
```python
-s /bin/
```
```python
false
```
```python
options
```
```python
to
```
```python
create a user
```
```python
that
```
```python
does
```
```python
not
```
```python
have login permissions
```
```python
to
```
```python
your server host. Omit these options
```
```python
if
```
```python
your useradd
```
```python
does
```
```python
not
```
```python
support them.
```
4、进入安装mysql软件目录，修改目录拥有者为mysql用户
```python
cd
```
```python
mysql/
chown -R mysql:mysql ./
```
5、安装数据库，此处可能出现错误。
```python
./scripts/mysql_install_db --user
```
```python
=
```
```python
mysql
```
**FATAL ERROR: please install the following Perl modules before executing scripts/mysql_install_db:Data::Dumper**
```python
#解决方法：
```
```python
yum install -y perl-Data-Dumper
```
6、修改当前目录拥有者为root用户
```python
chown
```
```python
-R root:root ./
```
7、修改当前data目录拥有者为mysql用户
```python
chown
```
```python
-
```
```python
R
```
```python
mysql:mysql
```
```python
data
```
============== 到此数据库安装完毕 =============
8、添加mysql服务开机自启动
添加开机启动，把启动脚本放到开机初始化目录。
```python
复制代码
```
```python
cp
```
```python
support-files/mysql
```
```python
.server
```
```python
/etc/init
```
```python
.d
```
```python
/mysql
```
```python
# 赋予可执行权限
```
```python
chmod +
```
```python
x
```
```python
/etc/init
```
```python
.d
```
```python
/mysql
```
```python
# 添加服务
```
```python
chkconfig --
```
```python
add
```
```python
mysql
```
```python
# 显示服务列表
```
```python
chkconfig --list
```
![这里写图片描述](https://img-blog.csdn.net/20171016223728615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171016223728615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果看到mysql的服务，并且3,4,5都是on的话则成功，如果是off，则执行
[

](https://img-blog.csdn.net/20171016223728615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
chkconfig
```
```python
-
```
```python
-
```
```python
level
```
```python
345
```
```python
mysql
```
```python
on
```
[
](https://img-blog.csdn.net/20171016223728615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)9、启动mysql服务
[

](https://img-blog.csdn.net/20171016223728615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
#创建缺少的文件夹
```
```python
mkdir /
```
```python
var
```
```python
/log/mariadb
service mysql start
```
[
](https://img-blog.csdn.net/20171016223728615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**正常提示信息：Starting MySQL. SUCCESS!**
[
](https://img-blog.csdn.net/20171016223728615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)12、把mysql客户端放到默认路径
[

](https://img-blog.csdn.net/20171016223728615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
ln
```
```python
-s /usr/
```
```python
local
```
```python
/mysql/bin/mysql /usr/
```
```python
local
```
```python
/bin/mysql
```
[
](https://img-blog.csdn.net/20171016223728615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)注意：建议使用软链过去，不要直接包文件复制，便于系统安装多个版本的mysql
=================== 这是分割线 ==================
通过使用 mysql -uroot -p 连接数据库（默认数据库的root用户没有密码，这个需要设置一个密码）。
错误信息：ERROR 2002 (HY000): Can’t connect to local MySQL server through socket ‘/tmp/mysql.sock’ (2)
**解决方法**：打开/etc/my.cnf,看看里面配置的socket位置是什么目录。“socket=/var/lib/mysql/mysql.sock”
路径和“/tmp/mysql.sock”不一致。建立一个软连接：ln -s /var/lib/mysql/mysql.sock /tmp/mysql.sock
到这里任务算是完成了。之后就可以创建数据库用户，然后使用数据库了。
![这里写图片描述](https://img-blog.csdn.net/20171016224102412?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
权限控制
1、去除匿名用户
```python
# 测试匿名用户登录
```
```python
mysql -ux3
```
```python
# 删除匿名用户，使用root用户登录数据库
```
```python
delete
```
```python
from
```
```python
mysql.
```
```python
user
```
```python
where
```
```python
User
```
```python
=
```
```python
''
```
```python
;
```
```python
flush privileges;
```

[
  ](https://img-blog.csdn.net/20171016223728615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)