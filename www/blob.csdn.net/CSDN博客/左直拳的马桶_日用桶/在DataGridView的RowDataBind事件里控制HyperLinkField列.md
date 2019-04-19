# 在DataGridView的RowDataBind事件里控制HyperLinkField列 - 左直拳的马桶_日用桶 - CSDN博客
2008年05月23日 16:13:00[左直拳](https://me.csdn.net/leftfist)阅读数：2149标签：[hyperlink																[object																[asp](https://so.csdn.net/so/search/s.do?q=asp&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=hyperlink&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
在DataGridView的RowDataBind事件里控制HyperLinkField列
左直拳
DataGridView里有一种列是HyperLinkField类型：
<asp:HyperLinkFieldDataTextField="Title"SortExpression="Title"DataNavigateUrlFields="Id"DataNavigateUrlFormatString="http://www.fuckman.com/fuck.aspx?Id={0}"HeaderText="标题"Target="_blank"/>
其实就是一种超链接。
如果想控制DataGridView里一些样式，可以在页面里面写<%# Eval() %>之类，也可以在代码里用DataGridView的RowDataBind事件控制。
可是在代码里控制这个HyperLinkField却遇到了麻烦。
首先要把这一列找出来，找出来之后不能强行转换成HyperLinkField，这样编译会出错，要转换成HyperLink。奇怪吧。
protectedvoid gvWebSite_RowDataBound(object sender, GridViewRowEventArgs e)
{
if (e.Row.RowType == DataControlRowType.DataRow)
{
DataRowView gv = (DataRowView)e.Row.DataItem;
((HyperLink)e.Row.Cells[1].Controls[0]).ForeColor = System.Drawing.Color.Red;
}
}
