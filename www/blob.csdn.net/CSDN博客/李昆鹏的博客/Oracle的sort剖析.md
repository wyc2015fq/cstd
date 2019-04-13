
# Oracle的sort剖析 - 李昆鹏的博客 - CSDN博客


2018年05月26日 21:42:57[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：372


---------------------------------------Oracle的sort剖析----------------------------------------------
## 对结果集排序
在sql中可以使用ORDER BY对查询结果进行排序
语法：SELECT * |列名FROM 表名 {WEHRE 查询条件}ORDER BY列名1 ASC|DESC，列名2...ASC|DESC
范例：查询雇员的工资从低到高
分析：ORDER BY 列名 默认的排序规则是升序排列，可以不指定ASC，如果按着降序排列必须指定DESC
![](https://img-blog.csdn.net/20180526214153149?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180526214204773?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果存在多个排序字段可以用逗号分隔
![](https://img-blog.csdn.net/20180526214212128?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注意ORDER BY语句要放在sql的最后执行。
代码示例：
|select  * from emp order by sal --|默认是从低到高排序
|select  * from emp order by sal asc --|从低到高排序
|select  * from emp order by sal desc --|按照从高到低排序
|select  * from emp order by sal desc,hiredate desc --|如果工资相同，就按入职日期倒序排列
|

