# javascript setTimeout和setInterval计时的区别详解 - z69183787的专栏 - CSDN博客
2014年03月08日 09:51:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：668
setTimeout方法是定时程序，也就是在什么时间以后干什么。干完了就拉倒。
setInterval方法则是表示间隔一定时间反复执行某操作。
如果用setTimeout实现setInerval的功能，就需要在执行的程序中再定时调用自己才行。如果要清除计数器需要 根据使用的方法不同，调用不同的清除方法：
例如：(1):
复制代码代码如下:
t=setTimeout('northsnow()',1000);
clearTimeout(t);
(2)：
t=setInterval('northsnow()',1000);
clearInteval(t);
setTimeout()
语法
复制代码代码如下:
var t=setTimeout("javascript语句",毫秒);
第一个参数是含有 JavaScript 语句的字符串。这个语句可能诸如 "alert('5 seconds!')"，或者对函数的调用，诸如 alertMsg()"。
第二个参数指示从当前起多少毫秒后执行第一个参数。
提示：1000 毫秒等于一秒。
实例
当下面这个例子中的按钮被点击时，一个提示框会在5秒中后弹出。
复制代码代码如下:
<html>
  <head>
<script type="text/javascript">
function timedMsg() {
 var t=setTimeout("alert('5 seconds!')",5000);
 }
</script>
  </head>
  <body>
    <form>
      <input type="button" value="运行计时!" onClick="timedMsg()">
    </form>
  </body>
</html>
setInterval()
setInterval() 方法可按照指定的周期（以毫秒计）来调用函数或计算表达式。
setInterval() 方法会不停地调用函数，直到 clearInterval() 被调用或窗口被关闭。由 setInterval() 返回的 ID 值可用作 clearInterval() 方法的参数。
语法
setInterval(code,millisec[,"lang"])
实例 
复制代码代码如下:
<html>
<head>
  <meta charset="utf-8"/>
  <title>setInterval实例 - 新锐工作室</title>
</head>
<body>
<script language="javascript">
  function endo(){
    alert("你好");
  }
  window.setInterval('endo()',5000);
</script>
</form>
<p> (c) Endige.net </p>
</body>
</html>
传参方法
无论是window.setTimeout还是window.setInterval，在使用函数名作为调用句柄时都不能带参数， 而在 许多场合必须要带参数，这就需要想方法解决。例如对于函数hello(_name)，它用于针对用户名显示欢
迎信息：
复制代码代码如下:
var userName="jack";
//根据用户名显示欢迎信息
function hello(_name){
  alert("hello,"+_name);
}
这时，如果企图使用以下语句来使hello函数延迟3秒执行是不可行的：
window.setTimeout(hello(userName),3000);
这将使hello函数立即执行，并将返回值作为调用句柄传递给setTimeout函数，其结果并不是程序需要的。 而使用字符串形式可以达到想要的结果：
这里的字符串是一段JavaScript代码，其中的userName表示的是变量。 但这种写法不够直观，而且有些场合必须使用函数名，下面用一个小技巧来实现带参数函数的调用：
复制代码代码如下:
<script language="JavaScript" type="text/javascript">
<!--
var userName="jack";
//根据用户名显示欢迎信息
function hello(_name){
  alert("hello,"+_name);
}
//创建一个函数，用于返回一个无参数函数
function _hello(_name){
  return function(){
    hello(_name);
  }
}
window.setTimeout(_hello(userName),3000);
//-->
</script>
这里定义了一个函数_hello，用于接收一个参数，并返回一个不带参数的函数，
在这个函数内部使用了外部函数的参数，从而对其调用，不需要使用参数。在 window.setTimeout函数中，使用_hello(userName)来返回一个不带参数的
函数句柄，从而实现了参数传递的功能。
A.当要执行的方法中不需要参数时
复制代码代码如下:
<script type=”text/javascript”> 
//循环执行，每隔3秒钟执行一次showalert（） 
window.setInterval(showalert, 3000);
function showalert() {
  alert(“你好”); 
}
//定时执行，5秒后执行show()
window.setTimeout(show,5000);
function show() {
  alert(“Hello”);
}
</script>
B.当要执行的方法中需要参数时
复制代码代码如下:
<script type=”text/javascript”> 
//循环执行，每隔3秒钟执行一次 showalert（）
window.setInterval(function(){ 
  showalert(“你好！”); 
}, 3000); 
function showalert(mess) { 
  alert(mess); 
} 
//定时执行，5秒后执行showalert()
window.setTimeout(function(){ 
  showalert(“Hello”);
},5000); 
</script>
