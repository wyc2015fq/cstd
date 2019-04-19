# WinForm-GridView - 毛小亮 - 博客园
# [WinForm-GridView](https://www.cnblogs.com/xianerwonder/p/4368683.html)
前端：
```csharp;gutter
<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="CKXM.aspx.cs" Inherits="WebMvc.CKXM" %>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
        </br>
        </br>
        <div style="text-align:Center";><label style="font-size: 35px; font-style: normal; font-weight: 600;">查看验收项目</label></div>
        </br>
        </br>
        <div>
            <asp:GridView ID="GridView1" runat="server" HorizontalAlign="Center" 
                RowStyle-Height="50px" RowStyle-HorizontalAlign="Center" HeaderStyle-Width="200" Width="1000px">
            </asp:GridView>
        </div>
    </form>
</body>
</html>
```
　　后端：
```csharp;gutter
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using Data.MSSQL;
namespace WebMvc
{
    public partial class CKXM : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            DBHelper dbhelper = new DBHelper("data source=.;initial catalog=sde;uid=sa;pwd=jt;");
            DataTable db = dbhelper.GetDataTable("select XMND,XMMC,XZ,XMZT from YSXM");
 
            db.Columns[0].ColumnName = "项目年度";
            db.Columns[1].ColumnName = "项目名称";
            db.Columns[2].ColumnName = "项目性质";
            db.Columns[3].ColumnName = "项目状态";
            //在第一排添加一个序号
            db.Columns.Add("序号");
            db.Columns[4].SetOrdinal(0);
            for (int i = 0; i < db.Rows.Count; i++)
            {
                db.Rows[i][0] = i + 1;
            }
            
            GridView1.DataSource = db.DefaultView;
            GridView1.DataBind();
        }
    }
}
```
