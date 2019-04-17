# 数论练习（4）——同余方程（扩gcd） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





CODEVS 1200 同余方程 
题目描述 Description 
求关于 x 同余方程 ax ≡ 1 (mod b)的最小正整数解。

输入描述 Input Description 
输入只有一行，包含两个正整数 a, b，用 一个 空格隔开。

输出描述 Output Description 
输出只有一行包含一个正整数x0，即最小正整数解，输入数据保证一定有解。 
【数据范围】 
对于 40% 的数据， 2 ≤b≤ 1,000 ； 
对于 60% 的数据， 2 ≤b≤ 50,000,000 
对于 100% 的数据， 2 ≤a, b≤ 2,000,000,000

```
#include<bits/stdc++.h>
using namespace std;
typedef long long  ll;

const int inf = 0x3f3f3f3f;
const int maxn = 40000 + 20;
const int moder = 1e9 + 7;
const int K = 256;

ll a,b,x,y;
void extendgcd(ll a,ll b,ll &x,ll &y )
{
    if(b == 0)
    {
        x = 1;
        y = 0;
    }
    else
    {
        extendgcd(b,a%b,y,x);
        y = y - (x * (a / b));
    }

    return;
}

int main()
{
    cin >> a >> b;
    extendgcd(a,b,x,y);
    cout << (x%b+b)%b << endl;
    return 0;
}
```

ax≡1（modb）等价于ax+bk=1（k为常数）

求出 x 的最小整数解，取模取正数。

1.题目保证有解，根据不定方程 ax+by=c 有解的条件: c mod gcd(a,b) = 0，我们可以得出gcd(a, b) ＝ 1 。

2.由于c＝1， 所以在扩展欧几里得算法后不需要再乘c/gcd(a,b). 直接设个将x调整到正数。

![](https://images2017.cnblogs.com/blog/1196023/201802/1196023-20180203220812750-135120809.png)













