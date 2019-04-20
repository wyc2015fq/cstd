# 学习ADO.NET技术（七） Command对象 - u014677855的博客 - CSDN博客
2018年08月15日 20:36:35[xiaobigben](https://me.csdn.net/u014677855)阅读数：159
前面几节主要学习了ADO.NET与外部数据源建立连接以及运用连接池提高连接性能的知识。连接对象（Connection）是ADO.NET的主力先锋，为用户与数据库交互搭建了扎实的桥梁。Connection对象为用户与数据库搭建好桥梁之后，它的任务就完成了。此时，Command对象就闪亮登场。Command对象在ADO.NET的世界里总是忙忙碌碌，像是一个外交官，为用户传达了所有操作数据库的信息。
## 目录
- - [目录](#目录)
- [1、准备工作](#1准备工作)
- [2、什么是Command对象？](#2什么是command对象)
- [3、Command对象的几个属性](#3command对象的几个属性)
- [4、Command对象的几个方法](#4command对象的几个方法)
- [5、如何创建Command对象？](#5如何创建command对象)
- [6、选择合适的执行命令](#6选择合适的执行命令)
- [7、总结](#7总结)
- [说明](#说明)
## 1、准备工作
- 创建一个数据库，名为db_MyDemo.
- 创建一个数据表，名为tb_SelCustomer。
## 2、什么是Command对象？
ADO.NET最主要的目的是对外部数据源提供一致的访问。而访问数据源，就少不了增删查改等操作。Connection对象连接好数据源之后，不会再对数据源进行任何操作。Command对象封装了所有对外部数据源的操作（包括增删查改等SQL语句与存储过程），并在执行完成后返回合适的结果。与Connection对象一样，对于不同数据源，ADO.NET提供了不同的Command对象。具体可见以下表格：
|.Net数据提供程序|对应Commad对象|
|----|----|
|用于OLE DB的.Net Framework数据提供程序|OleDbCommand对象|
|用于SQL Server的.Net Framework数据提供程序|SqlCommand对象|
|用于ODBC的.Net Framework数据提供程序|OdbcCommand对象|
|用于Oracle的.Net Framework数据提供程序|OracleCommand对象|
不管是哪种Command对象，它都继承于DBCommand类。它同样是抽象基类，不能实例化。DBCommand类定义了完善的健全的数据库操作的基本方法和属性。它的结构如下：
`public abstract class DbCommand:Component,IdbCommand,IDisposable`
## 3、Command对象的几个属性
- CommandText：获取或设置对数据源执行操作的文本命令
- CommandType：命令类型，指定如何解释CommandText属性。CommandType属性的值是一个枚举类型，定义结构如下： 
```
public enum CommandType 
{ 
   Text = 1;//SQL文本命令 
            StoredProcedure = 4;//存储过程的名称 
            TableDirect = 512;//表的名称 
}
```
将CommandType设置为StoredProsedure时，应将CommandText属性设置为存储过程的名称。调用Execute方法之一时，该命令将执行此存储过程。
- Connection：设置或获取与数据源的连接。
- Parameters：绑定SQL语句或存储过程的参数。参数化查询中不可或缺的对象，非常重要。
- Tranction：获取或设置在其中执行.NetFramework数据提供程序的Command对象的事务。
## 4、Command对象的几个方法
- ExecuteNonQuery:执行不返回数据行的操作，并返回一个Int类型的数据。
> 
注意：对于 UPDATE、INSERT 和 DELETE 语句，返回值为该命令所影响的行数。 对于其他所有类型的语句，返回值 为 -1。
- ExecuteReader：执行查询，并返回一个DataReader对象。
- ExecuteScalar：执行查询，并返回查询结果集中第一行的第一列。如果找不到第一行第一列，则返回null的引用。
## 5、如何创建Command对象？
在创建Command对象之前，需要明确两件事情：（1）需要对哪个数据源执行操作（2）执行什么样的操作。 
对哪个数据源执行操作：由连接对象来确定。执行什么样的操作，由SQL语句来决定。那么如何通过这两个信息来构造Command对象呢？一般来说，有两种方法： 
（1）通过构造函数。
```
string strSQL = "select * from tb_SelCustomer";
 string conn = "Data Source = LENOVO - PC\\MR2014; " + "Initial Catalog = db_MyDemo;User Id = sa;Password = ;";
 SqlCommand cmd = new SqlCommand(strSQL,conn);
```
（2）通过Command对象的属性
```
string strSQL = "select * from tb_SelCustomer";
 SqlCommand cmd = new SqlCommand();
 cmd.Connection = con;
 cmd.CommandText = strSQL;
```
## 6、选择合适的执行命令
- 执行增删改操作，不返回数据行，返回受影响的行数。 
当我们对数据表的行进行增加删除更新操作时，实际上数据是不返回数据行的，仅仅返回一个包含受影响行数信息的整数。一般地，在执行非查询操作时，我们需要调用ExecuteNonQuery方法。 
实例：
```
SqlConnectionStringBuilder connstr = new SqlConnectionStringBuilder();
            connstr.DataSource = "LENOVO-PC\\MR2014";
            connstr.InitialCatalog = "db_MyDemo";
            connstr.IntegratedSecurity = true;
            using (SqlConnection con = new SqlConnection(connstr.ConnectionString))
            {
                string strSql = "insert into tb_SelCustomer  values('liuhao','0','0','13822223333','liuhaorain@163.com','广东省深圳市宝安区',12.234556,34.222234,'422900','备注信息')";
                SqlCommand cmd = new SqlCommand();
                cmd.Connection = con;//连接对象
                cmd.CommandType = CommandType.Text;//CommandText的内容是SQL语句
                cmd.CommandText = strSql;//SQL语句内容
                try
                {
                    con.Open();
                    int rows = cmd.ExecuteNonQuery();
                    MessageBox.Show("受影响行数：" + rows.ToString());
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
```
查询数据库，发现表中已经成功插入一行了。 
![执行结果](https://img-blog.csdn.net/20180815171246968?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 - 执行查操作，返回多个数据 
 执行查询操作时，会返回一行或多行数据，这个时候就调用ExecutReader方法。ExcuteReader方法返回一个DataReader对象。DataReader是一个只读的遍历访问每一行数据的数据流。需要注意以下几点：
```
using (SqlConnection con = new SqlConnection(connstr.ConnectionString))
            {
                string strSql = "select * from tb_SelCustomer";
                SqlCommand cmd = new SqlCommand();
                cmd.Connection = con;//连接对象
                cmd.CommandType = CommandType.Text;//CommandText的内容是SQL语句
                cmd.CommandText = strSql;//SQL语句内容
                try
                {
                    int rows = 0;
                    con.Open();
                    SqlDataReader reader = cmd.ExecuteReader();
                    if (reader != null && reader.HasRows)
                    {
                        while (reader.Read())
                        {
                            for (int i = 0; i < reader.FieldCount; ++i)
                            {
                                MessageBox.Show(reader.GetName(i) + "  " + reader.GetValue(i));
                            }
                            ++rows;
                        }
                        MessageBox.Show("共" + rows + "条记录");
                    }
                    reader.Close();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
```
- 执行查操作，返回单个值 
有时查询时仅仅需要返回一个值，ExcuteScalar方法就是处理单个数据最优秀的人才。ExcuteScalar返回一个System.Object类型的数据，因此我们在获取数据时需要进行强制类型转换。当没有数据时，ExcuteScalar方法返回System.DBNull。 
实例：
```
string strSql = "select count(*) from tb_SelCustomer";
                SqlCommand cmd = new SqlCommand();
                cmd.Connection = con;//连接对象
                cmd.CommandType = CommandType.Text;//CommandText的内容是SQL语句
                cmd.CommandText = strSql;//SQL语句内容
                try
                {
                    int rows = 0;
                    con.Open();
                    rows =(int) cmd.ExecuteScalar();//返回单个值
                    MessageBox.Show("一共返回记录数：" + rows.ToString());
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
                finally
                {
                    con.Close();
                    con.Dispose();
                }
```
运行结果为：返回一条记录。当前实例返回的是数据表中记录的总个数。
## 7、总结
Command对象称为数据库命令对象，主要执行包括添加、删除、修改及查询数据的操作，也可以用来执行存储过程。用于执行存储过程时，需要将Command对象的属性CommandType设置为CommandType.StoredProcedure.。默认情况下执行SQL语句。
## 说明
[本文学习自此篇总结，详情请戳原作者文章](https://www.cnblogs.com/liuhaorain/archive/2012/02/27/2361825.html)
> 
我叫刘皓，很高兴您能阅读完我的这篇文章。 我花了大量时间和精力来完成这篇文章，如果文章对您有帮助，请不要忘了点推荐哦！ 
  如果您能点击右边的打赏按钮，打赏一杯咖啡钱，我将获得更多的动力和能量写出下一篇好文章。 
本文版权归作者和博客园共有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文链接，否则保留追究法律责任的权利。 
  我的博客即将搬运同步至腾讯云+社区，邀请大家一同入驻：[https://cloud.tencent.com/developer/support-plan](https://cloud.tencent.com/developer/support-plan)
