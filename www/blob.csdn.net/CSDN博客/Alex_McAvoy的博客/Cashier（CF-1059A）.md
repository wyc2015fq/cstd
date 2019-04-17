# Cashier（CF-1059A） - Alex_McAvoy的博客 - CSDN博客





2018年10月17日 18:34:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：68








# Problem Description

Vasya has recently got a job as a cashier at a local store. His day at work is Lminutes long. Vasya has already memorized nn regular customers, the i-th of which comes after ti minutes after the beginning of the day, and his service consumes liliminutes. It is guaranteed that no customer will arrive while Vasya is servicing another customer.

Vasya is a bit lazy, so he likes taking smoke breaks for aa minutes each. Those breaks may go one after another, but Vasya must be present at work during all the time periods he must serve regular customers, otherwise one of them may alert his boss. What is the maximum number of breaks Vasya can take during the day?

# Input

The first line contains three integers nn, LL and aa (0≤n≤105, 1≤L≤109, 1≤a≤L).

The ii-th of the next nn lines contains two integers titi and lili (0≤ti≤L−1, 1≤li≤L). It is guaranteed that ti+li≤ti+1 and tn+ln≤L.

# Output

Output one integer  — the maximum number of breaks.

# Examples

**Input**

2 11 3

0 1

1 1

**Output**

3

**Input**

0 5 2

**Output**

2

**Input**

1 3 2

1 2

**Output**

0

————————————————————————————————————————————

题意：一天的工作长度为 L，每次休息时间为 a，现有 n 个客人，给出每个客人的到来时间与占用时间，求最多能休息的次数

思路：对于每个客人的实际，算出它与上一个时间点的差距，然后除以每次休息的时间，就是能休息的最大值，注意最后一段时间也要算入休息时间中

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 100001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
int x[N],y[N];
int main()
{
    int n,l,a;
    while(scanf("%d%d%d",&n,&l,&a)!=EOF){
        if(n==0){
            printf("%d",l/a);
            continue;
        }


        int ans=0;
        for(int i=0;i<n;i++){
            scanf("%d%d",&x[i],&y[i]);
            if(n>0)
                ans+=((x[i]-(x[i-1]+y[i-1]))/a);
        }
        ans+=((l-(x[n-1]+y[n-1]))/a);
        printf("%d\n",ans);
    }
	return 0;
}
```





