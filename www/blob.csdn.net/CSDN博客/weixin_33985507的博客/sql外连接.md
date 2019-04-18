# sql外连接 - weixin_33985507的博客 - CSDN博客
2010年07月29日 10:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
[http://hi.baidu.com/69hwj/blog/item/a63eaadd8ca126ea76c638a5.html](http://hi.baidu.com/69hwj/blog/item/a63eaadd8ca126ea76c638a5.html)
在讲外连接之前，先举例介绍内连接，也就是一般的相等连接。
select * from a, b where a.id = b.id;
对于外连接，Oracle中可以使用“(+)”来表示，9i可以使用LEFT/RIGHT/FULL OUTER JOIN，下面将配合实例一一介绍。
1. LEFT OUTER JOIN：左外关联
SELECT e.last_name, e.department_id, d.department_name
                 FROM employees e
                 LEFT OUTER JOIN departments d
                 ON (e.department_id = d.department_id);
等价于
SELECT e.last_name, e.department_id, d.department_name
                 FROM employees e, departments d
                 WHERE e.department_id=d.department_id(+);
结果为：所有员工及对应部门的记录，包括没有对应部门编号department_id的员工记录。
**因该理解为:选择所有的员工记录,加上"department_name"字段,如果没有该字段的,试用Null来填补,目的是为了查看所有员工的信息.**
2. RIGHT OUTER JOIN：右外关联
SELECT e.last_name, e.department_id, d.department_name
                 FROM employees e
                 RIGHT OUTER JOIN departments d
                 ON (e.department_id = d.department_id);
等价于
SELECT e.last_name, e.department_id, d.department_name
                 FROM employees e, departments d
                 WHERE e.department_id(+)=d.department_id;
结果为：所有员工及对应部门的记录，包括没有任何员工的部门记录。
3. FULL OUTER JOIN：全外关联
SELECT e.last_name, e.department_id, d.department_name
                 FROM employees e
                 FULL OUTER JOIN departments d
                 ON (e.department_id = d.department_id);
结果为：所有员工及对应部门的记录，包括没有对应部门编号department_id的员工记录和没有任何员工的部门记录。
==============
left主表全选right从表全选
(+)在从表主键上，表示left
full outer join 没有简化模式
