# Party（HDU-3062） - Alex_McAvoy的博客 - CSDN博客





2018年12月12日 20:22:19[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：39








> 
# Problem Description

有n对夫妻被邀请参加一个聚会，因为场地的问题，每对夫妻中只有1人可以列席。在2n 个人中，某些人之间有着很大的矛盾（当然夫妻之间是没有矛盾的），有矛盾的2个人是不会同时出现在聚会上的。有没有可能会有n 个人同时列席？

# **Input**

n： 表示有n对夫妻被邀请 (n<= 1000)

m： 表示有m 对矛盾关系 ( m < (n - 1) * (n -1))

在接下来的m行中，每行会有4个数字，分别是 A1,A2,C1,C2 

A1,A2分别表示是夫妻的编号 

C1,C2 表示是妻子还是丈夫 ，0表示妻子 ，1是丈夫

夫妻编号从 0 到 n -1 

# Output

如果存在一种情况 则输出YES 

否则输出 NO 

# Sample Input

**2 10 1 1 1 **

# Sample Output

**YES**


思路：每对夫妻代表图中一个结点，只有 1、0 两种选择，对于有矛盾的夫妻对，使其不列席，让无矛盾夫妻对的列席即可

对于 m 矛盾关系，设 a、b 两对夫妇存在矛盾：
- 若第 a 对的妻子与第 b 对的妻子有矛盾（a b 0 0）

	则 a 的妻子去了 b 的丈夫必须去，b 的妻子去了 a 的丈夫必须去：<a,0,b,1>、<b,0,a,1>
- 若第 a 对的妻子与第 b 对的丈夫有矛盾（a b 0 1）

	则 a 的妻子去了 b 的妻子必须去，b 的丈夫去了 a 的丈夫必须去：<a,0,b,0>、<b,1,a,1>
- 若第 a 对的丈夫与第 b 对的妻子有矛盾（a b 1 0）

	则 a 的丈夫去了 b 的丈夫必须去，b 的妻子去了 a 的妻子必须去：<a,1,b,1>、<b,0,a,0,>
- 若第 a 对的丈夫与第 b 对的丈夫有矛盾（a b 1 1）

	则 a 的丈夫去了 b 的妻子必须去，b 的丈夫去了 a 的妻子必须去：<a,1,b,0>、<b,1,a,0>

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
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        init(n);
        while(m--){
            int a,b;
            int aVal,bVal;
            scanf("%d%d%d%d",&a,&b,&aVal,&bVal);

            if(aVal==0&&bVal==0){
                addAndClause(a,0,b,1);
                addAndClause(b,0,a,1);
            }

            if(aVal==0&&bVal==1){
                addAndClause(a,0,b,0);
                addAndClause(b,1,a,1);
            }

            if(aVal==1&&bVal==0){
                addAndClause(a,1,b,1);
                addAndClause(b,0,a,0);
            }

            if(aVal==1&&bVal==1){
                addAndClause(a,1,b,0);
                addAndClause(b,1,a,0);
            }
        }

        printf("%s\n",twoSAT(n)?"YES":"NO");
    }


    return 0;
}
```





