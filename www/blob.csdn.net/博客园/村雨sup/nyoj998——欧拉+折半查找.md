# nyoj998——欧拉+折半查找 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## Sum

时间限制：1000 ms  |  内存限制：65535 KB

难度：3



- 描述

            给你一个数N,使得在1~N之间能够找到x使得x满足gcd（ x ,  N  ） >= M，

求解gcd(x,N)的和


- 输入
多组测试数据

每行输出两个数N，M（N,M不超int）- 输出
输出sum- 样例输入
5 3- 样例输出
5- 上传者
[ACM_张书军](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=ACM_%E5%BC%A0%E4%B9%A6%E5%86%9B)
```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 2000;
const int moder = 10003;

ll eular(ll n)
{
    ll ans = n;
    for(int i=2;i*i <= n;i++){
        if(n%i == 0){
            ans = ans - ans/i;
            while(n%i == 0)
                n = n/i;
        }
    }
    if(n > 1)
        ans = ans - ans/n;
    return ans;
}
int main()
{
    ll n,m;
    while(scanf("%lld%lld",&n,&m)!=EOF){
        ll res = 0;
        for(int i=1;i*i <= n;i++) {
            if (n % i == 0) {
                if (i >= m) {
                    res += i * eular(n / i);
                }
                if (i * i != n && n / i >= m){
                    res += n/i*eular(i);
                }
            }
        }
        printf("%lld\n",res);
    }
    return 0;
}
```

————很巧妙的运用了欧拉函数

![](https://images2018.cnblogs.com/blog/1196023/201803/1196023-20180301195248616-452028028.png)











