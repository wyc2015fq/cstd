# Sequence（BZOJ-1345） - Alex_McAvoy的博客 - CSDN博客





2019年02月11日 20:02:09[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：59
个人分类：[基础算法——贪心																[其它 OJ](https://blog.csdn.net/u011815404/article/category/8820652)](https://blog.csdn.net/u011815404/article/category/7628972)








> 
# Problem Description

对于一个给定的序列a1,…,an，我们对它进行一个操作reduce(i)，该操作将数列中的元素ai和ai+1用一个元素max

(ai，ai+1)替代，这样得到一个比原来序列短的新序列。这一操作的代价是max(ai，ai+1)。进行n-1次该操作后，

可以得到一个长度为1的序列。我们的任务是计算代价最小的reduce操作步骤，将给定的序列变成长度为1的序列。

# **Input**

第一行为一个整数n( 1 <= n <= 1,000,000 )，表示给定序列的长度。

接下来的n行，每行一个整数ai（0 <=ai<= 1, 000, 000, 000），为序列中的元素。

# Output

只有一行，为一个整数，即将序列变成一个元素的最小代价。

# Sample Input

**3123**

# Sample Output

**5**


思路：贪心

对于 n 个数，每次将相邻的两个数，取他们的最大，依次类推，最后得到的一定是最小值

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<deque>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 2000001
#define LL long long
const int MOD=998244353;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
LL a[N];
int main(){
    int n;
    scanf("%d",&n);
    for(int i=1;i<=n;i++)
        scanf("%d",&a[i]);

    LL res=0;
    for(int i=1;i<=n-1;i++)
        res+=max(a[i],a[i+1]);

    printf("%lld\n",res);

    return 0;
}
```






