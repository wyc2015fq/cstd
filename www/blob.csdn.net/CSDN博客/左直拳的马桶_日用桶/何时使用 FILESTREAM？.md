# 何时使用 FILESTREAM？ - 左直拳的马桶_日用桶 - CSDN博客
2010年10月13日 16:30:00[左直拳](https://me.csdn.net/leftfist)阅读数：1210标签：[存储																[sql server																[数据库](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
# 何时使用 FILESTREAM
在 SQL Server 中，BLOB 可以是将数据存储在表中的标准 **varbinary(max)**
 
数据，也可以是将数据存储在文件系统中的 FILESTREAM **varbinary(max)**
 
对象。数据的大小和应用情况决定您应该使用数据库存储还是文件系统存储。如果满足以下条件，则应考虑使用 FILESTREAM：
- 所存储的对象平均大于 1 MB。
- 快速读取访问很重要。(快速读取谁？是这个BLOB类型对象，还是它所在的表？)
- 您开发的是使用中间层作为应用程序逻辑的应用程序。
对于较小的对象，将 **varbinary(max)**
 BLOB 
存储在数据库中通常会提供更为优异的流性能。
