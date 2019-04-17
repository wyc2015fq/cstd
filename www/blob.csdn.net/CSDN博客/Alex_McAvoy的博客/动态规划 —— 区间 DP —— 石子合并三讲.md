# 动态规划 ——  区间 DP —— 石子合并三讲 - Alex_McAvoy的博客 - CSDN博客





2018年08月17日 10:48:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：81








石子合并问题是最经典的 DP 问题，其有如下3种题型：

# 【任意合并】

1.问题：

有N堆石子，现要将石子有序的合并成一堆，规定如下：每次只能移动**任意**的2堆石子合并，合并花费为新合成的一堆石子的数量。求将这N堆石子合并成一堆的总花费最小（或最大）。

2.分析：

最简单的情况，合并的是任意两堆，直接贪心即可，每次选择最小的两堆合并，实际上就是哈夫曼树的变形。

3.实现：

```cpp
int Compare(const void *pleft, const void *pright)
{
    int *left = (int *)pleft;
    int *right = (int *)pright;

    return (*left - *right);
}


#define SIZE 100
int N;
int W[SIZE];

int main(void)
{
    while(scanf("%d", &N), N)
    {
        for(int i = 0; i < N; i++)
            scanf("%d", &W[i]);

        for(int i = 0; i < N; i++)
        {
            qsort(&W[i], N - i, sizeof(W[0]), Compare);

            for(int j = 0; j < N; j++)
                printf("%4d", W[j]);
            printf("\n");
            
            W[i + 1] += W[i]; //此时w[i]和w[i + 1]就是最小的两个
        }

        printf("%d\n", W[N - 1]);
    }
}
```

# 【直线下的相邻合并】

1.问题：

有N堆石子直线排列，现要将石子有序的合并成一堆，规定如下：每次只能移动**相邻**的2堆石子合并，合并花费为新合成的一堆石子的数量。求将这N堆石子合并成一堆的总花费最小（或最大）。

2.分析：

我们熟悉矩阵连乘，知道矩阵连乘也是每次合并相邻的两个矩阵，那么石子合并可以用矩阵连乘的方式来解决。

设 dp[i][j] 表示第 i 到第 j 堆石子合并的最优值，sum[i][j] 表示第 i 到第 j 堆石子的总数量。

那么就有状态转移方程：

![](https://img-blog.csdn.net/20180817103849833?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3.实现

```cpp
#include <iostream>  
#include <string.h>  
#include <stdio.h>  
  
using namespace std;  
const int INF = 1 << 30;  
const int N = 205;  
  
int dp[N][N];  
int sum[N];  
int a[N];  
  
int getMinval(int a[],int n)  
{  
    for(int i=0;i<n;i++)  
        dp[i][i] = 0;  
    for(int v=1;v<n;v++)  
    {  
        for(int i=0;i<n-v;i++)  
        {  
            int j = i + v;  
            dp[i][j] = INF;  
            int tmp = sum[j] - (i > 0 ? sum[i-1]:0);  
            for(int k=i;k<j;k++)  
                dp[i][j] = min(dp[i][j],dp[i][k]+dp[k+1][j] + tmp);  
        }  
    }  
    return dp[0][n-1];  
}  
  
int main()  
{  
    int n;  
    while(scanf("%d",&n)!=EOF)  
    {  
        for(int i=0;i<n;i++)  
            scanf("%d",&a[i]);  
        sum[0] = a[0];  
        for(int i=1;i<n;i++)  
            sum[i] = sum[i-1] + a[i];  
        printf("%d\n",getMinval(a,n));  
    }  
    return 0;  
}
```

平行四边形优化实现

```cpp
#include <iostream>  
#include <string.h>  
#include <stdio.h>  
  
using namespace std;  
const int INF = 1 << 30;  
const int N = 1005;  
  
int dp[N][N];  
int p[N][N];  
int sum[N];  
int n;  
  
int getMinval()  
{  
    for(int i=1; i<=n; i++)  
    {  
        dp[i][i] = 0;  
        p[i][i] = i;  
    }  
    for(int len=1; len<n; len++)  
    {  
        for(int i=1; i+len<=n; i++)  
        {  
            int end = i+len;  
            int tmp = INF;  
            int k = 0;  
            for(int j=p[i][end-1]; j<=p[i+1][end]; j++)  
            {  
                if(dp[i][j] + dp[j+1][end] + sum[end] - sum[i-1] < tmp)  
                {  
                    tmp = dp[i][j] + dp[j+1][end] + sum[end] - sum[i-1];  
                    k = j;  
                }  
            }  
            dp[i][end] = tmp;  
            p[i][end] = k;  
        }  
    }  
    return dp[1][n];  
}  
  
int main()  
{  
    while(scanf("%d",&n)!=EOF)  
    {  
        sum[0] = 0;  
        for(int i=1; i<=n; i++)  
        {  
            int val;  
            scanf("%d",&val);  
            sum[i] = sum[i-1] + val;  
        }  
        printf("%d\n",getMinval());  
    }  
    return 0;  
}
```

# 【环形下的相邻合并】

1.问题

有N堆石子环形排列，现要将石子有序的合并成一堆，规定如下：每次只能移动**相邻**的2堆石子合并，合并花费为新合成的一堆石子的数量。求将这N堆石子合并成一堆的总花费最小（或最大）。

2.分析：

状态转移方程：

![](https://img-blog.csdn.net/20180817104634706?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中：

![](https://img-blog.csdn.net/20180817104645392?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3.实现：

```cpp
#include <iostream>  
#include <string.h>  
#include <stdio.h>  
  
using namespace std;  
const int INF = 1 << 30;  
const int N = 205;  
  
int mins[N][N];  
int maxs[N][N];  
int sum[N],a[N];  
int minval,maxval;  
int n;  
  
int getsum(int i,int j)  
{  
    if(i+j >= n) return getsum(i,n-i-1) + getsum(0,(i+j)%n);  
    else return sum[i+j] - (i>0 ? sum[i-1]:0);  
}  
  
void Work(int a[],int n)  
{  
    for(int i=0;i<n;i++)  
        mins[i][0] = maxs[i][0] = 0;  
    for(int j=1;j<n;j++)  
    {  
        for(int i=0;i<n;i++)  
        {  
            mins[i][j] = INF;  
            maxs[i][j] = 0;  
            for(int k=0;k<j;k++)  
            {  
                mins[i][j] = min(mins[i][j],mins[i][k] + mins[(i+k+1)%n][j-k-1] + getsum(i,j));  
                maxs[i][j] = max(maxs[i][j],maxs[i][k] + maxs[(i+k+1)%n][j-k-1] + getsum(i,j));  
            }  
        }  
    }  
    minval = mins[0][n-1];  
    maxval = maxs[0][n-1];  
    for(int i=0;i<n;i++)  
    {  
        minval = min(minval,mins[i][n-1]);  
        maxval = max(maxval,maxs[i][n-1]);  
    }  
}  
  
int main()  
{  
    while(scanf("%d",&n)!=EOF)  
    {  
        for(int i=0;i<n;i++)  
            scanf("%d",&a[i]);  
        sum[0] = a[0];  
        for(int i=1;i<n;i++)  
            sum[i] = sum[i-1] + a[i];  
        Work(a,n);  
        printf("%d %d\n",minval,maxval);  
    }  
    return 0;  
}
```





