# poj3026——Borg Maze - westbrook1998的博客 - CSDN博客





2018年08月14日 13:28:54[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：20标签：[bfs																[最小生成树](https://so.csdn.net/so/search/s.do?q=最小生成树&t=blog)](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
The Borg is an immensely powerful race of enhanced humanoids from the delta quadrant of the galaxy. The Borg collective is the term used to describe the group consciousness of the Borg civilization. Each Borg individual is linked to the collective by a sophisticated subspace network that insures each member is given constant supervision and guidance.  

  Your task is to help the Borg (yes, really) by developing a program which helps the Borg to estimate the minimal cost of scanning a maze for the assimilation of aliens hiding in the maze, by moving in north, west, east, and south steps. The tricky thing is that the beginning of the search is conducted by a large group of over 100 individuals. Whenever an alien is assimilated, or at the beginning of the search, the group may split in two or more groups (but their consciousness is still collective.). The cost of searching a maze is definied as the total distance covered by all the groups involved in the search together. That is, if the original group walks five steps, then splits into two groups each walking three steps, the total distance is 11=5+3+3. 

  Input 

  On the first line of input there is one integer, N <= 50, giving the number of test cases in the input. Each test case starts with a line containg two integers x, y such that 1 <= x,y <= 50. After this, y lines follow, each which x characters. For each character, a space `'' stands for an open space, a hash mark`#” stands for an obstructing wall, the capital letter `A'' stand for an alien, and the capital letter`S” stands for the start of the search. The perimeter of the maze is always closed, i.e., there is no way to get out from the coordinate of the “S”. At most 100 aliens are present in the maze, and everyone is reachable. 

  Output 

  For every test case, output one line containing the minimal cost of a succesful search of the maze leaving no aliens alive. 

  Sample Input
```
2
6 5
##### 
#A#A##
# # A#
#S  ##
##### 
7 7
#####  
#AAA###
#    A#
# S ###
#     #
#AAA###
#####  
Sample Output
8
11
```

早上这题 Wa到爆炸 

最后看了题解发现是样例的坑，就是在读取字符串的时候坑了 

思路就是先bfs求各个点之间的距离，然后最小生成树一波
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <map>
#include <string>
#define _clr(x,a) memset(x,a,sizeof(x))
using namespace std;
const int N=120;
const int INF=0x3f3f3f3f;
int t,x,y;
string mp[N];
int g[N][N];
bool vis[N][N];
bool used[N];
int dir[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
int low[N];
int cnt;
int c[N][N];
struct node{
    int i;
    int j;
    int s;
    node(int _i,int _j,int _s):i(_i),j(_j),s(_s){}
};
void init(){
    _clr(g,INF);
    for(int i=0;i<N;i++){
        mp[i]="";
    }
    _clr(c,0);
}
//以(i,j)为起点bfs
void bfs(int i,int j){
    int tidx=c[i][j];
    queue<node> que;
    que.push(node(i,j,0));
    vis[i][j]=true;
    while(!que.empty()){
        node tmp=que.front();
        que.pop();
        int ti=tmp.i;
        int tj=tmp.j;
        int ts=tmp.s;
        //printf("%d %d %d\n",ti,tj,ts);
        for(int i=0;i<4;i++){
            int tti=ti+dir[i][0];
            int ttj=tj+dir[i][1];

            if(tti<0 || tti>y || ttj<0 || ttj>x || vis[tti][ttj] || mp[tti][ttj]=='#'){
                continue;
            }
            //printf("----%d %d %c\n",tti,ttj,mp[tti][ttj]);

            que.push(node(tti,ttj,ts+1));
            vis[tti][ttj]=true;
            if(mp[tti][ttj]=='S' || mp[tti][ttj]=='A'){
                int idx=c[tti][ttj];
                //printf("zheli %d %d\n",tidx,idx);
                g[tidx][idx]=g[idx][tidx]=ts+1;
            }
        }
    }
}
int prim(){
    int ans=0;
    int Min=INF;
    int pos=1;
    _clr(used,false);
    _clr(low,0);
    used[pos]=true;
    for(int i=1;i<=cnt;i++){
        if(i!=pos){
            low[i]=g[pos][i];
        }
    }
    for(int i=1;i<cnt;i++){
        Min=INF;
        for(int j=1;j<=cnt;j++){
            if(!used[j] && low[j]<Min){
                Min=low[j];
                pos=j;
            }
        }
        used[pos]=true;
        ans+=Min;
        //printf("%d %d %d\n",Min,ans,pos);
        for(int j=1;j<=cnt;j++){
            if(!used[j] && low[j]>g[pos][j]){
                low[j]=g[pos][j];
            }
        }
    }
    return ans;
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d",&t);
    while(t--){
        init();
        cnt=1;
        scanf("%d%d",&x,&y);
        getline(cin,mp[0]);
        for(int i=0;i<y;i++){
            getline(cin,mp[i]);
            //cout << mp[i] << endl;
            for(int j=0;j<x;j++){
                //printf("%c\n",mp[i][j]);
                if(mp[i][j]=='S' || mp[i][j]=='A'){
                    c[i][j]=cnt++;
                }
            }
        }
        cnt--;
        for(int i=0;i<y;i++){
            for(int j=0;j<x;j++){
                if(c[i][j]!=0){
                    _clr(vis,false);
                    bfs(i,j);
                }
            }
        }
        // for(int i=1;i<=cnt;i++){
        //     for(int j=1;j<=cnt;j++){
        //         printf("%d ",g[i][j]);
        //     }
        //     printf("\n");
        // }
        int ans=prim();
        printf("%d\n",ans);
    }
    return 0;
}
```







