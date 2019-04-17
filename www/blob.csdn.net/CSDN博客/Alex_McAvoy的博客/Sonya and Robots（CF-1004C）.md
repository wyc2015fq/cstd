# Sonya and Robots（CF-1004C） - Alex_McAvoy的博客 - CSDN博客





2018年08月26日 18:36:42[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：69
个人分类：[CodeForces																[基础算法——排序](https://blog.csdn.net/u011815404/article/category/7439201)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Since Sonya is interested in robotics too, she decided to construct robots that will read and recognize numbers.

Sonya has drawn nn numbers in a row, aiai is located in the ii-th position. She also has put a robot at each end of the row (to the left of the first number and to the right of the last number). Sonya will give a number to each robot (they can be either same or different) and run them. When a robot is running, it is moving toward to another robot, reading numbers in the row. When a robot is reading a number that is equal to the number that was given to that robot, it will turn off and stay in the same position.

Sonya does not want robots to break, so she will give such numbers that robots will stop before they meet. That is, the girl wants them to stop at different positions so that the first robot is to the left of the second one.

For example, if the numbers [1,5,4,1,3] are written, and Sonya gives the number 1 to the first robot and the number 4 to the second one, the first robot will stop in the 1-st position while the second one in the 3-rd position. In that case, robots will not meet each other. As a result, robots will not be broken. But if Sonya gives the number 4 to the first robot and the number 5 to the second one, they will meet since the first robot will stop in the 3-rd position while the second one is in the 2-nd position.

Sonya understands that it does not make sense to give a number that is not written in the row because a robot will not find this number and will meet the other robot.

Sonya is now interested in finding the number of different pairs that she can give to robots so that they will not meet. In other words, she wants to know the number of pairs (p, q), where she will give p to the first robot and q to the second one. Pairs (pi, qi) and (pj, qj) are different if pi≠pj orqi≠qj.

Unfortunately, Sonya is busy fixing robots that broke after a failed launch. That is why she is asking you to find the number of pairs that she can give to robots so that they will not meet.

# Input

The first line contains a single integer n (1≤n≤105) — the number of numbers in a row.

The second line contains nn integers a1,a2,…,an (1≤ai≤105) — the numbers in a row.

# Output

Print one number — the number of possible pairs that Sonya can give to robots so that they will not meet.

# Examples

**Input**

5

1 5 4 1 3

**Output**

9

**Input**

7

1 2 1 1 1 3 2

**Output**

7

————————————————————————————————————————————

题意：给出 n 个数字，让从这个序列中选一对数 (a,b)，使得从左边数第一个 a 出现的位置大于从右边数第一个 b 出现的位置，求符合要求的对数

思路来源：[点击这里](https://blog.csdn.net/ZscDst/article/details/80937660)

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
#define N 1000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int a[N],bucket[N],vis[N];
int main()
{
    int n;
    scanf("%d",&n);

    int cnt=0;
    for(int i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
        if(bucket[a[i]]++==0)
            cnt++;
    }
    LL ans=0;
    for(int i=0;i<n;i++)
    {
        if(--bucket[a[i]]==0)
            cnt--;
        if(!vis[a[i]])
        {
            ans+=cnt;
            vis[a[i]]=1;
        }
    }

    printf("%lld\n",ans);

    return 0;
}
```






