
# Oracle分组统计 - 李昆鹏的博客 - CSDN博客


2018年05月29日 13:27:49[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：2871


---------------------------------------------------Oracle分组统计---------------------------------------------------------
分组统计需要使用GROUP BY来分组
语法：语法：SELECT * |列名FROM 表名 {WEHRE 查询条件}   {GROUP BY分组字段，分组字段1…} ORDERBY 列名1 ASC|DESC，列名2...ASC|DESC
范例：查询每个部门的人数
![](https://img-blog.csdn.net/2018052913253044?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：查询出每个部门的平均工资
![](https://img-blog.csdn.net/20180529132537660?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果我们想查询出来部门编号，和部门下的人数
![](https://img-blog.csdn.net/20180529132600482?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们发现报了一个ORA-00937的错误
**注意：**
**1.****如果使用分组函数，SQL****只可以把GOURPBY分组条件字段和分组函数查询出来，不能有其他字段。**
**2.****如果使用分组函数，不使用GROUP BY****只可以查询出来分组函数的值**
![](https://img-blog.csdn.net/20180529132626823?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：按部门分组，查询出部门名称和部门的员工数量
![](https://img-blog.csdn.net/20180529132634119?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：查询出部门人数大于5人的部门
分析：需要给count(ename)加条件，此时在本查询中不能使用where，可以使用HAVING
Having在group by后面作用在分组函数上
![](https://img-blog.csdn.net/20180529132646243?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：查询出部门平均工资大于2000的部门
![](https://img-blog.csdn.net/20180529132653857?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：显示非销售人员工作名称以及从事同一工作的员工的月工资的总和，并且要满足从事同一工作月工资总和大于5000，结果按月工资总和的升序排列。
分析：
1.    查询出非销售人员
![](https://img-blog.csdn.net/20180529132709434?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.    以步骤1为基础按着工作分组求工资的总和
![](https://img-blog.csdn.net/20180529132714646?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.    以步骤2为基础查询出月工资总和大于5000的工作
![](https://img-blog.csdn.net/20180529132718804?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.    按着月工资的总和的升序排列
![](https://img-blog.csdn.net/20180529132722441?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**注意：只有分组条件在结果集中是重复的分组才有意义。**
代码示例：
|select count(deptno), deptno from emp  group by deptno order by deptno asc; --分组统计人数
|select avg(sal) salavg, deptno from emp  group by deptno; --求出各个部门的平均工资
|/* 会报错，因为没有group  by分组所以统计函数统计的是整张表的数据
|我们却非要强加在部门编号上是不对的，所以想要查询出来部门下的统计信息必须要分组
|--   14, 30
|--       20
|--       10 */
|select count(*), deptno from emp;
|select count(*), d.deptno,d.dname, d.loc  from emp e, dept d where e.deptno=d.deptno group by d.deptno, d.dname,d.loc;  --查询各部门人数和各部门名称
|select count(*),deptno,job,ename from emp  group by deptno,job,ename order by deptno;   --查询各部门中每一个人，
|--查询人数大于5的部门,having分组条件查询不能使用别名
|select count(*), d.deptno,d.dname, d.loc  from emp e, dept d where e.deptno=d.deptno group by d.deptno, d.dname,d.loc  having count(*)>5;
|--查询部门平均工资大于2000
|select avg(sal), deptno from emp group by  deptno having avg(sal) > 2000;
|select avg(sal) avgsal, deptno from emp  where deptno <> 20 group by deptno;
|--范例：显示非销售人员工作名称以及从事同一工作的员工的月工资的总和，并且要满足从事同一工作月工资总和大于5000，结果按月工资总和的升序排列。
|select sum(sal), e.job
|from emp e
|where e.job <> 'SALESMAN'
|group by e.job
|having sum(sal) > 5000
|order by sum(sal);
|


