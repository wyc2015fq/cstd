# jquery load 事件用法 - z69183787的专栏 - CSDN博客
2013年11月11日 16:32:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1627
如果绑定给window对象，则会在所有内容加载后触发，包括窗口，框架，对象和图像。如果绑定在元素上，则当元素的内容加载完毕后触发。
注意:只有当在这个元素完全加载完之前绑定load的处理函数,才会在他加载完后触发。如果之后再绑定就永远不会触发了。所以不要在$(document).ready()里绑定load事件，因为jQuery会在所有DOM加载完成后再绑定load事件。
调用load方法的完整格式是：`load( url, [data], [callback] ),`
`其中:`
` `
``
`url`：是指要导入文件的地址。
`data`：可选参数；因为Load不仅仅可以导入静态的html文件，还可以导入动态脚本，例如PHP文件，所以要导入的是动态文件时，我们可以把要传递的参数放在这里。
``callback`：可选参数；是指调用load方法并得到服务器响应后，再执行的另外一个函数。`
``
一：如何使用data
1.加载一个php文件，该php文件不含传递参数$("#myID").load("test.php");
//在id为#myID的元素里导入test.php运行后的结果
2. 加载一个php文件，该php文件含有一个传递参数
$("#myID").load("test.php",{"name" : "Adam"});
//导入的php文件含有一个传递参数，类似于：test.php?name=Adam
3. 加载一个php文件，该php文件含有多个传递参数。注：参数间用逗号分隔
$("#myID").load("test.php",{"name" : "Adam" ，"site":"61dh.com"});
//导入的php文件含有一个传递参数，类似于：test.php?name=Adam&site=61dh.com
4. 加载一个php文件，该php文件以数组作为传递参数
$("#myID").load("test.php",{'myinfo[]', ["Adam", "61dh.com"]});
//导入的php文件含有一个数组传递参数。
注意：使用`load`，这些参数是以POST的方式传递的，因此在test.php里，不能用GET来获取参数。
二：如何使用callback
比如我们要在`load方法`得到服务器响应后，慢慢地显示加载的内容，就可以使用`callback`函数。代码如下：
$("#go").click(function(){
  $("#myID").load("welcome.php", {"lname" : "Cai", "fname" : "Adam", function(){
    $("#myID").fadeIn('slow');}
  );
});
备注：
在load的url里加上空格后面就可以跟选择器了。
例如：
    $("body").load("test.html #a");
