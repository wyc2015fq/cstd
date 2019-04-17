# Codeforces1064D——Labyrinth - westbrook1998的博客 - CSDN博客





2018年10月14日 20:36:01[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：257








> 
题意一看就是搜索，多了一个限制就是往左和往右的次数，就在广搜队列的节点再加个当前可以往左/右走的个数


代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
using namespace std;
const int N=2010;
int n,m;
int r,c;
int x,y;
char mp[N][N];
//普通bfs出错的原因是因为我们只考虑第一次到达该点的情况，然后就标记为vis
//这样子第二次到达这个点的剩余左右步数有可能可以到达第一次到达这个点后不能到达的地方
struct node{
    int x,y,l,r;
    //改用优先队列
    bool operator <(const node &a) const{
        if(l==a.l){
            return r<a.r;
        }
        else{
            return l<a.l;
        }
    }
};
bool vis[N][N];
int dx[4]={-1,0,0,1};
int dy[4]={0,1,-1,0};
bool check(int x,int y){
    return x>=1 && x<=n && y>=1 && y<=m && !vis[x][y] && mp[x][y]!='*';
}
int main(void){
    scanf("%d%d",&n,&m);
    scanf("%d%d",&r,&c);
    scanf("%d%d",&x,&y);
    for(int i=1;i<=n;i++){
        scanf("%s",mp[i]+1);
    }
    int cnt=1;
    priority_queue<node> q;
    q.push(node{r,c,x,y});
    vis[r][c]=true;
    while(!q.empty()){
        node t=q.top();
        q.pop();
        for(int i=0;i<4;i++){
            int tx=t.x+dx[i];
            int ty=t.y+dy[i];
            if(!check(tx,ty)){
                continue;
            }
            if(i==0 || i==3){
                cnt++;
                vis[tx][ty]=true;
                q.push(node{tx,ty,t.l,t.r});
            }
            else if(i==1){
                if(t.r>=1){
                    cnt++;
                    vis[tx][ty]=true;
                    q.push(node{tx,ty,t.l,t.r-1});
                }
            }
            else if(i==2){
                if(t.l>=1){
                    cnt++;
                    vis[tx][ty]=true;
                    q.push(node{tx,ty,t.l-1,t.r});
                }
            }
        }
    }
    printf("%d\n",cnt);
    return 0;
}
```



