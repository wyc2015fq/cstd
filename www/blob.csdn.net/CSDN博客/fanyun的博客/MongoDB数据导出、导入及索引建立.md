# MongoDB数据导出、导入及索引建立 - fanyun的博客 - CSDN博客
2016年06月04日 09:54:55[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：11363
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)[大数据](https://blog.csdn.net/column/details/bigdata01.html)
MongoDB数据导出、导入及索引建立
（1）    数据导出——每个表一个文件：
# mongoexport --db project --collection 表名 --out 输出文件名，如：
# mongoexport --db project --collectionstock --out stock.json，导出所有F10数据到文件/root/stock.json中
（2）    数据导入——每个表一个文件：
# mongoimport --db users --collection 表名 --file输入文件名
（3）    数据备份：
一个表：
# mongodump --db project --collection 表名 --out 目的路径
整个库：
# mongodump --db project--out 目的路径
在目的路径生成以数据库名命名的备份文件夹，默认备份路径在用户主文件夹的dump目录下
（4）    数据恢复：
一个表:
# mongorestore  --db project --collection 表名备份路径
备份路径需要指向一个表的.bson文件，如：#mongorestore  --db project --collection news_cjyw/…/dump/project/news_cjyw.bson
整个库：
# mongorestore  --db project备份路径
备份路径需要指向以数据库名命的文件夹，如：
# mongorestore  --db project /…/…/dump/project
备份路径中对应的.bson文件与.metadata文件需要同时存在
（5）    建立索引：（用dump/restore方式恢复数据不用重新建立索引，export/import方式需要重新建立索引）
进入数据库：
# mongo
>use project
>db.stock.ensureIndex({“股票代码”:1，“lastChange”:-1}) //这个索引必须建，两个参量的顺序不能错
>exit
