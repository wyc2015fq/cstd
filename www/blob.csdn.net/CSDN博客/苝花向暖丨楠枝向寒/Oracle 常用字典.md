# Oracle 常用字典 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年01月26日 11:37:34[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：74


**约束字典**

----所有用户的约束

select * from all_constraints 

----当前用户的约束

select * from user_constraints;

**索引字典**

----针对索引的信息

select * from user_indexes;

----针对被添加索引的字段的信息

select * from user_ind_columns;


**select * from user_table    查看当前用户下 所有的表**

