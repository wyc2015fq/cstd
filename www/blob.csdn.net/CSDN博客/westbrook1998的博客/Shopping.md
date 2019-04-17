# Shopping - westbrook1998的博客 - CSDN博客





2018年10月03日 17:33:39[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：56








> 
链接：[https://www.nowcoder.com/acm/contest/203/D](https://www.nowcoder.com/acm/contest/203/D)

来源：牛客网

题目描述

你要买n件物品，其中有一些是凳子。

商场正在举行促销活动，如果购物车中有至少一个凳子，那么你可以半价购买这个购物车中最贵的一个物品。

你有m辆购物车，请最小化你的花费。

输入描述:

第一行一个整数t表示数据组数 (1 ≤ t ≤ 100)。

每组数据第一行两个整数n,m (1 ≤ n,m ≤ 1000)，接下来n行每行两个整数ai,bi，分别表示第i件物品的价格以及它是否是凳子 (1 ≤ ai ≤ 105, 0 ≤ bi ≤ 1)。

输出描述:

每组数据输出一行一个实数表示最小花费，保留一位小数。

示例1

输入

复制

2

5 1

1 0

2 1

3 1

4 0

5 0

5 10

1 0

2 1

3 1

4 0

5 0

输出

复制

12.5

10.5
又是两题签到，然后全场自闭…

这题才是真正的签到题，一开始跟了a题后来发现不对感觉看这个，这题无需考虑太复杂，就把板凳作为一个打五折的机会，然后和购物车数量取个小个，然后所有商品从大到小排序，前k个大的就打五折

代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int t;
int n,m;
const int N=1005;
int a[N],b[N];
bool cmp(int a,int b){
    return a>b;
}
int main(void){
    scanf("%d",&t);
    while(t--){
        int cnt=0;
        memset(a,0,sizeof(a));
        memset(b,0,sizeof(b));
        scanf("%d%d",&n,&m);
        for(int i=0;i<n;i++){
            scanf("%d%d",&a[i],&b[i]);
            if(b[i]==1){
                cnt++;
            }
        }
        sort(a,a+n,cmp);
        cnt=min(cnt,m);
        double ans=0;
        for(int i=0;i<cnt;i++){
            ans+=a[i]*1.0/2;
        }
        for(int i=cnt;i<n;i++){
            ans+=a[i];
        }
        printf("%.1lf\n",ans);
    }
    return 0;
}
```





