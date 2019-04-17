# nyoj——弃九法 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## A*B Problem

时间限制：1000 ms  |  内存限制：65535 KB

难度：2



- 描述

设计一个程序求出A*B，然后将其结果每一位相加得到C，如果C的位数大于等于2，继续将C的各位数相加，直到结果是个一位数k。

例如：

6*8=48；

4+8=12；

1+2=3；

输出3即可。


- 输入
第一行输入一个数N(0<N<=1000000)，表示N组测试数据。
随后的N行每行给出两个非负整数m，n（0<=m,n<=10^12)。- 输出
对于每一行数据，输出k。- 样例输入
3
6 8
1234567 67
454 1232- 样例输出
3
4
5
```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 2000;
const int moder = 10003;
ll pow_mod(ll x,ll p)
{
    ll ans = 1;
    while(p){
        if(p&1) ans = (ans*x)%moder;
        x = (x*x)%moder;
        p = p>>1;
    }
    return ans;
}


int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        ll a,b;
        scanf("%lld%lld",&a,&b);
        if(a == 0||b == 0){
            printf("0\n");
            continue;
        }
        int temp = (a%9*b%9)%9;
        if(temp ==0) temp = 9;
        printf("%d\n",temp);
    }
    return 0;
}
```

用到 1.同余定理： (a + b) % c == (a % c + b % c) % c;

         2.  k * 10^m % 9 == k % 9。

综上，设a == b *100 + c * 10 + d;  A == b + c + d，则a % 9 == (b*100 % 9 + c*10 % 9 + d % 9) % 9 == (b + c + d) % 9 % 9 == (b + c + d) % 9 == A % 9;。











