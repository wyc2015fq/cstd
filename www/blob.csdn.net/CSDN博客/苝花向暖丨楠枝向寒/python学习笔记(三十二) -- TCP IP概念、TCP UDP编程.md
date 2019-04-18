# python学习笔记(三十二) -- TCP\IP概念、TCP\UDP编程 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月10日 22:56:04[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：44
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**Table of Contents**

[TCP\IP概念](#TCP%5CIP%E6%A6%82%E5%BF%B5)

[TCP](#TCP)

[UDP](#UDP)

# TCP\IP概念

计算机之间通信需要统一通信的标准，才可以通信。就像不同国家的人说不同的语言是无法交流的。

TCP\IP协议就是通信的标准。

其中IP协议用来发送数据，它将数据划分为多个数据包，然后通过网络进行传输，数据包会经过多个路由器从一台计算机到另外一台计算机，其中路由器来控制每一个数据包传输的线路，所以就会导致数据包的到达顺序与原发送时的顺序不一样。

IP协议又分为IPV4和IPV6

P地址实际上是一个32位整数（称为IPv4），以字符串表示的IP地址如192.168.0.1实际上是把32位整数按8位分组后的数字表示，目的是便于阅读。

IPv6地址实际上是一个128位整数，它是目前使用的IPv4的升级版，以字符串表示类似于2001:0db8:85a3:0042:1000:8a2e:0370:7334。

# TCP

TCP协议，负责在两台计算机之间建立可靠连接，保证数据包按顺序到达。TCP协议会通过握手建立连接，然后，对每个IP包编号，确保对方按顺序收到，如果包丢掉了，就自动重发。

一个TCP报文除了包含要传输的数据外，还包含源IP地址和目标IP地址，源端口和目标端口。

端口：每一个计算机会有很多个接入网络的程序，为了区分这些程序使用ip地址+端口号来作为一个网络程序的唯一标识。

**例一：**

下面使用socket模块，实现通过tcp协议向百度服务器发送get方式的请求，并获取返回内容。

```python
# encoding=utf-8
import socket

# AF_INET指定使用IPv4协议，如果要用更先进的IPv6，就指定为AF_INET6, SOCK_STREAM指定使用面向流的TCP协议
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('www.baidu.com', 80))
# 以get方式使用http协议向百度发送请求:
s.send(b'GET / HTTP/1.1\r\nHost: www.baidu.com\r\nConnection: close\r\n\r\n')
# 接收数据:
buffer = []
while True:
    # 每次最多接收1k字节:
    d = s.recv(1024)
    if d:
        buffer.append(d)
    else:
        break
data = b''.join(buffer)
# 关闭连接:
s.close()
header, html = data.split(b'\r\n\r\n', 1)
print(header.decode('utf-8'))
# 把接收的数据写入文件:
with open(r'D:\sina.html', 'wb') as f:
    f.write(html)
```

效果如下 

![](https://img-blog.csdn.net/20180910220120734?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们可以发现图片并没有显示出来。我们看一下页面的源码

![](https://img-blog.csdn.net/20180910220318713?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以发现，这个图片前面缺少了http:  这几个字符，所以显示不出来。

以后爬取图片的时候一定要对爬取下来的源码进行字符串的拼接。

**例二：**

模拟一个客户端和服务端的交互

服务端代码如下：

```python
# encoding=utf-8
import socket
import threading
import time

def tcplink(sock, addr):
    print('Accept new connection from %s:%s...' % addr) # addr是一个元组，包含ip和端口号
    #  通过客户端的socket 给客户端发送信息，注意网络间的通信本质传递的是二进制数据
    #  所以发送的应该是byte类型的数据
    sock.send(b'Welcome!')
    while True:
        data = sock.recv(1024)  # 接收客户端发送过来的数据
        time.sleep(1)   # 让线程等待1s再执行
        if not data or data.decode('utf-8') == 'exit':
            break
        # 在客户端发送过来的数据前面加上Hello
        # 注意同样发送过去的必须是byte类型的数据
        sock.send(('Hello,%s!' % data.decode('utf-8')).encode('utf-8'))
    sock.close()  # 关掉客户端的socket
    print('Connection from %s:%s closed.' % addr)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 绑定ip和端口，如果ip为0.0.0.0则绑定所有网卡：
s.bind(('127.0.0.1', 9999))
# 开始监听
s.listen(5)

while True:
    print('Waiting for connection...')
    # 接收连接,通过服务端的socket接收客户端发送过来的请求，返回一个客户端的socket以及客户端的ip
    sock, addr = s.accept()  # 这里如果没有接收到连接，会阻塞在这里，不会执行下面的代码
    print('get a connection')
    # 创建出来一个线程来去回应客户端
    # 这里使用多线程的目的就是为了能够同时给多个客户端做出回应
    t = threading.Thread(target=tcplink, args=(sock, addr))
    t.start()
```

客户端代码如下：

```python
import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 建立连接:
s.connect(('127.0.0.1', 9999))
# 接收欢迎消息:
print(s.recv(1024).decode('utf-8'))
for data in [b'Michael', b'Tracy', b'Sarah']:
    # 发送数据:
    s.send(data)
    print(s.recv(1024).decode('utf-8'))
s.send(b'exit')
s.close()
```

# UDP

TCP的特点就是保证数据能够发送给对方，因为其先建立连接，再发送数据，如果过程中数据丢失还会重新发送，其实TCP还有三次握手确认，想了解的自行百度。

而UDP的特点就是传输效率高，效率高的原因是因为它不会去建立连接，你告诉它把数据发送到哪个ip地址，哪个端口号，它就直接传输了，数据丢失也不会重新发送。

下面是UDP协议使用的案例：

服务端

```python
# encoding=utf-8
import socket
import threading

def udplink(data, addr):
        print('Received from %s:%s.' % addr)
        # 向客户端发送数据
        s.sendto(b'Hello, %s!' % data, addr)

# socket.SOCK_DGRAM 就代表使用UDP协议
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# 绑定端口
s.bind(('127.0.0.1', 9999))
# 接收数据
# 注意这里和TCP的区别
# TCP是先获取连接，返回一个客户端的socket，再接收数据
# 这里直接通过客户端的socket接收数据
while 1:  # 这里我用 1代替了True，原因是 1效率更高
    data, addr = s.recvfrom(1024)  # 这里如果没有接收到数据也会阻塞
    t = threading.Thread(target=udplink, args=(data, addr))
    t.start()
```

客户端

```python
import socket

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
for data in [b'Michael', b'Tracy', b'Sarah']:
    # 发送数据:
    s.sendto(data, ('127.0.0.1', 9999))
    # 接收数据:
    print(s.recv(1024).decode('utf-8'))
s.close()
```

