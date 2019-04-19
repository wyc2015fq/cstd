# 控件Repeater的嵌套使用 - 左直拳的马桶_日用桶 - CSDN博客
2008年01月15日 18:42:00[左直拳](https://me.csdn.net/leftfist)阅读数：1598
控件Repeater的嵌套使用
左直拳
可以嵌套使用Repeater。
页面代码：
<asp:RepeaterID="rpParent"runat="server"**OnItemCommand****="rpParent_ItemCommand"**>
<HeaderTemplate>
<tablewidth="100%"height="100%"border="0"cellspacing="0"cellpadding="5"style="text-align:left;">
<tr>
<td>父字段1</td>
<td>父字段2</td>
<td></td>
</tr>
</HeaderTemplate>
<ItemTemplate>
<tr>
<td><%# Eval("Parent1")%><inputid="txtParent1"type="hidden"value='<%# Eval("Parent1")%>'runat="server"/></td>
<td><%# Eval("Parent2")%></td>
<td><asp:LinkButtonID="lnkShowChild"runat="server"**CommandName****="ShowChild"**>展开孩子</asp:LinkButton></td>
</tr>
<tr><tdcolspan="3">
<asp:RepeaterID="rpChild"runat="server">
<HeaderTemplate>
<tablewidth="100%"height="100%"border="0"cellspacing="0"cellpadding="5"style="text-align:left;">
<tr><td>子字段1</td><td>子字段2</td>
</tr>
</HeaderTemplate>
<ItemTemplate>
<tr><td><%# Eval("Child1")%></td><td><%# Eval("Child2")%></td></tr>
</ItemTemplate>
<FooterTemplate>
</table>
</FooterTemplate>
</asp:Repeater>
</td></tr>
</ItemTemplate>
<FooterTemplate>
</table>
</FooterTemplate>
</asp:Repeater>
后台代码：
protectedvoid rpParent_ItemCommand(object source, RepeaterCommandEventArgs e)
{
if (e.CommandName == "ShowChild")
{
Repeater rp = (Repeater)e.Item.FindControl("rpChild");
HtmlInputHiddentxtChild = (HtmlInputHidden)e.Item.FindControl("txtChild");
rp.DataSource = _GetData(txtChild.Value);
rp.DataBind();
}
}
有2个地方值得一提：
1、象 <%# Eval("Child1")%>这类的写法，是对距离最近的Repeater而言。象这里的例子，子Repeater嵌套在父Repeater里，但位于子Repeater里的<%# Eval("Child1")%>不会与父控件有冲突；
2、后台代码这里支持事件。不象网上找到的许多例子，都是装载时一口气打开。而是点击“打开孩子”才展开。参数是存放在一个Hidden的HTML文本框里。之前听说只有ASP.NET的控件才有可能在后台FindControl，但现在看来也可以，也许是2。0的缘故吧。
