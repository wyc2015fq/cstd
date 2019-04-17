# hive操作总结 - 快来学习鸭～～～ - CSDN博客





2018年11月06日 12:59:39[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：76








1. hive表中字段很多，需要从大量字段中去除一个或者几个，写出所有的字段很不美观，可以使用正则完成

将hive.support.quoted.identifiers设置为None，就可以使用正则表达式来取表的字段

```
set hive.support.quoted.identifiers=None;
select `(search_word|user_id)?+.+` from user_action_table;
```

2. 建表

```
create table if not exists table(
    user_id bigint comment '用户id',
    goods_id bigint comment '商品id'
 ) comment '交易表'
 partitioned by (par string comment '分区键为par')
 stored as orc;
```

3. 添加一列
`alter table user_action_table add columns ( rec_source string comment '商品来源')`
4.将数据插入分区

```
insert overwrite table user_action_table partition (par='1024')
select * from other_table
```

5.写子查询，hive在写复杂语句的时候可以将起拆分成子语句，分开写，再合起来

```
with tmp_table as (
select count(*) from table group by user_id),

tmp_table2 as (
select count(*) from table group by goods_id)
```

6.排序

```
select
user_id,
goods_id,
row_number() over (partition by user_id order by action_num desc ) as rank_number
from
user_action_table
```

7.hive使用笛卡尔乘积，最好不要用，找到其他方法代替，很慢

```
set hive.strict.checks.cartesian.product = false;
set hive.mapred.mode = 'nonstrict' ;

select T1.*, T2.* from

(select * from table1) T1

cross join

(select * from table2) T2

on 1=1;
```

8. 求比例，比如求当前行的值在该列所有值的和中的占比.善用开窗函数，partition by 后的字段类似 group by 的字段。

```
select
user_id,
goods_id,
action_num
action_num*1.00/sum(action_num) over (partition by user_id) as user_percent
from tmp_table
```

9. 过滤表1在表2中出现过的数据

```
select a.user_id,a.goods_id from 
(select user_id, goods_id  from table_1) as a
left join
(select user_id, goods_id from table_2) as b
on a.user_id = b.user_id and a.goods_id = b.goods_id
where b.user_id is null and b.goods_id is null
```





