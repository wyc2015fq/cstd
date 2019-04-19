# Python WebSocket长连接心跳与短连接 - _天枢 - 博客园
## [Python WebSocket长连接心跳与短连接](https://www.cnblogs.com/yhleng/p/9487764.html)
2018-08-16 15:51 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9487764)
**python websocket**
**安装**
```
pip install websocket-client
```
先来看一下，长连接调用方式：
```
ws = websocket.WebSocketApp("ws://echo.websocket.org/",
                              on_message = on_message,
                              on_error = on_error,
                              on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()
```
** 长连接，参数介绍：**
（1）url: websocket的地址。
（2）header: 客户发送websocket握手请求的请求头，{'head1:value1','head2:value2'}。
（3）on_open：在建立Websocket握手时调用的可调用对象，这个方法只有一个参数，就是该类本身。
（4）on_message：这个对象在接收到服务器返回的消息时调用。有两个参数，一个是该类本身，一个是我们从服务器获取的字符串（utf-8格式）。
（5）on_error：这个对象在遇到错误时调用，有两个参数，第一个是该类本身，第二个是异常对象。
（6）on_close：在遇到连接关闭的情况时调用，参数只有一个，就是该类本身。
（7）on_cont_message：这个对象在接收到连续帧数据时被调用，有三个参数，分别是：类本身，从服务器接受的字符串（utf-8），连续标志。
（8）on_data：当从服务器接收到消息时被调用，有四个参数，分别是：该类本身，接收到的字符串（utf-8），数据类型，连续标志。
（9）keep_running：一个二进制的标志位，如果为True，这个app的主循环将持续运行，默认值为True。
（10）get_mask_key：用于产生一个掩码。
（11）subprotocols：一组可用的子协议，默认为空。
**长连接关键方法：**ws.run_forever(ping_interval=60,ping_timeout=5)
如果不断开关闭websocket连接，会一直阻塞下去。另外这个函数带两个参数，如果传的话，启动心跳包发送。
ping_interval:自动发送“ping”命令，每个指定的时间(秒),如果设置为0，则不会自动发送。
ping_timeout:如果没有收到pong消息，则为超时(秒)。
```
ws.run_forever(ping_interval=60,ping_timeout=5)
#ping_interval心跳发送间隔时间
#ping_timeout 设置，发送ping到收到pong的超时时间
```
**我们看源代码，会发现这样一断代码：**
ping的超时时间，要大于ping间隔时间
```
if not ping_timeout or ping_timeout <= 0:
            ping_timeout = None
        if ping_timeout and ping_interval and ping_interval <= ping_timeout:
            raise WebSocketException("Ensure ping_interval > ping_timeout")
```
**长连接：**
**示例1：**
```
import websocket
try:
    import thread
except ImportError:
    import _thread as thread
import time
def on_message(ws, message):
    print(message)
def on_error(ws, error):
    print(error)
def on_close(ws):
    print("### closed ###")
def on_open(ws):
    def run(*args):
        ws.send("hello1")
        time.sleep(1)
        ws.close()
    thread.start_new_thread(run,())
if __name__ == "__main__":
    websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://echo.websocket.org/",
                              on_message = on_message,
                              on_error = on_error,
                              on_close = on_close)
    ws.on_open = on_open
    ws.run_forever(ping_interval=60,ping_timeout=5)
```
**示例2：**
```
import websocket
from threading import Thread
import time
import sys
class MyApp(websocket.WebSocketApp):
    def on_message(self, message):
        print(message)
    def on_error(self, error):
        print(error)
    def on_close(self):
        print("### closed ###")
    def on_open(self):
        def run(*args):
            for i in range(3):
                # send the message, then wait
                # so thread doesn't exit and socket
                # isn't closed
                self.send("Hello %d" % i)
                time.sleep(1)
            time.sleep(1)
            self.close()
            print("Thread terminating...")
        Thread(target=run).start()
if __name__ == "__main__":
    websocket.enableTrace(True)
    if len(sys.argv) < 2:
        host = "ws://echo.websocket.org/"
    else:
        host = sys.argv[1]
    ws = MyApp(host)
    ws.run_forever()
```
**短连接：**
```
from websocket import create_connection
ws = create_connection("ws://echo.websocket.org/")
print("Sending 'Hello, World'...")
ws.send("Hello, World")
print("Sent")
print("Receiving...")
result =  ws.recv()
print("Received '%s'" % result)
ws.close()
```
——
