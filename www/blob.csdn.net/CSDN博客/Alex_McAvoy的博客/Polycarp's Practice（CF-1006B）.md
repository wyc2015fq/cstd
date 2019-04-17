# Polycarp's Practice（CF-1006B） - Alex_McAvoy的博客 - CSDN博客





2018年08月06日 08:58:47[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：71
个人分类：[基础算法——贪心																[CodeForces](https://blog.csdn.net/u011815404/article/category/7923180)](https://blog.csdn.net/u011815404/article/category/7628972)








# Problem Description

Polycarp is practicing his problem solving skill. He has a list of nn problems with difficulties a1,a2,…,an, respectively. His plan is to practice for exactly kk days. Each day he has to solve at least one problem from his list. Polycarp solves the problems in the order they are given in his list, he cannot skip any problem from his list. He has to solve all nn problems in exactly k days.

Thus, each day Polycarp solves a contiguous sequence of (consecutive) problems from the start of the list. He can't skip problems or solve them multiple times. As a result, in k days he will solve all the n problems.

The profit of the j-th day of Polycarp's practice is the maximum among all the difficulties of problems Polycarp solves during the j-th day (i.e. if he solves problems with indices from l to r during a day, then the profit of the day is max ai,l≤i≤r). The total profit of his practice is the sum of the profits over all k days of his practice.

You want to help Polycarp to get the maximum possible total profit over all valid ways to solve problems. Your task is to distribute all nn problems between kk days satisfying the conditions above in such a way, that the total profit is maximum.

For example, if n=8,k=3 and a=[5,4,2,6,5,1,9,2], one of the possible distributions with maximum total profit is: [5,4,2],[6,5],[1,9,2]. Here the total profit equals 5+6+9=20.

# Input

The first line of the input contains two integers nn and k (1≤k≤n≤2000) — the number of problems and the number of days, respectively.

The second line of the input contains nn integers a1,a2,…,an (1≤ai≤2000) — difficulties of problems in Polycarp's list, in the order they are placed in the list (i.e. in the order Polycarp will solve them).

# Output

In the first line of the output print the maximum possible total profit.

In the second line print exactly kk positive integers t1,t2,…,tk(t1+t2+⋯+tk must equal nn), where tj means the number of problems Polycarp will solve during the j-th day in order to achieve the maximum possible total profit of his practice.

If there are many possible answers, you may print any of them.

# Examples

**Input**

8 3

5 4 2 6 5 1 9 2

**Output**

20

3 3 2

**Input**

5 1

1 1 1 1 1

**Output**

1

5

**Input**

4 2

1 2000 2000 2

**Output**

4000

2 2

# Note

The first example is described in the problem statement.

In the second example there is only one possible distribution.

In the third example the best answer is to distribute problems in the following way: [1,2000],[2000,2]. The total profit of this distribution is 2000+2000=4000.

————————————————————————————————————————————

题意：输入 n 个数，要找前 k 大的，并划分k区间

思路：贪心，用结构体，先按照大小排序，找出前 k 个最大的，再将这前 k 大的位置排序，然后大减小即可。

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
#define MOD 123
#define E 1e-6
using namespace std;
struct Node{
    int pos;
    int val;
}a[N];
int pos[N];
int cmp(Node a,Node b)
{
    return a.val>b.val;
}
int main()
{
    int n,k;
    while(scanf("%d%d",&n,&k)!=EOF)
    {
        for(int i=1;i<=n;i++)
        {
            scanf("%d",&a[i].val);
            a[i].pos=i;
        }
        sort(a+1,a+1+n,cmp);

        int maxx=0;
        for(int i=1;i<=k;i++)
        {
            maxx+=a[i].val;
            pos[i]=a[i].pos;
        }
        printf("%d\n",maxx);

        pos[k+1]=n+1;
        sort(pos+1,pos+1+k+1);

        printf("%d ",pos[2]-1);
        for(int i=2;i<=k;i++)
            printf("%d ",pos[i+1]-pos[i]);
        printf("\n");
    }

    return 0;
}
```






