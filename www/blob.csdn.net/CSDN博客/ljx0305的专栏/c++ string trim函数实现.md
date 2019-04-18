# c++ string trim函数实现 - ljx0305的专栏 - CSDN博客
2012年08月23日 11:00:10[ljx0305](https://me.csdn.net/ljx0305)阅读数：3360
```cpp
#include <string>
#include <algorithm>
#include <functional>
using namespace std;
inline string&  LeftTrim(string   &str)
{
    string::iterator   iter=find_if(str.begin(),str.end(),not1(ptr_fun<int>(::isspace)));
    str.erase(str.begin(),iter);
    return  str;
}
inline  string&  RightTrim(string   &ss) 
{     
    string::reverse_iterator  rev_iter=find_if(str.rbegin(),str.rend(),not1(ptr_fun<int>(::isspace)));
    str.erase(rev_iter.base(),str.end());
    return   str;
}
inline  string&   Trim(string   &st)
{
     return LeftTrim(RigtTrim(st));
}
```
//参考文档　[http://www.cplusplus.com/reference/std/functional/ptr_fun/](http://www.cplusplus.com/reference/std/functional/ptr_fun/)
