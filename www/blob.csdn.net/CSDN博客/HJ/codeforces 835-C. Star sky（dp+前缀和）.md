# codeforces 835-C. Star sky（dp+前缀和） - HJ - CSDN博客
2017年08月01日 16:55:22[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：294
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
The Cartesian coordinate system is set in the sky. There you can see n stars, the i-th has coordinates (xi, yi), a maximum brightness c, equal for all stars, and an initial brightness si (0 ≤ si ≤ c).
Over time the stars twinkle. At moment 0 the i-th star has brightness si. Let at moment t some star has brightness x. Then at moment (t + 1) this star will have brightness x + 1, if x + 1 ≤ c, and 0, otherwise.
You want to look at the sky q times. In the i-th time you will look at the moment ti and you will see a rectangle with sides parallel to the coordinate axes, the lower left corner has coordinates (x1i, y1i) and the upper right — (x2i, y2i). For each view, you want to know the total brightness of the stars lying in the viewed rectangle.
A star lies in a rectangle if it lies on its border or lies strictly inside it. 
Input
The first line contains three integers n, q, c (1 ≤ n, q ≤ 105, 1 ≤ c ≤ 10) — the number of the stars, the number of the views and the maximum brightness of the stars.
The next n lines contain the stars description. The i-th from these lines contains three integers xi, yi, si (1 ≤ xi, yi ≤ 100, 0 ≤ si ≤ c ≤ 10) — the coordinates of i-th star and its initial brightness.
The next q lines contain the views description. The i-th from these lines contains five integers ti, x1i, y1i, x2i, y2i (0 ≤ ti ≤ 109, 1 ≤ x1i < x2i ≤ 100, 1 ≤ y1i < y2i ≤ 100) — the moment of the i-th view and the coordinates of the viewed rectangle. 
Output
For each view print the total brightness of the viewed stars. 
Examples 
Input
2 3 3 
1 1 1 
3 2 0 
2 1 1 2 2 
0 2 1 4 5 
5 1 1 5 5
Output
3 
0 
3
Input
3 4 5 
1 1 2 
2 3 0 
3 3 1 
0 1 1 100 100 
1 2 2 4 4 
2 2 1 4 7 
1 50 50 51 51
Output
3 
3 
5 
0
Note
Let’s consider the first example.
At the first view, you can see only the first star. At moment 2 its brightness is 3, so the answer is 3.
At the second view, you can see only the second star. At moment 0 its brightness is 0, so the answer is 0.
At the third view, you can see both stars. At moment 5 brightness of the first is 2, and brightness of the second is 1, so the answer is 3.
题目大意： 
在x*y的格子中（最大规模为100*100），有n个星星，每个星星都有一个发光亮度。并且这种发光亮度随着时间增长，但是不会超过给定值C，如果超过了，就会重新从0开始增长。现在给出q个询问，每个询问给出一个矩形和时间，问在矩形内的星星发光总亮度为多少？
解题思路： 
因为每个星星带有亮度的属性，因此在压缩点的时候需要多开一维。 
用a[i][j][k] 表示点（i，j）发光亮度为k的星星个数。 
接着因为有q次询问，考虑到会超时，需要用到前缀和。 
dp[k][i][j] 表示 第 i 行前 j 个点中发光亮度为k 的星星的个数
```cpp
#include<iostream>
#include<cstring>
using namespace std;
int a[105][105][15],dp[15][105][105];    // 第i行前j个点中s为k的点的个数 
int main()
{
    int n,q,c,x,y,s;
    while(cin>>n>>q>>c)
    {
        memset(a,0,sizeof(a));
        memset(dp,0,sizeof(dp));
        for(int i=0;i<n;i++)
        {
            cin>>x>>y>>s;
            a[x][y][s]++; 
        }
        for(int k=0;k<=10;k++)
          for(int i=1;i<=100;i++)
            for(int j=1;j<=100;j++)
              dp[k][i][j]=dp[k][i][j-1]+a[i][j][k];
        int t,x1,x2,y1,y2;
        for(int i=0;i<q;i++)
        {
            cin>>t>>x1>>y1>>x2>>y2; 
            int ans=0;
            for(int k=0;k<=10;k++)
              for(int j=x1;j<=x2;j++)
                ans+=(dp[k][j][y2]-dp[k][j][y1-1])*((k+t)%(c+1));
            cout<<ans<<endl;
        }
    }
    return 0;
}
```
