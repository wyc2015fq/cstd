
# js或jquery实现页面打印可局部打印 - anlcy - 博客园






# [js或jquery实现页面打印可局部打印](https://www.cnblogs.com/camilla/p/7593088.html)
1、js实现（可实现局部打印）
代码如下:
<html>
<title>js打印</title>
<head></head><body>
<input id=btnprint type=button value=打印 onclick=javascript:window.print(); />
<input id=btnprint type=button value=打印预览 onclick=preview(1) />
<style type=text/css media=print>
.noprint{display : none }
</style>

<p class="noprint">不需要打印的地方</p>
<script>
function preview(oper)
{
if (oper < 10)
{
bdhtml=window.document.body.innerhtml;//获取当前页的html代码
sprnstr=<!--startprint+oper+-->;//设置打印开始区域
eprnstr=<!--endprint+oper+-->;//设置打印结束区域
prnhtml=bdhtml.substring(bdhtml.indexof(sprnstr)+18); //从开始代码向后取html
prnhtml=prnhtml.substring(0,prnhtml.indexof(eprnstr));//从结束代码向前取html
window.document.body.innerhtml=prnhtml;
window.print();
window.document.body.innerhtml=bdhtml;
} else {
window.print();
}
}
</script>
<p>xxxxx</p>
<!--startprint1-->要打印的内容<!--endprint1-->
</body>
</html>
2、调用windows底层打印，报安全警告，不建议使用（不支持局部打印）
代码如下:
<html>
<head>
<title>javascript打印-打印页面设置-打印预览代码</title>
<meta http-equiv=content-type content=text/html; charset=gb2312 />
<script language=javascript>
function printsetup(){
// 打印页面设置
wb.execwb(8,1);
}
function printpreview(){
// 打印页面预览
wb.execwb(7,1);
}
function printit()
{
if (confirm('确定打印吗？')) {
wb.execwb(6,6);
}
}
</script>
</head>
<body>
<div align=center>
<object id=wb height=0 width=0
classid=clsid:8856f961-340a-11d0-a96b-00c04fd705a2 name=wb></object>
<input onclick=javascript:printit() type=button value=打印 name=button_print />
<input onclick=javascript:printsetup(); type=button value=打印页面设置 name=button_setup />
<input onclick=javascript:printpreview(); type=button value=打印预览 name=button_show />
一按开始的减肥了卡时间段
</div>
</body>
</html>
3、jquery实现（支持局部打印）
代码如下:
<html>
<head>
<script type=text/javascript src=jquery-1.6.4.js></script>
<script>
$(function(){
$(input\#biuuu_button).click(function(){
$(div\#myprintarea).printarea();
});
</script>
</head>
<body>
<input id=biuuu_button type=button value=打印></input>
<div id=myprintarea>.....文本打印部分.....</div> <div class="quote_title">引用</div><div class="quote_div"></div>
</body>
</html>





