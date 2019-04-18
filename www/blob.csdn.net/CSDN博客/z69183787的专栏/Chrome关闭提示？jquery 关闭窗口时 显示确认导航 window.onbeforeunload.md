# Chrome关闭提示？jquery 关闭窗口时 显示确认导航 window.onbeforeunload - z69183787的专栏 - CSDN博客
2013年11月13日 21:43:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6883
unload事件在火狐和Chrome中对alert是阻止弹出的，所以你的代码看不到效果。
详见：[http://stackoverflow.com/questions/6063522/jquery-beforeunload](http://stackoverflow.com/questions/6063522/jquery-beforeunload)
但unload事件可以进行一些对象销毁，事件解除绑定等清理工作，这通常是难以看到的。
在IE10中，刷新可以看到弹出alert。
如果你想在用户离开页面之前确认是否离开，最好使用beforeunload事件在beforeunload事件里，不需要alert，该事件会自动产生一个弹出框，
就算你加了alert也是没效果的。
另外，要使beforeunload事件能自动弹出框，必须得有返回值，如：
JavaScript code
[?](http://bbs.csdn.net/topics/390390869?page=1#clipboardWindow)
```
```java
```
 $(window).on('beforeunload', function(event) {    
var message = 'I’m really going to miss you if you go.';
event.returnValue = message;
return message;
});
```java
```
```
上面的代码可以达到关闭、点击链接、输入地址都窗口弹出框的效果。
把返回值message设为你想显示的信息
