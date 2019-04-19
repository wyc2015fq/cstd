# 学习笔记——SQL SERVER的递归 - 左直拳的马桶_日用桶 - CSDN博客
2014年03月10日 18:21:42[左直拳](https://me.csdn.net/leftfist)阅读数：1516
SQL SERVER似乎天然具有支持递归的属性。
1、比如说，有几次，我编写或修改存储过程的时候，为图方便，在末尾随手写上 执行这个存储过程 的语句，比如
**[sql]**[view plain](http://blog.csdn.net/leftfist/article/details/8106973#)[copy](http://blog.csdn.net/leftfist/article/details/8106973#)
- ALTERPROCEDURE prd_GetData  
-     @TopINT = 10;  
- AS
- SELECT * FROM [dbo].Table1  ORDERBY ID DESC;  
- END
- 
- EXEC [dbo].prd_GetData  
本来应该用鼠标选定最后一行，指定它来执行，但一时手颤，没有选上就按下运行，结果出来好多个结果，最后系统警告太多层了，SQL自动退出。
究其原因，是系统将 
**[sql]**[view plain](http://blog.csdn.net/leftfist/article/details/8106973#)[copy](http://blog.csdn.net/leftfist/article/details/8106973#)
- EXEC [dbo].prd_GetData  
这句也当成了存储过程本身的一个语句，形成递归，因此无限循环了。
2、又比如说，采用select 变量=变量 + 某字段... from 表 的方式，这个变量会自动加上所有符合条件的记录的某字段。
详见 [http://blog.csdn.net/leftfist/article/details/9043863](http://blog.csdn.net/leftfist/article/details/9043863)
3、刚刚看了一篇文章，说可以用WITH AS语句来实现。如：
with w_Storage_Depository as
(
select DID,DName,PID from Storage_Depository where DID='A-1-1'
union all
select A.DID,A.DName,A.PID from Storage_Depository A,
w_Storage_Depository B where A.DID=B.PID
)
select * from w_Storage_Depository
详见：[http://www.lmwlove.com/ac/ID748](http://www.lmwlove.com/ac/ID748)
