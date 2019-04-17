# 高精度减法用string 和 stack - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include "bits/stdc++.h"
using namespace std;

int main()
{
    string a,b;
    while(cin >> a >> b)
    {
        if(a.size() < b.size() || (a.size() == b.size() && a < b))
        {
            cout << "-";
            swap(a,b);
        }

        int lena = a.size();
        int lenb = b.size();

        for(int i = 0;i < lena - lenb;i++)
            b = '0' + b;

        int jiewei = 0;
        stack<int>s;
        for(int i = lena-1; i >=0;i--)
        {
            int v = a[i] - b[i] + jiewei;
            s.push(v >= 0 ? v:v+10);
            jiewei = (v >= 0 ? 0:-1);
        }
        while(s.top() == 0)
        {
            s.pop();
        }
        while(!s.empty())
        {
            cout << s.top();
            s.pop();
        }

    }
    return 0;
}
```

我觉得多用用这些数据结构也是蛮好的；











