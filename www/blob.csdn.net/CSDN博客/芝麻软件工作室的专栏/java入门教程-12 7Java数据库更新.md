
# java入门教程-12.7Java数据库更新 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月29日 07:51:25[seven-soft](https://me.csdn.net/softn)阅读数：134个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



数据库更新操作包括数据表创建、删除、以及数据表记录的增加、删除、修改等操作。如果利用数据 SQL命令实现，则利用Statement对旬的executeUpdate()方法，执行SQL的update语句，实现数据表的修改；执行SQL的insert语句，实现数据表记录的添加。
例如，在前面数据为查询例子基础上，再增加对数据表的修改和插入。限于篇幅，不再给出完整程序，只给出实现修改和插入的方法。程序可再增设插入、，，除保存按钮，通过已有的浏览，定位到数据表的特定位置，对痈疽记录进行编辑修改，或插入，或删除，然后按保存按钮，完成修改后的数据表保存。
下面用代码说明数据表更新的方法。与数据表连接时，需指定获得的ResultSet 对象是可更新的。
stmt = connect.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE,ResultSet.CONCUR_UPDATABLE);

