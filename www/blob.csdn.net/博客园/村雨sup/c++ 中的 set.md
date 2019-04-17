# c++ 中的 set - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





set (集合) 中的元素是排序好的，而且是不重复的。

例题：hdu 4989

题目大意：求一组数列中任意两个不重复元素和，再求不重复和的和。

```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int moder = 1e9 + 7;
const int maxn = 1550;

int main()
{
    ll sum = 0;
    int n;
    set<int>s;
    while(~scanf("%d",&n))
    {
        sum = 0;
        int a[n];
        s.clear();
        for(int i=0;i < n;i++)
        {
            scanf("%d",&a[i]);
        }

        for(int i=0;i < n;i++)
        {
            for(int j=i+1;j < n;j++)
            {
                s.insert(a[i]+a[j]);
            }
        }

        set<int>::iterator it;   //利用迭代器访问，使用方法和指针差不多。
        for(it=s.begin();it != s.end();it++)
        {
            sum += *it;
        }
        printf("%lld\n",sum);

    }
    return 0;
}
```

——











