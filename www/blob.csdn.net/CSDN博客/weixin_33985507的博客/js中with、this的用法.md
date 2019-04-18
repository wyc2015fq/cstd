# js中with、this的用法 - weixin_33985507的博客 - CSDN博客
2009年09月09日 15:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
**with 语句** 为一个或一组语句指定默认对象。
用法：with (<对象>) <语句>;
with 语句通常用来缩短特定情形下必须写的代码量。在下面的例子中，请注意 Math 的重复使用：
> 
x = Math.cos(3 * Math.PI) + Math.sin(Math.LN10);
y = Math.tan(14 * Math.E);
当使用 with 语句时，代码变得更短且更易读：
> 
with (Math) {
   x = cos(3 * PI) + sin(LN10);
   y = tan(14 * E);
}
**this 对象** 返回“当前”对象。在不同的地方，this 代表不同的对象。如果在 JavaScript 的“主程序”中（不在任何 function 中，不在任何事件处理程序中）使用 this，它就代表 window 对象；如果在 with 语句块中使用 this，它就代表 with 所指定的对象；如果在事件处理程序中使用 this，它就代表发生事件的对象。
一个常用的 this 用法：
> 
<script>
...
function check(formObj) {
   ...
}
...
</script>
<body ...>
...
<form ...>
...
<input type="text" ... onchange="check(**this**.form)">
...
</form>
...
</body>
这个用法常用于立刻检测表单输入的有效性。
