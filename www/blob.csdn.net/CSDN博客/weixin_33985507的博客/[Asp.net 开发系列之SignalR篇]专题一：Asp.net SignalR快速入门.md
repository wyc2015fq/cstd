# [Asp.net 开发系列之SignalR篇]专题一：Asp.net SignalR快速入门 - weixin_33985507的博客 - CSDN博客
2019年01月11日 00:57:17[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
# 一、前言
之前半年时间感觉自己有点浮躁，导致停顿了半年多的时间没有更新博客，今天重新开始记录博文，希望自己可以找回初心，继续沉淀。由于最近做的项目中用到SignalR技术，所以打算总结下Asp.net SignalR的相关内容，希望对刚接触或者接触不多的朋友有所帮助。今天的专题就是让大家可以快速的上手Asp.net SignalR。废话不多说了，下面正式进入今天专题的内容。
# 二、Asp.net SignalR 是个什么东东
Asp.net SignalR是微软为实现实时通信的一个类库。一般情况下，SignalR会使用JavaScript的长轮询(long polling)的方式来实现客户端和服务器通信，随着Html5中WebSockets出现，SignalR也支持WebSockets通信。另外SignalR开发的程序不仅仅限制于宿主在IIS中，也可以宿主在任何应用程序，包括控制台，客户端程序和Windows服务等，另外还支持Mono，这意味着它可以实现跨平台部署在Linux环境下。
　　SignalR内部有两类对象：
- Http持久连接(Persisten Connection)对象：用来解决长时间连接的功能。还可以由客户端主动向服务器要求数据，而服务器端不需要实现太多细节，只需要处理PersistentConnection 内所提供的五个事件：OnConnected, OnReconnected, OnReceived, OnError 和 OnDisconnect 即可。
- Hub（集线器）对象：用来解决实时(realtime)信息交换的功能，服务端可以利用URL来注册一个或多个Hub，只要连接到这个Hub，就能与所有的客户端共享发送到服务器上的信息，同时服务端可以调用客户端的脚本。
　　SignalR将整个信息的交换封装起来，客户端和服务器都是使用JSON来沟通的，在服务端声明的所有Hub信息，都会生成JavaScript输出到客户端，.NET则依赖Proxy来生成代理对象，而Proxy的内部则是将JSON转换成对象。
　　客户端和服务端的具体交互情况如下图所示：
![Invoking methods with SignalR](http://media-www-asp.azureedge.net/media/4196527/what_is_signalr_invocation.png)
从上面的介绍可以看出，SignalR既然是为实时而生的，这样就决定了其使用场所。具体适用情景有如下几点：
- 聊天室，如在线客服系统，IM系统等
- 股票价格实时更新
- 消息的推送服务
- 游戏中人物位置的实时推送
目前，我所在公司在开发的就是在线客服系统。
# 三、使用Asp.net SignalR在Web端实现广播消息
通过第二部分的介绍，相信大家对Asp.net SignalR有了一个初步的了解，接下来通过两个例子来让大家加深对SignalR运行机制的理解。第一个例子就是在Web端如何使用SignalR来实现广播消息。
- 使用Visual Studio 2013，创建一个MVC工程
- 通过Nuget安装SignalR包。右键引用-》选择管理Nuget程序包-》在出现的窗口中输入SignalR来找到SignalR包进行安装。
- 安装SignalR成功后，SignalR库的脚本将被添加进Scripts文件夹下。具体如下图所示：
![](https://images2015.cnblogs.com/blog/383187/201603/383187-20160330231731988-1853447818.png)
4. 向项目中添加一个SignalR集线器(v2)并命名为ServerHub。
![](https://images2015.cnblogs.com/blog/383187/201603/383187-20160330232023582-590617526.png)
5. 将下面代码填充到刚刚创建的ServerHub类中。
```
using Microsoft.AspNet.SignalR;
using Microsoft.AspNet.SignalR.Hubs;
using System;
namespace SignalRQuickStart
{public class ServerHub : Hub
    {
        private static readonly char[] Constant =
        {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
            'W', 'X', 'Y', 'Z'
        };
        /// <summary>
        /// 供客户端调用的服务器端代码
        /// </summary>
        /// <param name="message"></param>
        public void Send(string message)
        {
            var name = GenerateRandomName(4);
            // 调用所有客户端的sendMessage方法
            Clients.All.sendMessage(name, message);
        }
        /// <summary>
        /// 产生随机用户名函数
        /// </summary>
        /// <param name="length">用户名长度</param>
        /// <returns></returns>
        public static string GenerateRandomName(int length)
        {
            var newRandom = new System.Text.StringBuilder(62);
            var rd = new Random();
            for (var i = 0; i < length; i++)
            {
                newRandom.Append(Constant[rd.Next(62)]);
            }
            return newRandom.ToString();
        }
    }
}
```
　6. 创建一个**Startup**类，如果开始创建MVC项目的时候没有更改身份验证的话，这个类会默认添加的，如果已有就不需要重复添加了。按照如下代码更新Startup类。
　　7. 在Home控制器中创建一个Home Action方法
```
public class HomeController : Controller
    {
        public ActionResult Index()
        {
            return View();
        }
        public ActionResult About()
        {
            ViewBag.Message = "Your application description page.";
            return View();
        }
        public ActionResult Contact()
        {
            ViewBag.Message = "Your contact page.";
            return View();
        }
        **public ActionResult Chat()        {            return**** View();        }**
    }
```
8. 在Views文件中Home文件夹中创建一个Chat视图，视图代码如下所示：
```
@{
    ViewBag.Title = "聊天窗口";
}
<h2>Chat</h2>
<div class="container">
    <input type="text" id="message" />
    <input type="button" id="sendmessage" value="Send" />
    <input type="hidden" id="displayname" />
    <ul id="discussion"></ul>
</div>
@section scripts
{
    <!--引用SignalR库. -->
    <script src="~/Scripts/jquery.signalR-2.2.0.min.js"></script>
     <!--引用自动生成的SignalR 集线器(Hub)脚本.在运行的时候在浏览器的Source下可看到 -->
    <script src="~/signalr/hubs"></script>
    
    <script>
        $(function () {
            // 引用自动生成的集线器代理
            var chat = $.connection.serverHub;
            // 定义服务器端调用的客户端sendMessage来显示新消息
           
            chat.client.sendMessage = function (name, message) {
                // 向页面添加消息
                $('#discussion').append('<li><strong>' + htmlEncode(name)
                    + '</strong>: ' + htmlEncode(message) + '</li>');
            };
           
            // 设置焦点到输入框
            $('#message').focus();
            // 开始连接服务器
            $.connection.hub.start().done(function () {
                $('#sendmessage').click(function () {
                    // 调用服务器端集线器的Send方法
                    chat.server.send($('#message').val());
                    // 清空输入框信息并获取焦点
                    $('#message').val('').focus();
                });
            });
        });
        
        // 为显示的消息进行Html编码
        function htmlEncode(value) {
            var encodedValue = $('<div />').text(value).html();
            return encodedValue;
        }
    </script>
    }
```
　9. 修改App_Start文件夹内的RoutConfig类，将Action方法默认设置为Chat.
```
public class RouteConfig
    {
        public static void RegisterRoutes(RouteCollection routes)
        {
            routes.IgnoreRoute("{resource}.axd/{*pathInfo}");
            routes.MapRoute(
                name: "Default",
                url: "{controller}/{action}/{id}",
                defaults: new { controller = "Home",** action = "Chat"**, id = UrlParameter.Optional }
            );
        }
    }
```
到此，我们的例子就实现完成了，接下来我们先来看看运行效果，之后再来解释到底SignalR是如何来完成广播消息的。运行的运行结果如下。
![](https://images2015.cnblogs.com/blog/383187/201603/383187-20160330234643473-921394612.png)
从运行结果，你可以发现，在任何一个窗口输入信息并发送，所有客户端将收到该消息。这样的效果在实际应用中很多，如QQ，一登录QQ的时候都会推送腾讯广告消息。
　　看完了运行结果，接下来我们来分析下代码，进而来剖析下SignalR到底是如何工作的。
　　按照B/S模式来看，运行程序的时候，Web页面就与SignalR的服务建立了连接，具体的建立连接的代码就是：**$.connection.hub.start()。这句代码的作用就是与SignalR服务建立连接，后面的done函数表明建立连接成功后为发送按钮注册了一个click事件，当客户端输入内容点击发送按钮后，该Click事件将会触发，触发执行的操作为：**** chat.server.send($('#message').val())。这句代码表示调用服务端的send函数，而服务端的Send韩式又是调用所有客户端的sendMessage函数，而客户端中sendMessage函数就是将信息添加到对应的消息列表中。这样就实现了广播消息的功能了。 **
**　　看到这里，有人是否会有疑问，前面的实现都只用到了集线器对象，而没有用到持久连接对象。其实并不是如此，**$.connection这句代码就是使用持久连接对象，当然你也可以在重新OnConnected方法来查看监控客户端的连接情况，更新的代码如下所示：****
```
public class ServerHub : Hub
    {
        private static readonly char[] Constant =
        {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
            'W', 'X', 'Y', 'Z'
        };
        /// <summary>
        /// 供客户端调用的服务器端代码
        /// </summary>
        /// <param name="message"></param>
        public void Send(string message)
        {
            var name = GenerateRandomName(4);
            // 调用所有客户端的sendMessage方法
            Clients.All.sendMessage(name, message);
        }
        /// <summary>
        /// 客户端连接的时候调用
        /// </summary>
        /// <returns></returns>
        **public override Task OnConnected()        {            Trace.WriteLine("客户端连接成功");            return base.OnConnected();        }        **/// <summary>
        /// 产生随机用户名函数
        /// </summary>
        /// <param name="length">用户名长度</param>
        /// <returns></returns>
        public static string GenerateRandomName(int length)
        {
            var newRandom = new System.Text.StringBuilder(62);
            var rd = new Random();
            for (var i = 0; i < length; i++)
            {
                newRandom.Append(Constant[rd.Next(62)]);
            }
            return newRandom.ToString();
        }
    }
```
这样在运行页面的时候，将在输出窗口看到“客户端连接成功”字样。运行效果如下图所示：
![](https://images2015.cnblogs.com/blog/383187/201603/383187-20160331000409566-933183657.png)
在第二部分介绍的时候说道，在服务端声明的所有Hub信息，都会生成JavaScript输出到客户端，为了验证这一点，可以在Chrome中F12来查看源码就明白了，具体如下图所示：
![](https://images2015.cnblogs.com/blog/383187/201603/383187-20160331000614644-189374985.png)
看到上图，你也就明白了为什么Chat.cshtml页面需要引入"signalr/hubs"脚本库了吧。
```
<!--引用SignalR库. -->
    <script src="~/Scripts/jquery.signalR-2.2.0.min.js"></script>
     <!--引用自动生成的SignalR 集线器(Hub)脚本.在运行的时候在浏览器的Source下可看到 -->
    **<script src="~/signalr/hubs"></script>    **
```
# 四、在桌面程序中如何使用Asp.net SignalR
上面部分介绍了SignalR在Asp.net MVC 中的实现，这部分将通过一个例子来看看SignalR在WPF或WinForm是如何使用的。其实这部分实现和Asp.net MVC中非常相似，主要不同在于，Asp.net MVC中的SignalR服务器寄宿在IIS中，而在WPF中应用，我们把SignalR寄宿在WPF客户端中。
下面让我们看看SignalR服务端的实现。
```
/// <summary>
        /// 启动SignalR服务，将SignalR服务寄宿在WPF程序中
        /// </summary>
        private void StartServer()
        {
            try
            {
                SignalR = WebApp.Start(ServerUri);  // 启动SignalR服务
            }
            catch (TargetInvocationException)
            {
                WriteToConsole("一个服务已经运行在：" + ServerUri);
                // Dispatcher回调来设置UI控件状态
                this.Dispatcher.Invoke(() => ButtonStart.IsEnabled = true);
                return;
            }
            this.Dispatcher.Invoke(() => ButtonStop.IsEnabled = true);
            WriteToConsole("服务已经成功启动，地址为：" + ServerUri);
        }
public class ChatHub : Hub
    {
        public void Send(string name, string message)
        {
            Clients.All.addMessage(name, message);
        }
        public override Task OnConnected()
        {
            //
            Application.Current.Dispatcher.Invoke(() =>
                ((MainWindow)Application.Current.MainWindow).WriteToConsole("客户端连接，连接ID是: " + Context.ConnectionId));
            return base.OnConnected();
        }
        public override Task OnDisconnected(bool stopCalled)
        {
             Application.Current.Dispatcher.Invoke(() =>
                ((MainWindow)Application.Current.MainWindow).WriteToConsole("客户端断开连接，连接ID是: " + Context.ConnectionId));
            return base.OnDisconnected(true);
        }
    }
 public class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            // 有关如何配置应用程序的详细信息，请访问 http://go.microsoft.com/fwlink/?LinkID=316888
            // 允许CORS跨域
            //app.UseCors(CorsOptions.AllowAll);
            app.MapSignalR();
        }
    }
```
通过上面的代码，我们SignalR服务端的实现就完成了，其实现逻辑与Asp.net MVC的代码类似。
　　接下来，让我们看看，WPF客户端是如何连接和与服务器进行通信的。具体客户端的实现如下：
```
public IHubProxy HubProxy { get; set; }
        const string ServerUri = "http://localhost:8888/signalr";
        public HubConnection Connection { get; set; }
        public MainWindow()
        {
            InitializeComponent();
            // 窗口启动时开始连接服务
            ConnectAsync();
        }
        /// <summary>
        /// 发送消息
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ButtonSend_Click(object sender, RoutedEventArgs e)
        {
            // 通过代理来调用服务端的Send方法
            // 服务端Send方法再调用客户端的AddMessage方法将消息输出到消息框中
            HubProxy.Invoke("Send",  GenerateRandomName(4), TextBoxMessage.Text.Trim());
            TextBoxMessage.Text = String.Empty;
            TextBoxMessage.Focus();
        }
        private async void ConnectAsync()
        {
            Connection = new HubConnection(ServerUri);
            Connection.Closed += Connection_Closed;
            // 创建一个集线器代理对象
            HubProxy = Connection.CreateHubProxy("ChatHub");
            // 供服务端调用，将消息输出到消息列表框中
            HubProxy.On<string, string>("AddMessage", (name, message) =>
                 this.Dispatcher.Invoke(() =>
                    RichTextBoxConsole.AppendText(String.Format("{0}: {1}\r", name, message))
                ));
            try
            {
                await Connection.Start();
            }
            catch (HttpRequestException)
            {
                // 连接失败
                return;
            }
            // 显示聊天控件
            ChatPanel.Visibility = Visibility.Visible;
            ButtonSend.IsEnabled = true;
            TextBoxMessage.Focus();
            RichTextBoxConsole.AppendText("连上服务：" + ServerUri + "\r");
        }
```
上面的代码也就是WPF客户端实现的核心代码，主要逻辑为，客户端启动的时候就调用Connection.Start方法与服务器进行连接。然后通过HubProxy代理类来调用集线器中Send方法，而集线器中的Send方法又通过调用客户端的addMessage方法将消息输出到客户端的消息框中进行显示，从而完成消息的推送过程。接下来，让我们看看其运行效果：
![](https://images2015.cnblogs.com/blog/383187/201604/383187-20160402222601816-829555628.gif)
从上面的运行效果看出，其效果和Asp.net MVC上的效果是一样的。
# 五、总结
到这里，本专题的所有内容就结束了，这篇SignalR快速入门也是本人在学习SignalR过程中的一些心得体会，希望可以帮助一些刚接触SignalR的朋友快速入门。本篇主要实现了SignalR的广播消息的功能，可以实现手机端消息推送的功能，接下来一篇将介绍如何使用SignalR实现一对一的聊天。
　　本文所有源码：[SignalR快速入门](https://code.msdn.microsoft.com/SignalR-3ec71545)
