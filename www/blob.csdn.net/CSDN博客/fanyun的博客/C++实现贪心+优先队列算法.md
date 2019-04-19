# C++实现贪心+优先队列算法 - fanyun的博客 - CSDN博客
2018年08月26日 14:29:49[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：89
1、实现贪心+优先队列算法要求（消灭兔子）
       n个兔子，每个兔子都有一个血量b[i] ；m种箭(每种各一支)，每种箭都有伤害值d[i]和价格p[i] ；每个兔子只能被射一次，伤害值大于血量则死，每种箭只能用一次 ；问杀死所有兔子需要的最小价格为多少，若不能杀死，则No Solution m,n小于50000)。
2、实现思路
      典型的贪心，每个兔子只能射一次，所以只能用伤害值大于其血量的箭，在此前提下，箭越便宜越好，故对兔子血量升序排列，箭对伤害值升序排列。 
      若i小于j 则第i支箭可以杀死的兔子，第j支箭也一定能杀死，而若j的价格小于i，就应该用j，所以用优先数列维护即可。 
3、代码实现
```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<queue>
#include<functional>
#include<vector>
using namespace std;
const int N = 50009;
pair<int, int> p[N];
int b[N];
int main()
{
    int n,m;
    scanf("%d%d",&n,&m);
    if(m < n)
    {
        printf("No Solution\n");
        return 0;
    }
    for(int i=0;i<n;i++)
        scanf("%d",&b[i]);
    for(int i=0;i<m;i++)
        scanf("%d%d",&p[i].first, &p[i].second);
    sort(b, b+n);
    sort(p, p+m);
    priority_queue<int> q;
    int i = 0, j = 0;
    long long ans = 0;
    while(j < m)
    {
        ans += p[j].second;
        q.push(p[j].second);
        if(p[j].first >= b[i] && i < n)
        {
            ++i;++j;
        }
        else
        {
            ans -= q.top();
            q.pop();
            ++j;
        }
    }
    if(i < n)
        printf("No Solution\n");
    else
        printf("%lld\n",ans);
    return 0;
}
```
