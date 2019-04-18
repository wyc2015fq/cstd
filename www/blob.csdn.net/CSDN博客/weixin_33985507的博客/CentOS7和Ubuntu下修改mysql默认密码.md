# CentOS7和Ubuntu下修改mysql默认密码 - weixin_33985507的博客 - CSDN博客
2017年11月06日 12:31:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
# CentOS7
安装mysql后会在`/var/log/mysqld.log`文件中给root生成了一个默认密码
CentOS7使用以下命令查看默认密码：
```
>>> grep 'temporary password' /var/log/mysqld.log
```
输出：
```
2017-10-08T12:18:48.803423Z 1 [Note] A temporary password is generated for root@localhost: ?&)kv%Gqu3jf
```
使用默认密码登陆：
```
>>> mysql -u root -p
```
修改密码：
```
>>> ALTER USER 'root'@'localhost' IDENTIFIED BY 'new_password';
```
mysql5.7默认安装安全检查，如果不需要密码策略，在/etc/my.cnf文件中添加如下配置禁用即可：
```
>>> validate_password = off
```
重新启动mysql服务使配置生效：
```
>>> systemctl restart mysqld
```
# Ubuntu
Ubuntu使用以下命令查看默认密码：
```
>>> sudo grep 'password' /etc/mysql/debian.cnf
```
输出:
```
password = QhiyczW1u5A0JFXw
```
使用如下命令查看mysql用户名:
```
>>> sudo grep 'user' /etc/mysql/debian.cnf
```
输出:
```
user     = debian-sys-maint
```
使用查询出的用户名和密码登陆mysql:
```
mysql -u debian-sys-maint -p
```
然后使用如下命令修改root用户密码:
```
update user set authentication_string=password('new password') where user='root';
```
注意:新安装的mysql5.7中mysql库下的user表已经没有password字段了,改成了authentication_string.
