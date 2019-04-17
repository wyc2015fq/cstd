# fzu2150——Fire Game - westbrook1998的博客 - CSDN博客





2018年08月12日 19:33:08[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：40标签：[bfs																[搜索](https://so.csdn.net/so/search/s.do?q=搜索&t=blog)](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Fat brother and Maze are playing a kind of special (hentai) game on an N*M board (N rows, M columns). At the beginning, each grid of this board is consisting of grass or just empty and then they start to fire all the grass. Firstly they choose two grids which are consisting of grass and set fire. As we all know, the fire can spread among the grass. If the grid (x, y) is firing at time t, the grid which is adjacent to this grid will fire at time t+1 which refers to the grid (x+1, y), (x-1, y), (x, y+1), (x, y-1). This process ends when no new grid get fire. If then all the grid which are consisting of grass is get fired, Fat brother and Maze will stand in the middle of the grid and playing a MORE special (hentai) game. (Maybe it’s the OOXX game which decrypted in the last problem, who knows.) 

  You can assume that the grass in the board would never burn out and the empty grid would never get fire. 

  Note that the two grids they choose can be the same. 

  Input 

  The first line of the date is an integer T, which is the number of the text cases. 

  Then T cases follow, each case contains two integers N and M indicate the size of the board. Then goes N line, each line with M character shows the board. “#” Indicates the grass. You can assume that there is at least one grid which is consisting of grass in the board. 

  1 <= T <=100, 1 <= n <=10, 1 <= m <=10 

  Output 

  For each case, output the case number first, if they can play the MORE special (hentai) game (fire all the grass), output the minimal time they need to wait after they set fire, otherwise just output -1. See the sample input and output for more details. 

  Sample Input
```
4
3 3
.#.
###
.#.
3 3
.#.
#.#
.#.
3 3
...
#.#
...
3 3
###
..#
#.#
```

> 
Sample Output 

  Case 1: 1 

  Case 2: -1 

  Case 3: 0 

  Case 4: 2
双节点bfs搜索的题目，搜索真的好难写啊，wa到自闭，一个不小心就爆炸，还很难调 

这题思路就是枚举两个起点，加入队列然后进行bfs 

把深度放结构体里确实比较容易处理 

代码：
```cpp
#include <cstdio>
#include <algorithm>
#include <queue>
#include <cstdlib>
#include <cstring>
using namespace std;
const int INF=0x3f3f3f3f;
char mp[15][15];
bool vis[15][15];
int ans;
int t,n,m;
struct node{
    int i;
    int j;
    int d;
    node(int _i,int _j,int _d):i(_i),j(_j),d(_d){}
};
bool check(int i,int j){
    if(i<0 || i>=n || j<0 || j>=m || vis[i][j] || mp[i][j]=='.'){
        return false;
    }
    else{
        return true;
    }
}
bool judge(){
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(mp[i][j]=='#' && !vis[i][j]){
                return false;
            }
        }
    }
    return true;
}
int bfs(int i,int j,int k,int l){
    queue<node> que;
    que.push(node(i,j,0));
    que.push(node(k,l,0));
    vis[i][j]=vis[k][l]=true;
    int time=0;
    while(!que.empty()){
        node tmp=que.front();
        que.pop();
        int ti=tmp.i;
        int tj=tmp.j;
        int td=tmp.d;
        time=max(time,td);
        if(check(ti+1,tj)){
            que.push(node(ti+1,tj,td+1));
            vis[ti+1][tj]=true;
        }
        if(check(ti-1,tj)){
            que.push(node(ti-1,tj,td+1));
            vis[ti-1][tj]=true;
        }
        if(check(ti,tj+1)){
            que.push(node(ti,tj+1,td+1));
            vis[ti][tj+1]=true;
        }
        if(check(ti,tj-1)){
            que.push(node(ti,tj-1,td+1));
            vis[ti][tj-1]=true;
        }
    }
    if(judge()){
        return time;
    }
    else{
        return INF;
    }
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d",&t);
    int c=1;
    while(t--){
        scanf("%d%d",&n,&m);
        memset(mp,'\0',sizeof(mp));
        memset(vis,false,sizeof(vis));
        for(int i=0;i<n;i++){
            scanf("%s",mp[i]);
        }
        int ans=INF;
        //枚举双起点
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                for(int k=0;k<n;k++){
                    for(int l=0;l<m;l++){
                        if(mp[i][j]=='.' || mp[k][l]=='.'){
                            continue;
                        }
                        memset(vis,false,sizeof(vis));
                        ans=min(ans,bfs(i,j,k,l));
                    }
                }
            }
        }
        if(ans==INF){
            ans=-1;
        }
        printf("Case %d: %d\n",c++,ans);
    }
    return 0;
}
```









