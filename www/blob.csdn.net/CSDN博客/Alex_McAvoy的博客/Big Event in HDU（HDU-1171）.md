# Big Event in HDU（HDU-1171） - Alex_McAvoy的博客 - CSDN博客





2018年03月30日 23:15:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：70








# Problem Description

    Nowadays, we all know that Computer College is the biggest department in HDU. But, maybe you don't know that Computer College had ever been split into Computer College and Software College in 2002.

    The splitting is absolutely a big event in HDU! At the same time, it is a trouble thing too. All facilities must go halves. First, all facilities are assessed, and two facilities are thought to be same if they have the same value. It is assumed that there is N (0<N<1000) kinds of facilities (different value, different kinds).

# Input

    Input contains multiple test cases. Each test case starts with a number N (0 < N <= 50 -- the total number of different facilities). The next N lines contain an integer V (0<V<=50 --value of facility) and an integer M (0<M<=100 --corresponding number of the facilities) each. You can assume that all V are different.

    A test case starting with a negative integer terminates input and this test case is not to be processed.

# Output

    For each case, print one line containing two integers A and B which denote the value of Computer College and Software College will get respectively. A and B should be as equal as possible. At the same time, you should guarantee that A is not less than B.

# Sample Input

**210 120 1310 1 20 230 1-1**

# Sample Output

**20 1040 40**

————————————————————————————————————————————

思路：统计总和，将总和平分后，就是0-1背包问题了

# Source Program

```cpp
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
int dp[250001];
int main()
{
    int n;
    int v[51],num[51];
    int sum;
    int i,j,k;

    while(cin>>n && n>0)
    {
        /*初始化*/
        sum=0;
        memset(dp,0,sizeof(dp));

        for(i=1;i<=n;i++)
        {
            cin>>v[i]>>num[i];//输入价值与数量
            sum+=v[i]*num[i];//统计总价值

        }

        for(i=1;i<=n;i++)//物品价值
            for(j=1;j<=num[i];j++)//每个物品数量
                for(k=sum/2;k>=v[i];k-=v[i])//把价值看作体积，从中间开始
                    dp[k]=max(dp[k],dp[k-v[i]]+v[i]);
        cout<<sum-dp[sum/2]<<" "<<dp[sum/2]<<endl;
    }
    return 0;
}
```



