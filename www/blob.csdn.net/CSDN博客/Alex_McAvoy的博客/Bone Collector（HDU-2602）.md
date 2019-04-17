# Bone Collector（HDU-2602） - Alex_McAvoy的博客 - CSDN博客





2018年03月30日 23:10:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51
个人分类：[动态规划——背包问题																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813163)








# Problem Description

    Many years ago , in Teddy’s hometown there was a man who was called “Bone Collector”. This man like to collect varies of bones , such as dog’s , cow’s , also he went to the grave …

    The bone collector had a big bag with a volume of V ,and along his trip of collecting there are a lot of bones , obviously , different bone has different value and different volume, now given the each bone’s value along his trip , can you calculate out the maximum of the total value the bone collector can get ?

# Input

    The first line contain a integer T , the number of cases.

    Followed by T cases , each case three lines , the first line contain two integer N , V, (N <= 1000 , V <= 1000 )representing the number of bones and the volume of his bag. And the second line contain N integers representing the value of each bone. The third line contain N integers representing the volume of each bone.

# Output

    One integer per line representing the maximum of the total value (this number will be less than 2^31).

# Sample Input

**15 101 2 3 4 55 4 3 2 1**

# Sample Output

**14**

————————————————————————————————————————————

思路：0-1背包经典裸题

# Source Program

```cpp
#include<iostream>
#include<cstring>
using namespace std;
int dp[1001];
int main()
{
    int t;
    int n,v;
    int c[1001],w[1001];
    int i,j;

    cin>>t;
    while(t--)
    {
        memset(dp,0,sizeof(dp));
        cin>>n>>v;
        for(i=1;i<=n;i++)
            cin>>w[i];
        for(i=1;i<=n;i++)
            cin>>c[i];

        for(i=1;i<=n;i++)
            for(j=v;j>=c[i];j--)
                dp[j]=max(dp[j],dp[j-c[i]]+w[i]);

        cout<<dp[v]<<endl;
    }
    return 0;
}
```




