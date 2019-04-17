# Travel - westbrook1998的博客 - CSDN博客





2018年10月03日 18:22:50[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：86标签：[找规律](https://so.csdn.net/so/search/s.do?q=找规律&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
链接：[https://www.nowcoder.com/acm/contest/203/H](https://www.nowcoder.com/acm/contest/203/H)

来源：牛客网

题目描述

魔方国有n座城市，编号为。城市之间通过n-1条无向道路连接，形成一个树形结构。

澜澜打算在魔方国进行m次旅游，每次游览至少一座城市。为了方便，每次旅游游览的城市必须是连通的。此外，澜澜希望游览所有城市恰好一次。

澜澜想知道有多少种旅游方案满足条件，两个方案不同当且仅当存在某一次旅游游览了不同的城市。

澜澜不会数数，所以只好让你来帮他数方案。

输入描述:

第一行一个整数t表示数据组数 (1 ≤ t ≤ 100)。

每组数据第一行两个整数n,m ，接下来n-1行每行两个整数ai,bi表示一条道路 (1≤ ai,bi≤ n)。

输出描述:

每组数据输出一行一个整数表示方案数对109+7取模的结果。

示例1

输入

复制

2

3 1

1 2

1 3

3 2

1 2

1 3

输出

复制

1

4
这题算是比较有成就感的一题，差不多花了一个小时，整理一下思路

一开始我就是直接直接看，然后手算出发现同个图不同的构造的在m不等于1的情况下方案数是一样的，然后队友过来帮忙组合数推了一下发现n等于3和4的情况确实如此，虽然和我手算的不一样，然后就推了一下5，就发现确实可以用组合数来算，比如n个点的情况，m次旅游的方案就是C的n-1取m-1乘以A的m取m，为什么这样呢，其实就是把n个点看成一条链，然后用插空法，然后分成的m份再全排列，然后约分一下就可以得到最后的公式了，边乘边取模，然后注意特判1

代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
typedef long long ll;
const int N=1e5+5;
const int MOD=1e9+7;
int t;
int n,m;
int a,b;
int deg[N*2];
int main(void){
    scanf("%d",&t);
    while(t--){
        int maxD=0;
        memset(deg,0,sizeof(deg));
        scanf("%d%d",&n,&m);
        for(int i=0;i<n-1;i++){
            scanf("%d%d",&a,&b);
            deg[a]++;
            deg[b]++;
            maxD=max(maxD,max(deg[a],deg[b]));
        }
        //非链
        if(maxD>2 && m==1){
            printf("0\n");
        }
        else{
            ll ans=m;
            for(ll i=n-1;i>=n-m+1;i--){
                ans=(ans*i)%MOD;
            }
            printf("%lld\n",ans);
        }
    }
}
```





