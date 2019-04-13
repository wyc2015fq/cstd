
# Oracle的视图view精讲 - 李昆鹏的博客 - CSDN博客


2018年06月06日 21:37:57[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：36标签：[Oracle																](https://so.csdn.net/so/search/s.do?q=Oracle&t=blog)个人分类：[Oracle																](https://blog.csdn.net/weixin_41547486/article/category/7679485)


-------------------------------------Oracle的视图view精讲--------------------------------------
视图就是封装了一条复杂查询的语句。
语法1.：CREATE VIEW 视图名称 AS 子查询
范例：建立一个视图，此视图包括了20部门的全部员工信息
createviewempvd20asselect*fromemp twheret.deptno =20
视图创建完毕就可以使用视图来查询，查询出来的都是20部门的员工
![](https://img-blog.csdn.net/20180606213629126?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
语法2：CREATE OR REPLACE VIEW 视图名称 AS 子查询
如果视图已经存在我们可以使用语法2来创建视图，这样已有的视图会被覆盖。
createorreplaceviewempvd20asselect*fromemp twheret.deptno =20
那么视图可以修改吗？
![](https://img-blog.csdn.net/20180606213640315?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018060621364828?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们尝试着修改视图但是发现是视图所查询的表的字段值被修改了。所以我们一般不会去修改视图。
我们可以设置视图为只读。
语法3：CREATE OR REPLACE VIEW 视图名称 AS 子查询 WITH READ ONLY
createorreplaceviewempvd20asselect*fromemp twheret.deptno =20withreadonly
![](https://img-blog.csdn.net/20180606213656193?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**创建视图需要权限比较大，所以我们使用管理员身份登录**
**在pl/sql中添加用户**
第一步
![](https://img-blog.csdn.net/20180606213703688?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
第二步
![](https://img-blog.csdn.net/20180606213709754?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
第三步
![](https://img-blog.csdn.net/20180606213713588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|select * from dept where deptno = 20;
|grant connect, resource, dba to scott; --在管理员用户下给予scott用户权限
|create view view_d20 as select * from emp  where deptno=20; --创建部门编号为20的视图,
|select * from view_d20; --查询视图
|drop view view_d20; --删除视图
|--创建视图时，查询的sql列名不能重复
|create view view_grade as  select e.empno,
|e.ename,
|d.dname,
|decode(s.grade,
|1,
|'一级',
|2,
|'二级',
|3,
|'三级',
|4,
|'四级',
|5,
|'五级',
|'无级')  esalgrade,
|e1.ename enamel,
|decode(s1.grade,
|1,
|'一级',
|2,
|'二级',
|3,
|'三级',
|4,
|'四级',
|5,
|'五级',
|'无级')  e1salgrade
|from emp e, dept d, salgrade s, emp e1, salgrade s1
|where e.deptno = d.deptno
|and e.sal between s.losal and s.hisal
|and e.mgr = e1.empno
|and e1.sal between s1.losal and s1.hisal；
|select * from view_grade;
|--创建视图或者覆盖视图
|create or replace view view_d20 as select  * from myemp where myemp.deptno=20;
|--修改视图,修改视图其实是修改视图对应的表的数据，视图不建议修改
|update view_d20 t set t.ename = '史密斯' where t.empno=7369;
|--下面两条查询语句结果一样
|select * from view_d20;
|select * from myemp;
|--创建一个只能读取不能修改的视图
|create or replace view view_d20 as select  * from myemp where myemp.deptno=20 with read only;
|


