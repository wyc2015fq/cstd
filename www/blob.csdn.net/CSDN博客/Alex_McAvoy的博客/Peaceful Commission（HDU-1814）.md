# Peaceful Commission（HDU-1814） - Alex_McAvoy的博客 - CSDN博客





2018年12月15日 21:55:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：73








> 
# Problem Description

The Public Peace Commission should be legislated in Parliament of The Democratic Republic of Byteland according to The Very Important Law. Unfortunately one of the obstacles is the fact that some deputies do not get on with some others. 

The Commission has to fulfill the following conditions: 

1.Each party has exactly one representative in the Commission, 

2.If two deputies do not like each other, they cannot both belong to the Commission. 

Each party has exactly two deputies in the Parliament. All of them are numbered from 1 to 2n. Deputies with numbers 2i-1 and 2i belong to the i-th party . 

Task 

Write a program, which: 

1.reads from the text file SPO.IN the number of parties and the pairs of deputies that are not on friendly terms, 

2.decides whether it is possible to establish the Commission, and if so, proposes the list of members, 

3.writes the result in the text file SPO.OUT. 

# **Input**

In the first line of the text file SPO.IN there are two non-negative integers n and m. They denote respectively: the number of parties, 1 <= n <= 8000, and the number of pairs of deputies, who do not like each other, 0 <= m <=2 0000. In each of the following m lines there is written one pair of integers a and b, 1 <= a < b <= 2n, separated by a single space. It means that the deputies a and b do not like each other. 

There are multiple test cases. Process to end of file. 

# Output

The text file SPO.OUT should contain one word NIE (means NO in Polish), if the setting up of the Commission is impossible. In case when setting up of the Commission is possible the file SPO.OUT should contain n integers from the interval from 1 to 2n, written in the ascending order, indicating numbers of deputies who can form the Commission. Each of these numbers should be written in a separate line. If the Commission can be formed in various ways, your program may write mininum number sequence.

# Sample Input

**3 21 32 4**

# Sample Output

**145**


题意：n 个党派，每个党派有 2 个代表，现在需要从每个党派中选出 1 个代表，构成一具有 n 个人的委员会，但有一些代表相互仇恨，不能同时出现在委员会中，现在问是否有一合理方案，若有则输出最小字典序结果

思路：将每个党派看做一个点，对于 a 代表与 b 代表不和，直接将其加入限制条件中即可，输出时，从小到大依次输出即可

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
#define N 10001
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
            scanf("%d%d",&a,&b);
            a--,b--;

            G[a].push_back(b^1);
            G[b].push_back(a^1);
        }

        if(!twoSAT(n))
            printf("NIE\n");
        else{
            for(int i=0;i<n*2;i++)
                if(vis[i])
                    printf("%d\n",i+1);
        }
    }

    return 0;
}
```





