
# std::map initializer list syntax ? - Augusdi的专栏 - CSDN博客


2016年10月10日 12:33:20[Augusdi](https://me.csdn.net/Augusdi)阅读数：830个人分类：[VC++程序设计																](https://blog.csdn.net/Augusdi/article/category/519968)


﻿﻿
C++11 supports the initialization of most containers by the simple syntax as follows:
```python
std::set<int> = {3,4,6,34,2,6,78,8,5,0};
std::list<int> = {7,3,5,6,3,4,6,8};
```
map also has a constructor that takes std::initializer_list<value_type>:

```python
#include <map>
#include <string>
#include <iostream> 
int main() {
    std::map<int, std::string> m{{1, "Hello"}, {2, "world"}, {4, "!!!"}};
    for (std::map<int, std::string>::const_iterator it = m.begin(); it != m.end(); ++it) {
        std::cout << it->first << ' ' << it->second << '\n';
    }
}
```



