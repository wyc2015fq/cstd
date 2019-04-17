# codeforce——因数筛 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





题目大意：给你一个 n 和 k 求 n 的第 k 个因数。



```
#include<iostream>
#include <algorithm>
#include <queue>
#include <string>
using namespace std;
typedef long long  ll;
const int inf = 0x3f3f3f3f;
const int moder = 1e9 + 7;
const int MAXN=510;

int main()
{
    ll n,k;
    cin >> n >> k;
    vector<ll>v;
    for(ll i=1;i <= sqrt(n);i++)
    {
        if(n%i == 0) v.push_back(i);
        if(n%i == 0&&i*i != n) v.push_back(n/i);
    }
    sort(v.begin(),v.end());
    if(k >= v.size()) puts("-1");
    else cout << v[k-1] << endl;
    return 0;
}
```

1.用了vector

2.i 如果是 n 的因数，n / i 也一定是 n 的因数











