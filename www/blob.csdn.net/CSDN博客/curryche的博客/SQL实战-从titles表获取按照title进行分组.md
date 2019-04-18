# SQL实战  - 从titles表获取按照title进行分组 - curryche的博客 - CSDN博客





2018年09月13日 16:41:49[curryche](https://me.csdn.net/whwan11)阅读数：47








## 题目

从titles表获取按照title进行分组

时间限制：1秒 空间限制：32768K 热度指数：23672 

 算法知识视频讲解 

题目描述 

从titles表获取按照title进行分组，每组个数大于等于2，给出title以及对应的数目t。 

CREATE TABLE IF NOT EXISTS “titles” ( 
`emp_no` int(11) NOT NULL, 
`title` varchar(50) NOT NULL, 
`from_date` date NOT NULL, 
`to_date` date DEFAULT NULL); 

输入描述: 

无 

输出描述:
title

t

Assistant Engineer

2

Engineer

4

省略

省略

Staff

3

## 代码

```
select title,count(*) as t
from titles
group by title having count(*)>=2;
```




