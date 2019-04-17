# mysql必知必会--学习笔记1 - zhuzhuxia183的博客 - CSDN博客





2018年07月27日 16:47:44[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：41标签：[mysql](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[数据库](https://blog.csdn.net/zhuzhuxia183/article/category/7879767)









mysql是我们经常使用的一种关系型数据库，一般我们配置好之后，比较常用的登陆方式为： 

> 
mysql -u root  -p


之后然后输入我们的root密码，便可以进入到我们的mysql数据库 

作为数据库，是用来保存数据的，当然我们想要了解数据库和表的话，也是可以通过以下几个常用的命令

> 
show databases;  #这个命令是用来展示当前登陆用户所能查看的所有数据库，返回一个数据库名的列表（包含mysql内部使用的数据库） 

  use xxx;  #这个命令是进入到名为 xxx的数据库内， 

  show tables; #这个命令是用来显示当前使用的数据库下的所有表名列表 

  show columns from xxx; #这个命令是用来显示一个表的结构信息，当然还可以用 

  describe xxx;#功能同上
然后关于检索数据的一些基本操作（select）： 
*1、列  *

    就是字段的选择，主要有三种： 

    选择一个：
> 
select column1 from XX;


选择指定多个

> 
select column1,column2 from XX;


选择所有的字段进行搜索

> 
select * from XX;


**当然我们得注意，这种使用*来选择所有的字段是一种十分耗时的操作，在实际应用中最好自己把所需要的字段给列出来，会提高速度**

*2、行*

这里就是对于我们搜索行的一个筛选，比如我们想要筛选第几行到第几行的这种操作 

有三种方式
> 
select column from xx limit 5;


上面的含义是最多选择5个，从最开始的一行，如果没有，就显示为empty;

> 
select column from xx limit 4,1;


上面的含义是从第行4行开始，选取1个

> 
select column from xx limit 4 offset 1;


上面的含义是从第1个偏移量开始选取4个 
**所以可知最后两种表达方式中参数的含义是相反的**







