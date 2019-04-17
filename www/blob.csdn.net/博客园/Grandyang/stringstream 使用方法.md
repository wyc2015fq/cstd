# stringstream 使用方法 - Grandyang - 博客园







# [stringstream 使用方法](https://www.cnblogs.com/grandyang/p/4315076.html)







C++中的stringstream是专门用来处理字符串流的，可以按顺序将string或int都拼接起来，而不用把int转换为string格式，使用方法如下：



```
#include <string>
#include <strstream>
#include <sstream>

stringstream strstream;
std::string s;
std::string s1 = "hello";
int d = 13;
strstream.str(""); strstream.clear();
strstream << s1 << "world" << d << "!"; strstream >> s;
```














