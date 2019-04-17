# 贪心（一）NYOJ题目12 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include <iostream>
#include<cmath>
#include "algorithm"
using namespace std;
bool cmp(const double a, const double b)
{
    return a>b;
}

int main()
{
    int n;
    cin>>n;
    while(n--)
    {
        int m;
        cin>>m;
        double a[m],s = 0;
        int sum = 0;
        for(int i=0;i<m;i++)
            cin>>a[i];
        sort(a,a+m,cmp);

        for(int i=0;s < 20;i++)
        {
            s = s + 2*sqrt(a[i]*a[i]-1);
            sum++;
        }
        cout << sum <<endl;

    }
    return 0;
}
//ac
```













