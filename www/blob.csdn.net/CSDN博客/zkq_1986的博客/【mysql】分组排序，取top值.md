# 【mysql】分组排序，取top值 - zkq_1986的博客 - CSDN博客





2018年12月05日 14:05:08[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：76








分组排序，取top值

select * from (select id,row1,ROW_NUMBER() over (partition by id order by row1 desc) as rownum from admin.a)as s where s.rownum<=5;



