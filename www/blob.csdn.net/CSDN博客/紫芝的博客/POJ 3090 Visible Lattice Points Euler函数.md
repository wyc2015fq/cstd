# POJ 3090 Visible Lattice Points Euler函数 - 紫芝的博客 - CSDN博客





2018年09月18日 15:30:03[紫芝](https://me.csdn.net/qq_40507857)阅读数：27
个人分类：[欧拉函数](https://blog.csdn.net/qq_40507857/article/category/8065000)









Visible Lattice Points
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 8628||**Accepted:** 5288|

Description

A lattice point (***x***, ***y***) in the first quadrant (***x*** and ***y*** are integers greater than or equal to 0), other than the origin, is visible from the origin if the line from (0, 0) to (***x***, ***y***) does not pass through any other lattice point. For example, the point (4, 2) is not visible since the line from the origin passes through (2, 1). The figure below shows the points (***x***, ***y***) with 0 ≤ ***x***, ***y*** ≤ 5 with lines from the origin to the visible points.

![](http://poj.org/images/3090_1.png)

Write a program which, given a value for the size, ***N***, computes the number of visible points (***x***, ***y***) with 0 ≤ ***x***, ***y*** ≤ ***N***.

Input

The first line of input contains a single integer ***C*** (1 ≤ ***C*** ≤ 1000) which is the number of datasets that follow.

Each dataset consists of a single line of input containing a single integer ***N*** (1 ≤ ***N*** ≤ 1000), which is the size.

Output

For each dataset, there is to be one line of output consisting of: the dataset number starting at 1, a single space, the size, a single space and the number of visible points for that size.

Sample Input

4
2
4
5
231
Sample Output

1 2 5
2 4 13
3 5 21
4 231 32549
给出范围为(0, 0)到(n, n)的整点，你站在原点处，问有多少个整点可见。

对于点(x, y), 若g = gcd(x, y) > 1，则该点必被点(x/g, y/g)所挡住。

因此所见点除了(1, 0)和(0, 1)满足横纵坐标互素。

最终答案为![](https://images0.cnblogs.com/blog/640481/201412/180526520626712.gif),其中的+3对应(1, 1) (1, 0) (0, 1)三个点

```cpp
//对于点（x,y），若g=gcd(x,y)>1,则一定会被gcd(x/g,y/g)点挡住
#include<cstdio>
#include<cstring>
#include<cmath>
#include<vector>
#include<algorithm>
using namespace std;
typedef long long ll;
const int maxn=1e3+7;
int euler[maxn];
void getEuler()
{
    for(int i=1;i<maxn;i++)
        euler[i]=i;
    for(int i=2;i<maxn;i++)
        if(euler[i]==i)
        for(int j=i;j<maxn;j+=i)
        euler[j]=euler[j]/i*(i-1);
}
int main()
{
    getEuler();
    int T,ca=1;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        ll ans=0;
        for(int i=2;i<=n;i++)
            ans+=euler[i];
        printf("%d %d %lld\n",ca++,n,ans*2+3);
    }
    return 0;
}
```





