
# Oracle外链接（左右连接） - 李昆鹏的博客 - CSDN博客


2018年05月26日 21:57:14[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：452


------------------------------------------------Oracle外链接（左右连接）----------------------------------------------------------
1.    右连接
当我们在做基本连接查询的时候，查询出所有的部门下的员工，我们发现编号为40的部门下没有员工，但是要求把该部门也展示出来，我们发现上面的基本查询是办不到的
![](https://img-blog.csdn.net/20180526215502798?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
使用(+)表示左连接或者右连接，当(+)在左边表的关联条件字段上时是左连接，如果是在右边表的关联条件字段上就是右连接。

范例：查询出所有员工的上级领导
分析：我们发现使用我们以前的做法发现KING的上级领导没有被展示，我们需要使用左右连接把他查询出来
![](https://img-blog.csdn.net/20180526215602974?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|select * from dept
|select * from dept order by deptno asc
|select * from emp e, dept d where  e.deptno = d.deptno order by d.deptno asc
|--查询出每一个部门的员工
|--两张表做连接查询时其中一张表要查询全量数据（不会因为另一张的表的数据的关联而被筛选掉）
|--在两张表关联的时候非全量表的关联字段后面加上(+)就可以做外链接查询
|--左外链接查询的时候全量表在右边就是右连接
|select * from emp e, dept d where e.deptno(+)  = d.deptno;
|--在做外链接查询的时候全量表在左端就是左连接，查询表名的位置和关联时候位置最好保持一致
|select * from dept d, emp e where  d.deptno = e.deptno(+);
|--查询员工编号、姓名和上级领导编号、姓名
|select e.empno, e.ename, e1.empno,  e1.ename from emp e, emp e1 where e.mgr = e1.empno(+);
|


