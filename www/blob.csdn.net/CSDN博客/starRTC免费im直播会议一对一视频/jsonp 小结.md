# jsonp 小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月31日 14:50:10[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：218
**JSONP是JSON with Padding的略称。**
**它允许在服务器端集成Script tags返回至客户端，通过javascript callback的形式实现跨域访问（这仅仅是JSONP简单的实现形式）**
<script>标签的src属性并不被同源策略所约束，所以可以获取任何服务器上脚本并执行。
Google的ajax搜索接口：[http://ajax.googleapis.com/ajax/services/search/web?v=1.0&q=?&callback=?](http://ajax.googleapis.com/ajax/services/search/web?v=1.0&q=?&callback=?)
返回一段js代码

var
 flightHandler =function(data){ alert('你查询的航班结果是：票价 '+ data.price +' 元，'+'余票 '+ data.tickets +' 张。'); };
// 提供jsonp服务的url地址（不管是什么类型的地址，最终生成的返回值都是一段javascript代码）var url ="http://flightQuery.com/jsonp/flightResult.aspx?code=CA1998&callback=flightHandler";
url中传递了一个code参数，告诉服务器我要查的是CA1998次航班的信息，而callback参数则告诉服务器，我的本地回调函数叫做flightHandler，所以请把查询结果传入这个函数中进行调用。
服务器flightResult.aspx页面生成了一段这样的代码提供给jsonp.html（服务端的实现说到底就是拼接字符串）：
flightHandler({ "code": "CA1998", "price": 1780, "tickets": 5});
我们看到，传递给flightHandler函数的是一个json，它描述了航班的基本信息。运行一下页面，成功弹出提示窗口，jsonp的执行全过程顺利完成！
利用<script>标签没有跨域限制的“漏洞”来达到与第三方通讯的目的。当需要通讯时，本站脚本创建一个<script>元素，地址指向第三方的API网址，形如：
<script src="http://www.example.net/api?param1=1¶m2=2"></script>
并提供一个回调函数来接收数据（函数名可约定，或通过地址参数传递）。
第三方产生的响应为json数据的包装（故称之为jsonp，即json padding），形如：
callback({"name":"hax","gender":"Male"})
这样浏览器会调用callback函数，并传递解析后json对象作为参数。本站脚本可在callback函数里处理所传入的数据。
允许用户传递一个callback参数给服务端，然后服务端返回数据时会将这个callback参数作为函数名来包裹住JSON数据

[http://www.runoob.com/json/json-jsonp.html](http://www.runoob.com/json/json-jsonp.html)


