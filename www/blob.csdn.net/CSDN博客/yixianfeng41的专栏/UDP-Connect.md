# UDP-Connect - yixianfeng41的专栏 - CSDN博客
2017年03月10日 16:17:26[第2梦](https://me.csdn.net/yixianfeng41)阅读数：544
所属专栏：[Linux/Unix 网络编程](https://blog.csdn.net/column/details/14513.html)
我们都知道TCP 是面向连接的，因此再传输数据之前必须要建立连接，也就是客户端调用connect()，服务端accept(),完成三次握手，建立连接。 
我们也知道UDP是面向无连接的，它传输数据之前是不需要建立连接的，那么对于UDP通信编程，我们是否需要调用connect了？如果可以，什么时候调用了？调用了有什么好处了？
### **一、发送接收函数**
首先回顾下，传输数据的两组函数
#### 1、tcp
```
int send(int s, const void *msg, size_t len, int flags); 
int recv(int s, void *buf, size_t len, int flags);
```
#### 2、udp
```
int sendto(int s, const void *msg, size_t len, int flags, const struct sockaddr *to, socklen_t tolen);
int  recvfrom(int  s, void *buf, size_t len, int flags, struct sockaddr *from,  socklen_t *fromlen);
```
### **二、UDP Connect**
从上面可以看到UDP的sendto、recvfrom类似于send、recv，只是后面多了三个参数，指定数据报接收者地址。而TCP 在connect就指定好地址了。
因此，在UDP通信中，如果你只往一个地址发送，那么你也可以先connect指定地址，然后调用 send,recv。
UDP，当你调用了connect,内核会检测其端口是否开放，有没有被使用，ip地址是否正确，也及时检查是否存在立即可以知道的错误，然后把对端的IP地址和端口号记录下来，发回给调用进程。注意，它是不会发生三次握手的。
对于已连接UDP套接字，调用connect，与缺省的未连接套接字相比，发生了以下变化： 
1）我们再也不能给输出操作指定目的IP地址和端口号。也就是说，我们不使用sendto,而改用write和send。写到已连接UDP的套接字上的任何内容都自动发送到由connect指定的协议地址。 
2）我么不必使用recvfrom已获悉数据报的发送者，而改用recv、read或recvnsg。 
3）由已连接UDP套接字引发的异步错误会返回给它们所在的进程，而未连接UDP套接字不接受任何异步错误。
#### **多次调用connect**
t拥有一个已连接UDP套接口的进程多次调用connec可以为下列2个目的之一： 
a.指定新的IP地址和端口号；  
b.断开套接口 
第一个目的（即给一个已连接UDP套接口指定新的对端）不同于TCP套接口中connect的使用:对于TCP套接口，connect只能调用一次。
为了断开一个已connect的UDP套接口连接，我们再次调用connect时把套接口地址结构的地址簇成员（sin_family）设置为AF_UNSPEC。 这么做可能返回一个EAFNOSUPPORT错误，不过没有关系。使得套接口断开连接的是在已连接UDP套接口上调用connect的进程。
#### **好处**
1）选定了对端，内核只会将绑定对象的对端发来的数据报传给套接口，因此在一定环境下可以提升安全性； 
2）会返回异步错误，如果对端没启动，默认情况下发送的包对应的ICMP回射包不会给调用进程，如果用了connect 
3）发送两个包间不要先断开再连接，提升了效率。
