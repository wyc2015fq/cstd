# POCO库中文编程参考指南（5）Poco::Net::SocketAddress - DoubleLi - 博客园






## 1 枚举

最大地址长度，这个与`Poco::Net::IPAddress`中的定义可以类比，不过这里指的是`struct sockaddr_in6

```
enum
{
    MAX_ADDRESS_LENGTH = 
#if defined(POCO_HAVE_IPv6)
        sizeof(struct sockaddr_in6)
#else
        sizeof(struct sockaddr_in)
#endif
        /// Maximum length in bytes of a socket address.
};
```

## 2 构造函数

创建一个 wildcard 的全 0 的 IPv4 的 socket 地址：

```
SocketAddress();
```

用 Poco::Net::IPAddress 和端口号创建 IPv4 或 IPv6 的 SocketAddress：

```
SocketAddress(const IPAddress& host, Poco::UInt16 port);
```

用 std::string 和端口号创建 IPv4 或 IPv6 的 SocketAddress：

```
SocketAddress(const std::string& host, Poco::UInt16 port);
SocketAddress(const std::string& host, const std::string& port);
explicit SocketAddress(const std::string& hostAndPort);
```

拷贝构造函数：

```
SocketAddress(const SocketAddress& addr);
```

原生方式构造 SocketAddress：

```
SocketAddress(const struct sockaddr* addr, poco_socklen_t length);
```

## 3 常用函数

swap：

```
void swap(SocketAddress& addr);
```

获取 IPAddress、端口和地址类型：

```
IPAddress host() const;
Poco::UInt16 port() const;
IPAddress::Family family() const;
```

获取原生方式的地址长度、地址和地址族：

```
poco_socklen_t length() const;  
const struct sockaddr* addr() const;
int af() const;
```

toString：

```
std::string toString() const;
```

## 4 重载运算符

```
bool operator == (const SocketAddress& addr) const;
bool operator != (const SocketAddress& addr) const;
```

## 5 protected 函数

```
void init(const IPAddress& host, Poco::UInt16 port);
void init(const std::string& host, Poco::UInt16 port);
Poco::UInt16 resolveService(const std::string& service);
```

-

from：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)









