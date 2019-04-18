# HDU 5319 Painter (模拟) - xcw0754 - 博客园
# [HDU 5319 Painter (模拟)](https://www.cnblogs.com/xcw0754/p/4684149.html)
题意：
　　一个画家画出一张，有3种颜色的笔，R、G、B。R看成'\'，B看成'/'，G看成这两种的重叠(即叉形)。给的是一个矩阵，矩阵中只有4种符号，除了3种颜色还有'.'，代表没有涂色。问最小耗费多少笔即可画成这副图？
思路：
　　最小耗费就是斜着的可以一笔搞定，但是如果中间隔着'.'或者其他一种形状，则不能一笔，要变两笔。主要麻烦在矩阵不是正方形，而可能是长方形。其实只要按照其画法，从左上往右下方向画，逐个条斜线扫描即可。另一个方向'/'也是如此。
　　我看到模拟本来就不想敲，扫两遍矩阵，用了vector将他们每一条可能可以一笔画的线先装起来，再进行扫vector进行统计。哎~其实可以直接扫两矩阵就统计的，就是懒。
```
#include <bits/stdc++.h>
using namespace std;
const int N=250;
char grid[N][N];
int n;
vector<int> R[N], B[N];
int cal(int col)
{
    int cnt=0;
    for(int i=1; i<=n; i++)//一个方向
    {
        for(int j=1; j<=col; j++)
        {
            if(grid[i][j]=='R'||grid[i][j]=='G')
            {
                if(i>j)    R[i-j].push_back(1);
                else if(i<j)    R[j-i+100].push_back(1);
                else    R[0].push_back(1);
            }
            else
            {
                if(i>j)    R[i-j].push_back(0);
                else if(i<j)    R[j-i+100].push_back(0);
                else    R[0].push_back(0);
            }
        }
    }
    for(int i=0; i<N; i++)
    {
        int flag=0;
        for(int j=0; j<R[i].size(); j++)
        {
            if(!flag && R[i][j]==1)
            {
                cnt++;
                flag=1;
            }
            if(!R[i][j])    flag=0;
        }
    }
    for(int i=1; i<=n; i++)//另一方向
    {
        for(int j=col,k=1; j>=1; j--,k++)
        {
            if(grid[i][j]=='B'||grid[i][j]=='G')
            {
                if(i>k)    B[i-k].push_back(1);
                else if(i<k)    B[k-i+100].push_back(1);
                else    B[0].push_back(1);
            }
            else
            {
                if(i>k)    B[i-k].push_back(0);
                else if(i<k)    B[k-i+100].push_back(0);
                else    B[0].push_back(0);
            }
        }
    }
    for(int i=0; i<N; i++)
    {
        int flag=0;
        for(int j=0; j<B[i].size(); j++)
        {
            if(!flag && B[i][j])
            {
                cnt++;
                flag=1;
            }
            if(!B[i][j])    flag=0;
        }
    }
    return cnt;
}
int main()
{
    //freopen("input.txt", "r", stdin);
    int t;
    cin>>t;
    while(t--)
    {
        scanf("%d",&n);
        for(int i=0; i<N; i++)    R[i].clear(),B[i].clear();
        for(int i=1; i<=n; i++)    scanf("%s", grid[i]+1);
        printf("%d\n",cal(  strlen( grid[1]+1 ) ));
    }
    return 0;
}
AC代码
```

