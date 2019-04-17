# POCO库中文编程参考指南（7）Poco::Net::DatagramSocket - DoubleLi - 博客园






## 1 构造函数

创建一个未连接的 IPv4 数据报 Socket：

```
DatagramSocket();
```

创建一个指定 IP 类型（IPv4 或 IPv6）的数据报 Socket：

```
explicit DatagramSocket(IPAddress::Family family);
```

创建一个指定 SocketAddress 的数据报 Socket

```
DatagramSocket(const SocketAddress& address, bool reuseAddress = false);

DatagramSocket(const Socket& socket);
    /// Creates the DatagramSocket with the SocketImpl
    /// from another socket. The SocketImpl must be
    /// a DatagramSocketImpl, otherwise an InvalidArgumentException
    /// will be thrown.
```

## 2 重载运算符

```
DatagramSocket& operator = (const Socket& socket);
    /// Assignment operator.
    ///
    /// Releases the socket's SocketImpl and
    /// attaches the SocketImpl from the other socket and
    /// increments the reference count of the SocketImpl.
```

## 3 常用操作

### 3.1 连接与绑定

```
void connect(const SocketAddress& address);
void bind(const SocketAddress& address, bool reuseAddress = false);
```

### 3.2 收发数据且不考虑 client

```
int sendBytes(const void* buffer, int length, int flags = 0);
int receiveBytes(void* buffer, int length, int flags = 0);
```

### 3.3 手法数据且获取 client 的 SocketAddress

```
int sendTo(const void* buffer, int length, const SocketAddress& address, int flags = 0);
int receiveFrom(void* buffer, int length, SocketAddress& address, int flags = 0);
```

### 3.4 收发广播

```
void setBroadcast(bool flag);
bool getBroadcast() const;
```

## 4 protected 函数

```
DatagramSocket(SocketImpl* pImpl);
```

-

from：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)









