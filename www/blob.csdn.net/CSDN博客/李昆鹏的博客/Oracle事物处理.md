
# Oracle事物处理 - 李昆鹏的博客 - CSDN博客


2018年05月29日 13:33:54[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：98


----------------------------------------Oracle事物处理----------------------------------------
范例：删除员工7934；
![](https://img-blog.csdn.net/2018052913332031?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从结果上看似乎数据已经删除，但是我们再打开另一个窗口查看发现7934的员工还在
![](https://img-blog.csdn.net/20180529133323551?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
事务处理：所谓的事务管理就是要保证数据的完整性，要么同时成功，要么同时失败
当我们执行完delete语句时，我们去数据库中去查看数据，发现并没有我们删除数据，这是因为oracle的事务对数据库的变更的处理，我们必须做提交事务才能让数据真正的插入到数据库中，在同样在执行完数据库变更的操作后还可以把事务进行回滚，这样就不会插入到数据库。如果事务提交后则不可以再回滚。
提交：commit
回滚：rollback
![](https://img-blog.csdn.net/20180529133328687?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在oracle中会数据库变更会发生锁的情况（此处应用可以解决项目多线程并发带来的数据安全问题）
当两个数据库的连接同时来修改同一数据时，一定会有一连接先修改，另一个连接就会等待直到第一个连接修改完毕再修改
代码示例：
|--增删改操作都要开启事务，事务必须提交我们的数据才会在数据库中真正的变更
|delete from myemp t where t.empno=8001;
|commit; --提交事务
|rollback; --事务没有提交之前都可以回滚，提交了就不能回滚
|--执行update时没有提交事务之前是挂起状态，这时这条数据被锁住了
|update myemp t set t.sal=sal+100 where  t.empno = 7369;
|select * from  myemp for update; --带着事务查询
|


