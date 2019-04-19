# SQL Server复制表结构和表数据生成新表的语句 - =朝晖= - 博客园
# [SQL Server复制表结构和表数据生成新表的语句](https://www.cnblogs.com/dhcn/p/7100273.html)
参考：[http://topic.csdn.net/t/20020621/09/820025.html](http://topic.csdn.net/t/20020621/09/820025.html)
SELECT   *   INTO   newTableName   FROM   oldTableName
此方法将把旧表的结构和数据同时copy生成新表，不过主键外键约束没有生成，需要手动设置。

