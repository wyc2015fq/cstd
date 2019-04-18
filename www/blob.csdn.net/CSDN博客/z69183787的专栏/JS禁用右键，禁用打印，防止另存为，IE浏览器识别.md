# JS禁用右键，禁用打印，防止另存为，IE浏览器识别 - z69183787的专栏 - CSDN博客
2014年04月01日 13:28:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5865

```java
function isIE6(){
String.prototype.inc=function(k1,k2){return k2==null?this.indexOf(k1)>-1?true:false:(k2+this+k2).indexOf(k2+k1+k2)>-1?true:false}
var agent=navigator.userAgent
var isIE,isIE4,isIE5,isIE6,isOpr,isMoz
isOpr=agent.inc("Opera")
isIE=agent.inc("IE")&&!isOpr
isIE4=agent.inc("IE 4")
isIE5=agent.inc("IE 5")||agent.inc("IE 4")
isIE6=isIE&&!isIE5
isMoz=agent.inc("Mozilla")&&!isOpr&&!isIE
if(!isIE6){
alert("您使用的浏览器不是IE或版本太低，建议使用IE6.0及以上版本的浏览器进行报名！");
}
}
```
oncontextmenu="window.event.returnValue=false" style="overflow-y: hidden; overflow-x: hidden"ajs9     leftmargin="0" topmargin="0"
<body oncontextmenu="return false"></body>
<!--禁止网页另存为： --> 
<noscript><iframe src=*.html></iframe></noscript>
<!-- 禁止选择文本： --> 
<script type="text/javascript">
var omitformtags=["input", "textarea", "select"]
omitformtags=omitformtags.join("|")
function disableselect(e){ 
if (omitformtags.indexOf(e.target.tagName.toLowerCase())==-1) 
return false 
}
function reEnable(){ 
return true 
}
if (typeof document.onselectstart!="undefined") 
document.onselectstart=new Function ("return false") 
else{ 
document.onmousedown=disableselect 
document.onmouseup=reEnable 
} 
</script>
<!-- 禁用右键: --> 
<script> 
function stop(){ 
return false; 
} 
document.oncontextmenu=stop; 
</script>
1. oncontextmenu="window.event.returnvalue=false" 将彻底屏蔽鼠标右键 
<table border oncontextmenu=return(false)><td>no</table> 可用于Table
2. <body onselectstart="return false"> 取消选取、防止复制
3. onpaste="return false" 不准粘贴
4. oncopy="return false;" oncut="return false;" 防止复制
5. <link rel="Shortcut Icon" href="favicon.ico"> IE地址栏前换成自己的图标
6. <link rel="Bookmark" href="favicon.ico"> 可以在收藏夹中显示出你的图标
7. <input style="ime-mode:-Disabled"> 关闭输入法
8. 永远都会带着框架 
<script language="javascript"><!-- 
if (window == top)top.location.href = "frames.htm"; //frames.htm为框架网页 
// --></script>
9. 防止被人frame 
<SCRIPT LANGUAGE=javascript><!-- 
if (top.location != self.location)top.location=self.location; 
// --></SCRIPT>
11. <input type=button value=查看网页源代码 
onclick="window.location = `view-source:`+ [http://www.tonightdream.com/](http://www.tonightdream.com/)`";>
12.删除时确认 
<a href=`javascript:if(confirm("确实要删除吗?"location="boos.asp?&areyou=删除&page=1"`>删除</a>
13. 取得控件的绝对位置 
//javascript 
<script language="javascript"> 
function getIE(E){ 
var t=e.offsetTop; 
var l=e.offsetLeft; 
while(e=e.offsetParent){ 
t+=e.offsetTop; 
l+=e.offsetLeft;
<!--右键开始-->
<script language="JavaScript">
<!--
if (window.Event) 
document.captureEvents(Event.MOUSEUP);
function nocontextmenu() 
{
event.cancelBubble = true
event.returnValue = false;
return false;
}
function norightclick(e) 
{
if (window.Event) 
{
if (e.which == 2 || e.which == 3)
   return false;
}
else
if (event.button == 2 || event.button == 3)
{
   event.cancelBubble = true
   event.returnValue = false;
   return false;
}
}
document.oncontextmenu = nocontextmenu; // for IE5+
document.onmousedown = norightclick; // for all others
//-->
   </script>
<!-- 禁止选择文本： -->
<script type="text/javascript">
var omitformtags=["input", "textarea", "select"]
omitformtags=omitformtags.join("|")
function disableselect(e){ 
if (omitformtags.indexOf(e.target.tagName.toLowerCase())==-1) 
return false 
}
function reEnable(){ 
return true 
}
if (typeof document.onselectstart!="undefined") 
document.onselectstart=new Function ("return false") 
else{ 
document.onmousedown=disableselect 
document.onmouseup=reEnable 
} 
   </script>
<script language="javascript" type="text/javascript">
<!--
function key(){ 
if(event.shiftKey){
window.close();}
//禁止shift
if(event.altKey){
window.close();}
//禁止alt
if(event.ctrlKey){
window.close();}
//禁止ctrl
return false;}
//document.onkeydown=key;
if (window.Event)
document.captureEvents(Event.MOUSEUP);
//swordmaple javascript article.
//from [http://www.jx165.com/](http://www.jx165.com/)
function nocontextmenu(){
event.cancelBubble = true
event.returnValue = false;
return false;}
function norightclick(e){
if (window.Event){
if (e.which == 2 || e.which == 3)
return false;}
else
if (event.button == 2 || event.button == 3){
event.cancelBubble = true
event.returnValue = false;
return false;}
}
//禁止右键
document.oncontextmenu = nocontextmenu; // for IE5+
document.onmousedown = norightclick; // for all others
//-->
</script>
---------------
要真正禁止打印页面是办不到的，但是如果能设置让用户打印到的是空白，也算是实现禁止打印的目的了
<html> 
<head><title>不可打印的页面</title> 
<style> 
@media print{ 
body{display:none} 
} 
</style> 
</head> 
<body> 
文档主要内容 
</html> 
