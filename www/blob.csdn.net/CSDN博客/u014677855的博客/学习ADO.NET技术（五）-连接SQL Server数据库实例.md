# 学习ADO.NET技术（五）-连接SQL Server数据库实例 - u014677855的博客 - CSDN博客
2018年08月14日 17:04:16[xiaobigben](https://me.csdn.net/u014677855)阅读数：51
# 在Winform中创建连接数据库的实例
## 1、在SQL Server中创建数据库和表
- 
打开SQL Server2014，以SQL Server身份认证模式登入。 
![打开SQL Server](https://img-blog.csdn.net/2018081416224712?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
记住登录界面的服务器名称：LENOVO-PC\MR2014(以我的电脑为例）
- 
创建新的数据库:MyTest和表Beautys。 
![创建数据库](https://img-blog.csdn.net/20180814162852936?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
创建数据表，添加数据到表中。
```
go
use Mytest
create table Beautys
(
Id int primary key identity(1,1),
Title nvarchar(128) not null,
Price int ,
Info ntext
)
```
```
insert into dbo.Beautys(Title,Price,Info)
select 'Lamer',1314,'海蓝之谜' union
select 'Lancome',690,'兰蔻' union
select 'YSL',520,'圣罗兰'
```
![数据表](https://img-blog.csdn.net/20180814164635387?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2、在VS中连接数据库
```
新建Winform项目，定义按钮事件：
```
```
private void button1_Click(object sender, EventArgs e)
        {
            string strConn = "Data Source=LENOVO-PC\\MR2014;" +
                "Initial Catalog = Mytest;User Id = sa;Password = ;";
            SqlConnection conn = new SqlConnection(strConn);
            try
            {
                conn.Open();
                if (conn.State == ConnectionState.Open)
                {
                    MessageBox.Show("打开数据库连接");
                    string StrConn = conn.ConnectionString;
                    string DataSource = conn.DataSource;
                    string inicat = conn.Database;
                    string state = conn.State.ToString();
                    MessageBox.Show("连接字符串："+strConn+"\n数据源："+DataSource+"\n数据库:"+inicat+"\n连接状态："+state);
                }
            }
            catch (Exception)
            {
                throw;
            }
            finally
            {
                conn.Close();
                MessageBox.Show("连接状态："+conn.State.ToString());
                conn.Dispose();
            }
        }
```
运行结果： 
![打开连接](https://img-blog.csdn.net/20180814174813157?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180814173749598?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![关闭连接时](https://img-blog.csdn.net/20180814173832481?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 3、说明
- 正确连接数据库的前提是数据库连接字符串正确，告知连接到的数据源、是何种数据以及其他信息。数据库连接字符串包含的信息有：Data Source(服务器名称）、Initial Catalog（数据库）、User Id(用户名)、Password（密码）、Integrated Security（设置为true时，表示为windows身份验证，用户名和密码是不起作用的。））。
- 记住SQL Server数据库连接字符串的组成！
