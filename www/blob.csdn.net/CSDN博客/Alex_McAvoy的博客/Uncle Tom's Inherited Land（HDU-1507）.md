# Uncle Tom's Inherited Land（HDU-1507） - Alex_McAvoy的博客 - CSDN博客





2019年03月13日 08:24:22[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：36
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Your old uncle Tom inherited a piece of land from his great-great-uncle. Originally, the property had been in the shape of a rectangle. A long time ago, however, his great-great-uncle decided to divide the land into a grid of small squares. He turned some of the squares into ponds, for he loved to hunt ducks and wanted to attract them to his property. (You cannot be sure, for you have not been to the place, but he may have made so many ponds that the land may now consist of several disconnected islands.) 

Your uncle Tom wants to sell the inherited land, but local rules now regulate property sales. Your uncle has been informed that, at his great-great-uncle's request, a law has been passed which establishes that property can only be sold in rectangular lots the size of two squares of your uncle's property. Furthermore, ponds are not salable property. 

Your uncle asked your help to determine the largest number of properties he could sell (the remaining squares will become recreational parks). 

![](https://img-blog.csdnimg.cn/20190313073036343.jpg)

# **Input**

Input will include several test cases. The first line of a test case contains two integers N and M, representing, respectively, the number of rows and columns of the land (1 <= N, M <= 100). The second line will contain an integer K indicating the number of squares that have been turned into ponds ( (N x M) - K <= 50). Each of the next K lines contains two integers X and Y describing the position of a square which was turned into a pond (1 <= X <= N and 1 <= Y <= M). The end of input is indicated by N = M = 0. 

# Output

For each test case in the input your program should first output one line, containing an integer p representing the maximum number of properties which can be sold. The next p lines specify each pair of squares which can be sold simultaneity. If there are more than one solution, anyone is acceptable. there is a blank line after each test case. See sample below for clarification of the output format. 

# Sample Input

**4 461 11 42 24 14 24 44 344 23 22 23 10 0**

# Sample Output

**4(1,2)--(1,3)(2,1)--(3,1)(2,3)--(3,3)(2,4)--(3,4)**

**3(1,1)--(2,1)(1,2)--(1,3)(2,3)--(3,3)**


题意：给出一个 n*m 的地，有若干组询问，对于每组询问，给出 k 个水池的坐标，其余的为空地，现在要售卖土地，要求水池不能卖，土地只有两块空地组成的 1*2 或 2*1 型才能卖，问最多可卖出多少土地，并给出卖出的土地的坐标

思路：

首先将池塘舍去，然后将所有 i+j 为偶数的点当作 x，将所有 i+j 为奇数的点当作 y，然后直接拿 (x,y) 寻找增广路，通过上下左右进行查找匹配，再通过 link[x1][y1]=(x2,y2) 来记录匹配点

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
const int MOD=1E9+7;
const int N=1000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int n,m;
bool vis[N][N];
bool G[N][N];
struct Node{
    int x;
    int y;
}link[N][N];//存储偶数点匹配奇数点的坐标
bool dfs(int x,int y){
    for(int i=0;i<4;i++){//向上下左右四个方向搜索
        int nx=x+dx[i];
        int ny=y+dy[i];
        if(nx>=1&&nx<=n&&ny>=1&&ny<=m&&G[nx][ny]&&!vis[nx][ny]){//未访问的土地
            vis[nx][ny]=true;
            if(link[nx][ny].x==-1||dfs(link[nx][ny].x,link[nx][ny].y)){//匹配成功
                link[nx][ny].x=x;
                link[nx][ny].y=y;
                return true;//存在增广路
            }
        }
    }
    return false;//不存在增广路
}

int main(){

    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        memset(link,-1,sizeof(link));
        memset(G,true,sizeof(G));

        int k;
        scanf("%d",&k);
        while(k--){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x][y]=false;//水池
        }

        //对结点编号
        int res=0;
        for(int i=1;i<=n;i++){
            for(int j=1;j<=m;j++){
                if(G[i][j]){//土地
                    if((i+j)%2==0){//偶数点
                        memset(vis,false,sizeof(vis));
                        if(dfs(i,j))//寻找增广路
                            res++;
                    }
                }
            }
        }

        printf("%d\n",res);
        for(int i=1;i<=n;i++){
            for(int j=1;j<=m;j++){
                if(G[i][j]){
                    if((i+j)%2==1){//奇数点
                        if(link[i][j].x!=-1){
                            printf("(%d,%d)",link[i][j].x,link[i][j].y);
                            printf("--");
                            printf("(%d,%d)",i,j);
                            printf("\n");
                        }
                    }
                }
            }
        }
    }
    return 0;
}
```






