# 字符串练习——唐纳德与子串 (Easy) - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





G1. 唐纳德与子串 (Easy)



Time limit per test: 1.0 seconds

Memory limit: 256 megabytes





子串的定义是在一个字符串中连续出现的一段字符。这里，我们使用 

对于一个给定的字符串 

![](http://acm.ecnu.edu.cn/upload/3441/286a9800f7c352c91e99824e5d22f710.WxpgjtYkDg53BuPg.jpg)


### Input

输入具有如下形式：



第一行一个字符串 

第二行一个整数 

接下来每行：首先两个整数 

字符串中只会出现 

数据规模约定：
- 对于 Easy 档：
- 对于 Hard 档：

### Output

对于每次询问，输出一个整数，表示答案。


### Examples



input
thisisagarbagecompetitionhahaha
5
0 30 a
1 5 is
25 30 hah
6 12 ag
7 12 ag



output
6
2
2
2
1
```
#include "bits/stdc++.h"
using namespace std;
int main()
{
    string s;
    cin >> s;
    int q;
    cin >> q;
    while(q--)
    {
        int l,r;
        string pattern;
        cin >> l >> r >> pattern;
        string s1 = s.substr(l,r-l+1);
        int len = pattern.length();
        int positon = 0;
        int count = 0;
        while(s1 != "")
        {
//                int s1len = s1.length();
            if(s1.find(pattern)>100000)
                break;
            positon = s1.find(pattern);
            s1 = s1.substr(positon+1);
            count++;
        }
        cout << count << endl;
    }


    return 0;
}
```

又是字符串匹配问题，踩坑无数啊！！！

1：s.find()  如果没找到会返回一个巨大的数，如果你想用这个来判断是否存在子串，不能用一个变量来储存，而是应该直接在if中判断。

2：s.substr(start,len)  substr 参数第一个是起始位置，第二个是子串长度（不写默认直到最后）。















