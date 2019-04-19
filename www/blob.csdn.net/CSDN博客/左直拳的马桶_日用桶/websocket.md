# websocket - 左直拳的马桶_日用桶 - CSDN博客
2019年03月04日 18:30:23[左直拳](https://me.csdn.net/leftfist)阅读数：375
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
**一、基本概念**
websocket，最大的作用就是可以让服务器主动推送消息到页面上。
与此相对的，是http协议。http是一种单向协议，什么都是让客户端向服务器请求资源。以前搞过WEB聊天室的人就知道多恶心，浏览器要定期向服务器轮询，才能得到聊天的内容。
现在好了，有了websocket，服务器就能主动推消息到页面了。这是一种双工协议。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190304182040924.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWZ0ZmlzdC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
其他特点包括：
（1）建立在 TCP 协议之上，服务器端的实现比较容易。
（2）与 HTTP 协议有着良好的兼容性。默认端口也是80和443，并且握手阶段采用 HTTP 协议，因此握手时不容易屏蔽，能通过各种 HTTP 代理服务器。
（3）数据格式比较轻量，性能开销小，通信高效。
（4）可以发送文本，也可以发送二进制数据。
（5）没有同源限制，客户端可以与任意服务器通信。
（6）协议标识符是ws（如果加密，则为wss），服务器网址就是 URL。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190304182246969.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sZWZ0ZmlzdC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
其服务地址，以ws或wss开头，类似http或https：
```
ws://192.168.0.13:9981/websocket
```
**二、应用**
```
function wsconnect() {
	var ws = new WebSocket("ws://192.168.0.13:9981/websocket");
	
	ws.onopen = function(evt) { 
		console.log("Connection open ..."); 
		ws.send("Hello WebSockets!");
	};
	ws.onmessage = function(evt) {
		console.log("receive message: " + (new Date())); 
		var json = eval("(" + evt.data + ")");
		//高高兴兴地处理这个json
	};
	ws.onclose = function(evt) {
		console.log("Connection closed.Reconnect will be attempted in 1 second.", evt.reason);
		setTimeout(function() {
		  wsconnect();
		}, 1000);
	};
	
	ws.onerror = function(err) {
		console.error('Socket encountered error: ', err.message, 'Closing socket');
		ws.close();
	};
}
$(function(){
	wsconnect();
});
```
参考资料：
[阮一峰：WebSocket 教程](http://www.ruanyifeng.com/blog/2017/05/websocket.html)
