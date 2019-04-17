# 贪心（二）NYOJ14题 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include <iostream>
#include<cmath>
#include "algorithm"
using namespace std;

struct Point
{
    int a;
    int b;
};
bool cmp(const Point &X, const Point &Y)
{
    return X.b  <  Y.b;
}

int main()
{
    int n;
    cin>>n;
    while(n--)
    {
        int m;
        cin>>m;
        Point point[m];
        for(int i=0;i<m;i++)
        {
            cin >> point[i].a >> point[i].b;
        }
        sort(point,point+m,cmp);
        int end = point[0].b;
        int sum = 0;
        for(int i=1;i<m;i++)
        {
            if(point[i].a > end)
            {
                end = point[i].b;
                sum++;
            }
        }
        cout << sum+1 << endl;
    }
    return 0;
}
//ac
```

思想就是优先选取开始时间晚的。

代码实现没有问题就是思路没有这么简单











