# Elections（CF-1043A） - Alex_McAvoy的博客 - CSDN博客





2018年11月07日 23:07:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51
个人分类：[CodeForces																[基础算法——模拟](https://blog.csdn.net/u011815404/article/category/7812141)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Awruk is taking part in elections in his school. It is the final round. He has only one opponent — Elodreip. The are nn students in the school. Each student has exactly k votes and is obligated to use all of them. So Awruk knows that if a person gives aiai votes for Elodreip, than he will get exactly k−ai votes from this person. Of course 0≤k−ai holds.

Awruk knows that if he loses his life is over. He has been speaking a lot with his friends and now he knows a1,a2,…,an — how many votes for Elodreip each student wants to give. Now he wants to change the number kk to win the elections. Of course he knows that bigger kk means bigger chance that somebody may notice that he has changed something and then he will be disqualified.

So, Awruk knows a1,a2,…,an — how many votes each student will give to his opponent. Help him select the smallest winning number k. In order to win, Awruk needs to get strictly more votes than Elodreip.

# Input

The first line contains integer n (1≤n≤100) — the number of students in the school.

The second line contains nn integers a1,a2,…,an (1≤ai≤100) — the number of votes each student gives to Elodreip.

# Output

Output the smallest integer kk (k≥maxaik≥maxai) which gives Awruk the victory. In order to win, Awruk needs to get strictly more votes than Elodreip.

# Examples

**Input**

5

1 1 1 5 1

**Output**

5

**Input**

5

2 2 3 2 2

**Output**

3

————————————————————————————————————————————

题意：两人竞选，n 个人可投票，每个人最多投 k 票，先在已知每个人一定会投 ai 票给 A，现在 B 可以改变每个人投票数 k，求 k 最小为多少时，B 可以获胜

思路：先判断当前情况下 B 是否可以获胜，若可以，则直接输出当前投票下的最大票数即可；若当前情况下 B 不可以获胜，则统计当前 A 获得的票数，然后 +1，就是 B 获胜的最小票数，将两者得票相加，若可以整除 n，所得结果即为最小 k 值，若不可以整除，所得结果取整后+1即可

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
#define N 1001
#define LL long long
using namespace std;
int a[N];
int main()
{
    int n;
    cin>>n;

    int sum1=0;
    int maxx=-INF;
    for(int i=0;i<n;i++){
        cin>>a[i];
        sum1+=a[i];
        maxx=max(maxx,a[i]);
    }

    int sum2=maxx*n-sum1;
    if(sum2>sum1){
        cout<<maxx<<endl;
        return 0;
    }


    int res;
    sum2=sum1+1;
    if((sum1+sum2)%n)
        res=(sum1+sum2)/n+1;
    else
        res=(sum1+sum2)/n;
    cout<<res<<endl;

    return 0;
}
```






