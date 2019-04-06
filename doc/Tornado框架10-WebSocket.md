# Tornado框架10-WebSocket

 

# 01-实时获取数据

- 前端轮询:有无数据立即回复
- 长轮询:没有数据改变，不做任何响应,当有数据改变时，服务器响应
- WebSocket

# 02-概述

WebSocket是HTML5规范中新提出的客户端-服务器通讯协议，协议本身使用新的ws://URL格式。

WebSocket 是独立的、创建在 TCP 上的协议，和 HTTP 的唯一关联是使用 HTTP 协议的101状态码进行协议切换，使用的 TCP 端口是80，可以用于绕过大多数防火墙的限制。

WebSocket 使得客户端和服务器之间的数据交换变得更加简单，允许服务端直接向客户端推送数据而不需要客户端进行请求，两者之间可以创建持久性的连接，并允许数据进行双向传送。

目前常见的浏览器如 Chrome、IE、Firefox、Safari、Opera 等都支持 WebSocket，同时需要服务端程序支持 WebSocket。

# 03-Tornado的WebSocket模块

Tornado提供支持WebSocket的模块是tornado.websocket，其中提供了一个WebSocketHandler类用来处理通讯。

被动调用的三种方法：

> WebSocketHandler.open()

当一个WebSocket连接建立后被调用。

> WebSocketHandler.on_message(message)

当客户端发送消息message过来时被调用，注意此方法必须被重写。

> WebSocketHandler.on_close()

当WebSocket连接关闭后被调用。

其他方法：

> WebSocketHandler.write_message(message, binary=False)

向客户端发送消息messagea，message可以是字符串或字典（字典会被转为json字符串）。若binary为False，则message以utf8编码发送；二进制模式（binary=True）时，可发送任何字节码。

> WebSocketHandler.close()

关闭WebSocket连接。

> WebSocketHandler.check_origin(origin)

判断源origin，对于符合条件（返回判断结果为True）的请求源origin允许其连接，否则返回403。可以重写此方法来解决WebSocket的跨域请求（如始终return True）。

# 03-demo

前端代码：

```
<body>
    <div id="contents" style="height:800px;width:800px;overflow:auto;background-color:red"></div>
    <div>
        <input id="message" type="text" placeholder="请输入聊天内容"/>
        <button onclick="sendMessage()">发送</button>
    </div>
    <script>
        //发起WebSocket链接
        var ws = new WebSocket("ws://10.0.147.77:8000/chat")

        //接收服务器推送的数据
        ws.onmessage = function(e){
            $("#contents").append("<p>"+e.data+"</p>")
        }

        //给服务器发送数据
        function sendMessage(){
            var dataStr = $("#message").val()
            ws.send(dataStr)
            $("#message").val("")
        }
    </script>
</body>
```

后端代码：

```
class ChatHandler(WebSocketHandler):
    users = []
    def open(self, *args, **kwargs):
        #将登陆的用户存储到用户列表
        self.users.append(self)
        for u in self.users:
            u.write_message("[%s]登陆聊天室"%(self.request.remote_ip))
    def on_close(self):
        self.users.remove(self)
        for u in self.users:
            u.write_message("[%s]离开聊天室"%(self.request.remote_ip))
    def on_message(self, message):
        for u in self.users:
            u.write_message("[%s]说：%s"%(self.request.remote_ip, message))
    def check_origin(self,origin):
        #允许WebSocket的跨域请求
        return True
```