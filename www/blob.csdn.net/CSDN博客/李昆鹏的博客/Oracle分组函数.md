
# Oracle分组函数 - 李昆鹏的博客 - CSDN博客


2018年05月29日 13:25:00[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：66


----------------------------------------------Oracle分组函数-------------------------------------------------
#### .统计记录数count()
范例：查询出所有员工的记录数
![](https://img-blog.csdn.net/20180529132314388?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
不建议使用count(*)，可以使用一个具体的列以免影响性能。
![](https://img-blog.csdn.net/20180529132320457?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 2.最小值查询min()
范例：查询出来员工最低工资
![](https://img-blog.csdn.net/20180529132327762?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 3.最大值查询max()
范例：查询出员工的最高工资
![](https://img-blog.csdn.net/20180529132335958?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 4.查询平均值avg()
范例：查询出员工的平均工资
![](https://img-blog.csdn.net/20180529132430172?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 5.求和函数sum()
范例：查询出20部门的员工的工资总和
![](https://img-blog.csdn.net/2018052913243516?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|select * from emp;
|select count(*) from emp; --统计记录数
|select count(empno) empnum from emp; --建议使用
|select min(sal) minsal from emp; --查出最少值
|select max(sal) maxsal from emp; --查出最大值
|select avg(sal) avgsal from emp; --查询列表的平均数
|select sum(sal) sumsal from emp where  emp.deptno=20; --查询部门编号为20的工资总和
|


