
# 数据库 Connection的dispose和close方法差异 - 追求卓越,做到专业 - CSDN博客


2013年08月30日 19:43:32[Waldenz](https://me.csdn.net/enter89)阅读数：3553


引用微软ADO.Team的经理的话说，sqlconnection的close和dispose实际是做的同一件事，唯一的区别是Dispose方法清空了connectionString，即设置为了null.

```python
SqlConnection con = new SqlConnection("Data Source=localhost;Initial Catalog=northwind;User ID=sa;Password=steveg");  
        con.Open();  
        con.Close();  
        con.Open();  
        con.Dispose();  
        con.Open();
```
上例运行发现，close掉的connection可以重新open，dispose的不行，因为connectionstring清空了，会抛出InvalidOperationException提示The ConnectionString property has not been initialized，但请注意此时sqlconnection对象还在。
如果dispose后给connectionString重新赋值，则不会报错。
由此得出的结论是不管是dispose还是close都不会销毁对象，即不会释放内存，它们会把sqlconnection对象丢到连接池中，那此对象什么时候销毁呢？我觉得应该是connection timeout设置的时间内，如果程序中没有向连接池发出请求说要connection对象，sqlconnection对象便会销毁，这也是连接池存在的意义。
刚开始以为dispose会释放资源清空内存，如果这样的话，连接池不是每次都是要创建新对象，那何来重用connection呢？在网上看到很多人说close比dispose好，我想真正的原因是dispose后的sqlconnection对象要重新初始化连接字符串而已，并不是象某些人说的dispose会释放对象。
所以在try..catch和using的选择上大胆的使用using吧，真正的效率差异我想可能只有百万分之一秒吧（连接池重用该连接对象初始化连接字符串的时间），而且enterprise library中封装的data access层全是用的using，从代码的美观和效率上综合考虑，using好
补充：using不会捕捉其代码快中的异常，只会最后执行dispose方法，相当于finally｛dispose｝，本文主要是想说明dispose和close的差异，因为using是绝对dispose的，可是如果人为的写try..finally有的人会选择close有的人会选择dispose，实际上在这2者的选择上是有差异的。

