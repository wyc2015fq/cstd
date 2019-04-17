# Battle ships（HDU-5093） - Alex_McAvoy的博客 - CSDN博客





2019年01月18日 12:07:01[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：126
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Dear contestant, now you are an excellent navy commander, who is responsible of a tough mission currently. 

Your fleet unfortunately encountered an enemy fleet near the South Pole where the geographical conditions are negative for both sides. The floating ice and iceberg blocks battleships move which leads to this unexpected engagement highly dangerous, unpredictable and incontrollable. 

But, fortunately, as an experienced navy commander, you are able to take opportunity to embattle the ships to maximize the utility of cannons on the battleships before the engagement. 

The target is, arrange as many battleships as you can in the map. However, there are three rules so that you cannot do that arbitrary: 

A battleship cannot lay on floating ice 

A battleship cannot be placed on an iceberg 

Two battleships cannot be arranged in the same row or column, unless one or more icebergs are in the middle of them. 

# Input

There is only one integer T (0<T<12) at the beginning line, which means following T test cases. 

For each test case, two integers m and n (1 <= m, n <= 50) are at the first line, represents the number of rows and columns of the battlefield map respectively. Following m lines contains n characters iteratively, each character belongs to one of ‘#’, ‘*’, ‘o’, that symbolize iceberg, ordinary sea and floating ice.

# Output

For each case, output just one line, contains a single integer which represents the maximal possible number of battleships can be arranged.

# Sample Input

**24 4*oooo###**#*ooo*4 4#****#****#*ooo#**

# **Sample Output**

**35**


题意：有一个 n*m 的图，* 代表空白海域，# 代表冰山，o 代表浮冰，在海域上布置战船，要求战船不能放到浮冰、冰山上，每一行每一列只能布置一艘船，除非中间有冰山隔开，问最多能布置多少艘

思路：与 [Fire Net（HDU-1045）](https://blog.csdn.net/u011815404/article/details/88541330)很像，只是多出浮冰不能布置船的情况，建图后跑一遍匈牙利即可

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
#define E 1e-6
#define MOD 1000003
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int n,m;
char s[N][N];
int x[N][N],y[N][N];
int cntX,cntY;
bool G[N][N];
int link[N];
bool vis[N];
bool dfs(int u){
    for(int v=0;v<cntY;v++){
        if(G[u][v]&&!vis[v]){
            vis[v]=true;
            if(link[v]==-1 || dfs(link[v])){
                link[v]=u;
                return true;
            }
        }
    }
    return false;
}
int hungarian(){
    int res=0;
    for(int i=0;i<cntX;i++){
        memset(vis,false,sizeof(vis));
        if(dfs(i))
            res++;
    }
    return res;
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%d%d",&n,&m);
        for(int i=0;i<n;i++)
            scanf("%s",s[i]);

        memset(x,0,sizeof(x));
        memset(y,0,sizeof(y));

        cntX=1;
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                if(s[i][j]=='*')
                    x[i][j]=cntX;
                if(s[i][j]=='#')
                    cntX++;
            }
            cntX++;
        }

        cntY=1;
        for(int j=0;j<m;j++){
            for(int i=0;i<n;i++){
                if(s[i][j]=='*')
                    y[i][j]=cntY;
                if(s[i][j]=='#')
                    cntY++;
            }
            cntY++;
        }

        memset(G,false,sizeof(G));
        memset(link,-1,sizeof(link));

        for(int i=0;i<n;i++)
            for(int j=0;j<m;j++)
                if(s[i][j]=='*')
                    G[x[i][j]][y[i][j]]=true;

        printf("%d\n",hungarian());
    }
    return 0;
}
```






