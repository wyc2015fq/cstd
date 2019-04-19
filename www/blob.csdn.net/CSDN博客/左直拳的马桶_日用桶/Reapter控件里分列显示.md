# Reapter控件里分列显示 - 左直拳的马桶_日用桶 - CSDN博客
2007年01月08日 18:10:00[左直拳](https://me.csdn.net/leftfist)阅读数：2653标签：[table																[asp																[css																[div																[float																[server](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=div&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)](https://so.csdn.net/so/search/s.do?q=asp&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
Reapter控件里分列显示
左直拳
Reapter控件也可以象DataList那样分列显示。
方法是使用CSS：
<divclass="divRepeater">
<asp:Repeaterrunat="server"ID="rptStakeOut"EnableViewState="false">
<ItemTemplate>
**<li>**
<tablecellpadding="0"cellspacing="0"width="100%">
<tr><td><%#Eval("Name")%></td></tr>
</table>
**</li>**
</ItemTemplate>
</asp:Repeater>
</div>
看到没，用了一个标记<li>将显示项包含在内。
相应地，定义<li>的CSS样式：
.divRepeater {……}
.divRepeater li{float:left;**width:33%;**text-align:left; }
这样，Repeater中每行即显示3列。如果2列，就改成width:50%。

