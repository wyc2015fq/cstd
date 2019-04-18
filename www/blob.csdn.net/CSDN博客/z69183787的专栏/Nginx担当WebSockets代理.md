# Nginx担当WebSockets代理 - z69183787的专栏 - CSDN博客
2018年10月12日 14:54:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：45
个人分类：[Nginx																[Websocket](https://blog.csdn.net/z69183787/article/category/2837859)](https://blog.csdn.net/z69183787/article/category/5774417)
[https://blog.csdn.net/chszs/article/details/26369257](https://blog.csdn.net/chszs/article/details/26369257)
**Nginx担当WebSockets代理**
英文原文：[http://nginx.com/blog/websocket-nginx/](http://nginx.com/blog/websocket-nginx/)
# 作者：chszs，转载需注明。博客主页：[http://blog.csdn.net/chszs](http://blog.csdn.net/chszs)
WebSocket 协议提供了一种创建支持客户端和服务端实时双向通信Web应用程序的方法。作为HTML5规范的一部分，WebSockets简化了开发Web实时通信程序的难度。目前主流的浏览器都支持WebSockets，包括火狐、IE、Chrome、Safari以及Opera等，而且，越来越多的服务器应用框架也开始支持WebSockets。
要在企业产品中使用WebSockets，为满足高性能和高可用性，需要多个WebSocket服务器。负载均衡层需要支持WebSocket协议。Nginx从1.3版起就开始支持WebSocket协议，而且可以担当WebSocket应用程序的反向代理以及实现负载均衡。
WebSocket协议和HTTP协议不同，但是WebSocket协议的握手和HTTP是兼容的，它使用HTTP的Upgrade协议头将连接从HTTP连接升级到WebSocket连接。这个特性使得WebSocket应用程序可以很容易地应用到现有的基础设施。例如，WebSocket应用可以使用标准的80和443 HTTP端口，因此可以通过现有的防火墙设施。
WebSockets应用程序会在客户端和服务器之间建立一个长连接，使得开发实时应用很容易。HTTP的Upgrade协议头机制用于将连接从HTTP连接升级到WebSocket连接，Upgrade机制使用了Upgrade协议头和Connection协议头。反向代理服务器在支持WebSocket协议方面面临着一些挑战。挑战之一是WebSocket是一个逐段转发（hop-by-hop）协议，因此当代理服务器拦截到来自客户端的Upgrade请求时，代理服务器需要将自己的Upgrade请求发送给后端服务器，包括适合的请求头。而且，由于WebSocket连接是长连接，与传统的HTTP端连接截然不同，故反向代理服务器还需要允许这些连接处于打开（Open）状态，而不能因为其空闲就关闭了连接。
Nginx通过在客户端和后端服务器之间建立隧道来支持WebSockets通信。为了让Nginx可以将来自客户端的Upgrade请求发送到后端服务器，Upgrade和Connection的头信息必须被显式的设置。如下所示：
- 
`location /wsapp/ {`
- 
`proxy_pass http://wsbackend;`
- 
`proxy_http_version 1.1;`
- 
`proxy_set_header Upgrade $http_upgrade;`
- 
`proxy_set_header Connection "upgrade";`
- 
`}`
一旦我们完成以上设置，Nginx就可以处理WebSocket连接了。
**Nginx WebSockets 实例**
下面的例子讲述了Nginx是如何为WebSocket做代理的。此例将使用ws模块，它是基于node.js构建的WebSocket实现。Nginx将担当反向代理服务器，后端服务器是一个使用了ws和Node.js的简单WebSockets应用。例子使用的命令在Ubuntu 13.10和CentOS 6.5上测试通过，但对于其他操作系统或许需要稍作修改。就这个例子来说，WebSocket服务器的IP地址是192.168.100.10，Nginx服务器的IP地址是192.168.100.20。如果你还没有安装node.js和npm，你可以通过以下命令安装：
对 Debian/Ubuntu 来说：
`sudo apt-get install nodejs npm`
对 RHEL/CentOS 来说：
`sudo yum install nodejs npm`
在Ubuntu上，node.js会被安装为"nodejs"，但在CentOS中被会安装为"node"。我们在例子中统一使用"node"，所以，我们会在Ubuntu上创建一个符号连接来允许我们使用“node”：
`ln -s /usr/bin/nodejs /usr/local/bin/node`
然后安装 ws:
`sudo npm install ws`
注意：如果你得到了一个错误：“Error: failed to fetch from registry: ws” ，那么运行下面的命令应该能解决这个问题：
`sudo npm config set registry http://registry.npmjs.org/`
接下来，你可以再次运行 sudo npm install ws
ws模块来自/root/node_modules/ws/bin/wscat，我们会为客户端使用它，但是我们需要创建一个程序来作为我们的服务器。将下面的代码保存到一个server.js文件中：
- 
`console.log("Server started");`
- 
`var Msg = '';`
- 
`var WebSocketServer = require('ws').Server`
- 
`, wss = new WebSocketServer({port: 8010});`
- 
`wss.on('connection', function(ws) {`
- 
`ws.on('message', function(message) {`
- 
`console.log('Received from client: %s', message);`
- 
`ws.send('Server received from client: ' + message);`
- 
`});`
- 
`});`
这个程序可以通过下面的命令执行：
`node server.js`
该程序会输出一条初始化消息“Server started”，之后监听8010端口，等待客户端的连接。它会处理收到的所有请求，并且将接收到的消息输出在控制台，之后向客户端返回一条包含该消息的消息。我们希望Nginx去代理客户端的请求，可以通过下面的配置实现：
- 
`map $http_upgrade $connection_upgrade {`
- 
`default upgrade;`
- 
`'' close;`
- 
`}`
- 
`upstream websocket {`
- 
`server 192.168.100.10:8010;`
- 
`}`
- 
`server {`
- 
`listen 8020;`
- 
`location / {`
- 
`proxy_pass http://websocket;`
- 
`proxy_http_version 1.1;`
- 
`proxy_set_header Upgrade $http_upgrade;`
- 
`proxy_set_header Connection "Upgrade";`
- 
`}`
- 
`}`
上面的配置会使Nginx监听8020端口，并把任何接收到的请求转发给后端的WebSocket服务器，让后端服务器更好地处理WebSocket协议。我们可以使用wscat作为客户端来测试一下：
`/root/node_modules/ws/bin/wscat –connect ws://192.168.100.20:8020`
上面的命令会通过Nginx反向代理服务器和后端WebSocket服务器建立连接，你可以向服务器发送任意消息，然后服务器会返回一条消息。每当你在客户端发送一条消息，在后端服务器上能看到该消息的输出，之后在客户端会显示一条来自服务端的消息。
这是一个交互示例：
|**Server:**|**Client:**|
|----|----|
|$ node server.js||
|Server started||
||wscat –connect ws://192.168.100.20:8020|
||Connected (press CTRL+C to quit)|
||> Hello|
|Received from client: Hello||
||< Server received from client: Hello|
由此我们可以看到客户端与服务器能通过Nginx反向代理建立WebSockets通信，而且消息可以持续地进行双向传输，直至客户端或服务器断开连接。为了让Nginx能正确处理WebSocket连接，只需正确地设置消息头来处理从HTTP连接升级到WebSocket连接的Upgrade请求。
