# 利用crontab定时备份数据库为sql文件 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月31日 15:53:54[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：351
测试：
每1分钟执行1次,利用crontab
*/1 * * * * /opt/nginx/db_bak.sh
每天的0点0分把数据库备份为日期格式
0 0 * * * /opt/nginx/db_bak.sh
脚本如下：
#!/bin/bash
logs_path="/opt/nginx/sql_bak/"
mkdir -p ${logs_path}$(date -d "yesterday" +"%Y")/$(date -d "yesterday" +"%m")/
cd /var/lib/mysql
mysqldump -uroot -pyourdbpasswordtablename>${logs_path}$(date -d "yesterday" +"%Y")/$(date -d "yesterday" +"%m")/tablename_$(date -d "yesterday" +"%Y%m%d").sql
注意需要修改上面的数据库用户名和密码

结果如下：
ls /opt/nginx/sql_bak/2016/03/
