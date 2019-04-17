# Hive表联合查询语句参考 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月22日 15:25:36[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2200
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









hive语句不是很熟悉，对表联合这个基本的都要找半天资料，这里写下，作为以后的参考：

select a.time_stamp,a.acc_nbr,b.bts_name from (select * from default.tableA where  start_date >=20170612 and start_date<=20170618 and acc_nbr ="18988888888" a left join (select * from default.tableB) b on (a.bsc=b.bsc_id and a.bts=b.bts_id and a.call_latn=b.latn);





hive表连接语法：



```
join_table:
    table_reference JOIN table_factor [join_condition]
  | table_reference {LEFT|RIGHT|FULL} [OUTER] JOIN table_reference join_condition
  | table_reference LEFT SEMI JOIN table_reference join_condition
  | table_reference CROSS JOIN table_reference [join_condition] (as of Hive 0.10)

table_reference:
    table_factor
  | join_table

table_factor:
    tbl_name [alias]
  | table_subquery alias
  | ( table_references )

join_condition:
    ON equality_expression ( AND equality_expression )*

equality_expression:
    expression = expression
```


语法还是要多练习，工具才能熟练掌握。





