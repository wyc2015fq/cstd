# T1212 LETTERS （#Ⅱ- 5） - Alex_McAvoy的博客 - CSDN博客





2018年05月11日 15:46:03[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：1595
个人分类：[信息学奥赛一本通																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/7429967)








> 
# 【题目描述】

给出一个roe×col的大写字母矩阵，一开始的位置为左上角，你可以向上下左右四个方向移动，并且不能移向曾经经过的字母。问最多可以经过几个字母。

# 【输入】

第一行，输入字母矩阵行数R和列数S，1≤R,S≤20。

接着输出R行S列字母矩阵。

# 【输出】

最多能走过的不同字母的个数。

# 【输入样例】

**3 6HFDFFBAJHGDHDGAGEH**

# 【输出样例】

**6**


# 【源程序】

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#define N 30
using namespace std;
int r,s;
char a[N][N];
int vis[N][N];
int num[26];
int dir[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
int maxx=0;
void dfs(int x,int y,int step)
{
    if(maxx<step)
        maxx=step;
    for(int i=0;i<4;i++)
    {
        int nx=x+dir[i][0];
        int ny=y+dir[i][1];
        if(nx>=0&&nx<r&&ny>=0&&ny<s&&vis[nx][ny]==0&&num[a[nx][ny]-'A']==0)
        {
            vis[nx][ny]=1;
            num[a[nx][ny]-'A']=1;
            dfs(nx,ny,step+1);
            vis[nx][ny]=0;
            num[a[nx][ny]-'A']=0;
        }
    }
}
int main()
{
    cin>>r>>s;
    for(int i=0;i<r;i++)
        for(int j=0;j<s;j++)
            cin>>a[i][j];
    num[a[0][0]-'A']=1;
    vis[0][0]=1;
    dfs(0,0,1);
    cout<<maxx<<endl;
    return 0;
}
```






