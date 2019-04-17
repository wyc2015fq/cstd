# hdu2612——Find a way - westbrook1998的博客 - CSDN博客





2018年07月02日 13:10:54[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：27标签：[bfs](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Pass a year learning in Hangzhou, yifenfei arrival hometown Ningbo at finally. Leave Ningbo one year, yifenfei have many people to meet. Especially a good friend Merceki.  

  Yifenfei’s home is at the countryside, but Merceki’s home is in the center of city. So yifenfei made arrangements with Merceki to meet at a KFC. There are many KFC in Ningbo, they want to choose one that let the total time to it be most smallest.  

  Now give you a Ningbo map, Both yifenfei and Merceki can move up, down ,left, right to the adjacent road by cost 11 minutes.  

  Input 

  The input contains multiple test cases.  

  Each test case include, first two integers n, m. (2<=n,m<=200).  

  Next n lines, each line included m character.  

  ‘Y’ express yifenfei initial position.  

  ‘M’    express Merceki initial position.  

  ‘#’ forbid road;  

  ‘.’ Road.  

  ‘@’ KCF  

  Output 

  For each test case output the minimum total time that both yifenfei and Merceki to arrival one of KFC.You may sure there is always have a KFC that can let them meet. 

  Sample Input 

  4 4 

  Y.#@ 

  …. 

  .#.. 

  @..M 

  4 4 

  Y.#@ 

  …. 

  .#.. 

  @#.M 

  5 5 

  Y..@. 

  .#… 

  .#… 

  @..M. 
`#...#`

  Sample Output 

  66 

  88 

  66
有两个起点 多个终点 分别对两个起点进行深搜并打表（从起点到任意一个能到的点的时间） 最后找出都两个起点都能到达的最小时间的点

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <queue>
#include <cstring>
using namespace std;
const int MAXN=210;
int n,m;
int ycnt[MAXN][MAXN];
int mcnt[MAXN][MAXN];
int vis[MAXN][MAXN];
int kfc[MAXN*MAXN][2];
int to[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
char map[MAXN][MAXN];
struct node{
    int x;
    int y;
};
bool check(int x,int y){
    if(x>=0 && x<n && y>=0 && y<m && map[x][y]!='#' && vis[x][y]==0){
        return true;
    }
    else{
        return false;
    }
}
void bfs(int flag,int x,int y)
{
    int dx,dy;
    node p,temp;
    queue<node> q;
    p.x=x;
    p.y=y;
    q.push(p);
    if(flag==1)
    {
        while(!q.empty())
        {
          p=q.front();
          q.pop();
          for(int i=0;i<4;i++)
          {
              dx=p.x+to[i][0];dy=p.y+to[i][1];
              if(check(dx,dy))
              {
                 vis[dx][dy]=-1;
                 temp.x=dx;
                 temp.y=dy;
                 q.push(temp);
                 ycnt[dx][dy]=ycnt[p.x][p.y]+11;
              }
          }
        }
    }
    else
    {
        while(!q.empty())
        {
          p=q.front();
          q.pop();
          for(int i=0;i<4;i++)
          {
              dx=p.x+to[i][0];dy=p.y+to[i][1];
              if(check(dx,dy))
              {
                 vis[dx][dy]=-1;
                 temp.x=dx;
                 temp.y=dy;
                 q.push(temp);
                 mcnt[dx][dy]=mcnt[p.x][p.y]+11;
              }
          }
        }
    }
}
int main()
{
    int kfc[40005][2];
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        int kfc_num=0;
        int min=1e8;
        int yx,yy,mx,my;
        getchar();
        for(int i=0;i<n;i++){
            scanf("%s",map[i]);
        }  
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<m;j++)
            {
                ycnt[i][j]=0;
                mcnt[i][j]=0;
                vis[i][j]=0;
                if(map[i][j]=='Y')
                {
                  yx=i;
                  yy=j;
                }
                else if(map[i][j]=='M')
                {
                  mx=i;
                  my=j;
                }
                else if(map[i][j]=='@')
                {
                  kfc[kfc_num][0]=i;
                  kfc[kfc_num++][1]=j;
                }
            }
        }
        memset(vis,0,sizeof(vis));
        vis[yx][yy]=1;
        bfs(1,yx,yy);
        memset(vis,0,sizeof(vis));
        vis[mx][my]=1;
        bfs(0,mx,my);
        for(int i=0;i<kfc_num;i++){
            //y m都能到达的kfc
            if(ycnt[kfc[i][0]][kfc[i][1]]!=0 && mcnt[kfc[i][0]][kfc[i][1]]){
                if((ycnt[kfc[i][0]][kfc[i][1]]+mcnt[kfc[i][0]][kfc[i][1]])<min){
                    min=(ycnt[kfc[i][0]][kfc[i][1]]+mcnt[kfc[i][0]][kfc[i][1]]);
                }
            }
        }
        printf("%d\n",min);
    }
    return 0;
}
```





