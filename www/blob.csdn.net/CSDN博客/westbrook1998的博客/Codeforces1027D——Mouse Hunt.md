# Codeforces1027D——Mouse Hunt - westbrook1998的博客 - CSDN博客





2018年08月28日 13:10:55[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：82








> 
Medicine faculty of Berland State University has just finished their admission campaign. As usual, about 80% of applicants are girls and majority of them are going to live in the university dormitory for the next 4 (hopefully) years. 

  The dormitory consists of n rooms and a single mouse! Girls decided to set mouse traps in some rooms to get rid of the horrible monster. Setting a trap in room number i costs ci burles. Rooms are numbered from 1 to n. 

  Mouse doesn’t sit in place all the time, it constantly runs. If it is in room i in second t then it will run to room ai in second t+1 without visiting any other rooms inbetween (i=ai means that mouse won’t leave room i). It’s second 0 in the start. If the mouse is in some room with a mouse trap in it, then the mouse get caught into this trap. 

  That would have been so easy if the girls actually knew where the mouse at. Unfortunately, that’s not the case, mouse can be in any room from 1 to n at second 0. 

  What it the minimal total amount of burles girls can spend to set the traps in order to guarantee that the mouse will eventually be caught no matter the room it started from? 

  Input 

  The first line contains as single integers n (1≤n≤2⋅105) — the number of rooms in the dormitory. 

  The second line contains n integers c1,c2,…,cn (1≤ci≤104) — ci is the cost of setting the trap in room number i. 

  The third line contains n integers a1,a2,…,an (1≤ai≤n) — ai is the room the mouse will run to the next second after being in room i. 

  Output 

  Print a single integer — the minimal total amount of burles girls can spend to set the traps in order to guarantee that the mouse will eventually be caught no matter the room it started from. 

  Examples 

  Input 

  5 

  1 2 3 2 10 

  1 3 4 3 3 

  Output 

  3 

  Input 

  4 

  1 10 2 10 

  2 4 2 2 

  Output 

  10 

  Input 

  7 

  1 1 1 1 1 1 1 

  2 2 2 3 6 7 6 

  Output 

  2 

  Note 

  In the first example it is enough to set mouse trap in rooms 1 and 4. If mouse starts in room 1 then it gets caught immideately. If mouse starts in any other room then it eventually comes to room 4. 

  In the second example it is enough to set mouse trap in room 2. If mouse starts in room 2 then it gets caught immideately. If mouse starts in any other room then it runs to room 2 in second 1. 

  Here are the paths of the mouse from different starts from the third example: 

  1→2→2→…; 

  2→2→…; 

  3→2→2→…; 

  4→3→2→2→…; 

  5→6→7→6→…; 

  6→7→6→…; 

  7→6→7→…; 

  So it’s enough to set traps in rooms 2 and 6.
这其实是个有向图的问题 老鼠能从一个房间走到另一个房间 而且必定有环 而且每个点都能连到一个环  因为如果出现1->2->3  就不符合条件了 因为每个点都有一条边连向另一个点 

然后dfs找环 找的过程中记录最小权值

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=200050;
const int INF=0x3f3f3f;
int n;
int a[N];
int c[N];
int vis[N];
int ans;
int pp;
int _min;
void dfs(int p){
    //正在栈中访问
    if(vis[p]==1){
        if(pp==p){
            //找到一个环，更新答案
            ans+=_min;
        }
        //这是开始的第一个节点
        else if(pp==-1){
            pp=p;
            _min=c[p];
            dfs(a[p]);
        }
        else{
            //更新环上最小值
            _min=min(_min,c[p]);
            dfs(a[p]);
        }
        //p已访问
        vis[p]=2;
    }
    //开始访问
    else if(vis[p]==0){
        vis[p]=1;
        dfs(a[p]);
        vis[p]=2;
    }
    return;
}
int main(void){
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        scanf("%d",&c[i]);
    }
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
    }
    for(int i=1;i<=n;i++){
        if(!vis[i]){
            //以i为起点搜索，因此i的前驱节点不存在，为-1
            pp=-1;
            _min=INF;
            dfs(i);
        }
    }
    printf("%d\n",ans);
    return 0;
}
```





