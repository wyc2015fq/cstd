# Let's go home（HDU-1824） - Alex_McAvoy的博客 - CSDN博客





2018年12月12日 19:31:40[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60
个人分类：[HDU																[图论——2-SAT](https://blog.csdn.net/u011815404/article/category/8348292)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

小时候，乡愁是一枚小小的邮票，我在这头，母亲在那头。

                        —— 余光中

集训是辛苦的，道路是坎坷的，休息还是必须的。经过一段时间的训练，lcy决定让大家回家放松一下，但是训练还是得照常进行，lcy想出了如下回家规定，每一个队（三人一队）或者队长留下或者其余两名队员同时留下；每一对队员，如果队员A留下，则队员B必须回家休息下，或者B留下，A回家。由于今年集训队人数突破往年同期最高记录，管理难度相当大，lcy也不知道自己的决定是否可行，所以这个难题就交给你了，呵呵，好处嘛~，免费**漂流一日。

# **Input**

第一行有两个整数，T和M，1<=T<=1000表示队伍数，1<=M<=5000表示对数。

接下来有T行，每行三个整数，表示一个队的队员编号，第一个队员就是该队队长。

然后有M行，每行两个整数，表示一对队员的编号。

每个队员只属于一个队。队员编号从0开始。

# Output

可行输出yes，否则输出no，以EOF为结束。

# Sample Input

**1 20 1 20 11 2**

**2 40 1 23 4 50 30 41 31 4**

# Sample Output

**yesno**


思路：每个人有两种选择，根据题意要满足两种条件：

1）队长留 或 两个队员留

2）由 M 指出的一对队员 a、b 的冲突条件

假设 a、b、c 三个组成一队，a 是队长，那么由条件 1 可知 队长 a 与队员 b、c 二者只能选一种，假设留为 1 走为 0，则：

对于任一人走：
- a 走，导致 b、c 留：<a,0,b,1>、<a,0,c,1>
- b 走，导致 a 留，c 走：<b,0,a,1>、<b,0,c,0>
- c 走，导致 a 留，b 走：<c,0,a,1>、<c,0,b,0>

对于任一人留：
- a 留，导致 b、c 走：<a,1,b,0>、<a,1,c,0>
- b 留，导致 a 走，c 留：<b,1,a,0>、<b,1,c,1>
- c 留，导致 a 走，b 留：<c,1,a,0>、<c,1,b,1>

对于 M 个条件：
- a 留，导致 b 走：<a,1,b,0>
- b 留，导致 a 走：<b,1,a,0>

根据以上关系，添加且关系至 2-SAT 中判断即可

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
#define N 100001
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
void addOrClause(int x,int xVal,int y,int yVal){
    x=x*2+xVal;
    y=y*2+yVal;
    G[x^1].push_back(y);
    G[y^1].push_back(x);
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
                while(top>0)
                    vis[Stack[--top]]=false;

                if(!dfs(i+1))
                    return false;
            }
        }
    }
    return true;
}
int main(){
    int t,m;
    while(scanf("%d%d",&t,&m)!=EOF&&(t+m)){
        init(t*3);
        for(int i=0;i<t;i++){
            int a,b,c;
            scanf("%d%d%d",&a,&b,&c);

            //a走，导致b、c留
            addAndClause(a,0,b,1);
            addAndClause(a,0,c,1);
            //b走，导致a留c走
            addAndClause(b,0,a,1);
            addAndClause(b,0,c,0);
            //c走，导致a留b走
            addAndClause(c,0,a,1);
            addAndClause(c,0,b,0);

            //a 留，导致b、c走
            addAndClause(a,1,b,0);
            addAndClause(a,1,c,0);
            //b留，导致a走c留
            addAndClause(b,1,a,0);
            addAndClause(b,1,c,1);
            //c留，导致a走b留
            addAndClause(c,1,a,0);
            addAndClause(c,1,b,1);

        }

        while(m--){
            int a,b;
            scanf("%d%d",&a,&b);

            //a留，导致b走
            addAndClause(a,1,b,0);
            //b留，导致a走
            addAndClause(b,1,a,0);
        }

        printf("%s\n",twoSAT(t*3)?"yes":"no");
    }
    return 0;
}
```






