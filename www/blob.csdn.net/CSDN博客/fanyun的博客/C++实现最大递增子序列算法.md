# C++实现最大递增子序列算法 - fanyun的博客 - CSDN博客
2018年08月26日 14:28:59[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：138
1、算法要求
       奶牛为自己规划下面n时间内的产奶，m个时间段，每个段有a，b，c表示从a时到b时共可产奶c。 挤奶工每次挤奶必须挤完完整的时间段，且每次挤完需要休息r时，求最终可获得的牛奶最大值 ps. 上面的n简直了，不仔细看题很容易误解，差点让我简单问题复杂化，所有的时间段的时间都不会超出1～n的范围，所以可以忽略这个数值。
2、思路
     时间为判断条件，且必然由小到大。以开始时间为条件对时间段进行排序，求最大递增子序列。
3、代码实现
```cpp
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;
const int N = 1009;
int dp[N];
struct Node
{
    int l, r, v;
    bool operator <(const Node &t) const
    {
        return l < t.l;
    }
}a[N];
int main()
{
    int n, m, r;
    scanf("%d%d%d", &n, &m, &r);
    for(int i=0; i<m; i++)
        scanf("%d%d%d", &a[i].l ,&a[i].r, &a[i].v);
    sort(a, a+m);
    int ans = 0;
    for(int i=0; i<m; i++)
    {
        int mmax = 0;
        for(int j=0; j<i; j++)
            if(a[j].r+r<=a[i].l && mmax<dp[j])
                mmax = dp[j];
        dp[i] = mmax+a[i].v;
        ans = max(ans, dp[i]);
    }
    cout<<ans<<endl;
    return 0;
}
```
