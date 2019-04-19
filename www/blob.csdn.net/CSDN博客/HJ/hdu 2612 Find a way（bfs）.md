# hdu 2612 Find a way（bfs） - HJ - CSDN博客
2018年04月25日 19:27:24[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：77
个人分类：[====ACM====																[-----------搜索-----------																[BFS																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238039)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=2612](http://acm.hdu.edu.cn/showproblem.php?pid=2612)
题目大意：给出两个人的坐标和若干个大峡谷的坐标，每个人走一步需要花费11秒的时间，问这两个人去某个大峡谷回合所需要的最小时间和为多少？
解题思路：裸的bfs，把两个人一个看成起点一个看成终点，bfs搜两遍，统计每个大峡谷坐标(x,y)被搜了两次之后的时间值，每次更新ans，求出最小即可。
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue> 
using namespace std;
#define INF 0x3f3f3f3f
const int N=205;
struct node{
    int x,y,step;   //x,y组成当前点的坐标  step表示从起点到到当前点(x,y)需要步数 
    node(int x,int y,int step):x(x),y(y),step(step){};
};
int sum[N][N];   //表示起点或者终点到该点的次数 如果sum[i][j]==2表示从起点到终点的某条路线经过点(i,j) 
int vis[N][N];    //访问标记 
int k[N][N];     //k[i][j]表示从起点或者终点到点(i,j)的总步数 
int n,m,sx,sy,ex,ey,ans;
int px[]={1,-1,0,0},py[]={0,0,1,-1};
char s[N][N];
void bfs(int x,int y)
{
    memset(vis,0,sizeof(vis));
    queue<node> q;
    q.push(node(x,y,0));
    vis[x][y]=1;
    while(!q.empty())
    {
        node now=q.front();
        q.pop();
        for(int i=0;i<4;i++)
        {
            int dx=now.x+px[i];
            int dy=now.y+py[i];
            if(!vis[dx][dy]&&dx>=0&&dx<n&&dy>=0&&dy<m&&s[dx][dy]!='#'&&s[dx][dy]!='M')
            {
                vis[dx][dy]=1;
                if(s[dx][dy]=='@')
                {
                    k[dx][dy]+=now.step+1;   //累加步数 
                    sum[dx][dy]++;    //统计(dx,dy)被访问的次数 
                }
                if(sum[dx][dy]==2)     //从起点到终点经过点(dx,dy),也就是点(dx,dy)已经被访问两次  
                  ans=min(ans,k[dx][dy]);   //更新最小所需要的步数 
                q.push(node(dx,dy,now.step+1)); 
            }
        }
    }
}
int main()
{
    while(cin>>n>>m)
    {
        memset(sum,0,sizeof(sum));
        memset(k,0,sizeof(k));
        for(int i=0;i<n;i++)
          for(int j=0;j<m;j++)
          {
              cin>>s[i][j];
              if(s[i][j]=='Y')
                sx=i,sy=j;
              else if(s[i][j]=='M')
                ex=i,ey=j;
          }
          ans=INF;
          bfs(sx,sy);    //从起点搜一遍 
          bfs(ex,ey);   //再从终点搜一遍 
          cout<<ans*11<<endl;
    }
    return 0;
}
```
