# sqlite3一些简单命令 - Arthur的随笔 - CSDN博客
2011年07月08日 11:20:11[largetalk](https://me.csdn.net/largetalk)阅读数：393标签：[sqlite																[schema																[数据库																[html](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=schema&t=blog)](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)
个人分类：[杂七杂八](https://blog.csdn.net/largetalk/article/category/823511)
                sqlite3 filename #打开或创建数据库sql
sqlite3 filename "select * from film;" #直接执行sql
sqlite3 -html filename "select * from film" #输出html格式
sqlite3 filename ".dump" > ouput.sql #dump database
sqlite3 filename < output.sql #import database
.help #查看帮助
.tables tablename #列出所有或匹配的数据表
.schema tablename #查看表的schema            
