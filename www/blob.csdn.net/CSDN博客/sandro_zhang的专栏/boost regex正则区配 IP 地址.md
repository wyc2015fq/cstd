# boost regex正则区配 IP 地址 - sandro_zhang的专栏 - CSDN博客
2011年09月21日 15:57:41[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：1527标签：[regex																[iterator																[include																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=regex&t=blog)
个人分类：[boost](https://blog.csdn.net/sandro_zhang/article/category/892177)
```cpp
#include <boost/regex.hpp>
#include <iostream>
#include <string>
int main()
{
    std::string text(" 192.168.0.1 abc 10.0.0.255 10.5.1 1.2.3.4a 5.4.3.2 ");
    const char* pattern =
        "\\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"
        "\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"
        "\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"
        "\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b";
    boost::regex ip_regex(pattern);
    boost::sregex_iterator it(text.begin(), text.end(), ip_regex);
    boost::sregex_iterator end;
    for (; it != end; ++it) {
        std::cout << it->str() << "\n";
        // v.push_back(it->str()); or something similar     
    }
}
```
输出：
```cpp
192.168.0.1
10.0.0.255
5.4.3.2
```
参考：[http://stackoverflow.com/questions/5804453/c-regular-expressions-with-boost-regex](http://stackoverflow.com/questions/5804453/c-regular-expressions-with-boost-regex)
