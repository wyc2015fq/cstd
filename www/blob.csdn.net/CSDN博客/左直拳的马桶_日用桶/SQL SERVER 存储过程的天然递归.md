# SQL SERVER 存储过程的天然递归 - 左直拳的马桶_日用桶 - CSDN博客
2012年10月24日 14:52:11[左直拳](https://me.csdn.net/leftfist)阅读数：2266
有几次，我编写或修改存储过程的时候，为图方便，在末尾随手写上 执行这个存储过程 的语句，比如
```
ALTER PROCEDURE prd_GetData
	@Top INT = 10;
AS
	SELECT * FROM [dbo].Table1  ORDER BY ID DESC;
END
EXEC [dbo].prd_GetData
```
本来应该用鼠标选定最后一行，指定它来执行，但一时手颤，没有选上就按下运行，结果出来好多个结果，最后系统警告太多层了，SQL自动退出。
究其原因，是系统将 
`EXEC [dbo].prd_GetData`
这句也当成了存储过程本身的一个语句，形成递归，因此无限循环了。
存储过程天然归 
调足一百八十龟
