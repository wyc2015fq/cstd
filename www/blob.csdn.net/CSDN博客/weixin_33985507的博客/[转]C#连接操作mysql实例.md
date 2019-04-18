# [转]C#连接操作mysql实例 - weixin_33985507的博客 - CSDN博客
2012年11月20日 10:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
本文转自：[http://hi.baidu.com/zhqngweng/item/c4d2520cb7216877bfe97edf](http://hi.baidu.com/zhqngweng/item/c4d2520cb7216877bfe97edf)
第三方组件：Mysql.Data.dll
说明：去官方网站下载Mysql.Data.dll，然后在项目中添加该组件的引用，在代码页里输入using Mysql.Data.MysqlClient，我们就可以顺利的使用该类库的函数建立连接了。
以下是几个常用函数：
#region  建立MySql数据库连接
    /// <summary>
    /// 建立数据库连接.
    /// </summary>
    /// <returns>返回MySqlConnection对象</returns>
    public MySqlConnection getmysqlcon()
    {
        string M_str_sqlcon = "server=localhost;user id=root;password=root;database=abc"; //根据自己的设置
        MySqlConnection myCon = new MySqlConnection(M_str_sqlcon);
        return myCon;
    }
    #endregion
    #region  执行MySqlCommand命令
    /// <summary>
    /// 执行MySqlCommand
    /// </summary>
    /// <param name="M_str_sqlstr">SQL语句</param>
    public void getmysqlcom(string M_str_sqlstr)
    {
        MySqlConnection mysqlcon = this.getmysqlcon();
        mysqlcon.Open();
        MySqlCommand mysqlcom = new MySqlCommand(M_str_sqlstr, mysqlcon);
        mysqlcom.ExecuteNonQuery();
        mysqlcom.Dispose();
        mysqlcon.Close();
        mysqlcon.Dispose();
    }
    #endregion
#region  创建MySqlDataReader对象
    /// <summary>
    /// 创建一个MySqlDataReader对象
    /// </summary>
    /// <param name="M_str_sqlstr">SQL语句</param>
    /// <returns>返回MySqlDataReader对象</returns>
    public MySqlDataReader getmysqlread(string M_str_sqlstr)
    {
        MySqlConnection mysqlcon = this.getmysqlcon();
        MySqlCommand mysqlcom = new MySqlCommand(M_str_sqlstr, mysqlcon);
        mysqlcon.Open();
        MySqlDataReader mysqlread = mysqlcom.ExecuteReader(CommandBehavior.CloseConnection);
        return mysqlread;
    }
    #endregion
另一篇：
测试环境：Windows XP + MySql 5.0.24 + Visual C# 2008 Exdivss Edition
By lucas 2008.12.29
1、用MySQLDriverCS连接MySQL数据库
先下载和安装MySQLDriverCS，地址：
[http://sourceforge.net/projects/mysqldrivercs/](http://sourceforge.net/projects/mysqldrivercs/)
在安装文件夹下面找到MySQLDriver.dll，然后将MySQLDriver.dll添加引用到项目中
注：我下载的是版本是 MySQLDriverCS-n-EasyQueryTools-4.0.1-DotNet2.0.exe
 using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.Odbc;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MySQLDriverCS;
namespace mysql
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            MySQLConnection conn = null;
            conn = new MySQLConnection(new MySQLConnectionString("localhost", "inv", "root", "831025").AsString);
            conn.Open();
            MySQLCommand commn = new MySQLCommand("set names gb2312", conn);
            commn.ExecuteNonQuery();
            string sql = "select * from exchange ";
            MySQLDataAdapter mda = new MySQLDataAdapter(sql, conn);
            DataSet ds = new DataSet();
            mda.Fill(ds, "table1");
            this.dataGrid1.DataSource = ds.Tables["table1"];
            conn.Close();
        }
    }
}
2、通过ODBC访问mysql数据库：
参考：[http://www.microsoft.com/china/community/Column/63.mspx](http://www.microsoft.com/china/community/Column/63.mspx)
1.      安装Microsoft ODBC.net：我安装的是mysql-connector-odbc-3.51.22-win32.msi
2.      安装MDAC 2.7或者更高版本：我安装的是mdac_typ.exe 2.7简体中文版
3.      安装MySQL的ODBC驱动程序：我安装的是 odbc_net.msi
4.      管理工具 -> 数据源ODBC –>配置DSN…
5.      解决方案管理中添加引用 Microsoft.Data.Odbc.dll(1.0.3300)
6.      代码中增加引用 using Microsoft.Data.Odbc;
 using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;   //vs2005好像没有这个命名空间，在c#2008下测试自动生成的
using System.Text;
using System.Windows.Forms;
using Microsoft.Data.Odbc;
namespace mysql
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            string MyConString = "DRIVER={MySQL ODBC 3.51 Driver};" +
                                 "SERVER=localhost;" +
                                 "DATABASE=inv;" +
                                 "UID=root;" +
                                 "PASSWORD=831025;" +
                                 "OPTION=3";
            OdbcConnection MyConnection = new OdbcConnection(MyConString);
            MyConnection.Open();
            Console.WriteLine("\n success, connected successfully !\n");
            string query = "insert into test values( ''hello'', ''lucas'', ''liu'')";
            OdbcCommand cmd = new OdbcCommand(query, MyConnection);
            //处理异常：插入重复记录有异常
try{
   cmd.ExecuteNonQuery();
}
catch(Exception ex){
                 Console.WriteLine("record duplicate.");
}finally{
                 cmd.Dispose();
}
/
/
           MyConnection.Close();
        }
    }
}
使用示例：
using System;
using System.Configuration;
using MySql.Data.MySqlClient;
/// <summary>
/// TestDatebase 的摘要说明
/// </summary>
public class TestDatebase
{
    public TestDatebase()
    {
        //
        // TODO: 在此处添加构造函数逻辑
        //
    }
    public static void Main(String[] args)
    {
        MySqlConnection mysql = getMySqlCon();
        //查询sql
        String sqlSearch = "select * from student";
        //插入sql
        String sqlInsert = "insert into student values (12,'张三',25,'大专')";
        //修改sql
        String sqlUpdate = "update student set name='李四' where id= 3";
        //删除sql
        String sqlDel = "delete from student where id = 12";
        //打印SQL语句
        Console.WriteLine(sqlDel);
        //四种语句对象
        //MySqlCommand mySqlCommand = getSqlCommand(sqlSearch, mysql);
        //MySqlCommand mySqlCommand = getSqlCommand(sqlInsert, mysql);
        //MySqlCommand mySqlCommand = getSqlCommand(sqlUpdate, mysql);
        MySqlCommand mySqlCommand = getSqlCommand(sqlDel, mysql);
        mysql.Open();
        //getResultset(mySqlCommand);
        //getInsert(mySqlCommand);
        //getUpdate(mySqlCommand);
        getDel(mySqlCommand);
        //记得关闭
        mysql.Close();
       String readLine = Console.ReadLine();
    }
    /// <summary>
    /// 建立mysql数据库链接
    /// </summary>
    /// <returns></returns>
    public static MySqlConnection getMySqlCon()
    {
        String mysqlStr = "Database=test;Data Source=127.0.0.1;User Id=root;Password=root;pooling=false;CharSet=utf8;port=3306";
        // String mySqlCon = ConfigurationManager.ConnectionStrings["MySqlCon"].ConnectionString;
        MySqlConnection mysql = new MySqlConnection(mysqlStr);
        return mysql;
    }
    /// <summary>
    /// 建立执行命令语句对象
    /// </summary>
    /// <param name="sql"></param>
    /// <param name="mysql"></param>
    /// <returns></returns>
    public static MySqlCommand getSqlCommand(String sql,MySqlConnection mysql)
    {
        MySqlCommand mySqlCommand = new MySqlCommand(sql, mysql);
        //  MySqlCommand mySqlCommand = new MySqlCommand(sql);
        // mySqlCommand.Connection = mysql;
        return mySqlCommand;
    }
    /// <summary>
    /// 查询并获得结果集并遍历
    /// </summary>
    /// <param name="mySqlCommand"></param>
    public static void getResultset(MySqlCommand mySqlCommand)
    {
        MySqlDataReader reader = mySqlCommand.ExecuteReader();
        try
        {
            while (reader.Read())
            {
                if (reader.HasRows)
                {
                    Console.WriteLine("编号:" + reader.GetInt32(0) + "|姓名:" + reader.GetString(1) + "|年龄:" + reader.GetInt32(2) + "|学历:" + reader.GetString(3));
                }
            }
        }
        catch (Exception)
        {
            Console.WriteLine("查询失败了！");
        }
        finally
        {
            reader.Close();
        }
    }
    /// <summary>
    /// 添加数据
    /// </summary>
    /// <param name="mySqlCommand"></param>
    public static void getInsert(MySqlCommand mySqlCommand)
    {
        try
        {
            mySqlCommand.ExecuteNonQuery();
        }
        catch (Exception ex)
        {
            String message = ex.Message;
            Console.WriteLine("插入数据失败了！" + message);
        }
    }
    /// <summary>
    /// 修改数据
    /// </summary>
    /// <param name="mySqlCommand"></param>
    public static void getUpdate(MySqlCommand mySqlCommand)
    {
        try
        {
            mySqlCommand.ExecuteNonQuery();
        }
        catch (Exception ex)
        {
            String message = ex.Message;
            Console.WriteLine("修改数据失败了！" + message);
        }
    }
    /// <summary>
    /// 删除数据
    /// </summary>
    /// <param name="mySqlCommand"></param>
    public static void getDel(MySqlCommand mySqlCommand)
    {
        try
        {
            mySqlCommand.ExecuteNonQuery();
        }
        catch (Exception ex)
        {
            String message = ex.Message;
            Console.WriteLine("删除数据失败了！" + message);
        }
    }
}
