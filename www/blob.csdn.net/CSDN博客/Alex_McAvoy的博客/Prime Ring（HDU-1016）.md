# Prime Ring（HDU-1016） - Alex_McAvoy的博客 - CSDN博客





2018年03月19日 19:23:17[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：62
个人分类：[HDU																[搜索——更具技巧的 DFS](https://blog.csdn.net/u011815404/article/category/8228515)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    A ring is compose of n circles as shown in diagram. Put natural number 1, 2, ..., n into each circle separately, and the sum of numbers in two adjacent circles should be a prime.

    Note: the number of first circle should always be 1.

![](https://img-blog.csdn.net/20180321120855956)

# Input

    n (0 < n < 20).

# Output

    The output format is shown as sample below. Each row represents a series of circle numbers in the ring beginning from 1 clockwisely and anticlockwisely. The order of numbers must satisfy the above requirements. Print solutions in lexicographical order.

    You are to write a program that completes above process.

    Print a blank line after each case.

# Sample Input

**6**

**8**

# Sample Output

**Case 1: **

**   1 4 3 2 5 6   1 6 5 2 3 4**


**   Case 2:   1 2 3 8 5 6 7 4   1 2 5 8 3 4 7 6   1 4 7 6 5 8 3 2   1 6 7 4 3 8 5 2**



————————————————————————————————————————————

思路：
- 由于要找的是一个环，因此先固定第一个数，再搜索寻找其它的数。
- 当找到最后一个数时，需判断与第一个数的和是否为素数。
- 注意输出格式，每两个Case间有一空行

# Source Program

```cpp
#include<cmath>
#include<cstdio>
#include<cstring>

int n;
int a[21];
bool vis[21];

bool prime(int n)//判断素数
{
    int i;
    for(i=2;i<=sqrt(n);i++)
        if(n%i==0)
           return false;
    return true;
}

void dfs(int x)
{
    int i;
    if(x==n)//当遍历到a[n-1]后，需要再次递归，此时x==n
    {
        if(prime(1+a[n-1]))//应该计算1与最后数的和是否为素数，若是则输出结果
        {
            printf("1");//输出1
            for(i=1;i<n;i++)//循环输入1后剩余的数
                printf(" %d",a[i]);
            printf("\n");
        }
    }
    for(i=2;i<=n;i++)
    {
        if(vis[i]==1)   continue;//若当前数已使用过，跳过
        if(prime(i+a[x-1]))//判断相邻两元素和是否为素数
        {
            vis[i]=1;//标记为使用
            a[x]=i;//记录当前数
            dfs(x+1);//向下搜索
            vis[i]=0;//回溯
        }
    }
}

int main()
{
    int t=1;

    while(scanf("%d",&n)!=EOF)
    {
        memset(vis,0,sizeof(vis));//用于判断的数组归零
        printf("Case %d:\n",t);
        t++;
        a[0]=1;//初值定义
        dfs(1);
        printf("\n");
    }
    return 0;
}
```




