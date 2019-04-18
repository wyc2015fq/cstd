# window.open 打开窗口最大化。。。 - z69183787的专栏 - CSDN博客
2014年02月18日 16:08:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：25652
最近在做项目的时候用到了弹出窗口，并且弹出窗口要求最大化处理，在网上搜了一些关于弹出窗口的例子，都不行，后来找到了解决问题的方法：
一下是普通弹出窗口的例子：
```java
<SCRIPT LANGUAGE="javascript">
　　<!--
　　window.open ('page.html', 'newwindow', 'height=100, width=400, top=0, left=0, toolbar=no, menubar=no, scrollbars=no, resizable=no,location=n o, status=no') //这句要写成一行
　　-->
　　</SCRIPT>
```
　　参数解释：
　　<SCRIPT LANGUAGE="javascript"> js脚本开始；
　　window.open 弹出新窗口的命令；
　　'page.html' 弹出窗口的文件名；
　　'newwindow' 弹出窗口的名字（不是文件名），非必须，可用空''代替；
　　height=100 窗口高度；
　　width=400 窗口宽度；
　　top=0 窗口距离屏幕上方的象素值；
　　left=0 窗口距离屏幕左侧的象素值；
　　toolbar=no 是否显示工具栏，yes为显示；
　　menubar，scrollbars 表示菜单栏和滚动栏。
　　resizable=no 是否允许改变窗口大小，yes为允许；
　　location=no 是否显示地址栏，yes为允许；
    fullscreen=yes (no) 是否全屏显示zzzz
　　status=no 是否显示状态栏内的信息（通常是文件已经打开），yes为允许；
　　</SCRIPT> js脚本结束
 如果加上fullscreen=yes 的话，弹出的浏览器是全屏状态，相当于按下快捷键F12，不是我想要的情况,下面的代码可以完成弹出窗口的最大化：
```java
<script type="text/javascript">
  var fulls = "left=0,screenX=0,top=0,screenY=0,scrollbars=1";    //定义弹出窗口的参数
  if (window.screen) {
     var ah = screen.availHeight - 30;
     var aw = screen.availWidth - 10;
     fulls += ",height=" + ah;
     fulls += ",innerHeight=" + ah;
     fulls += ",width=" + aw;
     fulls += ",innerWidth=" + aw;
     fulls += ",resizable"
 } else {
     fulls += ",resizable"; // 对于不支持screen属性的浏览器，可以手工进行最大化。 manually
 }
 function openNewWindow(url,name){
  window.open(url,name,fulls);
 }
</script>
```
然后在JSP页面中就可以直接调用openNewWindow()来打开页面了。
<script type="text/javascript">
     var xpwidth=window.screen.width-10;
        var xpheight=window.screen.height-35;
 </script>
window.open('map/mapview.aspx', '_blank', 'resizable=yes,directories=no,top=0,left=0,width='+xpwidth+',height='+xpheight);
