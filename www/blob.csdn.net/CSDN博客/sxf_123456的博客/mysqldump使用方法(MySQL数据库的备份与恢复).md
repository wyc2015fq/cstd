# mysqldump使用方法(MySQL数据库的备份与恢复) - sxf_123456的博客 - CSDN博客
2017年12月27日 14:39:00[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：112
个人分类：[mysql](https://blog.csdn.net/sxf_123456/article/category/7122238)
https://www.cnblogs.com/emanlee/p/5410177.html
# [mysqldump使用方法(MySQL数据库的备份与恢复)](http://www.cnblogs.com/emanlee/p/5410177.html)
1.mysqldump的几种常用方法：
（1）导出整个数据库(包括数据库中的数据）
    mysqldump -u username -p dbname >
 dbname.sql    
（2）导出数据库结构（不含数据）
    mysqldump -u username -p -ddbname >
 dbname.sql    
（3）导出数据库中的某张数据表（包含数据）
    mysqldump -u username -p dbname tablename >
 tablename.sql    
（4）导出数据库中的某张数据表的表结构（不含数据）
    mysqldump -u username -p -d dbname tablename >
 tablename.sql   
3.mysqldump常用实例：
（1） mysqldump常用于数据库的备份与还原，在备份的过程中我们可以根据自己的实际情况添加以上任何参数，假设有数据库test_db，执行以下命令，即可完成对整个数据库的备份：
    mysqldump -u root -p test_db > test_db.sql    
（2）如要对数据进行还原，可执行如下命令：
    mysql -u username -p test_db < test_db.sql    
（3）还原数据库操作还可以使用以下方法：
    mysql> sourcetest_db.sql   
