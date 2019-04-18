# js实现局部打印及其打印预览 - z69183787的专栏 - CSDN博客
2014年07月29日 14:39:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7475
```
JS 实现简单的页面局部打印 
function preview(oper)
...{
if (oper < 10)...{
bdhtml=window.document.body.innerHTML;//获取当前页的html代码
sprnstr="<!--startprint"+oper+"-->";//设置打印开始区域
eprnstr="<!--endprint"+oper+"-->";//设置打印结束区域
prnhtml=bdhtml.substring(bdhtml.indexOf(sprnstr)+18); //从开始代码向后取html
prnhtml=prnhtml.substring(0,prnhtml.indexOf(eprnstr));//从结束代码向前取html
window.document.body.innerHTML=prnhtml;
window.print();
window.document.body.innerHTML=bdhtml;
} else ...{
window.print();
}
}
使用很简单 将页面内要打印的内容加入中间<!--startprint1-->XXXXX<!--endprint1-->
再加个打印按纽 onclick=preview(1)
```
<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Test.aspx.cs" Inherits="WebApplication2.Test" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "[http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd](http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd)">
<html xmlns="[http://www.w3.org/1999/xhtml](http://www.w3.org/1999/xhtml)">
<head runat="server">
    <title></title>
    <script type="text/javascript" language="javascript">
        function printdiv() {
            var newstr = document.getElementById("PrintContentDiv").innerHTML;   //获得需要打印的内容
            // alert(newstr);
            var oldstr = document.body.innerHTML;   //保存原先网页的代码
            document.body.innerHTML = newstr; //将网页内容更改成需要打印
            window.print();
            document.body.innerHTML = oldstr;   //将网页还原
            return false;
        }
        //打印页面预览
        function printpreview() {
            var WebBrowser = '<OBJECT ID="WebBrowser1" WIDTH=0 HEIGHT=0 CLASSID="CLSID:8856F961-340A-11D0-A96B-00C04FD705A2"></OBJECT>';
            document.getElementById("divButton").style.display = "none";//隐藏打印及其打印预览页面
            document.body.insertAdjacentHTML('beforeEnd', WebBrowser); //在body标签内加入html（WebBrowser activeX控件）
            WebBrowser1.ExecWB(7, 1); //打印预览
        }
    </script>
</head>
<body>
    <form id="form1" runat="server">
    <div id="PrintContentDiv">
        序号总和:15<br />
        年龄总和:109<br />
        成绩总和:413<br />
        序号年龄成绩总和112<br />
        序号年龄成绩总和104<br />
        序号年龄成绩总和93<br />
        序号年龄成绩总和115<br />
        序号年龄成绩总和113<br />
        序号 姓名 年龄 学号 成绩<br />
        1 田 23 XH001 88<br />
        2 笑 22 XH002 80<br />
        3 高 20 XH003 70<br />
        4 汗 21 XH004 90<br />
        5 敏 23 XH005 85<br />
    </div>
    <div id="divButton">
        <input type="button" onclick="printdiv()" value="打 印" /><br />
