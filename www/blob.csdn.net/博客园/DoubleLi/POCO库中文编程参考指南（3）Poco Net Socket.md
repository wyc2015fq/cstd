# POCO库中文编程参考指南（3）Poco::Net::Socket - DoubleLi - 博客园






# POCO库中文编程参考指南（3）Poco::Net::Socket
- 作者：柳大·Poechant
- 博客：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)
- 邮箱：zhongchao.ustc#gmail.com (# -> @)
- 日期：April 14th, 2012

## 1 SelectMode

```
enum SelectMode
    /// The mode argument to poll() and select().
{
    SELECT_READ  = 1,
    SELECT_WRITE = 2,
    SELECT_ERROR = 4
};
```

## 2 SocketList

```
typedef std::vector<Socket> SocketList;
```

## 3 构造函数

未初始化的 socket：

```
Socket();
```

拷贝构造函数

```
Socket(const Socket& socket);
```

## 4 重载运算符

赋值运算符：

```
Socket& operator = (const Socket& socket);
```

比较运算符：

```
bool operator == (const Socket& socket) const;
bool operator != (const Socket& socket) const;
bool operator <  (const Socket& socket) const;
bool operator <= (const Socket& socket) const;
bool operator >  (const Socket& socket) const;
bool operator >= (const Socket& socket) const;
```

## 5 常用 socket 操作

返回该 socket 的可读数据的字节数，该操作不引起 socket 阻塞：

```
int available() const;
```

关闭 socket：

```
void close();
```

poll：

```
bool poll(const Poco::Timespan& timeout, int mode) const;
    /// Determines the status of the socket, using a 
    /// call to select().
    /// 
    /// The mode argument is constructed by combining the values
    /// of the SelectMode enumeration.
    ///
    /// Returns true if the next operation corresponding to
    /// mode will not block, false otherwise.

SocketImpl* impl() const;
    /// Returns the SocketImpl for this socket.
```

检查这个 socket 的连接是否是安全的（使用 SSL 或 TLS）：

```
bool secure() const;
```

## 6 缓冲区

发送数据的缓冲区：

```
void setSendBufferSize(int size);
int getSendBufferSize() const;
```

接收数据的缓冲区：

```
void setReceiveBufferSize(int size);
int getReceiveBufferSize() const;
```

## 7 超时时间

发送数据的超时时间：

```
void setSendTimeout(const Poco::Timespan& timeout);
Poco::Timespan getSendTimeout() const;
```

接收数据的超时时间：

```
void setReceiveTimeout(const Poco::Timespan& timeout);
Poco::Timespan getReceiveTimeout() const;
```

## 8 其他接口

```
void setOption(int level, int option, int value);
void setOption(int level, int option, unsigned value);
void setOption(int level, int option, unsigned char value);
void setOption(int level, int option, const Poco::Timespan& value);
void setOption(int level, int option, const IPAddress& value);

void getOption(int level, int option, int& value) const;
void getOption(int level, int option, unsigned& value) const;
void getOption(int level, int option, unsigned char& value) const;
void getOption(int level, int option, Poco::Timespan& value) const;
void getOption(int level, int option, IPAddress& value) const;

void setLinger(bool on, int seconds);
void getLinger(bool& on, int& seconds) const;

void setNoDelay(bool flag);
bool getNoDelay() const;

void setKeepAlive(bool flag);
bool getKeepAlive() const;

void setReuseAddress(bool flag);
bool getReuseAddress() const;

void setReusePort(bool flag);
bool getReusePort() const;

void setOOBInline(bool flag);
bool getOOBInline() const;

void setBlocking(bool flag);
bool getBlocking() const;
```

获取 socket 的 IP 和端口：

```
SocketAddress address() const;
```

获取 peer socket 的 IP 地址和端口：

```
SocketAddress peerAddress() const;
```

## 9 静态函数

select：

```
static int select(SocketList& readList,
                  SocketList& writeList, 
                  SocketList& exceptList, 
                  const Poco::Timespan& timeout);

    /// Determines the status of one or more sockets, 
    /// using a call to select().
    ///
    /// ReadList contains the list of sockets which should be
    /// checked for readability.
    ///
    /// WriteList contains the list of sockets which should be
    /// checked for writeability.
    ///
    /// ExceptList contains a list of sockets which should be
    /// checked for a pending error.
    ///
    /// Returns the number of sockets ready.
    ///
    /// After return, 
    ///   * readList contains those sockets ready for reading,
    ///   * writeList contains those sockets ready for writing,
    ///   * exceptList contains those sockets with a pending error.
    ///
    /// If the total number of sockets passed in readList, writeList and
    /// exceptList is zero, select() will return immediately and the
    /// return value will be 0.
    ///
    /// If one of the sockets passed to select() is closed while
    /// select() runs, select will return immediately. However,
    /// the closed socket will not be included in any list.
    /// In this case, the return value may be greater than the sum
    /// of all sockets in all list.
```

检查是否支持 IPv4 或 IPv6：

```
static bool supportsIPv4();
static bool supportsIPv6();
```

-

from：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)









