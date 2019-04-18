# window.close()和document.close()有什么区别(收藏) - z69183787的专栏 - CSDN博客
2013年10月25日 14:50:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3243
window.close()和document.close()有什么区别
![](http://www2.flash8.net/Uploadteach/2005/06/27/2005627215516810.gif)解决思路：
两者都可以简写为 close() 但什么时候这个 close() 代表的是 document.close()，什么时候代表的是 window.close()？看下面的例子就明白了。
![](http://www2.flash8.net/Uploadteach/2005/06/27/2005627215520924.gif)具体步骤：
<script>
function fnClose(){
  alert("调用的是window.close()，窗口将被关闭")
  close()
}
</script>
<button onclick="fnClose()">单击测试</button>
<button onclick="alert(’调用的是document.close(),文档输入流将被关闭’);close()">单击测试</button>
特别提示
运行代码，当单击第一个测试按钮时会弹出一个警告框，按确定之后调用 window.close() 方法关闭窗口。单击第二个测试按钮则弹出警告框，提示当前调用的方法是 document.close()。效果如图 3.46、图 3.47所示。
![](http://www2.flash8.net/Uploadteach/2005/06/27/2005627215539517.jpg)
图 3.46 触发函数时调用的是window.close()方法
![](http://www2.flash8.net/Uploadteach/2005/06/27/2005627215550254.jpg)
图 3.47 在文档对象上直接调用的是document.close()方法
特别说明
告诉读者通过该问题的解决过程，需要掌握什么内容或技巧，你能够学到什么，以及其他相关的知识点。
window.close() 方法的作用是关闭当前浏览器窗口或HTML应用程式( HTA )。关闭窗口的步骤决定了是否会得到确认提示。假如对一个不是用 window.open 方法打开的窗口调用此方法，将弹出一个确认对话框。当在最后一个IE窗口上调用此方法时同样会弹出一个确认对话框。而用此方法关闭HTML应用程序(HTA)时则不会被提示。
document.close() 方法的作用是关闭输入流，并强制发送数据显示。
当文档内的对象上的事件激发的函数调用 close 方法时， window 对象的 close 方法被使用。而当文档内的对象上的事件直接使用 close 方法时， document 对象的 close 方法被使用。
相关问题
window.open() 和 document.open() 的区别。
window.open() 方法打开新窗口并载入指定的HTML文档。
document.open() 方法打开新文档为 write() 和 writeln() 的输出做准备。
当文档内的对象上的事件激发的函数调用 open 方法时， window 对象的 open 方法被使用。而当文档内的对象上的事件直接使用 open 方法时， document 对象的 open 方法被使用。
