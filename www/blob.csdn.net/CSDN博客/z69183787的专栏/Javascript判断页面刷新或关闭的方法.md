# Javascript判断页面刷新或关闭的方法 - z69183787的专栏 - CSDN博客
2013年12月17日 14:50:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6214
onbeforeunload与onunload事件
Onunload，onbeforeunload都是在刷新或关闭时调用，可以在<script>脚本中通过window.onunload来指定或者在<body>里指定。区别在于onbeforeunload在onunload之前执行，它还可以阻止onunload的执行。
Onbeforeunload也是在页面刷新或关闭时调用，Onbeforeunload是正要去服务器读取新的页面时调用，此时还没开始读取；而onunload则已经从服务器上读到了需要加载的新的页面，在即将替换掉当前页面时调用。Onunload是无法阻止页面的更新和关闭的。而 Onbeforeunload 可以做到。
页面加载时只执行onload     
页面关闭时先执行onbeforeunload，最后onunload
页面刷新时先执行onbeforeunload，然后onunload，最后onload。
1、onbeforeunload事件：
说明：目前三大主流浏览器中firefox和IE都支持onbeforeunload事件,opera尚未支持。
用法：
·object.onbeforeunload = handler
·<element onbeforeunload = “handler” … ></element>
描述：
事件触发的时候弹出一个有确定和取消的对话框，确定则离开页面，取消则继续待在本页。handler可以设一个返回值作为该对话框的显示文本。
触发于：
·关闭浏览器窗口
·通过地址栏或收藏夹前往其他页面的时候
·点击返回，前进，刷新，主页其中一个的时候
·点击 一个前往其他页面的url连接的时候
·调用以下任意一个事件的时候：click，document write，document open，document close，window close ，window navigate ，window NavigateAndFind,location replace,location reload,form submit.
·当用window open打开一个页面，并把本页的window的名字传给要打开的页面的时候。
·重新赋予location.href的值的时候。
·通过input type=”submit”按钮提交一个具有指定action的表单的时候。
可以用在以下元素：
·BODY, FRAMESET, window
平台支持：
IE4+/Win, Mozilla 1.7a+, Netscape 7.2+, Firefox0.9+
示例：
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>onbeforeunload测试</title>
<script>
function checkLeave(){
event.returnValue="确定离开当前页面吗？";
}
</script>
</head>
<body onbeforeunload="checkLeave()">
</body>
</html>
2、onunload事件
用法：
·object.onbeforeunload = handler
·<element onbeforeunload = "handler"></element>
描述：
当用户关闭一个页面时触发 onunload 事件。
触发于：
·关闭浏览器窗口
·通过地址栏或收藏夹前往其他页面的时候
·点击返回，前进，刷新，主页其中一个的时候
·点击 一个前往其他页面的url连接的时候
·调用以下任意一个事件的时候：click，document write，document open，document close，window close ，window navigate ，window NavigateAndFind,location replace,location reload,form submit.
·当用window open打开一个页面，并把本页的window的名字传给要打开的页面的时候。
·重新赋予location.href的值的时候。
·通过input type=”submit”按钮提交一个具有指定action的表单的时候。
示例：
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>onunload测试</title>
<script>
function checkLeave(){
alert("欢迎下次再来！");
}
</script>
</head>
<body onunload="checkLeave()">
</body>
</html>
一个判断页面是否真的关闭和刷新的好方法：
window.onbeforeunload=function (){
alert("===onbeforeunload===");
if(event.clientX>document.body.clientWidth && event.clientY < 0 || event.altKey){
alert("你关闭了浏览器");
}else{
alert("你正在刷新页面");
}
}
这段代码就是判断触发onbeforeunload事件时，鼠标是否点击了关闭按钮，或者按了ALT+F4来关闭网页，如果是，则认为系统是关闭网页，否则在认为系统是刷新网页。
