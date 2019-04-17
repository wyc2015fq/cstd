# Visible Lattice Points（POJ-3090） - Alex_McAvoy的博客 - CSDN博客





2018年08月01日 20:50:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：69
个人分类：[POJ																[数论——欧拉函数](https://blog.csdn.net/u011815404/article/category/8796918)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

A lattice point (x, y) in the first quadrant (x and y are integers greater than or equal to 0), other than the origin, is visible from the origin if the line from (0, 0) to (x, y) does not pass through any other lattice point. For example, the point (4, 2) is not visible since the line from the origin passes through (2, 1). The figure below shows the points (x, y) with 0 ≤ x, y ≤ 5 with lines from the origin to the visible points.

Write a program which, given a value for the size, N, computes the number of visible points (x, y) with 0 ≤ x, y ≤ N.

![](https://img-blog.csdn.net/20180801202421865?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# **Input**

The first line of input contains a single integer C (1 ≤ C ≤ 1000) which is the number of datasets that follow.

Each dataset consists of a single line of input containing a single integer N (1 ≤ N ≤ 1000), which is the size.

# Output

For each dataset, there is to be one line of output consisting of: the dataset number starting at 1, a single space, the size, a single space and the number of visible points for that size.

# Sample Input

**4245231**

# Sample Output

**1 2 52 4 133 5 214 231 32549**


题意：t 组数据，每组数据给出一个 n，求从原点到第 (n，n) 的范围内，可看到的第一象限的所有点，要求输出每组数据号、n 的大小以及能直接看到的点的数目。

思路：

题目要求的是从原点能看到的点的个数。

只有 1*1的时候，3个点，根据图明显的可以看出，只要计算下三角，即：结果=下三角的个数*2+斜率为1的点

因此，我们只需计算斜率 ![k\in(0,1)](https://private.codecogs.com/gif.latex?k%5Cin%280%2C1%29) 的个数即可

1*1 时，有：0，共 1 个

2*2 时，有：0、1/2，共 2 个，添加 1 个

3*3 时，有：0、1/2、1/3、2/3，共 4 个，添加 3 个

4*4 时，有：0、1/2、1/3、2/3/、1/4、2/4、3/4，去重 1 个后，共 6 个，添加 2 个

5*5 时，有：0、1/2、1/3、2/3、1/4、3/4、1/5、2/5、3/5、4/5，共 10 个，添加 4 个

由上可发现一规律：对于 n*n，可从 (0, 0) 连接到 (n,0) 至 (n,n) 上，斜率是：1/n、2/n、3/n、...、(n-1)/n

凡是分子、分母能约分的，即有公约数的，前面都已经有过了，所以每次添加的个数就是分子和分母互质的个数，因此问题也就转换为：对一个数 n，求小于 n 的与 n 互质的数的个数并乘以2再加一，也就是求欧拉函数再乘2加1

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
#define N 1000005
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int phi[N],prime[N];
bool mark[N];
LL sum[N],res[N];
void Euler(int n)
{
    int cnt=0;
    phi[1]=1;
    for(int i=2;i<=n;i++)
    {
        if(!mark[i])
        {
            prime[++cnt]=i;//筛素数的时先判断i是否是素数
            phi[i]=i-1;//当i是素数时phi[i]=i-1
        }
        for(int j=1;j<=cnt;j++)
        {
            if(i*prime[j]>N)
                break;
            mark[i*prime[j]]=1;//确定i*prime[j]不是素数
            if(i%prime[j]==0)//看prime[j]是否是i的约数
            {
                phi[i*prime[j]]=phi[i]*prime[j];
                break;
            }
            else
                phi[i*prime[j]]=phi[i]*(prime[j]-1);//其prime[j]-1就是phi[prime[j]]，利用了欧拉函数的积性
       }
   }
}
int main()
{
    Euler(N);
    for(int i=1;i<=N;i++)
        sum[i]=sum[i-1]+phi[i];
    for(int i=1;i<=N;i++)
        res[i]=sum[i]*2+1;

    int t;
    scanf("%d",&t);

    int cnt=1;
    while(t--)
    {
        int n;
        scanf("%d",&n);
        printf("%d %d %d\n",cnt++,n,res[n]);
    }
    return 0;
}
```






