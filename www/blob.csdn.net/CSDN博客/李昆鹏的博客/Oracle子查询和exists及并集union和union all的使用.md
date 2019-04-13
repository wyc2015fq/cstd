
# Oracle子查询和exists及并集union和union all的使用 - 李昆鹏的博客 - CSDN博客


2018年05月29日 13:31:20[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：507


-----------------------------------------Oracle子查询和exists及并集union和union all的使用-----------------------------------------------
子查询：在一个查询的内部还包括另一个查询，则此查询称为子查询。
Sql的任何位置都可以加入子查询。
范例：查询比7654工资高的雇员
分析：查询出7654员工的工资是多少，把它作为条件
![](https://img-blog.csdn.net/20180529132940661?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所有的子查询必须在“（）”中编写
子查询在操作中有三类：
单列子查询：返回的结果是一列的一个内容
单行子查询：返回多个列，有可能是一个完整的记录
多行子查询：返回多条记录
范例：查询出比雇员7654的工资高，同时从事和7788的工作一样的员工
![](https://img-blog.csdn.net/2018052913295315?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：要求查询每个部门的最低工资和最低工资的雇员和部门名称
![](https://img-blog.csdn.net/20180529132958276?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在返回多条记录的子查询可以把它的结果集当做一张表，给起个别名， 如图中的a。
范例：查询出来每个部门最低工资的员工
![](https://img-blog.csdn.net/20180529133005952?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Exists和not exists关键字的用法：
exists（sql返回结果集为真）
notexists(sql不返回结果集为真）
范例：查询出有员工的部门有哪些？
![](https://img-blog.csdn.net/20180529133024337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180529133032545?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180529133039966?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Union和Union All的用法
Union：对两个结果集进行并集操作，不包括重复行。
![](https://img-blog.csdn.net/20180529133048764?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Union All：对两个结果集进行并集操作，包括重复行。
![](https://img-blog.csdn.net/20180529133054806?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|--查询出比员工号7654工资高的员工
|select *
|from emp t
|where t.sal > (select e.sal from emp e  where e.empno = 7654);
|--查询出比雇员7654的工资高，同时从事和7788的工作一样的员工
|select *
|from emp e2
|where e2.sal > (select e.sal from emp e  where e.empno = 7654)
|and e2.job = (select e1.job from emp e1 where e1.empno = 7788);
|--要求查询每个部门的最低工资和最低工资的雇员和部门名称
|select t.sal, t.deptno, t.ename, d.dname
|from emp t,
|(select min(e.sal) minsal, e.deptno from emp e group by e.deptno) a,
|dept d
|where t.deptno = a.deptno
|and t.sal = a.minsal
|and t.deptno = d.deptno;
|--查询出所有和每个部门最低工资的员工相等的人
|select * from emp t where t.sal in  (select min(e.sal) from emp e group by e.deptno);
|--查询出所有员工的部门有哪些？
|-- in关键字尽量要少用，因为性能比较低，可以使用exists来代替性能比较高。
|select * from dept t where t.deptno in  (select distinct deptno from emp);
|--用exists做外部关联查询出有人的部门
|select * from dept t where exists (select  * from emp e where t.deptno = e.deptno);
|--用not exists查询出没有人的部门
|select * from dept t where not exists  (select * from emp e where t.deptno = e.deptno);
|--exists()子查询的记录数是0则整个表达式是false，如果大于0为true，exists子查询一般是要和外层查询关联的
|select * from emp where exists(select *  from dept d where d.deptno = 50);
|
Union和union all的代码示例：
|--union把两个集合做并集的时候会把重复的数据去掉一份
|select * from emp where sal>1000
|union
|select * from emp where sal>2000;
|--union all把两个集合做并集的时候不会去重
|select * from emp where sal>1000
|union all
|select * from emp where sal>2000;
|--并集时要合并的列类型必须一致，列名可以不一致，两个合并的并集的列数必须一致
|select t.ename ,t.empno, t.job from emp t  where t.empno = 7499
|union
|select t.ename ,t.empno ,t.job from emp t  where t.empno = 7788;
|


