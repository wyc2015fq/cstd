# 学习ADO.NET技术（八）—Command对象高级应用 - u014677855的博客 - CSDN博客
2018年08月16日 12:00:27[xiaobigben](https://me.csdn.net/u014677855)阅读数：83
## 目录
- - [目录](#目录)
- [1、异步执行命令](#1异步执行命令)
- [2、使用参数化查询](#2使用参数化查询)
- [3、如何获取插入行的ID?](#3如何获取插入行的id)
- [4、总结](#4总结)
- [说明](#说明)
## 1、异步执行命令
在ADO.NET 2.0版本之前，执行Command对象命令时，需要等待命令完成才能执行其他操作。比如，执行ExecuteNonQuery（）方法,应用程序将会阻塞，直到数据操作完成或者异常终止或者连接超时。 
引入异步执行特性之后，使得ADO.NET更稳健了。 
异步执行的根本思想是，在执行命令操作时，无需等待命令操作完成，可以并发地处理其他操作。典型的异步执行方法有：BeginExecuteNonQuery和EndExecuteNonQuery。 
BeginExecuteNonQuery异步执行方法返回System.IAsyncResult接口对象。我们可以根据IAsyncResult的IsCompleted属性来轮询（检测）命令是否执行完成。 
实例：在上一节建立的表tb_SelCustomer中插入500行数据，并计算执行时间。
```
SqlConnectionStringBuilder connstr = new SqlConnectionStringBuilder();
            connstr.DataSource = "LENOVO-PC\\MR2014";
            connstr.InitialCatalog = "db_MyDemo";
            connstr.IntegratedSecurity = true;
            StringBuilder strSQL = new StringBuilder();
            //插入500个测试客户
            for (int i = 1; i <= 500; ++i)
            {
                strSQL.Append("insert into tb_SelCustomer ");
                strSQL.Append("values('");
                string name = "测试客户" + i.ToString();
                strSQL.Append(name);
                strSQL.Append("','0','0','13822223333','liuhaorain@163.com','广东省深圳市宝安区',12.234556,34.222234,'422900','备注信息'); ");
            }
            using (SqlConnection conn = new SqlConnection(connstr.ConnectionString))
            {
                SqlCommand cmd = new SqlCommand(strSQL.ToString(),conn);
                conn.Open();
                IAsyncResult pending = cmd.BeginExecuteNonQuery();//命令的异步执行操作
                double time = 0;
                while(pending.IsCompleted==false)//异步执行操作未完成
                {
                    Thread.Sleep(1);
                    time++;
                    richTextBox1.Text += (time * 0.001).ToString() + "s\n";
                }
                if(pending.IsCompleted)
                {
                    richTextBox1.Text+=("Data is inserted completely...\nTotal coast"+ (time * 0.001).ToString()+"s" );
                }
                cmd.EndExecuteNonQuery(pending);//结束异步执行操作
            }
```
执行结果为： 
![这里写图片描述](https://img-blog.csdn.net/20180816095423196?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2、使用参数化查询
在ADO.NET中，查询语句是以字符串的形式传递给外部数据服务器的。直接拼接字符串会存在安全隐患，参数化查询可以提高查询执行性能。下表总结了不同数据源对应的Parameter对象。
|数据提供程序|对应Parameter对象|命名空间|
|----|----|----|
|SQL SERVER数据源|Sqlparameter|System.Data.SqlClient.SqlParameter|
|Ole DB 数据源|使用OleDbParameter对象|System.Data.OleDb.OleDbParameter|
|ODBC 数据源|使用OdbcParamter对象|System.Data.Odbc.OdbcParameter|
|Oracle数据源|使用OracleParameter对象|System.Data.OracleClient.OracleParameter|
Parameter对象常见的属性有以下几个：
- DbType:获取或设置参数的数据类型。
- Direction：获取或设置一个值，该值只是参数是否只可输入、只可输出、双向还是存储过程返回值参数。
- IsNullable：获取或设置一个值，该值只是参数是否可以为空。
- ParameterName：获取或设置DbParamter的名称。
- size：获取或设置列中数据的最大大小。
- Value：参数的值。 
以SQL Server为例，Command对象包含一个Parameters集合，该集合中包含了所有需要的SqlParameter对象，当执行命令时，ADO.NET同时将SQL文本，占位符合参数集合传递给数据库。
> 
提示： 
  对于不同的数据源来说，占位符不同。SQLServer数据源用@parametername格式来命名参数，OleDb以及Odbc数据源均用问号（?）来标识参数位置，而Oracle则以:parmname格式使用命名参数。
修改上面例子中测试顾客1：
```
SqlCommand cmd = new SqlCommand(strSQL.ToString(), conn);
                //构造参数对象
                SqlParameter para1 = new SqlParameter("@Phone", SqlDbType.VarChar, 12);
                SqlParameter para2 = new SqlParameter("@Email", SqlDbType.VarChar, 50);
                SqlParameter para3 = new SqlParameter("@Address", SqlDbType.VarChar, 200);
                SqlParameter para4 = new SqlParameter("@Name", SqlDbType.VarChar, 20);
                //给参数对象赋值
                para1.Value = "15682019804";
                para2.Value = "test@163.com";
                para3.Value = "深圳南山";
                para4.Value = "测试客户1";
                cmd.Parameters.Add(para1);
                cmd.Parameters.Add(para2);
                cmd.Parameters.Add(para3);
                cmd.Parameters.Add(para4);
                try
                {
                    conn.Open();
                   int row =  cmd.ExecuteNonQuery();
                    if (row > 0)
                        MessageBox.Show("更新成功");
                }
                catch (Exception ex)
                {
                    MessageBox.Show("异常信息：" + ex.Message);
                }
```
运行之后，查看数据表，成功更新了第一行信息。
## 3、如何获取插入行的ID?
运用SQL Server数据库原生的Output关键字。output关键字返回Insert操作的一个字段（一般是主键ID）。因此我们只要结合Output关键字和ExecuteScalar方法，就很容易得到插入行的主键。 
实例：我们在上述表中插入一个新的顾客，并返回这个顾客的ID。
```
StringBuilder strSQL = new StringBuilder();
            //插入500个测试客户
            strSQL.Append("insert  tb_SelCustomer (Name) ");
            strSQL.Append("OUTPUT inserted.ID values(@Name)");
            using (SqlConnection conn = new SqlConnection(connstr.ConnectionString))
            {
                SqlCommand cmd = new SqlCommand(strSQL.ToString(), conn);
                //构造参数对象
                SqlParameter paras = new SqlParameter("@Name", SqlDbType.VarChar, 20);
                paras.Value = "测试客户2";
                cmd.Parameters.Add(paras);
                try
                {
                    conn.Open();
                    int insertedId = (int)cmd.ExecuteScalar();
                        MessageBox.Show("插入行ID:"+insertedId);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("异常信息：" + ex.Message);
                }
            }
        }
```
## 4、总结
简言之，Command对象的核心作用是执行命令。在执行命令过程中，面临的情况是十分复杂的。尽管如此，Command对象拥有优越的人力资源（属性和方法），来应对一切可能发生的事。可以说，Command对象的稳定发挥，为ADO.NET打下了扎实的根基。到目前为止，我们基本上了解ADO.NET DataProvider组件所有的内容。因此，后面我将重点讲述ADO.NET的心脏—-DataSet以及如何将数据源本地化。
## 说明
[原作者文章请戳](https://www.cnblogs.com/liuhaorain/archive/2012/03/11/2378108.html)
> 
原作者声明： 
  我叫刘皓，很高兴您能阅读完我的这篇文章。 
  我花了大量时间和精力来完成这篇文章，如果文章对您有帮助，请不要忘了点推荐哦！ 
  如果您能点击右边的打赏按钮，打赏一杯咖啡钱，我将获得更多的动力和能量写出下一篇好文章。 
本文版权归作者和博客园共有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文链接，否则保留追究法律责任的权利。 
  我的博客即将搬运同步至腾讯云+社区，邀请大家一同入驻：[https://cloud.tencent.com/developer/support-plan](https://cloud.tencent.com/developer/support-plan)
