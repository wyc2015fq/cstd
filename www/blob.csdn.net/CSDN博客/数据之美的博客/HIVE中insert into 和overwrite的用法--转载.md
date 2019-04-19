# HIVE中insert into 和overwrite的用法--转载 - 数据之美的博客 - CSDN博客
2017年06月13日 21:16:53[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：909
insert into 和overwrite的用法：
Insert into代码  ![收藏代码](http://snv.iteye.com/images/icon_star.png)
- INSERT INTO TABLE tablename1 [PARTITION \  
- (partcol1=val1, partcol2=val2 ...)] \  
- select_statement1 FROM from_statement;  
Sql代码  ![收藏代码](http://snv.iteye.com/images/icon_star.png)
- INSERT OVERWRITE TABLE tablename1 [PARTITION \  
- (partcol1=val1, partcol2=val2 ...) [IF NOT EXISTS]] \  
- select_statement1 FROM from_statement;  
两种方式的相同点：
1.两个表的维度必须一样，才能够正常写入
2.如果查询出来的数据类型和插入表格对应的列数据类型不一致，将会进行转换，但是不能保证转换一定成功，比如如果查询出来的数据类型为int，插入表格对应的列类型为string，可以通过转换将int类型转换为string类型；但是如果查询出来的数据类型为string，插入表格对应的列类型为int，转换过程可能出现错误，因为字母就不可以转换为int，转换失败的数据将会为NULL。
不同点：
1.insert into是增加数据
2.insert overwrite是删除原有数据然后在新增数据，如果有分区那么只会删除指定分区数据，其他分区数据不受影响
