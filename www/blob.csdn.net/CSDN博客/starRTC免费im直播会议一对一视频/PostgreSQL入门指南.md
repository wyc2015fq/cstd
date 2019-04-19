# PostgreSQL入门指南 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年04月12日 11:30:54[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：86
默认端口：5432
创建用户与数据库
PostgreSQL初次安装后，默认会生成一个名为postgres的数据库和一个名为postgres的数据库用户。这里需要注意的是，同时还生成了一个名为postgres的Linux系统用户。
下面，我们使用postgres用户，来生成其他用户和新数据库。
首先，用root新建一个Linux新用户，可以取你想要的名字，这里为dbuser。
adduser dbuser
然后，切换到postgres用户。
su - postgres
下一步，使用psql命令登录PostgreSQL控制台。
psql
如果一切正常，系统提示符会变为"postgres=#"，表示这时已经进入了数据库控制台。
以下的命令都在控制台内完成。
第一件事是使用\password命令，为postgres用户设置一个密码。
\password postgres
第二件事是创建数据库用户dbuser（刚才创建的是Linux系统用户），并设置密码。
CREATE USER dbuser WITH PASSWORD 'password';
第三件事是创建用户数据库，这里为exampledb，并指定所有者为dbuser。
CREATE DATABASE exampledb OWNER dbuser;
第四件事是将exampledb数据库的所有权限都赋予dbuser，否则dbuser只能登录控制台，没有任何数据库操作权限。
GRANT ALL PRIVILEGES ON DATABASE exampledb to dbuser;
最后，使用\q命令退出控制台（也可以直接ctrl+D）。
\q
## 登录数据库
添加新用户和新数据库以后，就要以新用户的名义登录数据库，这时使用的是psql命令。
psql -U dbuser -d exampledb -h 127.0.0.1 -p 5432
上面命令的参数含义如下：-U指定用户，-d指定数据库，-h指定服务器，-p指定端口。
输入上面命令以后，系统会提示输入dbuser用户的密码。输入正确，就可以登录控制台了。
- 报错：
psql: 致命错误:  用户 "dbuser" Ident 认证失败
解决：
vim  /var/lib/pgsql/9.3/data/pg_hba.conf
vim /var/lib/pgsql/9.6/data/pg_hba.conf 
修改如下：
service postgresql-9.3 restart
systemctl restart postgresql-9.6 
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=PostgreSQL%E5%85%A5%E9%97%A8%E6%8C%87%E5%8D%97&action=edit&section=3)]控制台命令
除了前面已经用到的\password命令（设置密码）和\q命令（退出）以外，控制台还提供一系列其他命令。
- \h：查看SQL命令的解释，比如\h select。
- \?：查看psql命令列表。
- \l：列出所有数据库。
- \c [database_name]：连接其他数据库。
- \d：列出当前数据库的所有表格。
- \d [table_name]：列出某一张表格的结构。
- \du：列出所有用户。
- \e：打开文本编辑器。
- \conninfo：列出当前数据库和连接的信息。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=PostgreSQL%E5%85%A5%E9%97%A8%E6%8C%87%E5%8D%97&action=edit&section=4)]数据库操作
# 创建新表 
**CREATE ****TABLE** user_tbl(name **VARCHAR**(20), signup_date **DATE**);
# 插入数据 
**INSERT ****INTO** user_tbl(name, signup_date)**VALUES**('张三','2013-12-22');
# 选择记录 
**SELECT *** **FROM** user_tbl;
# 更新数据 
**UPDATE** user_tbl **SET** name ='李四' **WHERE** name ='张三';
# 删除记录 
**DELETE ****FROM** user_tbl **WHERE** name ='李四' ;
# 添加栏位 **ALTER ****TABLE** user_tbl **ADD** email **VARCHAR**(40);
# 更新结构 
**ALTER ****TABLE** user_tbl **ALTER ****COLUMN** signup_date **SET ****NOT ****NULL**;
# 更名栏位 
**ALTER ****TABLE** user_tbl **RENAME ****COLUMN** signup_date **TO** signup;
# 删除栏位 
**ALTER ****TABLE** user_tbl **DROP ****COLUMN** email;
# 表更名 
**ALTER ****TABLE** user_tbl **RENAME****TO** backup_tbl;
# 删除表格
**DROP ****TABLE ****IF ****EXISTS** backup_tbl;
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=PostgreSQL%E5%85%A5%E9%97%A8%E6%8C%87%E5%8D%97&action=edit&section=5)]艺搜参考
[http://www.ruanyifeng.com/blog/2013/12/getting_started_with_postgresql.html](http://www.ruanyifeng.com/blog/2013/12/getting_started_with_postgresql.html)
