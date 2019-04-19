# 读懂 MySQL 执行计划 - 文章 - 伯乐在线
原文出处： [鞠骞](http://www.andyqian.com/2017/11/01/database/MySQLplain/)
### 前言
在之前的面试过程中,问到执行计划,有很多童鞋不知道是什么?甚至将执行计划与执行时间认为是同一个概念。今天我们就一起来了解一下执行计划到底是什么?有什么用途?
### 执行计划是什么?
执行计划，简单的来说，是SQL在数据库中执行时的表现情况,通常用于SQL性能分析,优化等场景。在MySQL使用 **explain** 关键字来查看SQL的执行计划。如下所示:


```
//1. 查询t_base_user
select * from t_base_user where name="andyqian";
//2. 查看上述语句的执行计划
explain select * from t_base_user where name="andyqian";
```
执行查看上述2语句后,我们可以得出以下执行计划结果
|id|select_type|table|type|possible_kes|key|key_len|ref|rows|Extra|
|----|----|----|----|----|----|----|----|----|----|
| | | | | | | | | | |
1SIMPLEt_base_userALL1Using where
上面执行计划是什么意思呢？有什么参考价值呢?
上面这个执行计划给到的信息是: 这个结果通过一个简单的语句全表扫描,共扫描1行,使用where条件在t_base_user表中筛选出的。发现该语句并没有走索引,为什么是这样的呢？别急，我们紧接着看下一节。
### 读懂执行计划
通过上面，我们知道了什么是执行计划,也看到了执行计划到底是什么东西,现在我们来具体了解一下,MySQL执行计划中,每个属性代表的是什么意思?
|id|select_type|table|type|possible_kes|key|key_len|ref|rows|Extra|
我们一一来介绍,并说明每个属性有哪些可选值,以及每个可选值的意思。
- **id**
表示查询中select操作表的顺序,按顺序从大到依次执行
- **select_type :**
该表示选择的类型,可选值有: SIMPLE(简单的),
- **type :**
该属性表示访问类型,有很多种访问类型。
最常见的其中包括以下几种: ALL(全表扫描), index(索引扫描),range(范围扫描),ref (非唯一索引扫描),eq_ref(唯一索引扫描,),(const)常数引用, 访问速度依次由慢到快。**其中 :** range(范围)常见与 between and …, 大于 and 小于这种情况。
**提示 :** 慢SQL是否走索引,走了什么索引,也就可以通过该属性查看了。
- **table :**
表示该语句查询的表
- **possible_keys :**
顾名思义,该属性给出了,该查询语句,可能走的索引,(如某些字段上索引的名字)这里提供的只是参考,而不是实际走的索引,也就导致会有possible_Keys不为null,key为空的现象。
- **key :**
显示MySQL实际使用的索引,其中就包括主键索引(PRIMARY),或者自建索引的名字。
- **key_len :**
表示索引所使用的字节数，
- **ref :**
连接匹配条件,如果走主键索引的话,该值为: const, 全表扫描的话,为null值
- **rows :**
扫描行数,也就是说，需要扫描多少行,采能获取目标行数,一般情况下会大于返回行数。通常情况下,rows越小,效率越高, 也就有大部分SQL优化，都是在减少这个值的大小。注意:  理想情况下扫描的行数与实际返回行数理论上是一致的,但这种情况及其少,如关联查询,扫描的行数就会比返回行数大大增加)
- **Extra**
这个属性非常重要,该属性中包括执行SQL时的真实情况信息,如上面所属,使用到的是”using where”，表示使用where筛选得到的值,常用的有:
“Using temporary”: 使用临时表 “using filesort”: 使用文件排序
看到这里,我们应该已经发现,在第一步中,我们的这条SQL


```
select * from t_base_user where name="andyqian";
```
是没有走索引的,而且还是全表扫描,在数据量少的情况下,问题还不会特别突出,如果数据量比较大,这可是个会造成生产事故的慢查询哦，现在我们改造一下,将name字段添加上索引,


```
# 添加索引
alter table t_base_user add index idx_name(name);
```
看看它的执行计划是怎样的。
|id|select_type|table|type|possible_kes|key|key_len|ref|rows|Extra|
|----|----|----|----|----|----|----|----|----|----|
| | | | | | | | | | |
1SIMPLEt_base_userrefidx_nameidx_name93cons1Using where
你看,现在已经走idx_name索引了,其type从All(全表扫描)到ref(非唯一索引了)，别看就只有这一点点小区别,在大数据量的时候,可是会起大作用的哦。
### 数据结
本文中演示的数据结构如下:


```
# 创建表  
create table t_base_user(
oid bigint(20) not null primary key auto_increment,
name varchar(30) null comment "name",
email varchar(30) null comment "email",
age int null comment "age",
telephone varchar(30) null comment "telephone",
status tinyint(4) null comment "0  无效 1 有效",
created_at datetime null comment "",
updated_at datetime null comment ""
)
## 新增记录:
insert into t_base_user(name,email,age,telephone,created_at,updated_at)values("andyqian","andytohome@gmail.com",20,"15608411",now(),now());
)
```
### 最后
一个好的数据库表设计,从一开始就应该考虑添加索引,而不是到最后发现慢SQL了,影响业务了,才来补救。其实我在工作经历当中，由于新建表,或新加字段后,忘记添加索引也造成了多次生产事故,记忆犹新!!!
其实新建索引也是有一定的原则的,建什么索引,建在哪些字段上,这里面还有不少知识呢，下一篇文章写,尽请期待吧!
