# nyoj299——如何优雅的写矩阵快速幂 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## Matrix Power Series

时间限制：1000 ms  |  内存限制：65535 KB

难度：4



- 描述
Given a *n* × *n* matrix *A* and a positive integer *k*, find the sum *S* = *A* + *A*2 + *A*3 + … + *Ak*.

- 输入
The input contains exactly one test case. The first line of input contains three positive integers n (n ≤ 30), k (k ≤ 10^9) and m (m < 10^4). Then follow n lines each containing n nonnegative integers below 32,768, giving A’s elements in row-major order.- 输出
Output the elements of S modulo m in the same way as A is given.- 样例输入
2 2 4
0 1
1 1- 样例输出
1 2
2 3


```
#include<bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int maxn = 1000000;
const int moder = 1000000;

const int N=61;
int c[N][N],a[N][N],b[N][N],n,mo;
void mult(int x[N][N],int y[N][N])//x = x*y
{
    int i,j,k;
    for (i=1;i<=n;i++)
        for (j=1;j<=n;j++)
        {
            c[i][j]=0;
            for (k=1;k<=n;k++) c[i][j]=(c[i][j]+x[i][k]*y[k][j])%mo;
        }
    for (i=1;i<=n;i++)
        for (j=1;j<=n;j++) x[i][j]=c[i][j];
}


int main()
{
    int m,i,j;
    scanf("%d%d%d",&n,&m,&mo);
    for (i=1;i<=n;i++)
    {
        for (j=1;j<=n;j++) scanf("%d",&a[i][j]);
        a[i][i+n]=a[i+n][i+n]=b[i][i]=b[i+n][i+n]=1; //b单位矩阵，a为所求的基础矩阵
    }
    n*=2;
    m++;
    while(m>0)
    {
        if (m%2) mult(b,a);
        m/=2;
        mult(a,a);
    }
    n/=2;
    for (i=1;i<=n;i++) b[i][i+n]--;
    for (i=1;i<=n;i++)
    {
        for (j=1;j<n;j++) printf("%d ",b[i][j+n]);
        printf("%d\n",b[i][j+n]);
    }
    return 0;
}
```

网上看了许多快速幂的写法，感觉都很麻烦，只有这个写的很通俗易懂

但题目要求的是 A+A2+...+Ak，而不是单个矩阵的幂

　　那么我们可以构造一个分块的辅助矩阵 S，其中 A 为原矩阵，E 为单位矩阵，O 为0矩阵

![](https://images2015.cnblogs.com/blog/999549/201609/999549-20160924161629731-1694213740.png)

　　我们将 S 取幂，会发现一个特性

![](https://images2015.cnblogs.com/blog/999549/201609/999549-20160924162453731-374268381.png)

　　Sk 右上角那一块不正是我们要求的 A+A2+...+Ak 吗？

　　于是我们构造出 S 矩阵，然后对它求矩阵快速幂即可，最后别忘了减去一个单位阵

转自 https://www.cnblogs.com/hadilo/p/5903514.html











