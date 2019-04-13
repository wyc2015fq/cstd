
# Oracle中的约束 - 李昆鹏的博客 - CSDN博客


2018年05月29日 13:41:05[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：72


---------------------------------------------------Oracle中的约束------------------------------------------
在数据库开发中，约束是必不可少，使用约束可以更好的保证数据的完整性。
### 1.主键约束
主键约束都是在id上使用，而且本身已经默认了内容不能为空，可以在建表的时候指定。
创建一张表，把pid作为主键
createtableperson(
pidnumber(10)primarykey,
namevarchar2(10),
gendernumber(1)default1,
birthdaydate
);
主键不可重复，SCOTT.SYS_C0017981是系统自动分配的约束的名字
![](https://img-blog.csdn.net/20180529133606881?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
主键不可为空
![](https://img-blog.csdn.net/20180529133625638?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们可以自己来指定主键约束的名字
createtableperson(
pidnumber(10),
namevarchar2(10),
gendernumber(1)default1,
birthdaydate,
constraintperson_pk_pidprimarykey(pid)
);
![](https://img-blog.csdn.net/20180529133639707?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 2.非空约束
使用非空约束，可以使指定的字段不可以为空。
范例：建立一张pid和name不可以为空的表
createtableperson(
pidnumber(10)notnull,
namevarchar2(10)notnull,
gendernumber(1)  ,
birthdaydate,
);
![](https://img-blog.csdn.net/20180529133658423?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 3.唯一约束（unique）
表中的一个字段的内容是唯一的
范例：建表一个name是唯一的表
createtableperson(
pidnumber(10) ,
namevarchar2(10)unique,
gendernumber(1)  ,
birthdaydate
);
![](https://img-blog.csdn.net/20180529133709931?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
唯一约束的名字也可以自定义
createtableperson(
pidnumber(10) ,
namevarchar2(10),
gendernumber(1)  ,
birthdaydate,
constraintperson_name_ukunique(name)
);
![](https://img-blog.csdn.net/20180529133730816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|--主键约束
|create table person(
|person_id number(4) primary key,--主键是唯一的，不可以为null的
|pname     varchar2(10),
|age       number(3),
|birthday  date
|);
|insert into person
|(person_id, pname, age, birthday)
|values
|(1000, '张三', 20, to_date('1985-04-22',  'yyyy-mm-dd'));
|drop table person;
|--自定义主键名称,constraint  + 自定义主键名 + primary key(列名)
|create table person(
|person_id number(4),
|pname     varchar2(10),
|age       number(3),
|birthday  date,
|constraint person_person_id_pk primary key(person_id)--自定义主键名称
|);
|--非空约束,定义了非空约束的列表值就不能为空
|create table person(
|person_id number(4),
|pname     varchar2(10) not  null,--定义非空约束
|age       number(3),
|birthday  date,
|constraint person_person_id_pk primary key(person_id)
|);
|--唯一约束
|create table person(
|person_id number(4),
|pname     varchar2(10) unique,--定义为唯一约束，一张表可以有多个唯一约束，但是值不能重复,可以有多个null
|age       number(3),
|birthday  date
|);
|insert into person
|(person_id, pname, age, birthday)
|values
|(1000,null, 20, to_date('1985-04-22', 'yyyy-mm-dd'));
|--自定义唯一约束名
|create table person(
|person_id number(4),
|pname     varchar2(10),--定义为唯一约束，一张表可以有多个唯一约束，但是值不能重复,可以有多个null
|age       number(3),
|birthday  date,
|constraint person_pname_uk unique(pname)--自定义唯一约束名
|);
|select * from person;
|

### 4.检查约束
使用检查约束可以来约束字段值的合法范围。
范例：创建一张表性别只能是1或2
createtableperson(
pidnumber(10) ,
namevarchar2(10),
gendernumber(1)check(genderin(1,2)),
birthdaydate
);
![](https://img-blog.csdn.net/2018052913380886?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
检查约束也可以自定义
createtableperson(
pidnumber(10) ,
namevarchar2(10),
gendernumber(1),
birthdaydate,
constraintperson_gender_ckcheck(genderin(1,2))
);
![](https://img-blog.csdn.net/20180529133818428?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 5.外键约束
之前所讲的都是单表的约束，外键是两张表的约束，可以保证关联数据的完整性。
范例：创建两张表，一张订单表，一张是订单明细表，订单和明细是一对多的关系
createtableorders(
order_idnumber(10) ,
total_pricenumber(10,2),
order_timedate,
constraintorders_order_id_pkprimarykey(order_id)
);
createtableorder_detail(
detail_idnumber(10) ,
order_idnumber(10),
item_namevarchar2(10),
quantitynumber(10),
constraintorder_detail_detail_id_pkprimarykey(detail_id)
);
insertintoordersvalues(1,200, to_date('2015-12-12','yyyy-MM-dd'));
insertintoorder_detailvalues(1,2,'java',1);
我们在两张表中插入如上两条数据，我们发现在order_detail表中插入的order_id在order表中并不存在，这样在数据库中就产生了脏数据。此时需要外键来约束它。
我们再次建表
createtableorders(
order_idnumber(10) ,
total_pricenumber(10,2),
order_timedate,
constraintorders_order_id_pkprimarykey(order_id)
);
createtableorder_detail(
detail_idnumber(10) ,
order_idnumber(10),
item_namevarchar2(10),
quantitynumber(10),
constraintorder_detail_detail_id_pkprimarykey(detail_id),
constraintorder_detail_order_id_fkforeignkey(order_id)referencesorders(order_id)
);
![](https://img-blog.csdn.net/20180529133949742?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
外键关联一定注意：
外键一定是主表的主键
删表时一定先删字表再删主表，如果直接删主表会出现由于约束存在无法删除的问题
但是可以强制删除drop table orders cascade constraint;(不建议)
删除主表的数据可以删除子表的关联数据，再删主表，也可以使用级联删除级联删除在外键约束上要加上on delete cascade如
constraintorder_detail_order_id_fkforeignkey(order_id)
referencesorders(order_id)ondeletecascade
这样删除主表数据的时候会把字表的关联数据一同删除
![](https://img-blog.csdn.net/2018052913402316?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**唯一约束和主键约束的区别：**
**简单说：**
**主键不能为空**
**而唯一可以为空**
**相同的就是****都不允许重复**
**详细的说：**
**1.****主键约束（PRIMARY KEY****）**
**主键用于唯一地标识表中的每一条记录，一张表只能有一个主键约束。**
**是不可能（或很难）更新．**
**主键列上没有任何两行具有相同值（即重复值），不允许空（NULL****）．**
**主健可作外健，****唯一索引****不可；**
**2.****唯一性约束（UNIQUE****）**
**唯一性约束用来限制不受主键约束的列上的数据的唯一性，用于作为访问某行的可选手段，一个表上可以放置多个唯一性约束．**
**只要唯一就可以更新．**
**即表中任意两行在****指定列上都不允许有相同的值，允许空（NULL）．**
**一个表上可以放置多个唯一性约束**
检查约束和外键约束代码示例：
|--检查约束
|drop table person;
|create table person(
|person_id number(4),
|pname     varchar2(10),
|gender    number(1) check  (gender in(1 , 2)),--定义检查约束
|age       number(3),
|birthday  date
|);
|insert into person
|(person_id, pname, age, birthday,gender)
|values
|(1000,'张三', 20, to_date('1985-04-22',  'yyyy-mm-dd'),2);
|--自定义检查约束的名字
|--检查约束
|drop table person;
|create table person(
|person_id number(4),
|pname     varchar2(10),
|gender    number(1),
|age       number(3),
|birthday  date,
|constraint person_gender_ck check (gender in(1 , 2))
|);
|--外键约束
|drop table orders;
|create table orders(
|order_id       number(4),
|total_price    number(7,2),
|order_time     date,
|constraint orders_order_id_pk primary key(order_id)
|);
|drop table order_detail;
|create table order_detail(
|detail_id      number(4),
|item_name      varchar2(10),
|price          number(7,2),
|quantity       number(4),
|order_id       number(4),
|constraint order_detail_id_pk primary key(detail_id),
|constraint order_detail_oder_id_fk foreign key(order_id) references  orders(order_id)--定义外键约束
|);
|insert into orders values(1000, 10000,  sysdate);
|insert into order_detail values(1000,  'iphone', 5000, 2, 1000);--子表插入时有外键约束
|select * from orders;
|select * from order_detail;
|delete from roders;--主表删除数据时有外键约束，如果想要删除需要先删除子表的数据再删除主表
|drop table orders;--主表删除时受外键约束，需要先删除子表再删除主表
|drop table orders cascade constraint;--强制删除主表，不建议这样删除
|--级联删除
|drop table orders;
|create table orders(
|order_id       number(4),
|total_price    number(7,2),
|order_time     date,
|constraint orders_order_id_pk primary key(order_id)
|);
|drop table order_detail;
|create table order_detail(
|detail_id      number(4),
|item_name      varchar2(10),
|price          number(7,2),
|quantity       number(4),
|order_id       number(4),
|constraint order_detail_id_pk primary key(detail_id),
|constraint order_detail_oder_id_fk foreign key(order_id)
|references orders(order_id) on delete cascade --级联删除设置
|);
|delete from orders; --级联删除
|


