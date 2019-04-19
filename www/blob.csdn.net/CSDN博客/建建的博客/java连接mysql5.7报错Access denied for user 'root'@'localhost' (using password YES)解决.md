# java连接mysql5.7报错Access denied for user 'root'@'localhost' (using password: YES)解决 - 建建的博客 - CSDN博客
2018年01月18日 10:52:25[纪建](https://me.csdn.net/u013898698)阅读数：113
## **第一情况mysql工具连接mysql报错 如sqlyong或navicat**
注意：
MySQL5.7 mysql.user表没有password字段，改 为authentication_string；
### 解决：
- 打开MySQL目录下的my.ini文件，在文件的最后添加一行“skip-grant-tables”，保存并关闭文件。（WIN7默认安装，my.ini在C:\ProgramData\MySQL\MySQL Server 5.6）
- 重启MySQL服务。
- 通过命令行进入MySQL的BIN目录，输入“mysql -u root -p”(不输入密码)，回车即可进入数据库。（WIN7默认安装，BIN目录为：C:\Program Files\MySQL\MySQL Server 5.6\bin）
- 执行“use mysql;”，使用mysql数据库。
- 执行“ update MySQL.user set authentication_string=password('123456') where user='root'”（修改root的密码）
- 打开MySQL目录下的my.ini文件，删除最后一行的“skip-grant-tables”，保存并关闭文件。
- 重启MySQL服务。
- 在命令行中输入“mysql -u root -p 123456”，即可成功连接数据库。
## 第二种情况mysql工具可以链接 服务器报错
解决：
![](https://img-blog.csdn.net/20170228171812897?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzMyNTE4NTk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# properties文件值后面一定不要有空格！！！
