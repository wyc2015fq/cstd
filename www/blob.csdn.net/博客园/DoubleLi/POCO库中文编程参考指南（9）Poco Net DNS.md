# POCO库中文编程参考指南（9）Poco::Net::DNS - DoubleLi - 博客园






## 1 Poco::Net::DNS

```
namespace Poco {
namespace Net {
class Net_API DNS {
public:
    static HostEntry hostByName(const std::string& hostname);
    static HostEntry hostByAddress(const IPAddress& address);
    static HostEntry resolve(const std::string& address);
    static IPAddress resolveOne(const std::string& address);
    static HostEntry thisHost();
    static std::string hostName();
};
}
}
```

HostEntry 中存储 host primary name（canonical name）、alias name list、IP address list。

## 2 相关 API

得到一个 HostEntry 实例：

```
const HostEntry& entry = DNS::hostByName("google.com");
```

输出域名：

```
std::cout << entry.name() << std::endl;
```

Address 和 Alias：

```
const HostEntry::AliasList& aliases = entry.aliases();
const HostEntry::AddressList& addrs = entry.addresses();
```

## 3 Source code

POCO 的官方文档中的 Sample 有错误，以下提供一个正确的示例，其中域名是通过参数传入的。

```
#include "Poco/Net/DNS.h"
#include <iostream>
using Poco::Net::DNS;
using Poco::Net::IPAddress;
using Poco::Net::HostEntry;
int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Invalid argument number." << std::endl;
    }
    const HostEntry& entry = DNS::hostByName(argv[1]);
    std::cout << "Canonical Name: " << entry.name() << std::endl;

    const HostEntry::AliasList& aliases = entry.aliases();
    for (HostEntry::AliasList::const_iterator it = aliases.begin();
        it !=   aliases.end(); ++it)
        std::cout << "Alias: " << *it << std::endl;

    const HostEntry::AddressList& addrs = entry.addresses();
    for (HostEntry::AddressList::const_iterator it = addrs.begin();
        it !=   addrs.end(); ++it)
        std::cout << "Address: " << it->toString() << std::endl;

    return 0;
}
```

编译：

```
$ g++ name_solver.cpp -o name_solver \
-I/usr/local/include -I/usr/local/lib -lPocoNet
```

运行：

```
$ ./name_solver baidu.com
Canonical Name: baidu.com
Address: 123.125.114.144
Address: 123.125.114.144
Address: 220.181.111.85
Address: 220.181.111.85
Address: 220.181.111.86
Address: 220.181.111.86

$ ./name_solver www.ustc.edu.cn
Canonical Name: ustc.edu.cn
Address: 202.38.64.246
Address: 202.38.64.246
```

-

转载请注明来自柳大的CSDN博客：[Blog.CSDN.net/Poechant](file:///Users/michael/Writing/CSDN%E5%8D%9A%E5%AE%A2-%E8%8D%89%E7%A8%BF%E7%AE%B1/Blog.CSDN.net/Poechant)









