# SQL Server LocalDB本地文件数据库操作实践 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月20日 11:11:42[boonya](https://me.csdn.net/boonya)阅读数：2949








这里以官方的[Microsoft SQL Server 2012 Express](https://docs.microsoft.com/zh-cn/previous-versions/sql/sql-server-2012/hh510202(v=sql.110))**[LocalDB](https://docs.microsoft.com/zh-cn/previous-versions/sql/sql-server-2012/hh510202(v=sql.110))加以说明，VS从2012**开始应该都集成了此功能。

Microsoft SQL Server 2012 Express **LocalDB** 是面向程序开发人员的 SQL Server Express 的执行模式。 **LocalDB** 安装将复制启动 SQL Server 数据库引擎 所需的最少的文件集。 安装 **LocalDB** 后，开发人员将使用特定连接字符串来启动连接。 连接时，将自动创建并启动所需的 SQL Server 基础结构，从而使应用程序无需执行复杂或耗时的配置任务即可使用数据库。 开发人员工具可以向开发人员提供 SQL Server 数据库引擎，使其不必管理 SQL Server 的完整服务器实例即可撰写和测试 Transact-SQL 代码。 通过使用 **SqlLocalDB.exe** 实用工具管理 SQL Server Express **LocalDB** 的实例。 SQL Server Express **LocalDB** 应该用于代替已不再推荐使用的 SQL Server Express 用户界面功能。



## 安装 LocalDB

安装 **LocalDB** 的主要方法是使用 SqlLocalDB.msi 程序。 **LocalDB** 是安装 SQL Server 2012 Express 的任何 SKU 时的一个选项。 在安装 SQL Server Express 的过程中在**“功能选择”**页上选择 **LocalDB**。 对于每个主要 SQL Server 数据库引擎 版本，只能存在 **LocalDB** 二进制文件的一个安装。 可以启动多个数据库引擎进程，并且这些进程都将使用相同的二进制文件。 作为 **LocalDB** 启动的 SQL Server 数据库引擎实例与 SQL Server Express 具有相同的限制。

## 说明

**LocalDB** 安装程序使用 SqlLocalDB.msi 程序在计算机上安装所需文件。 安装后，**LocalDB** 是可以创建和打开 SQL Server 数据库的 SQL Server Express 实例。 数据库的系统数据库文件存储于用户本地 AppData 路径中，这个路径通常是隐藏的。 例如 **C:\Users\<user>\AppData\Local\Microsoft\Microsoft SQL Server Local DB\Instances\LocalDBApp1\**。 用户数据库文件存储于用户指定的位置，通常是 **C:\Users\<user>\Documents\** 文件夹中的某处。

有关在应用程序中包含 **LocalDB** 的详细信息，请参阅 Visual Studio 文档[本地数据概述](http://msdn.microsoft.com/library/ms233817(vs.110).aspx)，[演练：创建 SQL Server LocalDB 数据库](http://msdn.microsoft.com/library/ms233763(vs.110).aspx) 和 [演练：连接 SQL Server LocalDB 数据库（Windows 窗体）中的数据](http://msdn.microsoft.com/en-us/library/ms171890(vs.110).aspx)。

有关 **LocalDB** API 的详细信息，请参阅 [SQL Server Express LocalDB 实例 API 参考](https://docs.microsoft.com/zh-cn/previous-versions/sql/sql-server-2012/hh234692%28v%3Dsql.110%29)和 [LocalDBStartInstance 函数](https://docs.microsoft.com/zh-cn/previous-versions/sql/sql-server-2012/hh217143%28v%3Dsql.110%29)。

SqlLocalDb 实用工具可以创建 **LocalDB** 的新实例，启动和停止 **LocalDB** 的实例，并且包含可帮助您管理 **LocalDB** 的选项。 有关 SqlLocalDb 实用工具的详细信息，请参阅 [SqlLocalDB 实用工具](https://docs.microsoft.com/zh-cn/previous-versions/sql/sql-server-2012/hh212961%28v%3Dsql.110%29)。

将 **LocalDB** 的实例排序规则设置为 SQL_Latin1_General_CP1_CI_AS，不能更改它。 通常支持数据库级、列级和表达式级排序规则。 包含数据库遵循[包含数据库的排序规则](https://docs.microsoft.com/zh-cn/previous-versions/sql/sql-server-2012/ff929080%28v%3Dsql.110%29)所定义的元数据和 tempdb 排序规则。

### 限制

**LocalDB** 不能是合并复制订阅服务器。

**LocalDB** 不支持 FILESTREAM。

**LocalDB** 仅允许 Service Broker 的本地队列。

由于 Windows 文件系统重定向，内置帐户（如 NT AUTHORITY\SYSTEM）所有的 **LocalDB** 的实例可能有可管理性问题；使用常规 Windows 帐户作为所有者。

### 自动实例和命名实例

**LocalDB** 支持两种实例：自动实例和命名实例。
- 
**LocalDB** 的自动实例是公共的。 系统自动为用户创建和管理此类实例，并可由任何应用程序使用。 安装在用户计算机上的每个 **LocalDB** 版本都存在一个自动 **LocalDB** 实例。 自动 **LocalDB** 实例提供无缝的实例管理。 无需创建实例；它可以自动执行工作。 这使得应用程序可以轻松地安装和迁移到另一台计算机。 如果目标计算机已安装指定版本的 LocalDB，则目标计算机也提供此版本的自动 LocalDB 实例。 自动 **LocalDB** 实例具有属于保留命名空间的特殊实例名称模式。 这可以防止名称与命名 **LocalDB** 实例发生冲突。 自动实例的名称是单个 v 字符后跟 **xx.x** 格式的 **LocalDB** 发行版本号。 例如，**v11.0** 表示 SQL Server 2012。

- 
**LocalDB** 的命名实例是专用的。 这些命名实例由负责创建和管理该实例的单个应用程序所拥有。 命名实例提供与其他实例的隔离，并可以通过减少与其他数据库用户的资源争用来提高性能。 命名实例必须由用户通过 **LocalDB** 管理 API 显式创建，或者通过托管应用程序的 app.config 文件隐式创建（尽管托管应用程序也会在需要时使用 API）。 **LocalDB** 的每个命名实例都具有关联的 LocalDB 版本，指向相应的 LocalDB 二进制文件集。 **LocalDB** 的命名实例为 sysname 数据类型并且可具有最多 128 个字符。 （这不同于常规的 SQL Server 命名实例，此类命名实例将名称限制为 16 个 ASCII 字符的常规 NetBIOS 名称。）**LocalDB** 实例名称可包含在文件名内合法的任何 Unicode 字符。 使用自动实例名称的命名实例将成为自动实例。


不同的计算机用户可具有同名的实例。 每个实例都是以不同的用户身份运行的不同的进程。

## LocalDB 的共享实例

为了支持多个计算机用户需要连接到单个 **LocalDB** 实例的方案，**LocalDB** 支持实例共享。 实例所有者可以选择允许计算机上的其他用户连接到其实例。 **LocalDB** 的自动实例和命名实例都可以共享。 若要共享 LocalDB 的某个实例，用户需要为其选择一个共享名称（别名）。 因为该共享名称对于该计算机的所有用户都是可见的，则此共享名称在计算机上必须唯一。 LocalDB 实例的共享名称具有与 **LocalDB** 的命名实例相同的格式。

只有计算机上的管理员才能创建 **LocalDB** 的共享实例。 **LocalDB** 的共享实例可由管理员或 **LocalDB** 共享实例的所有者取消共享。 若要共享和取消共享某一 **LocalDB** 实例，请使用 **LocalDB** API 的 LocalDBShareInstance 和 LocalDBUnShareInstance 方法，或者使用 SqlLocalDb 实用工具的共享和取消共享选项。

## 启动 LocalDB 和连接到 LocalDB

### 连接到自动实例

使用 **LocalDB** 的最简单方法是通过使用连接字符串 **"Server=(localdb)\v11.0;Integrated Security=true"** 连接到当前用户拥有的自动实例。 要使用文件名连接到特定数据库，请使用类似于 **"Server=(LocalDB)\v11.0; Integrated Security=true ;AttachDbFileName=D:\Data\MyDB1.mdf"** 的连接字符串连接。
|![注意](https://docs.microsoft.com/zh-cn/previous-versions/sql/sql-server-2012/images/ms190219.alert_note%28sql.110%29.gif)**注意**|
|----|
|在计算机上的用户首次尝试连接到 **LocalDB** 时，必须创建并启动该自动实例。 创建实例所用的额外时间可能会导致连接尝试失败并且具有超时消息。 在发生此情况时，等待几秒钟以便让创建过程完成，然后再次连接。|

### 创建和连接到命名实例

除了自动实例之外，**LocalDB** 还支持命名实例。 使用 SqlLocalDB.exe 程序可以创建、启动和停止 **LocalDB** 的命名实例。 有关 SqlLocalDB.exe 的详细信息，请参阅 [SqlLocalDB 实用工具](https://docs.microsoft.com/zh-cn/previous-versions/sql/sql-server-2012/hh212961%28v%3Dsql.110%29)。

```
REM Create an instance of LocalDB
"C:\Program Files\Microsoft SQL Server\110\Tools\Binn\SqlLocalDB.exe" create LocalDBApp1
REM Start the instance of LocalDB
"C:\Program Files\Microsoft SQL Server\110\Tools\Binn\SqlLocalDB.exe" start LocalDBApp1
REM Gather information about the instance of LocalDB
"C:\Program Files\Microsoft SQL Server\110\Tools\Binn\SqlLocalDB.exe" info LocalDBApp1
```

上面的最后一行将返回如下信息。
|名称|"LocalDBApp1"|
|----|----|
|版本|<当前版本>|
|共享名称|""|
|所有者|"<你的 Windows 用户>”|
|自动创建|否|
|State|运行|
|上次启动时间|<日期和时间>|
|实例管道名称|np:\\. \pipe\LOCALDB#F365A78E\tsql\query|
|![注意](https://docs.microsoft.com/zh-cn/previous-versions/sql/sql-server-2012/images/ms190219.alert_note%28sql.110%29.gif)**注意**|
|----|
|如果您的应用程序使用早于 .NET 4.0.2 的版本，您必须直接连接到 **LocalDB** 的命名管道。 “实例管道名称”值为 **LocalDB** 的实例正在侦听的命名管道。 LOCALDB# 之后的实例管道名称部分将在每次启动 **LocalDB** 实例时更改。 若要通过使用 SQL Server Management Studio 连接到 **LocalDB** 实例，请在**“连接到数据库引擎”**对话框的**“服务器名称”**框中键入实例管道名称。 从你的自定义程序，你可以通过使用类似 SqlConnection conn = new SqlConnection(@"Server=np:\\.\pipe\LOCALDB#F365A78E\tsql\query"); 的连接字符串建立与 **LocalDB** 实例的连接|

### 连接到 LocalDB 的共享实例

若要连接到某一 **LocalDB** 共享实例，请将 **.\**（句点 + 反斜杠）添加到连接字符串以便引用为共享实例保留的命名空间。 例如，若要连接到名为 AppData 的 **LocalDB** 的共享实例，使用连接字符串（例如 (localdb)\.\AppData）作为连接字符串的一部分。 连接到用户不拥有的 **LocalDB** 共享实例的用户必须具有 Windows 身份验证或 SQL Server 身份验证登录名。

## 故障排除

有关排除 **LocalDB** 问题的信息，请参阅[排除 SQL Server 2012 Express LocalDB 问题](http://social.technet.microsoft.com/wiki/contents/articles/4609.aspx)。

## 权限

SQL Server 2012 Express **LocalDB** 的实例是用户为其使用而创建的实例。 计算机上的任何用户都可以使用 **LocalDB** 实例创建数据库，在用户配置文件下存储文件并使用凭据来运行进程。 默认情况下，对 **LocalDB** 实例的访问仅限于其所有者。 **LocalDB** 中包含的数据受到对数据库文件的文件系统访问的保护。 如果用户数据库文件存储于某一共享位置，则通过使用他们自己拥有的 **LocalDB** 实例对该位置具有文件系统访问权限的任何人都可以打开该数据库。 如果数据库文件处于某一受保护的位置，例如用户数据文件夹，则只有该用户以及有权访问该文件夹的任何管理员才能打开该数据库。 **LocalDB** 文件只能一次通过一个 **LocalDB** 实例来打开。
|![注意](https://docs.microsoft.com/zh-cn/previous-versions/sql/sql-server-2012/images/ms190219.alert_note%28sql.110%29.gif)**注意**|
|----|
|**LocalDB** 始终在用户安全上下文中运行；即 **LocalDB** 从不使用本地 Administrator 组的凭据来运行。 这意味着 **LocalDB** 实例使用的所有数据库文件必须可以通过拥有的用户的 Windows 帐户来访问，而不必考虑本地 Administrators 组中的成员资格。|



# VS2017打开Localdb创建数据库





找到vs左上角“视图”>"SQL Server 对象管理器"

![](https://img-blog.csdn.net/20180620105815840?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

之后弹出如下界面：

![](https://img-blog.csdn.net/20180620105849955?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

都可以在这两个localdb实例上创建自己的数据库。如下图所示：

![](https://img-blog.csdn.net/20180620105958725?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 编写测试LocalDB数据库程序

项目视图如下：

![](https://img-blog.csdn.net/20180620110140281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改App.config配置数据库访问串：

![](https://img-blog.csdn.net/20180620110306196?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
<?xml version="1.0" encoding="utf-8" ?>
<configuration>
    <startup> 
        <supportedRuntime version="v4.0" sku=".NETFramework,Version=v4.5" />
    </startup>
    <appSettings>
      <add key="ConnectionString" value="server=(LocalDB)\MSSQLLocalDB; Integrated Security=true ;AttachDbFileName=C:\Users\Administrator\Desktop\LocalDB\ms_localdb_cvnaviav_audio_video.mdf;Pooling=true;Max Pool Size =1000;" />
    </appSettings>
</configuration>
```

注：上面是指定到数据库文件的。

连接字符串工具类：

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;
using System.Threading.Tasks;

namespace FileCacheLocaldb.Utils
{
    /// <summary>
    /// 连接字符串
    /// </summary>
    public class ConnectionString
    {
        /// <summary>
        /// 获取连接字符串
        /// </summary>
        public static string GetConnectionString
        {
            get
            {
                string connectionName = ConfigurationManager.AppSettings["ConnectionName"];
                //string connectionString = ConfigurationManager.AppSettings["ConnectionString"];       
                //string ConStringEncrypt = ConfigurationManager.AppSettings["ConStringEncrypt"];
                //if (ConStringEncrypt == "true")
                //{
                //    _connectionString = DESEncrypt.Decrypt(_connectionString);
                //}
                string connectionString = string.Empty;
                switch (connectionName)
                {
                    case "SQLServer":
                        connectionString = ConfigurationManager.AppSettings["ConnectionString"];
                        break;
                    case "Access":
                        connectionString = ConfigurationManager.AppSettings["AccessConnectionString"];
                        break;
                    default:
                        connectionString = ConfigurationManager.AppSettings["ConnectionString"];
                        break;
                }

                return connectionString;
            }
        }
    }
}
```

数据库连接工具类：

```
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileCacheLocaldb.Utils
{
    /// <summary>
    /// 数据库连接帮助类
    /// </summary>
    public class DBConnectionHelper
    {
        /// <summary>
        /// 数据库连接字符串(web.config来配置)，多数据库可使用DbHelperSQLP来实现.
        /// </summary>
        public static readonly string connectionString = ConnectionString.GetConnectionString;

        /// <summary>
        /// 创建数据库连接
        /// </summary>
        /// <returns></returns>
        public static IDbConnection CreateConnection()
        {
            IDbConnection conn = null;
            conn = new SqlConnection(connectionString);
            conn.Open();
            return conn;
        }
    }
}
```

数据库SQL语句工具类：

```
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Dapper;

namespace FileCacheLocaldb.Utils
{
    /// <summary>
    /// SQL语句执行帮助类
    /// </summary>
    public class DBSQLHelper
    {
        /// <summary>
        /// 获取该数据库所有表
        /// </summary>
        /// <returns></returns>
        public DataTable GetTables()
        {
            using (IDbConnection conn = DBConnectionHelper.CreateConnection())
            {
                DataTable db = new DataTable();
                string strSql = "SELECT name FROM SysObjects Where XType='U' ORDER BY Name";
                IDataReader reader = conn.ExecuteReader(strSql);
                db.Load(reader);
                return db;
            }
        }

        /// <summary>
        /// 获取某表的所有数据
        /// </summary>
        /// <param name="tableName"></param>
        /// <returns></returns>
        public DataTable GetRows(string tableName)
        {
            using (IDbConnection conn = DBConnectionHelper.CreateConnection())
            {
                DataTable db = new DataTable(tableName);
                string strSql = string.Format("select * from {0}", tableName);
                IDataReader reader = conn.ExecuteReader(strSql);
                db.Load(reader);
                return db;
            }
        }

        /// <summary>
        /// 根据输入的语句获取数据
        /// </summary>
        /// <param name="sql"></param>
        /// <returns></returns>
        public DataTable GetSelect(string sql)
        {
            using (IDbConnection conn = DBConnectionHelper.CreateConnection())
            {
                DataTable db = new DataTable();
                IDataReader reader = conn.ExecuteReader(sql);
                db.Load(reader);
                return db;
            }
        }

        /// <summary>
        /// 执行增删改操作SQL
        /// </summary>
        /// <param name="sql"></param>
        /// <returns></returns>
        public int ExecuteSql(string sql)
        {
            using (IDbConnection conn = DBConnectionHelper.CreateConnection())
            {
                return conn.Execute(sql);
            }
        }
    }
}
```

# 数据库案例

这里以查询缓存文件记录为例。

数据库表设计：

```
CREATE TABLE [dbo].[AVINFO]
(
	[F_ID] UNIQUEIDENTIFIER NOT NULL PRIMARY KEY, 
    [F_SIM] NVARCHAR(20) NOT NULL, 
    [F_CHANNEL_ID] TINYINT NOT NULL, 
    [F_STREAM_TYPE] TINYINT NOT NULL, 
    [F_START_TIME] NUMERIC NOT NULL, 
    [F_END_TIME] NUMERIC NOT NULL, 
    [F_FILE_PATH] NCHAR(50) NOT NULL
)
```



定义数据模型：



```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileCacheLocaldb
{
    /// <summary>
    /// 缓存多媒体数据对象
    /// </summary>
    public class AVInfo
    {
        /// <summary>
        /// UUID
        /// </summary>
        public string id { get; set; }

        /// <summary>
        /// SIM卡号
        /// </summary>
        public string Sim { get; set; }

        /// <summary>
        /// 通道号
        /// </summary>
        public int Channel { get; set; }


        /// <summary>
        /// 码流类型
        /// </summary>
        public int StreamType { get; set; }

        /// <summary>
        /// 开始时间
        /// </summary>
        public long StartTime { get; set; }

        /// <summary>
        /// 结束时间
        /// </summary>
        public long EndTime { get; set; }

        /// <summary>
        /// 缓存文件路径
        /// </summary>
        public string FilePath { get; set; }
    }
}
```



定义数据接口：



```
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileCacheLocaldb
{
    /// <summary>
    /// 缓存接口
    /// </summary>
    public interface ICache
    {
        /// <summary>
        /// 清空所有数据
        /// </summary>
        /// <returns></returns>
        int Clear();

        /// <summary>
        /// 保存视频缓存记录
        /// </summary>
        /// <param name="info"></param>
        /// <returns></returns>
        int Save(AVInfo info);

        /// <summary>
        /// 根据ID删除缓存记录
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        int Delete(string id);

        /// <summary>
        /// 查询视频缓存
        /// </summary>
        /// <param name="Sim"></param>
        /// <param name="ChannelId"></param>
        /// <param name="StreamType"></param>
        /// <param name="StartTime"></param>
        /// <param name="EndTime"></param>
        /// <returns></returns>
        DataTable QueryDataTable(string Sim, int ChannelId, int StreamType, long StartTime, long EndTime);

        /// <summary>
        /// 查询视频缓存
        /// </summary>
        /// <param name="Sim"></param>
        /// <param name="ChannelId"></param>
        /// <param name="StreamType"></param>
        /// <param name="StartTime"></param>
        /// <param name="EndTime"></param>
        /// <returns></returns>
        List<AVInfo> Query(string Sim,int ChannelId,int StreamType,long StartTime,long EndTime);

       
    }
}
```

实现数据接口：

```
using FileCacheLocaldb.Utils;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileCacheLocaldb
{
    /// <summary>
    /// 实现音视频缓存查询接口
    /// </summary>
    public class AVInfoCache:ICache
    {
        DBSQLHelper sqlHelper = new DBSQLHelper();

        /// <summary>
        /// 清空所有数据
        /// </summary>
        /// <returns></returns>
        public int Clear()
        {
            string sql = "DELETE FROM dbo.AVINFO";
            return sqlHelper.ExecuteSql(sql);
        }

        /// <summary>
        /// 保存视频缓存记录
        /// </summary>
        /// <param name="info"></param>
        /// <returns></returns>
        public int Save(AVInfo info)
        {
            if (string.IsNullOrEmpty(info.id))
            {
                info.id= System.Guid.NewGuid().ToString();
            }
            string sql = "insert into dbo.AVINFO (F_ID,F_SIM,F_CHANNEL_ID,F_STREAM_TYPE,F_START_TIME,F_END_TIME,F_FILE_PATH) values (";
            // F_ID
            sql += "'" + info.id + "'" + ",";
            // F_SIM
            sql += "'" + info.Sim + "'" + ",";
            // F_CHANNEL_ID
            sql += info.Channel + ",";
            // F_STREAM_TYPE
            sql += info.StreamType + ",";
            // F_START_TIME
            sql += info.StartTime + ",";
            // F_END_TIME
            sql += info.EndTime + ",";
            // F_FILE_PATH
            sql += "'" + info.FilePath + "'" + "";
            sql += ")";
            return sqlHelper.ExecuteSql(sql);
        }

        /// <summary>
        /// 根据ID删除缓存记录
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public int Delete(string id)
        {
            if (string.IsNullOrEmpty(id))
            {
                return -1;
            }
            else
            {
                string sql = "DELETE FROM dbo.AVINFO where F_ID="+ id;
                return sqlHelper.ExecuteSql(sql);
            }
        }

        /// <summary>
        /// 查询视频缓存
        /// </summary>
        /// <param name="Sim"></param>
        /// <param name="ChannelId"></param>
        /// <param name="StreamType"></param>
        /// <param name="StartTime"></param>
        /// <param name="EndTime"></param>
        /// <returns></returns>
        public DataTable QueryDataTable(string Sim, int ChannelId, int StreamType, long StartTime, long EndTime)
        {
            string sql = "select * from dbo.AVINFO where 1=1 ";
            if (!string.IsNullOrEmpty(Sim))
            {
                sql += " and F_SIM=" + "'" + Sim + "'";
            }
            if (ChannelId >= 0 && ChannelId <= 37)
            {
                sql += " and F_CHANNEL_ID=" + ChannelId;
            }

            if (StreamType >= 0 && StreamType <= 2)
            {
                sql += " and F_STREAM_TYPE=" + StreamType;
            }

            sql += " and (";
            sql += " ( F_START_TIME between " + StartTime + " and " + EndTime + ")";
            sql += " or (F_END_TIME between " + StartTime + " and " + EndTime + ")";
            sql += ")";
            return sqlHelper.GetSelect(sql);
        }

        /// <summary>
        /// 查询视频缓存
        /// </summary>
        /// <param name="Sim"></param>
        /// <param name="ChannelId"></param>
        /// <param name="StreamType"></param>
        /// <param name="StartTime"></param>
        /// <param name="EndTime"></param>
        /// <returns></returns>
        public List<AVInfo> Query(string Sim, int ChannelId, int StreamType, long StartTime, long EndTime)
        {
            DataTable dataTable = this.QueryDataTable(Sim, ChannelId, StreamType, StartTime, EndTime);
            if (dataTable!=null&& dataTable.Rows!=null&&dataTable.Rows.Count>0)
            {
                List<AVInfo> list = new List<AVInfo>();
                AVInfo avInfo = null;
                for (var i = 0; i < dataTable.Rows.Count; i++)
                {
                    avInfo = new AVInfo();
                    avInfo.id =dataTable.Rows[i]["F_ID"].ToString();
                    avInfo.Sim = dataTable.Rows[i]["F_SIM"].ToString();
                    avInfo.Channel = int.Parse(dataTable.Rows[i]["F_CHANNEL_ID"].ToString());
                    avInfo.StreamType = int.Parse(dataTable.Rows[i]["F_STREAM_TYPE"].ToString());
                    avInfo.StartTime = long.Parse(dataTable.Rows[i]["F_START_TIME"].ToString());
                    avInfo.EndTime = long.Parse(dataTable.Rows[i]["F_END_TIME"].ToString());
                    avInfo.FilePath = dataTable.Rows[i]["F_FILE_PATH"].ToString();
                    list.Add(avInfo);
                }
                return list;
            }
            else
            {
                return null;
            }
        }
    }
}
```



设计界面：



![](https://img-blog.csdn.net/20180620111101279?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

运行效果：

![](https://img-blog.csdn.net/20180620111131781?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

实例下载地址：[https://download.csdn.net/download/boonya/1048787](https://download.csdn.net/download/boonya/10487872)

**注意：多线程操作数据库可能会出现访问失败的情况，不建议使用此数据库作为大型商用数据库的方案，请选择比较成熟稳定的数据库方案。**



