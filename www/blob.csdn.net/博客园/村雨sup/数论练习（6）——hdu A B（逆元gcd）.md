# 数论练习（6）——hdu A/B（逆元gcd） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# A/B

**Time Limit: 1000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 7238    Accepted Submission(s): 5754**



Problem Description

要求(A/B)%9973，但由于A很大，我们只给出n(n=A%9973)(我们给定的A必能被B整除，且gcd(B,9973) = 1)。





Input

数据的第一行是一个T，表示有T组数据。
每组数据有两个数n(0 <= n < 9973)和B(1 <= B <= 10^9)。

Output

对应每组数据输出(A/B)%9973。

Sample Input


2
1000 53
87 123456789


 Sample Output


7922
6060



```
#include<bits/stdc++.h>
using namespace std;
#define ull unsigned long long
typedef long long  ll;

const int inf = 0x3f3f3f3f;
const int moder = 1e9 + 7;
const int MAXN=1000010;

ll extgcd(ll a,ll b,ll &x,ll &y)
{
    ll d = a;
    if(b == 0)
    {
        x = 1;
        y = 0;
    }
    else
    {
        d = extgcd(b,a%b,y,x);
        y -= (a/b)*x;
    }
    return d;
}

int main()
{
    int t;
    cin >> t;
    while(t--)
    {
        ll n,a,b,x,y;
        b = 9973;
        cin >> n >> a;
        extgcd(a,b,x,y);
        b = (x%b+b)%b;
        cout << n*b%9973 << endl;
    }

    return 0;
}
```




![](https://images2017.cnblogs.com/blog/1196023/201802/1196023-20180203225047562-2066916215.png)
ax _= 1(mod b)  

x为a膜b的逆元














