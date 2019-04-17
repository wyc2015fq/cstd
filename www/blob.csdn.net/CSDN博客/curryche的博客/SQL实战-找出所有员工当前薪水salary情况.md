# SQL实战 - 找出所有员工当前薪水salary情况 - curryche的博客 - CSDN博客





2018年09月13日 17:07:34[curryche](https://me.csdn.net/whwan11)阅读数：46








## 题目

找出所有员工当前薪水salary情况 

时间限制：1秒 空间限制：32768K 热度指数：32511 

 算法知识视频讲解 

题目描述 

找出所有员工当前(to_date=’9999-01-01’)具体的薪水salary情况，对于相同的薪水只显示一次,并按照逆序显示 

CREATE TABLE `salaries` ( 
`emp_no` int(11) NOT NULL, 
`salary` int(11) NOT NULL, 
`from_date` date NOT NULL, 
`to_date` date NOT NULL, 

PRIMARY KEY (`emp_no`,`from_date`)); 

输入描述: 

无 

输出描述:
salary

94692

94409

88958

88070

74057

72527

59755

43311

25828

## 代码

```
select distinct salary
from salaries
where to_date='9999-01-01'
order by salary DESC;
```




