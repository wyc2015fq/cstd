# string::npos总结 - 紫芝的博客 - CSDN博客





2018年06月17日 21:31:44[紫芝](https://me.csdn.net/qq_40507857)阅读数：81








# 一、定义

# std::[string](http://www.cplusplus.com/reference/string/string/)::npos的定义：


```cpp
static const size_t npos = -1;
```
表示size_t的最大值（Maximum value for size_t），

如果对 -1表示size_t的最大值有疑问可以采用如下代码验证：





```cpp
#include <iostream>
#include <limits>
#include <string>
using namespace std;

int main()
{
    size_t npos = -1;
    cout << "npos: " << npos << endl;
    cout << "size_t max: " << numeric_limits<size_t>::max() << endl;
}
```

在我的PC上执行结果为：

                 npos:           4294967295

                 size_t max:  4294967295

可见他们是相等的，也就是说npos表示size_t的最大值

二、使用
2.1 如果作为一个返回值（return value）表示没有找到匹配项，例如：

```cpp
#include <iostream>  
#include <limits>  
#include <string>  
using namespace std;  
  
int main()  
{  
    string filename = "test";  
    cout << "filename : " << filename << endl;  
  
    size_t idx = filename.find('.');   //作为return value，表示没有匹配项  
    if(idx == string::npos)      
    {  
        cout << "filename does not contain any period!" << endl;  
    }  
}
```
2.2 但是string::npos作为string的成员函数的一个长度参数时，表示“直到字符串结束

（until the end of the string）”。例如：



```cpp
tmpname.replace(idx+1, string::npos, suffix);
```


这里的string::npos就是一个长度参数，表示直到字符串的结束，配合idx+1表示，string的剩余部分。

```cpp
#include <iostream>  
#include <limits>  
#include <string>  
using namespace std;  
  
int main()  
{  
    string filename = "test.cpp";  
    cout << "filename : " << filename << endl;  
  
    size_t idx = filename.find('.');   //as a return value  
    if(idx == string::npos)      
    {  
        cout << "filename does not contain any period!" << endl;  
    }  
    else  
    {  
        string tmpname = filename;  
        tmpname.replace(idx + 1, string::npos, "xxx"); //string::npos作为长度参数，表示直到字符串结束  
        cout << "repalce: " << tmpname << endl;  
    }  
}
```




执行结果为：

filename:test.cpp

replace: test.xxx








