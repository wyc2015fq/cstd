# fib博弈 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





链接：[https://www.nowcoder.com/acm/contest/77/G](https://www.nowcoder.com/acm/contest/77/G)
来源：牛客网



 幼儿园开学了，为了让小盆友们能尽可能的多的享受假期。校长大人决定让小盆友分批到校，至于每批学生来多少人由一个小傻子和一个小仙女负责，两个人轮番负责，校长会在最后的时候去查看工作进度，小傻子不想被别人嘲笑自己傻，小仙女要证明自己比小傻子聪明。所以她们回去争抢安排最后一名小盆友。每次安排的小盆友至少为1，至多为上一次安排的2倍。小仙女抢到了先手的机会。第一次安排小盆友不能直接安排所有的小盆友一起回校。 

## 输入描述:
单组测试数据
输入一个整数n——n代表小盆的个数（n>=2&&n<=1e9)
## 输出描述:
输出获胜人的名字——“Xian”或者“Sha”



示例1



## 输入

3



## 输出

Sha



## 说明

（Fisrt）1 -> (Second)  2         ||     2 - > 1  无论小仙女先送一个还是两个都会被小傻子获胜





示例2



## 输入

4



## 输出

Xian



## 说明

1 -> 2 -> 1    ||    1 -> 1 -> 2        小仙女先送一个，小傻子无论送一个或者两个都会被小仙女取胜。
```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int moder = 10000;
const int maxn = 2000000;

int a[100005];

int main()
{
    int n;
    a[0] = 1;
    a[1] = 2;
    for(int i=2;i <= 50;i++)
    {
        a[i] = a[i-1] + a[i-2];
    }
    scanf("%d",&n);
    int flag = 0;
    for(int i=1;i < 50;i++){
        if(a[i]==n) {flag = 1;break;}
    }
    if(flag) printf("Sha");
    else printf("Xian");
    return 0;
}
```

——fib数列















