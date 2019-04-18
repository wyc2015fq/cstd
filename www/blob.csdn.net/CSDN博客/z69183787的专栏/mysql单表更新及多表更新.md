# mysql单表更新及多表更新 - z69183787的专栏 - CSDN博客
2015年08月31日 11:35:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：496
    历经Oracle，MS SQL Server，到现在的MySQL，跨越了3种资料库的学习与研究，现在对于MySQL的表更新，单表字段更新与其他类型的RDBMS资料库一样，多表级联更新还是有些区别的，不过研究过后，发现还是蛮简单的。
       如果对你有所帮助，请回复一下，给个赞！
UPDATE 语句用于中修改数据库表中的数据。
## 更新数据库中的数据
UPDATE 语句用于在数据库表中修改数据。
### 语法
UPDATE table_name
SET column_name = new_value
WHERE column_name = some_value
注释：SQL 对大小写不敏感。UPDATE 与 update 等效。
这里，关联的两张表为：tablename1、tablename2，如果需要进行关联更新tablename1里面的字段，
**[html]**[view
 plain](http://blog.csdn.net/langcai1981/article/details/25322087#)[copy](http://blog.csdn.net/langcai1981/article/details/25322087#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/334485/fork)
- update tablename1 a,tablename2 b   
**[html]**[view
 plain](http://blog.csdn.net/langcai1981/article/details/25322087#)[copy](http://blog.csdn.net/langcai1981/article/details/25322087#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/334485/fork)
- set a.grouping=b.grouping,a.size=b.size,a.description=b.description  
- where a.catalog=b.catalog and a.`schema`=b.`schema` and a.name=b.name  
当然多表更新，还有其他的方法，具体方法说明如下：
方法一：直接更新，同上面一样：
**[sql]**[view
 plain](http://blog.csdn.net/langcai1981/article/details/25322087#)[copy](http://blog.csdn.net/langcai1981/article/details/25322087#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/334485/fork)
- UPDATE product p, productPrice pp  
- SET pp.price = pp.price * 0.8  
- WHERE p.productId = pp.productId  
- AND p.dateCreated < '2014-01-01'
方法二：使用inner Join，然后更新：
**[sql]**[view
 plain](http://blog.csdn.net/langcai1981/article/details/25322087#)[copy](http://blog.csdn.net/langcai1981/article/details/25322087#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/334485/fork)
- UPDATE product p  
- INNERJOIN productPrice pp  
- ON p.productId = pp.productId  
- SET pp.price = pp.price * 0.8  
- WHERE p.dateCreated < '2014-01-01'
方法三：使用left join,然后更新：
**[sql]**[view
 plain](http://blog.csdn.net/langcai1981/article/details/25322087#)[copy](http://blog.csdn.net/langcai1981/article/details/25322087#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/334485/fork)
- UPDATE product p  
- LEFTJOIN productPrice pp  
- ON p.productId = pp.productId  
- SET p.deleted = 1  
- WHERE pp.productId ISnull
其实MySQL还可以同时更新两张表的数据的，如下：
**[sql]**[view
 plain](http://blog.csdn.net/langcai1981/article/details/25322087#)[copy](http://blog.csdn.net/langcai1981/article/details/25322087#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/334485/fork)
- UPDATE product p  
- INNERJOIN productPrice pp  
- ON p.productId = pp.productId  
- SET pp.price = pp.price * 0.8,  
- p.dateUpdate = CURDATE()  
- WHERE p.dateCreated < '2014-01-01'
以上学习心得介绍到这里，如果对你有所帮助，请回复一下，给个赞！
