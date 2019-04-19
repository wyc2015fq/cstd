# Python基础(四) socket简单通讯 - _天枢 - 博客园
## [Python基础(四) socket简单通讯](https://www.cnblogs.com/yhleng/p/7772295.html)
2017-11-02 14:52 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7772295)
**socket:我们通常听过的套接字：**
**服务端：**
1、创建socket对象
2、bing 绑定ip及端口
3、对该端口进行监听
4、消息阻塞（等待客户端消息）
**客户端：**
1、创建socket对象
2、连接服务ip及端口
3、发送消息到服务器ip
python 的socket通信，要使用socket内置包：
**服务端代码：**
```
#*_*coding:utf-8*_*
import socket
addr_ip = ('127.0.0.1',8005)
sk = socket.socket()
sk.bind(addr_ip)
sk.listen(5)
print('Service waiting.....')
conn,addr = sk.accept()
while True:
    client_msg =conn.recv(1024)
    print(str(client_msg))
    send_msg = raw_input('发送消息:')
    if send_msg !='exit':
        conn.sendall('服务小强:'+send_msg)
    else:
        break
conn.close()
```
**客户端代码：**
```
#*_*coding:utf-8*_*
import socket
addr_ip = ('127.0.0.1',8005)
sk = socket.socket()
sk.connect(addr_ip)
while True:
    send_msg = raw_input('发送消息:')
    if send_msg!='exit':
        sk.sendall('客户小强：'+send_msg)
        server_rev = sk.recv(1024)
        print(str(server_rev))
    else:
        break
sk.close()
```
启动服务端，接收客户端消息，并回复：
![](https://images2017.cnblogs.com/blog/1149221/201711/1149221-20171102145047451-587598822.png)
![](https://images2017.cnblogs.com/blog/1149221/201711/1149221-20171102145101998-886798634.png)
