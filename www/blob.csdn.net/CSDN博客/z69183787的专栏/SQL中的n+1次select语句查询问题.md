# SQL中的n+1次select语句查询问题 - z69183787的专栏 - CSDN博客
2015年05月31日 15:15:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7064
如果当[SQL](http://database.51cto.com/art/201009/223949.htm)数据库中select语句数目过多，就会影响数据库的性能，如果需要查询n个Customer对象，那么必须执行n+1次select查询语句，下文就将为您讲解这个n+1次select查询问题。
在Session的缓存中存放的是相互关联的对象图。默认情况下，当Hibernate从数据库中加载Customer对象时，会同时加载所有关联的Order对象。以Customer和Order类为例，假定ORDERS表的CUSTOMER_ID外键允许为null，图1列出了CUSTOMERS表和ORDERS表中的记录。
![](http://images.51cto.com/files/uploadimg/20100903/1558560.gif)
以下Session的find()方法用于到数据库中检索所有的Customer对象：
List customerLists=session.find("from Customer as c");
运行以上find()方法时，Hibernate将先查询CUSTOMERS表中所有的记录，然后根据每条记录的ID，到ORDERS表中查询有参照关系的记录，Hibernate将依次执行以下select语句：
select * from CUSTOMERS; 
select * from ORDERS where CUSTOMER_ID=1;
select * from ORDERS where CUSTOMER_ID=2;
select * from ORDERS where CUSTOMER_ID=3;
select * from ORDERS where CUSTOMER_ID=4;
通过以上5条select语句，Hibernate最后加载了4个Customer对象和5个Order对象，在内存中形成了一幅关联的对象图，参见图2。
![](http://images.51cto.com/files/uploadimg/20100903/1558561.gif)Hibernate在检索与Customer关联的Order对象时，使用了默认的立即检索策略。这种检索策略存在两大不足：
（1） select语句的数目太多，需要频繁的访问数据库，会影响检索性能。如果需要查询n个Customer对象，那么必须执行n+1次select查询语句。这就是经典的n+1次select查询问题。这种检索策略没有利用SQL的连接查询功能，例如以上5条select语句完全可以通过以下1条select语句来完成：
select * from CUSTOMERS left outer join ORDERS 
on CUSTOMERS.ID=ORDERS.CUSTOMER_ID 
以上select语句使用了SQL的左外连接查询功能，能够在一条select语句中查询出CUSTOMERS表的所有记录，以及匹配的ORDERS表的记录。
（2）在应用逻辑只需要访问Customer对象，而不需要访问Order对象的场合，加载Order对象完全是多余的操作，这些多余的Order对象白白浪费了许多内存空间。
为了解决以上问题，Hibernate提供了其他两种检索策略：延迟检索策略和迫切左外连接检索策略。延迟检索策略能避免多余加载应用程序不需要访问的关联对象，迫切左外连接检索策略则充分利用了SQL的外连接查询功能，能够减少select语句的数目。
