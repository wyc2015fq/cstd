# MySQL导入sql脚本 导出数据库 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月11日 15:47:59[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：326

导出数据库
不能停止服务
cd /var/lib/mysql (进入到MySQL库目录，根据自己的MySQL的安装情况调整目录) 
mysqldump -u用户名 -p 数据库名 > 导出的文件名
或在-p后紧跟密码，不留空格，不建议在命令行中直接输入密码，以防别人从历史记录中看到密码。
导入
mysql>use DATABASE_NAME;   
mysql>sourcepath/to/file.sql;
## 艺搜参考
[http://www.cnblogs.com/vitus_feng/archive/2010/05/21/1741262.html](http://www.cnblogs.com/vitus_feng/archive/2010/05/21/1741262.html)
导出数据库
不能停止服务
cd /var/lib/mysql (进入到MySQL库目录，根据自己的MySQL的安装情况调整目录) 
mysqldump -u用户名 -p 数据库名 > 导出的文件名
或在-p后紧跟密码，不留空格，不建议在命令行中直接输入密码，以防别人从历史记录中看到密码。
导入
mysql>use DATABASE_NAME;   
mysql>sourcepath/to/file.sql;
## 艺搜参考
[http://www.cnblogs.com/vitus_feng/archive/2010/05/21/1741262.html](http://www.cnblogs.com/vitus_feng/archive/2010/05/21/1741262.html)

