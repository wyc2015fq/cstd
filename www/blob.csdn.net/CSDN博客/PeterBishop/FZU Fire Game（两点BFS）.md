# FZU Fire Game（两点BFS） - PeterBishop - CSDN博客





2018年07月27日 10:26:16[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：34








```cpp
#include<stdio.h>
#include<string.h>
#include<queue>
using namespace std;
#define min(a,b)  (a<b?a:b)
#define max(a,b)  (a>b?a:b)
const int INF=0x3f3f3f3f;
int n,m;
int vis[15][15];
char s[15][15];
int to[4][2]={0,1,0,-1,1,0,-1,0};
struct node
{
    int x,y,step;
}w[100];
int judge(int x,int y)
{
    if(x>=0&&y>=0&&x<n&&y<m&&!vis[x][y]&&s[x][y]=='#')
        return 1;
    return 0;
}
int bfs(node a,node b)//两点进去同时搜
{
    queue<node>q;
    q.push(a);
    q.push(b);
    vis[a.x][a.y]=1;
    vis[b.x][b.y]=1;
    int cnt=0;
    while(!q.empty())
    {
        a=q.front();
        cnt=max(cnt,a.step);//获得这一轮搜索的烧草的值（花费的时间）
        for(int i=0;i<4;i++)
        {
            b.x=a.x+to[i][0],b.y=a.y+to[i][1],b.step=a.step+1;
            if(judge(b.x,b.y))
            {
                q.push(b);
                vis[b.x][b.y]=1;
            }
        }
        q.pop();
    }
    return cnt;
}
int main()
{
    int cont=0,t;
    scanf("%d",&t);
    while(++cont<=t)
    {
        int i,j,k,l,sum=0;
        scanf("%d%d",&n,&m);
        for(i=0;i<n;i++)
        {
            scanf("%s",s[i]);
            for(j=0;j<m;j++)
            {
                if(s[i][j]=='#')
                {
                    w[sum].x=i,w[sum].y=j,w[sum].step=0;//记录可搜点的坐标
                    sum++;
                }
            }
        }
        if(sum<=2)
            printf("Case %d: 0\n",cont);
        else
        {
            int ans=INF;
            for(i=0;i<sum;i++)
            {
                for(j=i;j<sum;j++)
                {
                    memset(vis,0,sizeof(vis));
                    int cnt=bfs(w[i],w[j]);//两点BFS
                    int flag=0;
                    for(k=0;k<n;k++)
                    {
                        for(l=0;l<m;l++)
                        {
                            if(s[k][l]=='#'&&!vis[k][l])//判断是否剩下没有点燃的草
                            {
                                flag=1;
                                break;
                            }
                        }
                        if(flag)
                            break;
                    }
                    if(!flag)//全部烧完，则求最短时间
                        ans=min(ans,cnt);
                }
            }
            if(ans==INF)
                printf("Case %d: -1\n",cont);
            else
                printf("Case %d: %d\n",cont,ans);
        }
    }
    return 0;
}
```





