# SOCKET.IO，理解SOCKET.IO - z69183787的专栏 - CSDN博客
2017年06月20日 14:25:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1606
原文：http://www.cnblogs.com/xiezhengcai/p/3957314.html
要理解socket.io ，不得不谈谈websocket
在html5之前，因为http协议是无状态的，要实现浏览器与服务器的实时通讯，如果不使用 flash、applet 等浏览器插件的话，就需要定期轮询服务器来获取信息。这造成了一定的延迟和大量的网络通讯。随着HTML5 的出现，这一情况有望彻底改观，它就是 WebSocket 。理论上，Socket能干的事Websocket都能完成，这与需要实现与服务器实时通信的应用来说，如从羊肠小道进入了高速公路。那么，使用Websocket我们要做些什么呢？
**首先，我们得先看看websocket的工作机制**
**工作流程：**
浏览器通过 JavaScript 向服务器发出建立 WebSocket 连接的请求，连接建立以后，客户端和服务器端就可以通过 TCP 连接直接交换数据。因为 WebSocket 连接本质上就是一个 TCP 连接，所以在数据传输的稳定性和数据传输量的大小方面，和传统轮询以技术比较，具有很大的性能优势。
为了建立一个 WebSocket 连接，客户端浏览器首先要向服务器发起一个 HTTP 请求，这个请求和通常的 HTTP 请求不同，包含了一些附加头信息，其中附加头信息”Upgrade: WebSocket”表明这是一个申请协议升级的 HTTP 请求，服务器端解析这些附加的头信息然后产生应答信息返回给客户端，客户端和服务器端的 WebSocket 连接就建立起来了，双方就可以通过这个连接通道自由的传递信息，并且这个连接会持续存在直到客户端或者服务器端的某一方主动的关闭连接。
**协议规范**：
一个典型的websocket发起请求到响应请求的例子如下
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
客户端到服务端：
GET / HTTP/1.1
Connection:Upgrade
Host:127.0.0.1:8088
Origin:null
Sec-WebSocket-Extensions:x-webkit-deflate-frame
Sec-WebSocket-Key:puVOuWb7rel6z2AVZBKnfw==
Sec-WebSocket-Version:13
Upgrade:websocket
服务端到客户端：
HTTP/1.1 101 Switching Protocols
Connection:Upgrade
Server:beetle websocket server
Upgrade:WebSocket
Date:Mon, 26 Nov 2013 23:42:44 GMT
Access-Control-Allow-Credentials:true
Access-Control-Allow-Headers:content-type
Sec-WebSocket-Accept:FCKgUr8c7OsDsLFeJTWrJw6WO8Q=
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
这是一个握手的http请求，它与普通的http请求有一些区别，首先请求和响应的，”Upgrade:WebSocket”表示请求的目的就是要将客户端和服务器端的通讯协议从 HTTP 协议升级到 WebSocket 协议。从客户端到服务器端请求的信息里包含有”Sec-WebSocket-Extensions”、“Sec-WebSocket-Key”这样的头信息。这是客户端浏览器需要向服务器端提供的握手信息，服务器端解析这些头信息，并在握手的过程中依据这些信息生成一个 28 位的安全密钥并返回给客户端，以表明服务器端获取了客户端的请求，同意创建 WebSocket 连接。
当握手成功后，这个时候tcp连接已在经建立了，客户发送上来的时候就是纯纯的数据了。不过服务端要判断什么时候是一次数据请求的开始，什么时候是请求的结束。在websocket中，由于浏览端和服务端已经打好招呼，如我发送的内容为utf-8 编码，如果我发送0x00,表示包的开始，如果发送了0xFF，就表示包的结束了。这就解决了黏包的问题。
从握手的协议可以看出，如果我们要使用Websocket，我们需要一个实现Websocket协议规范的服务器，这不在我们讨论的范围。
值得一提的是：websocket是可以和http共用监听端口的，也就是它可以公用端口完成socket任务。
**Socket.io**
主角终于上场了，听了上面对Websocket的介绍之后，你是不是想，socket.io就是对Websocket的封装呢，并且实现了Websocket的服务端代码。不错，但是不完全正确。刚才我们说到，在WebSocket没有出现之前，实现与服务端的实时通讯可以通过轮询来完成任务.。Socket.io将Websocket和轮询（Polling）机制以及其它的实时通信方式封装成了通用的接口，并且在服务端实现了这些实时机制的相应代码。也就是说，Websocket仅仅是Socket.io实现实时通信的一个子集。
那么，Socket.io都实现了Polling中的那些通信机制呢？
- Adobe® Flash® Socket
- AJAX long polling
- AJAX multipart streaming
- Forever Iframe
- JSONP Polling
Adobe® Flash® Socket 大部分PC浏览器都支持的socket模式，不过是通过第三方嵌入到浏览器，不在W3C规范内，所以可能将逐步被淘汰，况且，大部分的手机浏览器都不支持这种模式。
AJAX long polling 这个很好理解，所有浏览器都支持这种方式，就是定时的向服务器发送请求，缺点是会给服务器带来压力并且出现信息更新不及时的现象。
AJAX multipart streaming  这是在XMLHttpRequest对象上使用某些浏览器（比如说Firefox）支持的multi-part标志。Ajax请求被发送给服务器端并保持打开状态（挂起状态），每次需要向客户端发送信息，就寻找一个挂起的的http请求响应给客户端，并且所有的响应都会通过统一连接来写入
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
var xhr = $.ajaxSettings.xhr();
xhr.multipart =true;
xhr.open('GET', 'ajax', true);
xhr.onreadystatechange = function() {
　　if (xhr.readyState == 4) {
　　　　processEvents($.parseJSON(xhr.responseText));
　　}
};
xhr.send(null);
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
Forever Iframe （永存的Iframe）技术涉及了一个置于页面中的隐藏Iframe标签，该标签的src属性指向返回服务器端事件的servlet路径。每次在事件到达时，servlet写入并刷新一个新的script标签，该标签内部带有JavaScript代码，iframe的内容被附加上这一script标签，标签中的内容就会得到执行。这种方式的缺点是接和数据都是由浏览器通过HTML标签来处理的，因此你没有办法知道连接何时在哪一端已被断开了，并且Iframe标签在浏览器中将被逐步取消使用。
JSONP Polling  JSONP轮询基本上与HTTP轮询一样，不同之处则是JSONP可以发出跨域请求，详细请搜索查询jsonp的内容。
