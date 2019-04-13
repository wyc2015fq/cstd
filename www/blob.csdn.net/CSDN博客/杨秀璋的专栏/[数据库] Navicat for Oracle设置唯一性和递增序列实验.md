
# [数据库] Navicat for Oracle设置唯一性和递增序列实验 - 杨秀璋的专栏 - CSDN博客

2016年04月11日 05:23:36[Eastmount](https://me.csdn.net/Eastmount)阅读数：17719所属专栏：[数据库实战开发设计与优化](https://blog.csdn.net/column/details/14842.html)



这篇文章主要讲述Navicat for Oracle设置唯一性约束和设置某字段为递增序列。文章主要以图文为主，虽然都是非常简单基础的文章，但还是希望对你有所帮助。
推荐前一篇文章：[[数据库] Navicat for Oracle基本用法图文介绍](http://blog.csdn.net/eastmount/article/details/51062044)

## 一. 设置唯一性
参考文章：[Oracle之唯一性约束(UNIQUE Constraint)用法详解](http://blog.csdn.net/jssg_tzw/article/details/40981393)
唯一性约束英文是Unique Constraint，它是指表中一个字段或者多个字段联合起来能够唯一标识一条记录的约束。联合字段中，可以包含空值。
那唯一性约束与主键又有什么区别呢？
你可能会疑惑主键(Primary Key)不是唯一标识一张表的吗？
主键是所有组成主键的列都不能包含空值，而如果唯一性约束由多列组成，其中的部分列可以包含空值。同时，注意Oracle中不容许在相同列上既创建主键又创建唯一性约束。
例如学生表Student(id, stu_id, stu_name, sex, email, year)，其中主键是学生学号(stu_id, year)，假设该表学生每年都需要录入一遍数据，所以两个字段构成主键。
同时为了方便遍历，序号id也需要设置不同，这时候就需要id设置为唯一性。
唯一性约束可以在创建表时或使用ALTER TABLE语句创建。
创建唯一性约束的SQL语句如下：
```python
ALTER TABLE table_name  
ADD CONSTRAINT constraint_name  
UNIQUE (column1, column2, ... , column_n);
```
删除唯一性约束的SQL语句如下：
```python
ALTER TABLE table_name  
DROP CONSTRAINT constraint_name;
```
使用Navicat for Orcale方法如下：
![](https://img-blog.csdn.net/20160411032326868)
两种方法均可设置唯一性约束：
alter table T_USER_INFO add constraint u_id unique(ID);
alter table T_USER_INFO  add unique(ID);
![](https://img-blog.csdn.net/20160411032434183)
此时查看表T_USER_INFO结构如下所示，其中用户名为主键：
![](https://img-blog.csdn.net/20160411032500574)
同样查看唯一性可以看到刚设置的序号ID：![](https://img-blog.csdn.net/20160411032553231)
当修改表T_USER_INFO中ID字段，出现相同值或插入数据ID已经存在的时候，会报错：
![](https://img-blog.csdn.net/20160411032725702)

## 二. 设置递增序列
前面讲述的插入序号ID，但是数据库中序号肯定会很多，如何在插入ID数据过程中更方便避免重复的插入呢？这就涉及到了设置ID为递增序列的方法。
Navicat for Orcale不像MySQL只需要勾选递增序列即可，它需要SQL语句来设置。所以这部分内容也非常重要，希望对你有所帮助。
主要步骤包括：
1) 设置序列
2) 设置对应序列和表的触发器
3) 插入数据时使用对应的触发器
具体方法如下：
**第一部分 设置序列**
1.通过点击"其他=》序列"可以操作序列：
![](https://img-blog.csdn.net/20160411035225538)

2.点击进去后可以设计序列、新建序列、删除序列。
![](https://img-blog.csdn.net/20160411035324507)
3.同样可以通过代码增加序列，对应上面的表T_USER_INFO用户表。
序列参数详见：[oracle中创建序列以及序列使用注意事项](http://blog.csdn.net/zhengqiqiqinqin/article/details/9068923)
```python
CREATE SEQUENCE User_Info_SEQ  
minvalue 1  
maxvalue 9999999999999999999  
increment by 1  
start with 1;
```
![](https://img-blog.csdn.net/20160411035509336)
4.点击新建的序列User_Info_SEQ后可以发现具体的序列：
![](https://img-blog.csdn.net/20160411035820180)
**第二部分 设置触发器**
5. 然后设置对应表T_USER_INFO中字段ID的触发器，实现在向表中插入数据前自动插入自增长的序列。
```python
CREATE OR REPLACE TRIGGER Trigger_User    
BEFORE INSERT ON T_USER_INFO    
	for each row    
	when(NEW.ID is NULL)  
	begin    
		select User_Info_Seq.Nextval into :NEW.ID from dual;    
	end;
```
创建成功如下图所示，注意不能缺少when(NEW.ID is NULL)，因为在插入数据时会缺省ID这个值，让触发器自动补充。
![](https://img-blog.csdn.net/20160411042841192)

6.查看表T_USER_INFO结构可以查看对应的触发器：
![](https://img-blog.csdn.net/20160411040502845)
但有可能会报编译错误[Err] ORA-24344: success with compilation error。其原因是表的所有列名都需要大写，也可以手动添加“更新栏位”，否则如图虽然有增加触发器，但是更新栏位为空，所以这个插入是不成功的，插入时也不会触发主键的序列。
![](https://img-blog.csdn.net/20160411041400655)
**第三部分 插入数据**
7.插入数据
```python
insert into T_USER_INFO 
	(USERNAME,PWD,STATUS,DW_NAME,DW_CODE) 
values('eastmount','000000','2','软件学院','08111003');
```
此时能插入数据，其中字段ID会自动添加。
![](https://img-blog.csdn.net/20160411050257515)
如果报错[Err] ORA-00001: unique constraint (GUICAI.SYS_C0017014) violated，如下图所示。这可能是因为表中设置了唯一性ID，出现重复数据的现象。如果不能设置唯一性约束且需要保持唯一性，推荐使用sys_guid()函数产生唯一编码。其实质想法是获取表中最大ID值再自动加1。
![](https://img-blog.csdn.net/20160411045507834)
参考：[[OARCLE]在Oracle中生成GUID类型--SYS_GUID()函数](http://blog.itpub.net/27042095/viewspace-1064504/)
test01(ID varchar2 32, NAME varchar2 20)
insert into "test01" values (sys_guid(),'张三');
![](https://img-blog.csdn.net/20160411202151740)

如果报错：[Err] ORA-00001: unique constraint  violated
它是因为在插入数据过程中ID出现重复数据违背了唯一性约束，可以修改ID值，或设置增大序列为最大值。即：startwith5000，但连续插入两次时就会成功插入。
最后我删除了唯一性约束，仅仅通过触发器插入递增序列，防止violated错误。

参考：[Oracle实现类似SQL SERVER列自增长功能-jssg_tzw](http://blog.csdn.net/jssg_tzw/article/details/6917988)
[Oracle中添加自动编号的序列-左What](http://blog.sina.com.cn/s/blog_a261421801017027.html)
[oracle中字段递增的实现-百度文库](http://wenku.baidu.com/link?url=8nSYM377ltUzhtztLyitKNnvDIOwGK7C8K2on2kkuhb8vbwk55DEbmV5aCtQ3grurEXFPibAsq7TH7gKFefbLdyz58XZJ124ZinXxizqvT3)
[oracle中DB设计时id唯一性（或者自增）怎么设置](http://bbs.csdn.net/topics/390385719)
[SYS_GUID()与sequence的比较](http://www.cnblogs.com/BradMiller/archive/2010/11/10/1873314.html)
[oracle数据库的主键递增问题，插入数据问题](http://bbs.csdn.net/topics/390737580?page=1)

最后还是希望文章对你有所帮助，这篇文章主要是我关于Navicat for Orcale软件的使用方法，包括设置唯一性和递增序列的实验，主要通过图文的形式进行讲述，可能这个很简单的问题复杂化了，但是对初学者来说还是有一定帮助的，高手勿喷~
最近真的好忙，昨天就睡了4个小时，现在又熬夜到凌晨五点了，但是每每分享一篇博客之后，感觉都非常兴奋或开心，有些东西不能言表，开心就好！
(By：Eastmount 2016-04-11 清晨5点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


