# 牛客网——E求最值 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





链接：[https://www.nowcoder.com/acm/contest/59/E](https://www.nowcoder.com/acm/contest/59/E)
来源：牛客网



## 题目描述



给你一个长为n的序列a

定义f(i,j)=(i-j)2+g(i,j)2

g是这样的一个函数

![](https://uploadfiles.nowcoder.net/images/20180125/301599_1516886357781_321532365639F31B3B9F8EA8BE0C6BE2)


  求最小的f(i,j)的值，i!=j






## 输入描述:
第一行一个数n
之后一行n个数表示序列a
## 输出描述:
输出一行一个数表示答案

示例1



## 输入

4
1 0 0 -1



## 输出

1




## 备注:

对于100%的数据，2 <= n <= 100000 , |ai| <= 10000

```
#include<bits/stdc++.h>
using namespace std;
#define INF 0x3f3f3f3f
using ll = long long;
const int qq = 100005;
ll a[qq];


inline ll sqr(ll x){ return x*x;}

int main()
{
    int n;
    cin >> n;
    for(int i=1;i <= n;i++)
    {
        scanf("%lld",a + i);
        a[i] += a[i-1];
    }


    ll minans = INF;
    for(int i=1;i <= n;i++)
    {
        int k=1;
        for(int j=i-1;j >= 1&&k<=10;j--,k++)
        {
            ll ans = sqr(i-j) + sqr(a[i]-a[j]);
            if(ans < minans)
                minans = ans;
        }
    }
    cout << minans << endl;
    return 0;
}
```

1.第一处是计算从L到R区间的和用 (a[R] - a[L]) 其中 a[ i ] 是从开头加到第 i 个位置，在多组数据的时候可以避免重复计算。

2.第二个就是因为求得是最小值所以 (i - j)一定不会很大

3.牛客网可以看别人代码，蛮好的，要多学习一点大神们的代码方式。












