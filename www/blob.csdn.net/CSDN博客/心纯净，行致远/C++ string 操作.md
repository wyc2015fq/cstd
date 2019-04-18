# C++ string 操作 - 心纯净，行致远 - CSDN博客





2018年11月15日 15:06:27[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：229








# C++ 字符串长度：



求字符串长度用**.size()或者.length() **

不要用sizeof()

# C++删除string最后一个字符的几种方法：

```cpp
#include<iostream>  
#include<string>  
using namespace std;  
int main()   
{  
    string str;  
    str = "123456";  
    cout << str << endl;  

    //方法一：使用substr()  
    str = str.substr(0, str.length() - 1);  
    cout << str << endl;  

    //方法二：使用erase()  
    str.erase(str.end() - 1);  
    cout << str << endl;  

    //方法三：使用pop_back()  
    str.pop_back();  
    cout << str << endl;  
    return 0;  
}
```

# c++ 删除字符串指定位置的字符



string erase(int start, int len); //start为要删除字符的起始位置（从0数起），len为要删除字符的个数。

# 截取子串



s.substr(pos, n) //截取s中从pos开始（包括0）的n个字符的子串，并返回 

s.substr(pos) //截取s中从从pos开始（包括0）到末尾的所有字符的子串，并返回

# 替换子串



s.replace(pos, n, s1) //用s1替换s中从pos开始（包括0）的n个字符的子串

# 查找子串



s.find(s1) 查找s中第一次出现s1的位置，并返回（包括0）

s.rfind(s1) 查找s中最后次出现s1的位置，并返回（包括0）

s.find_first_of(s1) 查找在s1中任意一个字符在s中第一次出现的位置，并返回（包括0）

s.find_last_of(s1) 查找在s1中任意一个字符在s中最后一次出现的位置，并返回（包括0）

s.fin_first_not_of(s1) 查找s中第一个不属于s1中的字符的位置，并返回（包括0）

s.fin_last_not_of(s1) 查找s中最后一个不属于s1中的字符的位置，并返回（包括0）





