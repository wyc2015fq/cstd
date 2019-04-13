
# 一篇文章搞定SqlDataAdapter - 追求卓越,做到专业 - CSDN博客


2012年08月23日 11:30:16[Waldenz](https://me.csdn.net/enter89)阅读数：450标签：[数据库																](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)[table																](https://so.csdn.net/so/search/s.do?q=table&t=blog)[sql																](https://so.csdn.net/so/search/s.do?q=sql&t=blog)[dataset																](https://so.csdn.net/so/search/s.do?q=dataset&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=sql&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=table&t=blog)个人分类：[C\#.Net																](https://blog.csdn.net/enter89/article/category/877633)
[
																								](https://so.csdn.net/so/search/s.do?q=table&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)

ado.net提供了丰富的数据库操作，在这些操作中SqlConnection和SqlCommand类是必须使用的，但接下来可以分为两类操作：
一类是用SqlDataReader直接一行一行的读取数据库。
第二类是SqlDataAdapter联合DataSet来读取数据。

两者比较：
SqlDataReader由于是直接访问数据库，所以效率较高。但使用起来不方便。
SqlDataAdapter可以把数据库的数据缓存在内存中，以数据集的方式进行填充。这种方式使用起来更方，便简单。但性能较第一种稍微差一点。（在一般的情况下两者的性能可以忽略不计。）

先看一个比较简单的代码：
publicvoidSqlAdapterDemo1(stringconnStr)
{
SqlConnection conn=newSqlConnection(connStr);//连接对象
SqlCommand cmd=conn.CreateCommand();//sql命令对象
cmd.CommandType=CommandType.Text;
cmd.CommandText="select * from products = @ID";//sql语句
cmd.Parameters.Add("@ID", SqlDbType.Int);
cmd.Parameters["@ID"].Value=1;//给参数sql语句的参数赋值
SqlDataAdapter adapter=newSqlDataAdapter();//构造SqlDataAdapter
adapter.SelectCommand=cmd;//与sql命令对象绑定，这个必不可少
DataSet ds=newDataSet();
adapter.Fill(ds,"table1");//填充数据。第二个参数是数据集中内存表的名字，可以与数据库中的不同
//Fill方法其实是隐藏的执行了Sql命令对象的CommandText
//填充完了后，就可以方便的访问数据了。例如
WriteLine(ds.Tables["table1"].Rows[0][1]);｝
怎么样？是不是很简单?
以上代码有一个地方需要注意,就是没有显示的调用连接对象的Open()方法。只是因为在每次调用Fill()方法的时候会自己去维护连接。调用Fill()方法并不会改变之前连接对象的状态。
//连接对象和sql命令对象单从上面的代码只有查询语句，没有其他的操作。那么接下来我就针对其他的操作来进一步说明SqlDataAdapter的用法。其他操作（如：增加，修改，删除等操作）可以分为两种情况一个是单表操作，一个是多表操作。
先来看单表操作：
/直接把上面的代码赋值过来
publicvoidSqlAdapterDemo1(stringconnStr)
{
SqlConnection conn=newSqlConnection(connStr);//连接对象

cmd.Parameters.Add("@ID", SqlDbType.Int);
cmd.Parameters["@ID"].Value=1;//给参数sql语句的参数赋值
SqlDataAdapter adapter=newSqlDataAdapter();//构造SqlDataAdapter
adapter.SelectCommand=cmd;//与sql命令对象绑定，这个必不可少
SqlCommandBuilder builder=newSqlCommandBuilder(adapter);//在构造好了SqlDataAdapter对象//加上此句代码
DataSet ds=newDataSet();
adapter.Fill(ds,"table1");//填充数据。第二个参数是数据集中内存表的名字，可以与数据库中的不同
//Fill方法其实是隐藏的执行了Sql命令对象的CommandText
//填充完了后，就可以方便的访问数据了。例如
WriteLine(ds.Tables["table1"].Rows[0][1]);
//修改操作
ds.Tables["table1"].Rows[0][1]="Sample2";//此句代码只是修改了数据集中某够值
//更新到数据库
adapter.Update(ds,"table1");//把内存中的数据同步到数据库中
上面的修改代码很简单，原理就是修改了内存中数据集的数据，然后调用一下Update()方法就同步到数据库中去了。SqlDataAdapter帮我们自动生成了Sql语句，并且在这里Update()方法是带了事务处理功能的。其他的删除，增加操作同理，在对内存中的数据集进行相关修改后，只需要调用一下Update()方法即可同步到数据库中去。但遗憾的是，目前这种方式只支持单表的操作，不支持任何与多表相关的操作（包括同一视图中来自不同表的列）。

