# SQL SERVER 的 CLR表值函数 - 左直拳的马桶_日用桶 - CSDN博客
2010年07月09日 17:19:00[左直拳](https://me.csdn.net/leftfist)阅读数：2569
一、使用CLR表值函数的背景
在SQL SERVER里面，直接读取远程数据库的表，似乎会占用大量的内存，出现类似错误：
*链接服务器 "192.168.0.1" 的 OLE DB 访问接口 "SQLNCLI10" 报错。提供程序内存不足。 [SQLSTATE 42000] (错误 7399) 。*
我不知道读取远程数据库的表是一个什么样的原理，是首先从远程将该表一古脑全部加载到本地内存，然后再根据过滤条件来获得相应的记录，还是这个过滤在远程服务器完成，然后返回找到的记录？
从占用大量内存来看，我觉得应该是前者。
如果在远程数据库里面提供一个函数，获取什么记录，都通过这个函数，这样返回来的就是少量的记录，绝大部分的数据处理都已在远端完成，御敌于国门之外。
但是远程数据库的函数是不能直接使用的，会报“*不允许使用远程表值函数调用。*”的错误。存储过程可以，函数不行。只能通过OPENQUERY的方式来完成调用。但是，OPENQUERY又不能传递参数。（详见[如何给OPENQUERY传递参数](http://blog.csdn.net/leftfist/archive/2009/11/10/4792679.aspx)）
变通的办法是，在本地写一个CLR的表值函数，由它负责去访问远程函数，而本地访问它，等于间接访问远程函数。
之所以用CLR表值函数，是比较好传递参数给OPENQUERY，并且获得返回记录后，又能够装配好返回给本地调用方。假如不是函数而是存储过程，我都不知道调用方怎么去获取到这些记录。
二、编写CLR表值函数
using System;
using System.Data;
using System.Data.SqlClient;
using System.Data.SqlTypes;
using Microsoft.SqlServer.Server;
using System.Collections;
using System.Collections.Generic;
public partial class MyFunc
{
    [SqlFunction(DataAccess = DataAccessKind.Read, FillRowMethodName = "FillRow", TableDefinition = "Id INT,Name VARCHAR(300),CreateDate SMALLDATETIME")]
    public static IEnumerable GetDataById(SqlInt32 id)
    {
        string sql = String.Format("SELECT * FROM OPENQUERY([192.168.0.1],'SELECT * FROM [remotedb].dbo.cr_func_GetDataById({0})');", id);
        IList<model> items = new List<model>();
        using (SqlConnection conn = new SqlConnection("context connection = true"))
        {
            conn.Open();
            SqlCommand cmd = new SqlCommand(sql, conn);
            SqlDataAdapter da = new SqlDataAdapter(cmd);
            DataTable dt = new DataTable();
            try
            {
                da.Fill(dt);
            }
            catch
            {
                conn.Close();
            }
            foreach (DataRow dr in dt.Rows)
            {
                items.Add(new modelfull(dr));
            }
        }
        return items;
}
//这个方法一定要有，但究竟是什么时候被调用的，没搞明白
    static void FillRow (Object obj
        , out SqlInt32 iId
        , out SqlString sName
        , out SqlDateTime dCreateDate)
    {
        if (null != obj)
        {
            model item = (model)obj;
            iId = item.Id;
            sName = item.Name;
            dCreateDate = item.CreateDate;
        }
        else
        {
            iId = 0;
            sName = "";
            dCreateDate = DateTime.Now.AddYears(-4);
        }
    }
    struct model
    {
        public readonly SqlInt32 Id;
        public readonly SqlString Name;
        public readonly SqlDateTime CreateDate;
        public model(DataRow dr)
        {
            this.Id = Convert.ToInt32(dr["Id"]);
            this.Name = dr["Name"].ToString();
            this.CreateDate = Convert.ToDateTime(dr["CreateDate"]);
        }
    }
}
三、部署CLR表值函数
0、开启CLR
sp_configure 'show advanced options', 1; 
GO 
RECONFIGURE; 
GO 
sp_configure 'clr enabled', 1; 
GO 
RECONFIGURE; 
GO
1、    代码编写完后，编译成DLL。然后打开SQL SERVER的目标数据库 --》程序集 --》添加程序集，将DLL导入。给该程序集起个名字，比如叫myClrDll
2、如果代码更改，重新编译成DLL，可以用以下语句重新导入
ALTER ASSEMBLY [myClrDll]
FROM 'd:/code/temp/myClrDll.dll'
注意'd:/code/temp/myClrDll.dll'一定是数据库所在机器的路径。通常我们程序和数据库不同机器，所以这个路径常常搞错。
3、创建数据库表值函数，以使用这个CLR表值函数
USE [mydb]
GO
CREATE FUNCTION [dbo].[cr_func_getdatabyId](@Id [int])
RETURNS  TABLE (
    [Id] [int] NULL,
    [Name] [nvarchar](300) NULL,--VARCHAR不行，要用NVARCHAR
    [CreateDate] [smalldatetime] NULL
) WITH EXECUTE AS CALLER
AS 
EXTERNAL NAME [myClrDll].[myFunc].[GetDataById]
完成，从此您可以幸福地使用函数cr_func_getdatabyId鸟。
