# 实现IE下按回车提交 - 左直拳的马桶_日用桶 - CSDN博客
2011年09月05日 16:45:36[左直拳](https://me.csdn.net/leftfist)阅读数：1271标签：[ie																[textbox																[javascript																[button																[function																[object](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=textbox&t=blog)](https://so.csdn.net/so/search/s.do?q=ie&t=blog)
如果不加红色这句(btn.focus())，IE下，会先触发btn.click事件，但也接着触发一个什么默认的OnSubmit事件，反正是得不到想要的结果；FF下则正常。加了套红这句，就可以了。
页面代码：
<asp:TextBox ID="txtCondition" runat="server"></asp:TextBox>
<asp:Button Text="搜索" ID="btnSearch" runat="server" onclick="btnSearch_Click"/>
<script type="text/javascript" language="javascript">
    function mn_EnterDragon(e){
        var charCode = e.keyCode;
        if (charCode == 13) {
            var btn = document.getElementById("<%=btnSearch.ClientID %>");
btn.focus();//最关键是这一句
            btn.click();
        }
    }
</script>   
后台代码：
  protected void Page_Load(object sender, EventArgs e)
   {
        txtCondition.Attributes.Add("onkeydown", "mn_EnterDragon(event)");
  }
