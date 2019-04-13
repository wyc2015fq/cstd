
# Oracle数据库变更 - 李昆鹏的博客 - CSDN博客


2018年05月29日 13:32:52[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：223


-----------------------------------------------Oracle数据库变更------------------------------------------------
为了保存原始emp的信息保存，我们复制一份emp表
Create table myemp as select * from emp;
### 1.  Insert（增加）
语法：
INSERT INTO表名[(列名1，列名2，...)]VALUES(值1，值2，...)
标准写法
![](https://img-blog.csdn.net/20180529133159476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
简单写法（不建议）
INSERT INTO 表名VALUES(值1，值2，...)
insertinto myemp values(7789, '张三', '开发', 7839, to_date('1992-10-22','yyyy-MM-dd'), 2000, 200, 10);
注意：使用简单的写法必须按照表中的字段的顺序来插入值，而且如果有为空的字段使用null
insert intomyemp values(7790, '张三', '开发', null, to_date('1992-10-22','yyyy-MM-dd'), 2000, null, 10);

### 2.  update（修改）
全部修改：UPDATE 表名 SET 列名1=值1，列名2=值2，....
局部修改：UPDATE 表名 SET 列名1=值1，列名2=值2，....WHERE 修改条件；
全部更新
![](https://img-blog.csdn.net/20180529133212968?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
局部更新
![](https://img-blog.csdn.net/20180529133218840?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
把张三的领导取消
![](https://img-blog.csdn.net/201805291332245?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 3.  delete（删除）
语法 : DELETE FROM 表名  WHERE   删除条件;
![](https://img-blog.csdn.net/20180529133229774?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在删除语句中如果不指定删除条件的话就会删除所有的数据
代码示例：
|--复制表单
|Create   table myemp as select * from emp;
|select * from myemp;
|--向表单中插入一个员工，标准写法，建议使用
|insert into myemp
|(empno, ename, job, mgr, hiredate, sal, comm, deptno)
|values
|(8000,
|'KOBE',
|'SPORT',
|7369,
|to_date('1989-12-30', 'yyyy-mm-dd'),
|1000,
|100,
|10);
|--第二种插入方法，不用写列名，但是插入的数据，顺序必须跟列表保持一致，全部写全，如果有些数据没有可以用null来代替
|insert into myemp  values(8001,'CARTER', 'SPORT', 7369,  sysdate, 2000, 200, 10);
|--update修改数据,set后面必须要给值，如果不想修改任何值可以把值修改为原来的值
|update myemp t set t.ename='smith',  t.sal=1000 where t.empno=7369;
|update myemp t set t.empno=7369 where  t.empno =7369; --不做任何修改
|update myemp t set t.sal=sal+100 where  t.empno in(7369,7499); --批量修改数据
|--delete删除数据
|delete from myemp where ename='kobe';
|delete   myemp where ename='kobe';--Oracle中from关键字可以省略的，mysql中不行，而且mysql删除不能有表别名
|select * from myemp;
|


