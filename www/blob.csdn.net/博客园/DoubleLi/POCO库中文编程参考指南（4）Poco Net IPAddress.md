# POCO库中文编程参考指南（4）Poco::Net::IPAddress - DoubleLi - 博客园






# POCO库中文编程参考指南（4）Poco::Net::IPAddress
- 作者：柳大·Poechant
- 博客：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)
- 邮箱：zhongchao.ustc#gmail.com (# -> @)
- 日期：April 14th, 2012

## 1 Poco::Net::IPAddress

地址最大长度，IPv4 是 in_addr 的长度，IPv6 是 in6_addr 的长度

```
enum
{
    MAX_ADDRESS_LENGTH = 
#if defined(POCO_HAVE_IPv6)
        sizeof(struct in6_addr)
#else
        sizeof(struct in_addr)
#endif
        /// Maximum length in bytes of a socket address.
};
```

### 1.1 IPv4 or IPv6?

POCO 用一个枚举类来表示：

```
enum Family
    /// Possible address families for IP addresses.
{
    IPv4,
    IPv6
};
```

### 1.2 构造函数

返回一个全 0 的 IP 地址：

```
IPAddress();
```

拷贝构造函数：

```
IPAddress(const IPAddress& addr);
```

根据 IP 类型（IPv4 或 IPv6）创建一个全 0 的 IP 地址：

```
explicit IPAddress(Family family);
```

用字符串类型的 IPv4 地址或 IPv6 地址来创建一个 IP 地址，其中 IPv4 是十进制表示的，IPv6 是十六进制表示的：

```
explicit IPAddress(const std::string& addr);
```

用字符串类型的 IP 地址和指定的类型（IPv4 或 IPv6）创建一个 IP 地址，其中 IPv4 是十进制表示的，IPv6 是十六进制表示的：

```
IPAddress(const std::string& addr, Family family);
```

用原生 IP 地址 in_addr 或 in6_addr 数据结构和给定的字符串长度，创建一个 IPAddress：

```
IPAddress(const void* addr, poco_socklen_t length);
```

用原生 IP 地址 in_addr 或 in6_addr 数据结构和给定的字符串长度，创建一个 IPAddress，其中 scope 参数用于 IPv6，IPv4 会忽略这个参数：

```
IPAddress(const void* addr, poco_socklen_t length, Poco::UInt32 scope);
```

### 1.3 重载运算符

赋值运算符：

```
IPAddress& operator = (const IPAddress& addr);
```

比较运算符

```
bool operator == (const IPAddress& addr) const;
bool operator != (const IPAddress& addr) const;
bool operator <  (const IPAddress& addr) const;
bool operator <= (const IPAddress& addr) const;
bool operator >  (const IPAddress& addr) const;
bool operator >= (const IPAddress& addr) const;
```

### 1.4 常用操作

swap：

```
void swap(IPAddress& address);
```

获取地址类型：

```
Family family() const;
```

IPv6 专用函数，返回 scope identifier，如果是 IPv4 则返回 0：

```
Poco::UInt32 scope() const;
```

toString，IPv4 则显示“d.d.d.d”，IPv6 则

```
std::string toString() const;

poco_socklen_t length() const;

const void* addr() const;

int af() const;

void mask(const IPAddress& mask);

void mask(const IPAddress& mask, const IPAddress& set);

static IPAddress parse(const std::string& addr);

static bool tryParse(const std::string& addr, IPAddress& result);

static IPAddress wildcard(Family family = IPv4);

static IPAddress broadcast();
```

### 1.5 IS 函数

#### 1.5.1 是否是未初始化状态

是否是未初始化的全零状态（wildcard）：

```
bool isWildcard() const;
```

#### 1.5.2 是否是广播/组播/单播

是否是广播地址（全零），与 wildcard 的区别是 wildcard 是未经过初始化的。只有 IPv4 有广播地址，IPv6 则返回 false。

```
bool isBroadcast() const;
```

是否是回环地址：
- 对于 IPv4 是`127.0.0.1`
- 对于 IPv6 是`::1`

函数原型：

```
bool isLoopback() const;
```

是否是多播：
- 对于 IPv4 是`224.0.0.0`到`239.255.255.255`范围；
- 对于 IPv6 是`FFxx:x:x:x:x:x:x:x`的范围：

函数原型：

```
bool isMulticast() const;
```

是否是单播：

```
bool isUnicast() const;
```

#### 1.5.2 其他函数

```
bool isLinkLocal() const;
bool isSiteLocal() const;
bool isIPv4Compatible() const;
bool isIPv4Mapped() const;
bool isWellKnownMC() const;
bool isNodeLocalMC() const;
bool isLinkLocalMC() const;
bool isSiteLocalMC() const;
bool isOrgLocalMC() const;
bool isGlobalMC() const;
```

### 1.5 protected 函数

```
protected:
    void init(IPAddressImpl* pImpl);
```

-

from：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)









