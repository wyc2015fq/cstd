# Katu Puzzle（POJ-3678） - Alex_McAvoy的博客 - CSDN博客





2018年12月12日 20:52:42[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60
个人分类：[POJ																[图论——2-SAT](https://blog.csdn.net/u011815404/article/category/8348292)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Katu Puzzle is presented as a directed graph G(V, E) with each edge e(a, b) labeled by a boolean operator op (one of AND, OR, XOR) and an integer c (0 ≤ c ≤ 1). One Katu is solvable if one can find each vertex Vi a value Xi (0 ≤ Xi ≤ 1) such that for each edge e(a, b) labeled by op and c, the following formula holds:

 Xa op Xb = c

The calculating rules are:
||AND|0|1||----|----|----||0|0|0||1|0|1||AND|0|1|0|0|0|1|0|1||OR|0|1||----|----|----||0|0|1||1|1|1||OR|0|1|0|0|1|1|1|1||XOR|0|1||----|----|----||0|0|1||1|1|0||XOR|0|1|0|0|1|1|1|0|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|AND|0|1| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|0|0|0| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|1|0|1| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|OR|0|1| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|0|0|1| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|1|1|1| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|XOR|0|1| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|0|0|1| | | | | | | | | | | | | | | | | | | | | | | | | | | |
|1|1|0| | | | | | | | | | | | | | | | | | | | | | | | | | | |

Given a Katu Puzzle, your task is to determine whether it is solvable.

# **Input**

The first line contains two integers N (1 ≤ N ≤ 1000) and M,(0 ≤ M ≤ 1,000,000) indicating the number of vertices and edges.

The following M lines contain three integers a (0 ≤ a < N), b(0 ≤ b < N), c and an operator op each, describing the edges.

# Output

Output a line containing "YES" or "NO".

# Sample Input

**4 40 1 1 AND1 2 1 OR3 2 0 AND3 0 0 XOR**

# Sample Output

**YES**


题意：给出一 n 个点 m 条边的有向图，每个点只能取 0、1，现在每条边被一个操作符 or、and、xor 以及一个值标记了，表示 a、b 按操作符运算的结果，问这个有向图是否有可行解

思路：根据题意，该题是一个 2-SAT 问题，将操作符进行转换，进行判定即可

对于 A[x]，可以通过连边 <x',x> 实现，NOT A[x]，可以通过连边 <x,x'> 来实现，对于 NOT(A[x] AND A[y]) 需要连两条边 <x, y'> 和 <y, x'> 来实现，对于 A[x] OR A[y] 需要连两条边 <x', y> 和 <y', x> 来实现

故对于 and、or、xor 三种运算有：

and 运算：
- a and b = 0 时：若 a=1，则必定满足 b=0；若 b=1，则必定满足 a=0，即：<a,1,b,0>、<b,1,a,0>
- a and b = 1 时：a=1 且 b=1，即：<a,0,a,1>、<b,0,b,1>

or 运算：
- a or b = 0 时：a = 0 且 b = 0，即：<a,1,a,0>、<b,1,b,0> 
- a or b = 1 时：若 a=0，则必定满足 b=1；若 b=0，则必定满足 a=0，即：<a,0,b,1>、<b,0,a,1>

xor 运算：
- a xor b = 0 时，有以下四种情况：

	若 a = 0，则必定满足 b = 0，即：<a,0,b,0>

	若 b = 0，则必定满足 a = 0，即：<b,0,a,0>

	若 a = 1，则必定满足 b = 1，即：<a,1,b,1>

	若 b = 1，则必定满足 a = 1，即：<b,1,a,1>
- a xor b = 1 时，有以下四种情况：

	若 a = 0，则必定满足 b = 1，即：<a,0,b,1>

	若 b = 0，则必定满足 a = 1，即：<b,0,a,1>

	若 a = 1，则必定满足 b = 0，即：<a,1,b,0>

	若 b = 1，则必定满足 a = 0，即：<b,1,a,0>

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
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
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
            int a,b,c;
            char ch[10];
            scanf("%d%d%d%s",&a,&b,&c,ch);

            if(ch[0]=='A'){//and 运算
                if(c==0){//a and b=0
                    addAndClause(a,1,b,0);//若 a=1，则必定满足 b=0
                    addAndClause(b,1,a,0);//若 b=1，则必定满足 a=0
                }
                else{//a and b=1
                    addAndClause(a,0,a,1);//a=1
                    addAndClause(b,0,b,1);//b=1
                }
            }
            else if(ch[0]=='O'){//or 运算
                if(c==0){//a or b=0
                    addAndClause(a,1,a,0);//a=0
                    addAndClause(b,1,b,0);//b=0
                }
                else{//a or b=1
                    addAndClause(a,0,b,1);//若 a=0，则必定满足 b=1
                    addAndClause(b,0,a,1);//若 b=0，则必定满足 a=0
                }
            }
            else if(ch[0]=='X'){//xor 运算
                if(c==0){//a xor b=0
                    addAndClause(a,0,b,0);//若 a = 0，则必定满足 b = 0
                    addAndClause(b,0,a,0);//若 b = 0，则必定满足 a = 0
                    addAndClause(a,1,b,1);//若 a = 1，则必定满足 b = 1
                    addAndClause(b,1,a,1);//若 b = 1，则必定满足 a = 1
                }
                else{//a xor b=1
                    addAndClause(a,0,b,1);//若 a = 0，则必定满足 b = 1
                    addAndClause(b,0,a,1);//若 b = 0，则必定满足 a = 1
                    addAndClause(a,1,b,0);//若 a = 1，则必定满足 b = 0
                    addAndClause(b,1,a,0);//若 b = 1，则必定满足 a = 0
                }
            }
        }

        printf("%s\n",twoSAT(n)?"YES":"NO");
    }

    return 0;
}
```






