
# Oracle序列sequence - 李昆鹏的博客 - CSDN博客


2018年06月06日 21:39:38[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：60


---------------------------------------------------Oracle序列sequence------------------------------------------------
在很多数据库中都存在一个自动增长的列,如果现在要想在oracle 中完成自动增长的功能, 则只能依靠序列完成,所有的自动增长操作,需要用户手工完成处理。
语法：CREATE SEQUENCE 序列名
[INCREMENT BY n]
[START WITH n]
[{MAXVALUE/ MINVALUE n|NOMAXVALUE}]
[{CYCLE|NOCYCLE}]
[{CACHE n|NOCACHE}];
范例:创建一个seqpersonid的序列,验证自动增长的操作
CREATESEQUENCEseqpersonid;
序列创建完成之后,所有的自动增长应该由用户自己处理,所以在序列中提供了以下的两种操作：
NextVal :取得序列的下一个内容
CurrVal :取得序列的当前内容
selectseqpersonid.nextvalfromdual;
selectseqpersonid.currvalfromdual;
在插入数据时需要自增的主键中可以这样使用
![](https://img-blog.csdn.net/20180606213846318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在实际项目中每一张表会配一个序列，但是表和序列是没有必然的联系的，一个序列被哪一张表使用都可以，但是我们一般都是一张表用一个序列。
序列的管理一般使用工具来管理。
**在pl/sql中对表单约束修改**
![](https://img-blog.csdn.net/20180606213913261?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|--创建序列，序列虽然是给某个表使用，但是序列并没有绑定在某一张表，任何一张表使用这个序列都可以
|create sequence seqpersonid;
|--查询序列的下一个值(重点，常用)
|select seqpersonid.nextval from dual;
|--查看当前值是多少
|select seqpersonid.currval from dual;
|insert into person(person_id, pname,  gender, birthday)
|values(seqpersonid.nextval, '张三', 1, sysdate);
|select * from person for update;
|


