
# Oracle多表查询 - 李昆鹏的博客 - CSDN博客


2018年05月26日 21:54:10[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：61标签：[Oracle																](https://so.csdn.net/so/search/s.do?q=Oracle&t=blog)个人分类：[Oracle																](https://blog.csdn.net/weixin_41547486/article/category/7679485)


------------------------------------------------------Oracle多表查询--------------------------------------------------------
### 多表连接基本查询
使用一张以上的表做查询就是多表查询
语法：     SELECT{DISTINCT} *|列名.. FROM 表名 别名，表名1别名
{WHERE 限制条件 ORDER BY 排序字段 ASC|DESC...}
范例：查询员工表和部门表
![](https://img-blog.csdn.net/20180526215252103?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们发现产生的记录数是56条，我们还会发现emp表是16条，dept表是4条，56正是emp表和dept表的记录数的乘积，我们称其为笛卡尔积。
如果多张表进行一起查询而且每张表的数据很大的话笛卡尔积就会变得非常大，对性能造成影响，想要去掉笛卡尔积我们需要关联查询。
在两张表中我们发现有一个共同的字段是depno，depno就是两张表的关联的字段，我们可以使用这个字段来做限制条件，两张表的关联查询字段一般是其中一张表的主键，另一张表的外键。
![](https://img-blog.csdn.net/20180526215258971?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
关联之后我们发现数据条数是14条，不在是56条。
多表查询我们可以为每一张表起一个别名
![](https://img-blog.csdn.net/20180526215306766?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：查询出雇员的编号，姓名，部门的编号和名称，地址
![](https://img-blog.csdn.net/2018052621531398?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：查询出每个员工的上级领导
分析：emp表中的mgr字段是当前雇员的上级领导的编号，所以该字段对emp表产生了自身关联，可以使用mgr字段和empno来关联
![](https://img-blog.csdn.net/2018052621532093?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例:在上一个例子的基础上查询该员工的部门名称
分析：只要在上一个例子基础上再加一张表的关联，使用deptno来做关联字段即可
![](https://img-blog.csdn.net/20180526215326853?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：查询出每个员工编号，姓名，部门名称，工资等级和他的上级领导的姓名，工资等级
selecte.empno,
e.ename,
decode(s.grade,
1,'一级',
2,'二级',
3,'三级',
4,'四级',
5,'五级') grade,
d.dname,
e1.empno,
e1.ename,
decode(s1.grade,
1,'一级',
2,'二级',
3,'三级',
4,'四级',
5,'五级') grade
fromemp e, emp e1, dept d, salgrade s,salgrade s1
wheree.mgr = e1.empno
ande.deptno = d.deptno
ande.salbetweens.losalands.hisal
ande1.salbetweens1.losalands1.hisal
![](https://img-blog.csdn.net/20180526215338227?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|select|*|from|emp e, dept d|where|e.deptno = d.deptno|--|多变联动查询时如果没有关联的查询条件，查询就会发生笛卡尔积规律
|select|*|from|emp, dept|where|emp.deptno = dept.deptno|--|我们也可以用表名来区分相同的列名，在实际项目中建议使用别名来做区分
|select|e.empno, e.ename, d.deptno, d.dname, d.loc
|from|emp e, dept d
|where|e.deptno =  d.deptno|--|查询员工的员工编号，姓名，部门编号，名称，地址
|--|查询出每个员工的上级领导
|select|e.empno, e.ename,  e1.empno, e1.ename
|from|emp e, emp e1
|where|e.mgr = e1.empno
|--|查询出每一个员工的上级领导和员工部门名称
|select|e.empno, e.ename,  e1.empno, e1.ename, d.dname
|from|emp e, emp e1,  dept d
|where|e.mgr = e1.empno|and|d.deptno =  e.deptno
|--|查询出每个员工编号，姓名，部门名称，工资等级和他的上级领导的姓名，工资等级
|select|e.empno,
|e.ename,
|d.dname,
|decode|(s.grade,
|1|,
|'|一级|'|,
|2|,
|'|二级|'|,
|3|,
|'|三级|'|,
|4|,
|'|四级|'|,
|5|,
|'|五级|'|,
|'|无级|'|) esalgrade,
|e1.ename,
|decode|(s1.grade,
|1|,
|'|一级|'|,
|2|,
|'|二级|'|,
|3|,
|'|三级|'|,
|4|,
|'|四级|'|,
|5|,
|'|五级|'|,
|'|无级|'|) e1salgrade
|from|emp e, dept d,  salgrade s, emp e1, salgrade s1
|where|e.deptno =  d.deptno
|and|e.sal|between|s.losal|and|s.hisal
|and|e.mgr = e1.empno
|and|e1.sal|between|s1.losal|and|s1.hisal
|


