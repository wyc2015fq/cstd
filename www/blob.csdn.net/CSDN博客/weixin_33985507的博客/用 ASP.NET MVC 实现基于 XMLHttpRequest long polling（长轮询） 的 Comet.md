# 用 ASP.NET MVC 实现基于 XMLHttpRequest long polling（长轮询） 的 Comet - weixin_33985507的博客 - CSDN博客
2011年10月17日 16:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
之前在“[反向Ajax，第1部分：Comet介绍](http://kb.cnblogs.com/page/112185/)”（[英文版](http://www.ibm.com/developerworks/web/library/wa-reverseajax1/index.html)）文章中学习了“基于 Multipart XMLHttpRequest 的 Comet”的知识，然后用 ASP.NET MVC 实现了一个，详见[用 ASP.NET MVC 实现基于 Multipart XMLHttpRequest 的 Comet](http://www.cnblogs.com/dudu/archive/2011/10/15/mvc_comet_multipart_xmlhttprequest.html)。
今天继续学习了[基于 XMLHttpRequest long polling 的 Comet](http://kb.cnblogs.com/page/112185/#XMLHttpRequest_long_polling)，又用 ASP.NET MVC 实现了一个，在这篇文章中分享一下。
先了解一下什么是XMLHttpRequest long polling？
> 
这是一种推荐的实现Comet的做法，打开一个到服务器端的Ajax请求然后等待响应。服务器端需要一些特定的功能来允许请求被挂起，只要一有事件发生，服务器端就会在挂起的请求中送回响应并关闭该请求。然后客户端就会使用这一响应并打开一个新的到服务器端的长生存期的Ajax请求。
This is a recommended method to implement Comet is to open an Ajax request to the server and wait for the response. The server requires specific features on the server side to allow the request to be suspended. As soon as an event occurs, the server sends back the response in the suspended request and closes it. The client then consumes the response and opens a new long-lived Ajax request to the server.
我个人的理解是，看起来就像在Web环境中客户端能订阅服务端的事件，服务器端通过事件去通知客户端。如果服务器端用 ASP.NET 来实现，可以利用 .NET 的事件驱动机制，很有意思，下面的示例代码将展示这一点。
先看Web前端js代码：
```
jQuery(function ($) {
function long_polling() {
        $.getJSON('/comet/LongPolling', function (data) {
if (data.d) {
                $('#logs').append(data.d + "<br/>");
            } 
            long_polling();
        });
    }
    long_polling();
});
```
js代码很简单，就是一个递归调用（调用在callback时进行的），通过jQuery的$.getJSON发起Ajax请求，'/comet/LongPolling' 表示请求的服务端 CometController 的 LongPolling Action 的网址。这里我们可以看出实现 Comet 的难点不在 Web 前端，而是在服务器端。
接下来重点看 Web 服务器 ASP.NET MVC Controller 的代码。
首先要注意的是为了响应 XMLHttpRequest long polling 请求，我们需要实现一个异步控制器（AsyncController），如果您对 AsyncController 不熟悉，建议阅读MSDN上的文章 [Using an Asynchronous Controller in ASP.NET MVC](http://msdn.microsoft.com/en-us/library/ee728598.aspx) 。
先上 Controller 的实现代码：
（注：该控制器实现的功能是每隔5秒钟向客户端发送服务器当时间）
```
public class CometController : AsyncController
{
//LongPolling Action 1 - 处理客户端发起的请求
    public void LongPollingAsync()
    {
//计时器，5秒种触发一次Elapsed事件
        System.Timers.Timer timer = new System.Timers.Timer(5000);
//告诉ASP.NET接下来将进行一个异步操作
        AsyncManager.OutstandingOperations.Increment();
//订阅计时器的Elapsed事件
        timer.Elapsed += (sender, e) =>
            {
//保存将要传递给LongPollingCompleted的参数
                AsyncManager.Parameters["now"] = e.SignalTime;
//告诉ASP.NET异步操作已完成，进行LongPollingCompleted方法的调用
                AsyncManager.OutstandingOperations.Decrement();
            };
//启动计时器
        timer.Start();
    }
//LongPolling Action 2 - 异步处理完成，向客户端发送响应
    public ActionResult LongPollingCompleted(DateTime now)
    {
return Json(new { d = now.ToString("MM-dd HH:mm:ss ") + 
"-- Welcome to cnblogs.com!" }, 
            JsonRequestBehavior.AllowGet);
    }
}
```
实现异步控制器需要继承 System.Web.Mvc.AsyncController，并将 Action 分解为两个，比如 Action 叫 LongPolling，则分解为 LongPollingAsync 与 LongPollingCompleted 。LongPollingAsync 接受客户端请求，并发起异步操作；异步操作完成，调用LongPollingCompleted。
AsyncManager.OutstandingOperations.Increment(); 告诉ASP.NET接下来将进行一个异步操作。
AsyncManager.OutstandingOperations.Decrement(); 告诉ASP.NET异步操作完成，请调用LongPollingCompleted()方法。
示例代码中的异步操作就是将服务器当前时间作为参数传递给 LongPollingCompleted() 方法，LongPollingCompleted() 获取服务器当前时间并传递给客户端，客户端收到后将之显示出来，将继续发起 Ajax 请求 ... 这样周而复始，实现了基于 XMLHttpRequest long polling 的 Comet。
示例代码运行结果如下：
![](https://pic002.cnblogs.com/images/2011/1/2011101715484542.png)
**小结**
以前觉得 Comet 是很高深的东西，自己动手做了之后，发觉原来没那么难。所以，重要的是[动手去做](http://news.cnblogs.com/n/118379/)！
如果不能在实际项目中去做，那就写一篇博客吧！
**代码下载**
[CometMvcDemo_LongPolling.rar](http://files.cnblogs.com/dudu/CometMvcDemo_LongPolling.rar)
