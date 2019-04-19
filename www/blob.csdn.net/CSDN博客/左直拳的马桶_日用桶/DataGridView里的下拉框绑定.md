# DataGridView里的下拉框绑定 - 左直拳的马桶_日用桶 - CSDN博客
2012年02月15日 11:19:18[左直拳](https://me.csdn.net/leftfist)阅读数：3387标签：[server																[asp																[html																[rss](https://so.csdn.net/so/search/s.do?q=rss&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=asp&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
我对DataGridView一直心存敬畏，觉得功能强大。功能强大就意味着复杂。复杂就意味着对老眼昏花、记忆衰退的我而言，使用起来是件痛苦的事。
它的功能太强大了，有好多东西我都不知道。
下面这段代码是编辑某行，其中出现下拉框的：
```
GridViewRow aRow = gvWebSite.Rows[e.NewEditIndex];
        int wid = Convert.ToInt32(gvWebSite.DataKeys[e.NewEditIndex]["WebSiteID"]);
        int ctype = Convert.ToInt32(gvWebSite.DataKeys[e.NewEditIndex]["CatchType"]);
        int stype = Convert.ToInt32(gvWebSite.DataKeys[e.NewEditIndex]["SiteTypeId"]);
        //抓型
        DropDownList ddlCatchType = aRow.Cells[7].Controls[1] as DropDownList;
        ddlCatchType.Items.Add(new ListItem("普通","0"));
        ddlCatchType.Items.Add(new ListItem("不考虑重复", "1"));
        ddlCatchType.SelectedValue = ctype.ToString();
        //站点类型
        DropDownList ddlSiteType = aRow.Cells[8].Controls[1] as DropDownList;
        ddlSiteType.Items.Add(new ListItem("HTML", "0"));
        ddlSiteType.Items.Add(new ListItem("RSS", "1"));
        ddlSiteType.SelectedValue = stype.ToString();
```
下拉框浮现的时候，它的选中值应该是现有的值。那么现有的值该怎么获取呢？
原来可以保存在DataKeys里的。原来DataKeys可以不只保存一个值！我还以为那是主键呢！只要在DataGridView声明里这样设置：
`DataKeyNames="WebSiteID,CatchType,SiteTypeId"`
就行了。详细：
```
<asp:GridView runat="server" ID="gvWebSite" AutoGenerateColumns="false" BackColor="White"
            BorderColor="#CCCCCC" BorderStyle="None" BorderWidth="1px" CellPadding="1" Width="95%" EditRowStyle-VerticalAlign="Top"
            OnRowCancelingEdit="gvWebSite_RowCancelingEdit" DataKeyNames="WebSiteID,CatchType,SiteTypeId" OnRowDeleting="gvWebSite_RowDeleting"
            OnRowEditing="gvWebSite_RowEditing" OnRowUpdating="gvWebSite_RowUpdating">
```
注意，里面的键值名称区分大小写哟！比如
`gvWebSite.DataKeys[e.NewEditIndex]["WebSiteID"]`不能写成
`gvWebSite.DataKeys[e.NewEditIndex]["WebSiteId"]`
