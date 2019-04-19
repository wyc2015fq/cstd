# CLR程序集里的 ExecuteScalar() 和 @@IDENTITY - 左直拳的马桶_日用桶 - CSDN博客
2009年06月11日 17:30:00[左直拳](https://me.csdn.net/leftfist)阅读数：1165标签：[sqlserver																[insert																[c#](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=sqlserver&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
象这样的语句
command.CommandText = “INSERT ……；SELECT @@IDENTITY;”;
int
 iRe = (int)command.ExecuteScalar();
在C#里面，一直都没发现有什么问题。
可以写在CLR程序集里，给SQL
SERVER 2005执行，就有问题，报错：
Specified   cast   is   not   valid（数据类型转换出错。）
后来换成
int iRe = Convert.ToInt32(command.ExecuteScalar());
就可以鸟。真奇怪，原本改成
Int32
iRe = (Int32)command.ExecuteScalar();
都不行。

