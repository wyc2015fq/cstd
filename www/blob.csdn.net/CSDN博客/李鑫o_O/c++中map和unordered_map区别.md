# c++中map和unordered_map区别 - 李鑫o_O - CSDN博客





2016年03月24日 10:02:00[hustlx](https://me.csdn.net/HUSTLX)阅读数：1446








简单的说，map做插入后是有序的，按照关键字key排序，unordered_map是无序的， 顺序是插入的时候的顺序，但是unordered_map执行效率要比map高很多，所以推荐用unordered_map，下面是测试代码：



```cpp
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
using namespace std;

int main()
{
    unordered_map<string, int> m;
    m.insert(make_pair("test5", 20));
    m.insert(make_pair("test2", 25));
    m.insert(make_pair("test1", 22));
    m.insert(make_pair("test3", 22));
    m.insert(make_pair("test4", 23));
    for (auto iter = m.begin(); iter != m.end(); iter++)
    {
        cout << iter->first<< "\t" << iter->second << endl;
    }
    return 0;
}
```


上面是unordered_map输出：

![](https://img-blog.csdn.net/20160324100618132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





map：



```cpp
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
using namespace std;

int main()
{
    map<string, int> m;
    m.insert(make_pair("test5", 20));
    m.insert(make_pair("test2", 25));
    m.insert(make_pair("test1", 22));
    m.insert(make_pair("test3", 22));
    m.insert(make_pair("test4", 23));
    for (auto iter = m.begin(); iter != m.end(); iter++)
    {
        cout << iter->first<< "\t" << iter->second << endl;
    }
    return 0;
}
```


输出：

![](https://img-blog.csdn.net/20160324100845753?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




