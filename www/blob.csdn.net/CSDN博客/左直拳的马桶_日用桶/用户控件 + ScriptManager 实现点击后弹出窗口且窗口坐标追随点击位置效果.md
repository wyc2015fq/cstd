# 用户控件 + ScriptManager 实现点击后弹出窗口且窗口坐标追随点击位置效果 - 左直拳的马桶_日用桶 - CSDN博客
2012年09月11日 10:29:33[左直拳](https://me.csdn.net/leftfist)阅读数：2528标签：[javascript																[asp.net																[triggers																[object																[div																[server](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=div&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=triggers&t=blog)](https://so.csdn.net/so/search/s.do?q=asp.net&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[.NET																[Web																[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)](https://blog.csdn.net/leftfist/article/category/674421)](https://blog.csdn.net/leftfist/article/category/94497)
我之前做了一个用于打标签的用户控件，结合ASP.NET的ScriptManager，可以实现页面部分更新的AJAX效果。这段时间忙着网站改版，这个用户控件不想就那么丢掉，所以想稍加改进，然后继续使用。
现在有个问题：用户点击“添加标签”，弹出这个打标签控件。很自然，这个控件应该在点击位置附近，点击位置不同，弹出位置也应该跟着变化。
怎么样才能实现这种效果？
不能在c#代码里面设置这个弹出位置，比如将点击坐标传给后台代码，然后在后台代码里设置CSS，不起效果的。
只能依靠JS了。但是这个JS该怎么触发呢？象jquery，有回调函数。但ASP.NET的AJAX，有这个东西吗？
有的。
【用户控件页面代码 】
```
<div id="pt_divPasteTag" style="position:absolute;width:400px;">
            <div class="pt_t" style="width:100px;">打标签</div>
</div>
<div>
    <script type="text/javascript" language="javascript">
        var _pt_x = 0;
        var _pt_y = 0;
        function pt_ShowBox(btn) {//此函数供调用页面激发按钮调用
            triger = $("#" + btn.id);
            _pt_x = triger.position().left;
            _pt_y = triger.position().top;
            $("#" + pt_pastebtn).click();
        }
        function pt_OffsetBox() {//此函数供调用页面委托程序使用
            var box = $("#pt_divPasteTag");
            box.css("left",_pt_x);
            box.css("top",_pt_y);
        }
    </script>
</div>
```
【用户控件后台代码】
```
public delegate void ClickEventHandler(object sender, EventArgs e);
    public event ClickEventHandler ShowClick;
    protected void btnPaste_Click(object sender, EventArgs e)
    {
        if (ShowClick != null)
        {//执行受委托的方法
            ShowClick(this, e);
        }
        this.Show();
    }
```
【调用页面的页面代码】
```
<asp:ScriptManager ID="ScriptManager1" runat="server" EnablePartialRendering="True" EnableViewState="false">
    </asp:ScriptManager>
            
    <div>
        <asp:UpdatePanel ID="UpdatePanel1" runat="server" UpdateMode="Conditional">
        <Triggers>
        </Triggers>
        <ContentTemplate>
            <div>
                <div id="divPaste" onclick="pt_ShowBox(this)"><a id="lnkPaste" href="javascript:;">添加</a></div>
            </div>
            <uc1:PasteTag ID="PasteTag1" runat="server" OnShowClick="PasteTag1_ShowClick"/>
        </ContentTemplate>
        </asp:UpdatePanel>
    </div>
```
【调用页面的后台代码】
```
protected void PasteTag1_ShowClick(object sender, EventArgs e)
    {
        ScriptManager.RegisterStartupScript(
this.UpdatePanel1
, this.GetType()
, "pt_OffsetBox"
, "pt_OffsetBox()"
, true);    
}
```
最为关键的地方，就是【调用页面的后台代码】里，
```
ScriptManager.RegisterStartupScript(
this.UpdatePanel1
, this.GetType()
, "pt_OffsetBox"
, "pt_OffsetBox()"
, true);
```
使得打标签控件弹出以后，随之调用JS函数 pt_OffsetBox，对位置进行修正。

