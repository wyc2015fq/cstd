
# Oracle的sql1999对sql的支持 - 李昆鹏的博客 - CSDN博客


2018年05月29日 13:22:33[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：115


-----------------------------------------------Oracle的sql1999对sql的支持---------------------------------------------
#### 交叉连接CROSS  JOIN（了解）
交叉连接会产生笛卡尔积
![](https://img-blog.csdn.net/20180529132134325?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 1.  自然连接NATURAL JOIN（了解）
自然连接会自动的分析管理条件进行连接，去掉笛卡尔积。
![](https://img-blog.csdn.net/20180529132141426?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 2.  USING子句，直接管理操作（了解）
![](https://img-blog.csdn.net/2018052913215311?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 3.  ON子句，自己编写连接条件（重要）
On相当于where
![](https://img-blog.csdn.net/20180529132157677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 4.  左连接和右连接LEFT JOIN和RIGHT JOIN（重要）
![](https://img-blog.csdn.net/20180529132201903?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|select * from emp cross join dept; --交叉连接,会产生笛卡尔积
|select * from emp, dept;
|select * from emp natural join dept; --自然连接不会产生笛卡尔积
|select * from emp e,dept d where  e.deptno=d.deptno; --相当于自然连接，除了位置不同
|select * from emp join dept using(deptno)  where dept.deptno=20; --直接管理操作
|select * from emp e, dept d where  e.deptno=d.deptno and d.deptno = 20; --直接管理操作相当于这条语句
|select * from emp e join dept d on  e.deptno = d.deptno and d.deptno = 20; --on相当于where
|--sql 1999 外链接查询为重点掌握知识
|select * from dept d left join emp e on  d.deptno = e.deptno; --sql 1999 外链接-左连接
|select * from dept d ,emp e where  d.deptno=e.deptno(+);
|select * from emp e right join dept d on  e.deptno=d.deptno; --sql 1999 外链接-右连接
|select * from emp e ,dept d where  e.deptno(+) = d.deptno;
|select * from emp e left join emp e1 on  e.mgr=e1.empno; --使用sql 1999 查询出所有员工的上级领导
|select * from emp e, emp e1 where e.mgr =  e1.empno(+);
|

