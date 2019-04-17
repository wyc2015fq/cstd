# 一文读懂 WebSocket 通信过程与实现 - zhusongziye的博客 - CSDN博客





2018年05月14日 22:12:23[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：963








什么是 WebSocket ？

WebSocket 是一种标准协议，用于在客户端和服务端之间进行双向数据传输。但它跟 HTTP 没什么关系，它是一种基于 TCP 的一种独立实现。

以前客户端想知道服务端的处理进度，要不停地使用 Ajax 进行轮询，让浏览器隔个几秒就向服务器发一次请求，这对服务器压力较高。另外一种轮询就是采用 long poll 的方式，这就跟打电话差不多，没收到消息就一直不挂电话，也就是说，客户端发起连接后，如果没消息，就一直不返回 Response 给客户端，连接阶段一直是阻塞的。

而 WebSocket 解决了 HTTP 的这几个难题。首先，当服务器完成协议升级后（ HTTP -> WebSocket ），服务端可以主动推送信息给客户端，解决了轮询造成的同步延迟问题。由于 WebSocket 只需要一次 HTTP 握手，服务端就能一直与客户端保持通讯，直到关闭连接，这样就解决了服务器需要反复解析 HTTP 协议，减少了资源的开销。

![](https://img-blog.csdn.net/20180514221000929?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




随着新标准的推进，WebSocket 已经比较成熟了，并且各个浏览器对 WebSocket 的支持情况比较好，有空可以看看。




![](https://img-blog.csdn.net/2018051422103358?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





使用 WebSocket 的时候，前端使用是比较规范的，js 支持 ws 协议，感觉类似于一个轻度封装的 Socket 协议，只是以前需要自己维护 Socket 的连接，现在能够以比较标准的方法来进行。




客户端请求报文及实现




客户端请求报文：




GET / HTTP/1.1
Upgrade: websocket
Connection: Upgrade
Host: example.com
Origin: http://example.com
Sec-WebSocket-Key: sN9cRrP/n9NdMgdcy2VJFQ==
Sec-WebSocket-Version: 13

与传统 HTTP 报文不同的地方：

Upgrade: websocket
Connection: Upgrade

这两行表示发起的是 WebSocket 协议。



Sec-WebSocket-Key: sN9cRrP/n9NdMgdcy2VJFQ==
Sec-WebSocket-Version: 13





`Sec-WebSocket-Key` 是由浏览器随机生成的，提供基本的防护，防止恶意或者无意的连接。

`Sec-WebSocket-Version` 表示 WebSocket 的版本，最初 WebSocket 协议太多，不同厂商都有自己的协议版本，不过现在已经定下来了。如果服务端不支持该版本，需要返回一个 `Sec-WebSocket-Versionheader`，里面包含服务端支持的版本号。

创建 WebSocket 对象：



var ws = new websocket("ws://127.0.0.1:8001");

ws 表示使用 WebSocket 协议，后面接地址及端口





完整的客户端代码：



<script type="text/javascript">
var ws;
var box = document.getElementById('box');

functionstartWS(){
        ws = new WebSocket('ws://127.0.0.1:8001');
        ws.onopen = function(msg){
console.log('WebSocket opened!');
        };
        ws.onmessage = function(message){
console.log('receive message: ' + message.data);
            box.insertAdjacentHTML('beforeend', '<p>' + message.data + '</p>');
        };
        ws.onerror = function(error){
console.log('Error: ' + error.name + error.number);
        };
        ws.onclose = function(){
console.log('WebSocket closed!');
        };
    }

functionsendMessage(){
console.log('Sending a message...');
var text = document.getElementById('text');
        ws.send(text.value);
    }

window.onbeforeunload = function(){
        websocket.onclose = function(){};  // 首先关闭 WebSocket
        websocket.close()
    };
</script>



服务端响应报文及实现




首先我们来看看服务端的响应报文



HTTP/1.1101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: HSmrc0sMlYUkAGmm5OPpG2HaGWk=
Sec-WebSocket-Protocol: chat





我们一行行来解释

1、首先，101 状态码表示服务器已经理解了客户端的请求，并将通过 `Upgrade` 消息头通知客户端采用不同的协议来完成这个请求；

2、然后，`Sec-WebSocket-Accept` 这个则是经过服务器确认，并且加密过后的 `Sec-WebSocket-Key`；

3、最后，`Sec-WebSocket-Protocol` 则是表示最终使用的协议。

`Sec-WebSocket-Accept` 的计算方法：

1、将 `Sec-WebSocket-Key` 跟 258EAFA5-E914-47DA-95CA-C5AB0DC85B11 拼接；

2、通过 SHA1 计算出摘要，并转成 base64 字符串。

PS：`Sec-WebSocket-Key` / `Sec-WebSocket-Accept` 的换算，只能带来基本的保障，但连接是否安全、数据是否安全、客户端 / 服务端是否合法的 ws 客户端、ws 服务端，其实并没有实际性的保证。

创建主线程，用于实现接受 WebSocket 建立请求：



defcreate_socket():
# 启动 Socket 并监听连接
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
        sock.bind(('127.0.0.1', 8001))

# 操作系统会在服务器 Socket 被关闭或服务器进程终止后马上释放该服务器的端口，否则操作系统会保留几分钟该端口。
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.listen(5)
except Exception as e:
        logging.error(e)
return
else:
        logging.info('Server running...')

# 等待访问
whileTrue:
        conn, addr = sock.accept()  # 此时会进入 waiting 状态

        data = str(conn.recv(1024))
        logging.debug(data)

        header_dict = {}
        header, _ = data.split(r'\r\n\r\n', 1)
for line in header.split(r'\r\n')[1:]:
            key, val = line.split(': ', 1)
            header_dict[key] = val

if'Sec-WebSocket-Key'notin header_dict:
            logging.error('This socket is not websocket, client close.')
            conn.close()
return

        magic_key = '258EAFA5-E914-47DA-95CA-C5AB0DC85B11'
        sec_key = header_dict['Sec-WebSocket-Key'] + magic_key
        key = base64.b64encode(hashlib.sha1(bytes(sec_key, encoding='utf-8')).digest())
        key_str = str(key)[2:30]
        logging.debug(key_str)

        response = 'HTTP/1.1 101 Switching Protocols\r\n' \
'Connection: Upgrade\r\n' \
'Upgrade: websocket\r\n' \
'Sec-WebSocket-Accept: {0}\r\n' \
'WebSocket-Protocol: chat\r\n\r\n'.format(key_str)
        conn.send(bytes(response, encoding='utf-8'))

        logging.debug('Send the handshake data')

        WebSocketThread(conn).start()




进行通信解析 WebSocket 报文及实现





Server 端接收到浏览器发来的报文需要进行解析


浏览器包格式

![](https://img-blog.csdn.net/20180514221105769?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1、FIN: 占 1 个 bit

0：不是消息的最后一个分片
1：是消息的最后一个分片

2、RSV1, RSV2, RSV3：各占 1 个 bit

一般情况下全为 0。当客户端、服务端协商采用 WebSocket 扩展时，这三个标志位可以非
0，且值的含义由扩展进行定义。如果出现非零的值，且并没有采用 WebSocket 扩展，连接出错。

3、Opcode: 4 个 bit

%x0：表示一个延续帧。当 Opcode 为 0 时，表示本次数据传输采用了数据分片，当前收到的数据帧为其中一个数据分片；
%x1：表示这是一个文本帧（frame）；
%x2：表示这是一个二进制帧（frame）；
%x3-7：保留的操作代码，用于后续定义的非控制帧；
%x8：表示连接断开；
%x9：表示这是一个 ping 操作；
%xA：表示这是一个 pong 操作；
%xB-F：保留的操作代码，用于后续定义的控制帧。

4、Mask: 1 个 bit

表示是否要对数据载荷进行掩码异或操作。
0：否
1：是

5、Payload length: 7bit or 7 + 16bit or 7 + 64bit

表示数据载荷的长度
`x` 为 0~126：数据的长度为 `x` 字节；
`x` 为 126：后续 2 个字节代表一个 16 位的无符号整数，该无符号整数的值为数据的长度；
`x` 为 127：后续 8 个字节代表一个 64 位的无符号整数（最高位为 0），该无符号整数的值为数据的长度。

6、Masking-key: 0 or 4bytes

当 Mask 为 1，则携带了 4 字节的 Masking-key；
当 Mask 为 0，则没有 Masking-key。
PS：掩码的作用并不是为了防止数据泄密，而是为了防止早期版本的协议中存在的代理缓存污染攻击（proxy cache poisoning attacks）等问题。

7、Payload Data: 载荷数据




解析 WebSocket 报文代码如下：




defread_msg(data):
    logging.debug(data)

    msg_len = data[1] & 127# 数据载荷的长度
if msg_len == 126:
        mask = data[4:8]  # Mask 掩码
        content = data[8:]  # 消息内容
elif msg_len == 127:
        mask = data[10:14]
        content = data[14:]
else:
        mask = data[2:6]
        content = data[6:]

    raw_str = ''# 解码后的内容
for i, d in enumerate(content):
        raw_str += chr(d ^ mask[i % 4])
return raw_str




服务端发送 WebSocket 报文

返回时不携带掩码，所以 Mask 位为 0，再按载荷数据的大小写入长度，最后写入载荷数据。

struct 模块解析

struct.pack(fmt, v1, v2, ...)

按照给定的格式 (fmt)，把数据封装成字符串 ( 实际上是类似于 c 结构体的字节流 )

struct 中支持的格式如下表：

![](https://img-blog.csdn.net/20180514221129402?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

为了同 C 语言中的结构体交换数据，还要考虑有的 C 或 C++ 编译器使用了字节对齐，通常是以 4 个字节为单位的 32 位系统，故而 struct 根据本地机器字节顺序转换。可以用格式中的第一个字符来改变对齐方式，定义如下：

![](https://img-blog.csdn.net/20180514221136751?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

发送 WebSocket 报文代码如下：



defwrite_msg(message):
    data = struct.pack('B', 129)  # 写入第一个字节，10000001

# 写入包长度
    msg_len = len(message)
if msg_len <= 125:
        data += struct.pack('B', msg_len)
elif msg_len <= (2 ** 16 - 1):
        data += struct.pack('!BH', 126, msg_len)
elif msg_len <= (2 ** 64 - 1):
        data += struct.pack('!BQ', 127, msg_len)
else:
        logging.error('Message is too long!')
return

    data += bytes(message, encoding='utf-8')  # 写入消息内容
    logging.debug(data)
return data





## 总结

没有其他能像 WebSocket 一样实现双向通信的技术了，迄今为止，大部分开发者还是使用 Ajax 轮询来实现，但这是个不太优雅的解决办法，WebSocket 虽然用的人不多，可能是因为协议刚出来的时候有安全性的问题以及兼容的浏览器比较少，但现在都有解决。如果你有这些需求可以考虑使用 WebSocket：

1 、多个用户之间进行交互；

2、需要频繁地向服务端请求更新数据。

比如弹幕、消息订阅、多玩家游戏、协同编辑、股票基金实时报价、视频会议、在线教育等需要高实时的场景。

参考文章

https://www.zhihu.com/question/20215561/answer/40316953

http://www.52im.net/thread-1341-1-1.html

http://fullstackpython.atjiang.com/websockets.html




> 
来源： wzhvictor     链接：

https://segmentfault.com/a/1190000014643900




