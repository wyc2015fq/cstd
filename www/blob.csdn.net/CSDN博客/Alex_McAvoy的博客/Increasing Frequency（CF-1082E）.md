# Increasing Frequency（CF-1082E） - Alex_McAvoy的博客 - CSDN博客





2018年12月05日 21:30:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：187
个人分类：[CodeForces																[动态规划——线性 DP](https://blog.csdn.net/u011815404/article/category/7813170)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

You are given array a of length n. You can choose one segment [l,r] (1≤l≤r≤n) and integer value k (positive, negative or even zero) and change al,al+1,…,ar by k each (i.e. ai:=ai+k for each l≤i≤r).

What is the maximum possible number of elements with value cc that can be obtained after one such operation?

# Input

The first line contains two integers n and c (1≤n≤5⋅105, 1≤c≤5⋅105) — the length of array and the value c to obtain.

The second line contains nn integers a1,a2,…,an (1≤ai≤5⋅105) — array a.

# Output

Print one integer — the maximum possible number of elements with value cc which can be obtained after performing operation described above.

# Examples

**Input**

6 9

9 9 9 9 9 9

**Output**

6

**Input**

3 2

6 2 6

**Output**

2

————————————————————————————————————————————

题意：给出一长度为 n 的数列和一个数 c，能将一段连续区间里的数都加上 k，使得整个序列中 c 尽可能的多，区间和 k 都由自己决定，求最多的个数

思路：学长说这个题是 DP，然后写到自闭。。。

一开始想的是枚举 1~L 与 R~n 的区间，计算将他们变为 c 的个数，当前面的 c 的数量和后面的 c 的都已确定，再统计将中间出现次数最多的数都变成 c 的个数，但是写到最后发现中间出现次数最多的数并不好计算。

![](https://img-blog.csdnimg.cn/20181205210700617.png)

于是，可以从 1 开始枚举到某处 i，再加上 i 之后的数列中 c 的个数

![](https://img-blog.csdnimg.cn/201812052112579.png)

用两个数组 up[i]、down[i] 分别存储数列中从前向后、从后向前到 i 为止的等于 c 的个数

用 dp[i] 表示从前面某位置 pos 开始到现在位置 i，将 pos~i 之间出现次数最多的数变成 c、前 pos-1 个数最大的含 c 的数量

![](https://img-blog.csdnimg.cn/20181205211738302.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

对于 pos 的位置，可以用一数组 pre[x]=j 存储，其表示对于某个数 a[i]==x，他上一次出现的位置是 j，即：a[j]=a[i]=x

![](https://img-blog.csdnimg.cn/20181205211959741.png)

从而有了状态转移方程：

dp[i]=max(up[i-1]+1,dp[pre[a[i]]]+1);

pre[a[i]]=i;

ans=max(ans,dp[i]+down[i+1]);

从而保证从某个位置 pos 开始，dp[pos+1]~dp[i] 这一段出现次数最多的数变成了 c 的最大的

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
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1000001
#define LL long long
using namespace std;
int a[N];
int up[N],down[N];
int dp[N];
int pre[N];
int main(){
    int n,c;
    cin>>n>>c;

    for(int i=1;i<=n;i++){
        cin>>a[i];
        up[i]=up[i-1]+(a[i]==c);
    }

    for(int i=n;i>=1;i--)
        down[i]=down[i+1]+(a[i]==c);

    int maxx=-INF;
    for(int i=1;i<=n;i++){
        dp[i]=max(up[i-1]+1,dp[pre[a[i]]]+1);
        pre[a[i]]=i;
        maxx=max(maxx,dp[i]+down[i+1]);
    }

    cout<<maxx<<endl;

    return 0;
}
```






