# mysql篇第九课：查询语句(二) - PeterBishop - CSDN博客





2018年11月23日 16:59:28[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：11
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课继续学查询语句



  排序查询:



语法：

select

         要查询的东西

from

         表

where

         条件



order by 排序的字段|表达式|函数|别名 【asc|desc】



演示一下:



Demo 01:

-- 根据部门id排序查询所有员工

select * from tbl_employee ORDER BY d_id asc;



结果:

![](https://img-blog.csdnimg.cn/2018112316582537.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



asc是升序，desc是降序







