
# Oracle通用函数 - 李昆鹏的博客 - CSDN博客


2018年05月26日 21:51:59[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：72标签：[Oracle																](https://so.csdn.net/so/search/s.do?q=Oracle&t=blog)个人分类：[Oracle																](https://blog.csdn.net/weixin_41547486/article/category/7679485)


--------------------------------------------Oracle通用函数--------------------------------------------------
1．空值处理nvl
范例：查询所有的雇员的年薪
![](https://img-blog.csdn.net/20180526215106587?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们发现很多员工的年薪是空的，原因是很多员工的奖金是null，null和任何数值计算都是null，这时我们可以使用nvl来处理。
![](https://img-blog.csdn.net/20180526215110950?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.Decode函数
该函数类似if....else if...esle
语法：DECODE(col/expression, [search1,result1],[search2,result2]....[default])
Col/expression:列名或表达式
Search1，search2...:用于比较的条件
Result1, result2...:返回值
如果col/expression和Searchi匹配就返回resulti,否则返回default的默认值
![](https://img-blog.csdn.net/20180526215121856?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：查询出所有雇员的职位的中文名
![](https://img-blog.csdn.net/20180526215129767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**3.case when**
CASE expr WHEN comparison_expr1 THEN return_expr1
[WHENcomparison_expr2 THEN return_expr2
WHENcomparison_exprn THEN return_exprn
ELSE else_expr]
END
selectt.empno,
t.ename,
case
whent.job ='CLERK'then
'业务员'
whent.job ='MANAGER'then
'经理'
whent.job ='ANALYST'then
'分析员'
whent.job ='PRESIDENT'then
'总裁'
whent.job ='SALESMAN'then
'销售'
else
'无业'
end
fromemp t
![](https://img-blog.csdn.net/20180526215139143?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|select * from emp
|select ename,sal*12,nvl(comm,0) from emp  --空值和数值计算最后会得到空，使用nvl(带有空值得列表， 代替空值得数)来将空转换为数值
|select ename,sal*12+nvl(comm,0) from emp  --计算员工的年薪
|select decode(1,1,'我是1',2,'我是2','其他')  from dual --分支查询，第一个参数为要查询的参数，第二个参数是匹配的参数，第三个参数为匹配后的结果
|select ename,
|decode(job,
|'CLERK',
|'业务员',
|'SALESMAN',
|'推销员',
|'MANAGER',
|'经理',
|'ANALYST',
|'分析员',
|'PRESIDENT',
|'总裁',
|'无业')  cjob
|from emp --将员工的工作用中文显示
|select ename,
|case
|when job = 'CLERK' then
|'业务员'
|when job = 'SALESMAN' then
|'推销员'
|when job = 'MANAGER' then
|'经理'
|when job = 'ANALYST' then
|'分析员'
|when job = 'PRESIDENT' then
|'总裁'
|else
|'无业'
|end cjob
|from emp --将员工的工作用中文显示
|


