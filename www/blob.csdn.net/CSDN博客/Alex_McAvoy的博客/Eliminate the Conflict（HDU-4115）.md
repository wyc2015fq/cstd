# Eliminate the Conflict（HDU-4115） - Alex_McAvoy的博客 - CSDN博客





2018年12月12日 18:38:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：50








> 
# Problem Description

Conflicts are everywhere in the world, from the young to the elderly, from families to countries. Conflicts cause quarrels, fights or even wars. How wonderful the world will be if all conflicts can be eliminated.

Edward contributes his lifetime to invent a 'Conflict Resolution Terminal' and he has finally succeeded. This magic item has the ability to eliminate all the conflicts. It works like this:

If any two people have conflict, they should simply put their hands into the 'Conflict Resolution Terminal' (which is simply a plastic tube). Then they play 'Rock, Paper and Scissors' in it. After they have decided what they will play, the tube should be opened and no one will have the chance to change. Finally, the winner have the right to rule and the loser should obey it. Conflict Eliminated!

But the game is not that fair, because people may be following some patterns when they play, and if the pattern is founded by others, the others will win definitely.

Alice and Bob always have conflicts with each other so they use the 'Conflict Resolution Terminal' a lot. Sadly for Bob, Alice found his pattern and can predict how Bob plays precisely. She is very kind that doesn't want to take advantage of that. So she tells Bob about it and they come up with a new way of eliminate the conflict:

They will play the 'Rock, Paper and Scissors' for N round. Bob will set up some restricts on Alice.

But the restrict can only be in the form of "you must play the same (or different) on the ith and jth rounds". If Alice loses in any round or break any of the rules she loses, otherwise she wins.

Will Alice have a chance to win?

# **Input**

The first line contains an integer T(1 <= T <= 50), indicating the number of test cases.

Each test case contains several lines.

The first line contains two integers N,M(1 <= N <= 10000, 1 <= M <= 10000), representing how many round they will play and how many restricts are there for Alice.

The next line contains N integers B1,B2, ...,BN, where Bi represents what item Bob will play in the ith round. 1 represents Rock, 2 represents Paper, 3 represents Scissors.

The following M lines each contains three integers A,B,K(1 <= A,B <= N,K = 0 or 1) represent a restrict for Alice. If K equals 0, Alice must play the same on Ath and Bth round. If K equals 1, she must play different items on Ath and Bthround.

# Output

For each test case in the input, print one line: "Case #X: Y", where X is the test case number (starting with 1) and Y is "yes" or "no" represents whether Alice has a chance to win.

# Sample Input

**23 31 1 11 2 11 3 12 3 15 51 2 3 2 11 2 11 3 11 4 11 5 12 3 0**

# Sample Output

**Case #1: noCase #2: yes**


题意：两个人玩剪刀石头布，Alice 已经知道 Bob 每次要出什么，一共玩 n 轮，首先给出 n 个数分别表示 Bob 每轮出拳，其中 1 代表剪刀，2 代表石头，3 代表布，Bob 对 Alice 做出一些限制：

给出 m 行，每行为 a、b、k，如果 k 是 0，代表 Alice 第 a 次与 b 次 出拳必须相同，如果 k 是 1，代表 Alice 第 a 次与第 b 次出拳必须不相同

一旦 Alice 破坏限制规则，或者输了一局，就代表 Alice 彻底输了，求根据所给输入判断 Alice 能否获胜

思路：由于 Alice 知道 Bob 的所有出拳结果，因此可用数组 a[N][2] 来保存出拳结果，然后对于 m 行要求作出 Alice 在第 i 轮选 0 还是 1 的推断即可，看是否有方法满足 n 个解

由于 Alice 一次也不能输，因此根据 Bob 的出拳，Alice 只能赢或者平局，即每次只有 2 种选择结果，由于有 3 种可供选择的情况，因此存在一些矛盾对：

假设第 a 次可以出 a1、a2，第 b 次可以出 b1、b2

1）若 k=0，要求 a、b 出拳相同，则：
- 出拳 a1、b1 不同，矛盾，建立 <a1,b2>、<b1,a2>
- 出拳 a1、b2 不同，矛盾，建立 <a1,b1>、<b2,a2>
- 出拳 a2、b1 不同，矛盾，建立 <a2,b2>、<b1,a1>
- 出拳 a2、b2 不同，矛盾，建立 <a2,b1>、<b2,a1>

2）若 k=1，要求 a、b 出拳不同，则：
- 出拳 a1、b1 相同，矛盾，建立 <a1,b2>、 <b1,a2>
- 出拳 a1、b2 相同，矛盾，建立 <a1,b1>、 <b2,a2>
- 出拳 a2、b1 相同，矛盾，建立 <a2,b2>、 <b1,a1>
- 出拳 a2、b2 相同，矛盾，建立 <a2,b1>、 <b2,a1>

根据如上规则，填入数据，进行 2-SAT 判断即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
bool vis[N*2];
int Stack[N*2],top;
vector<int> G[N*2];
void init(int n){
    memset(vis,false,sizeof(vis));
    for(int i=0;i<2*n;i++)
        G[i].clear();
}
void addAndClause(int x,int xval,int y,int yval) {
    x=x*2+xval;
    y=y*2+yval;
    G[x].push_back(y);
}
bool dfs(int x){
    if(vis[x^1])
        return false;

    if(vis[x])
        return true;
    vis[x]=true;

    Stack[top++]=x;

    for(int i=0;i<G[x].size();i++)
        if(!dfs(G[x][i]))
            return false;

    return true;
}
bool twoSAT(int n){
    for(int i=0;i<2*n;i+=2){
        if(!vis[i] && !vis[i+1]){
            top=0;

            if(!dfs(i)){
                while(top>0)/
                    vis[Stack[--top]]=false;

                if(!dfs(i+1))/
                    return false;
            }
        }
    }
    return true;
}
int a[N][2];
int main(){
    int t;
    int Case=1;
    scanf("%d",&t);

    while(t--){
        int n,m;
        scanf("%d%d",&n,&m);

        init(n);
        for(int i=0;i<n;i++){
            scanf("%d",&a[i][0]);//Bob出拳的结果

            //Alice可能出拳的结果
            a[i][0]--;//与Bob打平,出拳情况1
            a[i][1]=(a[i][0]+1)%3;//取胜Bob，出拳情况2
        }

        while(m--){
            int x,y,k;
            scanf("%d%d%d",&x,&y,&k);
            x--,y--;

            if(k==0){//相同
                if(a[x][0]!=a[y][0]){//a1、b1不同
                    addAndClause(x,0,y,1);//建立<a1,b2>
                    addAndClause(y,0,x,1);//建立<b1,a2>
                }
                if(a[x][0]!=a[y][1]){//a1、b2不同
                    addAndClause(x,0,y,0);//建立<a1,b1>
                    addAndClause(y,1,x,1);//建立<b2,a2>
                }
                if(a[x][1]!=a[y][0]){//a2、b1不同
                    addAndClause(x,1,y,1);//建立<a2,b2>
                    addAndClause(y,0,x,0);//建立<b1,a1>
                }
                if(a[x][1]!=a[y][1]){//a2、b2不同
                    addAndClause(x,1,y,0);//建立<a2,b1>
                    addAndClause(y,1,x,0);//建立<b2,a1>
                }
            }
            else if(k==1){//不同
                if(a[x][0]==a[y][0]){//a1、b1相同
                    addAndClause(x,0,y,1);//建立<a1,b2>
                    addAndClause(y,0,x,1);//建立<b1,a2>
                }
                if(a[x][0]==a[y][1]){//a1、b2相同
                    addAndClause(x,0,y,0);//建立<a1,b1>
                    addAndClause(y,1,x,1);//建立<b2,a2>
                }
                if(a[x][1]==a[y][0]){//a2、b1相同
                    addAndClause(x,1,y,1);//建立<a2,b2>
                    addAndClause(y,0,x,0);//建立<b1,a1>
                }
                if(a[x][1]==a[y][1]){//a2、b2相同
                    addAndClause(x,1,y,0);//建立<a2,b1>
                    addAndClause(y,1,x,0);//建立<b2,a1>
                }
            }
        }

        printf("Case #%d: %s\n",Case++,twoSAT(n)?"yes":"no");
    }
    return 0;
}
```





