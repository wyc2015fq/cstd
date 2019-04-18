# MySQL语法结构--数据库的管理 - wsp_1138886114的博客 - CSDN博客





2018年05月17日 17:37:46[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：44








# MySQL的语法结构


### 一、基本语法格式

​    登陆MySQL                    mysql -h localhost -P 3306 -u root -p

                                      mysql -u root -p(本机登录时可用)

![](https://img-blog.csdn.net/20180520150012997)

![](https://img-blog.csdn.net/20180520150115899)

    查看数据库                     show databases;

    使用某个数据库                use db_name;

    查看某个数据库有哪些表        show tables;
    查看表结构                            desc tb_name;

    查询当前表中所有数据        select * from db_name;
    查询user表中所有数据        select * from user;
                                              select user,host from user;   
    数据库层次结构：
        类似于excel
            Excel文件    数据库
            sheet          表
            行，列        行，列


####     常见数据类型
            数值型：
            浮点型：    float ，double    
            字符型：    char(M) / varchar(一定要给长度)
            日期类型： date : YYYY-MM-DD
                              datetime:YYYY-MM-DD    HH:mm:ss        也可能是：DD/MMM/YYYY HH:mm:ss

    ​                           timestamp: 从1970点1月1日0点0分0秒
                               Java 工具类进行转换：simpledataformat

####     六大约束：  保证数据完整性，准确性，和一致性   

                    not null:不能为空     一般用于修饰某一列，修饰之后该值不能为空                    

                    default：(默认值)    一般用于修饰某列，修饰之后该值为默认值
                    unique key（UK）: 唯一约束
                                                    一般用于修饰某列，修饰之后该列的值不允许有重复值

                    primary key(PK):   主键（可以为多个）
                                                -功能：唯一标识一行
                                                -自带属性：唯一和非空
                                                -多列和并主键
                                                day        times
                                                YYYY-MM-DD    HH-mm-ss
                    auto_increment:   自动增长    修饰某列，某列可以自定增长
                                                必修是主键，必须是int类型
                                                自动增长值是不能回退的
                    foreign key:  外键，减少冗余
                                        是另一个sheet的主键

### 二、基本语法格式
​    分类
        DDL：数据定义语言：主要用于管理数据库（数据库，表）。（创建create，设计alter，删除drop）
        DML：数据操作语言：主要用于管理（表中的）数据的。（增inset，删delete，更updata，查sel）
        DCL：数据的控制语言：主要用于用户管理用户权限/安全性等系统管理
             grant all privileges


### 三、DDL：数据库与表的管理

####     --常见单词
                query ok:语句执行成功
                row：行
                col:colunmn表示列
                affected：受影响

####     --数据库的管理
        -创建        语法：create_databases [if not exists] db_name;
                        例子：create database hadoop01;
                                  create database if not exists hadoop01;

                              create database hadoop02; 

        -删除        语法：drop database[if exists] db_name;
                        例子：drop database if exists hadoop01;
        -使用：use db_name
        -查看: show db_name;

####     --表的管理

#####        -查看：
            show tabels;            #查看表
            desc db_name;        #查看表结构

#####         -使用表：
            use tabel_name;
            -在当前数据库下：直接使用表名
            -不在当前数据库下：数据名.表名

#####         -创建表
            create table[if not exists]`tb_name`(
            `col1_id` int(11) NOT NULL PRIMARY KEY AUTO_INCREMENT,    # 类型int[11]，主键，自增,
            `col2_name` varchar(25),                                                                # 类型    varchar(25)，
            `col3_type`  commet "字段注释",
            ……
            colN type[约束]

             )engine=InnoDB/MyISAM [default charset=utf8];             #表格类型InnoDB(数据量大，支持事务处理)

    详情：

![](https://img-blog.csdn.net/20180520115920778)


#####         -删除表
​            drop tables tb_name;


### 四、案例表创建

####     --创建商品类型表

```python
create database if not exists `shop02`;            #创建数据库shop02
            use shop02;                                        #使用数据库shop02
            create table if not exists`commoditytype`(         #创建表commoditytype    
                ct_id int ,
                ct_name varchar(50) not null,
                primary key(ct_id)                   #设置主键
                )default charset=utf8;
            show databases;        #显示数据库，查看是否创建成功
            show tables;           #显示表格
            desc commoditytype;    #查看表结构
```

####     --创建商品表

```python
create table if not exists `commodity`(
                `c_id` int primary key auto_increment,        #自增必须是主键
                `c_name` varchar(50) not null,
                `c_madein` varchar(50) ,
                `c_type` int ,
                `c_inprice` int not null,
                `c_outprice` int ,        #(unique)
                `c_num` int default '100',
            constraint `fk_01` foreign key(`c_type`) references `commoditytype`(`ct_id`)             
            )default charset=utf8;        #`fk_01`为外键名，   `ct_id`为外表`commoditytype`的主键字段
```

####     --创建用户表

```python
create table if not exists customer(
                cu_id int primary key,
                cu_name varchar(50) not null,
                cu_phone varchar(20) not null,
                cu_gender int default 1,
                cu_address varchar(100) not null)default charset=utf8;
```

####     --创建订单表

```python
create table if not exists `order`(
                o_id int primary key,
                o_cuid int not null,
                o_cid int not null,
                o_num int not null,
                constraint `fk_02` foreign key (`o_cuid`) references `customer`(`cu_id`),
                constraint `fk_03` foreign key (`o_cid`) references `commodity`(`c_id`))default charset=utf8;
            show tables;
            desc `order`;
```

####     --修改数据库，表结构的命令
    ​    alter命令
        修改表名:             alter table 旧表名 rename as 新表名
        添加字段:             alter table 表名 add 字段名 列类型 [ 属性 ]
        修改字段类型:      alter table 表名 modify 字段名 列类型 [ 属性 ]
        修改字段:             alter table 表名 change 旧字段名 新字段名 列类型 [ 属性 ]
        删除地段:             alter table 表名 drop 字段名

        添加主键：          alter table table_name（表名） add primary key;         
        显示主键：          how index from table_name（表名）;                    

​        添加外键             alter table table_name（表名） add constraint FK_stu_tea（外键名） foreign key(表名里面的表头)

    ​    例如：                alter table stu_name add constraint FK_stu_tea foreign key(stuNo) 


                                  references tea_name(tea_Num)；        #建表后添加外键方式

### 五、DML：数据操作语言

#### ​    --增：inset    

        语法：insert into tb_name[(col1,col2,……)]values(value1,value2,……),(value1,value2,……)……;
        注意：
            如果不写列名，那么value中必修包含每一列的值
            如果写列名，那么value的值必须与列名一一对应

```python
insert into commoditytype values(1,'书籍');
            insert into commoditytype (ct_id,ct_name) values(2,'文具');
            insert into commoditytype (ct_id,ct_name) values(3,'玩具'),(4,'食物');
```

            #主键自动增长（添加数据时，省略主键必须要写明字段）

```python
insert into commodity values(null/0,'变形金刚','中国',3,250,500,10);
            insert into commodity values(1,'java编程思想','美国',1,10,250,5);
            insert into commodity(c_name,c_madein,c_type,c_inprice,c_outprice,c_num)\
            values('文具盒','中国',2,2,10,50);
```
            select * from commodity;    #查看表
            #null/默认值 测试

```
insert into commodity values(null,'西游记','中国',1,10,null,100);
            insert into commodity values(null,'遥控汽车','日本',3,350,998,default);
            insert into commodity values(null,'三国演义','中国',1,10,98,null);
```

             批量插入

```
insert into customer values(1,'刘德华','110',1,'中国香港'),(2,'黎明','120',1,'中国香港'),\
          (3,'杨幂','119',0,'中国大陆'),(4,'林志颖','112',1,'中国台湾');
          insert into `order` values(1,1,1,1),(2,2,3,5),(3,3,5,10),(4,4,6,1);
```

####     --删：delete    

        语法：delete from tb_name [where 条件]
        注意：如果不加where子句，表示删除整张表的内容（索引不会重置）
             delete from `order` where o_id=4;
             delete from commodity where c_inprice is not null and c_madein = '日本';

             truncate (清空表数据)    （会将索引重置 ）
             truncate tb_name;


####     --更: updata

        update tb_name set col1 = value1,col2 = value2,……[where 条件]
        update commodity set c_num = 60 where c_name = '三国演义';

        select * from commodity;


####     --查:select     

        语法：select col1,col2,……from tb_name where[条件]
        直接查询常量
            select 'hello word!';
            select 'hello word!'as '你好';
        查询表中所有数据
            select * from commodity;
        查询固定的字段
            select c_name as '商品名称',c_inprice as '商品进价',c_num as '商品库存' from commodity;
        添加where 子句：查询商品表中所有的玩具名称

```python
select ct_id from commoditytype where ct_name = '玩具';
		select c_name as '名称' from commodity where c_type = 3;
```
        destinct：用于数据去重
            select distinct c_type as '类型'from commodity;


#####         四则运算
            #求每个商品的总成本
            select c_name as '商品',(c_inprice * c_num) as '成本' from commodity;
            #求每个商品的利润
             select c_name as '商品',(c_outprice - c_inprice)*c_num as '利润' from commodity;

#####         关系运算
            -- > < = >= <= !=
            #查询商品表中售价大于50的商品
            select c_name as '商品',c_outprice as '售价' from commodity where c_outprice>50;

#####         逻辑运算    and or
            #查询所有玩具和文具的商品
            select c_name as '商品',c_type as '类型'from commodity where c_type = 2 or c_type = 3;

            #查询产品是中国并且售价大于200的商品
            select c_name as '商品',c_outprice as '售价' from commodity where c_outprice > 200 and  c_madein = '中国';

            between and :用于过滤范围值 等价于：大于多少and少于多少
            not between and:不在这个范围之间
            #查询所有商品售价在50-200之间的商品
           select c_name as '商品',c_outprice as '售价' from commodity where c_outprice between 50 and 200;

#####         判断 空或者非空
            select * from commodity where c_outprice is not null;

#####         从范围中取值：in , not in
            #查询所有玩具和文具的商品
            select * from commodity where c_type in (2,3);
            #查询所有除玩具和文具以外的商品
            select * from commodity where c_type not in (2,3);

#####         值的匹配：like
            一般与通配符连用：_：表示匹配单个字符，%：表示匹配多个字符，*：表示匹配所有
            #查询所有商品类型中的名称带有“具”字的商品类型，并且名称只有两个字。
            select * from commoditytype where ct_name like '_具';
            #查询所有包含“Java的商品”
            select * from commodity where c_name like '%java%';


#####         排序检索：

        order by key :按照某一列进行排序
        默然升序排序：asc
        默然降序排序：desc
            #查询所有商品，按照商品售价升序排序
            select * from commodity order by c_outprice asc;
            select * from commodity order by c_outprice desc;


#####         选择部分行：limit 0,n
            #查询售价最高的三个商品
            select * from commodity order by c_outprice desc limit 1,3;
            select * from commodity limit 2,2;        #查看第二页前两个数据


### 六、数据表单的函数查询

####     统计函数：
        COUNT()函数：统计记录数；select count(*/c_num) from commodity;    #commodity表名
        AVG()函数：求平均值；     select avg(c_outprice) from commodity;
        SUM()函数：求和；
        MAX()函数：求最大数；
        MIN()函数：求最小数；

        分组查询语法如下: select * from tabel_name group by field;
                                     select * from c_name group by c_name;

        统计分组语法如下: select count(*) from  table group by field;
                                     select count(*) from commodity group by c_type;

        对分组查询结果进行条件限制查询，不能使用WHERE关键字，需要使用HAVING关键字，例如，

```python
select c_inprice from commodity where c_inprice>50 group by c_type;         #(先查询后分组: where...group by)
select c_inprice from commodity group by c_type having avg(c_inprice)>50    #(先分组后查询group by...having)
```

#### ​    连表查询:
        SELECT 字段1,字段2,… FROM table_1 inner join table_2 ON table_1.字段x = table_2.字段y;

```python
select c_name,c_inprice from commodity inner join commoditytype on commodity.c_type = commoditytype.ct_id;    #两表联查

select score_02.stu_num as 学号, stu_name as 姓名, avg(score) as 平均成绩 from score_02 inner join students_02 on \
score_02.stu_num=students_02.stu_num where score_02.stu_num in \                                #二连表多条件查询
(select stu_num from score_02 where score<60 group by score_02.stu_num having count(*)>1) group by score_02.stu_num;  

select sname as 学生姓名,cname as 课程名,score as 成绩 from score_02 inner join students_02 on \
score_02.stu_num=students_02.stu_num inner join course_02 on course_02.cno=score_02.cno;        #三表联查
```

        内外 左右连接查询的区别
|操作符名称|说明|
|----|----|
|inner join|如果两表都没有主外键，返回所有行|
|left join|即使右表中没有匹配，也返回左表所有行|
|right join|即使左表中没有匹配，也返回右表所有行|

        排序的关键字是ORDER BY，后面跟ASC（升序，可以不写默认）或者DESC逆序；
        具体语法如下：
            SELECT field1,field2,... FROM table_name WHERE CONDITION ORDER BY
            fieldn [ACS|DESC];
        例如：                        
            select c_name,c_madein,c_type,c_inprice from commodity order by c_inprice;      
        分页查看
        select * from commodity limit 2,2;        


####     子查询
        对连于接查询确实能将两张表格同时查询输出，用一张表的数据作为条件去查另一张表，我们需要用到子查询了；

#####         单行单例，查询语句：
            select * from table_a where sal > (
            select sal from table_b where name = 'TomCat'
            )
            先从b表中查询出tomcat的售价，那括号内的查询结果一定是单行单例的一个值了，那么再通过这个值去查询出售价大于这个值的所有商品；
            注意：这里的返回值必须是单行单例的值！

#####         单行多列，查询语句：
            select * from table_a where (sal,name) = (
            select sal,name from table_b where name = 'TomCat'
            );
          select xue_score from xuejibiao where (xuehao_id,xue_score)=(select xuehao_id,xue_score from xuejibiao2 where xue_name='梅超风');

            先从b表中查询出tomcat的售价,输出的结果有2个列属于单行多列，分别是售价和名字，
            那么再通过这两个值去查询和这两个值相等的所有商品；
            注意：这里的返回值和查询的列名顺便必须一致！

#####         单列多行：
            select * from table_a where sal IN (select sal from table_b;);         //查询结果在返回字段结果内的
            select * from table_a where sal NOT IN (select sal from table_b;);  //查询结果不在返回字段结果内的
            select * from table_a where sal >=ANY (select sal from table_b;);     //查询结果在返回字段结果内任意满足
            select * from table_a where sal >=ALL (select sal from table_b;);     //查询结果在返回字段结果内全部满足        








